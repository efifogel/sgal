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
// $Id: Arrangement_on_sphere_marked_geo.hpp 7789 2009-07-19 12:40:30Z efif $
// $Revision: 7789 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere with an extended dcel. Every vertex,
 * halfedge, and face is extended with a Boolean mark.
 */

#ifndef SCGAL_ARRANGEMENT_ON_SPHERE_MARKED_GEO_HPP
#define SCGAL_ARRANGEMENT_ON_SPHERE_MARKED_GEO_HPP


#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_marked.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_marked_overlay_traits.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */
class SGAL_SCGAL_DECL Arrangement_on_sphere_marked_geo :
  public Arrangement_on_sphere_base_geo
{
public:
  enum {
    FIRST = Arrangement_on_sphere_base_geo::LAST - 1,
    OVERLAY,
    AOS_MARKED_VERTEX_STYLE_ID,
    AOS_MARKED_VERTEX_RADIUS,
    AOS_MARKED_VERTEX_POINT_SIZE,
    AOS_MARKED_VERTEX_COLOR,
    AOS_MARKED_EDGE_ENABLED,
    AOS_MARKED_EDGE_STYLE_ID,
    AOS_MARKED_EDGE_COUNT_ID,
    AOS_MARKED_EDGE_DIRECTED,
    AOS_MARKED_EDGE_RADIUS,
    AOS_MARKED_EDGE_LINE_WIDTH,
    AOS_MARKED_EDGE_COLOR,
    LAST
  };

  typedef SGAL::Arrangement_on_sphere_marked        Arrangement_on_surface;
  typedef Arrangement_on_surface::Geometry_traits_2 Aos_geom_traits;
  typedef Arrangement_on_surface::Topology_traits   Aos_topol_traits;

  typedef boost::shared_ptr<Arrangement_on_sphere_marked_geo>
    Shared_arrangement_on_sphere_marked_geo;

protected:
  typedef Arrangement_on_surface                   Aos_marked;

  typedef Aos_topol_traits::Default_point_location_strategy
                                                   Aos_point_location_strategy;

public:
  /*! Constructor. */
  Arrangement_on_sphere_marked_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Arrangement_on_sphere_marked_geo();

  /* Construct the prototype. */
  static Arrangement_on_sphere_marked_geo* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* aos_marked_vertex_radius_handle(Field_info*)
  { return &m_aos_marked_vertex_radius; }
  Float* aos_marked_edge_radius_handle(Field_info*)
  { return &m_aos_marked_edge_radius; }
  //@}

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation. */
  virtual void clean();

  /*! */
  virtual void cull(Cull_context& /* cull_context */) {}

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();

  /*! Determine whether the representation empty. */
  virtual Boolean is_empty() const { return m_aos->is_empty(); }

  /*! Obrain the arrangement. */
  Aos_marked* get_aos();

  /*! Set the arrangement. */
  void set_aos(Aos_marked* aos);

  /*! Add a geometry container that represents an arrangement on a
   * sphere to the list of such geometry containers.
   */
  void add_aos_geo(Shared_arrangement_on_sphere_marked_geo aos_geo)
  { m_aoses.push_back(aos_geo); }

  /*! Obtain the overlay traits (const version). */
  const Arrangement_marked_overlay_traits<Aos_marked>& get_overlay_traits()
    const
  { return m_overlay_traits; }

  /*! Obtain the overlay traits (non-const version). */
  Arrangement_marked_overlay_traits<Aos_marked>& get_overlay_traits()
  { return m_overlay_traits; }

  // Marked vertex attributes:

  /*! Obtain the marked vertex shape style. */
  Vertex_style get_aos_marked_vertex_style() const
  { return m_aos_marked_vertex_style; }

  /*! Set the marked vertex shape style. */
  void set_aos_marked_vertex_style(Vertex_style style)
  { m_aos_marked_vertex_style = style; }

  /*! Obtain the marked vertex point size. */
  Float get_aos_marked_vertex_point_size() const
  { return m_aos_marked_vertex_point_size; }

  /*! Set the marked vertex point size. */
  void set_aos_marked_vertex_point_size(Float size)
  { m_aos_marked_vertex_point_size = size; }

  /*! Obtain the marked vertex radius. */
  Float get_aos_marked_vertex_radius() const
  { return m_aos_marked_vertex_radius; }

  /*! Set the marked vertex radius. */
  void get_aos_marked_vertex_radius(Float radius)
  { m_aos_marked_vertex_radius = radius; }

  /*! Obtain the marked vertex color. */
  const Vector3f& get_aos_marked_vertex_color() const
  { return m_aos_marked_vertex_color; }

  // Marked edge attributes:

  /*! Enable marked edge rendering. */
  void enable_aos_marked_edge() { m_aos_marked_edge_enabled = true; }

  /*! Disable marked edge rendering. */
  void disable_aos_marked_edge() { m_aos_marked_edge_enabled = false; }

  /*! Determine whether marked edge rendering is enabled. */
  Boolean is_aos_marked_edge_enabled() const
  { return m_aos_marked_edge_enabled; }

  /*! Obtain the marked edge style. */
  Edge_style get_aos_marked_edge_style() const
  { return m_aos_marked_edge_style; }

  /*! Set the marked edge style. */
  void set_aos_marked_edge_style(Edge_style style)
  { m_aos_marked_edge_style = style; }

  /*! Obtain the marked edge type. */
  Uint get_aos_marked_edge_count() const
  { return m_aos_marked_edge_count; }

  /*! Set the marked edge type */
  void set_aos_marked_edge_count(Uint count)
  { m_aos_marked_edge_count = count; }

  /*! Determine whether marked edges are rendered directed. */
  Boolean get_aos_marked_edge_directed() const
  { return m_aos_marked_edge_directed; }

  /*! Set the flag that determines whether marked edges are rendered directed. */
  void set_aos_marked_edge_directed(Boolean dir)
  { m_aos_marked_edge_directed = dir; }

  /*! Obtaint the radius of the tubes that represent aos marked edges. */
  Float get_aos_marked_edge_radius() const { return m_aos_marked_edge_radius; }

  /*! Set the radius of the tubes that represent aos marked edges. */
  void set_aos_marked_edge_radius(Float radius)
  { m_aos_marked_edge_radius = radius; }

  /*! Obtaint the width of the lines that represent aos marked edges. */
  Float get_aos_marked_edge_line_width() const
  { return m_aos_marked_edge_line_width; }

  /*! Set the width of the lines that represent aos marked edges. */
  void set_aos_marked_edge_line_width(Float width)
  { m_aos_marked_edge_line_width = width; }

  /*! Obtain the marked edge color. */
  const Vector3f& get_aos_marked_edge_color() const
  { return m_aos_marked_edge_color; }

  /*! Obtain the marked face color. */
  const Vector3f& get_aos_marked_face_color() const
  { return m_aos_marked_face_color; }

  // Aos marked face attributes:
  Float get_aos_marked_face_transparency() const
  { return m_aos_marked_face_transparency; }

  /*! Print statistics. */
  void print_stat() { Arrangement_on_surface_geo::print_stat(this); }

protected:
  /*! A function object that renders the vertices with color. */
  class Sphere_marked_colored_vertices_renderer :
    public Arrangement_renderer::Renderer
  {
  public:
    /*! The arrangement geometry. */
    Arrangement_on_sphere_marked_geo& m_geo;

  public:
    typedef Arrangement_on_sphere_marked_geo               Geometry;

    /*! Constructor. */
    Sphere_marked_colored_vertices_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the edges. */
    virtual void operator()(Draw_action* action);

  private:
    // Assignment operator cannot be generated.
    Sphere_marked_colored_vertices_renderer&
    operator=(const Sphere_marked_colored_vertices_renderer&);

    // In C++11, the following is supported:
    // Sphere_marked_colored_vertices_renderer&
    //   operator=(const Sphere_marked_colored_vertices_renderer&) = delete;
  };

  /*! A function object that renders the isolated vertices with color. */
  class Sphere_marked_colored_isolated_vertices_renderer :
    public Arrangement_renderer::Renderer
  {
  public:
    /*! The arrangement geometry. */
    Arrangement_on_sphere_marked_geo& m_geo;

  public:
    typedef Arrangement_on_sphere_marked_geo               Geometry;

    /*! Constructor. */
    Sphere_marked_colored_isolated_vertices_renderer(Geometry& geo) :
      m_geo(geo) {}

    /*! Render the edges. */
    virtual void operator()(Draw_action* action);

  private:
    // Assignment operator cannot be generated.
    Sphere_marked_colored_isolated_vertices_renderer&
    operator=(const Sphere_marked_colored_isolated_vertices_renderer&);

    // In C++11, the following is supported:
    // Sphere_marked_colored_isolated_vertices_renderer&
    //   operator=(const Sphere_marked_colored_isolated_vertices_renderer&) =
    //   delete;
  };

  /*! A function object that renders the edges with color. */
  class Sphere_marked_colored_edges_renderer :
    public Arrangement_renderer::Renderer
  {
  public:
    /*! The arrangement geometry. */
    Arrangement_on_sphere_marked_geo& m_geo;

  public:
    typedef Arrangement_on_sphere_marked_geo               Geometry;

    /*! Constructor. */
    Sphere_marked_colored_edges_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the edges. */
    virtual void operator()(Draw_action* action);

  private:
    // Assignment operator cannot be generated.
    Sphere_marked_colored_edges_renderer&
    operator=(const Sphere_marked_colored_edges_renderer&);

    // In C++11, the following is supported:
    // Sphere_marked_colored_edges_renderer&
    //   operator=(const Sphere_marked_colored_edges_renderer&) =
    //   delete;
  };

  /*! A function object that renders the marked primal vertex. */
  class Marked_face_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Arrangement_on_sphere_marked_geo    Geometry;

    /*! Constructor. */
    Marked_face_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the face. */
    virtual void operator()(Draw_action* action);

  protected:
    /*! The arrangement geometry. */
    Geometry& m_geo;

  private:
    // Assignment operator cannot be generated.
    Marked_face_renderer& operator=(const Marked_face_renderer&);

    // In C++11, the following is supported:
    // Marked_face_renderer& operator=(const Marked_face_renderer&) = delete;
  };

  typedef SGAL::Line_colored_edges_renderer
    <Sphere_marked_colored_edges_renderer>
    Sphere_marked_line_colored_edges_renderer;
  typedef SGAL::Point_colored_vertices_renderer
    <Sphere_marked_colored_vertices_renderer>
    Sphere_marked_point_colored_vertices_renderer;
  typedef SGAL::Ring_colored_vertices_renderer
    <Sphere_marked_colored_vertices_renderer>
    Sphere_marked_ring_colored_vertices_renderer;
  typedef SGAL::Point_colored_isolated_vertices_renderer
    <Sphere_marked_colored_isolated_vertices_renderer>
    Sphere_marked_point_colored_isolated_vertices_renderer;
  typedef SGAL::Ring_colored_isolated_vertices_renderer
    <Sphere_marked_colored_isolated_vertices_renderer>
    Sphere_marked_ring_colored_isolated_vertices_renderer;

  typedef Vertices_renderer<Self>
    Sphere_marked_vertices_renderer;
  typedef Isolated_vertices_renderer<Self>
    Sphere_marked_isolated_vertices_renderer;
  typedef Edges_renderer<Self>
    Sphere_marked_edges_renderer;

  typedef Inflated_line_edges_renderer<Sphere_marked_edges_renderer>
    Sphere_marked_inflated_line_edges_renderer;
  typedef Inflated_strip_edges_renderer<Sphere_marked_edges_renderer>
    Sphere_marked_inflated_strip_edges_renderer;
  typedef Inflated_tube_edges_renderer<Sphere_marked_edges_renderer>
    Sphere_marked_inflated_tube_edges_renderer;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Indicates whether the aos data structure is owned, i.e., explicitly
   * allocated. If the aos data structure is owned, it should be destructed
   * when this geometry node is destructed.
   */
  Boolean m_owned_aos;

  /*! The arrangement of great-circle arcs on a sphere. */
  Aos_marked* m_aos;

  typedef std::vector<unsigned int>                         Uint_vector;

  /*! The indices of the marked vertices. The index is the position in
   * the arrangement.
   */
  Uint_vector m_marked_vertices_indices;

  /*! The indices of the marked edges. The index is the position in
   * the arrangement.
   */
  Uint_vector m_marked_edges_indices;

  /*! The indices of the marked faces. The index is the position in
   * the arrangement.
   */
  Uint_vector m_marked_faces_indices;

  typedef std::vector<Shared_arrangement_on_sphere_marked_geo> Aos_geo_vector;
  typedef Aos_geo_vector::iterator                             Aos_geo_iter;
  typedef Aos_geo_vector::difference_type                      Aos_geo_diff;

  /*! A container of geometry nodes that represent arrangements of
   * great-circle arcs on a sphere.
   */
  Aos_geo_vector m_aoses;

  /*! An overlay-traits class for computing the overlay. */
  Arrangement_marked_overlay_traits<Aos_marked> m_overlay_traits;

  /*! The marked vertex shape style. */
  Vertex_style m_aos_marked_vertex_style;

  /*! The radius of the disc or ball that represents a marked vertex. */
  Float m_aos_marked_vertex_radius;

  /*! The size of the point that represents a marked vertex. */
  Float m_aos_marked_vertex_point_size;

  /*! The color of the marked vertices. */
  Vector3f m_aos_marked_vertex_color;

  // Marked edges:
  /*! Indicates whether the rendering of marked edges is enabled or not. */
  Boolean m_aos_marked_edge_enabled;

  /*! The marked edge style. */
  Edge_style m_aos_marked_edge_style;

  /*! The marked edge rendering type. */
  Uint m_aos_marked_edge_count;

  /*! Determines whether marked edges are rendered directed or not. */
  Boolean m_aos_marked_edge_directed;

  /*! The radius of the tube that represents a marked edge. */
  Float m_aos_marked_edge_radius;

  /*! The width of the line that represents a marked edge. */
  Float m_aos_marked_edge_line_width;

  /*! The color of the marked edges. */
  Vector3f m_aos_marked_edge_color;

  /*! The color of the marked face. */
  Vector3f m_aos_marked_face_color;

  /*! The transparency of the aos marked face. */
  Float m_aos_marked_face_transparency;

  /*! The marked primal vertex renderer. */
  Arrangement_renderer::Renderer* m_marked_face_renderer;

