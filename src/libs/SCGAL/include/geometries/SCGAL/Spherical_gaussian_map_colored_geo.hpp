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
// $Id: Spherical_gaussian_map_colored_geo.hpp 13463 2012-06-04 15:22:36Z efif $
// $Revision: 13463 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_SPHERICAL_GAUSSIAN_MAP_COLORED_GEO_HPP
#define SCGAL_SPHERICAL_GAUSSIAN_MAP_COLORED_GEO_HPP

/*! \file
 */

#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_polyhedron_3.h>
// #include <CGAL/IO/Arr_iostream.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Spherical_gaussian_map_colored.hpp"
#include "SCGAL/Spherical_gaussian_map_base_geo.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_renderer.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"
#include "SCGAL/Arrangement_renderers.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Container_proto;
class Cull_context;
class Draw_action;
class Scene_graph;
class Cylinder;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Spherical_gaussian_map_colored_geo :
  public Spherical_gaussian_map_base_geo
{
public:
  typedef boost::shared_ptr<Spherical_gaussian_map_colored_geo>
    Shared_spherical_gaussian_map_colored_geo;

  typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Spherical_gaussian_map_colored,
                                                Exact_kernel>
                                                  Polyhedron;
  /*! Fields */
  enum {
    FIRST = Spherical_gaussian_map_base_geo::LAST - 1,
    GEOMETRIES,
    LAST
  };

private:
  typedef Spherical_gaussian_map_colored          Sgm;

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

  /*! Notification for computing the sgm. */
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
    virtual void update_dual_vertex(Polyhedron_vertex_const_handle /* src */,
                                    Sgm_face_handle /* trg */)
    {
      // trg->set_color(src->color());
    }

    /*! Pass information from an aos face to another aos face that are duals
     * of the same polyhedron vertex
     */
    virtual void update_dual_vertex(Sgm_face_const_handle src,
                                    Sgm_face_handle trg)
    { trg->set_color(src->color()); }

    /*! Pass information from a polyhedron facet to its dual - an aos vertex */
    virtual void update_dual_face(Polyhedron_facet_const_handle /* src */,
                                  Sgm_vertex_handle /* trg */)
    {
      // trg->set_color(src->color());
    }

    /*! Pass information from a polyhedron halfedge to its dual - an aos
     * halfedge
     */
    virtual void update_dual_halfedge(Polyhedron_halfedge_const_handle /* src */,
                                      Sgm_halfedge_handle /* trg */)
    {
      // trg->set_color(src->color());
    }
  };

  typedef CGAL::
  Arr_polyhedral_sgm_initializer<Sgm, Polyhedron, Sgm_geo_initializer_visitor>
                                                  Sgm_initializer;

