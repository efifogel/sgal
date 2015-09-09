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

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include "basic.hpp"
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
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Obj_formatter.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Mesh_set::s_prototype(nullptr);

const Boolean Mesh_set::s_def_is_solid(true);
const Boolean Mesh_set::s_def_is_convex(true);
const Boolean Mesh_set::s_def_is_ccw(true);
const Float Mesh_set::s_def_crease_angle(0);
const Float Mesh_set::s_def_polygon_offset_factor(0);

//! \brief constructor from prototype.
Mesh_set::Mesh_set(Boolean proto) :
  Geo_set(proto),
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

//! \brief sets the attributes of this node.
void Mesh_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());

  // Add the field-info records to the prototype:
  // ccw
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto is_ccw_func =
    static_cast<Boolean_handle_function>(&Mesh_set::is_ccw_handle);
  s_prototype->add_field_info(new SF_bool(CCW, "ccw",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_ccw_func,
                                          s_def_is_ccw, exec_func));

  // solid
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto is_solid_func =
    static_cast<Boolean_handle_function>(&Mesh_set::is_solid_handle);
  s_prototype->add_field_info(new SF_bool(SOLID, "solid",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_solid_func,
                                          s_def_is_solid, exec_func));

  // convex
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto is_convex_func =
    static_cast<Boolean_handle_function>(&Mesh_set::is_convex_handle);
  s_prototype->add_field_info(new SF_bool(CONVEX, "convex",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_convex_func,
                                          s_def_is_convex, exec_func));

  // creaseAngle
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto crease_angle_func =
    static_cast<Float_handle_function>(&Mesh_set::crease_angle_handle);
  s_prototype->add_field_info(new SF_float(CREASE_ANGLE, "creaseAngle",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           crease_angle_func,
                                           s_def_crease_angle, exec_func));
}

//! \brief deletes the container prototype.
void Mesh_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Mesh_set::get_prototype()
{
  if (!s_prototype) Mesh_set::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Mesh_set::set_attributes(Element* elem)
{
  Geo_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
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
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_coord_indices.empty() && m_flat_coord_indices.empty()) return;
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();

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

//! \brief obtains the (const) coordinate-index array.
std::vector<Int32>& Mesh_set::get_coord_indices()
{
  if (is_dirty_coord_indices()) clean_coord_indices();
  return m_coord_indices;
}

//! \brief obtains the (const) normal-index array.
std::vector<Int32>& Mesh_set::get_normal_indices()
{
  if (is_dirty_normal_indices()) clean_normal_indices();
  return m_normal_indices;
}

//! \brief obtains the (const) color-index array.
std::vector<Int32>& Mesh_set::get_color_indices()
{
  if (is_dirty_color_indices()) clean_color_indices();
  return m_color_indices;
}

//! \brief obtains the (const) texture-coordinate-index array.
std::vector<Int32>& Mesh_set::get_tex_coord_indices()
{
  if (is_dirty_tex_coord_indices()) clean_tex_coord_indices();
  return m_tex_coord_indices;
}

//! \brief responds to a change in the coordinate-index array.
void Mesh_set::coord_indices_changed(const Field_info* field_info)
{
  m_coord_indices_flat = false;
  m_dirty_flat_coord_indices = true;
  m_dirty_coord_indices = false;
  Geo_set::coord_indices_changed(field_info);
}

//! \brief responds to a change in the normal-index array.
void Mesh_set::normal_indices_changed(const Field_info* field_info)
{
  m_normal_indices_flat = false;
  m_dirty_flat_normal_indices = true;
  m_dirty_normal_indices = false;
  Geo_set::normal_indices_changed(field_info);
}

//! \brief responds to a change in the color-index array.
void Mesh_set::color_indices_changed(const Field_info* field_info)
{
  m_color_indices_flat = false;
  m_dirty_flat_color_indices = true;
  m_dirty_color_indices = false;
  Geo_set::color_indices_changed(field_info);
}

//! \brief responds to a change in the texture-coordinate index array.
void Mesh_set::tex_coord_indices_changed(const Field_info* field_info)
{
  m_tex_coord_indices_flat = false;
  m_dirty_flat_tex_coord_indices = true;
  m_dirty_tex_coord_indices = false;
  Geo_set::tex_coord_indices_changed(field_info);
}

//! \brief calculates the sphere bound.
void Mesh_set::clean_bounding_sphere()
{
  if (m_bb_is_pre_set) {
    m_dirty_bounding_sphere = false;
    return;
  }

  // No need to clean the indices yet, cause the call bellow only uses the
  // coordinates.
  Geo_set::clean_bounding_sphere();
}

//! \brief obtains the flat coordinate-index array.
Mesh_set::Index_array& Mesh_set::get_flat_coord_indices()
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  return m_flat_coord_indices;
}

