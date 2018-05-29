// Copyright (c) 2004,2018 Israel.
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

#ifndef SGAL_DXF_XRECORD_OBJECT_HPP
#define SGAL_DXF_XRECORD_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_xrecord_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  int8_t m_flags;       // Duplicate record cloning flag (determines how to
                        // merge duplicate entries):
                        // 0 = Not applicable
                        // 1 = Keep existing
                        // 2 = Use clone
                        // 3 = <xref>$0$<name>
                        // 4 = $0$<name>
                        // 5 = Unmangle name

  // Application specific; These values can be used by an application in any way

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(Dxf_parser& parser, int code, const String& value);

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(Dxf_parser& parser, int code, int8_t value);

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(Dxf_parser& parser, int code, int16_t value);

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(Dxf_parser& parser, int code, int32_t value);

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(Dxf_parser& parser, int code, double value);
};

SGAL_END_NAMESPACE

#endif
