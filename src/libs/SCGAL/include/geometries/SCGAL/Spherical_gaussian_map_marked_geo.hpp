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
// $Id: Spherical_gaussian_map_marked_geo.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SPHERICAL_GAUSSIAN_MAP_MARKED_GEO_HPP
#define SGAL_SPHERICAL_GAUSSIAN_MAP_MARKED_GEO_HPP

/*! \file
 */

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

// #include <CGAL/IO/Arr_iostream.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_arr_dcel.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_polyhedron_3.h>

#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"

#include "SCGAL/Spherical_gaussian_map_marked.hpp"
#include "SCGAL/Spherical_gaussian_map_base_geo.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
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

class Spherical_gaussian_map_marked_geo :
  public Spherical_gaussian_map_base_geo
{
public:
  /*! Fields */
  enum {
    FIRST = Spherical_gaussian_map_base_geo::LAST - 1,
    GEOMETRIES,
    INCREASE_VERTEX_INDEX,
    INCREASE_EDGE_INDEX,
    INCREASE_FACET_INDEX,
    LAST
  };

private:
  typedef Spherical_gaussian_map_marked           Sgm;
  typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Sgm, Exact_kernel>
                                                  Polyhedron;

  typedef Sgm::Geometry_traits_2                  Geometry_traits_2;
  typedef Geometry_traits_2::Point_2              Point_2;
  typedef Geometry_traits_2::Curve_2              Curve_2;
  typedef Geometry_traits_2::X_monotone_curve_2   X_monotone_curve_2;

  typedef Sgm::Vertex                             Sgm_vertex;
  typedef Sgm::Halfedge                           Sgm_halfedge;
  typedef Sgm::Face                               Sgm_face;

  typedef Sgm::Vertex_handle                      Sgm_vertex_handle;
  typedef Sgm::Halfedge_handle                    Sgm_halfedge_handle;
  typedef Sgm::Face_handle                        Sgm_face_handle;

  typedef Sgm::Vertex_const_handle                Sgm_vertex_const_handle;
  typedef Sgm::Halfedge_const_handle              Sgm_halfedge_const_handle;
  typedef Sgm::Face_const_handle                  Sgm_face_const_handle;
  
  typedef Sgm::Vertex_iterator                    Sgm_vertex_iterator;
  typedef Sgm::Halfedge_iterator                  Sgm_halfedge_iterator;
  typedef Sgm::Edge_iterator                      Sgm_edge_iterator;
  typedef Sgm::Face_iterator                      Sgm_face_iterator;

  typedef Sgm::Vertex_const_iterator              Sgm_vertex_const_iterator;
  typedef Sgm::Edge_const_iterator                Sgm_edge_const_iterator;
  typedef Sgm::Halfedge_const_iterator            Sgm_halfedge_const_iterator;
  typedef Sgm::Face_const_iterator                Sgm_face_const_iterator;

  typedef Sgm::Halfedge_around_vertex_circulator
                                Sgm_halfedge_around_vertex_circulator;
  typedef Sgm::Halfedge_around_vertex_const_circulator
                                Sgm_halfedge_around_vertex_const_circulator;
  
  /*! Notification for computing the sgm */
  class Sgm_geo_initializer_visitor {
  public:
    typedef Polyhedron::Vertex_const_handle
                                        Polyhedron_vertex_const_handle;
    typedef Polyhedron::Halfedge_const_handle
                                        Polyhedron_halfedge_const_handle;
    typedef Polyhedron::Facet_const_handle
                                        Polyhedron_facet_const_handle;
    
    /*! Destructor */
    virtual ~Sgm_geo_initializer_visitor() {}
    
    /*! Pass information from the polyhedron vertex to its dual - an aos face */
    virtual void update_dual_vertex(Polyhedron_vertex_const_handle src,
                                    Sgm_face_handle trg)
    {
      trg->set_marked(src->marked());
    }

    /*! Pass information from an aos face to another aos face that are duals
     * of the same polyhedron vertex
     */
    virtual void update_dual_vertex(Sgm_face_const_handle src,
                                    Sgm_face_handle trg)
    {
      trg->set_marked(src->marked());
    }

    /*! Pass information from a polyhedron facet to its dual - an aos vertex */
    virtual void update_dual_face(Polyhedron_facet_const_handle src,
                                  Sgm_vertex_handle trg)
    {
      trg->set_marked(src->marked());
    }

    /*! Pass information from a polyhedron halfedge to its dual - an aos
     * halfedge
     */
    virtual void update_dual_halfedge(Polyhedron_halfedge_const_handle src,
                                      Sgm_halfedge_handle trg)
    {
      trg->set_marked(src->marked());
    }
  };
   
  typedef CGAL::
  Arr_polyhedral_sgm_initializer<Sgm, Polyhedron, Sgm_geo_initializer_visitor>
                                                  Sgm_initializer;
  
protected:
  typedef Spherical_gaussian_map_marked_geo            Self;

  /*! A function object that renders the edges with color */
  class Colored_edges_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Spherical_gaussian_map_marked_geo          Geometry;

    /*! Constructor */
    Colored_edges_renderer(Geometry & geo) : m_geo(geo) {}
    
    /*! Render the face */
    virtual void operator()(Draw_action * action);

  protected:
    /*! The arrangement geometry */
    Geometry & m_geo;
  };

  /*! A function object that renders the marked vertices with color */
  class Marked_vertices_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Spherical_gaussian_map_marked_geo          Geometry;

    /*! Constructor */
    Marked_vertices_renderer(Geometry & geo) : m_geo(geo) {}
    
    /*! Render the vertex */
    virtual void operator()(Draw_action * action);

  protected:
    /*! The arrangement geometry */
    Geometry & m_geo;
  };
  
  /*! A function object that renders the marked vertices with color */
  class Colored_marked_vertices_renderer : public Marked_vertices_renderer {
  public:
    typedef Spherical_gaussian_map_marked_geo          Geometry;

    /*! Constructor */
    Colored_marked_vertices_renderer(Geometry & geo) :
      Marked_vertices_renderer(geo) {}
    
    /*! Render the vertex */
    virtual void operator()(Draw_action * action);
  };
  
  /*! A function object that renders the marked edges */
  class Marked_edges_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Spherical_gaussian_map_marked_geo          Geometry;

    /*! Constructor */
    Marked_edges_renderer(Geometry & geo) : m_geo(geo) {}
    
    /*! Render the vertex */
    virtual void operator()(Draw_action * action);

  protected:
    /*! The arrangement geometry */
    Geometry & m_geo;
  };
  
  /*! A function object that renders the marked edges with color */
  class Colored_marked_edges_renderer : public Marked_edges_renderer {
  public:
    typedef Spherical_gaussian_map_marked_geo          Geometry;

    /*! Constructor */
    Colored_marked_edges_renderer(Geometry & geo) :
      Marked_edges_renderer(geo) {}
    
    /*! Render the vertex */
    virtual void operator()(Draw_action * action);
  };
  
  /*! A function object that renders the marked primal vertex */
  class Marked_face_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Spherical_gaussian_map_marked_geo          Geometry;

    /*! Constructor */
    Marked_face_renderer(Geometry & geo) : m_geo(geo) {}
    
    /*! Render the face */
    virtual void operator()(Draw_action * action);

  protected:
    /*! The arrangement geometry */
    Geometry & m_geo;
  };
  
  /*! A functor with an operator that sets the "marked" flag */
  class Face_set_marked_op {
  public:
    void operator()(Sgm_face_handle fh) { fh->set_marked(SGAL_TRUE); }
  };

  // List of pointers to Spherical_gaussian_map_marked_geo objects */
  typedef std::list<Spherical_gaussian_map_marked_geo *>        Sgm_node_list;
  typedef Sgm_node_list::iterator                               Sgm_node_iter;

  /*! The minkowski sum operands */
  Sgm_node_list m_sgm_nodes;

  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }
  
  /*! Create the renderers */
  void create_renderers();

  /*! Destroy the renderers */
  void destroy_renderers();
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! Default values */
  static const Vertex_style s_def_aos_marked_vertex_style;
  static const Float s_def_aos_marked_vertex_radius;
  static const Float s_def_aos_marked_vertex_point_size;

  static const Boolean s_def_aos_marked_edge_enabled;
  static const Edge_style s_def_aos_marked_edge_style;
  static const Int s_def_aos_marked_edge_count;
  static const Boolean s_def_aos_marked_edge_directed;
  static const Float s_def_aos_marked_edge_radius;
  static const Float s_def_aos_marked_edge_line_width;

  static const Float s_def_aos_marked_face_transparency;
  
  static const Boolean s_def_draw_marked_vertex;
  static const Boolean s_def_draw_marked_edge;
  static const Boolean s_def_draw_marked_facet;

  static const Vector3f s_def_marked_vertex_color;
  static const Vector3f s_def_marked_edge_color;
  static const Vector3f s_def_marked_facet_color;

  static const Float s_def_marked_vertex_radius;
  static const Float s_def_marked_edge_radius;

  /*! Indicates whther this node owns (allocated) the sgm data structure */
  Boolean m_own_sgm;

  /*! The cubical Gaussian map representation */
  Sgm * m_sgm;

  /*! Indicates whether to compute the minkowski sum */
  Boolean m_minkowski_sum;

  /*! For benchmarking */
  float m_time;

  // Marked vertex attributes:
  
  /*! The vertex shape style */
  Vertex_style m_aos_marked_vertex_style;

  /*! The radius of the geometry that represents a vertex */
  Float m_aos_marked_vertex_radius;

  /*! The size of the point that represents a vertex */
  Float m_aos_marked_vertex_point_size;

  // Marked edge attributes:

  /*! Indicates whether the rendering of marked edges is enabled or not */
  Boolean m_aos_marked_edge_enabled;

  /*! The marked edge style */
  Edge_style m_aos_marked_edge_style;
  
  /*! The marked edge rendering type */
  Int m_aos_marked_edge_count;

  /*! Determines whether marked edges are rendered directed or not */
  Boolean m_aos_marked_edge_directed;
  
  /*! The radius of the geometry that represents an aos marked edge */
  Float m_aos_marked_edge_radius;
  
  /*! The width of the lines that represent aos marked edges */
  Float m_aos_marked_edge_line_width;

  /*! The transparency of the aos marked face */
  Float m_aos_marked_face_transparency;
  
  // Feature attributes:
  
  // Marked vertex: 

  /*! Indicates whether to draw the marked vertex */
  Boolean m_draw_marked_vertex;

  /*! The index of the marked vertex */
  Uint m_marked_vertex_index;

  /*! The radius of the marked primal vertex */
  Float m_marked_vertex_radius;
  
  /*! The color of the marked vertex */
  Vector3f m_marked_vertex_color;

  /*! When trigerred the vertex index is increased */
  Boolean m_increase_vertex_index;

  // Marked edge: 

  /*! Indicates whether to draw the marked halfedge */
  Boolean m_draw_marked_edge;
  
  /*! The index of the marked edge */
  Uint m_marked_edge_index;

  /*! The radius of the geometry that represents a marked edge */
  Float m_marked_edge_radius;

  /*! The color of the marked edge */
  Vector3f m_marked_edge_color;

  /*! When trigerred the edge index is increased */
  Boolean m_increase_edge_index;

  // Makrked facet:
  
  /*! Indicates whether to draw the marked face */
  Boolean m_draw_marked_facet;

  /*! The color of the marked facet */
  Vector3f m_marked_facet_color;

  /*! The index of the marked face */
  Uint m_marked_facet_index;
  
  /*! When trigerred the face index is increased */
  Boolean m_increase_facet_index;

  /*! The vertices renderer */
  Arrangement_renderer::Renderer * m_vertices_renderer;

  /*! The vertices renderer */
  Arrangement_renderer::Renderer * m_colored_vertices_renderer;

  /*! The edges renderer */
  Arrangement_renderer::Renderer * m_edges_renderer;

  /*! The colored edges renderer */
  Arrangement_renderer::Renderer * m_colored_edges_renderer;

  /*! The inflated line edges renderer */
  Arrangement_renderer::Renderer * m_inflated_line_edges_renderer;

  /*! The inflated strip edges renderer */
  Arrangement_renderer::Renderer * m_inflated_strip_edges_renderer;
  
  /*! The inflated tube edges renderer */
  Arrangement_renderer::Renderer * m_inflated_tube_edges_renderer;
   
  /*! The marked primal vertex renderer */
  Arrangement_renderer::Renderer * m_marked_face_renderer;
  
  /*! The marked vertices renderer */
  Arrangement_renderer::Renderer * m_marked_vertices_renderer;

  /*! The colored marked vertices renderer */
  Arrangement_renderer::Renderer * m_colored_marked_vertices_renderer;

  /*! The marked edges renderer */
  Arrangement_renderer::Renderer * m_marked_edges_renderer;

  /*! The colored marked edges renderer */
  Arrangement_renderer::Renderer * m_colored_marked_edges_renderer;

  /*! Transform the coordinates of the SGM into spheres
   * \param spheres (o) the transformed coordinates
   */
  virtual void transform_coords(Approximate_sphere_vector & spheres)
  { transform_primal_coords(m_sgm, spheres); }

  /*! Draw the primal representation of the polyhedron */
  virtual void draw_primal(Draw_action * action);

  /*! Draw the arrangement on sphere opaque
   * \param action
   */
  virtual void draw_aos_opaque(Draw_action * action);

  virtual void isect_primary();
  
  /*! Draw the primal marked vertex */
  void draw_primal_marked_vertex(Draw_action * action);

  /*! Draw the primal marked edge */
  void draw_primal_marked_edge(Draw_action * action);

  /*! Draw an arrangement on sphere marked vertex
   * param action
   * param center the vertex center
   */
  void draw_aos_marked_vertex(Draw_action * action, Vector3f & center);
  
  /*! Draw an arrangement on sphere marked edge
   * \param action
   * \param source the edge source point
   * \param target the edge target point
   */
  void draw_aos_marked_edge(Draw_action * action,
                            Vector3f & source, Vector3f & target);

  /*! Draw the marked face (primal vertex)
   * \param action
   */
  void draw_aos_marked_face(Draw_action * action);

