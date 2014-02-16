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
// $Id: $
// $Revision: 7780 $
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
#include "SGAL/Coord_array.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Transform.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Coord_transformer::s_prototype(NULL);
const std::string Coord_transformer::s_tag = "CoordinateTransformer";

Boolean Coord_transformer::s_def_enabled = true;

/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Coord_transformer, "Coord_transformer");

//! \brief constructor.
Coord_transformer::Coord_transformer(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_reflect(false),
  m_changed(false),
  m_execute(false),
  m_translated(false),
  m_rotated(false),
  m_transform(NULL)
{}

//! \brief sets the attributes of this object.
void Coord_transformer::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "translation") {
      Vector3f vec(value);
      m_translation.set(vec);
      m_transform.set_translation(m_translation);
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

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Shared_container cont = elem->get_value(cai);
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
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled", enabled_func));

  // changed
  Boolean_handle_function changed_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::changed_handle);
  s_prototype->add_field_info(new SF_bool(CHANGED, "changed", changed_func));

  Boolean_handle_function translated_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::translated_handle);
  s_prototype->add_field_info(new SF_bool(TRANSLATED, "translated",
                                          translated_func));

  // rotated
  Boolean_handle_function rotated_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::rotated_handle);
  s_prototype->add_field_info(new SF_bool(ROTATED, "rotated", rotated_func));

  // translation
  exec_func = static_cast<Execution_function>(&Coord_transformer::translate);
  Vector3f_handle_function translation_func =
    static_cast<Vector3f_handle_function>
    (&Coord_transformer::translation_handle);
  s_prototype->add_field_info(new SF_vector3f(TRANSLATION, "translation",
                                              translation_func, exec_func));

  // rotation
  exec_func = static_cast<Execution_function>(&Coord_transformer::rotate);
  Rotation_handle_function rotation_func =
    static_cast<Rotation_handle_function>(&Coord_transformer::rotation_handle);
  s_prototype->add_field_info(new SF_rotation(ROTATION, "rotation",
                                              rotation_func, exec_func));

  // execute
  exec_func = static_cast<Execution_function>(&Coord_transformer::execute);
  Boolean_handle_function execute_func =
    static_cast<Boolean_handle_function>(&Coord_transformer::execute_handle);
  s_prototype->add_field_info(new SF_bool(EXECUTE, "execute", execute_func,
                                          exec_func));

  // coord
  Shared_container_handle_function coord_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_transformer::coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD, "coord",
                                                      coord_func, exec_func));

  // coord_changed
  Shared_container_handle_function coord_changed_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_transformer::coord_array_changed_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_CHANGED,
                                                      "coord_changed",
                                                      coord_changed_func));
}

//! \brief deletes the prototype.
void Coord_transformer::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the prototype.
Container_proto* Coord_transformer::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

//! \brief sets the translation field.
void Coord_transformer::set_translation(const Vector3f& translation)
{
  set_translation(translation[0], translation[1], translation[2]);
}

//! \brief sets the translation field.
void Coord_transformer::set_translation(Float v0, Float v1, Float v2)
{
  m_translation.set(v0, v1, v2);
  translate();
}

//! \brief sets the rotation field.
void Coord_transformer::set_rotation(const Rotation& rotation)
{
  set_rotation(rotation[0], rotation[1], rotation[2], rotation.get_angle());
}

//! \brief sets the rotation field.
void Coord_transformer::set_rotation(Float v0, Float v1, Float v2, Float angle)
{
  m_rotation.set(v0, v1, v2, angle);
  rotate();
}

/*! \brief translates the input vertices and store the results in the output
 * vertices.
 */
void Coord_transformer::translate(Field_info* field_info)
{
  m_transform.set_translation(m_translation);
  execute(field_info);

  m_translated = true;
  Field* changed_field = get_field(TRANSLATED);
  if (changed_field) changed_field->cascade();
}

/*! \brief rotates the input vertices and store the results in the output
 * vertices.
 */
void Coord_transformer::rotate(Field_info* field_info)
{
  m_transform.set_rotation(m_rotation);
  execute(field_info);

  m_rotated = true;
  Field* changed_field = get_field(ROTATED);
  if (changed_field) changed_field->cascade();
}

/*! \brief transforms the input vertices and store the results in the output
 * vertices.
 */
void Coord_transformer::execute(Field_info* /* field_info */)
{
  if (!m_enabled) return;
  if (!m_coord_array) return;

  Uint size = m_coord_array->size();

  if (!m_coord_array_changed)
    m_coord_array_changed.reset(new Coord_array(size));
  else
    m_coord_array_changed->resize(size);

  if (m_reflect) {
    for (Uint i = 0; i < size; ++i)
      (*m_coord_array_changed)[i].negate((*m_coord_array)[i]);
  }
  else {
    const Matrix4f& mat = m_transform.get_matrix();
    for (Uint i = 0; i < size; ++i)
      (*m_coord_array_changed)[i].xform_pt((*m_coord_array)[i], mat);
  }

  Field* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();

  m_changed = true;
  Field* changed_field = get_field(CHANGED);
  if (changed_field) changed_field->cascade();

  m_coord_array_changed->process_content_changed();
}

SGAL_END_NAMESPACE
