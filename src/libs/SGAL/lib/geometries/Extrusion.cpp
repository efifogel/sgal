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
// $Revision: 7261 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4996 )
#endif

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "SGAL/Extrusion.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

std::string Extrusion::s_tag = "Extrusion";
Container_proto* Extrusion::s_prototype = NULL;

// Default values:
const Boolean Extrusion::s_def_begin_cap(true);
const Boolean Extrusion::s_def_end_cap(true);
const Boolean Extrusion::s_def_loop(false);
const Float Extrusion::s_def_cross_section_radius(0.1f);
const Uint Extrusion::s_def_cross_section_slices(32);

REGISTER_TO_FACTORY(Extrusion, "Extrusion");

/*! Constructor */
Extrusion::Extrusion(Boolean proto) :
  Indexed_face_set(proto),
  m_begin_cap(s_def_begin_cap),
  m_end_cap(s_def_end_cap),
  m_loop(s_def_loop),
  m_cross_section_radius(s_def_cross_section_radius),
  m_cross_section_slices(s_def_cross_section_slices)
{
}

/*! Destructor */
Extrusion::~Extrusion()
{
  if (m_coord_array) delete m_coord_array;
  m_cross_section.clear();
  m_orientation.clear();
  m_scale.clear();
  m_spine.clear();
}

