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
// $Id: Arrangement_on_sphere_overlay_geo.cpp 7796 2009-07-19 13:32:38Z efif $
// $Revision: 7796 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4146 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4390 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#endif

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Arr_overlay_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"

#include "SCGAL/Arrangement_on_sphere_overlay_geo.hpp"
#include "SCGAL/Arrangement_on_surface_geo_overlay.hpp"
#include "SCGAL/Exact_coord_array.hpp"

SGAL_BEGIN_NAMESPACE

std::string Arrangement_on_sphere_overlay_geo::s_tag =
  "ArrangementOnSphereOverlay";
Container_proto * Arrangement_on_sphere_overlay_geo::s_prototype = NULL;

REGISTER_TO_FACTORY(Arrangement_on_sphere_overlay_geo,
                    "Arrangement_on_sphere_overlay_geo");

/*! Constructor */
Arrangement_on_sphere_overlay_geo::
Arrangement_on_sphere_overlay_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  m_own_aos(SGAL_FALSE),
  m_aos(NULL)
{
  if (!proto) create_renderers();
}

/*! Destructor */
Arrangement_on_sphere_overlay_geo::~Arrangement_on_sphere_overlay_geo()
{
  clear();
  if (m_aos && m_own_aos) delete m_aos;
}

/*! \brief initializes the container prototype */
void Arrangement_on_sphere_overlay_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());
}

/*! \brief deletes the container prototype */
void Arrangement_on_sphere_overlay_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto * Arrangement_on_sphere_overlay_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_sphere_overlay_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the ellpsoid attributes */
void Arrangement_on_sphere_overlay_geo::set_attributes(Element * elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);

  typedef Element::Cont_attr_iter       Cont_attr_iter;
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container * cont = elem->get_value(cai);
    if (name == "overlay") {
      Arrangement_on_sphere_overlay_geo * aos_geo =
        dynamic_cast<Arrangement_on_sphere_overlay_geo*>(cont);
      if (aos_geo) add_aos_geo(aos_geo);
      elem->mark_delete(cai);
      continue;
    }
  }

  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); mcai++)
  {
    const std::string & name = elem->get_name(mcai);
    Cont_list & cont_list = elem->get_value(mcai);
    if (name == "overlay") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Container * cont = *ci;
        Arrangement_on_sphere_overlay_geo * aos_geo =
          dynamic_cast<Arrangement_on_sphere_overlay_geo*>(cont);
        if (aos_geo) add_aos_geo(aos_geo);
      }
      elem->mark_delete(mcai);
    }
    continue;
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief cleans the representation */
void Arrangement_on_sphere_overlay_geo::clean()
{
  m_dirty = false;
  if (!m_aos) {
    m_aos = new Aos_overlay;
    m_own_aos = SGAL_TRUE;
  }
  insert_all(m_aos);
  Aos_overlay::Halfedge_iterator hei;
  for (hei = m_aos->halfedges_begin(); hei != m_aos->halfedges_end(); ++hei)
    hei->set_color(get_aos_edge_color());
  overlay_all(m_aoses.begin(), m_aoses.end(),
              std::distance(m_aoses.begin(), m_aoses.end()), this);
}

/*! \brief clears the internal representation and auxiliary data structures */
void Arrangement_on_sphere_overlay_geo::clear()
{
  m_aos->clear();
  m_dirty = true;
}

/*! \brief draws the arrangement edges */
void Arrangement_on_sphere_overlay_geo::Sphere_overlay_colored_edges_renderer::
operator()(Draw_action * action)
{
  Aos_overlay::Edge_const_iterator hei;
  for (hei = m_geo.m_aos->edges_begin(); hei != m_geo.m_aos->edges_end(); ++hei)
  {
    const Aos_overlay::Geometry_traits_2::X_monotone_curve_2 & curve =
      hei->curve();
    Vector3f src = to_vector3f(curve.source());
    Vector3f trg = to_vector3f(curve.target());
    src.normalize();
    trg.normalize();
    const Vector3f & color = hei->color();
    glColor3fv((float*)&color);
    m_geo.draw_aos_edge(action, src, trg);
  }
}

/*! \brief creates the renderers */
void Arrangement_on_sphere_overlay_geo::create_renderers()
{
  m_edges_renderer = new Sphere_overlay_edges_renderer(*this);
  m_vertices_renderer = new Sphere_overlay_vertices_renderer(*this);
  m_isolated_vertices_renderer =
    new Sphere_overlay_isolated_vertices_renderer(*this);
  m_colored_edges_renderer = new Sphere_overlay_colored_edges_renderer(*this);
  m_colored_vertices_renderer =
    new Sphere_overlay_colored_vertices_renderer(*this);
  m_colored_isolated_vertices_renderer =
    new Sphere_overlay_colored_isolated_vertices_renderer(*this);

  m_line_colored_edges_renderer =
    new Sphere_overlay_line_colored_edges_renderer(*this);
  m_point_colored_vertices_renderer =
    new Sphere_overlay_point_colored_vertices_renderer(*this);
  m_ring_colored_vertices_renderer =
    new Sphere_overlay_ring_colored_vertices_renderer(*this);
  m_point_colored_isolated_vertices_renderer =
    new Sphere_overlay_point_colored_isolated_vertices_renderer(*this);
  m_ring_colored_isolated_vertices_renderer =
    new Sphere_overlay_ring_colored_isolated_vertices_renderer(*this);

  m_inflated_line_edges_renderer =
    new Sphere_overlay_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer =
    new Sphere_overlay_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer =
    new Sphere_overlay_inflated_tube_edges_renderer(*this);
}

/*! \brief obtain the arrangement */
Arrangement_on_sphere_overlay_geo::Arrangement_on_sphere_overlay *
Arrangement_on_sphere_overlay_geo::get_aos()
{
  if (m_dirty) clean();
  return m_aos;
}

/*! \brief sets the arrangement */
void Arrangement_on_sphere_overlay_geo::
set_aos(Arrangement_on_sphere_overlay * aos)
{
  m_dirty = false;
  m_aos = aos;
}
  
SGAL_END_NAMESPACE
