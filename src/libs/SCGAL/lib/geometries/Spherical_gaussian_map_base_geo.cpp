// Copyright (c) 2004, 2005, 2006 Israel.
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
// $Id: Spherical_gaussian_map_base_geo.cpp 7805 2009-07-19 22:21:38Z efif $
// $Revision: 7805 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#pragma warning(disable: 4503)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)
#endif

#include <time.h>
#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_3.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "SCGAL/Spherical_gaussian_map_base_geo.hpp"
#include "SCGAL/Exact_coord_array.hpp"

#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Cylinder.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Extrusion.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto * Spherical_gaussian_map_base_geo::s_prototype = NULL;

/*! Default values */
const Boolean Spherical_gaussian_map_base_geo::s_def_draw_aos(false);

const Boolean Spherical_gaussian_map_base_geo::s_def_draw_aos_opaque(false);
const Boolean Spherical_gaussian_map_base_geo::s_def_draw_aos_haloed(false);

const Boolean Spherical_gaussian_map_base_geo::s_def_draw_aos_surface(true);
const Vector3f Spherical_gaussian_map_base_geo::
s_def_aos_surface_color(0.5f, 0.5f, 0.5f);

// Default vertex attributes:
const Spherical_gaussian_map_base_geo::Vertex_style
  Spherical_gaussian_map_base_geo::
  s_def_aos_vertex_style(Vertex_shape::DISC);
const Float Spherical_gaussian_map_base_geo::s_def_aos_vertex_radius(0.06f);
const Float Spherical_gaussian_map_base_geo::s_def_aos_vertex_point_size(2);
const Vector3f Spherical_gaussian_map_base_geo::
s_def_aos_vertex_color(0.5f, 0, 0);

// Boundary Vertex:
const Spherical_gaussian_map_base_geo::Vertex_style
  Spherical_gaussian_map_base_geo::
  s_def_aos_boundary_vertex_style(Vertex_shape::BALL);
const Float Spherical_gaussian_map_base_geo::
s_def_aos_boundary_vertex_radius(0.06f);
const Float Spherical_gaussian_map_base_geo::
s_def_aos_boundary_vertex_point_size(2);
const Vector3f Spherical_gaussian_map_base_geo::
s_def_aos_boundary_vertex_color(0.5f, 0.5f, 0);

// Default edge attributes:
const Boolean Spherical_gaussian_map_base_geo::s_def_aos_edge_enabled(true);
const Spherical_gaussian_map_base_geo::Edge_style
  Spherical_gaussian_map_base_geo::
  s_def_aos_edge_style(Edge_shape::TUBE);
const Int Spherical_gaussian_map_base_geo::s_def_aos_edge_count(1);
const Boolean Spherical_gaussian_map_base_geo::s_def_aos_edge_directed(false);
const Float Spherical_gaussian_map_base_geo::s_def_aos_edge_radius(.03f);
const Float Spherical_gaussian_map_base_geo::s_def_aos_edge_line_width(1);
const Vector3f Spherical_gaussian_map_base_geo::s_def_aos_edge_color(0, 0, .5f);

// General rendering attributes:
const Float Spherical_gaussian_map_base_geo::s_def_aos_delta_angle(.1f);

REGISTER_TO_FACTORY(Spherical_gaussian_map_base_geo,
                    "Spherical_gaussian_map_base_geo");

