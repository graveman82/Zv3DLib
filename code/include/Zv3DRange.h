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
#ifndef ZV3D_RANGE_H_
#define ZV3D_RANGE_H_

namespace Zv3D {

//-----------------------------------------------------------------------------

template <typename T>
class Range {
public:
    enum BoundaryType { kBT_Exclude, kBT_Include };
    struct Boundary {
        Boundary(T value = 0, BoundaryType boundary_type = kBT_Include);

        T value;
        BoundaryType boundary_type;
    };

    Range(T low_val  = 0, BoundaryType low_type  = kBT_Include,
          T high_val = 0, BoundaryType high_type = kBT_Include);

    bool in(T point) const;

    Boundary low;
    Boundary high;
};

//-----------------------------------------------------------------------------
template <typename T>
inline Range<T>::Boundary::Boundary(T value, BoundaryType boundary_type) :
    value(value), boundary_type(boundary_type) {
}

//-----------------------------------------------------------------------------
template <typename T>
inline Range<T>::Range(T low_val, BoundaryType low_type,
                       T high_val, BoundaryType high_type) :
    low(low_val, low_type),
    high(high_val, high_type) {
}

//-----------------------------------------------------------------------------
template <typename T>
inline bool Range<T>::in(T point) const {
    if (kBT_Include == low.boundary_type &&
        kBT_Include == high.boundary_type) {
        return (point >= low.value && point <= high.value);
    }
    else if (kBT_Include == low.boundary_type &&
             kBT_Exclude == high.boundary_type) {
        return (point >= low.value && point < high.value);
    }
    else if (kBT_Exclude == low.boundary_type &&
             kBT_Include == high.boundary_type) {
        return (point > low.value && point <= high.value);
    }
    else if (kBT_Exclude == low.boundary_type &&
             kBT_Exclude == high.boundary_type) {
        return (point > low.value && point < high.value);
    }
    return false;
}

} // end of Zv3D
#endif // ZV3D_RANGE_H_

