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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COLOR_MAP_HPP
#define SGAL_COLOR_MAP_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Context;

class Color_map {
public:
  /*! Initialize the color map. */
  void init(Context* context);

  /*! Convert the pixel color into an index. */
  Uint get_index(Uint pixel) const;

  /*! Convert the pixel color into an index. */
  Uint get_index(const Uchar* pixel) const;

  /*! Convert the index into a color. */
  void get_color(Uint index, Uchar* pixel) const;

private:
  Uint m_red_bits;
  Uint m_green_bits;
  Uint m_blue_bits;
  Uint m_alpha_bits;

  Uint m_red_mask;
  Uint m_green_mask;
  Uint m_blue_mask;
  Uint m_alpha_mask;

  Uint m_red_shift;
  Uint m_green_shift;
  Uint m_blue_shift;
  Uint m_alpha_shift;
};

SGAL_END_NAMESPACE

#endif
