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
// $Id: Spherical_gaussian_map_base_geo.hpp 14223 2012-11-29 22:33:55Z efif $
// $Revision: 14223 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SPHERICAL_GAUSSIAN_MAP_BASE_GEO_HPP
#define SGAL_SPHERICAL_GAUSSIAN_MAP_BASE_GEO_HPP

/*! \file
 */

#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include <CGAL/Cartesian.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Sphere.hpp"

#include "SCGAL/Min_sphere.hpp"
#include "SCGAL/Arrangement_renderer.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"
#include "SCGAL/Arrangement_renderers.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Container_proto;
class Cull_context;
class Isect_action;
class Draw_action;
class Scene_graph;
class Cylinder;

class Spherical_gaussian_map_base_geo : public Mesh_set {
public:
  /*! Fields */
  enum {
    FIRST = Mesh_set::LAST - 1,
    DRAW_AOS,
    DRAW_AOS_OPAQUE,
    DRAW_AOS_HALOED,
    DRAW_AOS_SPHERE,
    AOS_SPHERE_COLOR,
    AOS_EDGE_LINE_WIDTH,
    AOS_EDGE_COLOR1,
    AOS_EDGE_COLOR2,
    //
    AOS_BOUNDARY_VERTEX_STYLE,
    AOS_BOUNDARY_VERTEX_STYLE_ID,
    AOS_BOUNDARY_VERTEX_RADIUS,
    AOS_BOUNDARY_VERTEX_POINT_SIZE,
    AOS_BOUNDARY_VERTEX_COLOR,
    //
    TRANSLATED,
    ROTATED,
    TRUE_DRAW_PRIMAL,
    TRUE_DRAW_AOS,
    EXPORT,
    LAST
  };

  typedef Arrangement_renderer::Vertex_shape          Vertex_shape;
  typedef Vertex_shape::Style                         Vertex_style;
  
  typedef Arrangement_renderer::Edge_shape            Edge_shape;
  typedef Edge_shape::Style                           Edge_style;

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default values. */
  static const Boolean s_def_draw_aos;

  static const Boolean s_def_draw_aos_opaque;
  static const Boolean s_def_draw_aos_haloed;

  static const Boolean s_def_draw_aos_surface;
  static const Vector3f s_def_aos_surface_color;

  static const Float s_def_aos_delta_angle;

  static const Vertex_style s_def_aos_vertex_style;
  static const Float s_def_aos_vertex_radius;
  static const Float s_def_aos_vertex_point_size;
  static const Vector3f s_def_aos_vertex_color;

  static const Vertex_style s_def_aos_boundary_vertex_style;
  static const Float s_def_aos_boundary_vertex_radius;
  static const Float s_def_aos_boundary_vertex_point_size;
  static const Vector3f s_def_aos_boundary_vertex_color;

  static const Boolean s_def_aos_edge_enabled;
  static const Edge_style s_def_aos_edge_style;
  static const Int s_def_aos_edge_count;
  static const Boolean s_def_aos_edge_directed;
  static const Float s_def_aos_edge_radius;
  static const Float s_def_aos_edge_line_width;
  static const Vector3f s_def_aos_edge_color;

protected:
  /*! Extracts the approximate point from an aos face. */
  template <typename Sgm>
  struct Convert_approximate_sphere {
    typedef typename Sgm::Face                          Sgm_face;
    
    /*! */
    Approximate_sphere_3 operator()(const Sgm_face& face) const
    {
      SGAL_assertion(!face.is_unbounded());
      return to_approximate_sphere(face.point());
    }
  };

  /*! Extracts the approximate point from an aos face. */
  template <typename Sgm>
  struct Extract_aos_vector3f_point {
    typedef typename Sgm::Face                          Sgm_face;

    /*! */
    Vector3f operator()(const Sgm_face& face) const
    {
      SGAL_assertion(!face.is_unbounded());
      return to_vector3f(face.point());
    }
  };
  
  /*! Transform the coordinates of the SGM into spheres.
   * \param sgm (in) the spherical Gaussian map.
   * \param spheres (out) the transformed coordinates.
   */
  template <typename Sgm>
  void transform_primal_coords(Sgm* sgm, Approximate_sphere_vector& spheres)
  {
    spheres.resize(sgm->number_of_faces());
    std::transform(sgm->faces_begin(), sgm->faces_end(), &spheres[0],
                   Convert_approximate_sphere<Sgm>());
  }

