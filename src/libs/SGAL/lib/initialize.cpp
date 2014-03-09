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
// $Id: initialize.cpp 12384 2011-12-28 15:26:21Z efif $
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Initialize the Scene Graph Algorithm Library (SGAL).
 * The sole purpose of this file is to wrap a function that initializes the
 * ImageMagick library.
 */

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4244 )
#endif
#include <Magick++.h>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include <v8.h>

#include "SGAL/basic.hpp"
// #include "SGAL/Container_factory.hpp"

SGAL_BEGIN_NAMESPACE

void initialize(int /* argc */, char* argv[])
{
  Magick::InitializeMagick(*argv);
  v8::V8::InitializeICU();

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