/*! Constructor */
Spherical_gaussian_map_base_geo::
Spherical_gaussian_map_base_geo(Boolean proto) :
  Mesh_set(proto),
  m_translated(false),
  m_rotated(false),
  m_draw_aos(s_def_draw_aos), m_draw_primal(!s_def_draw_aos),
  m_draw_aos_opaque(s_def_draw_aos_opaque),
  m_draw_aos_haloed(s_def_draw_aos_haloed),
  m_draw_aos_surface(s_def_draw_aos_surface),
  m_aos_surface_color(s_def_aos_surface_color),
  m_aos_vertex_style(s_def_aos_vertex_style),
  m_aos_vertex_radius(s_def_aos_vertex_radius),
  m_aos_vertex_point_size(s_def_aos_vertex_point_size),
  m_aos_vertex_color(s_def_aos_vertex_color),
  m_aos_boundary_vertex_style(s_def_aos_boundary_vertex_style),
  m_aos_boundary_vertex_radius(s_def_aos_boundary_vertex_radius),
  m_aos_boundary_vertex_point_size(s_def_aos_boundary_vertex_point_size),
  m_aos_boundary_vertex_color(s_def_aos_boundary_vertex_color),
  m_aos_edge_enabled(s_def_aos_edge_enabled),
  m_aos_edge_style(s_def_aos_edge_style),
  m_aos_edge_count(s_def_aos_edge_count),
  m_aos_edge_directed(s_def_aos_edge_directed),
  m_aos_edge_radius(s_def_aos_edge_radius),
  m_aos_edge_line_width(s_def_aos_edge_line_width),
  m_aos_delta_angle(s_def_aos_delta_angle),

  m_renderer_dirty(true),
  m_surface_renderer(NULL),
  m_colored_surface_renderer(NULL),
  m_stencil_surface_renderer(NULL)
{
  if (!proto) {
    create_renderers();
    m_aos_edge_colors[0] = m_aos_edge_colors[1] = s_def_aos_edge_color;
    m_aos_sphere.set_stacks(64);
    m_aos_sphere.set_slices(64);
  }
}

/*! Copy Constructor */
Spherical_gaussian_map_base_geo::
Spherical_gaussian_map_base_geo(const Spherical_gaussian_map_base_geo & gm)
{
  // Not implemented yet!
  SGAL_assertion(0);
}

/*! Destructor */
Spherical_gaussian_map_base_geo::~Spherical_gaussian_map_base_geo()
{ clear(); }

/*! Clear the internal representation and auxiliary data structures */
void Spherical_gaussian_map_base_geo::clear()
{
  Mesh_set::clear();
  process_content_changed();
}

/*! */
void Spherical_gaussian_map_base_geo::cull(Cull_context & cull_context) {}

/*! */
void Spherical_gaussian_map_base_geo::isect(Isect_action * action)
{
  Context * context = action->get_context();
  if (!m_is_solid && context) context->draw_cull_face(Gfx::NO_CULL);

  isect_primary();
    
  if (!m_is_solid  && context) context->draw_cull_face(Gfx::BACK_CULL);
}

/*! Calculate the bounding sphere */
bool Spherical_gaussian_map_base_geo::calculate_sphere_bound()
{
  if (!m_is_sphere_bound_dirty) return false;
  if (is_dirty()) clean();
  if (m_bb_is_pre_set) return true;

  if (m_draw_aos) {
    m_sphere_bound.set_center(Vector3f(0, 0, 0));
    m_sphere_bound.set_radius(1);
  } else if (!m_bb_is_pre_set) {
    Approximate_sphere_vector spheres;
    transform_coords(spheres);
    if (!spheres.empty()) {
      Min_sphere min_sphere(spheres.begin(), spheres.end());
      Vector3f center_vec;
      std::copy(min_sphere.center_cartesian_begin(),
                min_sphere.center_cartesian_end(),
                &center_vec[0]);
      m_sphere_bound.set_center(center_vec);
      m_sphere_bound.set_radius(min_sphere.radius());
    }
  }
  m_is_sphere_bound_dirty = false;

  return true;
}

