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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <algorithm>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Bounding_sphere.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Transform::s_prototype(nullptr);
const std::string Transform::s_tag = "Transform";

const Vector3f Transform::s_def_translation(0, 0, 0);
const Rotation Transform::s_def_rotation(0, 0, 1, 0);
const Vector3f Transform::s_def_scale(1, 1, 1);
const Rotation Transform::s_def_scale_orientation(0, 0, 1, 0);
const Vector3f Transform::s_def_center(0, 0, 0);

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

//! \brief clones the container (virtual constructor) with deep-copy.
Container* Transform::clone()
{
  auto* cont = new Transform;
  SGAL_assertion(cont);

  // Clone only the fields that belong to the base container.
  auto* proto = get_prototype();
  for (auto it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    if ((*it).first > FIRST) continue;
    const auto* field_info = (*it).second;
    SGAL_assertion(field_info);
    auto rule = field_info->get_rule();
    if ((rule == Field_rule::RULE_IN) || (rule == Field_rule::RULE_OUT))
      continue;
    field_info->clone(this, cont);
  }

  // Clone (copy) the fields of this container:
  cont->copy_local(this);
  return cont;
}

//! \brief copies only the local fields of a given container.
void Transform::copy_local(const Transform* other)
{
  if (!other->m_dirty_parts) {
    m_translation = other->m_translation;
    m_rotation = other->m_rotation;
    m_scale = other->m_scale;
    m_scale_orientation = other->m_scale_orientation;
    m_center = other->m_center;
  }
  if (!other->m_dirty_matrix) {
    m_matrix.set(other->m_matrix);
  }
  m_dirty_parts = other->m_dirty_parts;
  m_dirty_matrix = other->m_dirty_matrix;
  m_dirty_inverse = other->m_dirty_inverse;
}

//! \brief applies an unary operation on every field of the container.
void Transform::apply(Field_value_applier& op)
{
  if (m_dirty_parts) clean_parts();
  Container::apply(op);
}

/*! \brief transforms every field of this container using a unary operation into
 * a corresponding field in a target container.
 */
void Transform::transform(Container* target, Field_value_transformer& op)
{
  if (m_dirty_parts) clean_parts();
  static_cast<Transform*>(target)->m_dirty_matrix = m_dirty_matrix;
  static_cast<Transform*>(target)->m_dirty_inverse = m_dirty_inverse;
  Container::transform(target, op);
}

//! \brief sets the affine transform 4x4 matrix.
void Transform::set_matrix(const Matrix4f& matrix)
{
  m_matrix.set(matrix);
  m_dirty_parts = true;
  m_dirty_matrix = false;
  m_dirty_inverse = true;
  m_dirty_bounding_sphere = true;
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
  set_transformed(true);
}

//! \brief extracts the individual Transformations from the matrix.
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
void Transform::get_translation(float& v0, float& v1, float& v2)
{
  if (m_dirty_parts) {
    v0 = m_matrix[3][0];
    v1 = m_matrix[3][1];
    v2 = m_matrix[3][2];
    return;
  }
  m_translation.get(v0, v1, v2);
}

//! \brief sets the translation.
void Transform::set_translation(float v0, float v1, float v2)
{
  if (m_dirty_parts) clean_parts();
  m_translation.set(v0, v1, v2);
  parts_changed(get_field_info(TRANSLATION));
}

//! \brief sets the rotation.
void Transform::set_rotation(float v0, float v1, float v2, float angle)
{
  if (m_dirty_parts) clean_parts();
  m_rotation.set_axis(v0, v1, v2);
  m_rotation.set_angle(angle);
  parts_changed(get_field_info(ROTATION));
}

//! \brief sets the scale factors.
void Transform::set_scale(float s0, float s1, float s2)
{
  if (m_dirty_parts) clean_parts();
  m_scale[0] = s0;
  m_scale[1] = s1;
  m_scale[2] = s2;
  parts_changed(get_field_info(SCALE));
}

//! \brief sets the translation.
void Transform::set_translation(const Vector3f& translation)
{ set_translation(translation[0], translation[1], translation[2]); }

//! \brief obtains the translation.
const Vector3f& Transform::get_translation()
{
  if (m_dirty_parts) {
    m_translation[0] = m_matrix[3][0];
    m_translation[1] = m_matrix[3][1];
    m_translation[2] = m_matrix[3][2];
  }
  return m_translation;
}

//! \brief sets the rotation.
void Transform::set_rotation(const Rotation& rotation)
{ set_rotation(rotation[0], rotation[1], rotation[2], rotation.get_angle()); }

//! \brief obtains the rotation.
void Transform::get_rotation(float& v0, float& v1, float& v2, float& v3)
{
  if (m_dirty_parts) clean_parts();
  m_rotation.get(v0, v1, v2, v3);
}

//! \brief obtains the (non-const) rotation.
const Rotation& Transform::get_rotation()
{
  if (m_dirty_parts) clean_parts();
  return m_rotation;
}

//! \brief obtains the scale factors.
void Transform::get_scale(float& v0, float& v1, float& v2)
{
  if (m_dirty_parts) clean_parts();
  v0 = m_scale[0];
  v1 = m_scale[1];
  v2 = m_scale[2];
}

