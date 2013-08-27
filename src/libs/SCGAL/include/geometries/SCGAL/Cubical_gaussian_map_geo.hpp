// Copyright (c) 2004 Israel.
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
// $Id: $
// $Revision: 14223 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_CUBICAL_GAUSSIAN_MAP_GEO_HPP
#define SCGAL_CUBICAL_GAUSSIAN_MAP_GEO_HPP

/*! \file
 * Conventions
 * -----------
 * All public Arrangement types and functions have suffices "Arr_" and "arr_"
 * respectively.
 */

#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include <CGAL/Cartesian.h>
#include <CGAL/IO/Arr_iostream.h>
#include <CGAL/Polyhedral_cgm.h>
#include <CGAL/Polyhedral_cgm_arr_dcel.h>
#include <CGAL/Polyhedral_cgm_initializer_visitor.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Min_sphere.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_renderer.hpp"
#include "SCGAL/Arrangement_renderers.hpp"
#include "SCGAL/Arrangement_on_cube_renderers.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Container_proto;
class Cull_context;
class Isect_action;
class Draw_action;
class Scene_graph;
class Sphere;
class Cylinder;

class SGAL_SCGAL_DECL Cubical_gaussian_map_geo : public SGAL::Mesh_set {
public:
  typedef boost::shared_ptr<Cubical_gaussian_map_geo>
    Shared_cubical_gaussian_map_geo;

  /*! Fields */
  enum {
    FIRST = Mesh_set::LAST - 1,
    DRAW_DUAL,
    DRAW_DUAL_OPAQUE,
    DRAW_DUAL_HALOED,
    DRAW_DUAL_UNFOLDED,
    DRAW_DUAL_CUBE,
    DUAL_CUBE_COLOR,
    DUAL_EDGE_LINE_WIDTH,
    DUAL_EDGE_COLOR1,
    DUAL_EDGE_COLOR2,
    COORD,
    TRANSLATED,
    ROTATED,
    TRUE_DRAW_PRIMAL,
    TRUE_DRAW_DUAL,
    TRUE_DRAW_DUAL_UNFOLDED,
    EXPORT,
    INCREASE_VERTEX_INDEX,
    INCREASE_EDGE_INDEX,
    INCREASE_FACET_INDEX,
    GEOMETRIES,
    LAST
  };

  typedef Arrangement_renderer::Vertex_shape          Vertex_shape;
  typedef Vertex_shape::Style                         Vertex_style;

  typedef Arrangement_renderer::Edge_shape            Edge_shape;
  typedef Edge_shape::Style                           Edge_style;

  // Kernel, Number type, etc.
  typedef Exact_number_type                           Number_type;
  typedef Exact_kernel                                Kernel;
  typedef Kernel::Point_3                             Point_3;
  typedef Kernel::Vector_3                            Vector_3;
  typedef Kernel::Aff_transformation_3                Aff_transformation_3;

private:
  /*! Extend the planar-map vertex */
  template <typename Point_2>
  class My_arr_vertex : public CGAL::Polyhedral_cgm_arr_vertex<Point_2> {
  private:
    /*! The (approximate) normal of dual facet */
    SGAL::Vector3f m_normal;

    /*! Indicates whether it is a marked (dual) face */
    Boolean m_marked;

  public:
    /*! Constructor */
    My_arr_vertex() : m_marked(false) {}

    /*! Obtain the "marked" flag */
    Boolean marked() const { return m_marked; }

    /*! Set the "marked" flag */
    void set_marked(Boolean marked) { m_marked = marked; }

    /*! Obtain the normal of the dual facet */
    const SGAL::Vector3f& get_normal() const { return m_normal; }

    /*! Set the normal of the dual facet */
    void set_normal(const SGAL::Vector3f& normal) { m_normal = normal; }
  };

  /*! Extend the planar-map halfedge */
  template <class X_monotone_curve_2>
  class My_arr_halfedge :
    public CGAL::Polyhedral_cgm_arr_halfedge<X_monotone_curve_2> {
  private:
    /*! Indicates whether it is a marked (dual) face */
    Boolean m_marked;

  public:
    /*! Constructor */
    My_arr_halfedge() : m_marked(false) {}

    /*! Obtain the "marked" flag */
    Boolean marked() const { return m_marked; }

    /*! Set the "marked" flag */
    void set_marked(Boolean marked) { m_marked = marked; }
  };

  /*! Extend the planar-map face */
  class My_arr_face : public CGAL::Polyhedral_cgm_arr_face<Point_3> {
  private:
    Vector3f m_rendered_point;

    /*! Indicates whether it is a marked (dual) vertex */
    Boolean m_marked;

  public:
    /*! Constructor */
    My_arr_face() : m_marked(false) {}

    Boolean marked() const { return m_marked; }
    void set_marked(Boolean marked) { m_marked = marked; }

    const Vector3f& get_rendered_point() const { return m_rendered_point; }
    void set_rendered_point(Vector3f& point) { m_rendered_point = point; }
  };

