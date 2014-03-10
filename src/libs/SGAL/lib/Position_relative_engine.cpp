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
 * PositionRelativeEngine - implementation
 */

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph_int.h"
#include "SGAL/Position_relative_engine.h"
#include "SGAL/Field_infos.h"
#include "SGAL/Field.h"
#include "SGAL/String_utils.h"
#include "SGAL/Element.h"
#include "SGAL/Container_proto.h"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Position_relative_engine::s_tag = "PositionRelativeEngine";
Container_proto* Position_relative_engine::s_prototype(NULL);

REGISTER_TO_FACTORY(Position_relative_engine, "Position_relative_engine");

/*! Constructor */
Position_relative_engine::Position_relative_engine(Boolean proto) :
  Node(proto),
  m_fraction(0),
  m_value(0, 0, 0),
  m_position(0, 0, 0),
  m_delta(0, 0, 0),
  m_rotation(0, 0, 1, 0),
  m_lastFraction(-1)
{}

/*! Destructor */
Position_relative_engine::~Position_relative_engine() {}

/*! Initialize the prototype. */
void Position_relative_engine::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());


  // Add the object fields to the prototype
  Execution_function exec_func =
    static_cast<Execution_function>(&RotationRelativeEngine::execute);
  Float_handle_function fraction_func =
    static_cast<Float_handle_function>(&Texture::fraction_handle);
  s_prototype->add_field_info(new SF_float(FRACTION, "fraction",
                                           RULE_EXPOSED_FIELD,
                                           fraction_func,
                                           exec_func));

  Vector3f_handle_function value_func =
    static_cast<Vector3f_handle_function>(&Texture::value_handle);
  s_prototype->add_field_info(new SF_vector3f(VALUE, "value",
                                              RULE_EXPOSED_FIELD,
                                              value_func));

  Vector3f_handle_function position_func =
    static_cast<Vector3f_handle_function>(&Texture::position_handle);
  s_prototype->add_field_info(new SF_vector3f(POSITION, "position",
                                              RULE_EXPOSED_FIELD,
                                              position_func));

  Vector3f_handle_function delta_func =
    static_cast<Vector3f_handle_function>(&Texture::delta_handle);
  s_prototype->add_field_info(new SF_vector3f(DELTA, "delta",
                                              RULE_EXPOSED_FIELD,
                                              delta_func));

  Rotation_handle_function rotation_func =
    static_cast<Rotation_handle_function>(&Texture::rotation_handle);
  s_prototype->add_field_info(new Rotation(ROTATION, "rotation",
                                           RULE_EXPOSED_FIELD,
                                           rotation_func));
}

/*! */
void Position_relative_engine::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto * Position_relative_engine::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/**
 The execution function.
 Activated through the cascade of the m_fraction field.
 The function calculates m_value, updates it and activate cascade on it.
 @param pointer (in) to the cascaded field's field info - not used for now
*/
void Position_relative_engine::execute(Field_info*)
{
  Matrix4f rotMat;
  Matrix4f transMat;
  Matrix4f mulMat;
  Vector3f freactionedDelta;

  // check if this is a new cycle - if so - update the current rotation
  if (m_fraction == 1) {
    rotMat.MakeRot(m_rotation[0],m_rotation[1],m_rotation[2],m_rotation[3]);
    transMat.MakeTrans(m_delta);
    mulMat.Mult(rotMat,transMat);
    mulMat.GetRow(3,m_value);
    m_position.Add(m_value);
    m_value = m_position;
  }
  else {
    // Calculate m_value
    rotMat.MakeRot(m_rotation[0],m_rotation[1],m_rotation[2],m_rotation[3]);
    freactionedDelta.Set(m_delta[0]*m_fraction,
               m_delta[1]*m_fraction,
               m_delta[2]*m_fraction);
    transMat.MakeTrans(freactionedDelta);
    mulMat.Mult(rotMat,transMat);
    mulMat.GetRow(3,m_value);
    m_value.Add(m_position);
  }

  //m_lastFraction = m_fraction;


  // Cascade the updated value
  Field* value = get_field(VALUE);
  if (value!=ENULL)
    value->Cascade();
};

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Position_relative_engine::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "delta") {
      m_delta = Vector3f(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "rotation") {
      m_rotation = Rotation(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "position") {
      m_position = Vector3f(value);
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/**
 * Get a list of atytributes in this object. This method is called only
 * from the Builder side.
 *
 * @return a list of attributes
 */
Attribute_list Position_relative_engine::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attrib.first = "position";
  attrib.second = m_position.get_text();
  attribs.push_back(attrib);

  attrib.first = "delta";
  attrib.second = m_delta.get_text();
  attribs.push_back(attrib);

  attrib.first = "rotation";
  attrib.second = m_rotation.get_text();;
  attribs.push_back(attrib);


  return attribs;
}

SGAL_END_NAMESPACE
