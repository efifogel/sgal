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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a quadric
 */

#ifndef SCGAL_ARRANGEMENT_ON_QUADRIC_GEO_HPP
#define SCGAL_ARRANGEMENT_ON_QUADRIC_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_qdx_topology_traits_2.h>

#include <QdX/Quadric_intersections_on_base_traits_2.h>
#include <NiX/Arithmetic_traits.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Epec_kernel.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_surface_geo.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;
class Quadric_geo;

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a quadric.
 */
class SGAL_SGAL_DECL Arrangement_on_quadric_geo :
  public Arrangement_on_surface_geo
{
public:
  // Shared_pointer
  typedef boost::shared_ptr<Quadric_geo>              Shared_quadric_geo;

  enum {
    FIRST = Arrangement_on_surface_geo::LAST - 1,
    BASE_QUADRIC,
    QUADRICS,
    LAST
  };

  // Kernel, Number type, etc.
  typedef Epec_kernel                                 Kernel;
  typedef Epec_point_3                                Point_3;

protected:
  // Inexact types:
  typedef CGAL::Min_sphere_of_spheres_d_traits_3<Inexact_kernel, Inexact_FT>
                                                      Min_sphere_traits;
  typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits>
                                                      Min_sphere;
  typedef Min_sphere_traits::Sphere                   Inexact_sphere_3;
  typedef Inexact_kernel::Point_3                     Inexact_point_3;
  typedef Inexact_kernel::Vector_3                    Inexact_vector_3;

  typedef std::vector<Inexact_sphere_3>
    Inexact_sphere_vector;
  typedef Inexact_sphere_vector::const_iterator       Inexact_sphere_iter;

  // List of pointers to Quadric_geo objects */
  typedef std::list<Quadric_geo *>                    Quadric_node_list;
  typedef Quadric_node_list::iterator                 Quadric_node_iter;

  typedef NiX::Arithmetic_traits AT;

  typedef QdX::Quadric_intersections_on_base_traits_2< AT >
                                                   Geom_traits_base;
#if defined(CGAL_ARR_TRACING_TRAITS)
  typedef CGAL::Arr_tracing_traits_2<Geom_traits_base>
                                                   Geom_traits;
#else
  typedef Geom_traits_base                         Geom_traits;
#endif
  typedef CGAL::Arr_qdx_topology_traits_2<Geom_traits>
                                                   Topol_traits;

  typedef CGAL::Arrangement_on_surface_2<Geom_traits, Topol_traits>
                                                   Arrangement_on_quadric;

  typedef Geom_traits::Point_2                     Point;
  typedef Geom_traits::X_monotone_curve_2          X_monotone_curve;
  typedef Geom_traits::Curve_2                     Curve;

  typedef Arrangement_on_quadric                   Aoq;

  typedef Aoq::Vertex                              Aoq_vertex;
  typedef Aoq::Halfedge                            Aoq_halfedge;
  typedef Aoq::Face                                Aoq_face;

  typedef Aoq::Vertex_handle                       Aoq_vertex_handle;
  typedef Aoq::Halfedge_handle                     Aoq_halfedge_handle;
  typedef Aoq::Face_handle                         Aoq_face_handle;

  typedef Aoq::Vertex_const_handle                 Aoq_vertex_const_handle;
  typedef Aoq::Halfedge_const_handle               Aoq_halfedge_const_handle;
  typedef Aoq::Face_const_handle                   Aoq_face_const_handle;

  typedef Aoq::Vertex_iterator                     Aoq_vertex_iterator;
  typedef Aoq::Halfedge_iterator                   Aoq_halfedge_iterator;
  typedef Aoq::Edge_iterator                       Aoq_edge_iterator;
  typedef Aoq::Face_iterator                       Aoq_face_iterator;

  typedef Aoq::Vertex_const_iterator               Aoq_vertex_const_iterator;
  typedef Aoq::Edge_const_iterator                 Aoq_edge_const_iterator;
  typedef Aoq::Halfedge_const_iterator             Aoq_halfedge_const_iterator;
  typedef Aoq::Face_const_iterator                 Aoq_face_const_iterator;

  typedef Aoq::Halfedge_around_vertex_circulator
    Aoq_halfedge_around_vertex_circulator;
  typedef Aoq::Halfedge_around_vertex_const_circulator
    Aoq_halfedge_around_vertex_const_circulator;

public:
  /*! Constructor */
  Arrangement_on_quadric_geo(Boolean proto = false);

  /*! Destructor */
  virtual ~Arrangement_on_quadric_geo();

  /* Construct the prototype. */
  static Arrangement_on_quadric_geo* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation. */
  virtual void clean();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! */
  virtual Boolean clean_bounding_sphere();

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();

  /*! Determine whether the representation empty. */
  virtual Boolean is_empty() const;

  /*! Set the base quadric.
   * \param quadric (in) a pointer to a quadric.
   */
  void set_base_quadric(Shared_quadric_geo quadric);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Draw a vertex.
   * \param center the vertex center.
   * \param shape
   * \param radius
   * \param delta_angle
   */
  void draw_vertex(Draw_action* action, Vector3f& center,
                   Vertex_shapes shape, Float radius, Float delta_angle);

  //! Vertex shapes.
  static const char* s_vertex_styles[];

  //! Edge shapes.
  static const char* s_edge_shapes[];

  //! The arrangement on quadric representation.
  Arrangement_on_quadric m_aoq;

  //! The base quadric.
  Shared_quadric_geo m_base_quadric;

  //! The intersecting quadrics.
  Quadric_node_list m_quadric_nodes;

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The container prototype.
  static Container_proto* s_prototype;

  /*! Draw the embedding quadric surface.
   * \param action
   */
  virtual void draw_surface(Draw_action* action);

  /*! Draw the arrangement on quadric vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action);

  /*! Draw the arrangement on quadric edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action);

  /*! Set the intersecting quadrics of the arrangement. */
  void insert_quadric(Quadric_geo* quadric);
};

//! \brief constructs the prototype.
inline Arrangement_on_quadric_geo* Arrangement_on_quadric_geo::prototype()
{ return new Arrangement_on_quadric_geo(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Arrangement_on_quadric_geo::create()
{ return new Arrangement_on_quadric_geo(); }

//! \brief sets the intersecting quadrics of the arrangement.
inline void Arrangement_on_quadric_geo::insert_quadric(Quadric_geo* quadric)
{ m_quadric_nodes.push_back(quadric); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Arrangement_on_quadric_geo::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