  /*! Compute the planes and the normals of the aos facets and store them at
   * the vertices of the planar map
   */
  template <typename Sgm>
  void update_facets(Sgm* sgm)
  {
    typename Sgm::Vertex_iterator vit;
    for (vit = sgm->vertices_begin(); vit != sgm->vertices_end(); ++vit) {
      // Vertices with boundary conditions may have degree 2. Skip them:
      if (vit->degree() < 3) continue;
      Vector3f normal = to_vector3f(vit->point());
      normal.normalize();
      vit->set_rendered_normal(normal);
    }
  }

  /*! An adapter from a container of Spherical gaussian map geometry nodes to
   * the a container of arrangements they hold.
   */
  template <typename Iterator, typename Sgm>
  class Sgm_iterator {    
  private:
    Iterator m_it;
    
  public:
    typedef typename Iterator::iterator_category iterator_category;
    typedef Sgm*                                 value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef Sgm**                                pointer;
    typedef Sgm*&                                reference;
    
  public:
    // Default constructor.
    Sgm_iterator () : m_it() {}

    // Constructor.
    Sgm_iterator (Iterator it) : m_it(it) {}

    // operator*
    const value_type operator*() const
    {
      Iterator& base = m_it;
      return (*base)->get_sgm();
    }
    
    value_type operator*()
    {
      Iterator& base = m_it;
      return (*base)->get_sgm();
    }

    /*! Operator pre ++ */
    Sgm_iterator& operator++()
    {
      ++m_it;
      return *this;
    }
    
    /*! Operator post ++ */
    Sgm_iterator operator++(int)
    {
      Sgm_iterator tmp = *this;
      ++*this;
      return tmp;
    }
    
    // operator->
    pointer operator->()
    {
      Iterator& base = m_it;
      return &((*base)->get_sgm());
    }
    
    const pointer operator->() const
    {
      Iterator& base = m_it;
      return &((*base)->get_sgm());
    }
  };

  typedef Spherical_gaussian_map_base_geo       Self;

  typedef SGAL::Sphere_renderer                 Surface_renderer;
  typedef SGAL::Colored_sphere_renderer         Colored_surface_renderer;
  typedef SGAL::Stencil_sphere_renderer         Stencil_surface_renderer;

  typedef SGAL::Vertices_renderer<Self>         Vertices_renderer;
  typedef SGAL::Edges_renderer<Self>            Edges_renderer;
  typedef SGAL::Colored_vertices_renderer<Vertices_renderer>
                                                Colored_vertices_renderer;
  typedef SGAL::Colored_edges_renderer<Edges_renderer>
                                                Colored_edges_renderer;
  
  typedef SGAL::Inflated_line_edges_renderer<Self::Edges_renderer>
                                                Inflated_line_edges_renderer;
  typedef SGAL::Inflated_strip_edges_renderer<Self::Edges_renderer>
                                                Inflated_strip_edges_renderer;
  typedef SGAL::Inflated_tube_edges_renderer<Self::Edges_renderer>
                                                Inflated_tube_edges_renderer;

  /*! The shape of an aos vertex. */
  Sphere m_aos_sphere;

  /*! Indicates that the coordinates were tranlated. */
  Boolean m_translated;

  /*! Indicates that the coordinates were rotated. */
  Boolean m_rotated;
  
  /*! When trigerred the CGM is exported. */
  Boolean m_export;

  /*! Indicates wether to draw the polyhedron or its Gaussian map. */
  Boolean m_draw_aos;

  /*! The opposite of the m_draw_aos flag. */
  Boolean m_draw_primal;

  /*! Indicates whether to draw the aos sphere opaque. */
  Boolean m_draw_aos_opaque;

  /*! Indicates whether to draw the aos edges haloed. */
  Boolean m_draw_aos_haloed;

  /*! Indicates whether to draw the aos sphere. */
  Boolean m_draw_aos_surface;

  /*! The color of the aos sphere (aos faces). */
  Vector3f m_aos_surface_color;

  // Vertex attributes:
  
  /*! The vertex shape style. */
  Vertex_style m_aos_vertex_style;

  /*! The radius of the geometry that represents a vertex. */
  Float m_aos_vertex_radius;

  /*! The size of the point that represents a vertex. */
  Float m_aos_vertex_point_size;
  
  /*! The color of the vertices. */
  Vector3f m_aos_vertex_color;

  // Aos boundary vertex attributes:
  
  /*! The boundary vertex shape style. */
  Vertex_style m_aos_boundary_vertex_style;

  /*! The radius of the disc or ball that represents an boundary vertex. */
  Float m_aos_boundary_vertex_radius;

  /*! The size of the point that represents an boundary vertex. */
  Float m_aos_boundary_vertex_point_size;
  
