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
#ifndef ZV3D_ENDIANNESS_H
#define ZV3D_ENDIANNESS_H

#include "Zv3DIntTypes.h"

/** @page Endiannes.
 *
 * This functionality is necessary in case of a mismatch in the byte order of the
 * data and the machine.
 * On a little-endian machine, the variable "x" in the source text
 * @code
 * U32 x = 0xb3fa00c7;
 * @endcode
 * in memory will look like "0xc7 0x00 0xfa 0xb3". That is, the program text matches
 * the big-endian byte order, and the memory byte order is machine dependent.
 *
 * The test programs are located in the "test_endianness.cpp" file.
 *
 * The byte order in readBuiltin () and writeBuiltin () refers to the data in the buffer.
 *
 * Parameters of read and write functions: buffer data (pointer, size), variable, byte order.
*/

namespace Zv3D {

//-----------------------------------------------------------------------------
/** @brief To set the order of the data bytes. */
enum EndianType {
    kIgnoreEndian,
    kLittleEndian,
    kBigEndian
};

//-----------------------------------------------------------------------------
//! Returns true if the machine's byte order is little endian.
inline bool isLittleEndian() {
    U32 x = U32(1);
    return (*reinterpret_cast<U8*>(&x));
}

//-----------------------------------------------------------------------------
/** @brief Returns platform endianness.
 */
EndianType platformEndian();

//-----------------------------------------------------------------------------
/** @brief Reads the built-in type from the buffer.
 *
 * The pointer to the buffer is shifted by the number of bytes read, and its size is reduced by this number.
 * The function allows you to read data given in both little-endian order and big-endian order
 * regardless of the byte order of the machine.
 * For a more complete understanding, analyze the TestEndiannes2 () example.
 *
 * @code
    // The first 4 bytes contain the number 1325435548 in little-endian.
    // From the 4th to 7th bytes, the number 991406340 is written in big-endian order.
    U8 buf[8] = {0x9c, 0x8a, 0x00, 0x4f, 0x3b, 0x17, 0xa9, 0x04};
    const U8* p = buf;
    U32 size = 8;
    EndianType et = kLittleEndian;
    while (size) {
        U32 x;
        if (readBuiltin(p, size, x, et))
            printf("x: %d\n", x);
        et = kBigEndian; // switch the byte order for reading the 2nd number
    }
 * @endcode
 * @param pSrc [in,out] source buffer.
 * @param srcSize [in,out] source buffer size in bytes.
 * @param dst [out] variable reference to store the read result.
 * @param endianType the type of data encoding in the source buffer.
 * @return true if the operation was successful, false on error. The error is related to
 * insufficient size of the source buffer.
 * @note Only for built-in types (integers, floats, char, wchar_t etc).
 */
template <typename T>
inline bool readBuiltin(const U8*& pSrc, U32& srcSize, T& dst, EndianType endianType) {
    if (srcSize < sizeof(T)) return false;

    U8* pDst = reinterpret_cast<U8*>(&dst);
    const U8* pSrcEnd = pSrc + sizeof(T);

    if (platformEndian() != endianType) {
        while (pSrc != pSrcEnd--) {
            *pDst++ = *pSrcEnd;
        }
        pSrc += sizeof(T);
    }
    else {
        while (pSrc != pSrcEnd) {
            *pDst++ = *pSrc++;
        }
    }

    srcSize -= sizeof(T);
    return true;
}

//-----------------------------------------------------------------------------
/** @brief Write a built-in type to a buffer using the specified byte order.
  *
  * The pointer to the buffer is shifted by the number of bytes written, and its size is reduced by this number.
  *
  * @param pDst [in, out] destination buffer.
  * @param dstSize [in, out] size of the destination buffer (in bytes).
  * @param src value to write.
  * @param endianType the specified byte order of the data in the buffer.
  * @return true if the operation was successful, false on error. The error is related to
  * insufficient size of the destination buffer.
  * @note For built-in types only (integers, floats, char, wchar_t etc).
  */
template <typename T>
inline bool writeBuiltin(U8*& pDst, U32& dstSize, T src, EndianType endianType) {
    if (dstSize < sizeof(T)) return false;

    U8* pSrc = reinterpret_cast<U8*>(&src);
    U8* pDstEnd = pDst + sizeof(T);
    if (platformEndian() != endianType) {
        while (pDst != pDstEnd--) {
            *pDstEnd = *pSrc++;
        }
        pDst += sizeof(T);
    }
    else {
        while (pDst != pDstEnd) {
            *pDst++ = *pSrc++;
        }
    }
    dstSize -= sizeof(T);
    return true;
}

//-----------------------------------------------------------------------------
/** @brief Retrieves the byte at the specified index.
 * @note This function assumes that bytes are numbered in memory on the LE machine
 * in ascending order of addresses.
 * On the BE machine, descending addresses.
 */
template <typename T>
inline U8 getByte(T src, U8 byteIndex) {
    if (kLittleEndian == platformEndian()) {
        return (reinterpret_cast<U8*>(&src))[byteIndex];
    }
    else {
        return (reinterpret_cast<U8*>(&src))[sizeof(T) - 1 - byteIndex];
    }
}

//-----------------------------------------------------------------------------
/** @brief Sets the byte at the given index.
 * @note In this function, it is assumed that the numbering of bytes in memory on the LE-machine
 * is performed in ascending order of addresses.
 * On the BE machine, descending addresses.
 */
template <typename T>
inline void setByte(T& dst, U8 byteIndex, U8 v) {
    if (kLittleEndian == platformEndian()) {
        (reinterpret_cast<U8*>(&dst))[byteIndex] = v;
    }
    else {
        (reinterpret_cast<U8*>(&dst))[sizeof(T) - 1 - byteIndex] = v;
    }
}

} // end of Zv3D
#endif // ZV3D_ENDIANNESS_H
