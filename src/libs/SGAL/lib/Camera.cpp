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
// $Source$
// $Revision: 14220 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <algorithm>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Math_defs.hpp"
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
Container_proto* Camera::s_prototype = 0;

// Defaults values:
const Vector3f Camera::s_def_position(0, 0, 10);
const Rotation Camera::s_def_orientation(0, 0, 1, 0);
const float Camera::s_def_field_of_view = 0.785398f;    // 45 degrees
Frustum Camera::s_def_base_frust;
const std::string Camera::s_def_description("");
const Float Camera::s_def_radius_scale = 1.1f;
const Float Camera::s_def_far_plane_scale = 64;

REGISTER_TO_FACTORY(Camera, "Camera");

/*! Constructor */
Camera::Camera(Boolean proto) :
  Bindable_node(proto),
  m_scene_graph(0),
  m_is_dynamic(SGAL_TRUE),
  m_position(s_def_position),
  m_orientation(s_def_orientation),
  m_view_mat(),
  m_base_frust(s_def_base_frust),
  m_field_of_view(s_def_field_of_view),
  m_dirty_matrix(true),
  m_nearest_clipping_plane(0.1f),
  m_radius_scale(s_def_radius_scale),
  m_far_plane_scale(s_def_far_plane_scale),
  m_position_translation(0,0,0),
  m_description("")
{
}

/*! Destructor */
Camera::~Camera() {}

/*! Set the camera position
 */
void Camera::set_position(const Vector3f& position)
{
  m_position = position;
  m_dirty_matrix = true;
}

/*! Set the camera orientation
 */
void Camera::set_orientation(const Rotation& orientation)
{
  m_orientation = orientation;
  m_dirty_matrix = true;
}

/*!
 */
void Camera::set_clipping_planes(float near_plane, float far_plane)
{
  m_base_frust.set_near(near_plane);
  m_base_frust.set_far(far_plane); 
}

/*! Set the clipping planes so that the frustum contains the bounding-sphere
 * \param bb_center the center of the bounding sphere
 * \param bb_radius the radius of the bounding sphere
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
  
  m_base_frust.set_near(near_plane);
  m_base_frust.set_far(far_plane);
}

/*! Set the cliping plane so the the frustum contains the bounding-sphere
 * of the current scene, if the dynamic flag is raised
 */
void Camera::set_dynamic_clipping_planes()
{
  if (!m_is_dynamic) return;
  
  Transform* nav_root = m_scene_graph->get_navigation_root();
  const Sphere_bound& sb = nav_root->get_sphere_bound();
  set_clipping_planes(sb.get_center(), sb.get_radius());
}

/*!
 */
void Camera::set_field_of_view(float fov)
{
  m_field_of_view = fov;
  m_base_frust.set_fov(m_field_of_view);
}

/*!
 */
float Camera::get_field_of_view()
{
  return m_field_of_view;
}


/*!
 */
void Camera::update_field_of_view(Field_info* /* info */)
{
  m_base_frust.set_fov(m_field_of_view);
  set_rendering_required();
}

/*!
 */
void Camera::get_clipping_planes(float& near_plane, float& far_plane)
{
  m_base_frust.get_near_far(near_plane, far_plane);
}

/*! Initialize some camera parameters. Cannot be called from the constructor,
 * but does not require a scene graph nor a context. In particular, initialize
 * the nearest clipping plane based on the accelerator type. This is a work
 * around a bug in some of the old accelerators
 */
void Camera::utilize()
{
  Gfx_conf* gfx_conf = Gfx_conf::get_instance();
  if (!gfx_conf) return;

  Gfx_conf::Vendor vendor = gfx_conf->get_vendor();
  Gfx_conf::Renderer renderer = gfx_conf->get_renderer();
  if (vendor == Gfx_conf::ve3Dfx ||
      renderer == Gfx_conf::reG400 ||
      renderer == Gfx_conf::reG200 ||
      renderer == Gfx_conf::reRADEON_IGP ||
      renderer == Gfx_conf::reGEFORCE_6200)
  {
    m_nearest_clipping_plane = 1;
  }
}

/*! Update the aspect ratio based on the context */
void Camera::set_aspect_ratio(const Context* context)
{
  if (context) {
    float ratio = context->get_aspect_ratio();
    m_base_frust.set_aspect_ratio(ratio);
  }
}

/*!
 */
void Camera::update_matrix_requiered(Field_info* /* info */)
{
#if 0
  m_dirty_matrix = true;
  set_rendering_required();
  if (m_is_bound)
    m_execution_coordinator->set_current_view_calculation_required();
#endif
}

/*!
 */
void Camera::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Bindable_node::get_prototype());

  // Add the object fields to the prototype
  Execution_function exec_func =
    static_cast<Execution_function>(&Camera::update_matrix_requiered);
  s_prototype->add_field_info(new SF_vector3f(POSITION, "position",
                                              get_member_offset(&m_position),
                                              exec_func));
  exec_func =
    static_cast<Execution_function>(&Camera::update_matrix_requiered);
  s_prototype->add_field_info(new SF_rotation(ORIENTATION, "orientation",
                                              get_member_offset(&m_orientation),
                                              exec_func));

  exec_func =
    static_cast<Execution_function>(&Camera::update_field_of_view);
  s_prototype->add_field_info(new SF_float(FIELDOFVIEW, "fieldOfView",
                                           get_member_offset(&m_field_of_view),
                                           exec_func));

  s_prototype->add_field_info(new SF_string(DESCRIPTION, "description",
                                            get_member_offset(&m_description)));

  s_prototype->add_field_info(new SF_float(RADIUS_SCALE, "radiusScale",
                                           get_member_offset(&m_radius_scale)));
}

