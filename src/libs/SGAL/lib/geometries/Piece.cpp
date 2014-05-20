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

#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Piece.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Piece::s_tag = "Piece";
Container_proto* Piece::s_prototype(NULL);

// Default values:
const Uint Piece::s_def_width(0);
const Uint Piece::s_def_height(0);
const Uint Piece::s_def_depth(0);
const Uint Piece::s_def_unit_size(1);

//! \brief constructor.
Piece::Piece(Boolean proto) :
  Indexed_face_set(proto),
  m_unit_size(s_def_unit_size)
{}

//! \brief destructor.
Piece::~Piece() { m_composition.clear(); }

//! \brief initializes the node prototype.
void Piece::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  // Add the field-info records to the prototype:
  // size
  Execution_function exec_func =
    static_cast<Execution_function>(&Piece::structure_changed);
  Uint_handle_function unit_size_func =
    static_cast<Uint_handle_function>(&Piece::unit_size_handle);
  s_prototype->add_field_info(new SF_uint(UNIT_SIZE, "size",
                                          RULE_EXPOSED_FIELD,
                                          unit_size_func,
                                          exec_func));
}

//! \brief deletes the node prototype.
void Piece::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the node prototype.
Container_proto* Piece::get_prototype()
{
  if (s_prototype == NULL) Piece::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this node.
void Piece::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  //! \todo sg->get_stats().add_num_mesh();

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "size") {
      m_unit_size = atoi(value.c_str());;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "width") {
      m_width = atoi(value.c_str());;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "height") {
      m_height = atoi(value.c_str());;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depth") {
      m_depth = atoi(value.c_str());;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "composition") {
      Uint num_values = get_num_tokens(value);
      m_composition.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_composition[i];
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! Clean the representation.
void Piece::clean()
{
  // Clear internal representation:
  if (!m_coord_array) m_coord_array.reset(new Coord_array_3d);

  // Generate points:
  Uint size = (m_width + 1) * (m_height + 1) * (m_depth + 1);
  m_coord_array->resize(size);

  boost::shared_ptr<Coord_array_3d> coord_array =
    boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coord_array);

  Uint i, j, k, l = 0;
  for (k = 0; k <= m_depth; ++k) {
    for (j = 0; j <= m_height; ++j) {
      for (i = 0; i <= m_width; ++i) {
        (*coord_array)[l++].set(static_cast<float>(i),
                                static_cast<float>(j),
                                static_cast<float>(k));
      }
    }
  }

  size = 0;
  l = 0;
  for (k = 0; k < m_depth; ++k) {
    for (j = 0; j < m_height; ++j) {
      for (i = 0; i < m_width; ++i) {
        if (m_composition[l]) {
          if ((k == 0) || (!m_composition[l - m_width * m_height])) ++size;
          if ((k == (m_depth - 1)) || (!m_composition[l + m_width * m_height]))
            ++size;
          if ((j == 0) || (!m_composition[l - m_width])) ++size;
          if ((j == (m_height - 1)) || (!m_composition[l + m_width])) ++size;
          if ((i == 0) || (!m_composition[l - 1])) ++size;
          if ((i == (m_width - 1)) || (!m_composition[l + 1])) ++size;
        }
        ++l;
      }
    }
  }

  m_num_primitives = size;

  set_solid(true);

  generate_coord_indices();

  Indexed_face_set::clean();
  Indexed_face_set::coord_point_changed();
}

//! \brief generates the coordinate index array.
void Piece::generate_coord_indices()
{
  //! \todo generate the indices flat to start with.
  m_coord_indices.resize(m_num_primitives * 5);
  Uint width = m_width + 1;
  Uint height = m_height + 1;
  Uint offset = height * width;
  Uint m = 0;
  Uint l = 0;
  for (Uint k = 0; k < m_depth; ++k) {
    for (Uint j = 0; j < m_height; ++j) {
      for (Uint i = 0; i < m_width; ++i) {
        if (m_composition[l]) {
          Uint start = i + width * (j + height * k);
          if ((k == 0) || (!m_composition[l - m_width * m_height])) {
            m_coord_indices[m++] = start;
            m_coord_indices[m++] = start + width;
            m_coord_indices[m++] = start + width + 1;
            m_coord_indices[m++] = start + 1;
            m_coord_indices[m++] = static_cast<Uint>(-1);
          }
          if ((k == (m_depth - 1)) || (!m_composition[l + m_width * m_height]))
          {
            m_coord_indices[m++] = offset + start;
            m_coord_indices[m++] = offset + start + 1;
            m_coord_indices[m++] = offset + start + width + 1;
            m_coord_indices[m++] = offset + start + width;
            m_coord_indices[m++] = static_cast<Uint>(-1);
          }
          if ((j == 0) || (!m_composition[l - m_width])) {
            m_coord_indices[m++] = start;
            m_coord_indices[m++] = start + 1;
            m_coord_indices[m++] = offset + start + 1;
            m_coord_indices[m++] = offset + start;
            m_coord_indices[m++] = static_cast<Uint>(-1);
          }
          if ((j == (m_height - 1)) || (!m_composition[l + m_width])) {
            m_coord_indices[m++] = start + width;
            m_coord_indices[m++] = offset + start + width;
            m_coord_indices[m++] = offset + start + width + 1;
            m_coord_indices[m++] = start + width + 1;
            m_coord_indices[m++] = static_cast<Uint>(-1);
          }
          if ((i == 0) || (!m_composition[l - 1])) {
            m_coord_indices[m++] = start;
            m_coord_indices[m++] = offset + start;
            m_coord_indices[m++] = offset + start + width;
            m_coord_indices[m++] = start + width;
            m_coord_indices[m++] = static_cast<Uint>(-1);
          }
          if ((i == (m_width - 1)) || (!m_composition[l + 1])) {
            m_coord_indices[m++] = start + 1;
            m_coord_indices[m++] = start + 1 + width;
            m_coord_indices[m++] = offset + start + 1 + width;
            m_coord_indices[m++] = offset + start + 1;
            m_coord_indices[m++] = static_cast<Uint>(-1);
          }
        }
        ++l;
      }
    }
  }

  clear_flat_coord_indices();
}

//! \brief processes change of structure.
void Piece::structure_changed(const Field_info* field_info)
{
  clear();
  field_changed(field_info);
}

SGAL_END_NAMESPACE
