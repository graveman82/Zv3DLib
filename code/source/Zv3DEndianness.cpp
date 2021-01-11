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

#include "Zv3DEndianness.h"

namespace Zv3D {

EndianType platformEndian() {
    static EndianType s_platform_endian = isLittleEndian() ? kLittleEndian : kBigEndian;
    return s_platform_endian;
}

} // end of Zv3D
