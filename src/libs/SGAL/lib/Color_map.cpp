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
// $Id: $
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Color_map.hpp"
#include "SGAL/Context.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief initializes the color map. */
void Color_map::init(Context* context)
{
  if (!context) return;
  m_red_bits = context->get_red_bits();
  m_green_bits = context->get_green_bits();
  m_blue_bits = context->get_blue_bits();
  m_alpha_bits = context->get_alpha_bits();
  
  Uint i = 0;
  Uint mask;

  m_red_shift = 0;
  mask = 0;
  for (i = 0; i < m_red_bits; ++i) {
    mask = mask << 1;
    mask |= 1;
  }
  m_red_mask = mask << m_red_shift;

  m_green_shift = m_red_shift + m_red_bits;
  mask = 0;
  for (i = 0; i < m_green_bits; ++i) {
    mask = mask << 1;
    mask |= 1;
  }
  m_green_mask = mask << m_green_shift;

  m_blue_shift =  m_green_shift + m_green_bits;
  mask = 0;
  for (i = 0; i < m_blue_bits; ++i) {
    mask = mask << 1;
    mask |= 1;
  }
  m_blue_mask = mask << m_blue_shift;

  m_alpha_shift =  m_blue_shift + m_blue_bits;
  mask = 0;
  for (i = 0; i < m_alpha_bits; ++i) {
    mask = mask << 1;
    mask |= 1;
  }
  m_alpha_mask = mask << m_alpha_shift;
}

/*! \brief converts the pixel color into an index. */
Uint Color_map::get_index(const Uchar* color, Uint size) const
{
  Uint r = (color[0] << m_red_shift) & m_red_mask;
  Uint g = (color[1] << m_green_shift) & m_green_mask;
  Uint b = (color[2] << m_blue_shift) & m_blue_mask;
  if (size == 3) return (r | g | b);
  Uint a = (color[3] << m_alpha_shift) & m_alpha_mask;
  return (r | g | b | a);
}

/*! \brief converts the index into a pixel color. */
void Color_map::get_color(Uint index, Uchar* color, Uint size) const 
{
  color[0] = ((index & m_red_mask) >> m_red_shift);
  color[1] = ((index & m_green_mask) >> m_green_shift);
  color[2] = ((index & m_blue_mask) >> m_blue_shift);
  if (size == 3) return;
  color[3] = ((index & m_alpha_mask) >> m_alpha_shift);
}

SGAL_END_NAMESPACE
