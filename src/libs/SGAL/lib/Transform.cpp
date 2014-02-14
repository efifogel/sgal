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
// $Id: Transform.cpp 12369 2011-12-26 13:42:24Z efif $
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <iostream>
#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Sphere_bound.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Transform::s_prototype(NULL);
const std::string Transform::s_tag = "Transform";

Vector3f Transform::s_def_translation(0, 0, 0);
Rotation Transform::s_def_rotation(0, 0, 1, 0);
Vector3f Transform::s_def_scale(1, 1, 1);
Rotation Transform::s_def_scale_orientation(0, 0, 1, 0);
Vector3f Transform::s_def_center(0, 0, 0);
Vector3f Transform::s_def_bbox_center(0, 0, 0);
Vector3f Transform::s_def_bbox_size(-1, -1, -1);

REGISTER_TO_FACTORY(Transform, "Transform");

//! \brief constructor.
Transform::Transform(Boolean proto) :
  Group(proto),
  m_translation(s_def_translation),
  m_rotation(s_def_rotation),
  m_scale(s_def_scale),
  m_scale_orientation(s_def_scale_orientation),
  m_center(s_def_center),
  m_dirty_matrix(false),
  m_dirty_inverse(false),
  m_dirty_parts(false)
{}

//! \brief destructor.
Transform::~Transform() {}

//! \brief sets the affine transform 4x4 matrix.
void Transform::set_matrix(const Matrix4f& matrix)
{
  m_matrix.set(matrix);
  m_dirty_parts = true;
  m_dirty_matrix = false;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
}

//! \brief obtains (a copy of) the matrix.
void Transform::get_matrix(Matrix4f& matrix)
{
  if (m_dirty_matrix) clean_matrix();
  m_matrix.get(matrix);
}

//! \brief obtains (a const reference to) the matrix.
const Matrix4f& Transform::get_matrix()
{
  if (m_dirty_matrix) clean_matrix();
  return m_matrix;
}

//! \brief builds the matrix out of the individual transforms.
void Transform::clean_matrix()
{
  const Vector3f& axis = m_rotation.get_axis();
  float angle = m_rotation.get_angle();
  float mySin = sinf(angle);
  float myCos = cosf(angle);
  float tmp = 1.0f - myCos;

  m_matrix[0][0] = myCos + tmp * axis[0] * axis[0];
  m_matrix[0][1] = tmp * axis[0] * axis[1];
  m_matrix[0][2] = tmp * axis[0] * axis[2];

  m_matrix[1][0] = m_matrix[0][1];
  m_matrix[1][1] = myCos + tmp * axis[1] * axis[1];
  m_matrix[1][2] = tmp * axis[1] * axis[2];

  m_matrix[2][0] = m_matrix[0][2];
  m_matrix[2][1] = m_matrix[1][2];
  m_matrix[2][2] = myCos + tmp * axis[2] * axis[2];

  Vector3f comp3;
  comp3.scale(mySin, axis);

  m_matrix[0][1] += comp3[2];
  m_matrix[0][2] -= comp3[1];
  m_matrix[1][0] -= comp3[2];
  m_matrix[1][2] += comp3[0];
  m_matrix[2][0] += comp3[1];
  m_matrix[2][1] -= comp3[0];

  m_matrix.set_row(3, m_translation);
  m_matrix[3][0] += m_center[0] -
    (m_center[0] * m_matrix[0][0] + m_center[1] * m_matrix[1][0] +
     m_center[2] * m_matrix[2][0]);
  m_matrix[3][1] += m_center[1] -
    (m_center[0] * m_matrix[0][1] + m_center[1] * m_matrix[1][1] +
     m_center[2] * m_matrix[2][1]);
  m_matrix[3][2] += m_center[2] -
    (m_center[0] * m_matrix[0][2] + m_center[1] * m_matrix[1][2] +
     m_center[2] * m_matrix[2][2]);

  // First scale, then rotation
  m_matrix[0][0] *= m_scale[0];
  m_matrix[0][1] *= m_scale[0];
  m_matrix[0][2] *= m_scale[0];
  m_matrix[1][0] *= m_scale[1];
  m_matrix[1][1] *= m_scale[1];
  m_matrix[1][2] *= m_scale[1];
  m_matrix[2][0] *= m_scale[2];
  m_matrix[2][1] *= m_scale[2];
  m_matrix[2][2] *= m_scale[2];

  m_dirty_matrix = false;
}

