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
#ifndef ZV3D_INTTYPES_H
#define ZV3D_INTTYPES_H

/** @file Zv3DIntTypes.h
Definitions of integer types with exact sizes and constants of that types.
*/

#if defined(__GNUC__)
#include <stdint.h>
#endif

namespace Zv3D {

//-----------------------------------------------------------------------------
//! 8-bit signed integer
#if defined(_MSC_VER)
typedef __int8			S8;
#elif defined(__GNUC__)
typedef int8_t          S8;
#else
static_assert(sizeof(signed char) == 1, "signed char size greater than byte");
typedef signed char		S8;
#endif

//-----------------------------------------------------------------------------
//! 8-bit unsigned integer
#if defined(_MSC_VER)
typedef unsigned __int8	    U8;
#elif defined(__GNUC__)
typedef uint8_t             U8;
#else
static_assert(sizeof(unsigned char) == 1, "unsigned char size greater than byte");
typedef unsigned char		U8;
#endif

//-----------------------------------------------------------------------------
//! 16-bit signed integer
#if defined(_MSC_VER)
typedef __int16			    S16;
#elif defined(__GNUC__)
typedef int16_t             S16;
#else
static_assert(sizeof(signed short) == 2, "signed short size greater than 2 bytes");
typedef signed short		S16;
#endif

//-----------------------------------------------------------------------------
//! 16-bit unsigned integer
#if defined(_MSC_VER)
typedef unsigned __int16	U16;
#elif defined(__GNUC__)
typedef uint16_t            U16;
#else
static_assert(sizeof(unsigned short) == 2, "unsigned short size greater than 2 bytes");
typedef unsigned short		U16;
#endif


//-----------------------------------------------------------------------------
//! 32-bit signed integer
#if defined(_MSC_VER)
typedef __int32			    S32;
#elif defined(__GNUC__)
typedef int32_t             S32;
#else
static_assert(sizeof(signed long) == 4, "signed long size greater than 4 bytes");
typedef signed long		    S32;
#endif

//-----------------------------------------------------------------------------
//! 32-bit unsigned integer
#if defined(_MSC_VER)
typedef unsigned __int32	U32;
#elif defined(__GNUC__)
typedef uint32_t            U32;
#else
static_assert(sizeof(unsigned long) == 4, "unsigned long size greater than 4 bytes");
typedef unsigned long		U32;
#endif

const S8 kBAD_INDEX_S8 = -1;
const U8 kBAD_INDEX_U8 = 0xFF;
const S16 kBAD_INDEX_S16 = -1;
const U16 kBAD_INDEX_U16 = 0xFFFF;
const S32 kBAD_INDEX_S32 = -1;
const U32 kBAD_INDEX_U32 = 0xFFFFFFFF;

} // end of Zv3D
#endif // ZV3D_INTTYPES_H
