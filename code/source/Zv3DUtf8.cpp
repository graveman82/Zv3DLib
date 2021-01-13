/*
-----------------------------------------------------------------------------
This source file is part of Zv3D (Zv3D game engine modules library).
For the latest info, see http://github.com/graveman82/zv3dlib

Copyright (C) 2012-2021 Marat Sungatullin.
This program is distributed under a dual-licensing scheme. You can use any
of them. I as an author will never ask you to open your source code even it under GPL.

(1) The zlib/libpng License (Zlib).
    This software is provided 'as-is', without any express or implied warranty. In
    no event will the authors be held liable for any damages arising from the use
    of this software.

    Permission is granted to anyone to use this software for any purpose, including
    commercial applications, and to alter it and redistribute it freely, subject to
    the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
    that you wrote the original software. If you use this software in a product,
    an acknowledgment in the product documentation would be appreciated but is not
    required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.

(2) GNU GENERAL PUBLIC LICENSE
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/
#include "Zv3DIntTypes.h"
#include "Zv3DError.h"
#include "Zv3DDebug.h"
#include "Zv3DEndianness.h"
#include "Zv3DPair.h"
#include "Zv3DRange.h"
#include "Zv3DSingleton.h"

#include <vector>

namespace Zv3D {

//-----------------------------------------------------------------------------
// Unicode point range and octets count needed to store values from it.
static const std::vector<Pair<Range<U32>, U32>> s_range_and_octets = {
    Pair<Range<U32>, U32>(Range<U32>(0,          Range<U32>::kBT_Include, 0x0000007F, Range<U32>::kBT_Include), 1),
    Pair<Range<U32>, U32>(Range<U32>(0x00000080, Range<U32>::kBT_Include, 0x000007FF, Range<U32>::kBT_Include), 2),
    Pair<Range<U32>, U32>(Range<U32>(0x00000800, Range<U32>::kBT_Include, 0x0000FFFF, Range<U32>::kBT_Include), 3),
    Pair<Range<U32>, U32>(Range<U32>(0x00010000, Range<U32>::kBT_Include, 0x0010FFFF, Range<U32>::kBT_Include), 4),
};

//-----------------------------------------------------------------------------
static U32 findOctetCount(U32 cp) {
    for (U32 i = 0; i < s_range_and_octets.size(); ++i) {
        if (s_range_and_octets[i].first.in(cp))
            return s_range_and_octets[i].second;
    }
    return 0;
}

//-----------------------------------------------------------------------------
ErrorCode ToUtf8(void* dstBuf, U32 dstSize, U32 cp, U32* dstSizeNeeded) {
    ErrorInfo& errorInfo = ErrorInfo::sErrorInfo;
    errorInfo.SetDefaults("Zv3D::ToUtf8()");

    U32 cOctets = findOctetCount(cp);
    if (0 == cOctets) { errorInfo.SetFnameLnCodeLevApi(ZV3D_FILE_AND_LINE, kERRC_VALUE_OUT_OF_RANGE);
        errorInfo.SetMessage("Unicode point(%x) passed is out of range", cp);
        return errorInfo.PushInStack();
    }

    if (dstSizeNeeded != 0) {
        *dstSizeNeeded = cOctets;
    }

    ZV3D_ASSERT_HIGH(dstSize > 0 || !dstBuf, "");
    if (0 == dstSize) {
        errorInfo.SetCode(kERRC_NOT_ENOUGH_BUF_SPACE);
        return errorInfo.code();
    }

    // check destination buffer size
    if (dstSize < cOctets) { errorInfo.SetFnameLnCodeLevApi(ZV3D_FILE_AND_LINE, kERRC_NOT_ENOUGH_BUF_SPACE);
        errorInfo.SetMessage("Not enough destination buffer size(%u bytes), %u bytes required",
                              dstSize, cOctets);
        return errorInfo.PushInStack();
    }

    ZV3D_ASSERT_HIGH(dstBuf, "");
    if (!dstBuf) { errorInfo.SetFnameLnCodeLevApi(ZV3D_FILE_AND_LINE, kERRC_INVALID_PTR, kERRL_FATAL);
        errorInfo.SetMessage("Its need to turn to debug build and to debug code properly");
        return errorInfo.PushInStack();
    }

    // fill the buffer
    U8* dstBufU8 = reinterpret_cast<U8*>(dstBuf);
    for (U32 i = cOctets - 1; i > 0; i--) {
        dstBufU8[i] = 0x80 | (0x3F & getByte(cp, 0)); // 6 bit mixes with a mask "0011 1111" and put in the octet.
        cp >>= 6;
    }
    dstBufU8[0] = 0;
    if (cOctets > 1) {
        dstBufU8[0] = U8(0x0F) << (8 - cOctets);
    }

    dstBufU8[0] |= getByte(cp, 0);

    return errorInfo.code();
}

//-----------------------------------------------------------------------------
ErrorCode FromUtf8(U32& cp, const void* srcBuf, U32 srcSize, U32* srcReadBytes) {
    ErrorInfo& errorInfo = ErrorInfo::sErrorInfo;
    errorInfo.SetDefaults("Zv3D::FromUtf8()");

    ZV3D_ASSERT_HIGH(srcBuf, "");
    if (!srcBuf) { errorInfo.SetFnameLnCodeLevApi(ZV3D_FILE_AND_LINE, kERRC_INVALID_PTR, kERRL_FATAL);
        errorInfo.SetMessage("Its need to turn to debug build and to debug code properly");
        return errorInfo.PushInStack();
    }

    if (0 == srcSize) { errorInfo.SetFnameLnCodeLevApi(ZV3D_FILE_AND_LINE, kERRC_NOT_ENOUGH_DATA);
        errorInfo.SetMessage("Not enough data in source buffer (source size: %u bytes)", srcSize);
        return errorInfo.PushInStack();
    }

    const U8* srcBufU8 = reinterpret_cast<const U8*>(srcBuf);

    // find octet num
    U32 cOctets = 1;
    U8 flag = 0x80; // 1000 0000
    U8 mask = 0x7F;
    if ((srcBufU8[0] & flag) != 0) { // cOctets > 1
        flag >>= 1; // 0100 0000
        while (srcBufU8[0] & flag) {
            cOctets++;
            mask >>= 1;
            flag >>= 1;
            if (cOctets == 4) break;
        }
    }

    if (cOctets > srcSize) { errorInfo.SetFnameLnCodeLevApi(ZV3D_FILE_AND_LINE, kERRC_NOT_ENOUGH_DATA);
        errorInfo.SetMessage("Not enough data in source buffer (source size: %u bytes), required %u bytes",
                             srcSize, cOctets);
        return errorInfo.PushInStack();
    }

    // fill cp
    setByte(cp, 0, srcBufU8[0] & mask);
    for (U32 i = 1; i < cOctets; i++) {
        cp <<= 6;
        setByte(cp, 0, getByte(cp, 0) | (srcBufU8[i] & 0x3F));
    }
    if (srcReadBytes)
        *srcReadBytes = cOctets;

    return errorInfo.code();
}

//-----------------------------------------------------------------------------
U32 Utf8Len(const U8* str) {
    if (!str) return 0;
    U32 len = 0;
    while (*str++) ++len;
    return len;
}
} // end of Zv3D
