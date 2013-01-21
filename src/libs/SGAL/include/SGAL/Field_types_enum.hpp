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
// $Source$
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_TYPE_ENUM
#define SGAL_FIELD_TYPE_ENUM

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

enum Field_type_enum {
  SFBOOL      = 1,
  SFFLOAT     = 2,
  SFUINT32    = 3,
  SFINT32     = 4,
  SFTIME      = 5,
  SFVEC2F     = 6,
  SFVEC3F     = 7,
  SFCOLOR     = 8,
  SFVEC4F     = 9,
  SFROTATION  = 10,
  SFSB        = 11,
  SFIMAGE     = 12,
  SFSTRING    = 13,
  SFCONTAINER = 14,
  MFFLOAT     = 22,
  MFUINT32    = 23,
  MFINT32     = 24,
  MFTIME      = 25,
  MFVEC2F     = 26,
  MFVEC3F     = 27,
  MFCOLOR     = 28,
  MFVEC4F     = 29,
  MFROTATION  = 30,
  MFSB        = 31,
  MFSTRING    = 32,
  MFCONTAINER = 33
};

class Field_types_utils {
public:
  static Field_type_enum get_field_type(const std::string& type)
  {
    if (type == "SFBool" || type == "Boolean") return SFBOOL;
    else if (type == "SFColor" || type == "Color") return SFCOLOR;
    else if (type == "SFFloat" || type == "Float") return SFFLOAT;
    else if (type == "SFImage" || type == "Image") return SFIMAGE;
    else if (type == "SFInt32" || type == "Integer") return SFINT32;
    else if (type == "SFNode" || type == "Container") return SFCONTAINER;
    else if (type == "SFRotation" || type == "Rotation") return SFROTATION;
    else if (type == "SFSphereBound" || type == "SphereBound") return SFSB;
    else if (type == "SFString" || type == "String") return SFSTRING;
    else if (type == "SFTime" || type == "Time") return SFTIME;
    else if (type == "SFVec2f" || type == "Vector2Float") return SFVEC2F;
    else if (type == "SFVec3F" || type == "Vector3Float") return SFVEC3F;
    else if (type == "SFVec4F" || type == "Vector4Float") return SFVEC4F;
    else if (type == "MFColor" || type == "Colors") return MFVEC3F;
    else if (type == "MFFloat" || type == "Floats") return MFFLOAT;
    else if (type == "MFInt32" || type == "Integers") return MFINT32;
    else if (type == "MFNode" || type == "Containers") return MFCONTAINER;
    else if (type == "MFRotation" || type == "Rotations") return MFROTATION;
    else if (type == "MFSphereBound" || type == "SphereBounds") return MFSB;
    else if (type == "MFString" || type == "Strings") return MFSTRING;
    else if (type == "MFTime" || type == "Times") return MFTIME;
    else if (type == "MFVec2F" || type == "Vector2Floats") return MFVEC2F;
    else if (type == "MFVec3F" || type == "Vector3Floats") return MFVEC3F;
    else if (type == "MFVec4F" || type == "Vector4Floats") return SFVEC4F;
    else {
      assert(false);
      return (Field_type_enum)0;
    }
  }
};

SGAL_END_NAMESPACE

#endif
