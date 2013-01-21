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
// $Source: $
// $Revision: 14184 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <sstream>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

std::string Indexed_line_set::s_tag = "sgalIndexedLineSet";
Container_proto* Indexed_line_set::s_prototype = 0;

// Default values:
const Boolean Indexed_line_set::m_def_color_per_vertex(true);
const Boolean Indexed_line_set::m_def_normal_per_vertex(true);
const Float Indexed_line_set::s_def_line_width(1);

REGISTER_TO_FACTORY(Indexed_line_set, "Indexed_line_set");

/*! A parameter-less constructor */
Indexed_line_set::Indexed_line_set(Boolean proto) :
  Geo_set(proto),
  m_color_per_vertex(m_def_color_per_vertex),
  m_normal_per_vertex(m_def_normal_per_vertex),
  m_line_width(s_def_line_width),
  m_elliminate_hiden(false),
  m_has_texture(false),
  m_bb_is_pre_set(false),
  m_use_display_list(false),
  m_display_list_id(-1)
{
  m_primitive_type = PT_LINES;
}

/*! Destructor */
Indexed_line_set::~Indexed_line_set() {}

/* \brief sets the flag that indicates whether normals are bound per vertex
 * or per line.
 */
void Indexed_line_set::set_normal_per_vertex(Boolean normal_per_vertex)
{
  m_normal_per_vertex = normal_per_vertex;
  m_normal_attachment = (normal_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/* \brief sets the flag that indicates whether colors are bound per vertex or
 * per line.
 */
void Indexed_line_set::set_color_per_vertex(Boolean color_per_vertex)
{
  m_color_per_vertex = color_per_vertex;
  m_color_attachment = (color_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/*! Set the coordinate set. Pass the pointer to the geometry object 
 * used by the decoder as well.
 * \param coord_array (in) a pointer to a coord set
 */
void Indexed_line_set::set_coord_array(Coord_array* coord_array)
{
  m_coord_array = coord_array;
  m_dirty_sphere_bound = true;
}

/*! Set the normal set. Pass the pointer to the geometry object 
 * used by the decoder as well.
 * \param coord_array (in) a pointer to a coord set
*/ 
void Indexed_line_set::set_normal_array(Normal_array* normal_array)
{ m_normal_array = normal_array; }

/*! Set the texture coordinate set. Pass the pointer to the geometry object 
 * used by the decoder as well.
 * \param tex_coord_array (in) a pointer to a coord set
 */
void Indexed_line_set::set_tex_coord_array(Tex_coord_array* tex_coord_array)
{ m_tex_coord_array = tex_coord_array; }

/*! Set the color set. Pass the pointer to the geometry object 
 * used by the decoder as well.
 * \param color_array (in) a pointer to a color set
 */
void Indexed_line_set::set_color_array(Color_array* color_array)
{ m_color_array = color_array; }

/*! Draws the geometry.
 * For efficiency reasons, differenrt methods were written to 
 * draw geometries with different kinds of data (texture/normal/color).
 * @param action action.
 */
void Indexed_line_set::draw(Draw_action* action)
{
  Context* context = action->get_context();
  if (!m_elliminate_hiden) {
    context->draw_depth_mask(false);
    context->draw_depth_enable(false);
  }
  context->draw_line_width(m_line_width);

  Fragment_source fragment_source = resolve_fragment_source();
  Attachment fragment_attached = (fragment_source == FS_NORMAL) ?
    m_normal_attachment : m_color_attachment;

  if (fragment_source == FS_COLOR) {
    context->draw_light_enable(false);
    if (fragment_attached == PER_MESH) glColor3fv(get(*m_color_array, 0));
  }

  if (m_primitive_type == PT_LINES) {
    Uint j = 0;
    Uint k = 0;
    glBegin(GL_LINES);

    for (Uint i = 0; i < m_num_primitives; i++) {
      if ((fragment_attached == PER_VERTEX) ||
          ((fragment_attached == PER_PRIMITIVE) && ((j & 0x1) == 0x0)))
      {
        if (fragment_source == FS_COLOR) {
          if (m_color_array) glColor3fv(get_by_ci(*m_color_array, k));
        } else {
          if (m_normal_array) glNormal3fv(get_by_ci(*m_normal_array, k));
        }
        ++k;
      }
      glVertex3fv(get_by_ci(*m_coord_array, j));
      glVertex3fv(get_by_ci(*m_coord_array, j+1));
      j += 2;
    }
    glEnd();
  } else if (m_primitive_type == PT_LINE_STRIPS) {
    Uint j = 0;
    Uint k = 0;
    for (Uint i = 0; i < m_num_primitives; i++) {
      if (fragment_attached == PER_PRIMITIVE) {
        if (fragment_source == FS_COLOR) {
          if (m_color_array) glColor3fv(get_by_ci(*m_color_array, k));
        } else {
          if (m_normal_array) glNormal3fv(get_by_ci(*m_normal_array, k));
        }
        ++k;
      }
      glBegin(GL_LINE_STRIP);
      for (; m_coord_indices[j] != (Uint) -1; ++j) {
        glVertex3fv(get_by_ci(*m_coord_array, j));
        if (fragment_attached == PER_VERTEX) {
          if (fragment_source == FS_COLOR)
            glColor3fv(get_by_ci(*m_color_array, k));
          else glNormal3fv(get_by_ci(*m_normal_array, k));
          ++k;
        }
      }
      glEnd();
    }
  }

  context->draw_line_width(1.0f);
  if (!m_elliminate_hiden) {
    context->draw_depth_enable(true);
    context->draw_depth_mask(true);
  }
  if (fragment_source) context->draw_light_enable(true);
}

/*!
 */
void Indexed_line_set::isect(Isect_action* /* action */)
{
}

/*! Calculate the sphere bound of the mesh. Returns true if the BS has
 * changed since lst time this was called.
 */
bool Indexed_line_set::clean_sphere_bound()
{
  if (!m_bb_is_pre_set && m_coord_array)
    m_sphere_bound.set_around(m_coord_array->begin(), m_coord_array->end());
  m_dirty_sphere_bound = false;
  return true;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Indexed_line_set::set_attributes(Element* elem) 
{ 
  Geo_set::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  typedef Element::Cont_attr_iter         Cont_attr_iter;

  std::string normal_indices_string;

  //! \todo sg->get_stats().add_num_mesh();

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "colorPerVertex") {
      set_color_per_vertex(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "normalPerVertex") {
      set_normal_per_vertex(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "lineWidth") {
      sscanf(value.c_str(), "%f", &m_line_width);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "elliminateHiden") {
      m_elliminate_hiden = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! sets the attributes of this node */
void Indexed_line_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());

  s_prototype->add_field_info(new SF_float(LINE_WIDTH, "lineWidth",
                                           get_member_offset(&m_line_width)));
}

/*! */
void Indexed_line_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Indexed_line_set::get_prototype() 
{  
  if (!s_prototype) Indexed_line_set::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