//! \brief extracts the individual Transforms from the matrix.
void Transform::clean_parts()
{
  m_translation.set(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);

  // GramSchmidt Algorithm, See Graphics Gems III pg 108

  // u* = u/|u|
  Vector3f ustar;
  m_matrix.get_row(0, ustar);
  float s1 = ustar.length();
  if (s1 == 0.0f) {
    // EnbWarn("Non singular matrix!");
    return;
  }
  ustar.scale(1.0f / s1, ustar);

  // v* = (v - (v . u*)u*)/|v - (v . u*)u*|
  Vector3f v;
  m_matrix.get_row(1, v);
  Vector3f tmp;
  tmp.scale(v.dot(ustar), ustar);
  Vector3f vstar;
  vstar.sub(v, tmp);
  float s2 = vstar.length();
  if (s2 == 0.0f) {
    // EnbWarn("Non singular matrix!");
    return;
  }
  vstar.scale(1.0f / s2, vstar);

  // w* =(w - (w . u*)u* - (w . v*)v*) / |(w - (w . u*)u* - (w . v*)v*)|
  Vector3f w;
  m_matrix.get_row(2, w);
  Vector3f tmp0;
  tmp0.scale(w.dot(ustar), ustar);
  Vector3f tmp1;
  tmp1.scale(w.dot(vstar), vstar);
  Vector3f wstar;
  tmp.add(tmp0, tmp1);
  wstar.sub(w, tmp);
  float s3 = wstar.length();
  if (s3 == 0.0f) {
    // EnbWarn("Non singular matrix!");
    return;
  }
  wstar.scale(1.0f / s3, wstar);

  float det = ustar[0] * (vstar[1] * wstar[2] - vstar[2] * wstar[1]);
  det += ustar[1] * (vstar[2] * wstar[0] - vstar[0] * wstar[2]);
  det += ustar[2] * (vstar[0] * wstar[1] - vstar[1] * wstar[0]);
  if (det < 0.0f) {
    wstar.negate(wstar);
    s3 = -s3;
  }

  Vector3f axis(1.0f, 0.0f, 0.0f);   // assume any unit axis
  float angle = 0.0f ;                // with no rotation.

  float trac = ustar[0] + vstar[1] + wstar[2];

  // if trace is 3 - R33 is unit matrix and this is null rotation
  // trying to solve the equation will fail anyway since W can be any unit
  // vector.
  if (trac != 3.0f) {
    // Solve the equation system:
    //
    //    (R_33 - I) * w = 0
    //
    // Where, I the identity matrix
    //
    // R_33 is the upper left submatrix of the rotation matrix comprises
    // of ustar, vstar, and wstar rows.
    //
    // The unknown w is an axis of rotation

    ustar[0] -= 1.0f;
    vstar[1] -= 1.0f;
    wstar[2] -= 1.0f;

    Boolean done = false;
    {
      axis[1] = wstar[0] * ustar[1] - wstar[1] * ustar[0];
      axis[2] = ustar[0] * vstar[1] - ustar[1] * vstar[0];
      if (axis[1] != 0.0f || axis[2] != 0.0f) {
        if (ustar[2] != 0.0f)
          axis[0] = (-vstar[2] * axis[1] - wstar[2] * axis[2]) / ustar[2];
        else if (ustar[0] != 0.0f)
          axis[0] = (-vstar[0] * axis[1] - wstar[0] * axis[2]) / ustar[0];
        else if (ustar[1] != 0.0f)
          axis[0] = (-vstar[1] * axis[1] - wstar[1] * axis[2]) / ustar[1];
        done = true;
      }
    }
    if (!done) {
      axis[2] = ustar[1] * vstar[2] - ustar[2] * vstar[1];
      axis[0] = vstar[1] * wstar[2] - vstar[2] * wstar[1];
      if (axis[2] != 0.0f || axis[0] != 0.0f) {
        if (vstar[0] != 0.0f)
          axis[1] = (-wstar[0] * axis[2] - ustar[0] * axis[0]) / vstar[0];
        else if (vstar[1] != 0.0f)
          axis[1] = (-wstar[1] * axis[2] - ustar[1] * axis[0]) / vstar[1];
        else if (vstar[2] != 0.0f)
          axis[1] = (-wstar[2] * axis[2] - ustar[2] * axis[0]) / vstar[2];
        done = true;
      }
    }
    if (!done) {
      axis[0] = vstar[2] * wstar[0] - vstar[0] * wstar[2];
      axis[1] = wstar[2] * ustar[0] - wstar[0] * ustar[2];
      if (axis[0] != 0.0f || axis[1] != 0.0f) {
        if (wstar[1] != 0.0f)
          axis[2] = (-ustar[1] * axis[0] - vstar[1] * axis[1]) / wstar[1];
        else if (wstar[2] != 0.0f)
          axis[2] = (-ustar[2] * axis[0] - vstar[2] * axis[1]) / wstar[2];
        else if (wstar[0] != 0.0f)
          axis[2] = (-ustar[0] * axis[0] - vstar[0] * axis[1]) / wstar[0];
        done = true;
      }
    }

    // This is wastful, should be set using quaternions!
    axis.normalize();
    float myCos = (trac - 1.0f) * 0.5f;
    float mySin = 0.0f;
    if (axis[2] != 0.0f)
      mySin = (ustar[1] + (myCos - 1.0f) * axis[0] * axis[1]) / axis[2];
    else if (axis[0] != 0.0f)
      mySin = (vstar[2] + (myCos - 1.0f) * axis[1] * axis[2]) / axis[0];
    else if (axis[1] != 0.0f)
      mySin = (wstar[0] + (myCos - 1.0f) * axis[2] * axis[0]) / axis[1];

    angle = arccosf(myCos);
    if (mySin < 0.0f) angle = SGAL_PI * 2.0f - angle;
  }

  m_rotation.set(axis[0], axis[1], axis[2], angle);
  m_scale.set(s1, s2, s3);

  m_dirty_parts = false;
}

