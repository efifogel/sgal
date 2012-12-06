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
// $Id: $
// $Revision: $
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
#include <vector>
#include <boost/lexical_cast.hpp>

#include "SGAL/Math_defs.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Coord_array.hpp"

#include "SEGO/Ego_brick.hpp"

SGAL_BEGIN_NAMESPACE

std::string Ego_brick::s_tag = "EgoBrick";
Container_proto* Ego_brick::s_prototype = NULL;

// Default values:
const Uint Ego_brick::s_def_number_of_knobs1(1);
const Uint Ego_brick::s_def_number_of_knobs2(1);
const Float Ego_brick::s_def_pitch(8);
const Float Ego_brick::s_def_height(9.6);
const Float Ego_brick::s_def_knob_radius(2.4);
const Float Ego_brick::s_def_knob_height(1.8);
const Float Ego_brick::s_def_tolerance(0.1);
const Uint Ego_brick::s_def_knob_slices(32);

REGISTER_TO_FACTORY(Ego_brick, "Ego_brick");

/*! Constructor */
Ego_brick::Ego_brick(Boolean proto) :
  Indexed_face_set(proto),
  m_number_of_knobs1(s_def_number_of_knobs1),
  m_number_of_knobs2(s_def_number_of_knobs2),
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
  /*! A 2D cross section of the knob */
  std::vector<Vector2f> knob_cross_section(m_knob_slices);
  std::vector<Vector2f>::iterator it;
  for (it = knob_cross_section.begin(); it != knob_cross_section.end(); ++it) {
    Float x = m_knob_radius * cosf(angle);
    Float y = m_knob_radius * sinf(angle);
    angle += delta;
    it->set(x, y);
  }
  Uint n = knob_cross_section.size();
  
  // Generate points:
  Uint points_per_knob = 1 + n * 2;
  Uint num_knobs = m_number_of_knobs1 * m_number_of_knobs2;
  Uint size = 8 + points_per_knob * num_knobs;
  m_coord_array->resize(size);

  // Corner points:
  float width = m_pitch * m_number_of_knobs1;
  float depth = m_pitch * m_number_of_knobs2;
  float center_x = width * 0.5f;
  float center_y = depth * 0.5f;
  float center_z = m_height * 0.5f;
  float base_x = -center_x;
  float base_y = -center_y;
  float base_z = -center_z;

  Uint k = 0;

  // Box
  (*m_coord_array)[k++].set(base_x, base_y, base_z);
  (*m_coord_array)[k++].set(base_x + width, base_y, base_z);
  (*m_coord_array)[k++].set(base_x + width, base_y + depth, base_z);
  (*m_coord_array)[k++].set(base_x, base_y + depth, base_z);
  (*m_coord_array)[k++].set(base_x, base_y, base_z + m_height);
  (*m_coord_array)[k++].set(base_x + width, base_y, base_z + m_height);
  (*m_coord_array)[k++].set(base_x + width, base_y + depth, base_z + m_height);
  (*m_coord_array)[k++].set(base_x, base_y + depth, base_z + m_height);

  // Knobs:
  base_x += m_pitch * 0.5;
  base_y += m_pitch * 0.5;
  float z = base_z + m_height;
  Uint i;
  Uint j;
  for (j = 0; j < m_number_of_knobs2; ++j) {
    float center_y = base_y + j * m_pitch;
    for (i = 0; i < m_number_of_knobs1; ++i) {
      float center_x = base_x + i * m_pitch;
      (*m_coord_array)[k++].set(center_x, center_y, z + m_knob_height);
      for (std::vector<Vector2f>::iterator it = knob_cross_section.begin();
           it != knob_cross_section.end(); ++it)
      {
        float x = center_x + (*it)[0];
        float y = center_y + (*it)[1];
        (*m_coord_array)[k++].set(x, y, z);
      }
      for (std::vector<Vector2f>::iterator it = knob_cross_section.begin();
           it != knob_cross_section.end(); ++it)
      {
        float x = center_x + (*it)[0];
        float y = center_y + (*it)[1];
        (*m_coord_array)[k++].set(x, y, z + m_knob_height);
      }
    }
  }
  knob_cross_section.clear();
  
  // Generates coordinate indices:

  // Box
  Uint num_box_primitives = 6 * 2;
  Uint primitives_per_knob = n * 3;
  // m_primitive_type = PT_TRIANGLES;
  m_num_primitives = num_box_primitives + primitives_per_knob * num_knobs;
  m_are_indices_flat = false;
  k = 0;
  Uint num_indices = m_num_primitives * 4;
  m_coord_indices.resize(num_indices);

  // Bottom
  m_coord_indices[k++] = 0;
  m_coord_indices[k++] = 2;
  m_coord_indices[k++] = 1;
  m_coord_indices[k++] = static_cast<Uint>(-1);
  m_coord_indices[k++] = 0;
  m_coord_indices[k++] = 3;
  m_coord_indices[k++] = 2;
  m_coord_indices[k++] = static_cast<Uint>(-1);

  // top
  m_coord_indices[k++] = 4;
  m_coord_indices[k++] = 5;
  m_coord_indices[k++] = 6;
  m_coord_indices[k++] = static_cast<Uint>(-1);
  m_coord_indices[k++] = 4;
  m_coord_indices[k++] = 6;
  m_coord_indices[k++] = 7;
  m_coord_indices[k++] = static_cast<Uint>(-1);

  // left
  m_coord_indices[k++] = 0;
  m_coord_indices[k++] = 4;
  m_coord_indices[k++] = 7;
  m_coord_indices[k++] = static_cast<Uint>(-1);
  m_coord_indices[k++] = 0;
  m_coord_indices[k++] = 7;
  m_coord_indices[k++] = 3;
  m_coord_indices[k++] = static_cast<Uint>(-1);

  // Right
  m_coord_indices[k++] = 1;
  m_coord_indices[k++] = 2;
  m_coord_indices[k++] = 6;
  m_coord_indices[k++] = static_cast<Uint>(-1);
  m_coord_indices[k++] = 1;
  m_coord_indices[k++] = 6;
  m_coord_indices[k++] = 5;
  m_coord_indices[k++] = static_cast<Uint>(-1);

  // Near
  m_coord_indices[k++] = 0;
  m_coord_indices[k++] = 1;
  m_coord_indices[k++] = 5;
  m_coord_indices[k++] = static_cast<Uint>(-1);
  m_coord_indices[k++] = 0;
  m_coord_indices[k++] = 5;
  m_coord_indices[k++] = 4;
  m_coord_indices[k++] = static_cast<Uint>(-1);

  // Far
  m_coord_indices[k++] = 2;
  m_coord_indices[k++] = 3;
  m_coord_indices[k++] = 7;
  m_coord_indices[k++] = static_cast<Uint>(-1);
  m_coord_indices[k++] = 2;
  m_coord_indices[k++] = 7;
  m_coord_indices[k++] = 6;
  m_coord_indices[k++] = static_cast<Uint>(-1);

  // Knobs:
  Uint base = 8;
  for (j = 0; j < m_number_of_knobs2; ++j) {
    for (i = 0; i < m_number_of_knobs1; ++i) {
      Uint l;
      for (l = 0; l < n; ++l) {
        Uint a = base + 1 + l;
        Uint b = base + 1 + ((l+1) % n);
        Uint c = b + n;
        Uint d = a + n;
        m_coord_indices[k++] = a;
        m_coord_indices[k++] = b;
        m_coord_indices[k++] = c;
        m_coord_indices[k++] = static_cast<Uint>(-1);
        m_coord_indices[k++] = a;
        m_coord_indices[k++] = c;
        m_coord_indices[k++] = d;
        m_coord_indices[k++] = static_cast<Uint>(-1);
      }

      // Top
      for (l = 0; l < n; ++l) {
        m_coord_indices[k++] = base;
        m_coord_indices[k++] = base + 1 + n + l;
        m_coord_indices[k++] = base + 1 + n + ((l+1) % n);
        m_coord_indices[k++] = static_cast<Uint>(-1);
      }
      base += 1 + n * 2;
    }
  }

  // Attributes
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
    if (name == "numberOfKnobs1") {
      set_number_of_knobs1(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "numberOfKnobs2") {
      set_number_of_knobs2(boost::lexical_cast<Uint>(value));
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

  // We use SF_int (instead of SF_uint) to allow connecting the value
  // field of an Incrementor, which is of int type (and not Uint) to this
  // field.
  s_prototype->add_field_info(new SF_int(NUMBER_OF_KNOBS1, "numberOfKnobs1",
                                         get_member_offset(&m_number_of_knobs1),
                                         exec_func));

  s_prototype->add_field_info(new SF_int(NUMBER_OF_KNOBS2, "numberOfKnobs2",
                                         get_member_offset(&m_number_of_knobs2),
                                         exec_func));
  
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