public:
  /*! Constructor */
  Spherical_gaussian_map_marked_geo(Boolean proto = SGAL_FALSE);

  /*! Copy constructor */
  Spherical_gaussian_map_marked_geo(const Spherical_gaussian_map_marked_geo & gm);
  
  /*! Destructor */
  virtual ~Spherical_gaussian_map_marked_geo();

  /*! Construct the prototype */
  static Spherical_gaussian_map_marked_geo * prototype()
  { return new Spherical_gaussian_map_marked_geo(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone()
  { return new Spherical_gaussian_map_marked_geo(); }

  /*! */
  virtual void cull(Cull_context & cull_context);

  /*! */
  virtual void isect(Isect_action * action);

  /*! Set the attributes of this node
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element * elem);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! \brief deletes the prototype */
  virtual void delete_prototype();

  /*! \brief obtains the prototype */
  virtual Container_proto * get_prototype();

  /*! \brief cleans the representation */
  virtual void clean();

  /*! \brief clears the internal representation and auxiliary data structures
   */
  virtual void clear();
  
  /*! \brief returns true if the representation is empty */
  virtual Boolean is_empty() const { return m_sgm->is_empty(); }

  /*! Draw the arrangement on sphere vertices
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action * action);

  /*! Draw the arrangement on sphere edges
   * \param action
   */
  virtual void draw_aos_edges(Draw_action * action);
  
  /*! Clean the renderer */
  virtual void clean_renderer();

  // Marked vertex attributes:
  
  /*! Obtain the marked vertex shape style */
  Vertex_style get_aos_marked_vertex_style() const
  { return m_aos_marked_vertex_style; }

  /*! Obtain the marked vertex point size */
  Float get_aos_marked_vertex_point_size() const
  { return m_aos_marked_vertex_point_size; }

  /*! Obtain the marked vertex radius */
  Float get_aos_marked_vertex_radius() const
  { return m_aos_marked_vertex_radius; }

  // Aos marked edge attributes:
  
  /*! Enable marked edge rendering */
  void enable_aos_marked_edge() { m_aos_marked_edge_enabled = SGAL_TRUE; }

  /*! Disable marked edge rendering */
  void disable_aos_marked_edge() { m_aos_marked_edge_enabled = SGAL_FALSE; }

  /*! Determine whether marked edge rendering is enabled */
  Boolean is_aos_marked_edge_enabled() const
  { return m_aos_marked_edge_enabled; }

  /*! Obtain the marked edge style */
  Edge_style get_aos_marked_edge_style() const
  { return m_aos_marked_edge_style; }

  /*! Set the marked edge style */
  void set_aos_marked_edge_style(Edge_style style)
  { m_aos_marked_edge_style = style; }

  /*! Obtain the marked edge type */
  Int get_aos_marked_edge_count() const
  { return m_aos_marked_edge_count; }

  /*! Set the marked edge type */
  void set_aos_marked_edge_count(Int count)
  { m_aos_marked_edge_count = count; }

  /*! Determine whether marked edges are rendered directed */
  Boolean get_aos_marked_edge_directed() const
  { return m_aos_marked_edge_directed; }

  /*! Set the flag that determines whether marked edges are rendered directed */
  void set_aos_marked_edge_directed(Boolean dir)
  { m_aos_marked_edge_directed = dir; }

  /*! Obtaint the radius of the tubes that represent aos marked edges */
  Float get_aos_marked_edge_radius() const { return m_aos_marked_edge_radius; }

  /*! Set the radius of the tubes that represent aos marked edges */
  void set_aos_marked_edge_radius(Float radius)
  { m_aos_marked_edge_radius = radius; }

  /*! Set the width of the lines that represent aos marked edges */
  void set_aos_marked_edge_line_width(Float width)
  { m_aos_marked_edge_line_width = width; }

  /*! \brief obtaints the width of the lines that represent aos marked edges */
  Float get_aos_marked_edge_line_width() const
  { return m_aos_marked_edge_line_width; }
  
  // Aos marked face attributes:
  Float get_aos_marked_face_transparency() const
  { return m_aos_marked_face_transparency; }
  
  /*! Obtain the flag that indicates whether to draw the marked vertex */
  Boolean get_draw_marked_vertex() const { return m_draw_marked_vertex; }

  /*! Obtain the flag that indicates whether to draw the marked halfedge */
  Boolean get_draw_marked_edge() const { return m_draw_marked_edge; }
  
  /*! Obtain the flag that indicates whether to draw the marked face */
  Boolean get_draw_marked_facet() const { return m_draw_marked_facet; }

  
  /*! Obtain the index of the marked vertex */
  Uint marked_vertex_index() const { return m_marked_vertex_index; }

  /*! Obtain the index of the marked edge */
  Uint marked_edge_index() const { return m_marked_edge_index; }
  
  /*! Obtain the index of the marked facet */
  Uint marked_facet_index() const { return m_marked_facet_index; }
  
  /*! Set the index of the marked vertex */
  void set_marked_vertex_index(Uint index) { m_marked_vertex_index = index; }

  /*! Set the index of the marked edge */
  void set_marked_edge_index(Uint index) { m_marked_edge_index = index; }

  /*! Set the index of the marked facet */
  void set_marked_facet_index(Uint index) { m_marked_facet_index = index; }
  
  /*! Obtain the color of the marked vertex */
  const Vector3f get_marked_vertex_color() const;

  /*! Set the color of the marked vertex */
  void set_marked_vertex_color(const Vector3f & color);

  /*! Set the color of the marked edge */
  void set_marked_edge_color(const Vector3f & color);

  /*! Set the color of the marked facet */
  void set_marked_facet_color(const Vector3f & color);
  
  /*! \brief prints statistics */
  virtual void print_stat();

  /*! Set the source gausian maps of the minkowski sum */
  void insert_sgm(Spherical_gaussian_map_marked_geo * sgm);

  /*! Obrain a reference to the cubical Gaussian map */
  Sgm * get_sgm();

  /*! Set the Gaussian map */
  void set_sgm(Sgm * sgm);
  
  /*! Set the flag that indicates whether to compute the minkowski sum */
  void set_minkowski_sum(Boolean flag) { m_minkowski_sum = flag; }

  /*! Compute the equations of, or the normals to, the planes of the aos
   * facets and store them at the vertices of the planar map
   */
  void update_facets()
  { Spherical_gaussian_map_base_geo::update_facets(m_sgm); }

  /*! Increase the vertex index */
  void increase_vertex_index(Field_info * field_info = NULL);

  /*! Increase the edge index */
  void increase_edge_index(Field_info * field_info = NULL);

  /*! Increase the face index */
  void increase_facet_index(Field_info * field_info = NULL);
};

/*! \brief obtains the color of the marked vertex */
inline
const Vector3f Spherical_gaussian_map_marked_geo::
get_marked_vertex_color() const
{ return m_marked_vertex_color; }

/*! \brief sets the color of the marked vertex */
inline void
Spherical_gaussian_map_marked_geo::
set_marked_vertex_color(const Vector3f & color)
{ m_marked_vertex_color = color; }

/*! \brief sets the color of the marked edge */
inline void
Spherical_gaussian_map_marked_geo::
set_marked_edge_color(const Vector3f & color)
{ m_marked_edge_color = color; }

/*! \brief sets the color of the marked facet */
inline void
Spherical_gaussian_map_marked_geo::
set_marked_facet_color(const Vector3f & color)
{ m_marked_facet_color = color; }

SGAL_END_NAMESPACE

#endif
