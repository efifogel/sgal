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
#include "SGAL/Element.hpp"
#include "SGAL/Elevation_grid.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Coord_array_1d.hpp"
#include "SGAL/Vrml_formatter.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Elevation_grid::s_tag = "ElevationGrid";
Container_proto* Elevation_grid::s_prototype(nullptr);

// Default values:
const Uint Elevation_grid::s_def_x_dimension(0);
const Float Elevation_grid::s_def_x_spacing(1.0f);
const Uint Elevation_grid::s_def_z_dimension(0);
const Float Elevation_grid::s_def_z_spacing(1.0f);
const Boolean Elevation_grid::s_def_is_closed(false);
const Float Elevation_grid::s_def_base_height(1.0f);

//! \brief constructor.
Elevation_grid::Elevation_grid(Boolean proto) :
  Indexed_face_set(proto),
  m_x_dimension(s_def_x_dimension),
  m_x_spacing(s_def_x_spacing),
  m_z_dimension(s_def_z_dimension),
  m_z_spacing(s_def_z_spacing),
  m_is_closed(s_def_is_closed),
  m_base_height(s_def_base_height),
  m_dirty_height(true),
  m_dirty_height_map(true)
{ set_solid(m_is_closed); }

//! \brief destructor.
Elevation_grid::~Elevation_grid()
{
  Observer observer(this, get_field_info(HEIGHT_MAP));
  if (m_height_map) m_height_map->unregister_observer(observer);
}