//! \brief sets the translation.
void Transform::get_translation(float* v0, float* v1, float* v2)
{
  if (m_dirty_parts) {
    *v0 = m_matrix[3][0];
    *v1 = m_matrix[3][1];
    *v2 = m_matrix[3][2];
    return;
  }
  m_translation.get(v0, v1, v2);
}

//! \brief sets the translation.
void Transform::set_translation(float v0, float v1, float v2)
{
  if (m_dirty_parts) clean_parts();
  m_translation.set(v0, v1, v2);
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
}

//! \brief sets the rotation.
void Transform::set_rotation(float v0, float v1, float v2, float angle)
{
  if (m_dirty_parts) clean_parts();
  m_rotation.set_axis(v0, v1, v2);
  m_rotation.set_angle(angle);
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
}

//! \brief sets the scale factors.
void Transform::set_scale(float s0, float s1, float s2)
{
  if (m_dirty_parts) clean_parts();
  m_scale[0] = s0;
  m_scale[1] = s1;
  m_scale[2] = s2;
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
}

//! \brief sets the translation.
void Transform::set_translation(const Vector3f& translation)
{ set_translation(translation[0], translation[1], translation[2]); }

//! \brief sets the translation.
void Transform::get_translation(Vector3f& translation)
{ get_translation(&translation[0], &translation[1], &translation[2]); }

//! \brief sets the rotation.
void Transform::set_rotation(const Rotation& rotation)
{ set_rotation(rotation[0], rotation[1], rotation[2], rotation.get_angle()); }

//! \brief obtains the rotation.
void Transform::get_rotation(float* v0, float* v1, float* v2, float* v3)
{
  if (m_dirty_parts) clean_parts();
  m_rotation.get(v0,v1,v2,v3);
}

//! \brief obtains the rotation.
void Transform::get_rotation(Rotation& rotation)
{
  if (m_dirty_parts) clean_parts();
  m_rotation.get(rotation);
}

//! \brief obtains the scale factors.
void Transform::get_scale(float* v0, float* v1, float* v2)
{
  if (m_dirty_parts) clean_parts();
  *v0 = m_scale[0];
  *v1 = m_scale[1];
  *v2 = m_scale[2];
}

//! \brief sets the scale factors.
void Transform::set_scale(const Vector3f& scale)
{ set_scale(scale[0], scale[1], scale[2]); }

//! \brief sets the scale factors.
void Transform::get_scale(Vector3f& scale)
{ get_scale(&scale[0], &scale[1], &scale[2]); }

