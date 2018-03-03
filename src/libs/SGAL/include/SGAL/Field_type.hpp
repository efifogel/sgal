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

#ifndef SGAL_FIELD_TYPE_HPP
#define SGAL_FIELD_TYPE_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

enum class Field_type {
  SF_BOOL = 0,
  SF_FLOAT,
  SF_UINT32,
  SF_INT32,
  SF_TIME,
  SF_VEC2F,
  SF_VEC3F,
  SF_COLOR,
  SF_VEC4F,
  SF_ROTATION,
  SF_SB,
  SF_IMAGE,
  SF_STR,
  SF_SHARED_CONTAINER,
  MF_BOOL,
  MF_FLOAT,
  MF_UINT32,
  MF_INT32,
  MF_TIME,
  MF_VEC2F,
  MF_VEC3F,
  MF_COLOR,
  MF_VEC4F,
  MF_ROTATION,
  MF_SB,
  MF_IMAGE,
  MF_STR,
  MF_SHARED_CONTAINER,
  NUM_FIELD_TYPES
};

/*! Obtain the name of a type.
 * \return the name of field type.
 */
extern const char* get_field_type_name(Field_type id);

SGAL_END_NAMESPACE

#endif
