// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include "basic.hpp"
#include "SGAL/Lines_set.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Tracer.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Obj_formatter.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Clean_lines_indices_visitor.hpp"
#include "SGAL/Clean_flat_lines_indices_visitor.hpp"
#include "SGAL/Resize_lines_indices_visitor.hpp"
#include "SGAL/Size_lines_indices_visitor.hpp"
#include "SGAL/Reverse_lines_indices_visitor.hpp"
#include "SGAL/Equal_indices_visitor.hpp"
#include "SGAL/Empty_indices_visitor.hpp"
#include "SGAL/Get_index_lines_indices_visitor.hpp"
#include "SGAL/Set_index_lines_indices_visitor.hpp"
#include "SGAL/Clear_lines_indices_visitor.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Lines_set::s_prototype(nullptr);

//! \brief constructs from prototype.
Lines_set::Lines_set(Boolean proto) :
  Geo_set(proto),
  m_dirty_lines_coord_indices(true),
  m_dirty_lines_normal_indices(true),
  m_dirty_lines_color_indices(true),
  m_dirty_lines_tex_coord_indices(true),
  m_dirty_coord_indices(true),
  m_dirty_normal_indices(true),
  m_dirty_color_indices(true),
  m_dirty_tex_coord_indices(true)
{}

//! \brief destructs.
Lines_set::~Lines_set() { clear_lines_indices(); }

//! \brief sets the attributes of this node.
void Lines_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());
}

