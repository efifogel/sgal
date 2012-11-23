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
// $Source: $
// $Revision: 5624 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *  Position_interpolator - implementation                              
 */

#include "SGAL/Position_interpolator.hpp"
#include "SGAL/Container_factory.hpp"

SGAL_BEGIN_NAMESPACE

std::string Position_interpolator::s_tag = "PositionInterpolator";

REGISTER_TO_FACTORY(Position_interpolator, "Position_interpolator");

/*! Constructor */
Position_interpolator::Position_interpolator(Boolean flag, Boolean proto) :
  Vector3f_interpolator(flag, proto)
{}

SGAL_END_NAMESPACE
