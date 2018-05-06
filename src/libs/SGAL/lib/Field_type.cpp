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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Field_type.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief the names of the field types.
static const char* s_type_names[] = {
  "SFBool", "SFFloat", "SFUint32", "SFInt32",
  "SFTime", "SFVec2f", "SFVec3f", "SFColor",
  "SFVec4f", "SFRotation", "SFSphereBound", "SFImage",
  "SFString", "SFNode", "MFBool", "MFFloat",
  "MFUint32", "MFInt32", "MFTime", "MFVec2f",
  "MFVec3f", "MFColor", "MFVec4f", "MFRotation",
  "MFSphereBound", "MFImage", "MFString", "MFNode",
};

//! \brief obtains the name of a type.
const char* get_field_type_name(Field_type id)
{ return s_type_names[static_cast<int>(id)]; }

SGAL_END_NAMESPACE
