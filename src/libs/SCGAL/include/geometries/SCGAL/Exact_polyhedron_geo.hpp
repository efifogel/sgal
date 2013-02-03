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
// $Source$
// $Revision: 13487 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_EXACT_POLYHEDRON_GEO_HPP
#define SGAL_EXACT_POLYHEDRON_GEO_HPP

/*! \file
 * A node in the scene graph that maintains a polyhedron.
 * The the point of each vertex and the plane of each facet is in exact
 * representation. Each facet is extended with the normal of the facet-plane
 * in Vector3f (approximate) representation. The later is used for rendering.
 */

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>

#if defined(CGAL_USE_LEDA)
#include <CGAL/leda_rational.h>
#endif
#include <CGAL/convex_hull_3.h>
#include <CGAL/enum.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Mesh_set.hpp"

#include "SCGAL/Min_sphere.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Polyhedron_geo_builder.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Cull_context;
class Isect_action;
class Coord_array;
class Color_array;
class Draw_action;
class Scene_graph;

class Exact_polyhedron_geo : public SGAL::Mesh_set {
public:
  typedef Exact_number_type                             Number_type;
  typedef Exact_kernel                                  Kernel;
  typedef Kernel::Point_3                               Point_3;
  typedef Kernel::Plane_3                               Plane_3;
  typedef Kernel::Vector_3                              Vector_3;

  /*! An extended vertex */
  template <class Refs>
  struct My_vertex :
    public CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point_3>
  {
    Vector3f m_vertex;
    My_vertex() {}
    My_vertex(const Point_3& p) :
      CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point_3>(p)
    {}
  };

  /*! An extended halfedge */
  template <class Refs>
  struct My_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
    bool m_flag;
    My_halfedge() : m_flag(false) {}
  };

  /*! A face type with the face normal data member */
  template <class Refs>
  struct My_face :
    public CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane_3>
  {
    Vector3f m_normal;
    My_face() {}
    My_face(const Plane_3& pln) :
      CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane_3>(pln)
    {}
  };

  /*! An items type using extended features */
  struct My_items : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Vertex_wrapper {
      typedef My_vertex<Refs> Vertex;
    };

    template <class Refs, class Traits>
    struct Halfedge_wrapper {
      typedef My_halfedge<Refs> Halfedge;
    };

    template <class Refs, class Traits>
    struct Face_wrapper {
      typedef My_face<Refs> Face;
    };
  };
  
  typedef Kernel                                        Traits;
  typedef CGAL::Polyhedron_3<Traits, My_items>          Polyhedron;
  typedef Polyhedron::Vertex                            Vertex;
  typedef Polyhedron::Facet                             Facet;
  typedef Polyhedron::Vertex_iterator                   Vertex_iterator;
  typedef Polyhedron::Facet_iterator                    Facet_iterator;
  typedef Polyhedron::HalfedgeDS                        HalfedgeDS;

public:
  enum {
    FIRST = Mesh_set::LAST - 1,
    INVALIDATE,
    LAST
  };

  /*! Constructor */
  Exact_polyhedron_geo(Boolean proto = false);

  /*! Destructor */
  ~Exact_polyhedron_geo();

  /*! Construct the prototype */
  static Exact_polyhedron_geo* prototype()
  { return new Exact_polyhedron_geo(true); }

  /*! Clone */
  virtual SGAL::Container* clone() { return new Exact_polyhedron_geo(); }

  virtual void cull(SGAL::Cull_context& cull_context);
  virtual void isect(SGAL::Isect_action* action);
  virtual bool clean_sphere_bound();

  /*! Set the attributes of this node */
  virtual void set_attributes(SGAL::Element* elem);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual SGAL::Container_proto* get_prototype();

 /*! Obtain the polyhedron data-structure.
   */
  const Polyhedron& get_polyhedron() const { return m_polyhedron; }
  Polyhedron& get_polyhedron() { return m_polyhedron; }

  /*! Obtains the flag that indicates whether to compute the convex hull
   * of the coordinate set.
   */
  bool get_convex_hull() const { return m_convex_hull; }

  /*! Sets the flag that indicates whether to compute the convex hull
   * of the coordinate set
   */
  void set_convex_hull(bool flag) { m_convex_hull = flag; }  

  /*! Computes the orientation of a point relative to the polyhedron. */
  CGAL::Oriented_side oriented_side(const Point_3& p);
  
  /*! Prints statistics. */
  void print_stat();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Cleans the representation. */
  virtual void clean();

  /*! Clears the internal representation. */
  virtual void clear();

  /*! Draws the internal representation. */
  virtual void draw_geometry(SGAL::Draw_action * action);

  /*! Returns true if the internal representation is empty. */
  virtual bool is_empty() const { return m_polyhedron.empty(); }

private:
  /*! Extracts the approximate point from a polyhedron vertex. */
  struct Convert_approximate_sphere {
    Approximate_sphere_3 operator()(const Vertex & vertex) const
    { return to_approximate_sphere(vertex.point()); }
  };

   /*! Convert a point in approximate number type to exact. */
  struct Vector_to_point {
    template <class Vector3f>
    Point_3 operator()(const Vector3f & vec)
    { return Point_3(vec[0], vec[1], vec[2]); }
  };

   /*! Convert a point in exact number type to approximate. */
  struct Point_to_vector {
    void operator()(Vertex& vertex)
    { vertex.m_vertex = to_vector3f(vertex.point()); }
  };

  /*! Convert Plane_3 to normal in Vector3f representation */
  struct Plane_to_normal {
    void operator()(Facet& facet)
    {
      Vector_3 normal = facet.plane().orthogonal_vector();
      float x = static_cast<float>(CGAL::to_double(normal.x()));
      float y = static_cast<float>(CGAL::to_double(normal.y()));
      float z = static_cast<float>(CGAL::to_double(normal.z()));
      facet.m_normal.set(x,y,z);
      facet.m_normal.normalize();
    }
  };

  
  /*! Transform a (planar) facet into a plane */
  struct Plane_equation {
    template <class Facet>
    typename Facet::Plane_3 operator()(Facet & f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      typedef typename Facet::Plane_3 Plane;
      return Plane( h->vertex()->point(),
                    h->next()->vertex()->point(),
                    h->next()->next()->vertex()->point());
    }
  };
  
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static SGAL::Container_proto* s_prototype;

  /*! The builder */
  Polyhedron_geo_builder<HalfedgeDS> m_surface;

  /*! The resulting polyhedron */
  Polyhedron m_polyhedron;
  
  /*! Indicates whether to compute the convex hull */
  bool m_convex_hull;

  /*! The time is took to compute the minkowski sum in seconds */
  float m_time;

  /*! Computes the convex hull of the coordinate set */
  void convex_hull();
};

SGAL_END_NAMESPACE

#endif
