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
// $Id: Arrangement_on_surface_geo.hpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A pure geometry container that represents an arrangement embeded on a
 * parametric surface
 */

#ifndef SCGAL_ARRANGEMENT_ON_SURFACE_GEO_HPP
#define SCGAL_ARRANGEMENT_ON_SURFACE_GEO_HPP

#include <CGAL/basic.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_renderer.hpp"
#include "SCGAL/Arrangement_renderers.hpp"

SGAL_BEGIN_NAMESPACE

class Coord_array;
class Index_array;
class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;

/*! A geometry container that represents an arrangement embeded on a
 * parametric surface.
 */
class SGAL_SCGAL_DECL Arrangement_on_surface_geo : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    INSERT_STRATEGY,
    DRAW_OPAQUE,
    DRAW_HALOED,
    DRAW_HALFTONE,
    //
    DRAW_AOS_SURFACE,
    AOS_SURFACE_COLOR,
    //
    AOS_VERTEX_STYLE,
    AOS_VERTEX_STYLE_ID,
    AOS_VERTEX_RADIUS,
    AOS_VERTEX_POINT_SIZE,
    AOS_VERTEX_COLOR,
    //
    AOS_ISOLATED_VERTEX_STYLE,
    AOS_ISOLATED_VERTEX_STYLE_ID,
    AOS_ISOLATED_VERTEX_RADIUS,
    AOS_ISOLATED_VERTEX_POINT_SIZE,
    AOS_ISOLATED_VERTEX_COLOR,
    //
    AOS_BOUNDARY_VERTEX_STYLE,
    AOS_BOUNDARY_VERTEX_STYLE_ID,
    AOS_BOUNDARY_VERTEX_RADIUS,
    AOS_BOUNDARY_VERTEX_POINT_SIZE,
    AOS_BOUNDARY_VERTEX_COLOR,
    //
    AOS_EDGE_ENABLED,
    AOS_EDGE_STYLE_ID,
    AOS_EDGE_COUNT_ID,
    AOS_EDGE_DIRECTED,
    AOS_EDGE_RADIUS,
    AOS_EDGE_LINE_WIDTH,
    AOS_EDGE_COLOR,
    LAST
  };

  /*! The insertion strategies. */
  enum Insertion_strategy {
    AGGREGATE = 0,
    INCREMENT,
    NUM_INSERTION_STRATEGIES
  };

protected:
  typedef Arrangement_on_surface_geo                  Self;

  // Approximate types:
  typedef float                                       Approximate_NT;
  typedef CGAL::Cartesian<Approximate_NT>             Approximate_kernel;

  typedef CGAL::Min_sphere_of_spheres_d_traits_3<Approximate_kernel,
                                                 Approximate_NT>
    Min_sphere_traits;
  typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits>
    Min_sphere;
  typedef Min_sphere_traits::Sphere                   Approximate_sphere_3;
  typedef Approximate_kernel::Point_3                 Approximate_point_3;
  typedef Approximate_kernel::Vector_3                Approximate_vector_3;

  typedef std::vector<Approximate_sphere_3>
    Approximate_sphere_vector;
  typedef Approximate_sphere_vector::const_iterator   Approximate_sphere_iter;

  typedef Arrangement_renderer::Vertex_shape          Vertex_shape;
  typedef Vertex_shape::Style                         Vertex_style;

  typedef Arrangement_renderer::Edge_shape            Edge_shape;
  typedef Edge_shape::Style                           Edge_style;

