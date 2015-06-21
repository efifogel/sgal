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

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
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

#include "SCGAL/Spherical_gaussian_map_base_geo.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Spherical_gaussian_map_base_geo::s_prototype(nullptr);

//! default values
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
const Uint Spherical_gaussian_map_base_geo::s_def_aos_edge_count(1);
const Boolean Spherical_gaussian_map_base_geo::s_def_aos_edge_directed(false);
const Float Spherical_gaussian_map_base_geo::s_def_aos_edge_radius(.03f);
const Float Spherical_gaussian_map_base_geo::s_def_aos_edge_line_width(1);
const Vector3f Spherical_gaussian_map_base_geo::s_def_aos_edge_color(0, 0, .5f);

// General rendering attributes:
const Float Spherical_gaussian_map_base_geo::s_def_aos_delta_angle(.1f);

//! \brief constructor.
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
  m_dirty_sgm(true),
  m_renderer_dirty(true),
  m_surface_renderer(nullptr),
  m_colored_surface_renderer(nullptr),
  m_stencil_surface_renderer(nullptr)
{
  if (!proto) {
    create_renderers();
    m_aos_edge_colors[0] = m_aos_edge_colors[1] = s_def_aos_edge_color;
    m_aos_sphere.set_stacks(64);
    m_aos_sphere.set_slices(64);
  }
}

//! \brief copy constructor.
Spherical_gaussian_map_base_geo::
Spherical_gaussian_map_base_geo(const Spherical_gaussian_map_base_geo& gm)
{
  // Not implemented yet!
  SGAL_assertion(0);
}

//! \brief destructor.
Spherical_gaussian_map_base_geo::~Spherical_gaussian_map_base_geo() {}

//! \brief clears the internal representation and auxiliary data structures.
void Spherical_gaussian_map_base_geo::clear_sgm() {}

//! \brief determines whether the representation is empty.
Boolean Spherical_gaussian_map_base_geo::is_sgm_empty() const { return true; }

//! \brief draws the geometry for selection.
void Spherical_gaussian_map_base_geo::isect(Isect_action* action)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_sgm) clean_sgm();

  Context* context = action->get_context();
  if (!m_is_solid && context) context->draw_cull_face(Gfx::NO_CULL);

  isect_primary();

  if (!m_is_solid  && context) context->draw_cull_face(Gfx::BACK_CULL);
}

//! \brief cleans the bounding sphere of the spherical Gaussian map.
void Spherical_gaussian_map_base_geo::clean_bounding_sphere()
{
  if (m_bb_is_pre_set) {
    m_dirty_bounding_sphere = false;
    return;
  }

  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_sgm) clean_sgm();

  if (m_draw_aos) {
    m_bounding_sphere.set_center(Vector3f(0, 0, 0));
    m_bounding_sphere.set_radius(1);
  }
  else {
    Inexact_sphere_vector spheres;
    transform_coords(spheres);
    if (!spheres.empty()) {
      Min_sphere min_sphere(spheres.begin(), spheres.end());
      Vector3f center_vec;
      std::copy(min_sphere.center_cartesian_begin(),
                min_sphere.center_cartesian_end(),
                &center_vec[0]);
      m_bounding_sphere.set_center(center_vec);
      m_bounding_sphere.set_radius(min_sphere.radius());
    }
  }
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of the object extracted from an input file.
void Spherical_gaussian_map_base_geo::set_attributes(Element* elem)
{
  Mesh_set::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
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
      m_aos_edge_count = boost::lexical_cast<Uint>(value);
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
      Int32 ref = boost::lexical_cast<Int32>(value);
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

//! \breif initializes the container prototype.
void Spherical_gaussian_map_base_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());

  // Add the object fields to the prototype
  // drawAos
  auto exec_func =
    static_cast<Execution_function>(&Spherical_gaussian_map_base_geo::
                                    draw_changed);
  auto draw_aos_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::draw_aos_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_AOS, "drawAos",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_func,
                                          exec_func));

  // drawAosOpaque
  auto draw_aos_opaque_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::draw_aos_opaque_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_AOS_OPAQUE, "drawAosOpaque",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_opaque_func));

  // drawAosHaloed
  auto draw_aos_haloed_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::draw_aos_haloed_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_AOS_HALOED,
                                          "drawAosHaloed",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_haloed_func));

  // drawAosSphere
  auto draw_aos_surface_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::draw_aos_surface_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_AOS_SPHERE,
                                          "drawAosSphere",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_surface_func));

  // aosLineWidth
  auto aos_edge_line_width_func =
    static_cast<Float_handle_function>
    (&Spherical_gaussian_map_base_geo::aos_edge_line_width_handle);
  s_prototype->add_field_info(new SF_float(AOS_EDGE_LINE_WIDTH,
                                           "aosLineWidth",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           aos_edge_line_width_func));

  // translated
  exec_func = static_cast<Execution_function>
    (&Spherical_gaussian_map_base_geo::coord_changed);
  auto translated_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::translated_handle);
  s_prototype->add_field_info(new SF_bool(TRANSLATED,
                                          "translated",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          translated_func,
                                          exec_func));

  // rotated
  auto rotated_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::rotated_handle);
  s_prototype->add_field_info(new SF_bool(ROTATED, "rotated",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          rotated_func,
                                          exec_func));

  // trueDrawPrimal
  auto draw_primal_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_base_geo::draw_primal_handle);
  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_PRIMAL,
                                          "trueDrawPrimal",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_primal_func));

  // trueDrawAos
