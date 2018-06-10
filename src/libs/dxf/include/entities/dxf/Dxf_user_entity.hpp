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

#ifndef DXF_USER_ENTITY_HPP
#define DXF_USER_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_user_entity : public Dxf_base_entity {
  typedef Dxf_base_entity             Base;

  bool handle_value(int code, const SGAL::String& value);
  bool handle_value(int code, int8_t value);
  bool handle_value(int code, int16_t value);
  bool handle_value(int code, int32_t value);
  bool handle_value(int code, uint value);
  bool handle_value(int code, bool value);
  bool handle_value(int code, double value);
};

DXF_END_NAMESPACE

#endif
