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

#include "SGAL/basic.hpp"
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
Container_proto* Extrusion::s_prototype(nullptr);

// Default values:
const Boolean Extrusion::s_def_begin_cap(true);
const Boolean Extrusion::s_def_end_cap(true);
const Boolean Extrusion::s_def_loop(false);
const Boolean Extrusion::s_def_cross_section_closed(true);
const Float Extrusion::s_def_cross_section_radius(0.1f);
const Uint Extrusion::s_def_cross_section_slices(32);

REGISTER_TO_FACTORY(Extrusion, "Extrusion");

//! \brief constructs.
Extrusion::Extrusion(Boolean proto) :
  Indexed_face_set(proto),
  m_begin_cap(s_def_begin_cap),
  m_end_cap(s_def_end_cap),
  m_loop(s_def_loop),
  m_cross_section_closed(s_def_cross_section_closed),
  m_cross_section_radius(s_def_cross_section_radius),
  m_cross_section_slices(s_def_cross_section_slices)
{}

//! \brief destructs.
Extrusion::~Extrusion()
{
  m_cross_section.clear();
  m_orientation.clear();
  m_scale.clear();
  m_spine.clear();
}

//! \brief sets the extrusion attributes.
void Extrusion::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
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
    if (name == "crossSectionClosed") {
      set_cross_section_closed(compare_to_true(value));
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
      auto num_values = get_num_tokens(value);
      auto size = num_values / 2;
      m_scale.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (auto i = 0; i < size; i++) svalue >> m_scale[i][0] >> m_scale[i][1];
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
  auto exec_func =
    static_cast<Execution_function>(&Extrusion::structure_changed);

  // beginCap
  auto begin_cap_func =
    static_cast<Boolean_handle_function>(&Extrusion::begin_cap_handle);
  s_prototype->add_field_info(new SF_bool(BEGIN_CAP, "beginCap",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          begin_cap_func,
                                          s_def_begin_cap, exec_func));

  // endCap
  auto end_cap_func =
    static_cast<Boolean_handle_function>(&Extrusion::end_cap_handle);
  s_prototype->add_field_info(new SF_bool(END_CAP, "endCap",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          end_cap_func,
                                          s_def_end_cap, exec_func));

  // loop
  auto loop_func = static_cast<Boolean_handle_function>(&Extrusion::loop_handle);
  s_prototype->add_field_info(new SF_bool(LOOP, "loop",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          loop_func,
                                          s_def_loop, exec_func));

  // crossSection
  auto cross_section_func =
    static_cast<Vector2f_array_handle_function>
    (&Extrusion::cross_section_handle);
  s_prototype->add_field_info(new MF_vector2f(CROSS_SECTION, "crossSection",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              cross_section_func, exec_func));

  // orientation
  auto orientation_func =
    static_cast<Rotation_array_handle_function>
    (&Extrusion::orientation_handle);
  s_prototype->add_field_info(new MF_rotation(ORIENTATION, "orientation",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              orientation_func, exec_func));

  // scale
  auto scale_func =
    static_cast<Vector2f_array_handle_function>(&Extrusion::scale_handle);
  s_prototype->add_field_info(new MF_vector2f(SCALE, "scale",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              scale_func,
                                              exec_func));

  // spine
  auto spine_func =
    static_cast<Vector3f_array_handle_function>(&Extrusion::spine_handle);
  s_prototype->add_field_info(new MF_vector3f(SPINE, "spine",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              spine_func,
                                              exec_func));

  // cross section closed
  auto cross_section_closed_func =
    static_cast<Boolean_handle_function>
    (&Extrusion::cross_section_closed_handle);
  s_prototype->add_field_info(new SF_bool(CROSS_SECTION_CLOSED,
                                          "cross_section_closed",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          cross_section_closed_func,
                                          s_def_cross_section_closed,
                                          exec_func));

  // crossSectionRadius
  auto cross_section_radius_func =
    static_cast<Float_handle_function>(&Extrusion::cross_section_radius_handle);
  s_prototype->add_field_info(new SF_float(CROSS_SECTION_RADIUS,
                                          "crossSectionRadius",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           cross_section_radius_func,
                                           s_def_cross_section_radius,
                                           exec_func));

  // crossSectionSlices
  auto cross_section_slices_func =
    static_cast<Uint_handle_function>(&Extrusion::cross_section_slices_handle);
  s_prototype->add_field_info(new SF_uint(CROSS_SECTION_SLICES,
                                          "crossSectionSlices",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           cross_section_slices_func,
                                           s_def_cross_section_slices,
                                           exec_func));
}