//! \brief obtains the flat normal-index array.
Mesh_set::Index_array& Mesh_set::get_flat_normal_indices()
{
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  return m_flat_normal_indices;
}

/*! \brief obtains the flat color-index array. */
Mesh_set::Index_array& Mesh_set::get_flat_color_indices()
{
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  return m_flat_color_indices;
}

//! \brief obtains the flat texture coordinate-index array.
Mesh_set::Index_array& Mesh_set::get_flat_tex_coord_indices()
{
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  return m_flat_tex_coord_indices;
}

//! \brief sets the flat coordinate-index array.
void Mesh_set::set_flat_coord_indices(const Index_array& indices)
{
  m_flat_coord_indices = indices;
  m_coord_indices_flat = true;
  m_dirty_flat_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief sets the flat normal-index array.
void Mesh_set::set_flat_normal_indices(const Index_array& indices)
{
  m_flat_normal_indices = indices;
  m_normal_indices_flat = true;
  m_dirty_flat_normal_indices = false;
  m_dirty_normal_indices = true;
}

//! \brief sets the flat color-index array.
void Mesh_set::set_flat_color_indices(const Index_array& indices)
{
  m_flat_color_indices = indices;
  m_color_indices_flat = true;
  m_dirty_flat_color_indices = false;
  m_dirty_color_indices = true;
}

//! \brief sets the flat texture coordinate-index array.
void Mesh_set::set_flat_tex_coord_indices(const Index_array& indices)
{
  m_flat_tex_coord_indices = indices;
  m_tex_coord_indices_flat = true;
  m_dirty_flat_tex_coord_indices = false;
  m_dirty_tex_coord_indices = true;
}

//! \brief cleans the flat coordinate-index array.
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

//! \brief cleans the flat normal-index array.
void Mesh_set::clean_flat_normal_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_normal_indices_flat && !m_normal_indices.empty() &&
      (m_normal_attachment == AT_PER_VERTEX))
  {
    flatten_indices(m_normal_indices, m_flat_normal_indices);
    m_normal_indices_flat = true;
  }
  m_dirty_flat_normal_indices = false;
}

//! \brief cleans the color-index array.
void Mesh_set::clean_flat_color_indices()
{
  if (((m_primitive_type == PT_TRIANGLES) || (m_primitive_type == PT_QUADS)) &&
      !m_color_indices_flat && !m_color_indices.empty() &&
      (m_color_attachment == AT_PER_VERTEX))
  {
    flatten_indices(m_color_indices, m_flat_color_indices);
    m_color_indices_flat = true;
  }
  m_dirty_flat_color_indices = false;
}

//! \brief cleans the texture-mapping coordinate-index array.
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

//! \brief clears the flat index arrays.
void Mesh_set::clear_flat_indices()
{
  clear_flat_coord_indices();
  clear_flat_normal_indices();
  clear_flat_color_indices();
  clear_flat_tex_coord_indices();
}

//! \brief clears the cordinate-index array.
void Mesh_set::clear_flat_coord_indices()
{
  m_flat_coord_indices.clear();
  m_coord_indices_flat = false;
  m_dirty_flat_coord_indices = true;
}

//! \brief clears the normal-index array.
void Mesh_set::clear_flat_normal_indices()
{
  m_flat_normal_indices.clear();
  m_normal_indices_flat = false;
  m_dirty_flat_normal_indices = true;
}

//! \brief clears the color-index array.
void Mesh_set::clear_flat_color_indices()
{
  m_color_indices_flat = false;
  m_dirty_flat_color_indices = true;
}

