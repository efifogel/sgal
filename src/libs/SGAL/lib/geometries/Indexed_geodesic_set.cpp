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

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_geodesic_set.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Coord_array_3d.hpp"
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

const std::string Indexed_geodesic_set::s_tag = "IndexedGeodesicSet";
Container_proto* Indexed_geodesic_set::s_prototype(nullptr);

// Default values:
const Boolean Indexed_geodesic_set::m_def_color_per_vertex(true);
const Boolean Indexed_geodesic_set::m_def_normal_per_vertex(true);
const Float Indexed_geodesic_set::s_def_line_width(1);

REGISTER_TO_FACTORY(Indexed_geodesic_set, "Indexed_geodesic_set");

//! \brief A parameter-less constructor.
Indexed_geodesic_set::Indexed_geodesic_set(Boolean proto) :
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

//! \brief destructor.
Indexed_geodesic_set::~Indexed_geodesic_set() {}

/* \brief set the flag that indicates whether normals are bound per vertex or
 * per face.
 */
void Indexed_geodesic_set::set_normal_per_vertex(Boolean normal_per_vertex)
{
  m_normal_per_vertex = normal_per_vertex;
  m_normal_attachment = (normal_per_vertex) ? AT_PER_VERTEX : AT_PER_PRIMITIVE;
}

/* \brief sets the flag that indicates whether colors are bound per vertex or
 * per face.
 */
void Indexed_geodesic_set::set_color_per_vertex(Boolean color_per_vertex)
{
  m_color_per_vertex = color_per_vertex;
  m_color_attachment = (color_per_vertex) ? AT_PER_VERTEX : AT_PER_PRIMITIVE;
}

/*! \brief sets the coordinate set. Pass the pointer to the geometry object
 * used by the decoder as well.
 */
void Indexed_geodesic_set::set_coord_array(Shared_coord_array coord_array)
{
  m_coord_array = coord_array;
  m_dirty_bounding_sphere = true;
}

/*! \brief sets the normal set. Pass the pointer to the geometry object
 * used by the decoder as well.
 */
void Indexed_geodesic_set::set_normal_array(Shared_normal_array normal_array)
{ m_normal_array = normal_array; }

/*! \brief sets the texture coordinate set. Pass the pointer to the geometry
 * object used by the decoder as well.
 */
void Indexed_geodesic_set::
set_tex_coord_array(Shared_tex_coord_array tex_coord_array)
{ m_tex_coord_array = tex_coord_array; }

/*! \brief sets the color set. Pass the pointer to the geometry object
 * used by the decoder as well.
 * \param color_array (in) a pointer to a color set
 */
void Indexed_geodesic_set::set_color_array(Shared_color_array color_array)
{ m_color_array = color_array; }

/*! \brief draws the geometry.
 * For efficiency reasons, differenrt methods were written to
 * draw geometries with different kinds of data (texture/normal/color).
 * \param action action.
 */
