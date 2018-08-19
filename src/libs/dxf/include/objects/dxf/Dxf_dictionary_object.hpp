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

#ifndef DXF_DICTIONARY_OBJECT_HPP
#define DXF_DICTIONARY_OBJECT_HPP

#include <map>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_object.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_dictionary_object : Dxf_base_object {
  typedef Dxf_base_object                       Base;

  /// \name Data members
  //@{

  SGAL::String m_handle; // Handle
  int8_t m_is_hard_owner; // Hard-owner flag. If set to 1, indicates that
                        // elements of the dictionary are to be treated as
                        // hard-owned
  int8_t m_duplicate_record_handling; // Duplicate record cloning flag
                        // (determines how to merge duplicate entries):
                        // 0 = Not applicable
                        // 1 = Keep existing
                        // 2 = Use clone
                        // 3 = <xref>$0$<name>
                        // 4 = $0$<name>
                        // 5 = Unmangle name
  std::map<SGAL::String, SGAL::String> m_value_handles; // A mapping from
                        // soft-owner ID/handles to entry objects (one for each
                        // entry) (optional)

  //@}

  /// \name Data handlers
  //@{

  /*! Handle an entry name.
   */
  void handle_entry_name(const SGAL::String& value) { m_entry_name = value; }

  /*! Handle an entry value.
   */
  void handle_entry_value(const SGAL::String& value)
  { m_value_handles[m_entry_name] = value; }

  //! The current entry name.
  SGAL::String m_entry_name;

  //@}
};

DXF_END_NAMESPACE

#endif