//   Boolean_handle_function draw_aos_func =
//     static_cast<Boolean_handle_function>
//     (&Spherical_gaussian_map_base_geo::draw_aos_handle);
  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_AOS,
                                          "trueDrawAos",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_func));

  // aosEdgeColor1
  auto aos_edge_colors1_func =
    static_cast<Vector3f_handle_function>
    (&Spherical_gaussian_map_base_geo::aos_edge_colors1_handle);
  s_prototype->add_field_info(new SF_vector3f(AOS_EDGE_COLOR1,
                                              "aosEdgeColor1",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              aos_edge_colors1_func));

  // aosEdgeColor2
  auto aos_edge_colors2_func =
    static_cast<Vector3f_handle_function>
    (&Spherical_gaussian_map_base_geo::aos_edge_colors2_handle);
  s_prototype->add_field_info(new SF_vector3f(AOS_EDGE_COLOR2,
                                              "aosEdgeColor2",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              aos_edge_colors2_func));
}

//!
void Spherical_gaussian_map_base_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//!
Container_proto* Spherical_gaussian_map_base_geo::get_prototype()
{
  if (!s_prototype) Spherical_gaussian_map_base_geo::init_prototype();
  return s_prototype;
}

//! \brief raises the flag that indicates that the sphere bound changed.
void Spherical_gaussian_map_base_geo::
draw_changed(const Field_info* /* field_info */)
{
  m_draw_primal = !m_draw_aos;
  m_dirty_bounding_sphere = true;

  if (m_draw_aos) {
    Field* field = get_field(TRUE_DRAW_AOS);
    if (field) field->cascade();
  }
  else {
    Field* field = get_field(TRUE_DRAW_PRIMAL);
    if (field) field->cascade();
  }
}

//! \brief drawss the internal representation.
void Spherical_gaussian_map_base_geo::draw_geometry(Draw_action* action)
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

//! \brief creates the renderers.
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

//! \brief destroys the renderers.
void Spherical_gaussian_map_base_geo::destroy_renderers()
{
  if (m_surface_renderer) delete m_surface_renderer;
  if (m_colored_surface_renderer) delete m_colored_surface_renderer;
  if (m_stencil_surface_renderer) delete m_stencil_surface_renderer;
}

//! \brief cleans the renderer.
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

//! \brief draws an arrangement on sphere vertex.
void Spherical_gaussian_map_base_geo::draw_aos_vertex(Draw_action* action,
                                                      Vector3f& center)
{
  draw_vertex_on_sphere(action, center,
                        m_aos_vertex_style,
                        m_aos_vertex_radius,
                        m_aos_delta_angle);
}

//! \brief Draw an arrangement on surface boundary_vertex.
void Spherical_gaussian_map_base_geo::
draw_aos_boundary_vertex(Draw_action* action, Vector3f& center)
{
  draw_vertex_on_sphere(action, center, m_aos_boundary_vertex_style,
                        m_aos_boundary_vertex_radius, m_aos_delta_angle);
}

//! \brief draws an arrangement on sphere edge.
void Spherical_gaussian_map_base_geo::draw_aos_edge(Draw_action* action,
                                                    Vector3f& source,
                                                    Vector3f& target,
                                                    Vector3f& normal)
{
  draw_edge_on_sphere(action, source, target, normal,
                      m_aos_edge_style,
                      m_aos_edge_count,
                      m_aos_edge_directed,
                      m_aos_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}

//! \brief processes change of coordinate field.
void
Spherical_gaussian_map_base_geo::coord_changed(const Field_info* field_info)
{
  clear_sgm();
  Mesh_set::coord_changed(field_info);
}

//! \brief processes change of points.
void
Spherical_gaussian_map_base_geo::field_changed(const Field_info* field_info)
{
  clear_sgm();
  Container::field_changed(field_info);
}

//! \brief cleans the geometry.
void Spherical_gaussian_map_base_geo::clean_sgm() {}

//! \brief draws the geometry.
void Spherical_gaussian_map_base_geo::draw(Draw_action* action)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  if (m_dirty_sgm) clean_sgm();
  if (is_sgm_empty()) return;

  draw_mesh(action);
}

SGAL_END_NAMESPACE
