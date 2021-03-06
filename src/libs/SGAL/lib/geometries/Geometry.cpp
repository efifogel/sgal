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

#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Geometry::s_prototype(nullptr);

const Boolean Geometry::s_def_generate_color(false);
const Boolean Geometry::s_def_generate_tex_coord(true);

//! \brief constructor.
Geometry::Geometry(Boolean proto) :
  Container(proto),
  m_dirty_bounding_sphere(true),
  m_bb_is_pre_set(false),
  m_generate_color(s_def_generate_color),
  m_generate_tex_coord(s_def_generate_tex_coord)
{}

//! \brief initializes the geometry prototype.
void Geometry::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // sphereBound
  auto bounding_sphere_func =
    static_cast<Bounding_sphere_handle_function>(&Geometry::bounding_sphere_handle);
  s_prototype->add_field_info(new SF_bounding_sphere(BOUNDING_SPHERE,
                                                     "boundingSphere",
                                                     Field_rule::RULE_OUT,
                                                     bounding_sphere_func));

  // generatedTexCoord
  auto generate_tex_coord_func =
    static_cast<Boolean_handle_function>
    (&Geometry::generate_tex_coord_handle);
  s_prototype->add_field_info(new SF_bool(GENERATE_TEX_COORD,
                                          "generateTexCoord",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          generate_tex_coord_func,
                                          s_def_generate_tex_coord));
}

//! \brief deletes the geometry prototype.
void Geometry::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the geometry prototype.
Container_proto* Geometry::get_prototype()
{
  if (!s_prototype) Geometry::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the geometric object.
void Geometry::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "bboxCenter") {
      m_bounding_sphere.set_center(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bboxSize") {
      Vector3f vec(value);
      float radius = vec.length();
      m_bounding_sphere.set_radius(radius);
      m_bb_is_pre_set = true;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "generateTexCoord") {
      set_generate_tex_coord(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief sets the flag that indicates that the sphere bound should be
 * cleaned.
 */
void Geometry::bounding_sphere_changed(const Field_info* /* field_info */)
{ m_dirty_bounding_sphere = true; }


//! \brief obtains bounding sphere of the geometry.
const Bounding_sphere& Geometry::get_bounding_sphere()
{
  if (m_dirty_bounding_sphere) clean_bounding_sphere();
  return m_bounding_sphere;
}

/*! \brief returns true if the current matrix contains scaling.
 * We get the current matrix from the matrix stack, multiply it
 * by a unit vector and check its length after the transformation.
 */
Boolean Geometry::has_scale()
{
  Vector4f v1(0.5773502f, 0.5773502f, 0.5773502f, 0.0f);
  float epsilon = 0.000001f;

  float modelview_mat[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_mat);
  Matrix4f m;
  m.set(modelview_mat);

  float pre_length = v1.length();
  v1.xform(v1, m);
  float post_length = v1.length();
#if 0
  std::cout << "post_length: " << post_length
            << ", pre_length: " << pre_length
            << std::endl;
#endif
  if (abs(post_length - pre_length) > epsilon) return true;
  return false;
}

SGAL_END_NAMESPACE