//! \brief sets the ellpsoid attributes.
void Elevation_grid::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "height") {
      auto size = get_num_tokens(value);
      m_height.resize(size);
      std::istringstream svalue(value, std::istringstream::in);
      for (size_t i = 0; i < size; ++i) svalue >> m_height[i];
      m_dirty_height = false;
      m_dirty_height_map = true;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "xDimension") {
      set_x_dimension(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "xSpacing") {
      set_x_spacing(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "zDimension") {
      set_z_dimension(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "zSpacing") {
      set_z_spacing(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "closed") {
      set_closed(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "baseHeight") {
      set_base_height(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "heightMap") {
      auto height_map = boost::dynamic_pointer_cast<Coord_array_1d>(cont);
      set_height_map(height_map);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initilalizes the container prototype.
void Elevation_grid::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  // height
  auto exec_func =
    static_cast<Execution_function>(&Elevation_grid::height_changed);
  auto height_func =
    reinterpret_cast<Float_array_handle_function>
    (&Elevation_grid::height_handle);
  s_prototype->add_field_info(new MF_float(HEIGHT, "height",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           height_func, exec_func));

  // heightMap
  exec_func =
    static_cast<Execution_function>(&Elevation_grid::height_map_changed);
  auto height_map_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Elevation_grid::height_map_handle);
  s_prototype->add_field_info(new SF_shared_container(HEIGHT_MAP,
                                                      "heightMap",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      height_map_func,
                                                      exec_func));

  // xDimension
  exec_func =
    static_cast<Execution_function>(&Elevation_grid::structure_changed);
  auto x_dimension_func =
    static_cast<Uint_handle_function>(&Elevation_grid::x_dimension_handle);
  s_prototype->add_field_info(new SF_uint(X_DIMENSION, "xDimension",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          x_dimension_func, s_def_x_dimension,
                                          exec_func));


  // xSpacing
  auto x_spacing_func =
    static_cast<Float_handle_function>(&Elevation_grid::x_spacing_handle);
  s_prototype->add_field_info(new SF_float(X_SPACING, "xSpacing",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           x_spacing_func, s_def_x_spacing,
                                           exec_func));

  // zDimension
  auto z_dimension_func =
    static_cast<Uint_handle_function>(&Elevation_grid::z_dimension_handle);
  s_prototype->add_field_info(new SF_uint(Z_DIMENSION, "zDimension",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          z_dimension_func, s_def_z_dimension,
                                          exec_func));

  // zSpacing
  auto z_spacing_func =
    static_cast<Float_handle_function>(&Elevation_grid::z_spacing_handle);
  s_prototype->add_field_info(new SF_float(Z_SPACING, "zSpacing",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           z_spacing_func, s_def_z_spacing,
                                           exec_func));

  // is closed
  auto is_closed_func =
    static_cast<Boolean_handle_function>(&Elevation_grid::is_closed_handle);
  s_prototype->add_field_info(new SF_bool(IS_CLOSED, "closed",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_closed_func,
                                          s_def_is_closed,
                                          exec_func));

  // baseHeight
  auto base_height_func =
    static_cast<Float_handle_function>(&Elevation_grid::base_height_handle);
  s_prototype->add_field_info(new SF_float(BASE_HEIGHT, "baseHeight",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           base_height_func, s_def_base_height,
                                           exec_func));
}

//! \brief deletes the container prototype.
void Elevation_grid::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Elevation_grid::get_prototype()
{
  if (!s_prototype) Elevation_grid::init_prototype();
  return s_prototype;
}

//! \brief Set the 2D array that represents the height above a grid.
void Elevation_grid::set_height(const Float_array& height)
{
  m_height = height;
  m_dirty_height = false;
  m_dirty_height_map = true;
  structure_changed(get_field_info(HEIGHT));
}

//! \brief sets the height map.
void Elevation_grid::set_height_map(Shared_coord_array_1d height_map)
{
  if (m_height_map == height_map) return;

  Observer observer(this, get_field_info(HEIGHT_MAP));
  if (m_height_map) m_height_map->unregister_observer(observer);

  m_height_map = height_map;
  m_dirty_height_map = false;
  m_dirty_height = true;

  if (m_height_map) m_height_map->register_observer(observer);
  structure_changed(get_field_info(HEIGHT_MAP));
}

//! \brief Set the number of grid points along the x-dimension.
void Elevation_grid::set_x_dimension(Uint x_dimension)
{
  if (m_x_dimension == x_dimension) return;
  m_x_dimension = x_dimension;
  structure_changed(get_field_info(X_DIMENSION));
}

//! \brief Set the distance between two successive grid points along the
void Elevation_grid::set_x_spacing(Float x_spacing)
{
  if (m_x_spacing == x_spacing) return;
  m_x_spacing = x_spacing;
  structure_changed(get_field_info(X_SPACING));
}

//! \brief Set the number of grid points along the z-dimension.
void Elevation_grid::set_z_dimension(Uint z_dimension)
{
  if (m_z_dimension == z_dimension) return;
  m_z_dimension = z_dimension;
  structure_changed(get_field_info(Z_DIMENSION));
}

//! \brief Set the distance between two successive grid points along the
void Elevation_grid::set_z_spacing(Float z_spacing)
{
  if (m_z_spacing == z_spacing) return;
  m_z_spacing = z_spacing;
  structure_changed(get_field_info(Z_SPACING));
}

//! \brief turns on the flag that indicates whether the shape should be closed.
void Elevation_grid::set_closed()
{
  if (m_is_closed) return;
  m_is_closed = true;
  set_solid(true);
  structure_changed(get_field_info(IS_CLOSED));
}

//! \brief turns off the flag that indicates whether the shape should be closed.
void Elevation_grid::set_open()
{
  if (!m_is_closed) return;
  m_is_closed = false;
  set_solid(false);
  structure_changed(get_field_info(IS_CLOSED));
}

//! \brief set the flag that indicates whether the shape should be closed.
void Elevation_grid::set_closed(Boolean flag)
{
  if (flag == m_is_closed) return;
  m_is_closed = flag;
  set_solid(m_is_closed);
  structure_changed(get_field_info(IS_CLOSED));
}

//! \brief Set the height of the base in case the surface is closed.
void Elevation_grid::set_base_height(Float base_height)
{
  if (m_base_height == base_height) return;
  m_base_height = base_height;
  structure_changed(get_field_info(BASE_HEIGHT));
}

//! \brief cleans the representation.
void Elevation_grid::clean_coords()
{
  m_dirty_coord_array = false;

  if ((m_x_dimension == 0) || (m_z_dimension == 0)) return;
  auto height_map = get_height_map();
  if (!height_map) return;

  const auto& heights = height_map->get_array();
  if (!m_coord_array) m_coord_array.reset(new Coord_array_3d);
  auto coords = boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);

  Uint size = m_x_dimension * m_z_dimension;
  if (is_closed()) size += 4;
  coords->resize(size);

  Float min_y(0.0f);
  size_t k(0);
  for (size_t j = 0; j < m_z_dimension; ++j) {
    auto z = static_cast<Float>(m_z_spacing * j);
    for (size_t i = 0; i < m_x_dimension; ++i) {
      auto x = static_cast<Float>(m_x_spacing * i);
      auto y = heights[i + j * m_x_dimension];
      min_y = (k == 0) ? y : std::min(y, min_y);
      (*coords)[k++].set(x, y, z);
    }
  }

  if (is_closed()) {
    Float y = min_y - get_base_height();
    Float x(0.0f);
    Float z(0.0f);
    (*coords)[k++].set(x, y, z);
    x = m_x_spacing * (m_x_dimension - 1);
    (*coords)[k++].set(x, y, z);
    z = m_z_spacing * (m_z_dimension - 1);
    (*coords)[k++].set(x, y, z);
    x = 0.0f;
    (*coords)[k++].set(x, y, z);
  }

  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief generates the coordinate indices.
void Elevation_grid::clean_facet_coord_indices()
{
  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;

  if ((m_x_dimension == 0) || (m_z_dimension == 0)) return;

  m_num_primitives = (m_x_dimension - 1) * (m_z_dimension - 1) * 2;
  if (is_closed()) m_num_primitives += (m_z_dimension + m_x_dimension) * 2 + 2;
  auto& indices = get_empty_triangle_coord_indices();
  indices.resize(m_num_primitives);

  // Generate:
  size_t k(0);
  for (size_t j = 0; j < m_z_dimension-1; ++j) {
    for (size_t i = 0; i < m_x_dimension-1; ++i) {
      Uint ll = i + j * m_x_dimension;
      Uint lr = ll + 1;
      Uint ur = lr + m_z_dimension;
      Uint ul = ll + m_z_dimension;
      indices[k][0] = ul;
      indices[k][1] = ur;
      indices[k][2] = lr;
      ++k;
      indices[k][0] = ul;
      indices[k][1] = lr;
      indices[k][2] = ll;
      ++k;
    }
  }
  if (is_closed()) {
    // // Front
    size_t anckor = m_x_dimension * m_z_dimension;
    size_t base = m_x_dimension-1;
    for (size_t i = 0; i < m_x_dimension-1; ++i) {
      indices[k][0] = anckor;
      indices[k][1] = i;
      indices[k][2] = i+1;
      ++k;
    }
    indices[k][0] = anckor;
    indices[k][1] = base;
    indices[k][2] = anckor+1;
    ++k;

    // right
    ++anckor;
    base = m_x_dimension * m_z_dimension - 1;
    for (size_t j = 0; j < m_z_dimension-1; ++j) {
      indices[k][0] = anckor;
      indices[k][1] = m_x_dimension*(j+1)-1;
      indices[k][2] = m_x_dimension*(j+2)-1;
      ++k;
    }
    indices[k][0] = anckor;
    indices[k][1] = m_x_dimension * m_z_dimension - 1;
    indices[k][2] = anckor+1;
    ++k;

    // // back
    ++anckor;
    base = m_x_dimension * (m_z_dimension - 1);
    for (size_t i = 0; i < m_x_dimension-1; ++i) {
      indices[k][0] = anckor;
      indices[k][1] = base + i + 1;
      indices[k][2] = base + i;
      ++k;
    }
    indices[k][0] = anckor;
    indices[k][1] = base;
    indices[k][2] = anckor+1;
    ++k;

    // left
    ++anckor;
    for (size_t j = 0; j < m_z_dimension-1; ++j) {
      indices[k][0] = anckor;
      indices[k][1] = m_x_dimension*(j+1);
      indices[k][2] = m_x_dimension*j;
      ++k;
    }
    indices[k][0] = anckor;
    indices[k][1] = 0;
    indices[k][2] = anckor-3;
    ++k;

    // bottom
    anckor = m_x_dimension * m_z_dimension;
    indices[k][0] = anckor;
    indices[k][1] = anckor+1;
    indices[k][2] = anckor+2;
    ++k;
    indices[k][0] = anckor;
    indices[k][1] = anckor+2;
    indices[k][2] = anckor+3;
    ++k;
  }

  set_primitive_type(PT_TRIANGLES);
}

//! calculates the default 2D texture-mapping oordinates.
void Elevation_grid::clean_tex_coord_array_2d()
{
  std::cout << "Not implemented yet!" << std::endl;
}

//! \brief processes change of height.
void Elevation_grid::height_changed(const Field_info* field_info)
{
  m_dirty_height = false;
  m_dirty_height_map = true;
  structure_changed(field_info);
}

//! \brief processes change of height map.
void Elevation_grid::height_map_changed(const Field_info* field_info)
{
  // Observe that the observer, being a pair of this container object and the
  // field_info argument, is automatically unregistered as an observer
  // of the previous value of the m_height_map.
  if (m_height_map) {
    Observer observer(this, field_info);
    m_height_map->register_observer(observer);
  }

  m_dirty_height_map = false;
  m_dirty_height = true;
  structure_changed(field_info);
}

//! \brief processes change of structure.
void Elevation_grid::structure_changed(const Field_info* field_info)
{
  clear_coord_array();
  clear_facet_coord_indices();
  bounding_sphere_changed(field_info);
}

//! \brief obtain the (const) 2D array that represents the height above a grid.
Float_array& Elevation_grid::get_height()
{
  if (m_dirty_height) clean_height();
  return m_height;
}

//! \brief cleans the 2D array representing the height above the grid.
void Elevation_grid::clean_height()
{
  if (m_height_map) m_height = m_height_map->get_array();
  else m_height.clear();
  m_dirty_height = false;
}

//! \brief obtain the (const) 2D array that represents the height above a grid.
inline Elevation_grid::Shared_coord_array_1d Elevation_grid::get_height_map()
{
  if (m_dirty_height_map) clean_height_map();
  return m_height_map;
}

//! \brief cleans the 2D array representing the height above the grid.
void Elevation_grid::clean_height_map()
{
  if (!m_height_map) m_height_map.reset(new Coord_array_1d);
  m_height_map->set_array(m_height);
  m_dirty_height_map = false;
}

//! \brief exports this container.
void Elevation_grid::write(Formatter* formatter)
{
  if (m_dirty_height) clean_height();
  Indexed_face_set::write(formatter);
}

//! \brief exports a field of this container.
void Elevation_grid::write_field(const Field_info* field_info,
                                 Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    // Skip the height map gield.
    if (HEIGHT_MAP == field_info->get_id()) return;
  }
  Indexed_face_set::write_field(field_info, formatter);
}

//! \brief Process change of field.
void Elevation_grid::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case HEIGHT_MAP: height_map_changed(field_info); return;
   default: break;
  }
  Indexed_face_set::field_changed(field_info);
}

SGAL_END_NAMESPACE
