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
// $Revision: 13463 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_EXACT_NEF_POLYHEDRON_HPP
#define SGAL_EXACT_NEF_POLYHEDRON_HPP

/*! \file
 * A node in the scene graph that maintains a polyhedron.
 * The the point of each vertex and the plane of each facet is in exact
 * representation. Each facet is extended with the normal of the facet-plane
 * in Vector3f (inexact) representation. The later is used for rendering.
 */

#include <CGAL/Cartesian.h>
#include <CGAL/enum.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/HalfedgeDS_face_base.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_3/SNC_indexed_items.h>
#include <CGAL/convex_decomposition_3.h>

#include <string>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_number_type.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Cull_context;
class Isect_action;
class Color_array;
class Draw_action;
class Scene_graph;

class SGAL_SCGAL_DECL Exact_nef_polyhedron : public SGAL::Mesh_set {
public:
  typedef CGAL::Tag_true                                Tag_true;
  typedef CGAL::Tag_false                               Tag_false;

  typedef Exact_number_type                             Number_type;
  typedef CGAL::Cartesian<Number_type>                  Kernel;
  typedef Kernel::Point_3                               Point_3;
  typedef Kernel::Plane_3                               Plane_3;
  typedef Kernel::Vector_3                              Vector_3;

  // Inexact types:
  typedef float                                         Inexact_NT;
  typedef CGAL::Cartesian<Inexact_NT>                   Inexact_kernel;

  typedef CGAL::Min_sphere_of_spheres_d_traits_3<Inexact_kernel,Inexact_NT>
    Min_sphere_traits;
  typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits>
    Min_sphere;
  typedef Min_sphere_traits::Sphere                     Inexact_sphere_3;
  typedef Inexact_kernel::Point_3                       Inexact_point_3;
  typedef Inexact_kernel::Vector_3                      Inexact_vector_3;

  typedef std::vector<Inexact_sphere_3>                 Inexact_sphere_vector;
  typedef Inexact_sphere_vector::const_iterator         Inexact_sphere_iter;

  // A polyhedron vertex extended with an inexact point.
  template <class Refs>
  struct Polyhedron_vertex :
    public CGAL::HalfedgeDS_vertex_base<Refs, Tag_true, Point_3>
  {
    typedef CGAL::HalfedgeDS_vertex_base<Refs, Tag_true, Point_3> Base;
    typedef typename Base::Point                                  Point;
    Inexact_point_3 m_inexact_point;
    Polyhedron_vertex() : Base() {}
    Polyhedron_vertex(const Point& p) : Base(p) {}
    Point& point() { return Base::point(); }
    const Point& point () const { return Base::point(); }

    /*! */
    void set_inexact_point(const Inexact_point_3& point)
    { m_inexact_point = point; }

    /*! */
    const Inexact_point_3& get_inexact_point(void) const
    { return m_inexact_point; }
  };
  
  /*! The polyhedron halfedge extended with a Boolean flag */
  template <class Refs>
  struct Polyhedron_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
    /*! */
    Boolean flag;

