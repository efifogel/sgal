// Copyright (c) 2004 Israel.
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

/*!
 *  Color_interpolator - implementation
 */

#include "SGAL/Container_factory.hpp"
#include "SGAL/Color_interpolator.hpp"

SGAL_BEGIN_NAMESPACE

std::string Color_interpolator::s_tag = "ColorInterpolator";

REGISTER_TO_FACTORY(Color_interpolator, "Color_interpolator");

/*! Constructor */
Color_interpolator::Color_interpolator(Boolean interpolate_flag, Boolean proto) :
  Vector3f_interpolator(interpolate_flag, proto)
{}

SGAL_END_NAMESPACE
