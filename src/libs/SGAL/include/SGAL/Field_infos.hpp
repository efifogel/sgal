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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_INFOS_HPP
#define SGAL_FIELD_INFOS_HPP

/*! \file
 * Specific field infos that type instantiate Field_infoTemplate
 *
 * These field infos are used by the scene graph nodes -
 * The scene graph nodes allocate specific field infos and
 * add them to their protoype (for each potential field the node has).
 * The typeId template argument should match the X3D field type IDs.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Sphere_bound.hpp"
#include "SGAL/Field_types_enum.hpp"
#include "SGAL/Field_info_template.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Array_types.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

typedef boost::shared_ptr<Container>                   Shared_container;

// Field information records
// Single field
typedef Field_info_template<Boolean, SF_BOOL>          SF_bool;
typedef Field_info_template<Float, SF_FLOAT>           SF_float;
typedef Field_info_template<Uint, SF_INT32>            SF_uint;
typedef Field_info_template<Int, SF_INT32>             SF_int;
typedef Field_info_template<Scene_time, SF_TIME>       SF_time;
typedef Field_info_template<Vector2f, SF_VEC2F>        SF_vector2f;
typedef Field_info_template<Vector3f, SF_VEC3F>        SF_vector3f;
typedef Field_info_template<Vector3f, SF_COLOR>        SF_color;
typedef Field_info_template<Vector4f, SF_VEC4F>        SF_vector4f;
typedef Field_info_template<Rotation, SF_ROTATION>     SF_rotation;
typedef Field_info_template<Sphere_bound, SF_SB>       SF_sphere_bound;
typedef Field_info_template<std::string, SF_STR>       SF_string;
typedef Field_info_template<Container*, SF_CONTAINER>  SF_container;
typedef Field_info_template<Shared_container, SF_SHARED_CONTAINER>
                                                       SF_shared_container;

// Multi field
typedef Field_info_template<Boolean_array, MF_FLOAT>   MF_boolean;
typedef Field_info_template<Float_array, MF_FLOAT>     MF_float;
typedef Field_info_template<Uint_array, MF_INT32>      MF_uint;
typedef Field_info_template<Int_array, MF_INT32>       MF_int;
typedef Field_info_template<Scene_time_array, MF_TIME> MF_time;
typedef Field_info_template<Vector2f_array, MF_VEC2F>  MF_vector2f;
typedef Field_info_template<Vector3f_array, MF_VEC3F>  MF_vector3f;
typedef Field_info_template<Vector3f_array, MF_COLOR>  MF_color;
typedef Field_info_template<Vector4f_array, MF_VEC4F>  MF_vector4f;
typedef Field_info_template<Rotation_array, MF_ROTATION>
                                                       MF_rotation;
typedef Field_info_template<Sphere_bound_array, MF_SB> MF_sphere_bound;
typedef Field_info_template<String_array, MF_STR>      MF_string;
typedef Field_info_template<Container_array, MF_CONTAINER>
                                                       MF_container;
typedef Field_info_template<Shared_container_array, MF_SHARED_CONTAINER>
                                                       MF_shared_container;

// Field handle functions
// Single field handle functions
typedef Handle_function<Boolean>::type        Boolean_handle_function;
typedef Handle_function<Float>::type          Float_handle_function;
typedef Handle_function<Uint>::type           Uint_handle_function;
typedef Handle_function<Int>::type            Int_handle_function;
typedef Handle_function<Scene_time>::type     Scene_time_handle_function;
typedef Handle_function<Vector2f>::type       Vector2f_handle_function;
typedef Handle_function<Vector3f>::type       Vector3f_handle_function;
typedef Handle_function<Vector3f>::type       Vector3f_handle_function;
typedef Handle_function<Vector4f>::type       Vector4f_handle_function;
typedef Handle_function<Rotation>::type       Rotation_handle_function;
typedef Handle_function<Sphere_bound>::type   Sphere_bound_handle_function;
typedef Handle_function<std::string>::type    String_handle_function;
typedef Handle_function<Container*>::type     Container_pointer_handle_function;
typedef Handle_function<Shared_container>::type
                                              Shared_container_handle_function;

// Multi field handle functions
typedef Handle_function<Boolean_array>::type  Boolean_array_handle_function;
typedef Handle_function<Float_array>::type    Float_array_handle_function;
typedef Handle_function<Uint_array>::type     Uint_array_handle_function;
typedef Handle_function<Int_array>::type      Int_array_handle_function;
typedef Handle_function<Scene_time_array>::type
                                              Scene_time_array_handle_function;
typedef Handle_function<Vector2f_array>::type Vector2f_array_handle_function;
typedef Handle_function<Vector3f_array>::type Vector3f_array_handle_function;
typedef Handle_function<Vector3f_array>::type Vector3f_array_handle_function;
typedef Handle_function<Vector4f_array>::type Vector4f_array_handle_function;
typedef Handle_function<Rotation_array>::type Rotation_array_handle_function;
typedef Handle_function<Sphere_bound_array>::type
  Sphere_bound_array_handle_function;
typedef Handle_function<String_array>::type   String_array_handle_function;
typedef Handle_function<Shared_container_array>::type
  Shared_container_array_handle_function;

#if 0
class Field_info_utils {
public:
  /*! */
  static Field_info* allocate_field_info(Uint id,
                                         const std::string& name,
                                         const std::string& type,
                                         const std::string& value,
                                         Ulong offset,
                                         Execution_function execution = NULL)
  {
    Field_type_enum type_enum = Field_types_utils::get_field_type(type);

    if (type_enum == SF_BOOL) {
      Boolean flag = false;
      if (value == "true" || value == "false") flag = true;
      return new SF_bool(id, name, offset, execution, false, true, flag);
    }
    else if (type_enum == SF_FLOAT)
      return new SF_float(id, name, offset, execution, false, true,
                          atoff(value.c_str()));
    else if (type_enum == SF_TIME)
      return new SF_time(id, name, offset, execution, false, true,
                         ::strtod(value.c_str(),0));
    else if (type_enum == SF_INT32)
      return new SF_int(id, name, offset, execution, false, true,
                        ::atoi(value.c_str()));
    else if (type_enum == SF_VEC2F)
      return new SF_vector2f(id, name, offset, execution, false, true,
                             Vector2f(value));
    else if (type_enum == SF_VEC3F)
      return new SF_vector3f(id, name, offset, execution, false, true,
                             Vector3f(value));
    else if (type_enum == SF_ROTATION)
      return new SF_rotation(id, name, offset, execution, false, true,
                             Rotation(value));
    else if (type_enum == SF_SB)
      return new SF_sphere_bound(id, name, offset, execution, false, true,
                                 Sphere_bound(value));
    else if (type_enum == SF_VEC4F)
      return new SF_vector4f(id, name, offset, execution, false, true,
                             Vector4f(value));
    else if (type_enum == SF_STRING)
      return new SF_string(id, name, offset, execution, false, true, value);
    else if (type_enum == SF_CONTAINER)
      return new SF_container(id, name, offset, execution, false, true, NULL);
    else if (type_enum == SF_CONTAINER)
      return new SF_container(id, name, offset, execution, false, true, NULL);
    else if (type_enum == SF_SHARED_CONTAINER)
      return new SF_shared_container(id, name, offset, execution,
                                     false, true, Shared_container());
    else
      return NULL;
  }
};
#endif

SGAL_END_NAMESPACE

#endif
