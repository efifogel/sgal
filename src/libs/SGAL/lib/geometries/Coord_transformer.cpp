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
 * \todo This node should be changed to have only one input field of type
 * MF_float through which a Matrix4f can be fed. The translation, rotation,
 * translated, and rotated fields should be eliminated. Then, a single
 * Coord_transformer node should be replaced with a Transform node and a
 * Coord_transformer node. The Transform node should be enhanced with a matrix
 * (input/output) field, and this field should be routed to the mtarix field of
 * the Coord_transformer.
 */
#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

#include "SGAL/Coord_transformer.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Transform.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Coord_transformer::s_tag = "CoordinateTransformer";

//! The node prototype.
Container_proto* Coord_transformer::s_prototype(nullptr);

const Boolean Coord_transformer::s_def_enabled = true;

/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Coord_transformer, "Coord_transformer");

//! \brief constructs.
Coord_transformer::Coord_transformer(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_reflect(false),
  m_execute(false),
  m_changed(false)
{}

//! \brief sets the attributes of this object.
void Coord_transformer::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "translation") {
      Vector3f vec(value);
      m_translation.set(vec);
      m_transform.set_translation(m_translation);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "scale") {
      Vector3f scale(value);
      m_scale.set(scale);
      m_transform.set_scale(m_scale);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "rotation") {
      Rotation rot(value);
      m_rotation.set(rot);
      m_transform.set_rotation(m_rotation);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "reflection") {
      m_reflect = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "enabled") {
      m_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "coord") {
      Shared_coord_array coord_array =
        boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief sets the attributes of this node.
void Coord_transformer::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  Execution_function exec_func;

  // Add the field-info records to the prototype:
  // enabled
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          enabled_func));

  // translation
  exec_func = static_cast<Execution_function>(&Coord_transformer::translate);
  Vector3f_handle_function translation_func =
    static_cast<Vector3f_handle_function>
    (&Coord_transformer::translation_handle);
  s_prototype->add_field_info(new SF_vector3f(TRANSLATION, "translation",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              translation_func, exec_func));

  // rotation
  exec_func = static_cast<Execution_function>(&Coord_transformer::rotate);
  Rotation_handle_function rotation_func =
    static_cast<Rotation_handle_function>(&Coord_transformer::rotation_handle);
  s_prototype->add_field_info(new SF_rotation(ROTATION, "rotation",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              rotation_func, exec_func));

  // scale
  exec_func = static_cast<Execution_function>(&Coord_transformer::translate);
  Vector3f_handle_function scale_func =
    static_cast<Vector3f_handle_function>
    (&Coord_transformer::scale_handle);
  s_prototype->add_field_info(new SF_vector3f(SCALE, "scale",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              scale_func, exec_func));

  // execute
  exec_func = static_cast<Execution_function>(&Coord_transformer::execute);
  Boolean_handle_function execute_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::execute_handle);
  s_prototype->add_field_info(new SF_bool(EXECUTE, "execute",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          execute_func,
                                          exec_func));

  // coord
  Shared_container_handle_function coord_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_transformer::coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD, "coord",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      coord_func, exec_func));

  // coord_changed
  Shared_container_handle_function coord_changed_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_transformer::coord_array_changed_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_CHANGED,
                                                      "coord_changed",
                                                      Field_info::RULE_OUT,
                                                      coord_changed_func));

  Boolean_handle_function changed_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::changed_handle);
  s_prototype->add_field_info(new SF_bool(CHANGED, "changed",
                                          Field_info::RULE_OUT,
                                          changed_func));
}

//! \brief deletes the prototype.
void Coord_transformer::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Coord_transformer::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

//! \brief sets the translation field.
void Coord_transformer::set_translation(const Vector3f& translation)
{ set_translation(translation[0], translation[1], translation[2]); }

//! \brief sets the translation field.
void Coord_transformer::set_translation(Float v0, Float v1, Float v2)
{
  m_translation.set(v0, v1, v2);
  translate();
}

//! \brief sets the rotation field.
void Coord_transformer::set_rotation(const Rotation& rotation)
{ set_rotation(rotation[0], rotation[1], rotation[2], rotation.get_angle()); }

//! \brief sets the rotation field.
void Coord_transformer::set_rotation(Float v0, Float v1, Float v2, Float angle)
{
  m_rotation.set(v0, v1, v2, angle);
  rotate();
}

//! \brief sets the scale field.
void Coord_transformer::set_scale(const Vector3f& scale)
{ set_scale(scale[0], scale[1], scale[2]); }

//! \brief sets the scale field.
void Coord_transformer::set_scale(Float v0, Float v1, Float v2)
{
  m_scale.set(v0, v1, v2);
  scale();
}

/*! \brief translates the input vertices and store the results in the output
 * vertices.
 */
void Coord_transformer::translate(const Field_info* field_info)
{
  m_transform.set_translation(m_translation);
  execute(field_info);
}

/*! \brief rotates the input vertices and store the results in the output
 * vertices.
 */
void Coord_transformer::rotate(const Field_info* field_info)
{
  m_transform.set_rotation(m_rotation);
  execute(field_info);
}

/*! \brief scales the input vertices and store the results in the output
 * vertices.
 */
void Coord_transformer::scale(const Field_info* field_info)
{
  m_transform.set_scale(m_scale);
  execute(field_info);
}

//! \brief applies the transformation.
void Coord_transformer::apply()
{
  if (!m_coord_array) return;
  if (!m_coord_array_changed) return;

  boost::shared_ptr<Coord_array_3d> coords_changed =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array_changed);
  SGAL_assertion(coords_changed);
  boost::shared_ptr<Coord_array_3d> coords =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);

  if (m_reflect)
    reflect(coords->begin(), coords->end(), coords_changed->begin());
  else transform(coords->begin(), coords->end(), coords_changed->begin());
}

/*! \brief applies the engine.
 */
void Coord_transformer::execute(const Field_info* /* field_info */)
{
  if (!m_enabled) return;

  auto size = m_coord_array->size();
  if (!m_coord_array_changed) {
    m_coord_array_changed.reset(new Coord_array_3d(size));
    SGAL_assertion(m_coord_array_changed);
  }
  else m_coord_array_changed->resize(size);

  apply();

  auto* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();

  m_changed = true;
  auto* changed_field = get_field(CHANGED);
  if (changed_field) changed_field->cascade();

  m_coord_array_changed->process_content_changed();
}

SGAL_END_NAMESPACE