void Indexed_geodesic_set::draw(Draw_action* action)
{
  boost::shared_ptr<Coord_array_3d> coord_array =
    boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coord_array);

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
    if (fragment_attached == AT_PER_MESH) glColor3fv(get(m_color_array, 0));
  }

  if (m_primitive_type == PT_LINES) {
    Uint j = 0;
    Uint k = 0;

    glBegin(GL_LINES);

    for (Uint i = 0; i < m_num_primitives; ++i) {
      if ((fragment_attached == AT_PER_VERTEX) ||
          ((fragment_attached == AT_PER_PRIMITIVE) && ((j & 0x1) == 0x0)))
      {
        if (fragment_source == FS_COLOR) {
          if (m_color_array) glColor3fv(get_by_coord_index(m_color_array, k));
        }
        else {
          if (m_normal_array)
            glNormal3fv(get_by_coord_index(m_normal_array, k));
        }
        ++k;
      }
      glVertex3fv(get_by_coord_index(m_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j+1));
      j += 2;
    }
    glEnd();
  }
  else if (m_primitive_type == PT_LINE_STRIPS) {
    Uint j = 0;
    Uint k = 0;
    int m_radius = 1;
    for (Uint i = 0; i < m_num_primitives; ++i) {
      if (fragment_attached == AT_PER_PRIMITIVE) {
        if (fragment_source == FS_COLOR) {
          if (m_color_array) glColor3fv(get_by_coord_index(m_color_array, k));
        }
        else {
          if (m_normal_array)
            glNormal3fv(get_by_coord_index(m_normal_array, k));
        }
        ++k;
      }

      glBegin(GL_LINE_STRIP);

      // std::cout << m_coord_indices[j] << " * " << m_coord_indices[j+1]
      //           << std::endl;
      Vector3f first_coord = (*coord_array)[m_coord_indices[j]];
      Vector3f second_coord = (*coord_array)[m_coord_indices[j+1]];
      j += 2;
      //std::cout << first_coord <<"  "<<second_coord<<std::endl;

      float x_start = m_radius*cos(first_coord[0])*sin(first_coord[1]);
      float y_start = m_radius*sin(first_coord[0])*sin(first_coord[1]);
      float z_start = m_radius*cos(first_coord[1]);

      Vector3f source = Vector3f(x_start,y_start,z_start);

      float x_end = m_radius*cos(second_coord[0])*sin(second_coord[1]);
      float y_end = m_radius*sin(second_coord[0])*sin(second_coord[1]);
      float z_end = m_radius*cos(second_coord[1]);

      Vector3f target = Vector3f(x_end,y_end,z_end);
      std::list<Vector3f> points;
      points.push_front(source);
      points.push_back(target);
      std::list<Vector3f>::iterator iter;
      std::list<Vector3f>::iterator nextIter;
      Uint breaks;

      for (breaks = 1; breaks <= 1; breaks++) {

        iter = points.begin();
        nextIter = iter;
        nextIter++;
        while (nextIter != points.end()) {
          Vector3f vertex1 = *iter;
          Vector3f vertex2 = *nextIter;
          float x = (vertex2[0] + vertex1[0])/2;
          float y = (vertex2[1] + vertex1[1])/2;
          float z = (vertex2[2] + vertex1[2])/2;

          float r = sqrt(x*x+y*y+z*z);
          float theta = atan2(y,x);
          float fi = acos(z/r);


          float x_new = m_radius*cos(theta)*sin(fi);
          float y_new = m_radius*sin(theta)*sin(fi);
          float z_new = m_radius*cos(fi);
          iter = points.insert(nextIter, Vector3f(x_new,y_new,z_new));
          iter++;
          nextIter++;
        }
      }
      for (iter = points.begin(); iter!= points.end(); ++iter) {
        Vector3f temp = *iter;
        glVertex3fv((float*)&temp);
      }
    }
    glEnd();

  }

  context->draw_line_width(1.0f);
  if (!m_elliminate_hiden) {
    context->draw_depth_enable(true);
    context->draw_depth_mask(true);
  }
  if (fragment_source) context->draw_light_enable(true);
}

//! \brief
void Indexed_geodesic_set::isect(Isect_action* /* action */) {}

//! \brief cleans the sphere bound of the geodesic set.
void Indexed_geodesic_set::clean_bounding_sphere()
{
  if (!m_bb_is_pre_set && m_coord_array) {
    boost::shared_ptr<Coord_array_3d> coord_array =
      boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
    SGAL_assertion(coord_array);
    m_bounding_sphere.set_around(coord_array->begin(), coord_array->end());
  }
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of the object.
void Indexed_geodesic_set::set_attributes(Element* elem)
{
  Geo_set::set_attributes(elem);

  std::string normal_indices_string;
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
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
      set_line_width(boost::lexical_cast<Float>(value));
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

//! \brief initializes the prototype.
void Indexed_geodesic_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());

  // lineWidth
  Float_handle_function line_width_func =
    static_cast<Float_handle_function>
    (&Indexed_geodesic_set::line_width_handle);
  s_prototype->add_field_info(new SF_float(LINE_WIDTH, "lineWidth",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           line_width_func));
}

//! \brief deletes the prototype.
void Indexed_geodesic_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Indexed_geodesic_set::get_prototype()
{
  if (!s_prototype) Indexed_geodesic_set::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