/*! Clean the ellipsoid internal representation */
void Extrusion::clean()
{
  // Clear internal representation:
  if (!m_coord_array) m_coord_array = new Coord_array;

  // Generate cross section:
  if (m_cross_section.size() == 0) {
    m_cross_section.resize(m_cross_section_slices);
    Float delta = SGAL_TWO_PI / m_cross_section_slices;
    Float angle = 0;
    Uint i;
    for (i = 0; i < m_cross_section_slices; ++i) {
      Float x = m_cross_section_radius * sinf(angle);
      Float y = m_cross_section_radius * cosf(angle);
      angle += delta;
      m_cross_section[i].set(x, y);
    }
  }
  
  // Generate points:
  Uint size = m_cross_section.size() * m_spine.size();
  m_coord_array->resize(size);
  
  Uint j, i, k = 0;
  const Vector3f vert(0, 1, 0);
  const Vector3f hor(0, 0, 1);
  Vector3f vec(vert);
  Rotation rot, prev_rot;
  Matrix4f mat;
  
  // First:
  if (m_loop) {
    vec.sub(m_spine[0], m_spine[m_spine.size()-1]);
    vec.normalize();
    float my_cos = vec.dot(vert);
    float angle = arccosf(my_cos);
    if (angle > SGAL_EPSILON) {
      if (abs_gt((angle - SGAL_PI), SGAL_EPSILON)) {
        Vector3f axis;
        axis.cross(vert, vec);
        axis.normalize();
        prev_rot.set(axis, angle);
      } else {
        prev_rot.set(hor, -SGAL_PI);
      }
    } else {
      prev_rot.set(hor, 0);
    }
  }
  Vector3f prev_vec(vec);
  vec.sub(m_spine[1], m_spine[0]);
  vec.normalize();
  float my_cos = vec.dot(prev_vec);
  float angle = arccosf(my_cos);
  Rotation tmp_rot;
  if (angle > SGAL_EPSILON) {
    if (abs_gt((angle - SGAL_PI), SGAL_EPSILON)) {
      Vector3f axis;
      axis.cross(prev_vec, vec);
      axis.normalize();
      tmp_rot.set(axis, angle);
    } else {
      tmp_rot.set(hor, -SGAL_PI);
    }
  } else {
    tmp_rot.set(hor, 0);
  }
  rot.mult(tmp_rot, prev_rot);
  if (m_loop) {
    Rotation applied_rot;
    applied_rot.slerp(0.5f, prev_rot, rot);
    mat.make_rot(applied_rot.get_axis(), applied_rot.get_angle());
  } else {
    mat.make_rot(rot.get_axis(), rot.get_angle());
  }
  for (i = 0; i < m_cross_section.size(); ++i) {
    Vector3f point(m_cross_section[i][0], 0, m_cross_section[i][1]);
    if (m_scale.size() > 0) {
      point[0] *= m_scale[0][0];
      point[2] *= m_scale[0][1];
    }
    point.xform_pt(point, mat);

    (*m_coord_array)[k++].add(m_spine[0], point);
  }
  
  // Middle:
  for (j = 1; j < m_spine.size() - 1; ++j) {
    Rotation prev_rot(rot);
    Vector3f prev_vec(vec);
    vec.sub(m_spine[j+1], m_spine[j]);
    vec.normalize();
    float my_cos = vec.dot(prev_vec);
    angle = arccosf(my_cos);
    Rotation tmp_rot;
    if (angle > SGAL_EPSILON) {
      if (abs_gt((angle - SGAL_PI), SGAL_EPSILON)) {
        Vector3f axis;
        axis.cross(prev_vec, vec);
        axis.normalize();
        tmp_rot.set(axis, angle);
      } else {
        tmp_rot.set(hor, -SGAL_PI);
      }
    } else {
      tmp_rot.set(hor, 0);
    }
    rot.mult(tmp_rot, prev_rot);
    Rotation applied_rot;
    applied_rot.slerp(0.5f, prev_rot, rot);
    mat.make_rot(applied_rot.get_axis(), applied_rot.get_angle());
    for (i = 0; i < m_cross_section.size(); ++i) {
      Vector3f point(m_cross_section[i][0], 0, m_cross_section[i][1]);
      if (m_scale.size() > j) {
        point[0] *= m_scale[j][0];
        point[2] *= m_scale[j][1];
      }
      point.xform_pt(point, mat);
      (*m_coord_array)[k++].add(m_spine[j], point);
    }
  }

  // Last:
  if (m_loop) {
    Vector3f prev_vec(vec);
    vec.sub(m_spine[0], m_spine[j]);
    vec.normalize();
    float my_cos = vec.dot(prev_vec);
    angle = arccosf(my_cos);
    Rotation prev_rot(rot);
    Rotation tmp_rot;
    if (angle > SGAL_EPSILON) {
      if (abs_gt((angle - SGAL_PI), SGAL_EPSILON)) {
        Vector3f axis;
        axis.cross(prev_vec, vec);
        axis.normalize();
        tmp_rot.set(axis, angle);
      } else {
        tmp_rot.set(hor, -SGAL_PI);
      }
    } else {
      tmp_rot.set(hor, 0);
    }
    rot.mult(tmp_rot, prev_rot);
    Rotation applied_rot;
    applied_rot.slerp(0.5f, prev_rot, rot);
    mat.make_rot(applied_rot.get_axis(), applied_rot.get_angle());
  } else {
    mat.make_rot(rot.get_axis(), rot.get_angle());
  }
  for (i = 0; i < m_cross_section.size(); ++i) {
    Vector3f point(m_cross_section[i][0], 0, m_cross_section[i][1]);
    if (m_scale.size() > j) {
      point[0] *= m_scale[j][0];
      point[2] *= m_scale[j][1];
    }
    point.xform_pt(point, mat);
    (*m_coord_array)[k++].add(m_spine[j], point);
  }

  // Generates coordinate indices:

  // Generate all:
  Uint sections = (m_loop) ? m_spine.size() : m_spine.size() - 1;
  Uint offset = m_cross_section.size() * (m_spine.size() - 1);
  m_num_primitives = m_cross_section.size() * sections;
  size = m_num_primitives * 5;
  if (m_begin_cap) {
    size += m_cross_section.size() + 1;
    ++m_num_primitives;
  }
  if (m_end_cap) {
    size += m_cross_section.size() + 1;
    ++m_num_primitives;
  }
  
  k = 0;
  m_coord_indices.resize(size);
  for (j = 0; j < m_spine.size() - 1; ++j) {
    Uint start = j * m_cross_section.size();
    for (i = 0; i < m_cross_section.size() - 1; ++i) {
      Uint ll = start + i;
      Uint ul = ll + m_cross_section.size();
      m_coord_indices[k++] = ll;
      m_coord_indices[k++] = ll + 1;
      m_coord_indices[k++] = ul + 1;
      m_coord_indices[k++] = ul;
      m_coord_indices[k++] = static_cast<Uint>(-1);
    }
    Uint ll = start + i;
    Uint ul = ll + m_cross_section.size();
    m_coord_indices[k++] = ll;
    m_coord_indices[k++] = ll + 1 - m_cross_section.size();
    m_coord_indices[k++] = ul + 1 - m_cross_section.size();
    m_coord_indices[k++] = ul;
    m_coord_indices[k++] = static_cast<Uint>(-1);
  }

  if (m_loop) {
    Uint start = j * m_cross_section.size();
    for (i = 0; i < m_cross_section.size() - 1; ++i) {
      Uint ll = start + i;
      Uint ul = i;
      m_coord_indices[k++] = ll;
      m_coord_indices[k++] = ll + 1;
      m_coord_indices[k++] = ul + 1;
      m_coord_indices[k++] = ul;
      m_coord_indices[k++] = static_cast<Uint>(-1);
    }
    Uint ll = start + i;
    Uint ul = i;
    m_coord_indices[k++] = ll;
    m_coord_indices[k++] = ll + 1 - m_cross_section.size();
    m_coord_indices[k++] = ul + 1 - m_cross_section.size();
    m_coord_indices[k++] = ul;
    m_coord_indices[k++] = static_cast<Uint>(-1);
  }
  
  // Generate caps:
  if (m_begin_cap) {
    for (i = 0; i < m_cross_section.size(); ++i)
      m_coord_indices[k++] = m_cross_section.size() - 1 - i;
    m_coord_indices[k++] = static_cast<Uint>(-1);
  }

  if (m_end_cap) {
    for (i = 0; i < m_cross_section.size(); ++i)
      m_coord_indices[k++] = offset + i;
    m_coord_indices[k++] = static_cast<Uint>(-1);
  }
  
  set_solid(m_loop || (m_begin_cap && m_end_cap));
  Indexed_face_set::clean();
}

