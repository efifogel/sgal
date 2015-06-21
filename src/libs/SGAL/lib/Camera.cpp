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
#include <algorithm>
#include <string>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Bounding_sphere.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Camera::s_tag = "Viewpoint";
Container_proto* Camera::s_prototype(nullptr);

// Defaults values:
const Vector3f Camera::s_def_position(0, 0, 0);
const Rotation Camera::s_def_orientation(0, 0, 1, 0);
const float Camera::s_def_field_of_view(0.785398f);    // 45 degrees
Frustum Camera::s_def_frustum;
const std::string Camera::s_def_description("");
const Float Camera::s_def_radius_scale(1.1f);
const Float Camera::s_def_far_plane_scale(64);
const Vector3f Camera::s_def_line_of_sight(0, 0, 1);
const Vector3f Camera::s_def_up(0, 1, 0);

REGISTER_TO_FACTORY(Camera, "Camera");

//! \brief construct.
Camera::Camera(Boolean proto) :
  Bindable_node(proto),
  m_scene_graph(nullptr),
  m_is_dynamic(true),
  m_position(s_def_position),
  m_orientation(s_def_orientation),
  m_line_of_sight(s_def_line_of_sight),
  m_up(s_def_up),
  m_xaxis(1, 0, 0),
  m_yaxis(0, 1, 0),
  m_zaxis(0, 0, 1),
  m_view_mat(),
  m_frustum(s_def_frustum),
  m_field_of_view(s_def_field_of_view),
  m_nearest_clipping_plane(0.1f),
  m_radius_scale(s_def_radius_scale),
  m_far_plane_scale(s_def_far_plane_scale),
  m_dirty_axes(false),
  m_dirty_matrix(true),
  m_description("")
{}

//! \brief destruct.
Camera::~Camera() {}

//! \brief sets the camera relative position.
void Camera::set_position(const Vector3f& position)
{
  m_position = position;
  m_dirty_matrix = true;
}

//! \brief sets the camera relative orientation.
void Camera::set_orientation(const Rotation& orientation)
{
  SGAL_assertion_msg((orientation.get_axis()[0] != 0) ||
                     (orientation.get_axis()[1] != 0) ||
                     (orientation.get_axis()[2] != 0),
                     "Camera: Invalid orientation!");
  m_orientation = orientation;
  m_dirty_matrix = true;
}

//! \brief sets The near and far clipping planes of the frustum.
void Camera::set_clipping_planes(float near_plane, float far_plane)
{
  m_frustum.set_near(near_plane);
  m_frustum.set_far(far_plane);
}

//! \brief sets the camera viewing transformation.
void Camera::look_at(const Vector3f& eye, const Vector3f& target,
                     const Vector3f& up)
{
  m_eye.set(eye);
  m_zaxis.sub(eye, target);
  m_zaxis.normalize();
  m_xaxis.cross(up, m_zaxis);
  m_xaxis.normalize();
  m_yaxis.cross(m_zaxis, m_xaxis);
  m_dirty_matrix = true;
}

//! \brief sets the camera viewing transformation.
void Camera::view(const Vector3f& eye, Float yaw, Float pitch)
{
  m_dirty_matrix = true;
}

/*! \brief sets the clipping planes so that the frustum contains the
 * bounding-sphere.
 */
void Camera::set_clipping_planes(const Vector3f& target, Float bb_radius)
{
  float radius = bb_radius * m_radius_scale;
  float my_sin = sinf(m_field_of_view / 2);
  float dist = radius / my_sin;

  float near_plane = dist - radius;
  if (near_plane < m_nearest_clipping_plane) {
    near_plane = m_nearest_clipping_plane;
    dist = near_plane + radius;
  }
  float far_plane = dist + radius;

  near_plane = m_nearest_clipping_plane;
  far_plane = dist * m_far_plane_scale;

  Vector3f eye;
  eye.add_scaled(target, dist, m_line_of_sight);
  look_at(eye, target, m_up);

  m_frustum.set_near(near_plane);
  m_frustum.set_far(far_plane);
}

/*! \brief sets the camera to view the scene.
 */
