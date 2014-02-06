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
// $Id: $
// $Revision: 14184 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/Mesh_set.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Mesh_set::s_prototype(NULL);

const Boolean Mesh_set::s_def_is_solid(true);
const Boolean Mesh_set::s_def_is_convex(true);
const Boolean Mesh_set::s_def_is_ccw(true);
const Float Mesh_set::s_def_crease_angle(0);
const Float Mesh_set::s_def_polygon_offset_factor(0);

//! \brief constructor from prototype.
Mesh_set::Mesh_set(Boolean proto) :
  Geo_set(proto),
  m_dirty(true),
  m_is_ccw(s_def_is_ccw),
  m_is_solid(s_def_is_solid),
  m_is_convex(s_def_is_convex),
  m_crease_angle(s_def_crease_angle),
  m_polygon_offset_factor(s_def_polygon_offset_factor),
  m_dirty_flat_coord_indices(true),
  m_dirty_flat_normal_indices(true),
  m_dirty_flat_color_indices(true),
  m_dirty_flat_tex_coord_indices(true),
  m_coord_indices_flat(false),
  m_normal_indices_flat(false),
  m_color_indices_flat(false),
  m_tex_coord_indices_flat(false),
  m_dirty_coord_indices(false),
  m_dirty_normal_indices(false),
  m_dirty_color_indices(false),
  m_dirty_tex_coord_indices(false)
{}

//! Destructor.
Mesh_set::~Mesh_set() { clear_flat_indices(); }

//! \brief clears the indices arrays, e.g., forces their cleaning.
void Mesh_set::clear_flat_indices()
{
  clear_flat_coord_indices();
  clear_flat_normal_indices();
  clear_flat_color_indices();
  clear_flat_tex_coord_indices();
}

//! \brief sets the attributes of this object.
void Mesh_set::set_attributes(Element* elem)
{
  Geo_set::set_attributes(elem);

  typedef Element::Str_attr_iter Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "ccw") {
      m_is_ccw = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "solid") {
      m_is_solid = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "convex") {
      m_is_convex = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "creaseAngle") {
      m_crease_angle = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "polygonOffsetFactor") {
      m_polygon_offset_factor = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief draws the mesh conditionaly.
void Mesh_set::draw(Draw_action* action)
{
  if (is_dirty()) clean();
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  if (is_empty()) return;

  draw_mesh(action);
}

//! \brief draws the mesh.
void Mesh_set::draw_mesh(Draw_action* action)
{
  Context* context = action->get_context();
  if (!m_is_solid && context) {
    context->draw_cull_face(Gfx::NO_CULL);
    context->draw_light_model_sides(Gfx::TWO_SIDE);
  }

  if (!m_is_ccw) glFrontFace(GL_CW);
  if (has_scale()) glEnable(GL_NORMALIZE);

  if (m_polygon_offset_factor != 0) {
    glPolygonOffset(m_polygon_offset_factor, 1);
    glEnable(GL_POLYGON_OFFSET_FILL);
  }

  draw_geometry(action);

  if (m_polygon_offset_factor != 0) {
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0, 0);
  }

  if (!m_is_ccw) glFrontFace(GL_CCW);

  if (!m_is_solid && context) {
    context->draw_cull_face(Gfx::BACK_CULL);
    context->draw_light_model_sides(Gfx::ONE_SIDE);
  }

  if (has_scale()) glDisable(GL_NORMALIZE);
}

//! \brief calculates the sphere bound.
Boolean Mesh_set::clean_sphere_bound()
{
  if (is_dirty()) clean();
  // No need to clean the indices yet, cause the call bellow only uses the
  // coordinates.
  return Geo_set::clean_sphere_bound();
}

//! \brief sets the attributes of this node.
void Mesh_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());

  // Add the field-info records to the prototype:
  // ccw
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function is_ccw_func =
    static_cast<Boolean_handle_function>(&Mesh_set::is_ccw_handle);
  s_prototype->add_field_info(new SF_bool(CCW, "ccw", is_ccw_func,
                                          s_def_is_ccw, exec_func));

  // solid
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function is_solid_func =
    static_cast<Boolean_handle_function>(&Mesh_set::is_solid_handle);
  s_prototype->add_field_info(new SF_bool(SOLID, "solid", is_solid_func,
                                          s_def_is_solid, exec_func));

  // convex
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function is_convex_func =
    static_cast<Boolean_handle_function>(&Mesh_set::is_convex_handle);
  s_prototype->add_field_info(new SF_bool(CONVEX, "convex", is_convex_func,
                                          s_def_is_convex, exec_func));

  // creaseAngle
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function crease_angle_func =
    static_cast<Float_handle_function>(&Mesh_set::crease_angle_handle);
  s_prototype->add_field_info(new SF_float(CREASE_ANGLE, "creaseAngle",
                                           crease_angle_func,
                                           s_def_crease_angle, exec_func));
}