/*! \brief sets the attributes of the object extracted from an input file */
void Spherical_gaussian_map_base_geo::set_attributes(Element * elem)
{
  Mesh_set::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "drawAos") {
      m_draw_aos = compare_to_true(value);
      m_draw_primal = !m_draw_aos;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawAosOpaque") {
      m_draw_aos_opaque = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawAosHaloed") {
      m_draw_aos_haloed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawAosSphere") {
      m_draw_aos_surface = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosSphereColor") {
      Vector3f col(value);
      set_aos_surface_color(col);
      elem->mark_delete(ai);
      continue;
    }

    // AOS vertex attributes:
    if (name == "aosVertexStyle") {
      m_aos_vertex_style = Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosVertexRadius") {
      m_aos_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosPointSize") {
      m_aos_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosVertexColor") {
      Vector3f col(value);
      set_aos_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }

    // AOS edge attributes:
    if (name == "aosEdgeEnabled") {
      m_aos_edge_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeStyle") {
      m_aos_edge_style = Edge_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeCount") {
      m_aos_edge_count = boost::lexical_cast<Int>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeDirected") {
      m_aos_edge_directed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeRadius") {
      m_aos_edge_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosLineWidth") {
      sscanf(value.c_str(), "%f", &m_aos_edge_line_width);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeColor") {
      Vector3f col(value);
      set_aos_edge_color(col, 0);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeColor1") {
      Vector3f col(value);
      set_aos_edge_color(col, 0);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeColor2") {
      Vector3f col(value);
      set_aos_edge_color(col, 1);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "deltaAngle") {
      m_aos_delta_angle = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stencilReference") {
      Int ref = boost::lexical_cast<Int>(value);
      m_renderer.set_stencil_ref(ref);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosBoundaryVertexStyle") {
      m_aos_boundary_vertex_style =
        Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosBoundaryVertexRadius") {
      m_aos_boundary_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosBoundaryVertexPointSize") {
      m_aos_boundary_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosBoundaryVertexColor") {
      Vector3f col(value);
      m_aos_boundary_vertex_color = col;
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! */
void Spherical_gaussian_map_base_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the object fields to the prototype
  Execution_function exec_func =
    static_cast<Execution_function>(&Spherical_gaussian_map_base_geo::draw_changed);
  s_prototype->add_field_info(new SF_bool(DRAW_AOS, "drawAos",
                                          get_member_offset(&m_draw_aos),
                                          exec_func));
 
  s_prototype->
    add_field_info(new SF_bool(DRAW_AOS_OPAQUE, "drawAosOpaque",
                               get_member_offset(&m_draw_aos_opaque)));
  
  s_prototype->
    add_field_info(new SF_bool(DRAW_AOS_HALOED, "drawAosHaloed",
                               get_member_offset(&m_draw_aos_haloed)));

  s_prototype->
    add_field_info(new SF_bool(DRAW_AOS_SPHERE, "drawAosSphere",
                               get_member_offset(&m_draw_aos_surface)));
  
  s_prototype->
    add_field_info(new SF_float(AOS_EDGE_LINE_WIDTH, "aosLineWidth",
                               get_member_offset(&m_aos_edge_line_width)));

  exec_func = static_cast<Execution_function>(&Mesh_set::coord_changed);
  s_prototype->add_field_info(new SF_bool(TRANSLATED, "translated",
                                          get_member_offset(&m_translated),
                                          exec_func));
  s_prototype->add_field_info(new SF_bool(ROTATED, "rotated",
                                          get_member_offset(&m_rotated),
                                          exec_func));  

  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_PRIMAL, "trueDrawPrimal",
                                          get_member_offset(&m_draw_primal)));

  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_AOS, "trueDrawAos",
                                         get_member_offset(&m_draw_aos)));

  exec_func =
    static_cast<Execution_function>(&Spherical_gaussian_map_base_geo::output);
  s_prototype->add_field_info(new SF_bool(EXPORT, "export",
                                          get_member_offset(&m_export),
                                          exec_func));

  s_prototype->
    add_field_info(new SF_vector3f(AOS_EDGE_COLOR1, "aosEdgeColor1",
                                   get_member_offset(&m_aos_edge_colors[0])));
  s_prototype->
    add_field_info(new SF_vector3f(AOS_EDGE_COLOR2, "aosEdgeColor2",
                                   get_member_offset(&m_aos_edge_colors[1])));
}