//! \brief clears the texture-mapping cordinate-index array.
void Mesh_set::clear_flat_tex_coord_indices()
{
  m_flat_tex_coord_indices.clear();
  m_tex_coord_indices_flat = false;
  m_dirty_flat_tex_coord_indices = true;
}

//! \brief converts non-flat indices (VRML style) to flat indices.
void  Mesh_set::flatten_indices(const std::vector<Int32>& src,
                                Index_array& dst)
{
  Uint size = (m_primitive_type == PT_TRIANGLES) ? m_num_primitives * 3 :
      (m_primitive_type == PT_QUADS) ? m_num_primitives * 4 : 0;
  dst.resize(size);
  if (m_primitive_type == PT_TRIANGLES) {
    auto sit = src.begin();
    auto dit = dst.begin();
    for (Uint j = 0; j < m_num_primitives; ++j) {
      *dit++ = *sit++;
      *dit++ = *sit++;
      *dit++ = *sit++;
      ++sit;
    }
    return;
  }
  if (m_primitive_type == PT_QUADS) {
    auto sit = src.begin();
    auto dit = dst.begin();
    for (Uint j = 0; j < m_num_primitives; ++j) {
      *dit++ = *sit++;
      *dit++ = *sit++;
      *dit++ = *sit++;
      *dit++ = *sit++;
      ++sit;
    }
    return;
  }
}

//! \brief assigns the coord indices with the reverse of given indices.
void Mesh_set::set_reverse_coord_indices(const std::vector<Int32>& indices)
{
  Geo_set::set_reverse_coord_indices(indices);
  m_coord_indices_flat = false;
  m_dirty_flat_coord_indices = true;
  m_dirty_coord_indices = false;
}

//! \brief assigns the flat coord indices with the reverse of given indices.
void Mesh_set::set_reverse_flat_coord_indices(const Index_array& indices)
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
void  Mesh_set::deflatten_indices(const Index_array& src,
                                  std::vector<Int32>& dst)
{
  Uint size = (m_primitive_type == PT_TRIANGLES) ? m_num_primitives * 4 :
    (m_primitive_type == PT_QUADS) ? m_num_primitives * 5 : 0;
  dst.resize(size);
  if (m_primitive_type == PT_TRIANGLES) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < m_num_primitives; ++j) {
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = -1;
    }
    return;
  }
  if (m_primitive_type == PT_QUADS) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < m_num_primitives; ++j) {
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = static_cast<Index_type>(src[k++]);
      dst[i++] = -1;
    }
    return;
  }
}

// Index Array Handling
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

//! \brief validates (cleans) the color-index array.
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

//! \brief clears the coordinate index array.
void Mesh_set::clear_coord_indices()
{
  m_dirty_coord_indices = true;
  m_coord_indices.clear();
}

//! \brief clears the normal-index array.
void Mesh_set::clear_normal_indices()
{
  m_dirty_normal_indices = true;
  m_normal_indices.clear();
}

//! \brief Invalidate (clear) the color-index array.
void Mesh_set::clear_color_indices()
{
  m_dirty_color_indices = true;
  m_color_indices.clear();
}

//! \brief clears the texture-mapping coordinate-index array.
void Mesh_set::clear_tex_coord_indices()
{
  m_dirty_tex_coord_indices = true;
  m_tex_coord_indices.clear();
}