protected:
  typedef Spherical_gaussian_map_colored_geo            Self;

  /*! A function object that renders the edges with color */
  class Colored_edges_renderer : public Arrangement_renderer::Renderer {
  public:
    typedef Spherical_gaussian_map_colored_geo          Geometry;

    /*! Constructor */
    Colored_edges_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the face */
    virtual void operator()(Draw_action* action);

  protected:
    /*! The arrangement geometry */
    Geometry& m_geo;

  private:
    // Assignment operator cannot be generated.
    Colored_edges_renderer& operator=(const Colored_edges_renderer&);

    // In C++11, the following is supported:
    // Colored_edges_renderer& operator=(const Colored_edges_renderer&) = delete;
  };

  // List of pointers to Spherical_gaussian_map_colored_geo objects. */
  typedef Array<Shared_spherical_gaussian_map_colored_geo>      Sgm_node_array;
  typedef Sgm_node_array::iterator                              Sgm_node_iter;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Create the renderers. */
  void create_renderers();

  /*! Destroy the renderers. */
  void destroy_renderers();

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whther the sgm data structure is owned, i.e., allocated
   * explicitly). If it is owned, the sgm data structure should be destructed
   * when the this node is destructed.
   */
  Boolean m_owned_sgm;

  /*! The cubical Gaussian map representation. */
  Sgm* m_sgm;

  /*! An intermediate polyhedron. */
  Polyhedron* m_polyhedron;

  /*! The minkowski sum operands. */
  Sgm_node_array m_sgm_nodes;

  /*! Indicates whether to compute the minkowski sum. */
  Boolean m_minkowski_sum;

  /*! For benchmarking. */
  float m_time;

  /*! The vertices renderer. */
  Arrangement_renderer::Renderer* m_vertices_renderer;

  /*! The vertices renderer. */
  Arrangement_renderer::Renderer* m_colored_vertices_renderer;

  /*! The edges renderer. */
  Arrangement_renderer::Renderer* m_edges_renderer;

  /*! The colored edges renderer. */
  Arrangement_renderer::Renderer* m_colored_edges_renderer;

  /*! The inflated line edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_line_edges_renderer;

  /*! The inflated strip edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_strip_edges_renderer;

  /*! The inflated tube edges renderer. */
  Arrangement_renderer::Renderer* m_inflated_tube_edges_renderer;

  /*! Default values. */

  /*! Transform the coordinates of the SGM into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  virtual void transform_coords(Approximate_sphere_vector& spheres)
  { transform_primal_coords(m_sgm, spheres); }

  /*! Draw the primal representation of the polyhedron. */
  virtual void draw_primal(Draw_action* action);

  /*! Draw the arrangement on sphere opaque.
   * \param action
   */
  virtual void draw_aos_opaque(Draw_action* action);

  virtual void isect_primary();

public:
  /*! Constructor. */
  Spherical_gaussian_map_colored_geo(Boolean proto = false);

  /*! Copy constructor. */
  Spherical_gaussian_map_colored_geo
  (const Spherical_gaussian_map_colored_geo& gm);

  /*! Destructor. */
  virtual ~Spherical_gaussian_map_colored_geo();

  /*! Construct the prototype. */
  static Spherical_gaussian_map_colored_geo* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the prototype. */
  virtual void delete_prototype();

  /*! Obtain the prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Sgm_node_array* sgm_nodes_handle(Field_info*) { return &m_sgm_nodes; }
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Clean the representation. */
  virtual void clean_sgm();

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();

  /*! Return true if the representation is empty. */
  virtual Boolean is_empty() const { return m_sgm->is_empty(); }

  /*! Draw the arrangement on sphere vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action);

  /*! Draw the arrangement on sphere edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action);

  /*! Clean the renderer. */
  virtual void clean_renderer();

  /*! Print statistics. */
  virtual void print_stat();

  /*! Set the source gausian maps of the minkowski sum. */
  void insert_sgm(Shared_spherical_gaussian_map_colored_geo sgm);

  /*! Obrain a reference to the cubical Gaussian map. */
  Sgm* get_sgm();

  /*! Set the Gaussian map. */
  void set_sgm(Sgm* sgm);

  /*! Set the flag that indicates whether to compute the minkowski sum. */
  void set_minkowski_sum(Boolean flag) { m_minkowski_sum = flag; }

  /*! Compute the equations of, or the normals to, the planes of the aos
   * facets and store them at the vertices of the planar map.
   */
  void update_facets()
  { Spherical_gaussian_map_base_geo::update_facets(m_sgm); }

  /*! Obtain an intermediate polyhedron. */
  Polyhedron* get_polyhedron() { return m_polyhedron; }

  /*! Set an intermediate polyhedron. */
  void set_polyhedron(Polyhedron* polyhedron);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype. */
inline Spherical_gaussian_map_colored_geo*
Spherical_gaussian_map_colored_geo::prototype()
{ return new Spherical_gaussian_map_colored_geo(true); }

/*! \brief clones. */
inline Container* Spherical_gaussian_map_colored_geo::clone()
{ return new Spherical_gaussian_map_colored_geo(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Spherical_gaussian_map_colored_geo::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
