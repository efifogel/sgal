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
  m_base_height(s_def_base_height)
{ if (m_is_closed) set_solid(false); }

//! \brief destructor.
Elevation_grid::~Elevation_grid() {}

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
  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initilalizes the container prototype.
void Elevation_grid::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Elevation_grid::structure_changed);

  // height
  auto height_func =
    reinterpret_cast<Float_array_handle_function>
    (&Elevation_grid::height_handle);
  s_prototype->add_field_info(new MF_float(HEIGHT, "height",
                                           Field_info::RULE_FIELD,
                                           height_func, exec_func));

  // xDimension
  Uint_handle_function x_dimension_func =
    static_cast<Uint_handle_function>(&Elevation_grid::x_dimension_handle);
  s_prototype->add_field_info(new SF_uint(X_DIMENSION, "xDimension",
                                          Field_info::RULE_FIELD,
                                          x_dimension_func, s_def_x_dimension,
                                          exec_func));


  // xSpacing
  Float_handle_function x_spacing_func =
    static_cast<Float_handle_function>(&Elevation_grid::x_spacing_handle);
  s_prototype->add_field_info(new SF_float(X_SPACING, "xSpacing",
                                           Field_info::RULE_FIELD,
                                           x_spacing_func, s_def_x_spacing,
                                           exec_func));

  // zDimension
  Uint_handle_function z_dimension_func =
    static_cast<Uint_handle_function>(&Elevation_grid::z_dimension_handle);
  s_prototype->add_field_info(new SF_uint(Z_DIMENSION, "zDimension",
                                          Field_info::RULE_FIELD,
                                          z_dimension_func, s_def_z_dimension,
                                          exec_func));

  // zSpacing
  Float_handle_function z_spacing_func =
    static_cast<Float_handle_function>(&Elevation_grid::z_spacing_handle);
  s_prototype->add_field_info(new SF_float(Z_SPACING, "zSpacing",
                                           Field_info::RULE_FIELD,
                                           z_spacing_func, s_def_z_spacing,
                                           exec_func));

  // is closed
  Boolean_handle_function is_closed_func =
    static_cast<Boolean_handle_function>(&Elevation_grid::is_closed_handle);
  s_prototype->add_field_info(new SF_bool(IS_CLOSED, "closed",
                                          Field_info::RULE_FIELD,
                                          is_closed_func,
                                          s_def_is_closed,
                                          exec_func));

  // baseHeight
  Float_handle_function base_height_func =
    static_cast<Float_handle_function>(&Elevation_grid::base_height_handle);
  s_prototype->add_field_info(new SF_float(BASE_HEIGHT, "baseHeight",
                                           Field_info::RULE_FIELD,
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
void Elevation_grid::set_height(Float_array& height)
{
  if (m_height == height) return;
  m_height = height;
  structure_changed(get_field_info(HEIGHT));
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
  if (m_is_closed) set_solid(false);
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
      auto y = m_height[i + j * m_x_dimension];
      min_y = (k == 0) ? y : std::min(y, min_y);
      (*coords)[k++].set(x, y, z);
    }
  }

  if (is_closed()) {
    Float y = min_y - get_base_height();
    Float x(0.0f);
    Float z(0.0f);
    (*coords)[k++].set(x, y, z);
    z = m_z_spacing * (m_z_dimension - 1);
    (*coords)[k++].set(x, y, z);
    x = m_x_spacing * (m_x_dimension - 1);
    (*coords)[k++].set(x, y, z);
    z = 0.0f;
    (*coords)[k++].set(x, y, z);
  }

  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief generates the coordinate indices.
void Elevation_grid::clean_flat_coord_indices()
{
  m_dirty_coord_indices = true;
  m_dirty_flat_coord_indices = false;
  m_coord_indices_flat = true;

  m_num_primitives = (m_x_dimension - 1) * (m_z_dimension - 1) * 2;
  if (is_closed()) m_num_primitives += (m_z_dimension + m_x_dimension) * 2 + 2;
  Uint size = m_num_primitives * 3;
  m_flat_coord_indices.resize(size);

  // Generate:
  size_t k(0);
  for (size_t j = 0; j < m_z_dimension-1; ++j) {
    for (size_t i = 0; i < m_x_dimension-1; ++i) {
      Uint ll = i + j * m_x_dimension;
      Uint lr = ll + 1;
      Uint ur = lr + m_z_dimension;
      Uint ul = ll + m_z_dimension;
      m_flat_coord_indices[k++] = ul;
      m_flat_coord_indices[k++] = ur;
      m_flat_coord_indices[k++] = lr;
      m_flat_coord_indices[k++] = ul;
      m_flat_coord_indices[k++] = lr;
      m_flat_coord_indices[k++] = ll;
    }
  }
  if (is_closed()) {
    // Front
    size_t anckor = m_x_dimension * m_z_dimension;
    for (size_t i = 0; i < m_x_dimension-1; ++i) {
      m_flat_coord_indices[k++] = anckor;
      m_flat_coord_indices[k++] = i+1;
      m_flat_coord_indices[k++] = i;
    }
    m_flat_coord_indices[k++] = anckor;
    m_flat_coord_indices[k++] = anckor+1;
    m_flat_coord_indices[k++] = m_x_dimension-1;

    // right
    ++anckor;
    for (size_t j = 0; j < m_z_dimension-1; ++j) {
      m_flat_coord_indices[k++] = anckor;
      m_flat_coord_indices[k++] = m_x_dimension*(j+2)-1;
      m_flat_coord_indices[k++] = m_x_dimension*(j+1)-1;
    }
    m_flat_coord_indices[k++] = anckor;
    m_flat_coord_indices[k++] = anckor+1;
    m_flat_coord_indices[k++] = m_x_dimension * m_z_dimension - 1;

    // back
    ++anckor;
    size_t base = m_x_dimension * (m_z_dimension - 1);
    for (size_t i = 0; i < m_x_dimension-1; ++i) {
      m_flat_coord_indices[k++] = anckor;
      m_flat_coord_indices[k++] = base + m_x_dimension - 2;
      m_flat_coord_indices[k++] = base + m_x_dimension - 1;
    }
    m_flat_coord_indices[k++] = anckor;
    m_flat_coord_indices[k++] = anckor+1;
    m_flat_coord_indices[k++] = base;

    // top
    ++anckor;
    for (size_t j = 0; j < m_z_dimension-1; ++j) {
      m_flat_coord_indices[k++] = anckor;
      m_flat_coord_indices[k++] = m_x_dimension*(m_z_dimension-j-2)-1;
      m_flat_coord_indices[k++] = m_x_dimension*(m_z_dimension-j-1)-1;
    }
    m_flat_coord_indices[k++] = anckor;
    m_flat_coord_indices[k++] = anckor+1;
    m_flat_coord_indices[k++] = 0;

    // bottom
    anckor = m_x_dimension * m_z_dimension;
    m_flat_coord_indices[k++] = anckor;
    m_flat_coord_indices[k++] = anckor+1;
    m_flat_coord_indices[k++] = anckor+2;
    m_flat_coord_indices[k++] = anckor;
    m_flat_coord_indices[k++] = anckor+2;
    m_flat_coord_indices[k++] = anckor+3;
  }

  set_primitive_type(PT_TRIANGLES);
}

//! calculates the default 2D texture-mapping oordinates.
void Elevation_grid::clean_tex_coord_array_2d()
{
  std::cout << "Not implemented yet!" << std::endl;
}

//! \brief processes change of structure.
void Elevation_grid::structure_changed(const Field_info* field_info)
{
  clear_coord_array();
  clear_flat_coord_indices();
  field_changed(field_info);
}

SGAL_END_NAMESPACE