  /*! A new dcel builder with extended features */
  template <typename Traits>
  class My_arr_dcel :
    public CGAL::Arr_dcel_base<My_arr_vertex<typename Traits::Point_2>,
                               My_arr_halfedge<typename Traits::X_monotone_curve_2>,
                               My_arr_face>
  {
  public:
    My_arr_dcel() {}
  };

  typedef CGAL::Polyhedral_cgm<Kernel,My_arr_dcel>      Polyhedral_cgm;

  typedef Polyhedral_cgm                                Cgm;
  typedef CGAL::Polyhedral_cgm_initializer<Cgm>         Cgm_initializer;

  typedef Cgm::Halfedge_around_vertex_const_circulator
    Halfedge_around_vertex_const_circulator;

  typedef Cgm::Arr_traits                               Arr_traits;
  typedef Arr_traits::Point_2                           Point_2;
  typedef Arr_traits::Curve_2                           Curve_2;
  typedef Arr_traits::X_monotone_curve_2                X_monotone_curve_2;

  typedef My_arr_vertex<Point_2>                        Arr_vertex;

  // Extended Cubical_gaussian_map, Arrangement stuff:
  typedef Cgm::Arrangement                              Arrangement;
  typedef Cgm::Arr_vertex_iterator                      Arr_vertex_iterator;
  typedef Cgm::Arr_vertex_const_iterator
    Arr_vertex_const_iterator;
  typedef Cgm::Arr_edge_iterator                        Arr_edge_iterator;
  typedef Cgm::Arr_edge_const_iterator                  Arr_edge_const_iterator;
  typedef Cgm::Arr_halfedge_iterator                    Arr_halfedge_iterator;
  typedef Cgm::Arr_halfedge_const_iterator
    Arr_halfedge_const_iterator;
  typedef Cgm::Arr_face_iterator                        Arr_face_iterator;
  typedef Cgm::Arr_face_const_iterator                  Arr_face_const_iterator;

  typedef Cgm::Arr_halfedge_around_vertex_const_circulator
    Arr_halfedge_around_vertex_const_circulator;
  typedef Cgm::Arr_ccb_halfedge_circulator
    Arr_ccb_halfedge_circulator;
  typedef Cgm::Arr_ccb_halfedge_const_circulator
    Arr_ccb_halfedge_const_circulator;

  typedef Cgm::Arr_vertex_handle                        Arr_vertex_handle;
  typedef Cgm::Arr_vertex_const_handle                  Arr_vertex_const_handle;
  typedef Cgm::Arr_face_handle                          Arr_face_handle;
  typedef Cgm::Arr_face_const_handle                    Arr_face_const_handle;

  // List of pointers to Cubical_gaussian_map_geo objects */
  typedef std::list<Shared_cubical_gaussian_map_geo>    Cgm_node_list;
  typedef Cgm_node_list::iterator                       Cgm_node_iter;

  /*! A function object that converts Point_3 into Vector3f */
  struct Point_to_vector3f {
    /*! */
    SGAL::Vector3f operator()(const Point_3& point)
    {
      return SGAL::Vector3f((float) CGAL::to_double(point.x()),
                            (float) CGAL::to_double(point.y()),
                            (float) CGAL::to_double(point.z()));
    }
  };

protected:
  typedef Cubical_gaussian_map_geo          Self;

  typedef Cube_renderer                     Surface_renderer;
  typedef Colored_cube_renderer             Colored_surface_renderer;
  typedef Stencil_cube_renderer             Stencil_surface_renderer;

  typedef Vertices_renderer<Self>           Cgm_vertices_renderer;
  typedef Edges_renderer<Self>              Cgm_edges_renderer;

  typedef Inflated_line_edges_renderer<Cgm_edges_renderer>
                                            Cgm_inflated_line_edges_renderer;
  typedef Inflated_strip_edges_renderer<Cgm_edges_renderer>
                                            Cgm_inflated_strip_edges_renderer;
  typedef Inflated_tube_edges_renderer<Cgm_edges_renderer>
                                            Cgm_inflated_tube_edges_renderer;

  /*! A function object that renders the edges with color. */
  class Colored_edges_renderer : public Cgm_edges_renderer {
  public:
    typedef Cubical_gaussian_map_geo          Geometry;

    /*! Constructor. */
    Colored_edges_renderer(Geometry& geo) : Cgm_edges_renderer(geo) {}

    /*! Render the face. */
    virtual void operator()(Draw_action* action);
  };

  /*! A function object that renders the vertices with color. */
  class Colored_vertices_renderer : public Cgm_vertices_renderer {
  public:
    typedef Cubical_gaussian_map_geo            Geometry;
    typedef Cgm_vertices_renderer               Base;

    /*! Constructor */
    Colored_vertices_renderer(Geometry& geo) : Cgm_vertices_renderer(geo) {}

    /*! Render the face */
    virtual void operator()(Draw_action* action);
  };

  /*! A function object that renders the marked vertices */
  class Marked_vertices_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Cubical_gaussian_map_geo          Geometry;

