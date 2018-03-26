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

#ifndef SGAL_FIELD_VALUE_APPLIER_HPP
#define SGAL_FIELD_VALUE_APPLIER_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Bounding_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

class SGAL_SGAL_DECL Field_value_applier {
public:
  typedef boost::shared_ptr<Container>                  Shared_container;

  virtual void operator()(Boolean value) {}
  virtual void operator()(Float value) {}
  virtual void operator()(Uint value) {}
  virtual void operator()(Int32 value) {}
  virtual void operator()(Scene_time value) {}
  virtual void operator()(const Vector2f& value) {}
  virtual void operator()(const Vector3f& value) {}
  virtual void operator()(const Vector4f& value) {}
  virtual void operator()(const Rotation& value) {}
  virtual void operator()(const Bounding_sphere& value) {}
  virtual void operator()(const String& value) {}
  virtual void operator()(Shared_container value) {}
};

SGAL_END_NAMESPACE

#endif
