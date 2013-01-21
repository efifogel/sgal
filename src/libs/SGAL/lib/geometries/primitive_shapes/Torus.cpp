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
// $Source: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4996 )
#endif

#include <iostream>
#include <sstream>

#include "SGAL/Torus.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

std::string Torus::s_tag = "Torus";
Container_proto* Torus::s_prototype = NULL;

// Default values:
const Float Torus::s_def_cross_section_radius(1); // Override Extrusion def.
const Float Torus::s_def_spine_radius(2);
const Uint Torus::s_def_stacks(16);
const Uint Torus::s_def_slices(16);

REGISTER_TO_FACTORY(Torus, "Torus");

/*! Constructor */
Torus::Torus(Boolean proto) :
  Extrusion(proto),
  m_spine_radius(s_def_spine_radius),
  m_slices(s_def_slices),
  m_stacks(s_def_stacks)
{
  m_cross_section_radius = s_def_cross_section_radius;
}

/*! Destructor */
Torus::~Torus(){}

/*! Clean the ellipsoid internal representation */
void Torus::clean()
{
  Uint i;
  float angle;
  
  m_cross_section.resize(m_stacks);
  angle = SGAL_PI * 2 / m_stacks;
  for (i = 0; i < m_stacks; ++i) {
    float x = m_cross_section_radius * cosf(angle * i);
    float z = m_cross_section_radius * sinf(angle * i);
    m_cross_section[i].set(x, -z);
  }

  m_spine.resize(m_slices);
  angle = SGAL_PI * 2 / m_slices;
  for (i = 0; i < m_slices; ++i) {
    float x = m_spine_radius * cosf(angle * i);
    float z = m_spine_radius * sinf(angle * i);
    m_spine[i].set(x, 0, z);
  }
  m_begin_cap = false;
  m_end_cap = false;
  m_loop = true;
  
  Extrusion::clean();
}

/*! Set the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Torus::set_attributes(Element* elem)
{
  Extrusion::set_attributes(elem);

  std::string name;
  std::string value;

  typedef Element::Str_attr_iter          Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "spineRadius") {
      set_spine_radius(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stacks") {
      set_stacks(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "slices") {
      set_slices(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    } 
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/* Initilalize the container prototype */
void Torus::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Extrusion::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Mesh_set::coord_changed);

  s_prototype->add_field_info(new SF_float(SPINE_RADIUS, "spineRadius",
                                           get_member_offset(&m_spine_radius),
                                           exec_func));

  // Rendering required
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          get_member_offset(&m_stacks),
                                          exec_func));

  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          get_member_offset(&m_slices),
                                          exec_func));
}

/*! \brief deletes the container prototype. */
void Torus::delete_prototype() 
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Torus::get_prototype() 
{  
  if (!s_prototype) Torus::init_prototype();
  return s_prototype;
} 

SGAL_END_NAMESPACE
