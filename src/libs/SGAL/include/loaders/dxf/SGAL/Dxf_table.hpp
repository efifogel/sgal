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

  // Table-entry types:
  typedef String Entry::*               String_entry;
  typedef bool Entry::*                 Bool_entry;
  typedef int8_t Entry::*               Int8_entry;
  typedef int16_t Entry::*              Int16_entry;
  typedef int32_t Entry::*              Int32_entry;
  typedef double Entry::*               Double_entry;
  typedef Uint Entry::*                 Uint_entry;
  typedef double (Entry::*Double_2d_entry)[2];
  typedef double (Entry::*Double_3d_entry)[3];

  typedef boost::variant<String_entry,
                         Bool_entry,
                         Int8_entry,
                         Int16_entry,
                         Int32_entry,
                         Double_entry,
                         Uint_entry,
                         Double_2d_entry,
                         Double_3d_entry>       Entry_member_type;

  std::vector<Entry> m_entries;

  //!
  struct Table_entry_member {
    Table_entry_member(Entry_member_type handle, int size, int index) :
      m_handle(handle),
      m_size(size),
      m_index(index)
    {}

    Entry_member_type m_handle;
    int m_size;
    int m_index;
  };

  static const std::map<int, Table_entry_member> s_entry_members;
};

SGAL_END_NAMESPACE

#endif