    /*! */
    Polyhedron_halfedge() : flag(false) {}
  };

  /*! The polyhedron facet extended with an inexact normal */
  template <class Refs>
  struct Polyhedron_face :
    public CGAL::HalfedgeDS_face_base<Refs, Tag_true, Plane_3>
  {
    Vector3f m_normal;

    /*! Constructor */
    Polyhedron_face() {}

    /*! Constructor */
    Polyhedron_face(const Plane_3& pln) :
      CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane_3>(pln)
    {}
  };

  // An items type using my halfedge.
  struct Polyhedron_items : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Vertex_wrapper {
      typedef Polyhedron_vertex<Refs> Vertex;
    };
    template <class Refs, class Traits>
    struct Halfedge_wrapper {
      typedef Polyhedron_halfedge<Refs> Halfedge;
    };
    template <class Refs, class Traits>
    struct Face_wrapper {
      typedef Polyhedron_face<Refs> Face;
    };
  };
  
  typedef Kernel                                        Polyhedron_traits;
  // typedef CGAL::Polyhedron_3<Polyhedron_traits,Polyhedron_items>
  // Polyhedron;
  typedef CGAL::Polyhedron_3<Polyhedron_traits>         Polyhedron;
  typedef Polyhedron::Facet_iterator
    Polyhedron_facet_iterator;
  typedef Polyhedron::Vertex_iterator                   Vertex_iterator;
  typedef Polyhedron::Point_iterator                    Point_iterator;
  typedef Polyhedron::HalfedgeDS                        HalfedgeDS;
  typedef Polyhedron::Halfedge_around_facet_circulator
    Halfedge_facet_circulator;
  typedef Polyhedron::Facet_const_handle                Facet_const_handle;
  typedef Polyhedron::Facet_handle                      Facet_handle;
  
  typedef Polyhedron::Vertex                            Vertex;
  
  // Nef Polyhedron stuff:
  typedef CGAL::Nef_polyhedron_3<Polyhedron_traits, CGAL::SNC_indexed_items>
    Nef_polyhedron_3;
  typedef Nef_polyhedron_3::Halffacet_const_iterator    Halffacet_iterator;
  typedef Nef_polyhedron_3::Volume_const_iterator       Volume_const_iterator;
  typedef Nef_polyhedron_3::Shell_entry_const_iterator
    Shell_entry_const_iterator;
  
public:
  enum {
    FIRST = Mesh_set::LAST - 1,
    DECOMPOSE_INTO_CONVEX,
    COORD,
    LAST
  };

  /*! Constructor */
  Exact_nef_polyhedron(Boolean proto = false);

  /*! Destructor */
  ~Exact_nef_polyhedron();

  /*! Construct the prototype */
  static Exact_nef_polyhedron* prototype()
  { return new Exact_nef_polyhedron(true); }

  /*! Clone */
  virtual SGAL::Container* clone() { return new Exact_nef_polyhedron(); }

  /*! Draw the Nef polyhedron */
  virtual void draw_geometry(Draw_action * action);

  /*! Clean the representation */
  virtual void clean();
  
  /*! Clear the internal representation */
  virtual void clear();

  /*! Return true if the representation is empty */
  virtual Boolean is_empty() const { return m_nef_polyhedron.is_empty(); }
  
  virtual void cull(SGAL::Cull_context& cull_context);

  virtual void isect(SGAL::Isect_action* action);

  virtual Boolean clean_sphere_bound();

  /*! Set the attributes of this node */
  virtual void set_attributes(SGAL::Element * elem);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();

  /*! Obtain the Nef polyhedron */
  Nef_polyhedron_3& get_nef_polyhedron() { return m_nef_polyhedron; }

  /*! Inserts additional facets, such that each bounded marked volume (the
   * outer volume is unbounded) is subdivided into convex pieces.
   */
  void convex_decomposition()
  { CGAL::convex_decomposition_3(m_nef_polyhedron); }

  /*! Obtain the beginning iterator of the volumes */
  Volume_const_iterator volumes_begin()
  { return m_nef_polyhedron.volumes_begin(); }

  /*! Obtain the past-the-end iterator of the volumes */
  Volume_const_iterator volumes_end()
  { return m_nef_polyhedron.volumes_end(); }

  /* */
  template <typename T_Polyhedron>
  void convert_inner_shell_to_polyhedron(Shell_entry_const_iterator it,
                                         T_Polyhedron & polyhedron)
  { m_nef_polyhedron.convert_inner_shell_to_polyhedron(it, polyhedron); }
  
  /*! Print statistics */
  void print_stat();

#if 0
  /*! Stores a point in inexact number type in the vertex */
  struct Convert_inexact_point {
    void operator()(Vertex& vertex)
    {
      const Point_3& point = vertex.point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      Inexact_point_3 inexact_point(x, y, z);
      vertex.set_inexact_point(inexact_point);
    }
  };
#endif

