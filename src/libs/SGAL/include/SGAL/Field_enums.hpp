// Copyright (c) 2004 Israel.
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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_ENUMS
#define SGAL_FIELD_ENUMS

/*! \file
 * Fields are the elemental data types used to define the properties of
 * containers. Container are composed of fields whose enumeration are defined
 * below.
 *
 * There are two general classes of field types: field types that contain
 * a single value (where a value may be a single number, a vector, or even
 * an image), and field types that contain an ordered list of multiple
 * values. Single-valued field types have names that begin with SF.
 * Multiple-valued field types have names that begin with MF.
 * Multiple-valued fields are written as an ordered list of values. If the
 * field has zero values, the value is empty but still represented.
 */

#include <assert.h>
#include <string>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

enum Field_type {
  SF_BOOL = 1,
  SF_FLOAT = 2,
  SF_UINT32 = 3,
  SF_INT32 = 4,
  SF_TIME = 5,
  SF_VEC2F = 6,
  SF_VEC3F = 7,
  SF_COLOR = 8,
  SF_VEC4F = 9,
  SF_ROTATION = 10,
  SF_SB = 11,
  SF_IMAGE = 12,
  SF_STR = 13,
  SF_SHARED_CONTAINER = 14,
  MF_BOOLEAN = 21,
  MF_FLOAT = 22,
  MF_UINT32 = 23,
  MF_INT32 = 24,
  MF_TIME = 25,
  MF_VEC2F = 26,
  MF_VEC3F = 27,
  MF_COLOR = 28,
  MF_VEC4F = 29,
  MF_ROTATION = 30,
  MF_SB = 31,
  MF_IMAGE = 32,
  MF_STR = 33,
  MF_SHARED_CONTAINER = 34
};

enum Field_rule {
  RULE_IN = 1,
  RULE_OUT,
  RULE_FIELD,
  RULE_EXPOSED_FIELD,
};

class Field_types_utils {
public:
  static Field_type get_field_type(const std::string& type)
  {
    if (type == "SF_Bool" || type == "Boolean") return SF_BOOL;
    else if (type == "SFColor" || type == "Color") return SF_COLOR;
    else if (type == "SFFloat" || type == "Float") return SF_FLOAT;
    else if (type == "SFImage" || type == "Image") return SF_IMAGE;
    else if (type == "SFInt32" || type == "Integer") return SF_INT32;
    else if (type == "SFSharedContainer" || type == "SharedContainer")
      return SF_SHARED_CONTAINER;
    else if (type == "SFRotation" || type == "Rotation") return SF_ROTATION;
    else if (type == "SFSphereBound" || type == "SphereBound") return SF_SB;
    else if (type == "SFString" || type == "String") return SF_STR;
    else if (type == "SFTime" || type == "Time") return SF_TIME;
    else if (type == "SFVec2f" || type == "Vector2Float") return SF_VEC2F;
    else if (type == "SFVec3F" || type == "Vector3Float") return SF_VEC3F;
    else if (type == "SFVec4F" || type == "Vector4Float") return SF_VEC4F;
    else if (type == "MFColor" || type == "Colors") return MF_VEC3F;
    else if (type == "MFFloat" || type == "Floats") return MF_FLOAT;
    else if (type == "MFInt32" || type == "Integers") return MF_INT32;
    else if (type == "MFSharedNode" || type == "SharedContainers")
      return MF_SHARED_CONTAINER;
    else if (type == "MFRotation" || type == "Rotations") return MF_ROTATION;
    else if (type == "MFSphereBound" || type == "SphereBounds") return MF_SB;
    else if (type == "MFString" || type == "Strings") return MF_STR;
    else if (type == "MFTime" || type == "Times") return MF_TIME;
    else if (type == "MFVec2F" || type == "Vector2Floats") return MF_VEC2F;
    else if (type == "MFVec3F" || type == "Vector3Floats") return MF_VEC3F;
    else if (type == "MFVec4F" || type == "Vector4Floats") return MF_VEC4F;
    else {
      assert(false);
      return (Field_type)0;
    }
  }
};

SGAL_END_NAMESPACE

#endif
