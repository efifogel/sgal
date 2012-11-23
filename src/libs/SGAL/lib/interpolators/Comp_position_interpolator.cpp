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
 * Comp_positionInterpolator - implementation                              
 */

#include "SGAL/Comp_position_interpolator.hpp"

SGAL_BEGIN_NAMESPACE

std::string Comp_position_interpolator::s_tag =
"sgalCompressedPositionInterpolator";

// Default values:
const float Comp_position_interpolator::m_default_key_bin = (float)1e-4;
const float Comp_position_interpolator::m_value_bin_factor = (float)2e-4;      

REGISTER_TO_FACTORY(Comp_positionInterpolator, "Comp_position_interpolator");

/*! Constructor */
Comp_position_interpolator::Comp_position_interpolator(Boolean flag, Boolean proto) :
  Comp_vector3f_interpolator(interpolate_flag, proto)
{
}

/*
float Comp_positionInterpolator::get_value_bin(float *vals)
{ 
  float radiusBin;
  if (m_pExecutionCoordinator)
  {
    radiusBin = m_pExecutionCoordinator->get_sceneBoundingSphereRadius();
    if (radiusBin == 0.0)
    {
      radiusBin = (float)1.0;
    }
    radiusBin = radiusBin * get_value_bin_factor();
  }
  float rangeBin = get_valueRange(vals, m_no_keys) * get_value_bin_factor();
  if (rangeBin < radiusBin)
    return rangeBin;
  else
    return radiusBin; 
}
*/

SGAL_END_NAMESPACE
