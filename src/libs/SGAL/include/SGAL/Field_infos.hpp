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
#include "SGAL/Bounding_sphere.hpp"
#include "SGAL/Field_info_template.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Array_types.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

typedef boost::shared_ptr<Container>                   Shared_container;

// Field information records
// Single field
typedef Field_info_template<Boolean, Field_info::SF_BOOL>          SF_bool;
typedef Field_info_template<Float, Field_info::SF_FLOAT>           SF_float;
typedef Field_info_template<Uint, Field_info::SF_INT32>            SF_uint;
typedef Field_info_template<Int32, Field_info::SF_INT32>           SF_int32;
typedef Field_info_template<Scene_time, Field_info::SF_TIME>       SF_time;
typedef Field_info_template<Vector2f, Field_info::SF_VEC2F>        SF_vector2f;
typedef Field_info_template<Vector3f, Field_info::SF_VEC3F>        SF_vector3f;
typedef Field_info_template<Vector3f, Field_info::SF_COLOR>        SF_color;
typedef Field_info_template<Vector4f, Field_info::SF_VEC4F>        SF_vector4f;
typedef Field_info_template<Rotation, Field_info::SF_ROTATION>     SF_rotation;
typedef Field_info_template<Bounding_sphere, Field_info::SF_SB>       SF_sphere_bound;
typedef Field_info_template<std::string, Field_info::SF_STR>       SF_string;
typedef Field_info_template<Shared_container, Field_info::SF_SHARED_CONTAINER>
                                                       SF_shared_container;

// Multi field
typedef Field_info_template<Boolean_array, Field_info::MF_BOOL>    MF_bool;
typedef Field_info_template<Float_array, Field_info::MF_FLOAT>     MF_float;
typedef Field_info_template<Uint_array, Field_info::MF_INT32>      MF_uint;
typedef Field_info_template<Int32_array, Field_info::MF_INT32>     MF_int32;
typedef Field_info_template<Scene_time_array, Field_info::MF_TIME> MF_time;
typedef Field_info_template<Vector2f_array, Field_info::MF_VEC2F>  MF_vector2f;
typedef Field_info_template<Vector3f_array, Field_info::MF_VEC3F>  MF_vector3f;
typedef Field_info_template<Vector3f_array, Field_info::MF_COLOR>  MF_color;
typedef Field_info_template<Vector4f_array, Field_info::MF_VEC4F>  MF_vector4f;
typedef Field_info_template<Rotation_array, Field_info::MF_ROTATION>
                                                                   MF_rotation;
typedef Field_info_template<Bounding_sphere_array, Field_info::MF_SB> MF_sphere_bound;
typedef Field_info_template<String_array, Field_info::MF_STR>      MF_string;
typedef Field_info_template<Shared_container_array, Field_info::MF_SHARED_CONTAINER>
                                                       MF_shared_container;

// Field handle functions
// Single field handle functions
typedef Handle_function<Boolean>::type        Boolean_handle_function;
typedef Handle_function<Float>::type          Float_handle_function;
typedef Handle_function<Uint>::type           Uint_handle_function;
typedef Handle_function<Int32>::type          Int32_handle_function;
typedef Handle_function<Scene_time>::type     Scene_time_handle_function;
typedef Handle_function<Vector2f>::type       Vector2f_handle_function;
typedef Handle_function<Vector3f>::type       Vector3f_handle_function;
typedef Handle_function<Vector3f>::type       Vector3f_handle_function;
typedef Handle_function<Vector4f>::type       Vector4f_handle_function;
typedef Handle_function<Rotation>::type       Rotation_handle_function;
typedef Handle_function<Bounding_sphere>::type   Bounding_sphere_handle_function;
typedef Handle_function<std::string>::type    String_handle_function;
typedef Handle_function<Shared_container>::type
                                              Shared_container_handle_function;

// Multi field handle functions
typedef Handle_function<Boolean_array>::type  Boolean_array_handle_function;
typedef Handle_function<Float_array>::type    Float_array_handle_function;
typedef Handle_function<Uint_array>::type     Uint_array_handle_function;
typedef Handle_function<Int32_array>::type    Int32_array_handle_function;
typedef Handle_function<Scene_time_array>::type
                                              Scene_time_array_handle_function;
typedef Handle_function<Vector2f_array>::type Vector2f_array_handle_function;
typedef Handle_function<Vector3f_array>::type Vector3f_array_handle_function;
typedef Handle_function<Vector3f_array>::type Vector3f_array_handle_function;
typedef Handle_function<Vector4f_array>::type Vector4f_array_handle_function;
typedef Handle_function<Rotation_array>::type Rotation_array_handle_function;
typedef Handle_function<Bounding_sphere_array>::type
  Bounding_sphere_array_handle_function;
typedef Handle_function<String_array>::type   String_array_handle_function;
typedef Handle_function<Shared_container_array>::type
  Shared_container_array_handle_function;

SGAL_END_NAMESPACE

#endif
