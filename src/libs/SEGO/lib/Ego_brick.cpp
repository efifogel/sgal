// Copyright (c) 2012 Israel.
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

#if defined(_WIN32)
#pragma warning( disable : 4503)
#endif

/*! \file
 * A geometry container that represents an ego brick.
 */

//#include <iostream>
//#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

#include "SGAL/Math_defs.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector2u.hpp"
#include "SGAL/Coord_array.hpp"

#include "SEGO/Ego_brick.hpp"

SGAL_BEGIN_NAMESPACE

std::string Ego_brick::s_tag = "EgoBrick";
Container_proto* Ego_brick::s_prototype = NULL;

// Default values:
const Vector2u Ego_brick::s_def_size(1, 1);
const Float Ego_brick::s_def_pitch(0);
const Float Ego_brick::s_def_height(0);
const Float Ego_brick::s_def_knob_radius(0);
const Float Ego_brick::s_def_knob_height(0);
const Float Ego_brick::s_def_tolerance(0.1);
const Uint Ego_brick::s_def_knob_slices(32);

REGISTER_TO_FACTORY(Ego_brick, "Ego_brick");

/*! Constructor */
Ego_brick::Ego_brick(Boolean proto) :
  Indexed_face_set(proto),
  m_size(s_def_size),
  m_pitch(s_def_pitch),
  m_height(s_def_height),
  m_knob_radius(s_def_knob_radius),
  m_knob_height(s_def_knob_height),
  m_tolerance(s_def_tolerance),
  m_knob_slices(s_def_knob_slices)
{
}

/*! Destructor */
Ego_brick::~Ego_brick()
{
  if (m_coord_array) {
    delete m_coord_array;
    m_coord_array = NULL;
  }
}

/*! Clean the ellipsoid internal representation */
void Ego_brick::clean()
{
  // Clear internal representation:
  if (!m_coord_array) m_coord_array = new Coord_array;

  // Generate cross section:
  Float delta = SGAL_TWO_PI / m_knob_slices;
  Float angle = 0;
  Uint i;
  for (i = 0; i < m_knob_slices; ++i) {
    Float x = m_knob_radius * cosf(angle);
    Float y = m_knob_radius * sinf(angle);
    angle += delta;
    m_knob_cross_section.set(x, y);
  }
  
  // Generate points:

  // Generates coordinate indices:
  
  set_solid(true);
  Indexed_face_set::clean();
}

/*! Set the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Ego_brick::set_attributes(Element * elem)
{
  Indexed_face_set::set_attributes(elem);

  std::string name;
  std::string value;

  typedef Element::Str_attr_iter Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "size") {
      Vector2u size(value);
      set_size(size);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "pitch") {
      set_pitch(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "height") {
      set_height(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "knobRadius") {
      set_knob_radius(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "knobHeight") {
      set_knob_height(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "tolerance") {
      set_tolerance(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "knobSlices") {
      set_knob_slices(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/* Initilalize the container prototype */
void Ego_brick::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Mesh_set::coord_changed);

  // s_prototype->
    // add_field_info(new MF_vector2u(CROSS_SECTION, "size",
    //                                get_member_offset(&m_size),
    //                                exec_func));

  s_prototype->add_field_info(new SF_float(PITCH, "pitch",
                                           get_member_offset(&m_pitch),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(HEIGHT, "height",
                                           get_member_offset(&m_height),
                                           exec_func));


  s_prototype->add_field_info(new SF_float(KNOB_RADIUS, "knobRadius",
                                           get_member_offset(&m_knob_radius),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(KNOB_HEIGHT, "knobHeight",
                                           get_member_offset(&m_knob_height),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(TOLERANCE, "tolerance",
                                           get_member_offset(&m_tolerance),
                                           exec_func));

  s_prototype->add_field_info(new SF_uint(KNOB_SLICES, "knobSlices",
                                          get_member_offset(&m_knob_slices),
                                          exec_func));
}

/*! Delete the container prototype */
void Ego_brick::delete_prototype() 
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the container prototype */
Container_proto * Ego_brick::get_prototype() 
{  
  if (!s_prototype) Ego_brick::init_prototype();
  return s_prototype;
} 

SGAL_END_NAMESPACE

