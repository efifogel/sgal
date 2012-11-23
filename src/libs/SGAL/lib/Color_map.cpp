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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Color_map.hpp"
#include "SGAL/Context.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Color_map::Color_map(Context * context)
{
  if (!context) return;
  m_red_bits = context->get_red_bits();
  m_green_bits = context->get_green_bits();
  m_blue_bits = context->get_blue_bits();

  m_red_bits--;
  m_green_bits--;
  m_blue_bits--;

  int i = 0;
  unsigned int mask;

  mask = 0;
  for (i = 0 ; i < m_red_bits ; i++) {
    mask = mask << 1;
    mask |= 1;
  }
  m_red_mask = mask;

  mask = 0;
  for (i = 0 ; i < m_green_bits ; i++) {
    mask = mask << 1;
    mask |= 1;
  }
  m_green_mask = mask << m_red_bits;

  mask = 0;
  for (i = 0 ; i < m_blue_bits ; i++) {
    mask = mask << 1;
    mask |= 1;
  }
  m_blue_mask = mask << (m_red_bits + m_green_bits);

  m_red_shift = 32 - m_red_bits;
  m_green_shift = 32 - (m_red_bits + m_green_bits);
  m_blue_shift = 32 - (m_red_bits + m_green_bits + m_blue_bits);

#if 0
  int i = 0;
  int mask = 0;
  for (i = 0 ; i < m_red_bits ; i++) {
    mask = mask << 1;
    mask |= 1;
  }
  m_red_mask = mask << (m_green_bits + m_blue_bits);

  mask = 0;
  for (i = 0 ; i < m_green_bits ; i++) {
    mask = mask << 1;
    mask |= 1;
  }
  m_green_mask = mask << m_blue_bits;

  mask = 0;
  for (i = 0 ; i < m_blue_bits ; i++) {
    mask = mask << 1;
    mask |= 1;
  }
  m_blue_mask = mask;

  m_red_shift = 32 - (m_red_bits+m_green_bits+m_blue_bits);
  m_green_shift = 32 - (m_green_bits+m_blue_bits);
  m_blue_shift = 32 - m_blue_bits;
#endif
}

/*!
 */
unsigned int Color_map::get_index(Uint * rgb) const
{
  unsigned int r = (rgb[0] >> m_red_shift) & m_red_mask;
  unsigned int g = (rgb[1] >> m_green_shift) & m_green_mask;
  unsigned int b = (rgb[2] >> m_blue_shift) & m_blue_mask;
  unsigned int tmp = r | g | b;
  return tmp;
#if 0
  unsigned int r = rgb[0] >> m_red_shift;
  unsigned int g = rgb[1] >> m_green_shift;
  unsigned int b = rgb[2] >> m_blue_shift;
  unsigned int tmp = (r << (m_blue_bits+m_green_bits) | g << m_blue_bits | b);
  return tmp;
#endif
}

/*!
 */
void Color_map::get_color(unsigned int index, Uint * rgb) const 
{
  rgb[0] = ((index & m_red_mask) << m_red_shift) + (0x1 << (32-(m_red_bits+1)));
  rgb[1] = ((index & m_green_mask) << m_green_shift) + (0x1 << (32-(m_green_bits+1)));
  rgb[2] = ((index & m_blue_mask) << m_blue_shift) + (0x1 << (32-(m_blue_bits+1))); 
#if 0
  rgb[0] = ((index & m_red_mask) << m_red_shift) + (1 << (m_red_shift-2));
  rgb[1] = ((index & m_green_mask) << m_green_shift) + (1 << (m_green_shift-2));
  rgb[2] = ((index & m_blue_mask) << m_blue_shift) + (1 << (m_blue_shift-2)); 
#endif
}

SGAL_END_NAMESPACE
