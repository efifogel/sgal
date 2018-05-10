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

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Ellipsoid.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Ellipsoid::s_tag = "Ellipsoid";
Container_proto* Ellipsoid::s_prototype(nullptr);

// Default values:
const Float Ellipsoid::s_def_width(1);
const Float Ellipsoid::s_def_height(1);
const Float  Ellipsoid::s_def_depth(1);
const Uint  Ellipsoid::s_def_slices(8);
const Uint  Ellipsoid::s_def_stacks(4);

REGISTER_TO_FACTORY(Ellipsoid, "Ellipsoid");

//! \brief constructor.
Ellipsoid::Ellipsoid(Boolean proto) :
  Indexed_face_set(proto),
  m_width(s_def_width),
  m_height(s_def_height),
  m_depth(s_def_depth),
  m_slices(s_def_slices),
  m_stacks(s_def_stacks)
{}

//! \brief destructor.
Ellipsoid::~Ellipsoid(){}

//! \brief cleans the ellipsoid coordinate array and coordinate indices.
void Ellipsoid::clean_coords()
{
  m_dirty_coord_array = false;

  // Clear internal representation:
  if (!m_coord_array) m_coord_array.reset(new Coord_array_3d);

  // Generate points:
  auto size = 2 + m_slices * (m_stacks - 1);
  m_coord_array->resize(size);

  auto coord_array = boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coord_array);

  size_t k(0);
  for (size_t i = 0; i <= m_stacks; ++i) {
    if (i == 0) {
      Float x = m_width;
      Float y = 0;
      Float z = 0;
      (*coord_array)[k++] = Vector3f(x, y, z);
      continue;
    }
    if (i == m_stacks) {
      Float x = -m_width;
      Float y = 0;
      Float z = 0;
      (*coord_array)[k++] = Vector3f(x, y, z);
      continue;
    }
    Float theta = SGAL_PI * i / m_stacks;
    for (size_t j =  0; j < m_slices; ++j) {
      Float phi = SGAL_PI * j * 2 / m_slices;
      Float x = m_width * cosf(theta);
      Float y = m_height * sinf(theta) * cosf(phi);
      Float z = m_depth * sinf(theta) * sinf(phi);
      (*coord_array)[k++] = Vector3f(x, y, z);
    }
  }

  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief cleans the facet coordinate indices.
void Ellipsoid::clean_coord_indices()
{
  m_dirty_coord_indices = false;

  // Generate start:
  m_num_primitives = m_slices * m_stacks;
  auto size = 5 * m_stacks * m_slices - 2 * m_slices;
  m_coord_indices.resize(size);

  size_t k(0);
  size_t start(0);
  auto end = m_slices;
  for (size_t i = 1; i <= end; ++i) {
    auto j = i + 1;
    if (j == end + 1) j = 1;
    m_coord_indices[k++] = 0;
    m_coord_indices[k++] = i;
    m_coord_indices[k++] = j;
    m_coord_indices[k++] = static_cast<Uint>(-1);
  }

  // Gen middle
  for (size_t l = 1; l < m_stacks - 1; ++l) {
    auto start = m_slices * (l - 1) + 1;
    auto end = start + m_slices - 1;
    for (size_t i = start; i <= end; ++i) {
      auto a = i;
      auto b = i + 1;
      if (b == end + 1) b = start;
      auto c = b + m_slices;
      auto d = a + m_slices;
      m_coord_indices[k++] = b;
      m_coord_indices[k++] = a;
      m_coord_indices[k++] = d;
      m_coord_indices[k++] = c;
      m_coord_indices[k++] = static_cast<Uint>(-1);
    }
  }

  // Gen end
  start = m_slices * (m_stacks - 2) + 1;
  end = m_slices * (m_stacks - 1);
  for (size_t i  = start; i <= end; ++i) {
    auto j = i + 1;
    if (j == end + 1) j = start;
    m_coord_indices[k++] = end + 1;
    m_coord_indices[k++] = j;
    m_coord_indices[k++] = i;
    m_coord_indices[k++] = static_cast<Uint>(-1);
  }
}

//! \brief sets the attributes of this object.
void Ellipsoid::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "width") {
      set_width(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "height") {
      set_height(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depth") {
      set_depth(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    else if (name == "slices") {
      set_slices(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    else if (name == "stacks") {
      set_stacks(boost::lexical_cast<Uint>(value));
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
Attribute_list Ellipsoid::get_attributes()
{
  Attribute_list attribs;
  // attribs = Geometry::get_attributes();
  Attribue attrib;
  char buf[32];

  attribs = Indexed_face_set::get_attributes();

  if(get_bottom_radius() != s_def_bottom_radius) {
    attrib.first = "width";
    sprintf(buf, "%g", get_width());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if(get_height() != s_def_height) {
    attrib.first = "height";
    sprintf(buf, "%g", get_height());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if(get_slices() != s_def_slices) {
    attrib.first = "depth";
    sprintf(buf, "%g", get_depth());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if(get_stacks() != s_def_stacks) {
    attrib.first = "slices";
    sprintf(buf, "%d", get_stacks());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief initilalizes the container prototype.
void Ellipsoid::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  // Add the field-info records to the prototype:
  // width
  auto exec_func =
    static_cast<Execution_function>(&Ellipsoid::structure_changed);
  auto width_func = static_cast<Float_handle_function>(&Ellipsoid::width_handle);
  s_prototype->add_field_info(new SF_float(WIDTH, "width",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           width_func,
                                           s_def_width, exec_func));

  // height
  auto height_func =
    static_cast<Float_handle_function>(&Ellipsoid::height_handle);
  s_prototype->add_field_info(new SF_float(HEIGHT, "height",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           height_func,
                                           s_def_height, exec_func));

  // depth
  auto depth_func = static_cast<Float_handle_function>(&Ellipsoid::depth_handle);
  s_prototype->add_field_info(new SF_float(DEPTH, "depth",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           depth_func,
                                           s_def_depth, exec_func));

  // slices
  auto slices_func =
    static_cast<Uint_handle_function>(&Ellipsoid::slices_handle);
  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          slices_func,
                                          s_def_slices, exec_func));

  // stacks
  auto stacks_func =
    static_cast<Uint_handle_function>(&Ellipsoid::stacks_handle);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          stacks_func,
                                          s_def_stacks, exec_func));
}

//! \brief deletes the container prototype.
void Ellipsoid::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Ellipsoid::get_prototype()
{
  if (!s_prototype) Ellipsoid::init_prototype();
  return s_prototype;
}

//! \brief sets the width of the ellipsoid.
void Ellipsoid::set_width(Float width)
{
  m_width = width;
  m_dirty_bounding_sphere = true;
  structure_changed(get_field_info(WIDTH));
}

//! \brief sets the height of the ellipsoid.
void Ellipsoid::set_height(Float height)
{
  m_height = height;
  m_dirty_bounding_sphere = true;
  structure_changed(get_field_info(HEIGHT));
}

//! \brief sets the depth of the ellipsoid.
void Ellipsoid::set_depth(Float depth)
{
  m_depth = depth;
  m_dirty_bounding_sphere = true;
  structure_changed(get_field_info(DEPTH));
}

//! \brief processes change of structure.
void Ellipsoid::structure_changed(const Field_info* field_info)
{
  clear_coord_array();
  clear_coord_indices();
  field_changed(field_info);
}

SGAL_END_NAMESPACE