    /*! Constructor */
    Marked_vertices_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the face */
    virtual void operator()(Draw_action* action);

  protected:
    /*! The arrangement geometry */
    Geometry& m_geo;
  };

  /*! A function object that renders the vertices with color */
  class Colored_marked_vertices_renderer : public Marked_vertices_renderer {
  public:
    typedef Cubical_gaussian_map_geo          Geometry;

    /*! Constructor */
    Colored_marked_vertices_renderer(Geometry& geo) :
      Marked_vertices_renderer(geo) {}

    /*! Render the face */
    virtual void operator()(Draw_action* action);
  };

  /*! A function object that renders the nonreal vertices */
  class Nonreal_vertices_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Cubical_gaussian_map_geo          Geometry;

    /*! Constructor */
    Nonreal_vertices_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the face */
    virtual void operator()(Draw_action* action);

  protected:
    /*! The arrangement geometry */
    Geometry& m_geo;
  };

  /*! A function object that renders the nonreal vertices with color. */
  class Colored_nonreal_vertices_renderer : public Nonreal_vertices_renderer {
  public:
    typedef Cubical_gaussian_map_geo          Geometry;

    /*! Constructor. */
    Colored_nonreal_vertices_renderer(Geometry& geo) :
      Nonreal_vertices_renderer(geo) {}

    /*! Render the face. */
    virtual void operator()(Draw_action* action);
  };

  /*! A function object that renders the marked primal vertex. */
  class Marked_face_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Cubical_gaussian_map_geo          Geometry;

    /*! Constructor. */
    Marked_face_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the face. */
    virtual void operator()(Draw_action* action);

  protected:
    /*! The arrangement geometry. */
    Geometry& m_geo;
  };

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Create the renderers */
  void create_renderers();

  /*! Destroy the renderers */
  void destroy_renderers();

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! */
  template <typename Iterator>
  class Cgm_iterator {
  private:
    Iterator m_it;

  public:
    typedef typename Iterator::iterator_category       iterator_category;
    typedef Cgm*                                       value_type;
    typedef typename Iterator::difference_type         difference_type;
    typedef Cgm**                                      pointer;
    typedef Cgm*&                                      reference;

  public:
    // Default constructor.
    Cgm_iterator () : m_it() {}

    // Constructor.
    Cgm_iterator (Iterator it) : m_it(it) {}

    // operator*
    const value_type operator*() const
    {
      Iterator& base = m_it;
      return (*base)->get_cgm();
    }

    value_type operator*()
    {
      Iterator& base = m_it;
      return (*base)->get_cgm();
    }

    /*! \brief operator pre ++ */
    Cgm_iterator& operator++()
    {
      ++m_it;
      return *this;
    }

    /*! \brief operator post ++ */
    Cgm_iterator operator++(int)
    {
      Cgm_iterator tmp = *this;
      ++*this;
      return tmp;
    }

    // operator->
    pointer operator->()
    {
      Iterator& base = m_it;
      return &((*base)->get_cgm());
    }

    const pointer operator->() const
    {
      Iterator& base = m_it;
      return &((*base)->get_cgm());
    }
  };

  typedef Cgm_iterator<Cgm_node_iter>                   Cgm_iter;

  /*! Extracts the approximate point from a planar-map face */
  struct Extract_dual_approximate_sphere {
    /*! */
    Approximate_sphere_3 operator()(const Arrangement::Face& face) const
    {
      SGAL_assertion(!face.is_unbounded());
      return to_approximate_sphere(face.point());
    }
  };

  /*! Extracts the approximate point from a planar-map face */
  struct Extract_dual_vector3f_point {
    /*! */
    SGAL::Vector3f operator()(const Arrangement::Face& face) const
    {
      SGAL_assertion(!face.is_unbounded());
      const Point_3& p = face.point();
      return Point_to_vector3f()(p);
    }
  };

  /*! A functor with an operator that sets the "marked" flag */
  class Face_set_marked_op {
  public:
    void operator()(Arr_face_handle fh) { fh->set_marked(true); }
  };

  /*! Notification for computing the cgm */
  class Cgm_geo_initializer_visitor :
    public CGAL::Polyhedral_cgm_initializer_visitor<Cgm>
  {
  public:
    /*! Destructor */
    virtual ~Cgm_geo_initializer_visitor() {}

    /*! Pass information from the polyhedron vertex to its dual - a cgm-face. */
    virtual void update_dual_vertex(Polyhedron_vertex_const_handle src,
                                    Arr_face_handle trg)
    {
      trg->set_marked(src->marked());
    }

    /*! Pass information from a cgm face to another cgm face that are duals
     * of the same vertex.
     */
    virtual void update_dual_vertex(Arr_face_const_handle src,
                                    Arr_face_handle trg)
    {
      trg->set_marked(src->marked());
    }

    /*! Pass information from a polyhedron facet to its dual a cgm-vertex. */
    virtual void update_dual_face(Polyhedron_facet_const_handle src,
                                  Arr_vertex_handle trg)
    {
      trg->set_marked(src->marked());
    }

    /*! Pass information from a polyhedron facet to its dual a cgm-vertex. */
    virtual void update_dual_halfedge(Polyhedron_halfedge_const_handle src,
                                      Arr_halfedge_handle trg)
    {
      trg->set_marked(src->marked());
    }
  };

  /*! The node prototype */
  static SGAL::Container_proto* s_prototype;

  /*! Default value */
  static const Boolean s_def_draw_aos;
  static const Boolean s_def_draw_aos_opaque;
  static const Boolean s_def_draw_aos_haloed;
  static const Boolean s_def_draw_aos_unfolded;

  static const Boolean s_def_draw_aos_surface;
  static const Vector3f s_def_aos_surface_color;

  // Default dual vertex attributes:
  static const Vertex_style s_def_aos_vertex_style;
  static const Float s_def_aos_vertex_radius;
  static const Float s_def_aos_vertex_point_size;
  static const Vector3f s_def_aos_vertex_color;

  // Default dual nonreal-vertex attributes:
  static const Vertex_style s_def_aos_nonreal_vertex_style;
  static const Float s_def_aos_nonreal_vertex_radius;
  static const Float s_def_aos_nonreal_vertex_point_size;
  static const Vector3f s_def_aos_nonreal_vertex_color;

  // Default dual edge attributes:
  static const Boolean s_def_aos_edge_enabled;
  static const Edge_style s_def_aos_edge_style;
  static const Int s_def_aos_edge_count;
  static const Boolean s_def_aos_edge_directed;
  static const Float s_def_aos_edge_radius;
  static const Float s_def_aos_edge_line_width;
  static const Vector3f s_def_aos_edge_color;

  // Default dual nonreal-edge attributes:
  static const Boolean s_def_aos_nonreal_edge_enabled;
  static const Edge_style s_def_aos_nonreal_edge_style;
  static const Int s_def_aos_nonreal_edge_count;
  static const Boolean s_def_aos_nonreal_edge_directed;
  static const Float s_def_aos_nonreal_edge_radius;
  static const Float s_def_aos_nonreal_edge_line_width;
  static const Vector3f s_def_aos_nonreal_edge_color;

  static const Float s_def_aos_delta_angle;

  static const Vertex_style s_def_aos_marked_vertex_style;
  static const Float s_def_aos_marked_vertex_radius;
  static const Float s_def_aos_marked_vertex_point_size;

  static const Boolean s_def_draw_marked_vertex;
  static const Boolean s_def_draw_marked_edge;
  static const Boolean s_def_draw_marked_facet;

  static const Vector3f s_def_marked_vertex_color;
  static const Vector3f s_def_marked_edge_color;
  static const Vector3f s_def_marked_facet_color;

  static const float s_def_marked_vertex_radius;
  static const float s_def_marked_edge_radius;

  /*! The cubical Gaussian map representation. */
  Polyhedral_cgm m_cgm;

  /*! Indicates wether to draw the polyhedron or its dual. */
  Boolean m_draw_aos;

  /*! The opposite of the m_draw_dual flag. */
  Boolean m_draw_primal;

  /*! Indicates whether to draw the cube opaque. */
  Boolean m_draw_aos_opaque;

  /*! Indicates whether to draw haloed lines. */
  Boolean m_draw_aos_haloed;

  /*! Indicates whether to draw cube unfolded. */
  Boolean m_draw_aos_unfolded;

  /*! Indicates whether to draw the background. */
  Boolean m_draw_aos_surface;

  /*! The color of the faces in the dual representation. */
  Vector3f m_aos_surface_color;

  // Dual vertex attributes:

  /*! The vertex shape style. */
  Vertex_style m_aos_vertex_style;

  /*! The radius of the dual vertices. */
  Float m_aos_vertex_radius;

  /*! The size of the point that represents a dual vertex. */
  Float m_aos_vertex_point_size;

  /*! The color of the vertices. */
  Vector3f m_aos_vertex_color;

  // Dual nonreal-vertex attributes:

  /*! The nonreal vertex shape style. */
  Vertex_style m_aos_nonreal_vertex_style;

  /*! The radius of the dual nonreal vertices. */
  Float m_aos_nonreal_vertex_radius;

  /*! The size of the point that represents a dual nonreal vertex. */
  Float m_aos_nonreal_vertex_point_size;

  /*! The color of the vertices. */
  Vector3f m_aos_nonreal_vertex_color;

  // Dual edge attributes:

  /*! Indicates whether the rendering of edges is enabled or not. */
  Boolean m_aos_edge_enabled;

  /*! The edge rendering style. */
  Edge_style m_aos_edge_style;

  /*! The edge rendering type. */
  Int m_aos_edge_count;

  /*! Indicates whether edges are rendered directed or not. */
  Boolean m_aos_edge_directed;

  /*! The radius of the dual edges. */
  Float m_aos_edge_radius;

  /*! The width of a line that represents a dual edge. */
  Float m_aos_edge_line_width;

  /*! The color of the real dual edges. */
  Vector3f m_aos_edge_color[2];

  // Non-real dual edge attributes:

  /*! Indicates whether the rendering of nonreal edges is enabled or not. */
  Boolean m_aos_nonreal_edge_enabled;

  /*! The nonreal edge rendering style. */
  Edge_style m_aos_nonreal_edge_style;

  /*! The nonreal edge rendering type. */
  Int m_aos_nonreal_edge_count;

  /*! Indicates whether nonreal edges are rendered directed or not. */
  Boolean m_aos_nonreal_edge_directed;

  /*! The radius of the nonreal dual edges. */
  Float m_aos_nonreal_edge_radius;

  /*! The width of the line that represents a non-real dual edge. */
  Float m_aos_nonreal_edge_line_width;

  /*! The color of the non-real dual edges. */
  Vector3f m_aos_nonreal_edge_color;

  /*! The angle of a single line strip of a spherical arc. */
  Float m_aos_delta_angle;

  // Dual marked vertex attributes:

  /*! The dual marked vertex shape style. */
  Vertex_style m_aos_marked_vertex_style;

  /*! The radius of the dual marked vertex. */
  Float m_aos_marked_vertex_radius;

  /*! The size of the point that represents a dual marked vertex. */
  Float m_aos_marked_vertex_point_size;

  // Marked feature attributes:

  /*! Indicates whether to draw the marked vertex. */
  Boolean m_draw_marked_vertex;

  /*! Indicates whether to draw the marked halfedge. */
  Boolean m_draw_marked_edge;

  /*! Indicates whether to draw the marked face. */
  Boolean m_draw_marked_facet;

  /*! The color of the marked vertex. */
  Vector3f m_marked_vertex_color;

  /*! The color of the marked edge. */
  Vector3f m_marked_edge_color;

  /*! The color of the marked face */
  Vector3f m_marked_facet_color;

  /*! Indicates that the coordinates were tranlated. */
  Boolean m_translated;

  /*! Indicates that the coordinates were rotated. */
  Boolean m_rotated;

  /*! Indicates whether to compute the minkowski sum. */
  Boolean m_minkowski_sum;

  /*! The minkowski sum operands. */
  Cgm_node_list m_cgm_nodes;

  /*! For benchmarking. */
  float m_time;

  /*! When trigerred the CGM is exported. */
  Boolean m_export;

  /*! The shape of a marked vertex. */
  Sphere* m_vertex_geom;

  /*! The shape of a marked edge. */
  Cylinder* m_edge_geom;

  /*! The radius of the geometry that represents a marked vertex. */
  Float m_marked_vertex_radius;

  /*! The radius of the geometry that represents a marked edge. */
  Float m_marked_edge_radius;

  /*! The index of the marked vertex. */
  Uint m_marked_vertex_index;

  /*! The index of the marked edge. */
  Uint m_marked_edge_index;

  /*! The index of the marked face. */
  Uint m_marked_facet_index;

  /*! When trigerred the vertex index is increased. */
  Boolean m_increase_vertex_index;

  /*! When trigerred the edge index is increased. */
  Boolean m_increase_edge_index;

  /*! When trigerred the face index is increased. */
  Boolean m_increase_facet_index;

  /*! Indicates whether the renderer must be cleaned. */
  Boolean m_renderer_dirty;

  /*! The renderer of the arrangement data structure. */
  Arrangement_renderer m_renderer;

  /*! The dual marked face (primal vertex) renderer. */
  Arrangement_renderer::Renderer* m_aos_marked_face_renderer;

  /*! The surface renderer. */
  Arrangement_renderer::Renderer* m_surface_renderer;

  /*! The surface renderer. */
  Colored_surface_renderer* m_colored_surface_renderer;

  /*! The surface renderer. */
  Arrangement_renderer::Renderer* m_stencil_surface_renderer;

  /*! The vertices renderer. */
  Arrangement_renderer::Renderer* m_vertices_renderer;

  /*! The vertices with color renderer. */
  Arrangement_renderer::Renderer* m_colored_vertices_renderer;

  /*! The nonreal vertices renderer. */
  Arrangement_renderer::Renderer* m_nonreal_vertices_renderer;

  /*! The nonreal vertices with color renderer. */
  Arrangement_renderer::Renderer* m_colored_nonreal_vertices_renderer;

  /*! The marked_vertices renderer. */
  Arrangement_renderer::Renderer* m_marked_vertices_renderer;

  /*! The marked_vertices renderer. */
  Arrangement_renderer::Renderer* m_colored_marked_vertices_renderer;

  /*! The edges renderer. */
  Arrangement_renderer::Renderer* m_edges_renderer;

  /*! The edges renderer. */
  Arrangement_renderer::Renderer* m_colored_edges_renderer;

  /*! The inflated line edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_line_edges_renderer;

  /*! The inflated strip edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_strip_edges_renderer;

  /*! The inflated tube edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_tube_edges_renderer;

  /*! brief computes the normals of the facets and store them in the dual
   * vertices.
   */
  void process_facets();

  /*! Draw the primal representation of the polyhedron. */
  void draw_primal(Draw_action* action);

  /*! Draw the primal marked vertex. */
  void draw_primal_marked_vertex(Draw_action* action);

  /*! Draw the primal marked edge. */
  void draw_primal_marked_edge(Draw_action* action);

  /*! Draw the dual representation opaque. */
  void draw_aos_opaque(Draw_action* action);

  /*! Draw a dual vertex.
   * \param action
   * \param center
   * \param normal
   */
  void draw_aos_vertex(Draw_action* action, const Vector3f& center,
                       const Vector3f& normal);

  /*! Draw a dual nonreal vertex.
   * \param action
   * \param center
   * \param normal
   */
  void draw_aos_nonreal_vertex(Draw_action* action, const Vector3f& center,
                               const Vector3f& normal);

  /*! Draw a dual marked vertex */
  void draw_aos_marked_vertex(Draw_action* action, const Vector3f& center,
                              const Vector3f& normal);

  /*! Draw a dual edge.
   * \param action
   * \param source
   * \param target
   * \param normal
   */
  void draw_aos_edge(Draw_action* action,
                     const Vector3f& source, const Vector3f& target,
                     const Vector3f& normal);

  /*! Draw a dual nonreal-edge.
   * \param action
   * \param source
   * \param target
   * \param normal
   */
  void draw_aos_nonreal_edge(Draw_action* action,
                             const Vector3f& source, const Vector3f& target,
                             const Vector3f& normal);

  /*! draws the face of the unit cube. */
  void draw_cube_face(unsigned int id);

  /*! \brief draws the dual marked face. */
  void draw_aos_marked_face(unsigned int id);

  /*! \brief draws the dual marked edge. */
  void draw_aos_marked_edge(unsigned int id);

  /*! \brief draws the dual marked vertex. */
  void draw_aos_marked_vertex(unsigned int id);

  /*! Draw the planar map associated with a face of the unit cube. */
  void draw_projection(SGAL::Draw_action* action, unsigned int id,
                       float non_edge_line_width);

  /*! Export the planar maps associated with the faces of the unit cube. */
  void output(SGAL::Field_info* field_info)
  {
    // std::cout << m_cgm << std::endl;
    for (Uint i = 0; i < Polyhedral_cgm::NUM_FACES; ++i) {
      std::cout << "# Arrangement " << i << std::endl << std::endl;
      const Arrangement& arr = m_cgm.arrangement(i);
      std::cout << arr << std::endl;
    }
  }

  /*! Import  the planar maps associated with the faces of the unit cube. */
  void input()
  {
    // std::in >> m_cgm;
  }

  void isect_primary();