public:
  /*! Constructor. */
  Arrangement_on_surface_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Arrangement_on_surface_geo();

  /*! Clone. */
  virtual Container* clone() = 0;

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* draw_opaque_handle(Field_info*) { return &m_draw_opaque; }
  Boolean* draw_haloed_handle(Field_info*) { return &m_draw_haloed; }
  Boolean* draw_halftone_handle(Field_info*) { return &m_draw_halftone; }
  Boolean* draw_aos_surface_handle(Field_info*) { return &m_draw_aos_surface; }
  Vertex_style* aos_vertex_style_handle(Field_info*)
  { return &m_aos_vertex_style; }
  Vertex_style* aos_isolated_vertex_style_handle(Field_info*)
  { return &m_aos_isolated_vertex_style; }
  Float* aos_isolated_vertex_radius_handle(Field_info*)
  { return &m_aos_isolated_vertex_radius; }
  Boolean* aos_edge_enabled_handle(Field_info*) { return &m_aos_edge_enabled; }
  Edge_style* aos_edge_style_handle(Field_info*) { return &m_aos_edge_style; }
  Uint* aos_edge_count_handle(Field_info*) { return &m_aos_edge_count; }
  Boolean* aos_edge_directed_handle(Field_info*)
  { return &m_aos_edge_directed; }
  //@}

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation. */
  virtual void clean() { m_dirty = false; }

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* /* action */) {}

  /*! */
  virtual Boolean clean_sphere_bound() { return false; }

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action);

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear() {}

  /*! Determine whether the representation hasn't been updated. */
  virtual Boolean is_dirty() const { return m_dirty; }

  /*! Determine whether the representation empty. */
  virtual Boolean is_empty() const = 0;

  /*! Draw an arrangement on surface vertex.
   * \param action
   * \param center the vertex center.
   */
  virtual void draw_aos_vertex(Draw_action* /* action */,
                               Vector3f& /* center */)
  {}

  /*! Draw an arrangement on surface isolated vertex.
   * \param action
   * \param center the vertex center.
   */
  virtual void draw_aos_isolated_vertex(Draw_action* /* action */,
                                        Vector3f& /* center */)
  {}

  /*! Draw an arrangement on surface boundary_vertex.
   * \param action
   * \param center the vertex center.
   */
  virtual void draw_aos_boundary_vertex(Draw_action* /* action */,
                                        Vector3f& /* center */)
  {}

  /*! Draw an arrangement on surface edge.
   * \param action
   * \param source the edge source point.
   * \param target the edge target point.
   */
  virtual void draw_aos_edge(Draw_action* /* action */, Vector3f& /* source */,
                             Vector3f& /* target */, Vector3f& /* normal */)
  {}

  /*! Clean the renderer. */
  virtual void clean_renderer();

  /*! Draw the arrangement on surface edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* /* action */) {}

  /*! Draw the arrangement on surface vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* /* action */) {}

  /*! Draw the arrangement on surface isolated vertices.
   * \param action
   */
  virtual void draw_aos_isolated_vertices(Draw_action* /* action */) {}

  /*! Raise the flag that indicates that the renderer changed. */
  void renderer_changed(Field_info* field_info = NULL);

  /*! Obtain the flag that indicates whether to draw the embedding surface. */
  Boolean get_draw_aos_surface() const { return m_draw_aos_surface; }

  /*! Set the flag that indicates whether to draw the embedding surface. */
  void set_draw_aos_surface(Boolean flag) { m_draw_aos_surface = flag; }

  /*! Obtain the flag that indicates whether to draw the backfacing features
   * halftone.
   */
  Boolean get_draw_halftone() const { return m_draw_halftone; }

  /*! Obtain the surface color. */
  const Vector3f& get_aos_surface_color() const { return m_aos_surface_color; }

  /*! Obtain the vertex-shape style. */
  Vertex_style get_aos_vertex_style() const { return m_aos_vertex_style; }

  /*! Set the vertex-shape style. */
  void set_aos_vertex_style(Vertex_style style) { m_aos_vertex_style = style; }

  /*! Set the aos vertex color.
   * \param color the new color.
   */
  void set_aos_vertex_color(Vector3f& color) { m_aos_vertex_color = color; }

  /*! Obtain the vertex radius */
  Float get_aos_vertex_radius() const { return m_aos_vertex_radius; }

  /*! Set the vertex radius */
  void set_aos_vertex_radius(Float radius) { m_aos_vertex_radius = radius; }

  /*! Obtain the vertex point size */
  Float get_aos_vertex_point_size() const { return m_aos_vertex_point_size; }

  /*! Obtain the aos vertex color.
   * \return the vertex color.
   */
  const Vector3f& get_aos_vertex_color() const { return m_aos_vertex_color; }

  /*! Obtain the isolated vertex-shape style. */
  Vertex_style get_aos_isolated_vertex_style() const
  { return m_aos_isolated_vertex_style; }

  /*! Set the isolated vertex-shape style. */
  void set_aos_isolated_vertex_style(Vertex_style style)
  { m_aos_isolated_vertex_style = style; }

  /*! Obtain the isolated vertex point size. */
  Float get_aos_isolated_vertex_point_size() const
  { return m_aos_isolated_vertex_point_size; }

  /*! Set the isolated vertex point size. */
  void set_aos_isolated_vertex_point_size(Float size)
  { m_aos_isolated_vertex_point_size = size; }

  /*! Obtain the isolated_vertex radius. */
  Float get_aos_isolated_vertex_radius() const
  { return m_aos_isolated_vertex_radius; }

  /*! Set the isolated_vertex radius. */
  void get_aos_isolated_vertex_radius(Float radius)
  { m_aos_isolated_vertex_radius = radius; }

  /*! Obtain the aos isolated vertex color.
   * \return the isolated vertex color.
   */
  const Vector3f& get_aos_isolated_vertex_color() const
  { return m_aos_isolated_vertex_color; }

  /*! Set the aos isolated vertex color.
   * \param color the isolated vertex color.
   */
  void set_aos_isolated_vertex_color(const Vector3f& color)
  { m_aos_isolated_vertex_color = color; }

  /*! Obtain the boundary vertex-shape style. */
  Vertex_style get_aos_boundary_vertex_style() const
  { return m_aos_boundary_vertex_style; }

  /*! Obtain the boundary vertex point size. */
  Float get_aos_boundary_vertex_point_size() const
  { return m_aos_boundary_vertex_point_size; }

  /*! Obtain the boundary_vertex radius. */
  Float get_aos_boundary_vertex_radius() const
  { return m_aos_boundary_vertex_radius; }

  /*! Obtain the aos boundary vertex color.
   * \return the boundary vertex color.
   */
  const Vector3f& get_aos_boundary_vertex_color() const
  { return m_aos_boundary_vertex_color; }

  /*! Set the aos edge color.
   * \param color the new color.
   */
  void set_aos_edge_color(Vector3f& color) { m_aos_edge_color = color; }

  /*! Obtain the aos edge color.
   * \return the edge color.
   */
  const Vector3f& get_aos_edge_color() const { return m_aos_edge_color; }

  /*! Enable edge rendering. */
  void enable_aos_edge() { m_aos_edge_enabled = true; }

  /*! Disable edge rendering. */
  void disable_aos_edge() { m_aos_edge_enabled = false; }

  /*! Determine whether edge rendering is enabled. */
  Boolean is_aos_edge_enabled() const { return m_aos_edge_enabled; }

  /*! Obtain the edge shape style. */
  Edge_style get_aos_edge_style() const { return m_aos_edge_style; }

  /*! Set the edge shape style. */
  void set_aos_edge_style(Edge_style style) { m_aos_edge_style = style; }

  /*! Obtain the edge shape count. */
  Uint get_aos_edge_count() const { return m_aos_edge_count; }

  /*! Set the edge shape count. */
  void set_aos_edge_count(Uint count) { m_aos_edge_count = count; }

  /*! Determine whether edges are rendered directed. */
  Boolean get_aos_edge_directed() const { return m_aos_edge_directed; }

  /*! Set the flag that determines whether edges are rendered directed. */
  void set_aos_edge_directed(Boolean dir) { m_aos_edge_directed = dir; }

  /*! Obtain the edge radius. */
  Float get_aos_edge_radius() const { return m_aos_edge_radius; }

  /*! Set the edge radius. */
  void set_aos_edge_radius(Float radius) { m_aos_edge_radius = radius; }

  /*! Obtain the edge line width. */
  Float get_aos_edge_line_width() const { return m_aos_edge_line_width; }

  /*! Set the edge line width. */
  void set_aos_edge_line_width(Float width) { m_aos_edge_line_width = width; }

  /*! Obtain the delta angle. */
  Float get_aos_delta_angle() const { return m_aos_delta_angle; }

  /*! Set the stencil reference value. */
  void set_stencil_ref(Int ref) { m_renderer.set_stencil_ref(ref); }

 protected:
  /*! Insertion strategy names. */
  static const char* s_insertion_strategy_names[];

  /*! Indicates whether the mesh must be cleaned. */
  Boolean m_dirty;

  /*! Indicates the insertion strategy. */
  Insertion_strategy m_insertion_strategy;

  /*! Indicates whether to draw the sphere opaque. */
  Boolean m_draw_opaque;

  /*! Indicates whether to draw haloed lines. */
  Boolean m_draw_haloed;

  /*! Indicates whether to apply halftoning to the backfacing edges abd
   * vertices.
   */
  Boolean m_draw_halftone;

  /*! Indicates whether to draw the embedding surface. */
  Boolean m_draw_aos_surface;

  /*! The embedding surface color. */
  Vector3f m_aos_surface_color;

  /*! The vertex shape. */
  Vertex_style m_aos_vertex_style;

  /*! The radius of the disc or ball that represents a vertex. */
  Float m_aos_vertex_radius;

  /*! The size of the point that represents a vertex. */
  Float m_aos_vertex_point_size;

  /*! The color of the vertices. */
  Vector3f m_aos_vertex_color;

  /*! The isolated vertex shape. */
  Vertex_style m_aos_isolated_vertex_style;

  /*! The radius of the disc or ball that represents an isolated vertex. */
  Float m_aos_isolated_vertex_radius;

  /*! The size of the point that represents an isolated vertex. */
  Float m_aos_isolated_vertex_point_size;

  /*! The color of the isolated vertices. */
  Vector3f m_aos_isolated_vertex_color;

  /*! The boundary vertex shape. */
  Vertex_style m_aos_boundary_vertex_style;

  /*! The radius of the disc or ball that represents an boundary vertex. */
  Float m_aos_boundary_vertex_radius;

  /*! The size of the point that represents an boundary vertex. */
  Float m_aos_boundary_vertex_point_size;

  /*! The color of the boundary vertices. */
  Vector3f m_aos_boundary_vertex_color;

  /*! Indicates whether the rendering of edges is enabled or not. */
  Boolean m_aos_edge_enabled;

  /*! The edge rendering style. */
  Edge_style m_aos_edge_style;

  /*! The edge rendering type. */
  Uint m_aos_edge_count;

  /*! Indicates whether edges are rendered directed or not. */
  Boolean m_aos_edge_directed;

  /*! The radius of the tube that represents an edge. */
  Float m_aos_edge_radius;

  /*! The width of the line that represents an edge. */
  Float m_aos_edge_line_width;

  /*! The angle of a single line strip of a spherical arc. */
  Float m_aos_delta_angle;

  /*! The color of the edges. */
  Vector3f m_aos_edge_color;

  /*! Indicates whether the renderer must be cleaned. */
  Boolean m_renderer_dirty;

  /*! The renderer of the arrangement data structure. */
  Arrangement_renderer m_renderer;

  /*! The surface renderer. */
  Arrangement_renderer::Renderer* m_surface_renderer;

  /*! The surface renderer. */
  Arrangement_renderer::Renderer* m_colored_surface_renderer;

  /*! The surface renderer. */
  Arrangement_renderer::Renderer* m_stencil_surface_renderer;

  /*! The edges renderer. */
  Arrangement_renderer::Renderer* m_edges_renderer;

  /*! The vertices renderer. */
  Arrangement_renderer::Renderer* m_vertices_renderer;

  /*! The isolated vertices renderer. */
  Arrangement_renderer::Renderer* m_isolated_vertices_renderer;

  /*! The edges renderer. */
  Arrangement_renderer::Renderer* m_colored_edges_renderer;

  /*! The vertices renderer. */
  Arrangement_renderer::Renderer* m_colored_vertices_renderer;

  /*! The isolated vertices renderer. */
  Arrangement_renderer::Renderer* m_colored_isolated_vertices_renderer;

  /*! The line edges renderer. */
  Arrangement_renderer::Renderer* m_line_colored_edges_renderer;

  /*! The point vertices renderer. */
  Arrangement_renderer::Renderer* m_point_colored_vertices_renderer;

  /*! The ring vertices renderer. */
  Arrangement_renderer::Renderer* m_ring_colored_vertices_renderer;

  /*! The point isolated vertices renderer. */
  Arrangement_renderer::Renderer* m_point_colored_isolated_vertices_renderer;

  /*! The ring isolated vertices renderer. */
  Arrangement_renderer::Renderer* m_ring_colored_isolated_vertices_renderer;

  /*! The inflated line edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_line_edges_renderer;

  /*! The inflated strip edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_strip_edges_renderer;

  /*! The inflated tube edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_tube_edges_renderer;

  /*! Draw the arrangement on surface opaque.
   * \param action
   */
  virtual void draw_opaque(Draw_action* action);

  /*! Draw the vertices of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_vertices(Aos* aos, Draw_action* action)
  {
    typedef typename Aos::Vertex_const_iterator Vertex_const_iterator;
    typedef typename Aos::Geometry_traits_2     Geom_traits;
    typedef typename Geom_traits::Point_2       Point;

    Vertex_const_iterator vi;
    for (vi = aos->vertices_begin(); vi != aos->vertices_end(); ++vi) {
      if (vi->is_isolated()) continue;
      Vector3f center = to_vector3f(vi->point());
      center.normalize();
      draw_aos_vertex(action, center);
    }
  }

  /*! Draw the isolated vertices of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_isolated_vertices(Aos* aos, Draw_action* action)
  {
    typedef typename Aos::Vertex_const_iterator Vertex_const_iterator;
    typedef typename Aos::Geometry_traits_2     Geom_traits;
    typedef typename Geom_traits::Point_2       Point;

    Vertex_const_iterator vi;
    for (vi = aos->vertices_begin(); vi != aos->vertices_end(); ++vi) {
      if (!(vi->is_isolated())) continue;
      Vector3f center = to_vector3f(vi->point());
      center.normalize();
      draw_aos_isolated_vertex(action, center);
    }
  }

  /*! Draw the edges of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_edges(Aos* aos, Draw_action* action)
  {
    typedef typename Aos::Edge_const_iterator           Edge_const_iterator;
    typedef typename Aos::Geometry_traits_2             Geom_traits;
    typedef typename Geom_traits::Point_2               Point;
    typedef typename Geom_traits::X_monotone_curve_2    X_monotone_curve;

    Edge_const_iterator hei;
    for (hei = aos->edges_begin(); hei != aos->edges_end(); ++hei) {
      const X_monotone_curve& curve = hei->curve();
      Vector3f src = to_vector3f(curve.source());
      Vector3f trg = to_vector3f(curve.target());
      Vector3f normal = to_vector3f(curve.normal());
      src.normalize();
      trg.normalize();
      draw_aos_edge(action, src, trg, normal);
    }
  }

  /*! Print statistics. */
  template <typename Aos_geo>
  void print_stat(Aos_geo* aos_geo)
  {
    std::cout << "Information for " << aos_geo->get_name() << ":" << std::endl;
    if (aos_geo->is_dirty()) aos_geo->clean();

    std::cout << "# vertices: " << aos_geo->get_aos()->number_of_vertices()
              << std::endl
              << "# edges: " << aos_geo->get_aos()->number_of_edges()
              << std::endl
              << "# faces: " << aos_geo->get_aos()->number_of_faces()
              << std::endl;
  }

