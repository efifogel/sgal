// Copyright (c) 2014 Israel.
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

#include "SCGAL/Arrangement_of_polygeodesics_on_sphere_geo.hpp"
#include "SCGAL/Arrangement_on_surface_geo_overlay.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Arrangement_of_polygeodesics_on_sphere_geo::s_tag =
  "ArrangementOfPolygeodesicsOnSphere";
Container_proto*
Arrangement_of_polygeodesics_on_sphere_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Arrangement_of_polygeodesics_on_sphere_geo,
                    "Arrangement_of_polygeodesics_on_sphere_geo");

//! \brief constructor.
Arrangement_of_polygeodesics_on_sphere_geo::
Arrangement_of_polygeodesics_on_sphere_geo(Boolean proto) :
  Arrangement_of_polygeodesics_on_sphere_base_geo(proto),
  m_owned_aos(false),
  m_aos(nullptr)
{ if (!proto) create_renderers(); }

//! \brief destructor.
Arrangement_of_polygeodesics_on_sphere_geo::
~Arrangement_of_polygeodesics_on_sphere_geo()
{
  clear();
  if (m_owned_aos){
    if (m_aos) {
      delete m_aos;
      m_aos = nullptr;
    }
    m_owned_aos = false;
  }
}

//! \brief initializes the container prototype.
void Arrangement_of_polygeodesics_on_sphere_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_of_polygeodesics_on_sphere_base_geo::get_prototype());
}

//! \brief deletes the container prototype.
void Arrangement_of_polygeodesics_on_sphere_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Arrangement_of_polygeodesics_on_sphere_geo::get_prototype()
{
  if (!s_prototype)
    Arrangement_of_polygeodesics_on_sphere_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
void Arrangement_of_polygeodesics_on_sphere_geo::set_attributes(Element* elem)
{
  Arrangement_of_polygeodesics_on_sphere_base_geo::set_attributes(elem);
  auto cai = elem->cont_attrs_begin();
  for (; cai != elem->cont_attrs_end(); ++cai) {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "overlay") {
      Shared_aos_geo aos_geo =
        boost::dynamic_pointer_cast<Arrangement_of_polygeodesics_on_sphere_geo>(cont);
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
          boost::dynamic_pointer_cast<Arrangement_of_polygeodesics_on_sphere_geo>(cont);
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
void Arrangement_of_polygeodesics_on_sphere_geo::clean()
{
  m_dirty = false;
  if (!m_aos) {
    m_aos = new Aos;
    SGAL_assertion(m_aos);
    m_owned_aos = true;
  }
  insert_all(m_aos);
  overlay_all(m_aoses.begin(), m_aoses.end(),
              std::distance(m_aoses.begin(), m_aoses.end()), this);
}

//! \brief clears the internal representation and auxiliary data structures.
void Arrangement_of_polygeodesics_on_sphere_geo::clear()
{
  if (m_aos) m_aos->clear();
  m_aoses.clear();
  m_dirty = true;
}

//! \brief creates the renderers.
void Arrangement_of_polygeodesics_on_sphere_geo::create_renderers()
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
void Arrangement_of_polygeodesics_on_sphere_geo::destroy_renderers()
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
Arrangement_of_polygeodesics_on_sphere_geo::Aos*
Arrangement_of_polygeodesics_on_sphere_geo::get_aos()
{
  if (m_dirty) clean();
  return m_aos;
}

//! \brief sets the arrangement.
void Arrangement_of_polygeodesics_on_sphere_geo::set_aos(Aos* aos)
{
  m_dirty = false;
  m_aos = aos;
}

SGAL_END_NAMESPACE
