// Copyright (c) 2016  Israel.
// All rights reserved.
//
// This file is part of SGAL; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the
// License. See the file LICENSE.LGPL distributed with SGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the
// software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Clean the Scene Graph Algorithm Library (SGAL).
 * The sole purpose of this file is to wrap a function that cleans the
 * V8 library.
 *
 * It seems that all possible mistakes have been done here (see below).
 * Some workarounds have been placed to deals with these mistakes. (They
 * should be removed when a better version is used.)
 *
 * 1. The library file name is libv8_libplatform.a. The second 'lib' is
 *    redundant. This is reflected in src/libs/SGAL/CMakeList.txt
 * 2. The cmake V8_LIBRARIES (and V8_LIBS) is empty. V8_LIBRARY is 'v8', so the
 *    v8_libplatform library has to be explicitly specified.
 * 3. In contrast, the include directly has entirely claimed the generic name
 *    'libplatform', without any hint that this is related to v8.
 * 4. Also, include file names also don't usually use the string 'lib'.
 * 5. Also, the single include file really doesn't need its own directory.
 * 6. libplatform.h contains `#include "include/v8-platform.h"`.
 *    This of course doesn't work with default include paths.
 *    The workaround requires the introduction of:
 *      src/libs/SGAL/include/include/v8-platform.h
 * 7. There is no GetCurrentPlatform(). (It seems that there used to be at some
 *    point.) It forced me to introduce the extern s_platform.
 * 8. v8::platform::CreateDefaultPlatform() returns null if a default one
 *    already exists. This forces a user to shut it down in the clean function
 *    below. This problem necessitates the provision of the clean() function
 *    in the first place.
 */

#if defined(SGAL_USE_V8)
#include <libplatform/libplatform.h>
#include <v8.h>
#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(SGAL_USE_V8)
extern v8::Platform* s_platform;
#endif

SGAL_SGAL_DECL void clean()
{
#if defined(SGAL_USE_V8)
  // auto* platform = v8::V8::GetCurrentPlatform();
  if (s_platform) {
    v8::V8::ShutdownPlatform();
    delete s_platform;
  }
#endif
}

SGAL_END_NAMESPACE
