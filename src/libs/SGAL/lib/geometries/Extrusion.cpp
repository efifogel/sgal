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
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Extrusion::s_tag = "Extrusion";
Container_proto* Extrusion::s_prototype(NULL);

// Default values:
const Boolean Extrusion::s_def_begin_cap(true);
const Boolean Extrusion::s_def_end_cap(true);
const Boolean Extrusion::s_def_loop(false);
const Float Extrusion::s_def_cross_section_radius(0.1f);
const Uint Extrusion::s_def_cross_section_slices(32);

REGISTER_TO_FACTORY(Extrusion, "Extrusion");

//! \brief constructor.
Extrusion::Extrusion(Boolean proto) :
  Indexed_face_set(proto),
  m_begin_cap(s_def_begin_cap),
  m_end_cap(s_def_end_cap),
  m_loop(s_def_loop),
  m_cross_section_radius(s_def_cross_section_radius),
  m_cross_section_slices(s_def_cross_section_slices)
{}

//! \brief destructor.
Extrusion::~Extrusion()
{
  m_cross_section.clear();
  m_orientation.clear();
  m_scale.clear();
  m_spine.clear();
}

//! Clean the extrusion internal representation.
void Extrusion::clean()
{
  // Clear internal representation:
  if (!m_coord_array) m_coord_array.reset(new Coord_array_3d);
  boost::shared_ptr<Coord_array_3d> coord_array =
    boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coord_array);

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
  coord_array->resize(size);

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
      }
      else {
        prev_rot.set(hor, -SGAL_PI);
      }
    }
    else {
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
    }
    else {
      tmp_rot.set(hor, -SGAL_PI);
    }
  }
  else {
    tmp_rot.set(hor, 0);
  }
  rot.mult(tmp_rot, prev_rot);
  if (m_loop) {
    Rotation applied_rot;
    applied_rot.slerp(0.5f, prev_rot, rot);
    mat.make_rot(applied_rot.get_axis(), applied_rot.get_angle());
  }
  else {
    mat.make_rot(rot.get_axis(), rot.get_angle());
  }
  for (i = 0; i < m_cross_section.size(); ++i) {
    Vector3f point(m_cross_section[i][0], 0, m_cross_section[i][1]);
    if (m_scale.size() > 0) {
      point[0] *= m_scale[0][0];
      point[2] *= m_scale[0][1];
    }
    point.xform_pt(point, mat);

    (*coord_array)[k++].add(m_spine[0], point);
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
      }
      else {
        tmp_rot.set(hor, -SGAL_PI);
      }
    }
    else {
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
      (*coord_array)[k++].add(m_spine[j], point);
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
    }
    else {
      tmp_rot.set(hor, 0);
    }
    rot.mult(tmp_rot, prev_rot);
    Rotation applied_rot;
    applied_rot.slerp(0.5f, prev_rot, rot);
    mat.make_rot(applied_rot.get_axis(), applied_rot.get_angle());
  }
  else {
    mat.make_rot(rot.get_axis(), rot.get_angle());
  }
  for (i = 0; i < m_cross_section.size(); ++i) {
    Vector3f point(m_cross_section[i][0], 0, m_cross_section[i][1]);
    if (m_scale.size() > j) {
      point[0] *= m_scale[j][0];
      point[2] *= m_scale[j][1];
    }
    point.xform_pt(point, mat);
    (*coord_array)[k++].add(m_spine[j], point);
  }

  set_solid(m_loop || (m_begin_cap && m_end_cap));

  generate_coord_indices();

  Indexed_face_set::clean();
  Indexed_face_set::coord_point_changed();
}

//! \brief generates the coordinate indices.
void Extrusion::generate_coord_indices()
{
  //! \todo generate the indices flat to start with.
  // Generate all:
  Uint sections = (m_loop) ? m_spine.size() : m_spine.size() - 1;
  Uint offset = m_cross_section.size() * (m_spine.size() - 1);
  m_num_primitives = m_cross_section.size() * sections;
  Uint size = m_cross_section.size() * m_spine.size();
  size = m_num_primitives * 5;
  if (m_begin_cap) {
    size += m_cross_section.size() + 1;
    ++m_num_primitives;
  }
  if (m_end_cap) {
    size += m_cross_section.size() + 1;
    ++m_num_primitives;
  }

  Uint j, i, k = 0;
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

  clear_flat_coord_indices();
}

