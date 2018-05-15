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

#ifndef SGAL_DXF_TABLE_HPP
#define SGAL_DXF_TABLE_HPP

#include <map>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_base_table.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Entry>
struct SGAL_SGAL_DECL Dxf_table : Dxf_base_table {

  // VPORT table-entry types
  typedef String Entry::*               String_entry;
  typedef bool Entry::*                 Bool_entry;
  typedef int8_t Entry::*               Int8_entry;
  typedef int16_t Entry::*              Int16_entry;
  typedef int32_t Entry::*              Int32_entry;
  typedef double Entry::*               Double_entry;
  typedef Uint Entry::*                 Uint_entry;

  typedef boost::variant<String_entry,
                         Bool_entry,
                         Int8_entry,
                         Int16_entry,
                         Int32_entry,
                         Double_entry,
                         Uint_entry>    Entry_variable_type;

  std::vector<Entry> m_entries;

  static const std::map<int, Entry_variable_type> s_entry_variables;
};

SGAL_END_NAMESPACE

#endif
