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

SGAL_BEGIN_NAMESPACE

Container_proto* Mesh_set::s_prototype = 0;

const Boolean Mesh_set::s_def_is_solid(true);
const Boolean Mesh_set::s_def_is_convex(true);
const Boolean Mesh_set::s_def_is_ccw(true);
const Float Mesh_set::s_def_crease_angle(0);
const Float Mesh_set::s_def_polygon_offset_factor(0);

/*! A parameter-less constructor */
Mesh_set::Mesh_set(Boolean proto) :
  Geo_set(proto),
  m_dirty(true),
  m_is_ccw(s_def_is_ccw),
  m_is_solid(s_def_is_solid),
  m_is_convex(s_def_is_convex),
  m_crease_angle(s_def_crease_angle),
  m_polygon_offset_factor(s_def_polygon_offset_factor),
  m_dirty_indices(true),
  m_flatten_indices(false),
  m_indices_flat(false)
{}

/*! \brief sets the attributes of this object. */
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
      m_crease_angle = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "polygonOffsetFactor") {
      m_polygon_offset_factor = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief draws the mesh conditionaly. */
void Mesh_set::draw(Draw_action* action)
{
  if (is_dirty()) clean();
  if (is_empty()) return;

  draw_mesh(action);
}

/*! \brief draws the mesh. */
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

/*! \brief calculates the sphere bound. */
Boolean Mesh_set::clean_sphere_bound()
{
  if (is_dirty()) clean();
  return Geo_set::clean_sphere_bound();
}

/*! \brief sets the attributes of this node. */
void Mesh_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_bool(CCW, "ccw",
                                          get_member_offset(&m_is_ccw),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_bool(SOLID, "solid",
                                          get_member_offset(&m_is_solid),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_bool(CONVEX, "convex",
                                          get_member_offset(&m_is_convex),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_float(CREASE_ANGLE, "convex",
                                          get_member_offset(&m_crease_angle),
                                          exec_func));  
}

/*! \brief deletes the container prototype. */
void Mesh_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Mesh_set::get_prototype() 
{  
  if (!s_prototype) Mesh_set::init_prototype();
  return s_prototype;
}

/*! \brief determines whether colors are generated by the geometry. */
inline Boolean Mesh_set::are_generated_color()
{
  if (is_dirty()) clean();
  return (m_generated_color && (m_color_array != NULL));
}  
  
/*! \brief determines whether texture coordinates are generated by the geometry.
 */
inline Boolean Mesh_set::are_generated_tex_coord()
{
  if (is_dirty()) clean();
  return (m_generated_tex_coord && (m_tex_coord_array != NULL));
}  

/*! \brief obtains the coord-index array. */
Array<Uint>& Mesh_set::get_coord_indices()
{
  if (is_dirty_indices()) clean_indices();
  return Geo_set::get_coord_indices();
}

/*! \brief cleans the representation. */
void Mesh_set::clean()
{
  if (is_dirty_indices()) clean_indices();
  m_dirty = false;
}

void Mesh_set::clean_indices()
{
  if (!m_indices_flat && m_flatten_indices) flatten_indices();
  m_dirty_indices = false;
}

/*! \brief proceses the indices (in places). */
void Mesh_set::flatten_indices(Uint* src, Uint* dst, Uint num)
{
  if (m_primitive_type == PT_TRIANGLES) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < num; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      k++;
    }
  }
  if (m_primitive_type == PT_QUADS) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < num; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      k++;
    }
  }
}

/*! \brief processes the indices.
 * In case of triangles or quads remove the '-1' end-of-polygon indication
 * from the index buffers. This operation changes the the structure of the
 * index buffers, and must be reflected in the drawing routines.
 */
void Mesh_set::flatten_indices()
{
  Uint size = (m_primitive_type == PT_TRIANGLES) ? m_num_primitives * 3 :
    (m_primitive_type == PT_QUADS) ? m_num_primitives * 4 : 0;
  if (!size) return;

  Uint* indices = m_coord_indices.get_vector();
  flatten_indices(indices, indices, m_num_primitives);
  m_coord_indices.resize(size);

  if (m_tex_coord_indices.size()) {
    Uint* indices = m_tex_coord_indices.get_vector();
    flatten_indices(indices, indices, m_num_primitives);
    m_tex_coord_indices.resize(size);
  }

  if (m_normal_indices.size() && m_normal_attachment == PER_VERTEX) {
    Uint* indices = m_normal_indices.get_vector();
    flatten_indices(indices, indices, m_num_primitives);
    m_normal_indices.resize(size);
  }

  if (m_color_indices.size() && m_color_attachment == PER_VERTEX) {
    Uint* indices = m_color_indices.get_vector();
    flatten_indices(indices, indices, m_num_primitives);
    m_color_indices.resize(size);
  }
  m_indices_flat = true;
}

/*! \brief processes change of coordinates. */
void Mesh_set::coord_changed(Field_info* /* field_info */)
{
  m_dirty = true;
  m_dirty_sphere_bound = true;
}

SGAL_END_NAMESPACE