private:
  typedef Arrangement_on_sphere_marked_geo                      Self;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! Default values. */
  static const Vertex_style s_def_aos_marked_vertex_style;
  static const Float s_def_aos_marked_vertex_radius;
  static const Float s_def_aos_marked_vertex_point_size;
  static const Vector3f s_def_aos_marked_vertex_color;

  static const Boolean s_def_aos_marked_edge_enabled;
  static const Edge_style s_def_aos_marked_edge_style;
  static const Uint s_def_aos_marked_edge_count;
  static const Boolean s_def_aos_marked_edge_directed;
  static const Float s_def_aos_marked_edge_radius;
  static const Float s_def_aos_marked_edge_line_width;
  static const Vector3f s_def_aos_marked_edge_color;

  static const Vector3f s_def_aos_marked_face_color;
  static const Float s_def_aos_marked_face_transparency;

  static const Boolean s_def_draw_aos_marked_vertex;
  static const Boolean s_def_draw_aos_marked_edge;
  static const Boolean s_def_draw_aos_marked_face;

  /*! Draw an arrangement on sphere marked vertex.
   * \param action
   * \param center the point center.
   */
  void draw_aos_marked_vertex(Draw_action* action, Vector3f& center);

  /*! Draw an arrangement on sphere marked edge.
   * \param action
   * \param source the edge source point
   * \param target the edge target point
   * \param normal the normal to the plane containing the edge.
   */
  void draw_aos_marked_edge(Draw_action* action,
                            Vector3f& source, Vector3f& target,
                            Vector3f& normal);

  /*! Draw an arrangement on sphere marked face.
   * \param action
   */
  void draw_aos_marked_face(Draw_action* action);

  /*! Draw the arrangement vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action)
  { my_draw_aos_vertices(m_aos, action); }

  /*! Draw the arrangement isolated vertices.
   * \param action
   */
  virtual void draw_aos_isolated_vertices(Draw_action* action)
  { my_draw_aos_isolated_vertices(m_aos, action); }

  /*! Draw the arrangement edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action)
  { my_draw_aos_edges(m_aos, action); }

  /*! Clean the renderer. */
  virtual void clean_renderer();

  /*! Create the renderers. */
  void create_renderers();

  /*! Detsroy the renderers. */
  void destroy_renderers();
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

/* \brief constructs the prototype. */
inline Arrangement_on_sphere_marked_geo*
Arrangement_on_sphere_marked_geo::prototype()
{ return new Arrangement_on_sphere_marked_geo(true); }

/*! \brief clones. */
inline Container* Arrangement_on_sphere_marked_geo::clone()
{ return new Arrangement_on_sphere_marked_geo(); }

SGAL_END_NAMESPACE

#endif
