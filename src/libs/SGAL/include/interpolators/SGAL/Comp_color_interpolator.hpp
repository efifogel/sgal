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
// $Source$
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Implements color interpolator with asynchronic loading of compressed data
 */

#ifndef SGAL_COMP_COLOR_INTERPOLATOR_HPP
#define SGAL_COMP_COLOR_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "Comp_vector3f_interpolator.h"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Comp_color_interpolator :
  public Comp_vector3f_interpolator
{
public:
  /*! Constructor */
  Comp_color_interpolator(Boolean interpolate_flag = true,
                          Boolean proto = false);

  /*! Destructor */
  virtual ~Comp_color_interpolator() {}

  /* Construct the prototype */
  static Comp_color_interpolator* prototype()
  { return new Comp_color_interpolator(false, true); }
  
  /*! Clone */
  virtual Container* clone() { return new Comp_color_interpolator(); }

protected:
  virtual float get_key_bin() { return m_default_key_bin; }
  virtual float get_value_bin_factor() { return m_value_bin_factor; }
  
private:
  static const float m_default_key_bin;
  static const float m_value_bin_factor;
};

SGAL_END_NAMESPACE

#endif
