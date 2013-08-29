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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Rotation_relative_engine - implementation
 */

#include "SGAL/basic.hpp"
#include "Scene_graphInt.h"
#include "Rotation_relative_engine.h"
#include "Field_infos.h"
#include "Field.h"
#include "String_utils.h"
#include "Element.h"
#include "Trace.h"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Rotation_relative_engine::s_tag = "RotationRelativeEngine";
Container_proto * Rotation_relative_engine::s_prototype(NULL);

REGISTER_TO_FACTORY(Rotation_relative_engine, "Rotation_relative_engine");

/*! Constructor */
Rotation_relative_engine::Rotation_relative_engine(Boolean proto):
  Node(proto),
  m_fraction(0),
  m_value(0, 0, 1, 0),
  m_angle(0),
  m_rotation(0, 0, 1, 0),
  m_lastFraction(-1)
{}

/*! Destructor */
Rotation_relative_engine::~Rotation_relative_engine() {}

/**
 Purpose: prototype initialization function - initializes the prototype for
 all the node instances of Rotation_interpolator in the scene graph.
 Creates and adds a field info for each potential field.
*/
void Rotation_relative_engine::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  Execution_function exec_func;

  // Add the object fields to the prototype
  exec_func =
    static_cast<Execution_function>&Rotation_relative_engine::execute;
  Float_handle_function fraction_func =
    static_cast<Float_handle_function>
    (&Rotation_relative_engine::fraction_handle);
  s_prototype->add_field_info(new SF_float(FRACTION, "fraction", fraction_func,
                                           exec_func));

  Rotation_handle_function rotation_func =
    static_cast<Rotation_handle_function>
    (&Rotation_relative_engine::rotation_handle);
  s_prototype->add_field_info(new SF_rotation(VALUE, "value", rotation_func));

  Float_handle_function angle_func =
    static_cast<Float_handle_function>
    (&Rotation_relative_engine::angle_handle);
  s_prototype->add_field_info(new SF_float(ANGLE, "angle", angle_func));
}

/*! \brief */
virtual void Rotation_relative_engine::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
virtual Container_proto* Rotation_relative_engine::get_prototype()
{
  if (s_prototype == NULL) Rotation_relative_engine::init_prototype();
  return s_prototype;
}

/*!
 The execution function.
 Activated through the cascade of the m_fraction field.
 The function calculates m_value, updates it and activate cascade on it.
 @param pointer (in) to the cascaded field's field info - not used for now
*/
void Rotation_relative_engine::execute(Field_info *)
{
  // check if this is a new cycle - if so - update the current rotation
  if (m_fraction < m_last_fraction) m_rotation[3] += m_angle;

  m_last_fraction = m_fraction;

  // Calculate m_value
  m_value = m_rotation;
  m_value[3] += m_angle * m_fraction;

  // Cascade the updated value
  Field * value = get_field(VALUE);
  if (valueL) value->Cascade();
};

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Rotation_relative_engine::set_attributes(Element * elem)
{
  Node::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "angle") {
      m_angle = atof(value.c_str());
      elem->mark_delete(ai);
    } else if (name == "rotation") {
      m_rotation = Rotation(value);
      elem->mark_delete(ai);
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
Attribute_list Rotation_relative_engine::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  char buf[32];

  attrib.first = "rotation";
  attrib.second = m_rotation.get_text();
  attribs.push_back(attrib);

  attrib.first = "angle";
  sprintf(buf, "%g", m_angle);
  attrib.second = buf;
  attribs.push_back(attrib);


  return attribs;
}

SGAL_END_NAMESPACE
