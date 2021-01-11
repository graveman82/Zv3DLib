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

#ifndef ZV3D_DEBUG_H
#define ZV3D_DEBUG_H
/** @file Zv3DDebug.h
	Assertion macros for the purposes of debugging.
*/
//#include <stdio.h> // uncomment if you want to use printf()
#include "Zv3DStaticAllocSingleton.h"

namespace Zv3D {

// Default debug level setting.
#ifndef ZV3D_DEBUG_LEVEL
#   define ZV3D_DEBUG_LEVEL ZV3D_DEBUG_LEVEL_NONE
#endif

//-----------------------------------------------------------------------------
// Debug level values
#define ZV3D_DEBUG_LEVEL_NONE         0   ///< Debugging is off (for production builds).
#define ZV3D_DEBUG_LEVEL_LOW          1   ///< Low cost debugging mode (for release builds).
#define ZV3D_DEBUG_LEVEL_MEDIUM       2   ///< Developing mode when most of code is well tested.
#define ZV3D_DEBUG_LEVEL_HIGH         3   ///< Developing mode for intensive bug catching.
#define ZV3D_DEBUG_LEVEL_EXTRA        4   ///< To catch very serious bugs.
#define ZV3D_DEBUG_LEVEL_PARANOIC     5   ///< Paranoidal bug catching mode.

//-----------------------------------------------------------------------------
// Break point
#if ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_LOW

#   ifdef _MSC_VER

#       define ZV3D_DEBUG_BP() do { __asm int 3 } while (0)

#   elif defined(__GNUC__)

#       define ZV3D_DEBUG_BP() do {__asm__ ("int $3\n\t"); } while (0)

#   endif

#else

#   define ZV3D_DEBUG_BP(skip)

#endif // ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_LOW

//-----------------------------------------------------------------------------
// Auxiliary class to fix file and line where aseertion was failed (Don't use it directly!)
class AssertPoint : public StaticAllocatedSingleton<AssertPoint, sizeof(void*) * 4> {
public:
    friend class StaticAllocatedSingleton<AssertPoint, sizeof(void*) * 4>;

    void Set(const char* exp, const char* filename, int line) {
        exp_      = exp;
        filename_ = filename;
        line_     = line;
    }

    void Clear() { Set("", "", -1); }

    const char* exp() const         { return exp_; }
    const char* filename() const    { return filename_; }
    int         line() const        { return line_; }

private:
    AssertPoint() : exp_(""), filename_(""), line_(-1) {}