/*!
 */
void Camera::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*!
 */
Container_proto* Camera::get_prototype() 
{  
  if (s_prototype == NULL) Camera::init_prototype();
  return s_prototype;
}

/*!
 */
const Matrix4f& Camera::get_view_mat()
{
  if (m_dirty_matrix) clean_matrix();
  return m_view_mat;
}

/*!
 */
void Camera::clean_matrix()
{
#if 1
  Vector3f axis;
  m_orientation.get_axis(axis);
  float angle = m_orientation.get_angle();
  m_view_mat.make_rot(axis, 2 * SGAL_PI - angle);
  Vector3f new_pos;
  new_pos.scale(-1.0f, m_position);
  new_pos.xform_pt(new_pos, m_view_mat);
  m_view_mat.set_row(3, new_pos);
#else
  Vector3f newz;
  m_orientation.get_axis(newz);      // asured normalized

  Vector3f newx;
  Vector3f newy;

  float angle = m_orientation.get_angle();

  if (newz[0] == 1.0f) {
    // the camera points twards the X axis:
    newx.set(0.0f, 0.0f, -1.0f);
    newy.set(0.0f, -1.0f, 0.0f);
    newz[0] = -1.0f;
  } else if (newz[0] == -1.0f) {
    // the camera points twards the -X axis:
    newx.set(0.0f, 0.0f, 1.0f);
    newy.set(0.0f, -1.0f, 0.0f);
    newz[0] = 1.0f;
  } else if (newz[1] == 1.0f) {
    // the camera points twards the Y axis:
    newx.set(-1.0f, 0.0f, 0.0f);
    newy.set(0.0f, 0.0f, -1.0f);
    newz[1] = -1.0f;
  } else if (newz[1] == -1.0f) {
    // the camera points twards the -Y axis:
    newx.set(-1.0f, 0.0f, 0.0f);
    newy.set(0.0f, 0.0f, 1.0f);
    newz[1] = 1.0f;
  } else if (newz[2] == 1.0f) {
    // the camera points twards the Z axis:
    newx.set(-1.0f, 0.0f, 0.0f);
    newy.set(0.0f, 1.0f, 0.0f);
    newz[2] = -1.0f;
  } else if (newz[2] == -1.0f) {
    // the camera points twards the -Z axis:
    newx.set(1.0f, 0.0f, 0.0f);
    newy.set(0.0f, 1.0f, 0.0f);
    newz[2] = 1.0f;
  } else {
    Vector3f up(0.0f, 1.0f, 0.0f);
    newz.scale(-1.0f, newz);
    newx.cross(up, newz);
    newx.normalize();

    // compute the new, real (corrected) up vector
    newy.cross(newz, newx);
    newy.normalize();
  }

  m_view_mat.make_identity();

  m_view_mat.set_col(0, newx);
  m_view_mat.set_col(1, newy);
  m_view_mat.set_col(2, newz);

  // rotation:
  m_view_mat.post_rot(m_view_mat, 0, 0, 1, angle);

  // position:
  Vector3f new_pos;
  new_pos.scale(-1.0f, m_position);
  new_pos.xform_pt(new_pos, m_view_mat);
  m_view_mat.set_row(3, new_pos);
#endif
  m_dirty_matrix = false;
}  

/*! Apply the camera */
void Camera::draw(Draw_action* action)
{
  Configuration* conf = action->get_configuration();
  Accumulation* acc = NULL;
  if (conf) acc = conf->get_accumulation();

  if (acc && acc->is_enabled() && acc->is_active()) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float xjitter, yjitter;
    acc->get_jitter(xjitter, yjitter);
    float xpert = xjitter / viewport[2];
    float ypert = yjitter / viewport[3];
    m_base_frust.set_perturbation_scale(xpert, ypert);
  }
  
  draw();
}

/*! Apply the camera */
void Camera::draw(Isect_action* /* action */) { draw(); }

/*! Apply the camera */
void Camera::draw()
{
  m_base_frust.apply();
  if (m_dirty_matrix) clean_matrix();
  
  glMatrixMode(GL_MODELVIEW);
  if (Gfx_conf::get_instance()->get_renderer() != Gfx_conf::reGeneric) {
    glLoadMatrixf((GLfloat*) &m_view_mat);
  } else {
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
  m_base_frust.set_attributes(elem);
  
  typedef Element::Str_attr_iter                Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
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

/*!
 */
void Camera::add_to_scene(Scene_graph* sg, XML_entity* parent)
{
  Container::add_to_scene(sg, parent);
  m_camera_pool = sg->get_camera_pool();

  if (m_camera_pool)
    m_camera_pool->add_camera(this);
  if (sg)
    init(sg->get_context());
}
#endif

/*! Obtains the bindable stack */
Bindable_stack* Camera::get_stack()
{
  return m_scene_graph->get_camera_stack();
}

/*! \bried enable the camera - called when the camera is bound */
void Camera::enable()
{
  utilize();
  const Context* context = m_scene_graph->get_context();
  init(context);
}

/*! Initialize the camera based on the current context */
void Camera::init(const Context* context)
{
  set_aspect_ratio(context);
  // Update the frustum to contain the model:
  set_dynamic_clipping_planes();
}

SGAL_END_NAMESPACE
