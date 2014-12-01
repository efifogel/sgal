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
#include "SGAL/Normal_array.hpp"

#include "SCGAL/Arrangement_of_polygeodesics_on_sphere_base_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto*
Arrangement_of_polygeodesics_on_sphere_base_geo::s_prototype(nullptr);

// Polygeodesic vertex:
const Arrangement_on_surface_geo::Vertex_style
Arrangement_of_polygeodesics_on_sphere_base_geo::
s_def_pg_vertex_style(Vertex_shape::BALL);
const Float Arrangement_of_polygeodesics_on_sphere_base_geo::
s_def_pg_vertex_radius(.06f);
const Float Arrangement_of_polygeodesics_on_sphere_base_geo::
s_def_pg_vertex_point_size(2);
const Vector3f Arrangement_of_polygeodesics_on_sphere_base_geo::
s_def_pg_vertex_color(.5f, 0, 0);

//! \brief constructor.
Arrangement_of_polygeodesics_on_sphere_base_geo::
Arrangement_of_polygeodesics_on_sphere_base_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  // Vertex
  m_pg_vertex_style(s_def_pg_vertex_style),
  m_pg_vertex_radius(s_def_pg_vertex_radius),
  m_pg_vertex_point_size(s_def_pg_vertex_point_size),
  m_pg_vertex_color(s_def_pg_vertex_color)
{ if (!proto) create_renderers(); }

//! \brief destructor.
Arrangement_of_polygeodesics_on_sphere_base_geo::
~Arrangement_of_polygeodesics_on_sphere_base_geo() {}

//! \brief initializes the container prototype.
void Arrangement_of_polygeodesics_on_sphere_base_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Arrangement_on_surface_geo::
                                    renderer_changed);
  // pgVertexStyleId
  Uint_handle_function pg_vertex_style_func =
    reinterpret_cast<Uint_handle_function>
    (&Arrangement_of_polygeodesics_on_sphere_base_geo::pg_vertex_style_handle);
  s_prototype->add_field_info(new SF_uint(PG_VERTEX_STYLE_ID,
                                          "pgVertexStyleId",
                                          RULE_EXPOSED_FIELD,
                                          pg_vertex_style_func,
                                          exec_func));
}

//! \brief deletes the container prototype.
void Arrangement_of_polygeodesics_on_sphere_base_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto*
Arrangement_of_polygeodesics_on_sphere_base_geo::get_prototype()
{
  if (!s_prototype)
    Arrangement_of_polygeodesics_on_sphere_base_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
void Arrangement_of_polygeodesics_on_sphere_base_geo::
set_attributes(Element* elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "pgVertexStyle") {
      set_pg_vertex_style(Vertex_shape::style(strip_double_quotes(value)));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "pgVertexRadius") {
      set_pg_vertex_radius(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "pgVertexPointSize") {
      set_pg_vertex_point_size(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "pgVertexColor") {
      Vector3f col(value);
      set_pg_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief
void Arrangement_of_polygeodesics_on_sphere_base_geo::
cull(Cull_context& cull_context) {}

//! \brief
void Arrangement_of_polygeodesics_on_sphere_base_geo::
isect(Isect_action* action)
{
  //! \todo isect with a sphere
}

//! \brief draws an arrangement on surface edge.
void Arrangement_of_polygeodesics_on_sphere_base_geo::
draw_aos_edge(Draw_action* action, Vector3f& src, Vector3f& trg,
              Vector3f& normal)
{
  draw_edge_on_sphere(action, src, trg, normal,
                      m_aos_edge_style, m_aos_edge_count,
                      m_aos_edge_directed, m_aos_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}

SGAL_END_NAMESPACE