/*! */
void Spherical_gaussian_map_base_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto * Spherical_gaussian_map_base_geo::get_prototype() 
{  
  if (!s_prototype) Spherical_gaussian_map_base_geo::init_prototype();
  return s_prototype;
}

/*! Raise the flag that indicates that the sphere bound changed
 */
void Spherical_gaussian_map_base_geo::
draw_changed(Field_info * /* field_info */)
{
  m_draw_primal = !m_draw_aos;
  m_is_sphere_bound_dirty = true;

  if (m_draw_aos) {
    Field * field = get_field(TRUE_DRAW_AOS);
    if (field) field->cascade();
  } else {
    Field * field = get_field(TRUE_DRAW_PRIMAL);
    if (field) field->cascade();
  }
}

/*! Draws the internal representation */
void Spherical_gaussian_map_base_geo::draw_geometry(Draw_action * action)
{  
  if (!m_draw_aos) {
    draw_primal(action);
    return;
  }

  if (m_renderer_dirty) clean_renderer();
  if (m_draw_aos_opaque) {
    draw_aos_opaque(action);
    return;
  }

  m_renderer(action);
}

/*! Reverse the coordinate indices */
void Spherical_gaussian_map_base_geo::
set_reverse_coord_indices(const SGAL::Array<Uint> & indices)
{
  m_coord_indices.resize(indices.size());
  Uint i = 0;
  const Uint * ii = indices.end() - 2;
  for (; ii >= indices.begin(); --ii) {
    m_coord_indices[i++] = *ii;
  }
  m_coord_indices[i++] = (Uint) -1;
}

/*! \biref processes change of points */
void Spherical_gaussian_map_base_geo::field_changed(Field_info * field_info)
{ clear(); }

/*! \brief creates the renderers */
void Spherical_gaussian_map_base_geo::create_renderers()
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

/*! \brief destroys the renderers */
void Spherical_gaussian_map_base_geo::destroy_renderers()
{
  if (m_surface_renderer) delete m_surface_renderer;
  if (m_colored_surface_renderer) delete m_colored_surface_renderer;
  if (m_stencil_surface_renderer) delete m_stencil_surface_renderer;
}

/*! \brief cleans the renderer */
void Spherical_gaussian_map_base_geo::clean_renderer()
{
  // Surface and faces:
  if (get_draw_aos_surface())
    m_renderer.push_back(m_colored_surface_renderer,
                      Arrangement_renderer::SURFACE);
  else m_renderer.push_back(m_stencil_surface_renderer,
                         Arrangement_renderer::SURFACE);

  // Depth:
  if (get_draw_aos_surface())
    m_renderer.push_back(m_surface_renderer, Arrangement_renderer::DEPTH);

  m_renderer_dirty = false;
}

/*! \brief draws an arrangement on sphere vertex */
void Spherical_gaussian_map_base_geo::draw_aos_vertex(Draw_action * action,
                                                      Vector3f & center)
{
  draw_vertex_on_sphere(action, center,
                        m_aos_vertex_style,
                        m_aos_vertex_radius,
                        m_aos_delta_angle);
}
  
/*! \brief Draw an arrangement on surface boundary_vertex */
void Spherical_gaussian_map_base_geo::
draw_aos_boundary_vertex(Draw_action * action, Vector3f & center)
{
  draw_vertex_on_sphere(action, center, m_aos_boundary_vertex_style,
                        m_aos_boundary_vertex_radius, m_aos_delta_angle);
}

/*! \brief draws an arrangement on sphere edge */
void Spherical_gaussian_map_base_geo::draw_aos_edge(Draw_action * action,
                                                    Vector3f & source,
                                                    Vector3f & target)
{
  draw_edge_on_sphere(action, source, target,
                      m_aos_edge_style,
                      m_aos_edge_count,
                      m_aos_edge_directed,
                      m_aos_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}
    
SGAL_END_NAMESPACE