//! \brief sets the scale-orientation.
void Transform::set_scale_orientation(float v0, float v1, float v2, float v3)
{
  if (m_dirty_parts) clean_parts();
  m_scale_orientation.set(v0,v1,v2,v3);
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
}

//! \brief obtains the scale-orientation.
void
Transform::get_scale_orientation(float* v0, float* v1, float* v2, float* v3)
{
  if (m_dirty_parts) clean_parts();
  m_scale_orientation.get(v0,v1,v2,v3);
}

//! \brief sets the scale-orientation.
void Transform::set_scale_orientation(const Rotation& scale_orientation)
{
  set_scale_orientation(scale_orientation[0],
                        scale_orientation[1],
                        scale_orientation[2],
                        scale_orientation.get_angle());
}

//! \brief obtains the scale-orientation.
void Transform::get_scale_orientation(Rotation& scale_orientation)
{
  if (m_dirty_parts) clean_parts();
  m_scale_orientation.get(scale_orientation);
}

//! \brief sets the center of rotation.
void Transform::set_center(float v0, float v1, float v2)
{
  if (m_dirty_parts) clean_parts();
  m_center[0] = v0;
  m_center[1] = v1;
  m_center[2] = v2;
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
}

//! \brief obtains the center of rotation.
void Transform::get_center(float* v0, float* v1, float* v2)
{
  if (m_dirty_parts) clean_parts();
  *v0 = m_center[0];
  *v1 = m_center[1];
  *v2 = m_center[2];
}

//! \brief sets the center of rotation.
void Transform::set_center(const Vector3f& center)
{ set_center(center[0], center[1], center[2]); }

//! \brief obtains the center of rotation.
void Transform::get_center(Vector3f& center)
{ get_center(&center[0], &center[1], &center[2]); }

/*! \brief raises the matrix dirty flag, and sets the flag that indicates that
 * rendering is required.
 */
void Transform::parts_changed(Field_info* /* field_info */)
{
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_sphere_bound = true;
  set_rendering_required();
}

/*! \brief culls the transform node.
 * If this node should be rendered, its matrices is passed to cull_context.
 */
void Transform::cull(Cull_context& cull_context)
{
  if (! is_visible()) return;
  if (m_dirty_matrix) clean_matrix();
  cull_context.push_matrix(m_matrix);
  Group::cull(cull_context);
  cull_context.pop_matrix();
}

//! \brief traverses the children for selections.
void Transform::isect(Isect_action* isect_action)
{
  if (!is_visible()) return;
  if (m_dirty_matrix) clean_matrix();
  glPushMatrix();
  glMultMatrixf((float *)&m_matrix);
  Group::isect(isect_action);
  glPopMatrix();
}

//! \brief resets the transform.
void Transform::reset(Field_info* /* field_info */)
{
  set_translation(s_def_translation);
  set_rotation(s_def_rotation);
  set_scale(s_def_scale);
  set_center(s_def_center);
  set_scale_orientation(s_def_scale_orientation);
}

//! \brief cleans the bounding sphereof the transformation node.
Boolean Transform::clean_sphere_bound()
{
  if (m_locked_sphere_bound) return false;

  Group::clean_sphere_bound();
  if (m_dirty_matrix) clean_matrix();

  // transform the center of the boundng sphere
  Vector3f new_center;
  Vector3f current_center = m_sphere_bound.get_center();
  new_center.xform_pt(current_center, m_matrix);
  m_sphere_bound.set_center(new_center);

  // multiply the radius by the max scaling factor
  float scale_factor = m_scale.get_max_comp();
  m_sphere_bound.set_radius(m_sphere_bound.get_radius() * scale_factor);
  return true;
}

