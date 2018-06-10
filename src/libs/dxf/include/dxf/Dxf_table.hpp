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
};

DXF_END_NAMESPACE

#endif