//! \brief deletes the container prototype.
void Mesh_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Mesh_set::get_prototype()
{
  if (!s_prototype) Mesh_set::init_prototype();
  return s_prototype;
}

//! \brief obtains the coord-index array.
Array<Uint>& Mesh_set::get_flat_coord_indices()
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  return m_flat_coord_indices;
}

//! \brief sets the flat coordinate-index array.
void Mesh_set::set_flat_coord_indices(Array<Uint>& indices)
{
  m_flat_coord_indices = indices;
  m_coord_indices_flat = true;
  m_dirty_flat_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief sets the coordinate-index array.
void Mesh_set::set_coord_indices(Array<Uint>& indices)
{
  Geo_set::set_coord_indices(indices);
  m_coord_indices_flat = false;
  m_dirty_flat_coord_indices = true;
  m_dirty_coord_indices = false;
}

/*! \brief flatten the coordinate index array.
 * In case of triangles or quads remove the '-1' end-of-polygon indication
 * from the index buffers. This operation changes the structure of the
 * index buffer, and must be reflected in the drawing routines.
 */
void Mesh_set::clean_flat_coord_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_coord_indices_flat && !m_coord_indices.empty())
  {
    flatten_indices(m_coord_indices, m_flat_coord_indices);
    m_coord_indices_flat = true;
  }
  m_dirty_flat_coord_indices = false;
}

//! \brief sets the flat normal-index array.
void Mesh_set::set_flat_normal_indices(Array<Uint>& indices)
{
  m_flat_normal_indices = indices;
  m_normal_indices_flat = true;
  m_dirty_flat_normal_indices = false;
  m_dirty_normal_indices = true;
}

//! \brief sets the normal-index array.
void Mesh_set::set_normal_indices(Array<Uint>& indices)
{
  Geo_set::set_normal_indices(indices);
  m_normal_indices_flat = false;
  m_dirty_flat_normal_indices = true;
  m_dirty_normal_indices = false;
}

//! \brief obtains the normal-index array.
Array<Uint>& Mesh_set::get_flat_normal_indices()
{
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  return m_flat_normal_indices;
}

/*! \brief flatten the normal index array.
 * In case of triangles or quads remove the '-1' end-of-polygon indication
 * from the index buffers. This operation changes the structure of the
 * index buffer, and must be reflected in the drawing routines.
 */
void Mesh_set::clean_flat_normal_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_normal_indices_flat && !m_normal_indices.empty() &&
      (m_normal_attachment == PER_VERTEX))
  {
    flatten_indices(m_normal_indices, m_flat_normal_indices);
    m_normal_indices_flat = true;
  }
  m_dirty_flat_normal_indices = false;
}

//! \brief sets the flat color-index array.
void Mesh_set::set_flat_color_indices(Array<Uint>& indices)
{
  m_flat_color_indices = indices;
  m_color_indices_flat = true;
  m_dirty_flat_color_indices = false;
  m_dirty_color_indices = true;
}

//! \brief sets the color-index array.
void Mesh_set::set_color_indices(Array<Uint>& indices)
{
  Geo_set::set_color_indices(indices);
  m_color_indices_flat = false;
  m_dirty_flat_color_indices = true;
  m_dirty_color_indices = false;
}

/*! \brief obtains the color-index array. */
Array<Uint>& Mesh_set::get_flat_color_indices()
{
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  return m_flat_color_indices;
}

/*! \brief flatten the color index array.
 * In case of triangles or quads remove the '-1' end-of-polygon indication
 * from the index buffers. This operation changes the structure of the
 * index buffer, and must be reflected in the drawing routines.
 */
void Mesh_set::clean_flat_color_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_color_indices_flat && !m_color_indices.empty() &&
      (m_color_attachment == PER_VERTEX))
  {
    flatten_indices(m_color_indices, m_flat_color_indices);
    m_color_indices_flat = true;
  }
  m_dirty_flat_color_indices = false;
}

//! \brief sets the flat texture coordinate-index array.
void Mesh_set::set_flat_tex_coord_indices(Array<Uint>& indices)
{
  m_flat_tex_coord_indices = indices;
  m_tex_coord_indices_flat = true;
  m_dirty_flat_tex_coord_indices = false;
  m_dirty_tex_coord_indices = true;
}

//! \brief sets the texture coordinate-index array.
void Mesh_set::set_tex_coord_indices(Array<Uint>& indices)
{
  Geo_set::set_tex_coord_indices(indices);
  m_tex_coord_indices_flat = false;
  m_dirty_flat_tex_coord_indices = true;
  m_dirty_tex_coord_indices = false;
}

//! \brief obtains the texture coordinate-index array.
Array<Uint>& Mesh_set::get_flat_tex_coord_indices()
{
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  return m_flat_tex_coord_indices;
}

/*! \brief flatten the texture coordinate index array.
 * In case of triangles or quads remove the '-1' end-of-polygon indication
 * from the index buffers. This operation changes the structure of the
 * index buffer, and must be reflected in the drawing routines.
 */
