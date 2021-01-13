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

#include "Zv3DUtf.h"
#include <stdio.h>
#include <conio.h>
#include "samples/utf.h"
#include "Zv3DError.h"

using Zv3D::U8;
using Zv3D::U32;

//-----------------------------------------------------------------------------
void SampleUtf8() {
    U8 encoded_text[16] = {};
    U8* p_encoded_text = &encoded_text[0];
    U32 encoded_text_size = 16;
    U32 write_count = 0;
    for (U32 i = 0; ; ++i) {
        if (0 == hieroglyphics[i])
            break;
        if (Zv3D::kERRC_NO == Zv3D::ToUtf8(p_encoded_text, encoded_text_size, hieroglyphics[i], &write_count)) {
            p_encoded_text += write_count;
            encoded_text_size -= write_count;
        }
        else {
            printf("Error: %s\n", Zv3D::ErrorInfo::errorInfoStack().top().message());
            return;
        }
    }

    // save to file
    FILE* fp = fopen("encoded_text.txt", "wb");
    if (!fp) {
        printf("SampleUtf8().Failed to save results in file \"%s\"\n", "encoded_text.txt");
        return;
    }
    fwrite(&encoded_text[0], 1, p_encoded_text - &encoded_text[0], fp);
    fclose(fp);
}

//-----------------------------------------------------------------------------
void SampleUtf8_2() {
    U8 decoded_text[20] = {};
    U8* p_decoded_text = &decoded_text[0];
    U32 decoded_text_size = 0;

    // load from file
    FILE* fp = fopen("encoded_text.txt", "rb");
    if (!fp) {
        printf("SampleUtf82().Failed to load data from file \"%s\"\n", "encoded_text.txt");
        return;
    }
    while (!feof(fp)) {
        fread(p_decoded_text++, 1, 1, fp);
        ++decoded_text_size;
        if (decoded_text_size == 16)
            break;
    }
    fclose(fp);

    // decode
    p_decoded_text = &decoded_text[0];
    U32 read_count = 0;
    for (U32 i = 0; ; ++i) {
        if (0 == hieroglyphics[i])
            break;
        U32 cp = 0;
        if (Zv3D::kERRC_NO == Zv3D::FromUtf8(cp, p_decoded_text, decoded_text_size, &read_count)) {
            if (cp == hieroglyphics[i])
                printf("read %s\n", hieroglyphic_descs[i]);
            else
                printf("read unknown code\n");
            p_decoded_text += read_count;
            if (decoded_text_size < read_count)
                break;
            decoded_text_size -= read_count;
            if (0 == decoded_text_size)
                break;
        }
        else {
            printf("Error: %s\n", Zv3D::ErrorInfo::errorInfoStack().top().message());
            return;
        }
    }
}