void Camera::set_dynamic_clipping_planes()
{
  if (!m_is_dynamic) return;

  Scene_graph::Shared_transform nav_root = m_scene_graph->get_navigation_root();
  const Bounding_sphere& sb = nav_root->get_bounding_sphere();
  set_clipping_planes(sb.get_center(), sb.get_radius());
}

//! \brief sets the frustum field of view angle.
void Camera::set_field_of_view(float fov)
{
  m_field_of_view = fov;
  m_frustum.set_fov(m_field_of_view);
  if (m_is_dynamic) m_dirty_matrix = true;
}

//! \brief obtains the field-of-view
float Camera::get_field_of_view() { return m_field_of_view; }

//! \brief processes change of viewing components.
void Camera::components_changed(const Field_info* /* info */)
{ m_dirty_matrix = true; }

//! \brief processes change of field-of-view.
void Camera::field_of_view_changed(const Field_info* /* info */)
{
  m_frustum.set_fov(m_field_of_view);
  if (m_is_dynamic) m_dirty_matrix = true;
}

//! \brief obtains the camera near and far clipping planes.
void Camera::get_clipping_planes(float& near_plane, float& far_plane)
{ m_frustum.get_near_far(near_plane, far_plane); }

/*! \brief initialize some camera parameters. Cannot be called from the
 * constructor, but does not require a scene graph nor a context. In
 * particular, initialize the nearest clipping plane based on the
 * accelerator type. This is a work around a bug in some of the old
 * accelerators
 */
void Camera::utilize()
{
  Gfx_conf* gfx_conf = Gfx_conf::get_instance();
  if (!gfx_conf) return;

  Gfx_conf::Vendor vendor = gfx_conf->get_vendor();
  Gfx_conf::Renderer renderer = gfx_conf->get_renderer();
  if ((vendor == Gfx_conf::ve3Dfx) ||
      (renderer == Gfx_conf::reG400) ||
      (renderer == Gfx_conf::reG200) ||
      (renderer == Gfx_conf::reRADEON_IGP) ||
      (renderer == Gfx_conf::reGEFORCE_6200))
  { m_nearest_clipping_plane = 1; }
}

//! \brief updates the aspect ratio based on the context.
void Camera::set_aspect_ratio(const Context* context)
{
  if (context) {
    float ratio = context->get_aspect_ratio();
    m_frustum.set_aspect_ratio(ratio);
  }
}

//! \brief initializes the container prototype.
void Camera::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Bindable_node::get_prototype());

  // Add the object fields to the prototype
  // position
  auto exec_func = static_cast<Execution_function>(&Camera::components_changed);
  auto position_func =
    static_cast<Vector3f_handle_function>(&Camera::position_handle);
  s_prototype->add_field_info(new SF_vector3f(POSITION, "position",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              position_func, s_def_position,
                                              exec_func));

  // orientation
  auto orientation_func =
    static_cast<Rotation_handle_function>(&Camera::orientation_handle);
  s_prototype->add_field_info(new SF_rotation(ORIENTATION, "orientation",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              orientation_func,
                                              s_def_orientation, exec_func));

  // fieldOfView
  exec_func = static_cast<Execution_function>(&Camera::field_of_view_changed);
  auto fov_func = static_cast<Float_handle_function>(&Camera::fov_handle);
  s_prototype->add_field_info(new SF_float(FIELDOFVIEW, "fieldOfView",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           fov_func, s_def_field_of_view,
                                           exec_func));

  // description
  auto description_func =
    static_cast<String_handle_function>(&Camera::description_handle);
  s_prototype->add_field_info(new SF_string(DESCRIPTION, "description",
                                            Field_info::RULE_FIELD,
                                            description_func));

  // radiusScale
  auto radius_scale_func =
    static_cast<Float_handle_function>(&Camera::radius_scale_handle);
  s_prototype->add_field_info(new SF_float(RADIUS_SCALE, "radiusScale",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           radius_scale_func,
                                           s_def_radius_scale));
}

//! \brief deletes the camera prototype.
void Camera::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the camera prototype.
Container_proto* Camera::get_prototype()
{
  if (s_prototype == nullptr) Camera::init_prototype();
  return s_prototype;
}

