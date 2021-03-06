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
 *  Normal_interpolator - implementation
 */

#include "SGAL/Normal_interpolator.hpp"

SGAL_BEGIN_NAMESPACE

std::string Normal_interpolator::s_tag = "NormalInterpolator";

REGISTER_TO_FACTORY(Normal_interpolator, "Normal_interpolator");

//! \brief constructor.
Normal_interpolator::Normal_interpolator(Boolean interpolate_flag,
                                         Boolean proto) :
  Vector3f_interpolator(interpolate_flag, proto)
{}

SGAL_END_NAMESPACE
