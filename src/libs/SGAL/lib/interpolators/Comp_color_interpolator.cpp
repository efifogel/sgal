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
// $Revision: 4971 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *  Comp_color_interpolator - implementation                              
 */

#include "SGAL/Comp_color_interpolator.hpp"

// Default values:
const float Comp_color_interpolator::m_default_key_bin = (float)1e-4;
const float Comp_color_interpolator::m_value_bin_factor = (float)5e-3;      

REGISTER_TO_FACTORY(Comp_color_interpolator, "Comp_color_interpolator");

/*! Constructor */
Comp_color_interpolator::Comp_color_interpolator(Boolean interpolate_flag) :
  Comp_vector3f_interpolator(interpolate_flag)
{
  m_tag = "sgalCompressedColorInterpolator";
}