/*! Set the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Extrusion::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  std::string name;
  std::string value;

  typedef Element::Str_attr_iter          Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "beginCap") {
      set_begin_cap(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "endCap") {
      set_end_cap(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "loop") {
      set_loop(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "crossSection") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 2;
      m_cross_section.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0 ; i < size ; i++) {
        svalue >> m_cross_section[i][0] >> m_cross_section[i][1];
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "crossSectionRadius") {
      set_cross_section_radius(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "crossSectionSlices") {
      set_cross_section_slices(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "orientation") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 4;
      m_orientation.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < size; i++) svalue >> m_orientation[i];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "scale") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 2;
      m_scale.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < size; i++) svalue >> m_scale[i][0] >> m_scale[i][1];
      elem->mark_delete(ai);
      continue;
    } 
    if (name == "spine") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 3;
      m_spine.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < size; ++i) {
        svalue >> m_spine[i][0] >> m_spine[i][1] >> m_spine[i][2];
      }
      elem->mark_delete(ai);
      continue;
    } 
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/**
 * Get a list of attributes for the object.
 * The list is of name=value pairs.
 * @return a list of pairs of strings
 */
Attribute_list Extrusion::get_attributes() 
{ 
  Attribute_list attribs; 
  // attribs = Geometry::get_attributes();
  Attribue attrib;
  char buf[32];

  attribs = Indexed_face_set::get_attributes();

  if(get_begin_cap() != s_def_begin_cap) {
    attrib.first = "beginCap";
    sprintf(buf, "%g", get_begin_cap());
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  return attribs;
}
#endif

/* Initilalize the container prototype */
void Extrusion::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Mesh_set::coord_changed);
  s_prototype->add_field_info(new SF_bool(BEGIN_CAP, "beginCap",
                                          get_member_offset(&m_begin_cap),
                                          exec_func));

  s_prototype->add_field_info(new SF_bool(END_CAP, "endCap",
                                          get_member_offset(&m_end_cap),
                                          exec_func));

  s_prototype->add_field_info(new SF_bool(LOOP, "loop",
                                          get_member_offset(&m_loop),
                                          exec_func));

  s_prototype->
    add_field_info(new MF_vector2f(CROSS_SECTION, "crossSection",
                                   get_member_offset(&m_cross_section),
                                   exec_func));

  s_prototype->
    add_field_info(new MF_rotation(ORIENTATION, "orientation",
                                   get_member_offset(&m_orientation),
                                   exec_func));
  s_prototype->add_field_info(new MF_vector2f(SCALE, "scale",
                                              get_member_offset(&m_scale),
                                              exec_func));

  s_prototype->add_field_info(new MF_vector3f(SPINE, "spine",
                                              get_member_offset(&m_spine),
                                              exec_func));
}

/*! Delete the container prototype */
void Extrusion::delete_prototype() 
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the container prototype */
Container_proto* Extrusion::get_prototype() 
{  
  if (!s_prototype) Extrusion::init_prototype();
  return s_prototype;
} 

SGAL_END_NAMESPACE

