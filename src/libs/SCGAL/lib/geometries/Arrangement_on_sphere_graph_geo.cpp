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

#include "SCGAL/Arrangement_on_sphere_graph_geo.hpp"
#include "SCGAL/Arrangement_on_surface_geo_overlay.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Arrangement_on_sphere_graph_geo::s_tag =
  "ArrangementOnSphereGraph";
Container_proto* Arrangement_on_sphere_graph_geo::s_prototype(NULL);

REGISTER_TO_FACTORY(Arrangement_on_sphere_graph_geo,
                    "Arrangement_on_sphere_graph_geo");

//! \brief constructor.
Arrangement_on_sphere_graph_geo::
Arrangement_on_sphere_graph_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  m_owned_aos(false),
  m_aos(NULL)
{ if (!proto) create_renderers(); }

//! \brief destructor.
Arrangement_on_sphere_graph_geo::~Arrangement_on_sphere_graph_geo()
{
  clear();
  if (m_owned_aos) {
    if (m_aos) {
      delete m_aos;
      m_aos = NULL;
    }
    m_owned_aos = false;
  }
}

//! \brief initializes the container prototype.
void Arrangement_on_sphere_graph_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());
}

//! \brief deletes the container prototype.
void Arrangement_on_sphere_graph_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Arrangement_on_sphere_graph_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_sphere_graph_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
void Arrangement_on_sphere_graph_geo::set_attributes(Element* elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);

  typedef Element::Cont_attr_iter       Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Element::Shared_container cont = elem->get_value(cai);
    if (name == "overlay") {
      Shared_arrangement_on_sphere_graph_geo aos_geo =
        boost::dynamic_pointer_cast<Arrangement_on_sphere_graph_geo>(cont);
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
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "overlay") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Element::Shared_container cont = *ci;
        Shared_arrangement_on_sphere_graph_geo aos_geo =
          boost::dynamic_pointer_cast<Arrangement_on_sphere_graph_geo>(cont);
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
void Arrangement_on_sphere_graph_geo::clean()
{
  m_dirty = false;
  if (!m_aos) {
    m_aos = new Aos_graph;
    SGAL_assertion(m_aos);
    m_owned_aos = true;
  }
  insert_all(m_aos);
#if 0
  overlay_all(m_aoses.begin(), m_aoses.end(),
              std::distance(m_aoses.begin(), m_aoses.end()), this);
#endif
}

//! \brief clears the internal representation and auxiliary data structures.
void Arrangement_on_sphere_graph_geo::clear()
{
  if (m_aos) m_aos->clear();
  m_dirty = true;
}

//! \brief creates the renderers.
void Arrangement_on_sphere_graph_geo::create_renderers()
{
  m_edges_renderer = new Sphere_edges_renderer(*this);
  m_vertices_renderer = new Sphere_vertices_renderer(*this);
  m_isolated_vertices_renderer = new Sphere_isolated_vertices_renderer(*this);
  m_colored_edges_renderer = new Sphere_colored_edges_renderer(*this);
  m_colored_vertices_renderer = new Sphere_colored_vertices_renderer(*this);
  m_colored_isolated_vertices_renderer =
    new Sphere_colored_isolated_vertices_renderer(*this);

  m_line_colored_edges_renderer =
    new Sphere_line_colored_edges_renderer(*this);
  m_point_colored_vertices_renderer =
    new Sphere_point_colored_vertices_renderer(*this);
  m_ring_colored_vertices_renderer =
    new Sphere_ring_colored_vertices_renderer(*this);
  m_point_colored_isolated_vertices_renderer =
    new Sphere_point_colored_isolated_vertices_renderer(*this);
  m_ring_colored_isolated_vertices_renderer =
    new Sphere_ring_colored_isolated_vertices_renderer(*this);

  m_inflated_line_edges_renderer =
    new Sphere_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer =
    new Sphere_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer =
    new Sphere_inflated_tube_edges_renderer(*this);
}

//! \brief destroys the renderers.
void Arrangement_on_sphere_graph_geo::destroy_renderers()
{
  if (m_edges_renderer) delete m_edges_renderer;
  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_isolated_vertices_renderer) delete m_isolated_vertices_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;
  if (m_colored_isolated_vertices_renderer)
    delete m_colored_isolated_vertices_renderer;
  if (m_line_colored_edges_renderer) delete m_line_colored_edges_renderer;
  if (m_point_colored_vertices_renderer)
    delete m_point_colored_vertices_renderer;
  if (m_ring_colored_vertices_renderer)
    delete m_ring_colored_vertices_renderer;
  if (m_point_colored_isolated_vertices_renderer)
    delete m_point_colored_isolated_vertices_renderer;
  if (m_ring_colored_isolated_vertices_renderer)
    delete m_ring_colored_isolated_vertices_renderer;
  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;
}

//! \brief obrains the arrangement.
Arrangement_on_sphere_graph* Arrangement_on_sphere_graph_geo::get_aos()
{
  if (m_dirty) clean();
  return m_aos;
}

//! \brief sets the arrangement.
void Arrangement_on_sphere_graph_geo::set_aos(Arrangement_on_sphere_graph* aos)
{
  m_dirty = false;
  m_aos = aos;
}

SGAL_END_NAMESPACE
