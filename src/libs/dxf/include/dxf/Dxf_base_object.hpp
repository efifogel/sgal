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

#ifndef DXF_BASE_OBJECT_HPP
#define DXF_BASE_OBJECT_HPP

#include <string>
#include <vector>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_extended_data.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_base_object {
  /// Record members
  //@{
  SGAL::String m_handle; // Handle
  SGAL::String m_owner_handle; // Soft pointer ID/handle to owner dictionary
                        // (optional)
  SGAL::String m_owner_dict; // Hard owner ID/handle to owner dictionary
                        // (optional)
  // SGAL::String m_owner_handle; // Soft-pointer ID/handle to owner
                        // BLOCK_RECORD object
  std::map<SGAL::String, std::vector<SGAL::String> > m_xdata;
  std::vector<Dxf_extended_data> m_extended_data;
  //@}

  /*! Construct.
   */
  Dxf_base_object() : m_parser(nullptr) {}

  /*! Destruct.
   * This is necessary to enable dynamic_cast.
   */
  virtual ~Dxf_base_object() {}

  /*! Set the parser.
   * \param[in] parser the parser.
   */
  void set_parser(Dxf_parser* parser) { m_parser = parser; }

  /*! Initialize */
  void init() {}

  //! The parser.
  Dxf_parser* m_parser;
};

DXF_END_NAMESPACE

#endif
