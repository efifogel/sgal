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
 * great circles embeded on a sphere.
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

SGAL_BEGIN_NAMESPACE

std::string Arrangement_on_sphere_overlay_geo::s_tag =
  "ArrangementOnSphereOverlay";
Container_proto* Arrangement_on_sphere_overlay_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Arrangement_on_sphere_overlay_geo,
                    "Arrangement_on_sphere_overlay_geo");

//! \brief constructor.
Arrangement_on_sphere_overlay_geo::
Arrangement_on_sphere_overlay_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  m_owned_aos(false),
  m_aos(nullptr)
{
  if (!proto) create_renderers();
}

//! \brief destructor.
Arrangement_on_sphere_overlay_geo::~Arrangement_on_sphere_overlay_geo()
{
  clear();
  if (m_owned_aos) {
    if (m_aos) {
      delete m_aos;
      m_aos = nullptr;
    }
    m_owned_aos = false;
  }
}

//! \brief initializes the container prototype.
void Arrangement_on_sphere_overlay_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());
}

//! \brief deletes the container prototype.
void Arrangement_on_sphere_overlay_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Arrangement_on_sphere_overlay_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_sphere_overlay_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
void Arrangement_on_sphere_overlay_geo::set_attributes(Element* elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);

  auto cai = elem->cont_attrs_begin();
  for (; cai != elem->cont_attrs_end(); ++cai) {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "overlay") {
      Shared_arrangement_on_sphere_overlay_geo aos_geo =
        boost::dynamic_pointer_cast<Arrangement_on_sphere_overlay_geo>(cont);
      if (aos_geo) add_aos_geo(aos_geo);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  auto mcai = elem->multi_cont_attrs_begin();
  for (; mcai != elem->multi_cont_attrs_end(); ++mcai) {
    const auto& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (name == "overlay") {
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        auto cont = *ci;
        auto aos_geo =
          boost::dynamic_pointer_cast<Arrangement_on_sphere_overlay_geo>(cont);
        if (aos_geo) add_aos_geo(aos_geo);
      }
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief cleans the representation.
void Arrangement_on_sphere_overlay_geo::clean()
{
  m_dirty = false;
  if (!m_aos) {
    m_aos = new Aos_overlay;
    SGAL_assertion(m_aos);
    m_owned_aos = true;
  }
  insert_all(m_aos);
  Aos_overlay::Halfedge_iterator hei;
  for (hei = m_aos->halfedges_begin(); hei != m_aos->halfedges_end(); ++hei)
    hei->set_color(get_aos_edge_color());
  overlay_all(m_aoses.begin(), m_aoses.end(),
              std::distance(m_aoses.begin(), m_aoses.end()), this);
}

//! \brief clears the internal representation and auxiliary data structures.
void Arrangement_on_sphere_overlay_geo::clear()
{
  m_aos->clear();
  m_dirty = true;
}

//! \brief draws the arrangement edges.
void Arrangement_on_sphere_overlay_geo::Sphere_overlay_colored_edges_renderer::
operator()(Draw_action* action)
{
  for (auto hei = m_geo.m_aos->edges_begin(); hei != m_geo.m_aos->edges_end();
       ++hei)
  {
    const auto& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    const auto& color = hei->color();
    glColor3fv((float*)&color);
    m_geo.draw_aos_edge(action, src, trg, normal);
  }
}

//! \brief creates the renderers.
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

//! \brief obtain the arrangement.
Arrangement_on_sphere_overlay_geo::Arrangement_on_sphere_overlay *
Arrangement_on_sphere_overlay_geo::get_aos()
{
  if (m_dirty) clean();
  return m_aos;
}

//! \brief sets the arrangement.
void Arrangement_on_sphere_overlay_geo::
set_aos(Arrangement_on_sphere_overlay* aos)
{
  m_dirty = false;
  m_aos = aos;
}

SGAL_END_NAMESPACE