private:
  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! Default values. */
  static const Insertion_strategy s_def_insertion_strategy;

  static const Boolean s_def_draw_opaque;
  static const Boolean s_def_draw_haloed;
  static const Boolean s_def_draw_halftone;

  static const Boolean s_def_draw_aos_surface;
  static const Vector3f s_def_aos_surface_color;

  static const Vertex_style s_def_aos_vertex_style;
  static const Float s_def_aos_vertex_radius;
  static const Float s_def_aos_vertex_point_size;
  static const Vector3f s_def_aos_vertex_color;

  static const Vertex_style s_def_aos_isolated_vertex_style;
  static const Float s_def_aos_isolated_vertex_radius;
  static const Float s_def_aos_isolated_vertex_point_size;
  static const Vector3f s_def_aos_isolated_vertex_color;

  static const Vertex_style s_def_aos_boundary_vertex_style;
  static const Float s_def_aos_boundary_vertex_radius;
  static const Float s_def_aos_boundary_vertex_point_size;
  static const Vector3f s_def_aos_boundary_vertex_color;

  static const Boolean s_def_aos_edge_enabled;
  static const Edge_style s_def_aos_edge_style;
  static const Uint s_def_aos_edge_count;
  static const Boolean s_def_aos_edge_directed;
  static const Float s_def_aos_edge_radius;
  static const Float s_def_aos_edge_line_width;
  static const Float s_def_aos_delta_angle;
  static const Vector3f s_def_aos_edge_color;
};

SGAL_END_NAMESPACE

#endif