public:
  /*! Constructor. */
  Cubical_gaussian_map_geo(Boolean proto = false);

  /*! Copy Constructor. */
  Cubical_gaussian_map_geo(const Cubical_gaussian_map_geo& gaussian_map);

  /*! Destructor. */
  virtual ~Cubical_gaussian_map_geo();

  /*! Construct the prototype. */
  static Cubical_gaussian_map_geo* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! \brief deletes the prototype. */
  virtual void delete_prototype();

  /*! \brief obtains the prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* draw_aos_handle(Field_info*) { return &m_draw_aos; }
  Boolean* draw_aos_unfolded_handle(Field_info*) { return &m_draw_aos_unfolded; }
  Boolean* draw_aos_opaque_handle(Field_info*) { return &m_draw_aos_opaque; }
  Boolean* draw_aos_haloed_handle(Field_info*) { return &m_draw_aos_haloed; }
  Boolean* draw_aos_surface_handle(Field_info*) { return &m_draw_aos_surface; }
  Float* aos_edge_line_width_handle(Field_info*)
    { return &m_aos_edge_line_width; }
  Boolean* translated_handle(Field_info*) { return &m_translated; }
  Boolean* rotated_handle(Field_info*) { return &m_rotated; }
  Boolean* draw_primal_handle(Field_info*) { return &m_draw_primal; }
  Boolean* export_handle(Field_info*) { return &m_export; }
  Boolean* increase_vertex_index_handle(Field_info*)
    { return &m_increase_vertex_index; }
  Boolean* increase_edge_index_handle(Field_info*)
    { return &m_increase_edge_index; }
  Boolean* increase_facet_index_handle(Field_info*)
    { return &m_increase_facet_index; }
  Vector3f* aos_edge_color1_handle(Field_info*) { return &m_aos_edge_color[0]; }
  Vector3f* aos_edge_color2_handle(Field_info*) { return &m_aos_edge_color[1]; }
//   MF_container cgm_nodes_handle(Field_info*) { return &m_cgm_nodes); }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action);

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! Calculate the bounding sphere.
   * \return a Boolean flag that indicates whether the sphere bound changed.
   */
  virtual Boolean clean_sphere_bound();

  /*! \brief draws the internal representation. */
  virtual void draw_geometry(Draw_action* action);

  /*! \brief cleans the representation. */
  virtual void clean();

  /*! \brief clears the internal representation and auxiliary data structures.
   */
  virtual void clear();

  /*! \brief determineswhether the representation is empty. */
  virtual Boolean is_empty() const { return m_cgm.is_empty(); }

  /*! Draw the dual vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action);

  /*! Draw the dual nonreal vertices.
   * \param action
   */
  virtual void draw_aos_nonreal_vertices(Draw_action* action);

  /*! Draw the dual marked vertices.
   * \param action
   */
  virtual void draw_aos_marked_vertices(Draw_action* action);

  /*! Draw the dual edges
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action);

  /*! Clean the renderer */
  virtual void clean_renderer();

  /*! Update the representation */
  void update();

  /*! Process change of coordinate field.
   * Generally process the change of coordinate field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void coord_changed(Field_info* field_info);

  /*! Raise the flag that indicates that the sphere bound changed. */
  void draw_changed(Field_info* field_info = NULL);

  /*! Draw the dual representation of the polyhedron in 2D. */
  void draw_aos_unfolded(Draw_action* action);

  /*! Obtain the dual surface color. */
  const Vector3f& get_aos_surface_color(void) const;

  /*! Set the dual surface color. */
  void set_aos_surface_color(const Vector3f& color);

  // Dual vertex attributes:

  /*! Obtain the vertex shape style. */
  Vertex_style get_aos_vertex_style() const { return m_aos_vertex_style; }

  /*! Obtain the vertex shape. */
  Float get_aos_vertex_point_size() const { return m_aos_vertex_point_size; }

  /*! Obtain the vertex radius. */
  Float get_aos_vertex_radius() const { return m_aos_vertex_radius; }

  /*! Obtain the dual vertex color. */
  const Vector3f& get_aos_vertex_color() const { return m_aos_vertex_color; }

  /*! Set the vertex color. */
  void set_aos_vertex_color(const Vector3f& color)
  { m_aos_vertex_color = color; }

  // Dual nonreal vertex attributes:

  /*! Obtain the nonreal-vertex shape style. */
  Vertex_style get_aos_nonreal_vertex_style() const
  { return m_aos_nonreal_vertex_style; }

  /*! Obtain the nonreal vertex shape. */
  Float get_aos_nonreal_vertex_point_size() const
  { return m_aos_nonreal_vertex_point_size; }

  /*! Obtain the nonreal vertex radius. */
  Float get_aos_nonreal_vertex_radius() const
  { return m_aos_nonreal_vertex_radius; }

  /*! Obtain the dual nonreal vertex color. */
  const Vector3f& get_aos_nonreal_vertex_color() const
  { return m_aos_nonreal_vertex_color; }

  /*! Set the dual nonreal vertex color. */
  void set_aos_nonreal_vertex_color(const Vector3f& color)
  { m_aos_nonreal_vertex_color = color; }

  // Dual edge attributes:

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
  Int get_aos_edge_count() const { return m_aos_edge_count; }

  /*! Set the edge shape count. */
  void set_aos_edge_count(Int count) { m_aos_edge_count = count; }

  /*! Determine whether edges are rendered directed. */
  Boolean get_aos_edge_directed() const { return m_aos_edge_directed; }

  /*! Set the flag that determines whether edges are rendered directed. */
  void set_aos_edge_directed(Boolean dir) { m_aos_edge_directed = dir; }

  /*! Obtaint the radius of the tubes that represent dual edges. */
  Float get_aos_edge_radius() const { return m_aos_edge_radius; }

  /*! Set the radius of the tubes that represent dual edges. */
  void set_aos_edge_radius(Float radius) { m_aos_edge_radius = radius; }

  /*! Set the width of the edges drawn as lines. */
  void set_aos_edge_line_width(Float width)
  { m_aos_edge_line_width = width; }

  /*! Obtaint the width of the edges drawn as lines. */
  Float get_aos_edge_line_width() const
  { return m_aos_edge_line_width; }

  /*! Set the edge color. */
  void set_aos_edge_color(const Vector3f& color, unsigned int id = 0);

  /*! Obtain the edge color. */
  const Vector3f& get_aos_edge_color(unsigned int id = 0) const;

  // Nonreal edges:

  /*! Set the nonreal-edge color. */
  void set_aos_nonreal_edge_color(const SGAL::Vector3f& color)
  { m_aos_nonreal_edge_color = color; }

  /*! Obtain the nonreal-edge color. */
  const Vector3f& get_aos_nonreal_edge_color() const
  { return m_aos_nonreal_edge_color; }

  /*! Obtain the angle of a trangle fan. */
  Float get_aos_delta_angle() const { return m_aos_delta_angle; }

  // Aos marked vertex attributes:

  /*! Obtain the marked vertex shape style. */
  Vertex_style get_aos_marked_vertex_style() const
  { return m_aos_marked_vertex_style; }

  /*! Obtain the marked vertex shape. */
  Float get_aos_marked_vertex_point_size() const
  { return m_aos_marked_vertex_point_size; }

  /*! Obtain the marked vertex radius. */
  Float get_aos_marked_vertex_radius() const
  { return m_aos_marked_vertex_radius; }

  // Marked feature attributes:

  /*! Obtain the index of the marked vertex. */
  Uint marked_vertex_index() const { return m_marked_vertex_index; }

  /*! Obtain the index of the marked edge */
  Uint marked_edge_index() const { return m_marked_edge_index; }

  /*! Obtain the index of the marked facet. */
  Uint marked_facet_index() const { return m_marked_facet_index; }

  /*! Set the index of the marked vertex. */
  void set_marked_vertex_index(Uint index) { m_marked_vertex_index = index; }

  /*! Set the index of the marked edge. */
  void set_marked_edge_index(Uint index) { m_marked_edge_index = index; }

  /*! Set the index of the marked facet. */
  void set_marked_facet_index(Uint index) { m_marked_facet_index = index; }

  /*! Obtain the color of the marked vertex. */
  const Vector3f& get_marked_vertex_color() const;

  /*! Set the color of the marked vertex. */
  void set_marked_vertex_color(const Vector3f& color);

  /*! Set the color of the marked edge. */
  void set_marked_edge_color(const SGAL::Vector3f& color);

  /*! Set the color of the marked facet. */
  void set_marked_facet_color(const SGAL::Vector3f& color);

  /*! Obtain the flag that indicates whether to draw the embedding surface. */
  Boolean get_draw_aos_surface() const { return m_draw_aos_surface; }

  /*! Set the flag that indicates whether to draw the background. */
  void set_draw_aos_surface(Boolean draw_bg)
  { m_draw_aos_surface = draw_bg; }

  /*! Set the source gausian maps of the minkowski sum. */
  void insert_cgm(Shared_cubical_gaussian_map_geo cgm);

  /*! Print statistics. */
  void print_stat();

  /*! Set the flag that indicates whether to draw the primal or dual. */
  void set_draw_aos(Boolean flag) { m_draw_aos = flag; }

  /*! Set the flag that indicates whether to draw haloed lines. */
  void set_draw_aos_haloed(Boolean flag) { m_draw_aos_haloed = flag; }

  /*! Obtain the flag that indicates whether to draw haloed lines. */
  Boolean get_draw_aos_haloed() const { return m_draw_aos_haloed; }

  /*! Obrain a reference to the cubical Gaussian map. */
  Polyhedral_cgm* get_cgm() { return &m_cgm; }

  /*! Process change of geometry. */
  virtual void field_changed(Field_info* field_info);

  /*! Set the flag that indicates whether to compute the minkowski sum. */
  void set_minkowski_sum(Boolean flag) { m_minkowski_sum = flag; }

  /*! Increase the vertex index. */
  void increase_vertex_index(Field_info* field_info = NULL);

  /*! Increase the edge index. */
  void increase_edge_index(Field_info* field_info = NULL);

  /*! Increase the face index. */
  void increase_facet_index(Field_info* field_info = NULL);
};