//! \brief obtain the camera viewing matrix.
const Matrix4f& Camera::get_view_mat()
{
  if (m_dirty_axes) clean_axes();
  if (m_dirty_matrix) clean_matrix();
  return m_view_mat;
}

//! \brief cleans the camera viewing axes.
void Camera::clean_axes()
{
  m_dirty_axes = false;
}

//! \brief cleans the camera viewing matrix.
void Camera::clean_matrix()
{
  m_view_mat.make_identity();
  m_view_mat.set_col(0, m_xaxis);
  m_view_mat.set_col(1, m_yaxis);
  m_view_mat.set_col(2, m_zaxis);

  // rotation:
  const auto& newz = m_orientation.get_axis();      // asured normalized
  const auto angle = m_orientation.get_angle();
  m_view_mat.post_rot(m_view_mat, newz[0], newz[1], newz[2], -angle);

  // Position
  Vector3f new_pos;
  new_pos.add(m_eye, m_position);
  new_pos.scale(-1.0f);
  new_pos.xform_pt(new_pos, m_view_mat);
  m_view_mat.set_row(3, new_pos);

  m_dirty_matrix = false;
}

//! \brief applies the camera.
void Camera::draw(Draw_action* action)
{
  const auto* conf = action->get_configuration();
  if (conf) {
    Configuration::Shared_accumulation acc = conf->get_accumulation();
    if (acc && acc->is_enabled() && acc->is_active()) {
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
      float xjitter, yjitter;
      acc->get_jitter(xjitter, yjitter);
      float xpert = xjitter / viewport[2];
      float ypert = yjitter / viewport[3];
      m_frustum.set_perturbation_scale(xpert, ypert);
    }
  }

  draw();
}

//! \brief applies the camera.
void Camera::draw(Isect_action* /* action */) { draw(); }

//! \brief applies the camera.
void Camera::draw()
{
  m_frustum.apply();
  if (m_dirty_matrix) clean_matrix();

  glMatrixMode(GL_MODELVIEW);
  if (Gfx_conf::get_instance()->get_renderer() != Gfx_conf::reGeneric) {
    glLoadMatrixf((GLfloat*) &m_view_mat);
  }
  else {
    // It looks like there is a bug related to lights in the soft version
    // of opengl. Therefore, if we recognize the driver as software, we
    // use a non visualy apperant scale to work around the bug.
    glLoadIdentity();
    glScalef(1.001f, 1.001f, 1.001f);
    glMultMatrixf((GLfloat*) &m_view_mat);
  }
}

//! \brief sets the attributes of the object extracted from the input file.
void Camera::set_attributes(Element* elem)
{
  Bindable_node::set_attributes(elem);
  m_frustum.set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "fieldOfView") {
      // m_is_dynamic = false;
      set_field_of_view(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "orientation") {
      Rotation rot(value);
      set_orientation(rot);
      m_is_dynamic = false;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "position") {
      Vector3f vec(value);
      m_is_dynamic = false;
      set_position(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "description") {
      m_description = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "radiusScale") {
      m_radius_scale = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Camera::add_to_scene(Scene_graph* sg)
{
  set_scene_graph(sg);
  insert_stack(this);
}

//! \brief obtains the bindable stack.
Bindable_stack* Camera::get_stack()
{ return m_scene_graph->get_camera_stack(); }

//! \brief enables the camera---called when the camera is bound.
void Camera::enable()
{
  utilize();
  init();
}

//! \brief initializes the camera based on the current context.
void Camera::init()
{
  const auto* conf = m_scene_graph->get_configuration();
  if (Configuration::VM_MODELING == conf->get_viewpoint_mode()) {
    Vector3f up(0, 0, 1);
    Vector3f line_of_sight(0, -1, 0);
    set_viewpoint(line_of_sight, up);
  }
  const auto* context = m_scene_graph->get_context();
  set_aspect_ratio(context);
  set_dynamic_clipping_planes();
}

//! \brief sets the viewpoint line-of-sight and up default vectors.
void Camera::set_viewpoint(const Vector3f& line_of_sight, const Vector3f& up)
{
  m_line_of_sight.set(line_of_sight);
  m_up.set(up);
  m_dirty_matrix = true;
}

SGAL_END_NAMESPACE
