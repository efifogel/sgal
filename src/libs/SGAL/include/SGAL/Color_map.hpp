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
  /*! Constructor */
  Color_map(Context * context);

  /*! Destructor */
  virtual ~Color_map() {}

  unsigned int get_index(Uint * rgb) const;
  void get_color(unsigned int index, Uint * rgb) const;

private:
  int m_red_bits;
  int m_green_bits;
  int m_blue_bits;

  Uint m_red_mask;
  Uint m_green_mask;
  Uint m_blue_mask;

  int m_red_shift;
  int m_green_shift;
  int m_blue_shift;

  Color_map() {}
  Color_map(const Color_map &) {}
};

SGAL_END_NAMESPACE

#endif
