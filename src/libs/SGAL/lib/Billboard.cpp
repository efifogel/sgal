// Copyright (c) 2014 Israel.
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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Billboard.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Billboard::s_prototype(nullptr);
const std::string Billboard::s_tag = "Billboard";

const Vector3f Billboard::s_def_axis_of_rotation(0, 1, 0);

REGISTER_TO_FACTORY(Billboard, "Billboard");

//! \brief constructor.
Billboard::Billboard(Boolean proto) :
  Group(proto),
  m_axis_of_rotation(s_def_axis_of_rotation),
  m_dirty_matrix(true),
  m_transformed(true)
{}

//! \brief destructor.
Billboard::~Billboard() {}

//! \brief initializes the node prototype.
void Billboard::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func =
    static_cast<Execution_function>(&Billboard::axis_of_rotation_changed);

  // center
  auto axis_of_rotation_func =
    static_cast<Vector3f_handle_function>(&Billboard::axis_of_rotation_handle);
  s_prototype->add_field_info(new SF_vector3f(AXIS_OF_ROTATION,
                                              "axis_of_rotation",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              axis_of_rotation_func,
                                              s_def_axis_of_rotation,
                                              exec_func));
}

//! \brief deletes the node prototype.
void Billboard::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Billboard::get_prototype()
{
  if (!s_prototype) Billboard::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Billboard::set_attributes(Element* elem)
{
  Group::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "axisOfRotation") {
      Vector3f vec(value);
      set_axis_of_rotation(vec);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief culls the transform node.
void Billboard::cull(Cull_context& cull_context)
{
  if (! is_visible()) return;
  if (cull_context.is_transformed()) m_dirty_matrix = true;
  if (m_dirty_matrix) clean_matrix(cull_context.get_current_wtm());
  auto transformed = cull_context.is_transformed();
  if (is_transformed()) {
    set_transformed(false);
    cull_context.set_transformed(true);
  }
  cull_context.push_matrix(m_matrix);
  Group::cull(cull_context);
  cull_context.pop_matrix();
  cull_context.set_transformed(transformed);
}

//! \brief traverses the children for selections.
void Billboard::isect(Isect_action* isect_action)
{
  /*! \todo use cull_context.
   * Until then, the dirty-matrix flag is set upon invocation essentially
   * forcing the cleaning of the matrix.
   */
  if (!is_visible()) return;
  GLfloat model_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, model_matrix);
  Matrix4f matrix(model_matrix);
  m_dirty_matrix = true;
  if (m_dirty_matrix) clean_matrix(matrix);
  glPushMatrix();
  glMultMatrixf((float *)&m_matrix);
  Group::isect(isect_action);
  glPopMatrix();
}

//! \brief cleans the bounding sphere of the billboard node.
void Billboard::clean_bounding_sphere()
{ return Group::clean_bounding_sphere(); }

//! \brief processes change of axis of rotation.
void Billboard::axis_of_rotation_changed(const Field_info* field_info)
{ m_dirty_matrix = true; }

//! \brief sets the axis used to perform the rotation.
void Billboard::set_axis_of_rotation(const Vector3f& axis_of_rotation)
{
  m_dirty_matrix = true;
  m_axis_of_rotation = axis_of_rotation;
}

//! \brief alters the matrix so that the z-axis turns to point at the viewer.
void Billboard::clean_matrix(const Matrix4f& matrix)
{
  Matrix4f inverse;
  inverse.invert_affine(matrix);
  Vector3f eye;
  eye[0] = inverse[0][2];
  eye[1] = inverse[1][2];
  eye[2] = inverse[2][2];
  eye.normalize();

  const Vector3f zero(0, 0, 0);
  auto screen_align = m_axis_of_rotation.equal(zero);
  Vector3f up;
  if (screen_align) up.set(0, 1, 0);
  else up.set(m_axis_of_rotation);
  Vector3f right, look;
  right.cross(up, eye);
  right.normalize();
  if (screen_align) {
    up.cross(eye, right);
    up.normalize();
  }
  look.cross(right, up);
  look.normalize();

  m_matrix[0][0] = right[0];
  m_matrix[0][1] = up[0];
  m_matrix[0][2] = look[0];
  m_matrix[0][3] = 0;

  m_matrix[1][0] = right[1];
  m_matrix[1][1] = up[1];
  m_matrix[1][2] = look[1];
  m_matrix[1][3] = 0;

  m_matrix[2][0] = right[2];
  m_matrix[2][1] = up[2];
  m_matrix[2][2] = look[2];
  m_matrix[2][3] = 0;

  m_matrix[3][0] = 0;
  m_matrix[3][1] = 0;
  m_matrix[3][2] = 0;
  m_matrix[3][3] = 1;

  m_dirty_matrix = false;
  set_transformed(true);
}

SGAL_END_NAMESPACE
