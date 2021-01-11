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
#ifndef ZV3D_ERROR_H
#define ZV3D_ERROR_H

#include <stack>

#include "Zv3DErrorCodes.h"

//-----------------------------------------------------------------------------
//! Macro to insert filename and line with error.
#define ZV3D_FILE_AND_LINE __FILE__, __LINE__

namespace Zv3D {

//-----------------------------------------------------------------------------
//! Error level list.
enum ErrorLevel {
    kERRL_WARN,     ///< no critical error
    kERRL_ERROR,    ///< an attempt could be made to handle error
    kERRL_FATAL     ///< application must be terminated
};

//-----------------------------------------------------------------------------
enum ErrorApi {
    kERRAPI_Zv3D,
    kERRAPI_WINDOWS,
    kERRAPI_DIRECTX9,
    kERRAPI_DIRECTX11,
    kERRAPI_OPENGL,
    kERRAPI_SDL,
    kERRAPI_NO_IN_LIST_FOR_NOW
};

//-----------------------------------------------------------------------------
/** @class ErrorInfo.

*/
class ErrorInfo {
public:
    enum BufferSize { kFNAME_BUFSIZE = 260, kSRC_BUFSIZE = 256, kMSG_BUFSIZE = 512 };

    ErrorInfo();
    void SetApi(ErrorApi api);
    void SetLevel(ErrorLevel level);
    void SetCode(ErrorCode code);

    /** @brief Puts informative description related to error. */
    void SetMessage(const char* fmt, ...);

    /** @brief Sets source of error (module, function etc).
     * The source of the error is chosen quite arbitrarily - you can specify the
     * name of the engine system, module, class name, function name, block name.
     * The basic rule is that the content of the source should help to determine
     * the location of the error and its cause.
    */
    void SetSource(const char* src);

    /** @brief Sets source file name and line number where error occurred. */
    void SetFilenameAndLine(const char* filename, int line);

     ErrorLevel level()    const { return level_; }
      ErrorCode code()     const { return code_; }
       ErrorApi api()      const { return api_; }
            int line()     const { return line_; }
    const char* filename() const { return filename_; }
    const char* message()  const { return message_; }
    const char* source()   const { return source_; }

    static std::stack<ErrorInfo>& errorInfoStack() { return sErrorInfoStack_; }
    static ErrorInfo sErrorInfo;

private:
    ErrorLevel level_;
    ErrorCode code_;
    ErrorApi api_;
    int line_;                      // related source code line
    char filename_[kFNAME_BUFSIZE]; // related source code file name
    char message_[kMSG_BUFSIZE];    // description of error
    char source_[kSRC_BUFSIZE];
    static std::stack<ErrorInfo> sErrorInfoStack_;
     // to avoid occupy stack space of function where it used
};

} // end of Zv3D
#endif // ZV3D_ERROR_H
