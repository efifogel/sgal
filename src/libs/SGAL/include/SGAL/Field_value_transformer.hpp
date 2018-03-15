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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_VALUE_TRANSFORMER_HPP
#define SGAL_FIELD_VALUE_TRANSFORMER_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Field_value_transformer {
public:
  virtual Boolean operator()(Boolean value) { return value; }
  virtual Float operator()(Float value) { return value; }
  virtual Uint operator()(Uint value) { return value; }
  virtual Int32 operator()(Int32 value) { return value; }
  virtual Scene_time operator()(Scene_time value) { return value; }
  virtual Vector2f operator()(const Vector2f& value) { return value; }
  virtual Vector3f operator()(const Vector3f& value) { return value; }
  virtual Vector4f operator()(const Vector4f& value) { return value; }
  virtual Rotation operator()(const Rotation& value) { return value; }
  virtual Bounding_sphere operator()(const Bounding_sphere& value)
  { return value; }
  virtual String operator()(const String& value) { return value; }
  virtual Shared_container operator()(Shared_container value) { return value; }
};

SGAL_END_NAMESPACE

#endif