//! \brief sets the scale.
void Transform::set_scale(const Vector3f& scale)
{ set_scale(scale[0], scale[1], scale[2]); }

//! \brief obtains the scale.
const Vector3f& Transform::get_scale()
{
  if (m_dirty_parts) clean_parts();
  return m_scale;
}

//! \brief sets the scale-orientation.
void Transform::set_scale_orientation(float v0, float v1, float v2, float v3)
{
  if (m_dirty_parts) clean_parts();
  m_scale_orientation.set(v0, v1, v2, v3);
  parts_changed(get_field_info(SCALE_ORIENTATION));
}

//! \brief obtains the scale-orientation.
void
Transform::get_scale_orientation(float& v0, float& v1, float& v2, float& v3)
{
  if (m_dirty_parts) clean_parts();
  m_scale_orientation.get(v0, v1, v2, v3);
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
const Rotation& Transform::get_scale_orientation()
{
  if (m_dirty_parts) clean_parts();
  return m_scale_orientation;
}

//! \brief sets the center of rotation.
void Transform::set_center(float v0, float v1, float v2)
{
  if (m_dirty_parts) clean_parts();
  m_center[0] = v0;
  m_center[1] = v1;
  m_center[2] = v2;
  parts_changed(get_field_info(CENTER));
}

//! \brief obtains the center of rotation.
void Transform::get_center(float& v0, float& v1, float& v2)
{
  if (m_dirty_parts) clean_parts();
  v0 = m_center[0];
  v1 = m_center[1];
  v2 = m_center[2];
}

//! \brief sets the center of rotation.
void Transform::set_center(const Vector3f& center)
{ set_center(center[0], center[1], center[2]); }

//! \brief obtains the center of rotation.
const Vector3f& Transform::get_center()
{
  if (m_dirty_parts) clean_parts();
  return m_center;
}

/*! \brief raises the matrix dirty flag, and sets the flag that indicates that
 * rendering is required.
 */
void Transform::parts_changed(const Field_info* field_info)
{
  m_dirty_matrix = true;
  m_dirty_inverse = true;
  m_dirty_bounding_sphere = true;
  set_rendering_required();
  field_changed(field_info);
}

/*! \brief culls the transform node.
 * If this node should be rendered, its matrices is passed to cull_context.
 */
void Transform::cull(Cull_context& cull_context)
{
  if (! is_visible()) return;
  if (m_dirty_matrix) clean_matrix();
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
void Transform::reset(const Field_info* /* field_info */)
{
  set_translation(s_def_translation);
  set_rotation(s_def_rotation);
  set_scale(s_def_scale);
  set_center(s_def_center);
  set_scale_orientation(s_def_scale_orientation);
}

/*! \brief cleans the bounding sphere of the transformation node.
 * Notice that the a call to clean_matrix() must precede the call to
 * Group::clean_bounding_sphere(), cause the former may reset the flag
 * m_dirty_bounding_sphere.
 */
void Transform::clean_bounding_sphere()
{
  if (m_locked_bounding_sphere) {
    m_dirty_bounding_sphere = false;
    return;
  }

  if (m_dirty_matrix) clean_matrix();
  Group::clean_bounding_sphere();

  // transform the center of the boundng sphere
  Vector3f new_center;
  Vector3f current_center = m_bounding_sphere.get_center();
  new_center.xform_pt(current_center, m_matrix);
  m_bounding_sphere.set_center(new_center);

  // multiply the radius by the max scaling factor
  float scale_factor = m_scale.max();
  m_bounding_sphere.set_radius(m_bounding_sphere.get_radius() * scale_factor);
}

//! \brief initializes the node prototype.
void Transform::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func = static_cast<Execution_function>(&Transform::parts_changed);

  // center
  auto center_func =
    static_cast<Vector3f_handle_function>(&Transform::center_handle);
  s_prototype->add_field_info(new SF_vector3f(CENTER, "center",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              center_func,
                                              s_def_center, exec_func));

  // translation
  auto translation_func =
    static_cast<Vector3f_handle_function>(&Transform::translation_handle);
  s_prototype->add_field_info(new SF_vector3f(TRANSLATION, "translation",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              translation_func,
                                              s_def_translation, exec_func));

  // rotation
  auto rotation_func =
    static_cast<Rotation_handle_function>(&Transform::rotation_handle);
  s_prototype->add_field_info(new SF_rotation(ROTATION, "rotation",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              rotation_func, s_def_rotation,
                                              exec_func));

  // scale
  auto scale_func =
    static_cast<Vector3f_handle_function>(&Transform::scale_handle);
  s_prototype->add_field_info(new SF_vector3f(SCALE, "scale",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              scale_func,
                                              s_def_scale, exec_func));

  // reset
  exec_func = static_cast<Execution_function>(&Transform::reset);
  auto reset_func =
    static_cast<Boolean_handle_function>(&Transform::reset_handle);
  s_prototype->add_field_info(new SF_bool(RESET, "reset",
                                          Field_rule::RULE_IN,
                                          reset_func,
                                          exec_func));
}

//! \brief deletes the node prototype.
void Transform::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
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

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
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
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

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
