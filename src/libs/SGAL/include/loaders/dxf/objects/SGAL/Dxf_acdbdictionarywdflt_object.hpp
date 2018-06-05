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

#ifndef SGAL_DXF_ACDBDICTIONARYWDFLT_OBJECT_HPP
#define SGAL_DXF_ACDBDICTIONARYWDFLT_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_acdbdictionarywdflt_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  int8_t m_duplicate_record_handling; // Duplicate record cloning flag
                        // (determines how to merge duplicate entries):
                        // 0 = Not applicable
                        // 1 = Keep existing
                        // 2 = Use clone
                        // 3 = <xref>$0$<name>
                        // 4 = $0$<name>
                        // 5 = Unmangle name
                        // 3 Entry name (one for each entry)
  String m_entry_name;  // Entry name (one for each entry)
  String m_entry_object; // Soft-owner ID/handle to entry object (one for each
                        // entry)
  String m_object_handle; // Hard pointer to default object ID/handle (currently
                        // only used for plot style dictionary's default entry,
                        // named "Normal")
};

SGAL_END_NAMESPACE

#endif
