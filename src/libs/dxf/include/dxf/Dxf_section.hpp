// Copyright (c) 2018 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef DXF_SECTION_HPP
#define DXF_SECTION_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

/*! A generic section that consists of two items:
 *  (i) a list of items, where each item is a pair of code and string, and
 * (ii) a list of subsections, where each subsection is a pair of name and a
 *      a handle to (sub) section.
 */
struct SGAL_SGAL_DECL Dxf_section {
  std::list<std::pair<int, std::string> > m_items;
  std::list<std::pair<std::string, Dxf_section*> > m_sub_sections;
};

DXF_END_NAMESPACE

#endif