//! \brief deletes the container prototype.
void Extrusion::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief sets the path that the cross section travels to create the shape.
void Extrusion::set_spine(std::vector<Vector3f>& spine)
{
  m_spine = spine;
  structure_changed(get_field_info(SPINE));
}

//! \brief sets the flag that specifies whether the spine is a closed loop.
void Extrusion::set_loop(Boolean loop)
{
  m_loop = loop;
  structure_changed(get_field_info(LOOP));
}

/*! \brief sets the 2-D cross section of the final shape defined in the XZ
 * plane.
 */
void Extrusion::set_cross_section(std::vector<Vector2f>& cross_section)
{
  m_cross_section = cross_section;
  structure_changed(get_field_info(CROSS_SECTION));
}

/*! \brief set the flag that indicates whether the cross section is a closed
 * loop.
 */
void Extrusion::set_cross_section_closed(Boolean closed)
{
  m_cross_section_closed = closed;
  structure_changed(get_field_info(CROSS_SECTION_CLOSED));
}

//! \brief sets the cross section radius.
void Extrusion::set_cross_section_radius(Float radius)
{
  m_cross_section_radius = radius;
  structure_changed(get_field_info(CROSS_SECTION_RADIUS));
}

/*! \brief sets the cross section slicess number. */
void Extrusion::set_cross_section_slices(Uint slices)
{
  m_cross_section_slices = slices;
  structure_changed(get_field_info(CROSS_SECTION_SLICES));
}

//! \brief sets the orientation of the cross section.
void Extrusion::set_orientation(std::vector<Rotation>& orientation)
{
  m_orientation = orientation;
  structure_changed(get_field_info(ORIENTATION));
}

//! \brief sets the scale of the cross section.
void Extrusion::set_scale(std::vector<Vector2f>& scale)
{
  m_scale = scale;
  structure_changed(get_field_info(SCALE));
}

/*! \brief sets the flag that specifies whether the extruded shape is open at
 * the beginning.
 */
void Extrusion::set_begin_cap(Boolean begin_cap)
{
  m_begin_cap = begin_cap;
  clear_facet_coord_indices();
  field_changed(get_field_info(BEGIN_CAP));
}

/*! \brief sets the flag that specifies whether the extruded shape is open
 * at the end.
 */
void Extrusion::set_end_cap(Boolean end_cap)
{
  m_end_cap = end_cap;
  clear_facet_coord_indices();
  field_changed(get_field_info(END_CAP));
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
  clear_coord_array();
  clear_facet_coord_indices();
  field_changed(field_info);
}

//! \brief generates the cross section (in case it is not provided).
void Extrusion::generate_cross_section()
{
  m_cross_section.resize(m_cross_section_slices + 1);
  SGAL_assertion(m_cross_section_slices != 0);
  Float delta = SGAL_TWO_PI / m_cross_section_slices;
  Float angle = 0;
  for (Uint i = 0; i < m_cross_section_slices; ++i) {
    auto x = m_cross_section_radius * sinf(angle);
    auto y = m_cross_section_radius * cosf(angle);
    angle += delta;
    m_cross_section[i].set(x, y);
  }
  m_cross_section[m_cross_section_slices] = m_cross_section[0];
}

