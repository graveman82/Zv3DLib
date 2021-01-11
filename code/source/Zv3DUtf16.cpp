/*
-----------------------------------------------------------------------------
This source file is part of Zv3D (Zv3D game engine modules library).
For the latest info, see http://github.com/graveman82/creativemind

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

namespace Zv3D {

//-----------------------------------------------------------------------------
/*Debugging cheat sheet.
Each surrogate is allocated 10 bits for storing meaningful data, which gives 2 ^ 20 = 1,048,576 characters.
Since surrogate pairs store code positions greater than 0x10000, we can subtract this number.
We put the older 10 bits in the higher surrogate, the lower 10 bits in the lower surrogate. It is not
difficult to guess that 0x3FF is mask for extracting the lower 10 bits of a number.
*/
static bool Utf16_makeSurrogates(U16& s1, U16& s2, U32 cp) {
    if (cp >= 0x10000) {
        U32 cp2 = cp - 0x10000;
        s1 = U32(0xD800) + (cp2 >> 10);
        s2 = U32(0xDC00) + (cp2 & 0x3FF);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
ErrorCode ToUtf16(void* dstBuf, U32 dstSize, U32 cp, U32* dstSizeNeeded, EndianType endianType) {
    ErrorInfo& errorInfo = ErrorInfo::sErrorInfo;
    errorInfo.SetSource("Zv3D::ToUtf16()");
    errorInfo.SetApi(kERRAPI_Zv3D);
    errorInfo.SetLevel(kERRL_ERROR);
    errorInfo.SetCode(kERRC_NO);

    U8* dstBufU8 = reinterpret_cast<U8*>(dstBuf);

    if (cp >= 0x10000) { // need surrogate pairs
        if (dstSizeNeeded)
            *dstSizeNeeded = 4;

        ZV3D_ASSERT_HIGH(dstSize > 0 || !dstBuf, "");
        if (0 == dstSize) {
            errorInfo.SetCode(kERRC_NOT_ENOUGH_BUF_SPACE);
            return errorInfo.code();
        }

        U16 s[2];
        Utf16_makeSurrogates(s[0], s[1], cp);

        ZV3D_ASSERT_HIGH(dstBuf, "");
        if (!dstBuf) {
            errorInfo.SetLevel(kERRL_FATAL);
            errorInfo.SetCode(kERRC_INVALID_PTR);
            errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
            errorInfo.SetMessage("Its need to turn to debug build and to debug code properly");
            errorInfo.errorInfoStack().push(errorInfo);
            return errorInfo.code();
        }

        if (!writeBuiltin(dstBufU8, dstSize, s[0], endianType)) {
            errorInfo.SetCode(kERRC_NOT_ENOUGH_BUF_SPACE);
            errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
            errorInfo.SetMessage("Not enough destination buffer size(%u bytes), %u bytes required",
                                  dstSize, dstSizeNeeded);
            errorInfo.errorInfoStack().push(errorInfo);
            return errorInfo.code();
        }
        if (!writeBuiltin(dstBufU8, dstSize, s[1], endianType)) {
            errorInfo.SetCode(kERRC_NOT_ENOUGH_BUF_SPACE);
            errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
            errorInfo.SetMessage("Not enough destination buffer size(%u bytes), %u bytes required",
                                  dstSize, dstSizeNeeded);
            errorInfo.errorInfoStack().push(errorInfo);
        }
    }
    else {
        if (dstSizeNeeded)
            *dstSizeNeeded = 2;

        ZV3D_ASSERT_HIGH(dstSize > 0 || !dstBuf, "");
        if (0 == dstSize) {
            errorInfo.SetCode(kERRC_NOT_ENOUGH_BUF_SPACE);
            return errorInfo.code();
        }

        ZV3D_ASSERT_HIGH(dstBuf, "");
        if (!dstBuf) {
            errorInfo.SetLevel(kERRL_FATAL);
            errorInfo.SetCode(kERRC_INVALID_PTR);
            errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
            errorInfo.SetMessage("Its need to turn to debug build and to debug code properly");
            errorInfo.errorInfoStack().push(errorInfo);
            return errorInfo.code();
        }

        if (!writeBuiltin(dstBufU8, dstSize, U16(cp), endianType)) {
            errorInfo.SetCode(kERRC_NOT_ENOUGH_BUF_SPACE);
            errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
            errorInfo.SetMessage("Not enough destination buffer size(%u bytes), %u bytes required",
                                  dstSize, dstSizeNeeded);
            errorInfo.errorInfoStack().push(errorInfo);
        }
    }

    return errorInfo.code();
}

//-----------------------------------------------------------------------------
ErrorCode FromUtf16(U32& cp, const void* srcBuf, U32 srcSize, U32* srcReadBytes, EndianType endianType) {
    ErrorInfo& errorInfo = ErrorInfo::sErrorInfo;
    errorInfo.SetSource("Zv3D::FromUtf16()");
    errorInfo.SetApi(kERRAPI_Zv3D);
    errorInfo.SetLevel(kERRL_ERROR);
    errorInfo.SetCode(kERRC_NO);

    ZV3D_ASSERT_HIGH(srcBuf, "");
    if (!srcBuf) {
        errorInfo.SetLevel(kERRL_FATAL);
        errorInfo.SetCode(kERRC_INVALID_PTR);
        errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
        errorInfo.SetMessage("Its need to turn to debug build and to debug code properly");
        errorInfo.errorInfoStack().push(errorInfo);
        return errorInfo.code();
    }

    const U8* srcBufU8 = reinterpret_cast<const U8*>(srcBuf);
    const U8* const srcBufU80 = srcBufU8;

    U16 s1;
    if (!readBuiltin(srcBufU8, srcSize, s1, endianType)) {
        errorInfo.SetCode(kERRC_NOT_ENOUGH_DATA);
        errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
        errorInfo.SetMessage("Not enough data in source buffer (source size: %u bytes)", srcSize);
        errorInfo.errorInfoStack().push(errorInfo);
        return errorInfo.code();
    }
    if ((s1 & 0xFC00) == 0xD800) {
        U16 s2;
        if (!readBuiltin(srcBufU8, srcSize, s2, endianType)) {
            errorInfo.SetCode(kERRC_NOT_ENOUGH_DATA);
            errorInfo.SetFilenameAndLine(ZV3D_FILE_AND_LINE);
            errorInfo.SetMessage("Not enough data in source buffer (source size: %u bytes)", srcSize);
            errorInfo.errorInfoStack().push(errorInfo);
            return errorInfo.code();
        }
        cp = 0x10000 + ((U32(s1 - 0xD800)) << 10) + U32(s2 - 0xDC00);
    }
    else {
        cp = U32(s1);
    }

    if (srcReadBytes)
        *srcReadBytes = srcBufU8 - srcBufU80;
    return errorInfo.code();
}
} // end of Zv3D