//! \brief writes this container.
void Mesh_set::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Trace::EXPORT,
                  std::cout << "Mesh_set: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);

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
    // Export the facets.
    if ((PT_TRIANGLES == get_primitive_type()) ||
        (PT_QUADS == get_primitive_type()))
    {
      if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
      if (m_flat_coord_indices.empty()) return;

      //! \todo use the following:
      // if (m_dirty_flat_color_indices) clean_flat_color_indices();
      // if (m_dirty_flat_normal_indices) clean_flat_normal_indices();
      // if (m_dirty_flat_tex_coord_indices) clean_flat_tex_coord_indices();

      const auto& indices = get_flat_coord_indices();
      Uint j = 0;
      for (Uint i = 0; i < get_num_primitives(); ++i) {
        if (PT_TRIANGLES == get_primitive_type()) {
          if (is_ccw())
            obj_formatter->triangle(indices[j], indices[j+1], indices[j+2]);
          else obj_formatter->triangle(indices[j+2], indices[j+1], indices[j]);
          j += 3;
        }
        else {
          if (is_ccw())
            obj_formatter->quad(indices[j], indices[j+1], indices[j+2],
                                indices[j+3]);
          else
            obj_formatter->quad(indices[j+3], indices[j+2], indices[j+1],
                                indices[j]);
          j += 4;
        }
      }
    }
    else {
      if (m_dirty_coord_indices) clean_coord_indices();
      if (m_coord_indices.empty()) return;

      //! \todo use the following:
      // if (m_dirty_color_indices) clean_color_indices();
      // if (m_dirty_normal_indices) clean_normal_indices();
      // if (m_dirty_tex_coord_indices) clean_tex_coord_indices();

      //! \todo triangulate and export.
      const auto& indices = get_coord_indices();
      std::cout << "size: " << indices.size() << std::endl;
      const auto& flat_indices = get_flat_coord_indices();
      std::cout << "flat size: " << flat_indices.size() << std::endl;
      SGAL_error_msg("Not impelmented yet!");
    }
    obj_formatter->add_index(world_coords.size());
    return;
  }

  Stl_formatter* stl_formatter = dynamic_cast<Stl_formatter*>(formatter);
  if (stl_formatter) {
    // Apply the active (top) transform matrix to the coordinates.
    const Matrix4f& matrix = stl_formatter->top_matrix();
    if (!m_coord_array || (m_coord_array->size() == 0)) return;
    std::vector<Vector3f> world_coords(m_coord_array->size());
    std::vector<Vector3f>::iterator it;
    Uint i = 0;
    for (it = world_coords.begin(); it != world_coords.end(); ++it)
      it->xform_pt(get_coord_3d(i++), matrix);

    // Export the facets.
    if ((PT_TRIANGLES == get_primitive_type()) ||
        (PT_QUADS == get_primitive_type()))
    {
      if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
      if (m_flat_coord_indices.empty()) return;

      const auto& indices = get_flat_coord_indices();
      Uint j = 0;
      for (Uint i = 0; i < get_num_primitives(); ++i) {
        const Vector3f& v1 = world_coords[indices[j++]];
        const Vector3f& v2 = world_coords[indices[j++]];
        const Vector3f& v3 = world_coords[indices[j++]];
        if (PT_TRIANGLES == get_primitive_type()) {
          if (is_ccw()) write_facet(v1, v2, v3, stl_formatter);
          else write_facet(v3, v2, v1, stl_formatter);
        }
        else {
          const Vector3f& v4 = world_coords[indices[j++]];
          SGAL_assertion(PT_QUADS == get_primitive_type());
          if (is_ccw()) write_facet(v1, v2, v3, v4, stl_formatter);
          else write_facet(v4, v3, v2, v1, stl_formatter);
        }
      }
    }
    else {
      if (m_dirty_coord_indices) clean_coord_indices();
      if (m_coord_indices.empty()) return;

      //! \todo triangulate and export.
      const auto& indices = get_coord_indices();
      std::cout << "size: " << indices.size() << std::endl;
      const auto& flat_indices = get_flat_coord_indices();
      std::cout << "flat size: " << flat_indices.size() << std::endl;
      SGAL_error_msg("Not impelmented yet!");
    }
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

//! \brief writes a field of this container.
void Mesh_set::write_field(const Field_info* field_info, Formatter* formatter)
{
  /*! \todo the following can be replaced with the call to
   * Geo_set::write(Formatter* formatter)
   * after m_coord_indices is made Array<Int> (and not Array<Uint>).
   */

  auto* vrml_formatter = static_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (COORD_INDEX_ARRAY == field_info->get_id()) {
      const auto& indices = get_coord_indices();
      Uint size = indices.size();
      std::vector<int> value(size), default_value;
      auto rit = value.begin();
      for (auto it = indices.begin(); it != indices.end(); ++it) *rit++ = *it;
      formatter->multi_int32(field_info->get_name(), value, default_value);
      return;
    }
    if (NORMAL_INDEX_ARRAY == field_info->get_id()) {
      const auto& indices = get_normal_indices();
      Uint size = indices.size();
      std::vector<int> value(size), default_value;
      auto rit = value.begin();
      for (auto it = indices.begin(); it != indices.end(); ++it) *rit++ = *it;
      formatter->multi_int32(field_info->get_name(), value, default_value);
      return;
    }
  }
  field_info->write(this, formatter);
}