//! \brief cleans the coordinate array.
void Extrusion::clean_coords()
{
  m_dirty_coord_array = false;

  if (!m_coord_array) m_coord_array.reset(new Coord_array_3d);
  auto coord_array = boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coord_array);

  // Generate (closed) cross section:
  if (m_cross_section.empty()) generate_cross_section();

  // Generate points:
  Boolean cross_section_loop =
    m_cross_section.front() == m_cross_section.back();
  size_t cross_section_size =
    (cross_section_loop) ? m_cross_section.size() - 1 : m_cross_section.size();

  Uint size = cross_section_size * m_spine.size();
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
  for (i = 0; i < cross_section_size; ++i) {
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
    for (i = 0; i < cross_section_size; ++i) {
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
  for (i = 0; i < cross_section_size; ++i) {
    Vector3f point(m_cross_section[i][0], 0, m_cross_section[i][1]);
    if (m_scale.size() > j) {
      point[0] *= m_scale[j][0];
      point[2] *= m_scale[j][1];
    }
    point.xform_pt(point, mat);
    (*coord_array)[k++].add(m_spine[j], point);
  }

  set_primitive_type(PT_TRIANGLES);
  set_solid(m_loop || (m_begin_cap && m_end_cap));

  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief adds triangle indices given four points that form a quad.
size_t Extrusion::add_triangle_indices(size_t k, Triangle_indices& indices,
                                       Uint ll, Uint lr, Uint ur, Uint ul)
{
  indices[k][0] = ll;
  indices[k][1] = lr;
  indices[k][2] = ur;
  ++k;
  indices[k][0] = ll;
  indices[k][1] = ur;
  indices[k][2] = ul;
  ++k;
  return k;
}

//! \brief cleans the coordinate indices.
void Extrusion::clean_facet_coord_indices()
{
  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;

  Boolean cross_section_loop =
    m_cross_section.front() == m_cross_section.back();
  size_t cross_section_size =
    (cross_section_loop) ? m_cross_section.size() - 1 : m_cross_section.size();

  Boolean cross_section_closed = m_cross_section_closed || cross_section_loop;
  size_t slices = (cross_section_closed) ?
    cross_section_size : cross_section_size + 1;

  // Count:
  Uint stacks = (m_loop) ? m_spine.size() : m_spine.size() - 1;
  m_num_primitives = slices * stacks * 2;
  // Caps are always closed
  if (m_begin_cap) m_num_primitives += cross_section_size - 2;
  if (m_end_cap) m_num_primitives += cross_section_size - 2;

  // Generate:
  auto& coord_indices = empty_triangle_coord_indices();
  coord_indices.resize(m_num_primitives);
  size_t j, i, k = 0;
  for (j = 0; j < m_spine.size() - 1; ++j) {
    Uint start = j * cross_section_size;
    for (i = 0; i < slices - 1; ++i) {
      Uint ll = start + i;
      Uint ul = ll + cross_section_size;
      Uint lr = ll + 1;
      Uint ur = ul + 1;
      k = add_triangle_indices(k, coord_indices, ll, lr, ur, ul);
    }
    Uint ll = start + i;
    Uint ul = ll + cross_section_size;
    Uint lr, ur;
    if (cross_section_closed) {
      lr = ll + 1 - cross_section_size;
      ur = ul + 1 - cross_section_size;
    }
    else {
      lr = ll + 1;
      ur = ul + 1;
    }
    k = add_triangle_indices(k, coord_indices, ll, lr, ur, ul);
  }

  if (m_loop) {
    Uint start = j * cross_section_size;
    for (i = 0; i < slices - 1; ++i) {
      Uint ll = start + i;
      Uint ul = i;
      Uint lr = ll + 1;
      Uint ur = ul + 1;
      k = add_triangle_indices(k, coord_indices, ll, lr, ur, ul);
    }
    Uint ll = start + i;
    Uint ul = i;
    Uint lr, ur;
    if (cross_section_closed) {
      lr = ll + 1 - cross_section_size;
      ur = ul + 1 - cross_section_size;
    }
    else {
      lr = ll + 1;
      ur = ul + 1;
    }
    k = add_triangle_indices(k, coord_indices, ll, lr, ur, ul);
  }

  // Generate caps:
  if (m_begin_cap) {
    Uint anchor = 0;
    for (i = 0; i < cross_section_size-2; ++i) {
      coord_indices[k][0] = anchor;
      coord_indices[k][1] = anchor + cross_section_size - i - 1;
      coord_indices[k][2] = anchor + cross_section_size - i - 2;
      ++k;
    }
  }

  if (m_end_cap) {
    Uint offset = cross_section_size * (m_spine.size() - 1);
    Uint anchor = offset;
    for (i = 0; i < cross_section_size-2; ++i) {
      coord_indices[k][0] = anchor;
      coord_indices[k][1] = anchor + i + 1;
      coord_indices[k][2] = anchor + i + 2;
      ++k;
    }
  }
}

//! calculates the default 2D texture-mapping oordinates.
void Extrusion::clean_tex_coord_array_2d()
{
  m_dirty_tex_coord_array = false;

  Boolean cross_section_loop =
    m_cross_section.front() == m_cross_section.back();
  size_t cross_section_size =
    (cross_section_loop) ? m_cross_section.size() - 1 : m_cross_section.size();

  Boolean cross_section_closed = m_cross_section_closed || cross_section_loop;
  size_t slices = (cross_section_closed) ?
    cross_section_size : cross_section_size + 1;
  size_t stacks = (m_loop) ? m_spine.size() : m_spine.size() - 1;

  // Texture coordinates
  auto num_tex_coords = (slices + 1) * (stacks + 1);
  if (m_begin_cap || m_end_cap) num_tex_coords += cross_section_size;

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
    for (j = 1; j < cross_section_size; ++j) {
      auto s = m_cross_section[j][0];
      auto t = m_cross_section[j][1];
      if (s < s_min) s_min = s;
      if (s > s_max) s_max = s;
      if (t < t_min) t_min = t;
      if (t > t_max) t_max = t;
    }
    auto s_range = s_max - s_min;
    auto t_range = t_max - t_min;

    for (j = 0; j < cross_section_size; ++j) {
      auto s = (m_cross_section[j][0] - s_min) / s_range;
      auto t = (m_cross_section[j][1] - t_min) / t_range;
      (*tex_coord_array)[k++].set(s, t);
    }
  }
}

//! Generate the texture coordinate indices.
void Extrusion::clean_facet_tex_coord_indices()
{
  m_dirty_tex_coord_indices = true;
  m_dirty_facet_tex_coord_indices = false;

  Boolean cross_section_loop =
    m_cross_section.front() == m_cross_section.back();
  size_t cross_section_size =
    (cross_section_loop) ? m_cross_section.size() - 1 : m_cross_section.size();

  Boolean cross_section_closed = m_cross_section_closed || cross_section_loop;
  size_t slices = (cross_section_closed) ?
    cross_section_size : cross_section_size + 1;
  size_t stacks = (m_loop) ? m_spine.size() : m_spine.size() - 1;

  auto& tex_coord_indices = empty_triangle_tex_coord_indices();
  tex_coord_indices.resize(m_num_primitives);
  size_t i, j, k = 0;

  for (j = 0; j < stacks; ++j) {
    Uint start = j * (slices + 1);
    for (i = 0; i < slices; ++i) {
      Uint ll = start + i;
      Uint ul = ll + (slices + 1);
      Uint lr = ll + 1;
      Uint ur = ul + 1;
      k = add_triangle_indices(k, tex_coord_indices, ll, lr, ur, ul);
    }
  }

  // Generate caps:
  Uint offset = (slices + 1) * (stacks + 1);
  if (m_begin_cap) {
    Uint anchor = offset;
    for (i = 0; i < cross_section_size-2; ++i) {
      tex_coord_indices[k][0] = anchor;
      tex_coord_indices[k][1] = anchor + cross_section_size - i - 1;
      tex_coord_indices[k][2] = anchor + cross_section_size - i - 2;
      ++k;
    }
  }

  if (m_end_cap) {
    Uint anchor = offset;
    for (i = 0; i < cross_section_size-2; ++i) {
      tex_coord_indices[k][0] = anchor;
      tex_coord_indices[k][1] = anchor + i + 1;
      tex_coord_indices[k][2] = anchor + i + 2;
      ++k;
    }
  }
}

SGAL_END_NAMESPACE
