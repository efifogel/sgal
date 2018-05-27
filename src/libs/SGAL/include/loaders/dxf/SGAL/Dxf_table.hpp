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
#include "SGAL/Dxf_extended_data.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Entry>
struct SGAL_SGAL_DECL Dxf_table : Dxf_base_table {

  // Table-entry types:
  typedef String Entry::*               String_record;
  typedef bool Entry::*                 Bool_record;
  typedef int8_t Entry::*               Int8_record;
  typedef int16_t Entry::*              Int16_record;
  typedef int32_t Entry::*              Int32_record;
  typedef double Entry::*               Double_record;
  typedef Uint Entry::*                 Uint_record;
  typedef double (Entry::*Double_2d_record)[2];
  typedef double (Entry::*Double_3d_record)[3];

  typedef boost::variant<String_record,
                         Bool_record,
                         Int8_record,
                         Int16_record,
                         Int32_record,
                         Double_record,
                         Uint_record,
                         Double_2d_record,
                         Double_3d_record>       Entry_member_type;

  std::vector<Entry> m_entries;

  /*! Has_xdata is a generic-template-struct service that evaluates as follows:
   * If A has a data member called x_data, then Has_xdata<A>::value == true,
   * else (A doesn't have a data member called x_data)
   * Has_xdata<B>::value == false.
   */
  template <typename T, typename = int>
  struct Has_extended_data : std::false_type {};

  //!
  template <typename T>
  struct Has_extended_data<T, decltype((void) T::m_extended_data, 0)> :
    std::true_type {};

  //!
  template <bool what>
  Dxf_extended_data* get_extended_data_impl(const String& name,
                                            char (*)[what] = 0)
  {
    auto it = std::find_if(m_entries.begin(), m_entries.end(),
                           [&](Entry& entry)
                           { return (entry.m_name == name); });
    if (it == m_entries.end()) return nullptr;

    it->m_extended_data.resize(it->m_extended_data.size() + 1);
    return &(it->m_extended_data.back());
  }

  //!
  template <bool what>
  Dxf_extended_data* get_extended_data_impl(const String& name,
                                            char (*)[!what] = 0)
  { return nullptr; }

  //!
  Dxf_extended_data* get_extended_data(const String& name)
  { return get_extended_data_impl<Has_extended_data<Entry>::value>(name); }

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