#if 0
  /*! Transforms a (planar) facet into an inexact normal */
  struct Normal_vector {
    template <class Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      // Facet::Plane_3 is the normal vector type. We assume the
      // CGAL Kernel here and use its global functions.
      Vector_3 normal =
        CGAL::cross_product(h->next()->vertex()->point() - h->vertex()->point(),
                            h->next()->next()->vertex()->point() -
                            h->next()->vertex()->point());
      Number_type sqr_length = normal.squared_length();
      double tmp = CGAL::to_double(sqr_length);
      return normal / CGAL::sqrt(tmp);
    }
  };
#endif
  
  /*! */
  template <class HDS>
  class Build_surface : public CGAL::Modifier_base<HDS> {
  public:
    /*! Constructor */
    Build_surface() {}

    /*! Destructor */
    virtual ~Build_surface() {}

    /*! builds the polyhedron */
    void operator()(HDS& hds) {
      // Postcondition: `hds' is a valid polyhedral surface.
      CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
      typedef typename CGAL::Polyhedron_incremental_builder_3<HDS>::size_type
        size_type;
      const Exact_nef_polyhedron::Shared_coord_array coord_array =
        m_nef_polyhedron->get_coord_array();
      size_type coord_array_size = coord_array->size();
      Uint num_primitives = m_nef_polyhedron->get_num_primitives();
      B.begin_surface(coord_array_size, num_primitives);
      typedef typename HDS::Vertex Vertex;
      typedef typename Vertex::Point Point;
      // Add the points:
      Uint i;
      for (i = 0; i < coord_array_size; ++i) {
        const Vector3f& v = (*coord_array)[i];
        B.add_vertex(Point(v[0], v[1], v[2]));
      }
      
      // Add the faces:
      Uint j = 0;
      for (i = 0; i < num_primitives; ++i) {
        B.begin_facet();
        for (; m_nef_polyhedron->get_coord_index(j) != (Uint) -1; j++) {
          B.add_vertex_to_facet(m_nef_polyhedron->get_coord_index(j));
        }
        ++j;
        B.end_facet();
      }
      B.end_surface();
    }

    /*! sets a pointer to the geometry */
    void set_nef_polyhedron(Exact_nef_polyhedron* p)
    { m_nef_polyhedron = p; }
    
  private:
    /*! The Geometry node */
    Exact_nef_polyhedron* m_nef_polyhedron;
  };

private:
  /*! Transforms a (planar) facet into a plane */
  struct Plane_equation {
    template <class Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      typedef typename Facet::Plane_3 Plane;
      return Plane( h->vertex()->point(),
                    h->next()->vertex()->point(),
                    h->next()->next()->vertex()->point());
    }
  };
  
  /*! Extracts the inexact point from a polyhedron vertex */
  struct Convert_inexact_sphere {
    Inexact_sphere_3 operator()(const Nef_polyhedron_3::Vertex& vertex) const
    {
      const Point_3& point = vertex.point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      Inexact_point_3 inexact_point(x,y,z);
      return Inexact_sphere_3(inexact_point, 0.0f);
    }

    Inexact_sphere_3 operator()(const Polyhedron::Vertex& vertex) const
    {
      const Point_3& point = vertex.point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      Inexact_point_3 inexact_point(x,y,z);
      return Inexact_sphere_3(inexact_point, 0.0f);
    }
  };

  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static SGAL::Container_proto* s_prototype;

  /*! The builder */
  Build_surface<HalfedgeDS> m_surface;

  /*! Indicates whether the intermediate polyhedron has been built */
  Boolean m_dirty_polyhedron;
  
  /*! The resulting polyhedron */
  Polyhedron m_polyhedron;

  /*! The Nef_3 representation */
  Nef_polyhedron_3 m_nef_polyhedron;
  
  /*! The time is took to compute the minkowski sum in seconds */
  float m_time;
  
protected:
  /*! Clean the polyhedron data structure */
  void clean_polyhedron();

  /*! Calculate the polyhedron spere-bound */
  Boolean clean_sphere_bound_polyhedron();
  
  /*! Draw the intermediate polyhedron (for debugging purpose) */
  void draw_polyhedron(Draw_action* action);  

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }
};

SGAL_END_NAMESPACE

#endif
