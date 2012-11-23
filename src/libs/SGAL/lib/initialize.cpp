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

#ifndef SGAL_INITIALIZE_HPP
#define SGAL_INITIALIZE_HPP

#include <Magick++.h>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

void initialize(int /* argc */, char* argv[])
{
  Magick::InitializeMagick(*argv);
}

SGAL_END_NAMESPACE

#endif

