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

#ifndef DXF_TABLE_HPP
#define DXF_TABLE_HPP

#include <map>
#include <vector>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_table.hpp"
#include "dxf/Dxf_record_wrapper.hpp"

DXF_BEGIN_NAMESPACE

template <typename Entry_>
struct SGAL_SGAL_DECL Dxf_table : Dxf_base_table {
  typedef Entry_                        Entry;

  Dxf_record_wrapper<Entry> m_entry_wrapper;
  std::vector<Entry> m_entries;

  /*! Destruct.
   */
  ~Dxf_table() { clear(); }

  /*! Find an entry by name.
   */
  typename std::vector<Entry>::const_iterator find(const std::string& name)
  {
    return std::find_if(m_entries.begin(), m_entries.end(),
                        [&](const Entry& entry)
                        { return (entry.m_name == name); });
  }

  /*! Determine whether the table has entries.
   */
  bool empty() const;

  /*! Clear all entries.
   */
  void clear();
};

//! \brief determines whether the table has entries.
template <typename Entry>
inline bool Dxf_table<Entry>::empty() const { return m_entries.empty(); }

//! \brief clear all entries.
template <typename Entry>
inline void Dxf_table<Entry>::clear() { m_entries.clear(); }

DXF_END_NAMESPACE

#endif