//! \brief colapses identical coordinates.
void Mesh_set::collapse_identical_coordinates()
{
  if (m_coord_indices_flat) {
    auto& indices = get_flat_coord_indices();
    collapse_identical_coordinates(indices);
    m_dirty_flat_coord_indices = false;
    m_dirty_coord_indices = true;
  }
  else {
    auto& indices = get_coord_indices();
    collapse_identical_coordinates(indices);
    m_dirty_flat_coord_indices = true;
    m_dirty_coord_indices = false;
  }
}

//! \brief colapses identical coordinates.
void Mesh_set::collapse_identical_coordinates(std::vector<Int32>& indices)
{ SGAL_error_msg("Not implemented yet"); }

//! \brief colapses identical coordinates.
void Mesh_set::collapse_identical_coordinates(Index_array& indices)
{
  typedef std::pair<const Vector3f*, Uint>      Coord_index_pair;
  if (indices.empty()) return;

  // Construct a vector of pairs of items, where each item is a point and the
  // index into the indices array where the point was indexed.
  std::vector<Coord_index_pair> vec(indices.size());
  std::vector<Coord_index_pair>::iterator it;
  Index_type i(0);
  for (it = vec.begin(); it != vec.end(); ++it) {
    const Vector3f& vecf = get_coord_3d(m_flat_coord_indices[i]);
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
  Uint num = 1;
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
  indices[index] = i;
  for (++it; it != vec.end(); ++it) {
    const Vector3f* curr = it->first;
    index = it->second;
    if ((*curr) != (*prev)) (*coords)[++i] = *curr;
    indices[index] = i;
    prev = curr;
  }
  Shared_coord_array old_shared_coord = get_coord_array();
  old_shared_coord->clear();
  set_coord_array(shared_coords);
}

//! \brief writes a triangular facet.
void Mesh_set::write_facet(const Vector3f& p1, const Vector3f& p2,
                           const Vector3f& p3, Formatter* formatter)
{
  if (Vector3f::collinear(p1, p2, p3)) {
    std::cerr << "Cannot write a triangular facet using collinear points!"
              << std::endl;
    return;
  }
  Vector3f n;
  n.normal(p1, p2, p3);
  formatter->facet(p1, p2, p3, n);
}

//! \brief writes a quadrilateral facet.
void Mesh_set::write_facet(const Vector3f& p1, const Vector3f& p2,
                           const Vector3f& p3, const Vector3f& p4,
                           Formatter* formatter)
{
  Vector3f l11, l12;
  l11.sub(p3, p2);
  l12.sub(p1, p2);

  Vector3f l21, l22;
  l21.sub(p2, p1);
  l22.sub(p4, p1);

  if (abs(l11.dot(l12)) < abs(l21.dot(l22))) {
    write_facet(p1, p2, p3, formatter);
    write_facet(p1, p3, p4, formatter);
  }
  else {
    write_facet(p1, p2, p4, formatter);
    write_facet(p2, p3, p4, formatter);
  }
}

//! \brief obtains the bounding box.
Bounding_box Mesh_set::bounding_box()
{
  if (!m_coord_indices_flat) return Geo_set::bounding_box();

  auto it = m_flat_coord_indices.begin();
  const Vector3f& v = get_coord_3d(*it);
  Bounding_box bbox(v[0], v[1], v[2], v[0], v[1], v[2]);
  for (++it; it != m_flat_coord_indices.end(); ++it) {
    const Vector3f& v = get_coord_3d(*it);
    Bounding_box tmp(v[0], v[1], v[2], v[0], v[1], v[2]);
    bbox += tmp;
  }
  return bbox;
}

SGAL_END_NAMESPACE
