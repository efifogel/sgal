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

/*!
 * View_sensor - implementation
 */

#include "SGAL/View_sensor.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Camera_pool.hpp"
#include "SGAL/Navigation_sensor.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Single_key_sensor::s_tag = "ViewSensor";
Container_proto* View_sensor::s_prototype(NULL);

REGISTER_TO_FACTORY(View_sensor, "View_sensor");

/*! Constructor */
View_sensor::View_sensor(Camera_pool* camera_pool,
                         Transform* navigation_root,
                         Navigation_sensor* navigation_sensor,
                         Boolean proto) :
  Node(proto),
  m_translation(0.0, 0.0, 0.0),
  m_rotation(0.0, 0.0, 1.0, 0.0),
  m_camera_pool(camera_pool),
  m_navigation_root(navigation_root),
  m_navigation_sensor(navigation_sensor),
  m_local_view_name(""),
  m_current_view_name(""),
  m_local_view_name_set(false),
  m_last_camera(NULL)
{}

/*! \brief initializes the view sensor. */
void View_sensor::init(Camera_pool* camera_pool,
                        Transform* navigation_root,
                        Navigation_sensor* navigation_sensor)
{
  m_camera_pool = camera_pool;
  m_navigation_root = navigation_root;
  m_navigation_sensor = navigation_sensor;
}

/*! Activated every application loop.
 * Calculates and cascades the scene current view translation and rotation.
 */
Boolean View_sensor::update()
{
  // if (!m_executionCoordinator->IsCurrentViewCalculationRequired())
  //   return true;

  // Reset the CurrentViewCalculationRequired flag
  // m_executionCoordinator->SetCurrentViewCalculationRequired(false);

  SGAL_assertion(m_navigation_root);
  SGAL_assertion(m_camera_pool);

  // Get the current camera
  Camera* camera = m_camera_pool->get_active_camera();
  SGAL_assertion(camera);

  if (m_last_camera != camera) {
    m_current_view_name = camera->get_description();
    Field* cur_view_field = get_field(CURRENTVIEWNAME);
    if (cur_view_field) cur_view_field->Cascade();
    m_last_camera = camera;
  }

  if (!m_local_view_name_set && (m_local_view_name != "")) {
    Field* local_view_field = get_field(LOCALVIEWNAME);
    if (local_view_field) local_view_field->cascade();
    m_local_view_name_set = true;
  }

  // If the camera is a remote camera - reset the navigation sensor and dont
  // update the navigation sensor
  if (camera->get_remote()) {
    m_navigation_sensor->reset();
    return true;
  }

  // Get the navigation root matrix
  EMatrix4f view_matrix;
  m_navigation_root->get_matrix(view_matrix);

  // Get the current camera matrix
  const Matrix4f& camera_mat = camera->get_view_mat();

  // Multiply the navigation root and camera matrices
  view_matrix.mult(camera_mat, view_matrix);

  // Inverse the matrix
  view_matrix.invert_affine(view_matrix);

  // Create a temp transform with the view matrix
  Transform trans;
  trans.set_matrix(view_matrix);

  // Get the current view translation and rotation from the temp transform
  trans.get_translation(m_translation);
  trans.get_rotation(m_rotation);

  // Set the field of view to the current camera's field of view
  m_FOV = camera->get_field_of_view();

  // Cascade the view fields if needed
  Field* rot_field = get_field(ROTATION);
  if (rot_field) rot_field->Cascade();
  Field* trans_field = get_field(TRANSLATION);
  if (trans_field) trans_field->Cascade();
  Field* FOVField = get_field (FOV);
  if (FOVField) FOVField->Cascade();
  return true;
};

/*! initializes the node prototype */
void View_sensor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the object field infos to the prototype
  // translation
  Vector3f_handle_function translation_func =
    static_cast<Vector3f_handle_function>(&View_sensor::translation_handle);
  s_prototype->AddFieldInfo(new SF_vector3f(TRANSLATION, "translation",
                                            RULE_EXPOSED_FIELD,
                                            translation_func));

  // rotation
  _handle_function rotation_func =
    static_cast<_handle_function>(&View_sensor::rotation_handle);
  s_prototype->AddFieldInfo(new SF_rotation(ROTATION, "rotation",
                                            RULE_EXPOSED_FIELD,
                                            rotation_func));

  // fieldOfView
  _handle_function fov_func =
    static_cast<_handle_function>(&View_sensor::fov_handle);
  s_prototype->AddFieldInfo(new SF_float(FOV, "fieldOfView",
                                         RULE_EXPOSED_FIELD,
                                         fov_func));

  // localViewName
  _handle_function local_view_name_func =
    static_cast<_handle_function>(&View_sensor::local_view_name_handle);
  s_prototype->AddFieldInfo(new SF_string(LOCALVIEWNAME, "localViewName",
                                          RULE_EXPOSED_FIELD,
                                          local_view_name_func));

  // currentViewName
  _handle_function current_view_name_func =
    static_cast<_handle_function>(&View_sensor::current_view_name_handle);
  s_prototype-> AddFieldInfo(new SF_string(CURRENTVIEWNAME,
                                           "currentViewName",
                                           RULE_EXPOSED_FIELD,
                                           current_view_name_func));
}

/*! */
void View_sensor::delete_prototyp()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* View_sensor::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this container. */
void View_sensor::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
}

#if 0
/*! */
Attribute_list View_sensor::get_attributes()
{
  Attribute_list attribs;
  attribs = Container::get_attr_list();
  return attribs;
}
#endif

SGAL_END_NAMESPACE