  /*! The color of the boundary vertices. */
  Vector3f m_aos_boundary_vertex_color;    
  
  // Edge attributes:
  
  /*! Indicates whether the rendering of edges is enabled or not. */
  Boolean m_aos_edge_enabled;

  /*! The edge rendering style. */
  Edge_style m_aos_edge_style;

  /*! The edge rendering type. */
  Int m_aos_edge_count;

  /*! Indicates whether edges are rendered directed or not. */
  Boolean m_aos_edge_directed;
  
  /*! The radius of the geometry that represents an edge. */
  Float m_aos_edge_radius;
  
  /*! The width of the lines that represent aos edges. */
  Float m_aos_edge_line_width;

  /*! The color of the aos edges. */
  Vector3f m_aos_edge_colors[2];

  /*! The angle of a single line strip of a spherical arc. */
  Float m_aos_delta_angle;

  /*! Indicates whether the renderer must be cleaned. */
  Boolean m_renderer_dirty;

  /*! Indicates whether the facets must be cleaned. */
  Boolean m_facets_dirty;
  
  /*! The renderer of the arrangement data structure. */
  Arrangement_renderer m_renderer;

  /*! The surface renderer */
  Arrangement_renderer::Renderer* m_surface_renderer;

  /*! The surface renderer */
  Colored_surface_renderer* m_colored_surface_renderer;

  /*! The surface renderer */
  Arrangement_renderer::Renderer* m_stencil_surface_renderer;
  
  /*! Create the renderers */
  void create_renderers();

  /*! Destroy the renderers */
  void destroy_renderers();
  
  /*! Transform the coordinates of the SGM into spheres.
   * \param spheres (out) the transformed coordinates.
   */
  virtual void transform_coords(Approximate_sphere_vector& spheres) {}

  /*! Draw the primal representation of the polyhedron. */
  virtual void draw_primal(Draw_action* action) {}

  /*! Draw the Gausian representation of the polyhedron opaque. */
  virtual void draw_aos_opaque(Draw_action* action) {}
  
  /*! Draw an arrangement on sphere vertex.
   * param action
   * param center the vertex center.
   */
  void draw_aos_vertex(Draw_action* action, Vector3f& center);

  /*! Draw an arrangement on surface boundary_vertex.
   * \param action
   * \param center the vertex center.
   */
  void draw_aos_boundary_vertex(Draw_action* action, Vector3f& center);

  /*! Draw an arrangement on sphere edge.
   * \param action
   * \param source the edge source point.
   * \param target the edge target point.
   * \param normal the normal to the plane containing the edge.
   */
  void draw_aos_edge(Draw_action* action, Vector3f& source, Vector3f& target,
                     Vector3f& normal);
    
  virtual void isect_primary() {}

  /*! Export the planar maps associated with the faces of the unit sphere.
   */
  void output(Field_info* field_info)
  { SGAL_assertion_msg(0, "Not implemented yet!"); }

  /*! \brief imports  the planar maps associated with the faces of the unit
   * sphere.
   */
  void input() { SGAL_assertion_msg(0, "Not implemented yet!"); }
  
public:
  /*! Constructor. */
  Spherical_gaussian_map_base_geo(Boolean proto = false);

  /*! Copy constructor. */
  Spherical_gaussian_map_base_geo(const Spherical_gaussian_map_base_geo& sgm);
  
  /*! Destructor. */
  virtual ~Spherical_gaussian_map_base_geo();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! Calculate the bounding sphere.
   * \return a Boolean flag that indicates whether the sphere bound changed.
   */
  virtual Boolean clean_sphere_bound();

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Draw the internal representation. */
  virtual void draw_geometry(Draw_action* action);
    
  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();
  
  /*! Clean the renderer. */
  virtual void clean_renderer();

  /*! Process change of content. */
  virtual void field_changed(Field_info* field_info);

  /*! Raise the flag that indicates that the sphere bound changed. */
  void draw_changed(Field_info* field_info = NULL);
  
  /*! Draw the arrangement on sphere vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action) {}

  /*! Draw the arrangement on sphere edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action) {}

  /*! Reverse the coordinate indices. */
  void set_reverse_coord_indices(const SGAL::Array<Uint>& indices);
  
  /*! Obtain the aos surface color. */
  const Vector3f& get_aos_surface_color(void) const;

  /*! Set the aos surface color. */
  void set_aos_surface_color(const Vector3f& color);

  // Vertex attributes:
  
  /*! Obtain the vertex shape style. */
  Vertex_style get_aos_vertex_style() const { return m_aos_vertex_style; }

