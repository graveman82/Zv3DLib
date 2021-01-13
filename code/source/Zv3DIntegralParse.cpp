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


#include <math.h>

#include "Zv3DIntegralParse.h"
#include "Zv3DDebug.h"

namespace Zv3D {

typedef std::string::size_type CharIndex;
typedef std::string::size_type CharCount;

//-----------------------------------------------------------------------------
template <typename T>
T TenPower(S32 exp) {
    T b = 1;
    if (0 == exp) return T(1);
    else if (exp > 0) {
        while (exp--) b *= 10;
    }
    else {
        while (exp++) b *= T(0.1);
    }
    return b;
}


//-----------------------------------------------------------------------------
U32 SixteenPower(U32 exp) {
    U32 b = 1;
    if (0 == exp) return U32(1);
    while (exp--) b *= 16;
    return b;
}

//-----------------------------------------------------------------------------
static bool MoreThanOne(const std::string& s, char ch) {
    std::string::size_type i = s.find(ch);
    if (s.npos == i)
        return false;
    std::string::size_type li = s.rfind(ch);
    if (s.npos == li)
        return false;
    if (i == li)
        return false;
    return true;
}

//-----------------------------------------------------------------------------
static int CharToDigit(char ch) {
    switch(ch) {
        case '0' : return 0;
        case '1' : return 1;
        case '2' : return 2;
        case '3' : return 3;
        case '4' : return 4;
        case '5' : return 5;
        case '6' : return 6;
        case '7' : return 7;
        case '8' : return 8;
        case '9' : return 9;
        case 'a' : return 10;
        case 'b' : return 11;
        case 'c' : return 12;
        case 'd' : return 13;
        case 'e' : return 14;
        case 'f' : return 15;
        case 'A' : return 10;
        case 'B' : return 11;
        case 'C' : return 12;
        case 'D' : return 13;
        case 'E' : return 14;
        case 'F' : return 15;
        default : return 0;
    }
    return 0;
}

//-----------------------------------------------------------------------------
bool ParseNumber(const char* s, S32& i_result, double* d_result) {
    static std::string buf;
    buf = s;
    return ParseNumber(buf, i_result, d_result);
}

//-----------------------------------------------------------------------------
bool ParseNumber(const std::string& s, S32& i_result, double* d_result) {
    CharCount len = s.length();
    CharIndex unsigned_start = 0;
    S32 sign = 1;

    if (!len)
        return false;

    // test for invalid chars
    if (s.npos != s.find_first_not_of("0123456789.-+f"))
        return false;
    // at least one digit
    if (s.npos == s.find_first_of("0123456789"))
        return false;

    const char* specs = ".+-f";
    for (int i = 0; i < 4; ++i) {
        if (MoreThanOne(s, specs[i]))
            return false;
    }

    // leading '-' tests
    CharIndex minus_i = s.find('-');
    if (s.npos != minus_i) {
        if (s.front() != '-')
            return false;
        sign = -1;
        unsigned_start++;
        ZV3D_ASSERT_HIGH(unsigned_start < len, "at least one digit test must be passed above");
    }

    // leading '+' tests
    CharIndex plus_i = s.find('+');
    if (s.npos != plus_i) {
        if (s.front() != '+')
            return false;
        unsigned_start++;
        ZV3D_ASSERT_HIGH(unsigned_start < len, "at least one digit test must be passed above");
    }

    // terminating 'f' tests
    CharIndex f_i = s.find('f');
    if (s.npos != f_i) {
        if (s.back() != 'f')
            return false;
        --len;
        ZV3D_ASSERT_HIGH(unsigned_start < len, "at least one digit test must be passed above");
    }
    ZV3D_ASSERT_PARANOIC(unsigned_start < len, "at least one digit test must be passed above");

    // valid float, go on

    // find dot index
    CharIndex dot_i = s.find('.', unsigned_start);
    CharIndex int_start = unsigned_start;
    CharIndex int_end = len;
    CharIndex frac_start = len; // assume not frac
    CharIndex frac_end = len;
    if (s.npos != dot_i) {
        frac_start = dot_i + 1;
        int_end = dot_i;
    }

    // int part
    S32 int_part = 0;
    while (int_start < int_end && s[int_start] == '0') ++int_start;
    int digit_count = int_end - int_start;
    while (digit_count) {
        int_part += TenPower<S32>(digit_count-- - 1) * CharToDigit(s[int_start++]);
    }
    i_result = int_part * sign;

    if (!d_result)
        return true;
    // frac_part
    double frac_part = 0.0f;
    while (frac_start < frac_end && s[frac_end - 1] == '0') --frac_end;
    digit_count = frac_end - frac_start;
    while (digit_count) {
        frac_part += TenPower<double>(-digit_count--) * CharToDigit(s[frac_end-- - 1]);
    }

    *d_result = (int_part + frac_part) * sign;
    return true;
}

//-----------------------------------------------------------------------------
bool ParseHex(const char* s, U32& h_result) {
    static std::string buf;
    buf = s;
    return ParseHex(buf, h_result);
}

//-----------------------------------------------------------------------------
bool ParseHex(const std::string& s, U32& h_result) {
    CharCount len = s.length();
    CharIndex digits_start = 0;

    if (len < 3)
        return false;

    // leading '0' tests
    if (s[0] == '0') {
        digits_start++;
        if (!(s[1] == 'x' || s[1] == 'X'))
            return false;
        digits_start++;
        ZV3D_ASSERT_HIGH(digits_start < len, "at least one digit test must be passed above");
    }
    else {
        return false;
    }
    ZV3D_ASSERT_PARANOIC(digits_start < len, "at least one digit test must be passed above");

    // test for invalid chars
    if (s.npos != s.find_first_not_of("0123456789ABCDEFabcdef", digits_start))
        return false;
    ZV3D_ASSERT_PARANOIC(s.npos == s.find_first_of("0123456789ABCDEFabcdef", digits_start), "at least one digit test");
    CharIndex hex_start = digits_start;
    CharIndex hex_end = len;
    CharCount digit_count = hex_end - hex_start;
    if (0 == digit_count) return false;
    U32 hex_value = 0;
    S32 exp = 0;
    while (digit_count) {
        hex_value += SixteenPower(exp++) * CharToDigit(s[hex_end - 1]);
    }
    h_result = hex_value;
    return true;
}

//-----------------------------------------------------------------------------
bool ParseBoolean(const char* s, bool& b_result) {
    static std::string buf;
    buf = s;
    return ParseBoolean(buf, b_result);
}

//-----------------------------------------------------------------------------
bool ParseBoolean(const std::string& s, bool& b_result) {
    if (s.empty())
        return false;
    if (s == "true" || s == "1" || s == "yes") {
        b_result = true;
        return true;
    }
    else if (s == "false" || s == "0" || s == "no") {
        b_result = false;
        return true;
    }
    return false;
}
} // end of Zv3D