//! \brief sets the ellpsoid attributes.
void Extrusion::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
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

//! \brief initilalizes the container prototype.
void Extrusion::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  // Add the field-info records to the prototype:
  // beginCap
  Execution_function exec_func =
    static_cast<Execution_function>(&Extrusion::structure_changed);
  Boolean_handle_function begin_cap_func =
    static_cast<Boolean_handle_function>(&Extrusion::begin_cap_handle);
  s_prototype->add_field_info(new SF_bool(BEGIN_CAP, "beginCap",
                                          RULE_EXPOSED_FIELD,
                                          begin_cap_func,
                                          s_def_begin_cap, exec_func));

  // endCap
  Boolean_handle_function end_cap_func =
    static_cast<Boolean_handle_function>(&Extrusion::end_cap_handle);
  s_prototype->add_field_info(new SF_bool(END_CAP, "endCap",
                                          RULE_EXPOSED_FIELD,
                                          end_cap_func,
                                          s_def_end_cap, exec_func));

  // loop
  Boolean_handle_function loop_func =
    static_cast<Boolean_handle_function>(&Extrusion::loop_handle);
  s_prototype->add_field_info(new SF_bool(LOOP, "loop",
                                          RULE_EXPOSED_FIELD,
                                          loop_func,
                                          s_def_loop, exec_func));

  // crossSection
  Vector2f_array_handle_function cross_section_func =
    static_cast<Vector2f_array_handle_function>
    (&Extrusion::cross_section_handle);
  s_prototype->add_field_info(new MF_vector2f(CROSS_SECTION, "crossSection",
                                              RULE_EXPOSED_FIELD,
                                              cross_section_func, exec_func));

  // orientation
  Rotation_array_handle_function orientation_func =
    static_cast<Rotation_array_handle_function>
    (&Extrusion::orientation_handle);
  s_prototype->add_field_info(new MF_rotation(ORIENTATION, "orientation",
                                              RULE_EXPOSED_FIELD,
                                              orientation_func, exec_func));

  // scale
  Vector2f_array_handle_function scale_func =
    static_cast<Vector2f_array_handle_function>(&Extrusion::scale_handle);
  s_prototype->add_field_info(new MF_vector2f(SCALE, "scale",
                                              RULE_EXPOSED_FIELD,
                                              scale_func,
                                              exec_func));

  // spine
  Vector3f_array_handle_function spine_func =
    static_cast<Vector3f_array_handle_function>(&Extrusion::spine_handle);
  s_prototype->add_field_info(new MF_vector3f(SPINE, "spine",
                                              RULE_EXPOSED_FIELD,
                                              spine_func,
                                              exec_func));
}

//! \brief deletes the container prototype.
void Extrusion::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief sets the path that the cross section travels to create the shape.
void Extrusion::set_spine(std::vector<Vector3f>& spine)
{
  m_spine = spine;
  clear();
}

//! \brief sets the flag that specifies whether the spine is a closed loop.
void Extrusion::set_loop(Boolean loop)
{
  m_loop = loop;
  clear();
}

/*! \brief sets the 2-D cross section of the final shape defined in the XZ
 * plane.
 */
void Extrusion::set_cross_section(std::vector<Vector2f>& cross_section)
{
  m_cross_section = cross_section;
  clear();
}

//! \brief sets the cross section radius.
void Extrusion::set_cross_section_radius(Float radius)
{
  m_cross_section_radius = radius;
  clear();
}

/*! \brief sets the cross section slicess number. */
void Extrusion::set_cross_section_slices(Uint slices)
{
  m_cross_section_slices = slices;
  clear();
}

//! \brief sets the orientation of the cross section.
void Extrusion::set_orientation(std::vector<Rotation>& orientation)
{
  m_orientation = orientation;
  clear();
}

//! \brief sets the scale of the cross section.
void Extrusion::set_scale(std::vector<Vector2f>& scale)
{
  m_scale = scale;
  clear();
}

/*! \brief sets the flag that specifies whether the extruded shape is open at
 * the beginning.
 */
