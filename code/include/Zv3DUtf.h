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
#include "Zv3DEndianness.h"
#include "Zv3DError.h"

namespace Zv3D {

//-----------------------------------------------------------------------------
//! Utf-8 byte order mark bytes
enum Utf8Bom {
    kUtf8Bom0 = 0xef,
    kUtf8Bom1 = 0xbb,
    kUtf8Bom2 = 0xbf

};

//-----------------------------------------------------------------------------
//! Utf-16 byte order mark bytes
enum Utf16Bom {
    kUtf16Bom0 = 0xfe,
    kUtf16Bom1 = 0xff,
};

//-----------------------------------------------------------------------------
/** @brief Encodes unicode code point with Utf8 format and stores result in a buffer.
 *  @param dstBuf destination buffer.
 *  @param dstSize size of destination buffer in bytes.
 *  @param cp Unicode code point.
 *  @param dstSizeNeeded pointer to variable for storing destination buffer size enough
 * to place encoding result in.
 *  @return kERRC_NO, kERRC_NOT_ENOUGH_BUF_SPACE, kERRC_VALUE_OUT_OF_RANGE, kERRC_INVALID_PTR.
 * @note You can request the required buffer size to store the UTF-8 encoded Unicode position:
 * @code
    U32 nSize;
    if (kERRC_NOT_ENOUGH_BUF_SPACE == ToUtf8(0, 0, cp, &nSize)) {
        // nSize contains the required buffer size
    }
 * @endcode
 */
ErrorCode ToUtf8(void* dstBuf, U32 dstSize, U32 cp, U32* dstSizeNeeded);

//-----------------------------------------------------------------------------
/** @brief Decodes a Unicode code point from UTF-8 format and places the result in a variable.
 *  @param cp variable where the Unicode code point will be stored.
 *  @param srcBuf source buffer (with Unicode encoded positions).
 *  @param srcSize the size of the source buffer.
 *  @param srcReadBytes [out] (optional parameter) Pointer to a variable where the number of bytes read will be stored.
 *  @return kERRC_NO, kERRC_NOT_ENOUGH_DATA, kERRC_INVALID_PTR.
 */
ErrorCode FromUtf8(U32& cp, const void* srcBuf, U32 srcSize, U32* srcReadBytes);

//-----------------------------------------------------------------------------
//! Returns the length of a null-terminated UTF-8 string.
U32 utf8len(const U8* str);

//-----------------------------------------------------------------------------
/** @brief Encodes unicode code point with Utf16 format and stores result in a buffer.
 *  @param dstBuf destination buffer.
 *  @param dstSize size of destination buffer in bytes.
 *  @param cp Unicode code point.
 *  @param dstSizeNeeded pointer to variable for storing destination buffer size enough
 * to place encoding result in.
 *  @param endianType byte order to store result in.
 *  @return kERRC_NO, kERRC_NOT_ENOUGH_BUF_SPACE, kERRC_INVALID_PTR.
 * @note You can request the required buffer size to store the UTF-16 encoded Unicode position:
 * @code
    U32 nSize;
    if (kERRC_NOT_ENOUGH_BUF_SPACE == ToUtf16(0, 0, cp, &nSize)) {
        // nSize contains the required buffer size
    }
 * @endcode
 */
ErrorCode ToUtf16(void* dstBuf, U32 dstSize, U32 cp, U32* dstSizeNeeded, EndianType endianType);

//-----------------------------------------------------------------------------
/** @brief Decodes a Unicode code point from UTF-16 format and places the result in a variable.
 *  @param cp variable where the Unicode code point will be stored.
 *  @param srcBuf source buffer (with Unicode encoded positions).
 *  @param srcSize the size of the source buffer.
 *  @param srcReadBytes [out] (optional parameter) Pointer to a variable where the number of bytes
 *  read will be stored.
 *  @param endianType byte order source is stored in.
 *  @return kERRC_NO, kERRC_NOT_ENOUGH_DATA, kERRC_INVALID_PTR.
 */
ErrorCode FromUtf16(U32& cp, const void* srcBuf, U32 srcSize, U32* srcReadBytes, EndianType endianType);
} // end of Zv3D
