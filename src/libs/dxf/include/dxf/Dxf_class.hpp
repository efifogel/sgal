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

#ifndef SGAL_DXF_CLASS_HPP
#define SGAL_DXF_CLASS_HPP

#include <cstdint>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_class {
  String m_record_name;      // Class DXF record name; always unique
  String m_class_name;       // C++ class name. Used to bind with software that
                             // defines object class behavior. Always unique
  String m_application_name; // Application name. Posted in Alert box when a
                             // class definition listed in this section is not
                             // currently loaded
  int m_version_number;
  int32_t m_proxy_capability_flags;
  int32_t m_instance_count;
  int8_t m_was_class_loaded_with_file; // Was-a-proxy flag. Set to 1 if class
                             // was not loaded when this DXF file was created,
                             // and 0 otherwise
  int8_t m_is_entity;        // Is-an-entity flag. Set to 1 if class was derived
                             // from the AcDbEntity class and can reside in the
                             // BLOCKS or ENTITIES section. If 0, instances may
                             // appear only in the OBJECTS section
};

SGAL_END_NAMESPACE

#endif