  /*! Set the vertex shape style. */
  void set_aos_vertex_style(Vertex_style style) { m_aos_vertex_style = style; }
  
  /*! Obtain the vertex shape style */
  Float get_aos_vertex_point_size() const { return m_aos_vertex_point_size; }

  /*! Obtain the vertex radius. */
  Float get_aos_vertex_radius() const { return m_aos_vertex_radius; }
  
  /*! Obtain the aos vertex color. */
  const Vector3f& get_aos_vertex_color() const { return m_aos_vertex_color; }

  /*! Set the vertex color. */
  void set_aos_vertex_color(const Vector3f& color);

  // Aos boundary vertex attributes:
  
  /*! Obtain the boundary vertex shape style. */
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
  
  // Edge attributes:
  
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
  
  /*! Obtain the edge shape type. */
  Int get_aos_edge_count() const { return m_aos_edge_count; }

  /*! Set the edge shape count. */
  void set_aos_edge_count(Int count) { m_aos_edge_count = count; }
    
  /*! Determine whether edges are rendered directed. */
  Boolean get_aos_edge_directed() const { return m_aos_edge_directed; }

  /*! Set the flag that determines whether edges are rendered directed. */
  void set_aos_edge_directed(Boolean dir) { m_aos_edge_directed = dir; }

  /*! Obtaint the radius of the tubes that represent aos edges. */
  Float get_aos_edge_radius() const { return m_aos_edge_radius; }

  /*! Set the radius of the tubes that represent aos edges. */
  void set_aos_edge_radius(Float radius) { m_aos_edge_radius = radius; }

  /*! Set the width of the lines that represent aos edges. */
  void set_aos_edge_line_width(Float width);

  /*! Obtaint the width of the lines that represent aos edges. */
  Float get_aos_edge_line_width() const;
  
  /*! Set the line color. */
  void set_aos_edge_color(const Vector3f& color, unsigned int id = 0);

  /*! Obtain the line color. */
  const Vector3f& get_aos_edge_color(unsigned int id = 0);

  /*! Obtain the angle of a trangle fan. */
  Float get_aos_delta_angle() const { return m_aos_delta_angle; }

  /*! Obtain the flag that indicates whether to draw the embedding surface.
   */
  Boolean get_draw_aos_surface() const { return m_draw_aos_surface; }

  /*! Set the flag that indicates whether to draw the background. */
  void set_draw_aos_surface(Boolean draw_bg);

  /*! Set the flag that indicates whether to draw the primal or aos. */
  void set_draw_aos(Boolean flag) { m_draw_aos = flag; }

  /*! Set the flag that indicates whether to draw haloed lines. */
  void set_draw_aos_haloed(Boolean flag) { m_draw_aos_haloed = flag; }

  /*! Obtain the flag that indicates whether to draw haloed lines. */
  Boolean get_draw_aos_haloed() const { return m_draw_aos_haloed; }
};

/*! brief sets the curve color. */
inline void Spherical_gaussian_map_base_geo::
set_aos_edge_color(const Vector3f& color, Uint id)
{ m_aos_edge_colors[id] = color; }

/*! \brief obtains the curve color. */
inline const Vector3f& Spherical_gaussian_map_base_geo::
get_aos_edge_color(Uint id)
{ return m_aos_edge_colors[id]; }

/*! \brief sets the aos sphere color. */
inline void Spherical_gaussian_map_base_geo::
set_aos_surface_color(const Vector3f& color)
{
  m_aos_surface_color = color;
  if (m_colored_surface_renderer) m_colored_surface_renderer->set_color(color);
}

/*! \brief sets the aos vertex color. */
inline void Spherical_gaussian_map_base_geo::
set_aos_vertex_color(const Vector3f& color)
{ m_aos_vertex_color = color; }

/*! \brief sets the flag that indicates whether to draw the surface sphere. */
inline void Spherical_gaussian_map_base_geo::
set_draw_aos_surface(Boolean draw_bg)
{ m_draw_aos_surface = draw_bg; }

/*! \brief sets the width of the lines. */
inline void
Spherical_gaussian_map_base_geo::set_aos_edge_line_width(Float width)
{ m_aos_edge_line_width = width; }

/*! \brief obtains the width of the lines. */
inline Float Spherical_gaussian_map_base_geo::get_aos_edge_line_width() const
{ return m_aos_edge_line_width; }

/*! \brief obtains the aos surface color. */
inline const Vector3f&
Spherical_gaussian_map_base_geo::get_aos_surface_color(void) const
{ return m_aos_surface_color; }

SGAL_END_NAMESPACE

#endif
