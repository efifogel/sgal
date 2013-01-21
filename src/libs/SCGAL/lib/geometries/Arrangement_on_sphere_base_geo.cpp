// Copyright (c) 2007 Israel.
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
// $Id: Arrangement_on_sphere_base_geo.cpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4146)
#pragma warning( disable : 4244)
#pragma warning( disable : 4390)
#pragma warning( disable : 4503)
#pragma warning( disable : 4800)
#pragma warning( disable : 4996)
#endif

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Coord_array.hpp"

#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Arrangement_on_sphere_base_geo::s_prototype = NULL;

/*! Constructor */
Arrangement_on_sphere_base_geo::
Arrangement_on_sphere_base_geo(Boolean proto) :
  Arrangement_on_surface_geo(proto),
  m_coord_array(NULL)
{
  if (!proto) create_renderers();
}

/*! Destructor */
Arrangement_on_sphere_base_geo::~Arrangement_on_sphere_base_geo()
{
  if (m_surface_renderer) delete m_surface_renderer;
  if (m_colored_surface_renderer) delete m_colored_surface_renderer;
  if (m_stencil_surface_renderer) delete m_stencil_surface_renderer;
}

/*! \brief initializes the container prototype. */
void Arrangement_on_sphere_base_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_surface_geo::get_prototype());
}

/*! \brief deletes the container prototype. */
void Arrangement_on_sphere_base_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Arrangement_on_sphere_base_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_sphere_base_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the ellpsoid attributes. */
void Arrangement_on_sphere_base_geo::set_attributes(Element* elem)
{
  Arrangement_on_surface_geo::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "pointLocationIndex") {
      tokenizer tokens(value, sep);      
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_point_location_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error        
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_point_location_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_point_location_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }
    if (name == "pointIndex") {
      tokenizer tokens(value, sep);      
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_point_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error        
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_point_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_point_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }

    if (name == "curveIndex") {
      tokenizer tokens(value, sep);      
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0 || size & 0x1) {
        m_curve_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error        
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_curve_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_curve_indices[i++] = static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }

    if (name == "xMonotoneCurveIndex") {
      tokenizer tokens(value, sep);      
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0 || size & 0x1) {
        m_x_monotone_curve_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error        
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_x_monotone_curve_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_x_monotone_curve_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }
  }

  typedef Element::Cont_attr_iter       Cont_attr_iter;
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "coord") {
      Coord_array* coord_array = dynamic_cast<Coord_array*>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }
    
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief */
void Arrangement_on_sphere_base_geo::cull(Cull_context & cull_context)
{
}

/*! \brief */
void Arrangement_on_sphere_base_geo::isect(Isect_action* action)
{
  //! \todo isect with a sphere
}

/*! \brief */
Boolean Arrangement_on_sphere_base_geo::clean_sphere_bound()
{
  if (is_dirty()) clean();
  if (m_bb_is_pre_set) return true;

  m_sphere_bound.set_center(Vector3f(0, 0, 0));
  m_sphere_bound.set_radius(1);
  m_dirty_sphere_bound = false;
  return true;
}

/*! \brief sets the coordinate array */
void Arrangement_on_sphere_base_geo::set_coord_array(Coord_array* coord_array)
{
  m_coord_array = coord_array;
  m_dirty_sphere_bound = true;
}

/*! \brief creates the renderers */
void Arrangement_on_sphere_base_geo::create_renderers()
{
  m_surface_renderer = new Surface_renderer();
  Vector4f color;
  color[0] = m_aos_surface_color[0];
  color[1] = m_aos_surface_color[1];
  color[2] = m_aos_surface_color[2];
  color[3] = 0.5f;
  m_colored_surface_renderer = new Colored_surface_renderer(color);
  m_stencil_surface_renderer = new Stencil_surface_renderer();
}

/*! \brief draws an arrangement on surface vertex */
void Arrangement_on_sphere_base_geo::draw_aos_vertex(Draw_action* action,
                                                     Vector3f& center)
{
 draw_vertex_on_sphere(action, center, m_aos_vertex_style,
                       m_aos_vertex_radius, m_aos_delta_angle);
}

/*! \brief draws an arrangement on surface isolated vertex */
void Arrangement_on_sphere_base_geo::
draw_aos_isolated_vertex(Draw_action* action, Vector3f & center)
{
  draw_vertex_on_sphere(action, center, m_aos_isolated_vertex_style,
                        m_aos_isolated_vertex_radius, m_aos_delta_angle);
}

/*! \brief Draw an arrangement on surface boundary_vertex */
void Arrangement_on_sphere_base_geo::
draw_aos_boundary_vertex(Draw_action* action, Vector3f & center)
{
  draw_vertex_on_sphere(action, center, m_aos_boundary_vertex_style,
                        m_aos_boundary_vertex_radius, m_aos_delta_angle);
}

/*! \brief draws an arrangement on surface edge */
void Arrangement_on_sphere_base_geo::draw_aos_edge(Draw_action* action,
                                                   Vector3f& src,
                                                   Vector3f& trg)
{
  draw_edge_on_sphere(action, src, trg, m_aos_edge_style, m_aos_edge_count,
                      m_aos_edge_directed, m_aos_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}

SGAL_END_NAMESPACE