//! \brief deletes the container prototype.
void Lines_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Lines_set::get_prototype()
{
  if (!s_prototype) Lines_set::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Lines_set::set_attributes(Element* elem)
{
  Geo_set::set_attributes(elem);

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief draws the mesh conditionaly.
void Lines_set::draw(Draw_action* action)
{
  if (is_dirty_lines_coord_indices()) clean_lines_coord_indices();
  if (m_lines_coord_indices.empty()) return;
  if (is_dirty_lines_normal_indices()) clean_lines_normal_indices();
  if (is_dirty_lines_color_indices()) clean_lines_color_indices();
  if (is_dirty_lines_tex_coord_indices()) clean_lines_tex_coord_indices();

  if (has_scale()) glEnable(GL_NORMALIZE);
  draw_lines(action);
  if (has_scale()) glDisable(GL_NORMALIZE);
}

//! \brief obtains the (const) coordinate indices.
std::vector<Int32>& Lines_set::get_coord_indices()
{
  if (is_dirty_coord_indices()) clean_coord_indices();
  return m_coord_indices;
}

//! \brief obtains the (const) normal indices.
std::vector<Int32>& Lines_set::get_normal_indices()
{
  if (is_dirty_normal_indices()) clean_normal_indices();
  return m_normal_indices;
}

//! \brief obtains the (const) color indices.
std::vector<Int32>& Lines_set::get_color_indices()
{
  if (is_dirty_color_indices()) clean_color_indices();
  return m_color_indices;
}

//! \brief obtains the (const) texture-coordinate indices.
std::vector<Int32>& Lines_set::get_tex_coord_indices()
{
  if (is_dirty_tex_coord_indices()) clean_tex_coord_indices();
  return m_tex_coord_indices;
}

//! \brief responds to a change in the coordinate indices.
void Lines_set::coord_indices_changed(const Field_info* field_info)
{
  m_dirty_lines_coord_indices = true;
  m_dirty_coord_indices = false;
  Geo_set::coord_indices_changed(field_info);
}

//! \brief responds to a change in the normal indices.
void Lines_set::normal_indices_changed(const Field_info* field_info)
{
  m_dirty_lines_normal_indices = true;
  m_dirty_normal_indices = false;
  Geo_set::normal_indices_changed(field_info);
}

//! \brief responds to a change in the color indices.
void Lines_set::color_indices_changed(const Field_info* field_info)
{
  m_dirty_lines_color_indices = true;
  m_dirty_color_indices = false;
  Geo_set::color_indices_changed(field_info);
}

//! \brief responds to a change in the texture-coordinate index array.
void Lines_set::tex_coord_indices_changed(const Field_info* field_info)
{
  m_dirty_lines_tex_coord_indices = true;
  m_dirty_tex_coord_indices = false;
  Geo_set::tex_coord_indices_changed(field_info);
}

//! \brief responds to a change in the lines coordinate indices.
void Lines_set::lines_coord_indices_changed()
{
  m_dirty_coord_indices = true;
  m_dirty_lines_coord_indices = false;
  Geo_set::coord_indices_changed();
}

//! \auto responds to a change in the lines normal indices.
void Lines_set::lines_normal_indices_changed()
{
  m_dirty_normal_indices = true;
  m_dirty_lines_normal_indices = false;
  Geo_set::normal_indices_changed();
}

//! \brief responds to a change in the lines color indices.
void Lines_set::lines_color_indices_changed()
{
  m_dirty_color_indices = true;
  m_dirty_lines_color_indices = false;
  Geo_set::color_indices_changed();
}

//! \brief responds to a change in the lines texture-coordinate index array.
void Lines_set::lines_tex_coord_indices_changed()
{
  m_dirty_tex_coord_indices = true;
  m_dirty_lines_tex_coord_indices = false;
  Geo_set::tex_coord_indices_changed();
}

//! \brief calculates the sphere bound.
void Lines_set::clean_bounding_sphere()
{
  if (m_bb_is_pre_set) {
    m_dirty_bounding_sphere = false;
    return;
  }

  // No need to clean the indices yet, cause the call bellow only uses the
  // coordinates.
  Geo_set::clean_bounding_sphere();
}

//! \brief obtains the empty lines coordinate indices.
Lines_indices& Lines_set::get_empty_lines_coord_indices()
{
  init_lines_coord_indices();
  return m_lines_coord_indices;
}

//! \brief obtains the empty lines normal indices.
Lines_indices& Lines_set::get_empty_lines_normal_indices()
{
  init_lines_normal_indices();
  return m_lines_coord_indices;
}

//! \brief obtains the empty lines color indices.
Lines_indices& Lines_set::get_empty_lines_color_indices()
{
  init_lines_color_indices();
  return m_lines_coord_indices;
}

//! \brief obtains the empty lines texture-coordinate indices.
Lines_indices& Lines_set::get_empty_lines_tex_coord_indices()
{
  init_lines_tex_coord_indices();
  return m_lines_coord_indices;
}

//! \brief obtains the line coord indices.
Line_indices& Lines_set::get_empty_line_coord_indices()
{
  m_lines_coord_indices = Line_indices();
  return boost::get<Line_indices>(m_lines_coord_indices);
}

//! \brief obtains the polyline coord indices.
Polyline_indices& Lines_set::get_empty_polyline_coord_indices()
{
  m_lines_coord_indices = Polyline_indices();
  return boost::get<Polyline_indices>(m_lines_coord_indices);
}

//! \brief obtains the flat coord indices.
Flat_indices& Lines_set::get_empty_flat_coord_indices()
{
  m_lines_coord_indices = Flat_indices();
  return boost::get<Flat_indices>(m_lines_coord_indices);
}

//! \brief obtains the line normal indices.
Line_indices& Lines_set::get_empty_line_normal_indices()
{
  m_lines_normal_indices = Line_indices();
  return boost::get<Line_indices>(m_lines_normal_indices);
}

//! \brief obtains the polyline normal indices.
Polyline_indices& Lines_set::get_empty_polyline_normal_indices()
{
  m_lines_normal_indices = Polyline_indices();
  return boost::get<Polyline_indices>(m_lines_normal_indices);
}

//! \brief obtains the flat normal indices.
Flat_indices& Lines_set::get_empty_flat_normal_indices()
{
  m_lines_normal_indices = Flat_indices();
  return boost::get<Flat_indices>(m_lines_normal_indices);
}

//! \brief obtains the line color indices.
Line_indices& Lines_set::get_empty_line_color_indices()
{
  m_lines_color_indices = Line_indices();
  return boost::get<Line_indices>(m_lines_color_indices);
}

//! \brief obtains the polyline color indices.
Polyline_indices& Lines_set::get_empty_polyline_color_indices()
{
  m_lines_color_indices = Polyline_indices();
  return boost::get<Polyline_indices>(m_lines_color_indices);
}

//! \brief obtains the flat color indices.
Flat_indices& Lines_set::get_empty_flat_color_indices()
{
  m_lines_color_indices = Flat_indices();
  return boost::get<Flat_indices>(m_lines_color_indices);
}

//! \brief obtains the line tex-coord indices.
Line_indices& Lines_set::get_empty_line_tex_coord_indices()
{
  m_lines_tex_coord_indices = Line_indices();
  return boost::get<Line_indices>(m_lines_tex_coord_indices);
}

//! \brief obtains the polyline tex-coord indices.
Polyline_indices& Lines_set::get_empty_polyline_tex_coord_indices()
{
  m_lines_tex_coord_indices = Polyline_indices();
  return boost::get<Polyline_indices>(m_lines_tex_coord_indices);
}

//! \brief obtains the flat tex-coord indices.
Flat_indices& Lines_set::get_empty_flat_tex_coord_indices()
{
  m_lines_tex_coord_indices = Flat_indices();
  return boost::get<Flat_indices>(m_lines_tex_coord_indices);
}

//! \brief obtains the lines coordinate indices.
const Lines_indices& Lines_set::get_lines_coord_indices()
{
  if (is_dirty_lines_coord_indices()) clean_lines_coord_indices();
  return m_lines_coord_indices;
}

//! \brief obtains the lines normal indices.
const Lines_indices& Lines_set::get_lines_normal_indices()
{
  if (is_dirty_lines_normal_indices()) clean_lines_normal_indices();
  return m_lines_normal_indices;
}

//! \brief obtains the lines color indices.
const Lines_indices& Lines_set::get_lines_color_indices()
{
  if (is_dirty_lines_color_indices()) clean_lines_color_indices();
  return m_lines_color_indices;
}

//! \brief obtains the lines texture coordinate indices.
const Lines_indices& Lines_set::get_lines_tex_coord_indices()
{
  if (is_dirty_lines_tex_coord_indices()) clean_lines_tex_coord_indices();
  return m_lines_tex_coord_indices;
}

//! \brief sets the lines coordinate indices.
void Lines_set::set_lines_coord_indices(const Lines_indices& indices)
{
  m_lines_coord_indices = indices;
  m_dirty_lines_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief sets the lines normal indices.
void Lines_set::set_lines_normal_indices(const Lines_indices& indices)
{
  m_lines_normal_indices = indices;
  m_dirty_lines_normal_indices = false;
  m_dirty_normal_indices = true;
}

//! \brief sets the lines color indices.
void Lines_set::set_lines_color_indices(const Lines_indices& indices)
{
  m_lines_color_indices = indices;
  m_dirty_lines_color_indices = false;
  m_dirty_color_indices = true;
}

//! \brief sets the lines texture coordinate indices.
void Lines_set::set_lines_tex_coord_indices(const Lines_indices& indices)
{
  m_lines_tex_coord_indices = indices;
  m_dirty_lines_tex_coord_indices = false;
  m_dirty_tex_coord_indices = true;
}

//! \brief sets the lines coordinate indices and dispose the input indices.
void Lines_set::set_lines_coord_indices(const Lines_indices&& indices)
{
  m_lines_coord_indices = indices;
  m_dirty_lines_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief sets the lines normal indices and dispose the input indices.
void Lines_set::set_lines_normal_indices(const Lines_indices&& indices)
{
  m_lines_normal_indices = indices;
  m_dirty_lines_normal_indices = false;
  m_dirty_normal_indices = true;
}

//! \brief sets the lines color indices and dispose the input indices.
void Lines_set::set_lines_color_indices(const Lines_indices&& indices)
{
  m_lines_color_indices = indices;
  m_dirty_lines_color_indices = false;
  m_dirty_color_indices = true;
}

/*! \brief sets the lines texture coordinate indices and dispose the input
 * indices.
 */
void Lines_set::set_lines_tex_coord_indices(const Lines_indices&& indices)
{
  m_lines_tex_coord_indices = indices;
  m_dirty_lines_tex_coord_indices = false;
  m_dirty_tex_coord_indices = true;
}

//! \brief cleans the lines coordinate-index vector.
void Lines_set::clean_lines_coord_indices()
{
  init_lines_coord_indices();
  if (!m_coord_indices.empty()) {
    Clean_lines_indices_visitor visitor(m_coord_indices, m_num_primitives);
    boost::apply_visitor(visitor, m_lines_coord_indices);
  }
  m_dirty_lines_coord_indices = false;
}

//! \brief cleans the lines normal indices.
void Lines_set::clean_lines_normal_indices()
{
  init_lines_normal_indices();
  if (!m_normal_indices.empty()) {
    Clean_lines_indices_visitor visitor(m_normal_indices, m_num_primitives);
    boost::apply_visitor(visitor, m_lines_normal_indices);
  }
  m_dirty_lines_normal_indices = false;
}

//! \brief cleans the color indices.
void Lines_set::clean_lines_color_indices()
{
  init_lines_color_indices();
  if (!m_color_indices.empty()) {
    Clean_lines_indices_visitor visitor(m_color_indices, m_num_primitives);
    boost::apply_visitor(visitor, m_lines_color_indices);
  }
  m_dirty_lines_color_indices = false;
}

//! \brief cleans the texture-coordinate indices.
void Lines_set::clean_lines_tex_coord_indices()
{
  init_lines_tex_coord_indices();
  if (!m_tex_coord_indices.empty()) {
    Clean_lines_indices_visitor visitor(m_tex_coord_indices,
                                             m_num_primitives);
    boost::apply_visitor(visitor, m_lines_tex_coord_indices);
  }
  m_dirty_lines_tex_coord_indices = false;
}

//! \brief clears the lines index arrays.
void Lines_set::clear_lines_indices()
{
  clear_lines_coord_indices();
  clear_lines_normal_indices();
  clear_lines_color_indices();
  clear_lines_tex_coord_indices();
}

//! \brief clears the coordinate indices.
void Lines_set::clear_lines_coord_indices()
{
  clear_lines_indices(m_lines_coord_indices);
  m_dirty_lines_coord_indices = true;
}

//! \brief clears the normal indices.
void Lines_set::clear_lines_normal_indices()
{
  clear_lines_indices(m_lines_normal_indices);
  m_dirty_lines_normal_indices = true;
}

//! \brief clears the color indices.
void Lines_set::clear_lines_color_indices()
{
  clear_lines_indices(m_lines_color_indices);
  m_dirty_lines_color_indices = true;
}

//! \brief clears the texture-coordinate indices.
void Lines_set::clear_lines_tex_coord_indices()
{
  clear_lines_indices(m_lines_tex_coord_indices);
  m_dirty_lines_tex_coord_indices = true;
}

//! \brief initializes the lines coordinate indices.
void Lines_set::init_lines_coord_indices()
{
  switch (m_primitive_type) {
   case PT_LINES: m_lines_coord_indices = Line_indices(); break;
   case PT_LINE_STRIPS: m_lines_coord_indices = Polyline_indices(); break;
   case PT_LINE_LOOPS: m_lines_coord_indices = Polyline_indices(); break;

   default: break;
  }
}

//! \brief initializes the normal coordinate indices.
void Lines_set::init_lines_normal_indices()
{
  if (m_normal_attachment == AT_PER_VERTEX) {
    switch (m_primitive_type) {
     case PT_LINES: m_lines_normal_indices = Line_indices(); return;
     case PT_LINE_STRIPS: m_lines_normal_indices = Polyline_indices(); return;
     case PT_LINE_LOOPS: m_lines_normal_indices = Polyline_indices(); return;

     default: return;
    }
  }
  m_lines_normal_indices = Flat_indices();
}

//! \brief initializes the color coordinate indices.
void Lines_set::init_lines_color_indices()
{
  if (m_color_attachment == AT_PER_VERTEX) {
    switch (m_primitive_type) {
     case PT_LINES: m_lines_color_indices = Line_indices(); return;
     case PT_LINE_STRIPS: m_lines_color_indices = Polyline_indices(); return;
     case PT_LINE_LOOPS: m_lines_color_indices = Polyline_indices(); return;

     default: return;
    }
  }
  m_lines_color_indices = Flat_indices();
}

//! \brief initializes the lines texture-coordinate indices.
void Lines_set::init_lines_tex_coord_indices()
{
  switch (m_primitive_type) {
   case PT_LINES: m_lines_tex_coord_indices = Line_indices(); break;
   case PT_LINE_STRIPS: m_lines_tex_coord_indices = Polyline_indices(); break;
   case PT_LINE_LOOPS: m_lines_tex_coord_indices = Polyline_indices(); break;

   default: break;
  }
}

//! \brief assigns the coord indices with the reverse of given indices.
void Lines_set::reverse_coord_indices(const std::vector<Int32>& source)
{
  Geo_set::reverse_coord_indices(source);
  coord_indices_changed();
}

//! \brief reverses the lines coordinate indices.
void Lines_set::reverse_lines_coord_indices()
{
  Reverse_lines_indices_visitor visitor;
  boost::apply_visitor(visitor, m_lines_coord_indices);
  lines_coord_indices_changed();
}

//! \brief assigns the lines coord indices with the reverse of given indices.
void Lines_set::reverse_lines_coord_indices(const Lines_indices& source)
{
  if (source == m_lines_coord_indices) {
    Reverse_lines_indices_visitor visitor;
    boost::apply_visitor(visitor, m_lines_coord_indices);
  }
  else {
    init_lines_coord_indices();
    Reverse_lines_indices_visitor visitor;
    boost::apply_visitor(visitor, m_lines_coord_indices, source);
  }
  lines_coord_indices_changed();
}

//! \brief cleans the coordinate indices.
void Lines_set::clean_coord_indices()
{
  clean_indices(m_coord_indices, m_lines_coord_indices);
  m_dirty_coord_indices = false;
  Geo_set::coord_indices_changed();
}

//! \brief cleans the normal indices.
void Lines_set::clean_normal_indices()
{
  clean_indices(m_normal_indices, m_lines_normal_indices);
  m_dirty_normal_indices = false;
  Geo_set::normal_indices_changed();
}

//! \brief validates (cleans) the color indices.
void Lines_set::clean_color_indices()
{
  clean_indices(m_color_indices, m_lines_color_indices);
  m_dirty_color_indices = false;
  Geo_set::color_indices_changed();
}

//! \brief cleans the texture coordinate indices.
void Lines_set::clean_tex_coord_indices()
{
  clean_indices(m_tex_coord_indices, m_lines_tex_coord_indices);
  m_dirty_tex_coord_indices = false;
  Geo_set::tex_coord_indices_changed();
}

//! \brief clears the coordinate index array.
void Lines_set::clear_coord_indices()
{
  m_coord_indices.clear();
  m_dirty_coord_indices = true;
  Geo_set::coord_indices_changed();

  //! todo remove!!!
  // coord_indices_changed();
}

//! \brief clears the normal indices.
void Lines_set::clear_normal_indices()
{
  m_normal_indices.clear();
  m_dirty_normal_indices = true;
  Geo_set::normal_indices_changed();
}

//! \brief Invalidate (clear) the color indices.
void Lines_set::clear_color_indices()
{
  m_color_indices.clear();
  m_dirty_color_indices = true;
  Geo_set::color_indices_changed();
}

//! \brief clears the texture-coordinate indices array.
void Lines_set::clear_tex_coord_indices()
{
  m_tex_coord_indices.clear();
  m_dirty_tex_coord_indices = true;
  Geo_set::tex_coord_indices_changed();
}

//! \brief writes this container.
void Lines_set::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Tracer::EXPORT,
                  std::cout << "Lines_set: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);

  if (is_dirty_lines_coord_indices()) clean_lines_coord_indices();
  if (empty_lines_indices(m_lines_coord_indices)) return;

  auto* obj_formatter = dynamic_cast<Obj_formatter*>(formatter);
  if (obj_formatter) {
    // Apply the active (top) transform matrix to the coordinates.
    const Matrix4f& matrix = obj_formatter->top_matrix();
    if (!m_coord_array || (m_coord_array->size() == 0)) return;
    std::vector<Vector3f> world_coords(m_coord_array->size());
    Uint i = 0;
    for (auto it = world_coords.begin(); it != world_coords.end(); ++it)
      it->xform_pt(get_coord_3d(i++), matrix);
    for (auto it = world_coords.begin(); it != world_coords.end(); ++it)
      obj_formatter->vertex(*it);

    // Export the liness.

    //! \todo use the following:
    // if (m_dirty_lines_color_indices) clean_lines_color_indices();
    // if (m_dirty_lines_normal_indices) clean_lines_normal_indices();
    // if (m_dirty_lines_tex_coord_indices) clean_lines_tex_coord_indices();

    if (PT_LINES == get_primitive_type()) {
      // const auto& indices = lines_coord_indices();
      // for (Uint i = 0; i < get_num_primitives(); ++i) {
      //   obj_formatter->lines(indices[i][0], indices[i][1]);
      // }
      SGAL_error_msg("Not impelmented yet!");
    }
    else if ((PT_LINE_STRIPS == get_primitive_type()) ||
             (PT_LINE_LOOPS == get_primitive_type()))
    {
      // const auto& indices = polyline_coord_indices();
      SGAL_error_msg("Not impelmented yet!");
    }
    obj_formatter->add_index(world_coords.size());
    return;
  }

  Vrml_formatter* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (m_dirty_coord_indices) clean_coord_indices();
    if (m_dirty_color_indices) clean_color_indices();
    if (m_dirty_normal_indices) clean_normal_indices();
    if (m_dirty_tex_coord_indices) clean_tex_coord_indices();
    if (m_coord_indices.empty()) return;

    Geo_set::write(formatter);
    return;
  }
  Geo_set::write(formatter);
}

//! \brief colapses identical coordinates.
void Lines_set::collapse_identical_coordinates()
{
  if (is_dirty_lines_coord_indices()) clean_lines_coord_indices();
  collapse_identical_coordinates(m_lines_coord_indices);
  m_dirty_coord_indices = true;
}

//! \brief colapses identical coordinates.
void Lines_set::collapse_identical_coordinates(std::vector<Int32>& indices)
{ SGAL_error_msg("Not implemented yet"); }

//! \brief colapses identical coordinates.
void Lines_set::collapse_identical_coordinates(Lines_indices& indices)
{
  typedef std::pair<const Vector3f*, Uint>      Coord_index_pair;
  if (empty_lines_indices(indices)) return;

  // Construct a vector of pairs of items, where the first item is a point and
  // the second item is the index into the indices array where the point was
  // indexed.
  std::vector<Coord_index_pair> vec(size_lines_indices(indices));
  std::vector<Coord_index_pair>::iterator it;
  Index_type i(0);
  for (it = vec.begin(); it != vec.end(); ++it) {
    auto index = get_index_lines_indices(m_lines_coord_indices, i);
    const Vector3f& vecf = get_coord_3d(index);
    *it = std::make_pair(&vecf, i++);
  }

  // Sort the vector lexicographically.
  std::sort(vec.begin(), vec.end(),
            [](const Coord_index_pair& a, const Coord_index_pair& b)
            {
              return ((b.first->less(*(a.first))) ? true :
                      ((*(a.first) == (*b.first)) ?
                       (a.second > b.second) : false));
            });

  // Count the number of distinct points.
  size_t num = 1;
  it = vec.begin();
  const Vector3f* prev = it->first;
  for (++it; it != vec.end(); ++it) {
    const Vector3f* curr = it->first;
    if ((*curr) != (*prev)) ++num;
    prev = curr;
  }

  // Allocate a new coords structure
  Coord_array_3d* coords = new Coord_array_3d(num);
  Shared_coord_array shared_coords(coords);

  // Initialize the newly created coords structure and update the indices
  i = 0;
  it = vec.begin();
  prev = it->first;
  Uint index = it->second;
  (*coords)[i] = *prev;
  set_index_lines_indices(m_lines_coord_indices, index, i);
  for (++it; it != vec.end(); ++it) {
    const Vector3f* curr = it->first;
    index = it->second;
    if ((*curr) != (*prev)) (*coords)[++i] = *curr;
    set_index_lines_indices(m_lines_coord_indices, index, i);
    prev = curr;
  }
  m_coord_array->clear();
  set_coord_array(shared_coords);
}

//! \brief obtains the bounding box.
Bounding_box Lines_set::bounding_box()
{
  if (is_dirty_lines_coord_indices()) clean_lines_coord_indices();
  if (empty_lines_indices(m_lines_coord_indices)) return Bounding_box();

  auto it = begin_lines_indices(m_lines_coord_indices);
  const Vector3f& v = get_coord_3d(*it);
  Bounding_box bbox(v[0], v[1], v[2], v[0], v[1], v[2]);
  for (++it; it != end_lines_indices(m_lines_coord_indices); ++it) {
    const Vector3f& v = get_coord_3d(*it);
    Bounding_box tmp(v[0], v[1], v[2], v[0], v[1], v[2]);
    bbox += tmp;
  }
  return bbox;
}

//! \brief resizes a lines indices structure with the same size as another.
void Lines_set::resize_lines_indices(Lines_indices& target,
                                     const Lines_indices& source)
{
  Resize_lines_indices_visitor visitor(target);
  boost::apply_visitor(visitor, source);
}

/*! \brief assigns a lines indices structure with a sequence of indices
 * starting at 0.
 */
void Lines_set::sequence_lines_indices(Lines_indices& indices)
{
  Sequence_lines_indices_visitor visitor;
  return boost::apply_visitor(visitor, indices);
}

//! \brief tests for equality two lines indices structures.
Boolean Lines_set::equal_lines_indices(const Lines_indices& other,
                                       const Lines_indices& source)
{
  Equal_indices_visitor visitor;
  return boost::apply_visitor(visitor, source, other);
}

//! \brief determines whether the given lines indices structure is empty.
Boolean Lines_set::empty_lines_indices(const Lines_indices& indices)
{
  Empty_indices_visitor visitor;
  return boost::apply_visitor(visitor, indices);
}

//! \brief obtains the number of entries in a lines indices structure.
size_t Lines_set::size_lines_indices(const Lines_indices& indices)
{
  Size_lines_indices_visitor visitor;
  return boost::apply_visitor(visitor, indices);
}

//! \brief obtain a begin iterator of a lines indices structure.
Lines_set::Lines_indices_const_iterator
Lines_set::begin_lines_indices(const Lines_indices& indices)
{
  Begin_lines_indices_visitor visitor;
  return boost::apply_visitor(visitor, indices);
}

//! \brief obtains a past-the-end iterator of a lines indices structure.
Lines_set::Lines_indices_const_iterator
Lines_set::end_lines_indices(const Lines_indices& indices)
{
  End_lines_indices_visitor visitor;
  return boost::apply_visitor(visitor, indices);
}

//! \brief clears a lines indices structure.
void Lines_set::clear_lines_indices(Lines_indices& indices)
{
  Clear_lines_indices_visitor visitor;
  boost::apply_visitor(visitor, indices);
}

//! \brief cleans an index array from a lines indices structure.
void Lines_set::clean_indices(std::vector<Int32>& indices,
                             const Lines_indices& source)
{
  Clean_flat_lines_indices_visitor visitor(indices);
  boost::apply_visitor(visitor, source);
}

//! \brief sets the value of an entry in a lines indices structure.
void Lines_set::set_index_lines_indices(Lines_indices& indices, size_t address,
                                       Index_type value)
{
  Set_index_lines_indices_visitor visitor(address, value);
  boost::apply_visitor(visitor, indices);
}

//! \brief gets the value of an entry in a lines indices structure.
Index_type
Lines_set::get_index_lines_indices(const Lines_indices& indices, size_t address)
{
  Get_index_lines_indices_visitor visitor(address);
  return boost::apply_visitor(visitor, indices);
}

SGAL_END_NAMESPACE