//! \brief initializes the node prototype.
void Transform::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Transform::parts_changed);

  // center
  Vector3f_handle_function center_func =
    static_cast<Vector3f_handle_function>(&Transform::center_handle);
  s_prototype->add_field_info(new SF_vector3f(CENTER, "center", center_func,
                                              s_def_center, exec_func));

  // translation
  Vector3f_handle_function translation_func =
    static_cast<Vector3f_handle_function>(&Transform::translation_handle);
  s_prototype->add_field_info(new SF_vector3f(TRANSLATION, "translation",
                                              translation_func,
                                              s_def_translation, exec_func));

  // rotation
  Rotation_handle_function rotation_func =
    static_cast<Rotation_handle_function>(&Transform::rotation_handle);
  s_prototype->add_field_info(new SF_rotation(ROTATION, "rotation",
                                              rotation_func, s_def_rotation,
                                              exec_func));

  // scale
  Vector3f_handle_function scale_func =
    static_cast<Vector3f_handle_function>(&Transform::scale_handle);
  s_prototype->add_field_info(new SF_vector3f(SCALE, "scale", scale_func,
                                              s_def_scale, exec_func));

  // reset
  exec_func = static_cast<Execution_function>(&Transform::reset);
  Boolean_handle_function reset_func =
    static_cast<Boolean_handle_function>(&Transform::reset_handle);
  s_prototype->add_field_info(new SF_bool(RESET, "reset", reset_func,
                                          exec_func));
}

//! \brief deletes the node prototype.
void Transform::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the node prototype.
Container_proto* Transform::get_prototype()
{
  if (!s_prototype) Transform::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Transform::set_attributes(Element* elem)
{
  Group::set_attributes(elem);

  typedef Element::Str_attr_iter Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "translation") {
      Vector3f vec(value);
      set_translation(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "center") {
      Vector3f vec(value);
      set_center(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "rotation") {
      Rotation rot(value);
      set_rotation(rot);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "scale") {
      Vector3f vec(value);
      set_scale(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "scaleOrientation") {
      Rotation vec(value);
      set_scale_orientation(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bboxCenter") {
      m_sphere_bound.set_center(value);
      m_dirty_sphere_bound = false;
      m_locked_sphere_bound = true;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bboxSize") {
      Vector3f vec(value);
      float radius = vec.length();
      m_sphere_bound.set_radius(radius);
      m_dirty_sphere_bound = false;
      m_locked_sphere_bound = true;
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
Attribute_list Transform::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  Vector3f vec;
  Rotation rot;

  attribs = Group::get_attributes();

  if (m_translation != s_def_translation) {
    attrib.first = "translation";
    get_translation(vec);
    attrib.second = vec.get_text();
    attribs.push_back(attrib);
  }
  if (m_rotation != s_def_rotation) {
    attrib.first = "rotation";
    get_rotation(rot);
    attrib.second = rot.get_text();
    attribs.push_back(attrib);
  }
  if (m_scale != s_def_scale) {
    attrib.first = "scale";
    get_scale(vec);
    attrib.second = vec.get_text();
    attribs.push_back(attrib);
  }
  if (m_scale_orientation != s_def_scale_orientation) {
    attrib.first = "scale_orientation";
    get_scale_orientation(rot);
    attrib.second = rot.get_text();
    attribs.push_back(attrib);
  }
  if (m_center != s_def_center) {
    attrib.first = "center";
    get_center(vec);
    attrib.second = vec.get_text();
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

#if 0
void Transform::pr_transform(Transform_handle Transform)
{
  if (m_dirty_matrix) clean_matrix();
  Matrix4f matrix;
  Transform->get_matrix(matrix);
  m_matrix.pre_mult(matrix);
}

void Transform::PostTransform(Transform_handle Transform)
{
  if (m_dirty_matrix) clean_matrix();
  Matrix4f matrix;
  Transform->get_matrix(matrix);
  m_matrix.post_mult(matrix);
}

Transform::Transform(const Transform& xform) { *this = xform; }

const Transform& Transform::operator=(const Transform& xform)
{
  if (&xform != this) {
    m_dirty_matrix = xform.m_dirty_matrix;
    m_dirty_inverse = xform.m_dirty_inverse;
    m_dirty_parts = xform.m_dirty_parts;
    m_PrevNormalize = xform.m_PrevNormalize;
    m_changedNormalize = xform.m_changedNormalize;
    if (!m_dirty_matrix) m_matrix.set(xform.m_matrix);
    if (!m_dirty_parts) {
      m_translation.set(xform.m_translation);
      m_rotation.set(xform.m_rotation);
      m_scale.set(xform.m_scale);
      m_scale_orientation.set(xform.m_scale_orientation);
      m_center.set(xform.m_center);
    }
    if (!m_dirty_inverse) m_inverse_matrix.set(xform.m_inverse_matrix);
  }
  return *this;
}

#endif

SGAL_END_NAMESPACE
