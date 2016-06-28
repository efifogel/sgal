// Copyright (c) 2004  Israel.
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
 * Initialize the Scene Graph Algorithm Library (SGAL).
 * The sole purpose of this file is to wrap a function that initializes the
 * ImageMagick and V8 libraries.
 */

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4244 )
#endif
#include <Magick++.h>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#if defined(SGAL_USE_V8)
#include <libplatform/libplatform.h>
#include <v8.h>
#endif

#include "SGAL/basic.hpp"
// #include "SGAL/Container_factory.hpp"

SGAL_BEGIN_NAMESPACE

v8::Platform* s_platform(nullptr);

SGAL_SGAL_DECL void initialize(int /* argc */, char* argv[])
{
  Magick::InitializeMagick(*argv);
#if defined(SGAL_USE_V8)
  v8::V8::InitializeICU();
  v8::V8::InitializeExternalStartupData(argv[0]);
  s_platform = v8::platform::CreateDefaultPlatform();
  SGAL_assertion(s_platform);
  v8::V8::InitializePlatform(s_platform);
  v8::V8::Initialize();
#endif

  /*! Container factory initialization.
   * During initialization of the container factory an instance of every
   * container is constructed and registered. This is done automatically
   * through the REGISTER_TO_FACTORY() macro (provided in
   * Container_factory.h). An alternative way is to call the initialize()
   * method of the factory explicitly, in case allocation is desired to be
   * concentrated elsewhere.
   *
   * Container_factory* factory = Container_factory::get_instance();
   * factory->initialize();
   */
}

SGAL_END_NAMESPACE
