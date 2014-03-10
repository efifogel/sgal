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
#include "SGAL/Sphere_bound.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Camera::s_tag = "Viewpoint";
Container_proto* Camera::s_prototype(NULL);

// Defaults values:
const Vector3f Camera::s_def_position(0, 0, 10);
const Rotation Camera::s_def_orientation(0, 0, 1, 0);
const float Camera::s_def_field_of_view(0.785398f);    // 45 degrees
Frustum Camera::s_def_frustum;
const std::string Camera::s_def_description("");
const Float Camera::s_def_radius_scale(1.1f);
const Float Camera::s_def_far_plane_scale(64);

REGISTER_TO_FACTORY(Camera, "Camera");

/*! Constructor */
Camera::Camera(Boolean proto) :
  Bindable_node(proto),
  m_scene_graph(0),
  m_is_dynamic(true),
  m_position(s_def_position),
  m_orientation(s_def_orientation),
  m_view_mat(),
  m_frustum(s_def_frustum),
  m_field_of_view(s_def_field_of_view),
  m_dirty_matrix(true),
  m_nearest_clipping_plane(0.1f),
  m_radius_scale(s_def_radius_scale),
  m_far_plane_scale(s_def_far_plane_scale),
  m_position_translation(0,0,0),
  m_description("")
{}

/*! Destructor */
Camera::~Camera() {}

/*! \brief sets the camera position. */
void Camera::set_position(const Vector3f& position)
{
  m_position = position;
  m_dirty_matrix = true;
}

/*! \brief sets the camera orientation. */
void Camera::set_orientation(const Rotation& orientation)
{
  const Vector3f& axis = orientation.get_axis();
  if ((axis[0] == 0) && (axis[1] == 0) && (axis[2] == 0)) {
    SGAL_warning_msg(0, "Camera: Invalid orientation!");
    return;
  }
  m_orientation = orientation;
  m_dirty_matrix = true;
}

/*! \brief */
void Camera::set_clipping_planes(float near_plane, float far_plane)
{
  m_frustum.set_near(near_plane);
  m_frustum.set_far(far_plane);
}

/*! \brief sets the clipping planes so that the frustum contains the
 * bounding-sphere.
 */
void Camera::set_clipping_planes(const Vector3f& bb_center, Float bb_radius)
{
  float radius = bb_radius * m_radius_scale;
  float my_sin = sinf(m_field_of_view / 2);
  float dist = radius / my_sin;

  //! \todo Introduce a user-option that makes the frustum tight
  float near_plane = dist - radius;
  if (near_plane < m_nearest_clipping_plane) {
    near_plane = m_nearest_clipping_plane;
    dist = near_plane + radius;
  }
  float far_plane = dist + radius;

  near_plane = m_nearest_clipping_plane;
  far_plane = dist * m_far_plane_scale;

  Vector3f los(0,0,1);
  los.scale(dist);
  m_position.add(bb_center, los);
  m_position.add(m_position_translation);

  m_frustum.set_near(near_plane);
  m_frustum.set_far(far_plane);
}

/*! \brief sets the cliping plane so the the frustum contains the
 * bounding-sphere of the current scene, if the dynamic flag is raised
 */
void Camera::set_dynamic_clipping_planes()
{
  if (!m_is_dynamic) return;

  Scene_graph::Shared_transform nav_root = m_scene_graph->get_navigation_root();
  const Sphere_bound& sb = nav_root->get_sphere_bound();
  set_clipping_planes(sb.get_center(), sb.get_radius());
}

/*! \brief */
void Camera::set_field_of_view(float fov)
{
  m_field_of_view = fov;
  m_frustum.set_fov(m_field_of_view);
}

/*! \brief */
float Camera::get_field_of_view() { return m_field_of_view; }

/*! \brief */
void Camera::update_field_of_view(Field_info* /* info */)
{
  m_frustum.set_fov(m_field_of_view);
  set_rendering_required();
}

/*! \brief */
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

/*! \brief updates the aspect ratio based on the context. */
void Camera::set_aspect_ratio(const Context* context)
{
  if (context) {
    float ratio = context->get_aspect_ratio();
    m_frustum.set_aspect_ratio(ratio);
  }
}

/*! \brief */
void Camera::update_matrix_requiered(Field_info* /* info */)
{
#if 0
  m_dirty_matrix = true;
  set_rendering_required();
  if (m_is_bound)
    m_execution_coordinator->set_current_view_calculation_required();
#endif
}

