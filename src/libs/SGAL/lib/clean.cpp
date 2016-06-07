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
 */

#if defined(SGAL_USE_V8)
#include <libplatform/libplatform.h>
#include <v8.h>
#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

extern v8::Platform* s_platform;

SGAL_SGAL_DECL void clean()
{
#if defined(SGAL_USE_V8)
  // auto* platform = v8::V8::GetCurrentPlatform();
  v8::V8::ShutdownPlatform();
  if (s_platform) delete s_platform;
#endif
}

SGAL_END_NAMESPACE
