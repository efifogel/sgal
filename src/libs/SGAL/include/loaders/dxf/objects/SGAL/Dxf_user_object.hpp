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

#ifndef SGAL_DXF_USER_OBJECT_HPP
#define SGAL_DXF_USER_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_user_object : public Dxf_base_object {
  typedef Dxf_base_object             Base;

  bool handle_value(int code, const String& value);
  bool handle_value(int code, int8_t value);
  bool handle_value(int code, int16_t value);
  bool handle_value(int code, int32_t value);
  bool handle_value(int code, uint value);
  bool handle_value(int code, bool value);
  bool handle_value(int code, double value);
};

SGAL_END_NAMESPACE

#endif