/*! \brief initializes the container prototype. */
void Camera::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Bindable_node::get_prototype());

  // Add the object fields to the prototype
  // position
  Execution_function exec_func =
    static_cast<Execution_function>(&Camera::update_matrix_requiered);
  Vector3f_handle_function position_func =
    static_cast<Vector3f_handle_function>(&Camera::position_handle);
  s_prototype->add_field_info(new SF_vector3f(POSITION, "position",
                                              RULE_EXPOSED_FIELD,
                                              position_func, exec_func));

  // orientation
  exec_func =
    static_cast<Execution_function>(&Camera::update_matrix_requiered);
  Rotation_handle_function orientation_func =
    static_cast<Rotation_handle_function>(&Camera::orientation_handle);
  s_prototype->add_field_info(new SF_rotation(ORIENTATION, "orientation",
                                              RULE_EXPOSED_FIELD,
                                              orientation_func, exec_func));

  // fieldOfView
  exec_func =
    static_cast<Execution_function>(&Camera::update_field_of_view);
  Float_handle_function fov_func =
    static_cast<Float_handle_function>(&Camera::fov_handle);
  s_prototype->add_field_info(new SF_float(FIELDOFVIEW, "fieldOfView",
                                           RULE_EXPOSED_FIELD,
                                           fov_func, exec_func));

  // description
  String_handle_function description_func =
    static_cast<String_handle_function>(&Camera::description_handle);
  s_prototype->add_field_info(new SF_string(DESCRIPTION, "description",
                                            RULE_FIELD,
                                            description_func));

  // radiusScale
  Float_handle_function radius_scale_func =
    static_cast<Float_handle_function>(&Camera::radius_scale_handle);
  s_prototype->add_field_info(new SF_float(RADIUS_SCALE, "radiusScale",
                                           RULE_EXPOSED_FIELD,
                                           radius_scale_func));
}

/*! \brief deletes the camera prototype. */
void Camera::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the camera prototype. */
Container_proto* Camera::get_prototype()
{
  if (s_prototype == NULL) Camera::init_prototype();
  return s_prototype;
}

/*! \brief obtain the camera viewing matrix. */
const Matrix4f& Camera::get_view_mat()
{
  if (m_dirty_matrix) clean_matrix();
  return m_view_mat;
}

/*! \brief cleans the camera viewing matrix. */
void Camera::clean_matrix()
{
  m_view_mat.make_identity();

  // rotation:
  Vector3f newz;
  m_orientation.get_axis(newz);      // asured normalized
  float angle = m_orientation.get_angle();
  m_view_mat.post_rot(m_view_mat, newz[0], newz[1], newz[2], -angle);

  Vector3f new_pos;
  new_pos.scale(-1.0f, m_position);
  new_pos.xform_pt(new_pos, m_view_mat);
  m_view_mat.set_row(3, new_pos);

  m_dirty_matrix = false;
}

/*! \brief applies the camera. */
void Camera::draw(Draw_action* action)
{
  Configuration* conf = action->get_configuration();
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

/*!  \brief applies the camera. */
void Camera::draw(Isect_action* /* action */) { draw(); }

/*!  \brief applies the camera. */
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

/*! \brief sets the attributes of the object extracted from the input file */
void Camera::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  m_frustum.set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "fieldOfView") {
      // m_is_dynamic = false;
      set_field_of_view(atoff(value.c_str()));
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
      m_radius_scale = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "positionTranslation") {
      Vector3f vec(value);
      m_position_translation.set(vec);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene */
void Camera::add_to_scene(Scene_graph* sg)
{
  set_scene_graph(sg);

  // Push this bindable Navigation_info node onto the respective stack:
  insert_stack(this);
}

#if 0
/*!
 */
Attribute_list Camera::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  char buf[32];

  attribs = Container::get_attributes();

  if (m_field_of_view != s_def_field_of_view) {
    attrib.first = "field_of_view";
    sprintf(buf, "%g",   m_field_of_view);
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_orientation != s_def_orientation) {
    attrib.first = "orientation";
    attrib.second = get_orientation().get_text();
    attribs.push_back(attrib);
  }

  if (m_position != s_def_position) {
    attrib.first = "position";
    attrib.second = get_position().get_text();
    attribs.push_back(attrib);
  }

  if (m_description != s_def_description) {
    attrib.first = "description";
    attrib.second = m_description;
    attribs.push_back(attrib);
  }

  return attribs;
}

/*! \brief */
void Camera::add_to_scene(Scene_graph* sg, XML_entity* parent)
{
  Container::add_to_scene(sg, parent);
  m_camera_pool = sg->get_camera_pool();

  if (m_camera_pool) m_camera_pool->add_camera(this);
  if (sg) init(sg->get_context());
}
#endif

/*! \brief obtains the bindable stack. */
Bindable_stack* Camera::get_stack()
{ return m_scene_graph->get_camera_stack(); }

/*! \brief enables the camera---called when the camera is bound */
void Camera::enable()
{
  utilize();
  const Context* context = m_scene_graph->get_context();
  init(context);
}

/*! \brief initializes the camera based on the current context. */
void Camera::init(const Context* context)
{
  set_aspect_ratio(context);
  // Update the frustum to contain the model:
  set_dynamic_clipping_planes();
}

SGAL_END_NAMESPACE