void Mesh_set::clean_flat_tex_coord_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_tex_coord_indices_flat && !m_tex_coord_indices.empty())
  {
    flatten_indices(m_tex_coord_indices, m_flat_tex_coord_indices);
    m_tex_coord_indices_flat = true;
  }
  m_dirty_flat_tex_coord_indices = false;
}

//! \brief converts non-flat indices (VRML style) to flat indices.
void  Mesh_set::flatten_indices(const Array<Uint>& src, Array<Uint>& dst)
{
  Uint size = (m_primitive_type == PT_TRIANGLES) ? m_num_primitives * 3 :
      (m_primitive_type == PT_QUADS) ? m_num_primitives * 4 : 0;
  dst.resize(size);
  const Uint* src_vector = src.get_vector();
  Uint* dst_vector = dst.get_vector();
  flatten_indices(src_vector, dst_vector);
}

//! \brief converts non-flat indices (VRML style) to flat indices.
void Mesh_set::flatten_indices(const Uint* src, Uint* dst)
{
  if (m_primitive_type == PT_TRIANGLES) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < m_num_primitives; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      ++k;
    }
    return;
  }
  if (m_primitive_type == PT_QUADS) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < m_num_primitives; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      ++k;
    }
    return;
  }
}

//! \brief assigns the coord indices with the reverse of given indices.
void Mesh_set::set_reverse_coord_indices(const Array<Uint>& indices)
{
  Geo_set::set_reverse_coord_indices(indices);
  m_coord_indices_flat = false;
  m_dirty_flat_coord_indices = true;
  m_dirty_coord_indices = false;
}

//! \brief assigns the flat coord indices with the reverse of given indices.
void Mesh_set::set_reverse_flat_coord_indices(const Array<Uint>& indices)
{
  m_flat_coord_indices.resize(indices.size());
  Uint i = 0;
  Uint j = indices.size();
  while (i < indices.size()) m_flat_coord_indices[i++] = indices[--j];
  m_coord_indices_flat = true;
  m_dirty_flat_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief converts flat indices to flat non-indices (VRML style).
void  Mesh_set::deflatten_indices(const Array<Uint>& src, Array<Uint>& dst)
{
  Uint size = (m_primitive_type == PT_TRIANGLES) ? m_num_primitives * 4 :
    (m_primitive_type == PT_QUADS) ? m_num_primitives * 5 : 0;
  dst.resize(size);
  // const Uint* src_vector = src.get_vector();
  // Uint* dst_vector = dst.get_vector();
  if (m_primitive_type == PT_TRIANGLES) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < m_num_primitives; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = -1;
    }
    return;
  }
  if (m_primitive_type == PT_QUADS) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < m_num_primitives; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = -1;
    }
    return;
  }
}

//! \brief cleans the coord-index array.
void Mesh_set::clean_coord_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_flat_coord_indices.empty())
    deflatten_indices(m_flat_coord_indices, m_coord_indices);
  m_dirty_coord_indices = false;
}

//! \brief cleans the normal-index array.
void Mesh_set::clean_normal_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_flat_normal_indices.empty())
    deflatten_indices(m_flat_normal_indices, m_normal_indices);
  m_dirty_normal_indices = false;
}

//! \brief cleans the color-index array.
void Mesh_set::clean_color_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_flat_color_indices.empty())
    deflatten_indices(m_flat_color_indices, m_color_indices);
  m_dirty_color_indices = false;
}

//! \brief cleans the texture coord-index array.
void Mesh_set::clean_tex_coord_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_flat_tex_coord_indices.empty())
    deflatten_indices(m_flat_tex_coord_indices, m_tex_coord_indices);
  m_dirty_tex_coord_indices = false;
}

//! \brief writes this container.
void Mesh_set::write(Formatter* formatter)
{
  if (m_dirty_coord_indices) clean_coord_indices();
  if (m_dirty_color_indices) clean_color_indices();
  if (m_dirty_normal_indices) clean_normal_indices();
  if (m_dirty_tex_coord_indices) clean_tex_coord_indices();

  /*! \todo the following can be replaced with the call to
   * Geo_set::write(Formatter* formatter)
   * after m_coord_indices is make Array<Int> (and not Array<Uint>).
   */
  formatter->container_begin(get_tag());
  // Travese prototype field-info records
  Container_proto* proto = get_prototype();
  Container_proto::Id_const_iterator it = proto->ids_begin(proto);
  for (; it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if (COORD_INDEX_ARRAY == field_info->get_id()) {
      const Array<Uint>& indices = get_coord_indices();
      Uint size = indices.size();
      Array<int> value(size), default_value;
      Array<Uint>::const_iterator it;
      Array<Int>::iterator rit = value.begin();
      for (it = indices.begin(); it != indices.end(); ++it)
        *rit++ = *it;
      formatter->multi_int(field_info->get_name(), value, default_value);
    }
    else field_info->write(this, formatter);
  }
  formatter->container_end();
}

SGAL_END_NAMESPACE