void Extrusion::set_begin_cap(Boolean begin_cap)
{
  m_begin_cap = begin_cap;
  clear_flat_indices();
}

/*! \brief sets the flag that specifies whether the extruded shape is open
 * at the end.
 */
void Extrusion::set_end_cap(Boolean end_cap)
{
  m_end_cap = end_cap;
  clear_flat_indices();
}

//! \brief obtains the container prototype.
Container_proto* Extrusion::get_prototype()
{
  if (!s_prototype) Extrusion::init_prototype();
  return s_prototype;
}

//! \brief processes change of structure.
void Extrusion::structure_changed(const Field_info* field_info)
{
  clear();
  field_changed(field_info);
}

//! calculates the default 2D texture-mapping oordinates.
void Extrusion::clean_tex_coords_2d()
{
  size_t stacks = (m_loop) ? m_spine.size() : m_spine.size() - 1;
  size_t slices = m_cross_section.size();

  // Texture coordinates
  Uint offset = (slices + 1) * (stacks + 1);
  Uint num_tex_coords = offset;
  if (m_begin_cap) num_tex_coords += m_cross_section.size();
  if (m_end_cap) num_tex_coords += m_cross_section.size();

  if (m_tex_coord_array) m_tex_coord_array->resize(num_tex_coords);
  else {
    m_tex_coord_array.reset(new Tex_coord_array_2d(num_tex_coords));
    SGAL_assertion(m_tex_coord_array);
  }
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);

  size_t i, j, k = 0;
  Float delta_s = 1.0f / slices;
  Float delta_t = 1.0f / stacks;
  for (i = 0; i <= stacks; ++i) {
    auto t = delta_t * i;
    for (j = 0; j <= slices; ++j) {
      auto s = delta_s * j;
      (*tex_coord_array)[k++].set(s, t);
    }
  }

  if (m_begin_cap || m_end_cap) {
    // Find ranges:
    auto s = m_cross_section[0][0];
    auto t = m_cross_section[0][1];
    Float s_min = s;
    Float s_max = s;
    Float t_min = t;
    Float t_max = t;
    for (j = 1; j < slices; ++j) {
      auto s = m_cross_section[j][0];
      auto t = m_cross_section[j][1];
      if (s < s_min) s_min = s;
      if (s > s_max) s_max = s;
      if (t < t_min) t_min = t;
      if (t > t_max) t_max = t;
    }
    auto s_range = s_max - s_min;
    auto t_range = t_max - t_min;

    if (m_begin_cap) {
      for (j = slices; j != 0; --j) {
        auto s = (m_cross_section[j-1][0] - s_min) / s_range;
        auto t = (m_cross_section[j-1][1] - t_min) / t_range;
        (*tex_coord_array)[k++].set(s, t);
      }
    }

    if (m_end_cap) {
      for (j = 0; j < slices; ++j) {
        auto s = (m_cross_section[j][0] - s_min) / s_range;
        auto t = (m_cross_section[j][1] - t_min) / t_range;
        (*tex_coord_array)[k++].set(s, t);
      }
    }
  }

  // Texture coordinate indices
  m_tex_coord_indices.resize(m_coord_indices.size());
  k = 0;

  for (j = 0; j < stacks; ++j) {
    Uint start = j * (slices + 1);
    for (i = 0; i < slices; ++i) {
      Uint ll = start + i;
      Uint ul = ll + (slices + 1);
      m_tex_coord_indices[k++] = ll;
      m_tex_coord_indices[k++] = ll + 1;
      m_tex_coord_indices[k++] = ul + 1;
      m_tex_coord_indices[k++] = ul;
      m_tex_coord_indices[k++] = static_cast<Uint>(-1);
    }
  }

  // Generate caps:
  if (m_begin_cap) {
    for (i = 0; i < slices; ++i) m_tex_coord_indices[k++] = offset++;
    m_tex_coord_indices[k++] = static_cast<Uint>(-1);
  }

  if (m_end_cap) {
    for (i = 0; i < slices; ++i) m_tex_coord_indices[k++] = offset++;
    m_tex_coord_indices[k++] = static_cast<Uint>(-1);
  }

  clear_flat_tex_coord_indices();
}

SGAL_END_NAMESPACE