/*! \brief constructs the prototype. */
inline Cubical_gaussian_map_geo* Cubical_gaussian_map_geo::prototype()
{ return new Cubical_gaussian_map_geo(true); }

/*! \brief clones. */
inline Container* Cubical_gaussian_map_geo::clone()
{ return new Cubical_gaussian_map_geo(); }

/*! \brief set the curve color */
inline void
Cubical_gaussian_map_geo::set_aos_edge_color(const Vector3f& color,
                                             unsigned int id)
{ m_aos_edge_color[id] = color; }

/*! \brief obtains the curve color. */
inline const SGAL::Vector3f &
Cubical_gaussian_map_geo::get_aos_edge_color(unsigned int id) const
{ return m_aos_edge_color[id]; }

/*! \brief obtains the dual surface color. */
inline
const Vector3f& Cubical_gaussian_map_geo::get_aos_surface_color(void) const
{ return m_aos_surface_color; }

/*! \brief sets dual the surface color. */
inline
void Cubical_gaussian_map_geo::
set_aos_surface_color(const SGAL::Vector3f& color)
{
  m_aos_surface_color = color;
  if (m_colored_surface_renderer) m_colored_surface_renderer->set_color(color);
}

/*! \brief obtains the color of the marked vertex. */
inline
const Vector3f& Cubical_gaussian_map_geo::get_marked_vertex_color() const
{ return m_marked_vertex_color; }

/*! \brief sets the color of the marked vertex. */
inline void
Cubical_gaussian_map_geo::set_marked_vertex_color(const Vector3f& color)
{ m_marked_vertex_color = color; }

/*! \brief sets the color of the marked edge. */
inline void
Cubical_gaussian_map_geo::set_marked_edge_color(const Vector3f& color)
{ m_marked_edge_color = color; }

/*! \brief sets the color of the marked facet. */
inline void
Cubical_gaussian_map_geo::set_marked_facet_color(const Vector3f& color)
{ m_marked_facet_color = color; }

SGAL_END_NAMESPACE

#endif