    const char* exp_;
    const char* filename_;
    int line_;
};

//-----------------------------------------------------------------------------
// Fixing assert failed point (Don't use it directly!)
#define ZV3D_FIX_ASSERT_POINT(exp) { if (!(exp)) { Zv3D::AssertPoint::Instance().Set(#exp, __FILE__, __LINE__); } }

//-----------------------------------------------------------------------------
// Shows assert related info (Don't use it directly!).
void OutputAssertMessage(const char* fmt, ...);

// Assert macros for different debugging levels [--
// Dubug level: LOW
#if ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_LOW // [LOW, PARANOIC]

#   define ZV3D_ASSERT_LOW(exp,fmt)                 { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_LOW1(exp,fmt,a1)             { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_LOW2(exp,fmt,a1,a2)          { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_LOW3(exp,fmt,a1,a2,a3)       { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_LOW4(exp,fmt,a1,a2,a3,a4)    { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3, a4); ZV3D_DEBUG_BP(); } }

#else

#   define ZV3D_ASSERT_LOW(exp,fmt)                 ((void) 0)
#   define ZV3D_ASSERT_LOW1(exp,fmt,a1)             ((void) 0)
#   define ZV3D_ASSERT_LOW2(exp,fmt,a1,a2)          ((void) 0)
#   define ZV3D_ASSERT_LOW3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZV3D_ASSERT_LOW4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_LOW

// Dubug level: MEDIUM
#if ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_MEDIUM // [MEDIUM - PARANOIC]

#   define ZV3D_ASSERT_MEDIUM(exp,fmt)                 { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_MEDIUM1(exp,fmt,a1)             { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_MEDIUM2(exp,fmt,a1,a2)          { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_MEDIUM3(exp,fmt,a1,a2,a3)       { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_MEDIUM4(exp,fmt,a1,a2,a3,a4)    { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3, a4); ZV3D_DEBUG_BP(); } }

#else

#   define ZV3D_ASSERT_MEDIUM(exp,fmt)                 ((void) 0)
#   define ZV3D_ASSERT_MEDIUM1(exp,fmt,a1)             ((void) 0)
#   define ZV3D_ASSERT_MEDIUM2(exp,fmt,a1,a2)          ((void) 0)
#   define ZV3D_ASSERT_MEDIUM3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZV3D_ASSERT_MEDIUM4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_MEDIUM

// Dubug level: HIGH
#if ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_HIGH // [HIGH - PARANOIC]

#   define ZV3D_ASSERT_HIGH(exp,fmt)                 { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_HIGH1(exp,fmt,a1)             { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_HIGH2(exp,fmt,a1,a2)          { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_HIGH3(exp,fmt,a1,a2,a3)       { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_HIGH4(exp,fmt,a1,a2,a3,a4)    { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3, a4); ZV3D_DEBUG_BP(); } }

#else

#   define ZV3D_ASSERT_HIGH(exp,fmt)                 ((void) 0)
#   define ZV3D_ASSERT_HIGH1(exp,fmt,a1)             ((void) 0)
#   define ZV3D_ASSERT_HIGH2(exp,fmt,a1,a2)          ((void) 0)
#   define ZV3D_ASSERT_HIGH3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZV3D_ASSERT_HIGH4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_HIGH

// Dubug level: EXTRA
#if ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_EXTRA // [EXTRA - PARANOIC]

#   define ZV3D_ASSERT_EXTRA(exp,fmt)                 { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_EXTRA1(exp,fmt,a1)             { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_EXTRA2(exp,fmt,a1,a2)          { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_EXTRA3(exp,fmt,a1,a2,a3)       { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_EXTRA4(exp,fmt,a1,a2,a3,a4)    { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3, a4); ZV3D_DEBUG_BP(); } }

#else

#   define ZV3D_ASSERT_EXTRA(exp,fmt)                 ((void) 0)
#   define ZV3D_ASSERT_EXTRA1(exp,fmt,a1)             ((void) 0)
#   define ZV3D_ASSERT_EXTRA2(exp,fmt,a1,a2)          ((void) 0)
#   define ZV3D_ASSERT_EXTRA3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZV3D_ASSERT_EXTRA4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_EXTRA

// Dubug level: PARANOIC
#if ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_PARANOIC // [PARANOIC]

#   define ZV3D_ASSERT_PARANOIC(exp,fmt)                 { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_PARANOIC1(exp,fmt,a1)             { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_PARANOIC2(exp,fmt,a1,a2)          { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_PARANOIC3(exp,fmt,a1,a2,a3)       { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3); ZV3D_DEBUG_BP(); } }
#   define ZV3D_ASSERT_PARANOIC4(exp,fmt,a1,a2,a3,a4)    { if (!(exp)) { ZV3D_FIX_ASSERT_POINT(exp); Zv3D::OutputAssertMessage(fmt, a1, a2, a3, a4); ZV3D_DEBUG_BP(); } }

#else

#   define ZV3D_ASSERT_PARANOIC(exp,fmt)                 ((void) 0)
#   define ZV3D_ASSERT_PARANOIC1(exp,fmt,a1)             ((void) 0)
#   define ZV3D_ASSERT_PARANOIC2(exp,fmt,a1,a2)          ((void) 0)
#   define ZV3D_ASSERT_PARANOIC3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZV3D_ASSERT_PARANOIC4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZV3D_DEBUG_LEVEL >= ZV3D_DEBUG_LEVEL_PARANOIC

// -- ] Assert macros for different debugging levels

} // end of Zv3D
#endif // ZV3D_DEBUG_H
