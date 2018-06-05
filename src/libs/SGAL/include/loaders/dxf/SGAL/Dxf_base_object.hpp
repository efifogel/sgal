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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DXF_BASE_OBJECT_HPP
#define SGAL_DXF_BASE_OBJECT_HPP

#include <string>
#include <vector>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_extended_data.hpp"

SGAL_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_base_object {
  String m_handle;      // Handle
  String m_owner_object; // Soft pointer ID/handle to owner dictionary (optional)
  String m_owner_dict;    // Hard owner ID/handle to owner dictionary (optional)
  // String m_owner_handle;  // Soft-pointer ID/handle to owner BLOCK_RECORD object
  std::map<String, std::vector<String> > m_xdata;
  std::vector<Dxf_extended_data> m_extended_data;

  /*! Construct.
   */
  Dxf_base_object() : m_parser(nullptr) {}

  /*! Set the parser.
   * \param[in] parser the parser.
   */
  void set_parser(Dxf_parser* parser) { m_parser = parser; }

  /*! Initialize */
  void init() {}

  //! The parser.
  Dxf_parser* m_parser;
};

SGAL_END_NAMESPACE

#endif
