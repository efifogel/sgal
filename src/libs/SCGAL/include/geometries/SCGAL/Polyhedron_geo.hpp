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
// $Revision: 14182 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node in the scene graph that maintains a planar map.
 */

#ifndef SGAL_POLYHEDRON_GEO_HPP
#define SGAL_POLYHEDRON_GEO_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#if !defined(USE_MIN_SPHERE)
#include <CGAL/Min_sphere_of_spheres_d.h>
#else
#include <CGAL/Min_sphere_d.h>
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Cull_context;
class Isect_action;
class Coord_array;
class Draw_action;
class Scene_graph;

class Polyhedron_geo : public SGAL::Mesh_set {
public:
  // A halfedge type with a flag member variable.
  template <class Refs>
  struct My_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
    bool flag;
    My_halfedge() : flag(false) {}
  };

  // An items type using my halfedge.
  struct My_items : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Halfedge_wrapper {
      typedef My_halfedge<Refs> Halfedge;
    };
  };
  
  typedef CGAL::Cartesian<float>                         Kernel;
  typedef Kernel::Point_3                                Point;
  typedef Kernel::Vector_3                               Vector;
  typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
  // typedef CGAL::HalfedgeDS_vector                        My_hds;
  typedef CGAL::Polyhedron_3<Traits,My_items>            Polyhedron;
  typedef Polyhedron::Facet_iterator                     Facet_iterator;
  typedef Polyhedron::Vertex_iterator                    Vertex_iterator;
  typedef Polyhedron::Halfedge_around_facet_circulator   Halfedge_facet_circ;
  typedef Polyhedron::HalfedgeDS                         HalfedgeDS;
  typedef Polyhedron::Halfedge_around_facet_circulator
        Halfedge_facet_circulator;

  typedef std::vector<int>                      Coord_index_vector;
  typedef Coord_index_vector::const_iterator    Coord_index_iter;

  // Min sphere stuff:
#if !defined(USE_MIN_SPHERE)
  typedef CGAL::Min_sphere_of_spheres_d_traits_3<Kernel,float>
    Min_sphere_traits;
  typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits>
    Min_sphere;
  typedef Min_sphere_traits::Sphere                     Sphere_3;
  typedef std::vector<Sphere_3>                         Sphere_vector;
  typedef Sphere_vector::const_iterator                 Sphere_iter;
#else
  typedef CGAL::Optimisation_d_traits_3<Polyhedron_geo::Kernel>
    Min_sphere_traits;
  typedef CGAL::Min_sphere_d<Min_sphere_traits>         Min_sphere;
#endif

  /*! Convert the point to a sphere */
  struct Extract_sphere {
    /*! */
    Sphere_3 operator()(const Point & point) const
    { return Sphere_3(point, 0); }
  };
  
private:
  /*! */
  struct Normal_vector {
    template <class Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      // Facet::Plane_3 is the normal vector type. We assume the
      // CGAL Kernel here and use its global functions.
      Vector normal = CGAL::cross_product(h->next()->vertex()->point() -
                                          h->vertex()->point(),
                                          h->next()->next()->vertex()->point() -
                                          h->next()->vertex()->point());
      return normal / CGAL::sqrt(normal.squared_length());
    }
  };

  /*! */
  template <class HDS>
  class Build_surface : public CGAL::Modifier_base<HDS> {
  public:
    /*! Constructor */
    Build_surface() : m_polyhedron(NULL) {}

    /*! Destructor */
    virtual ~Build_surface() {}

    /*! builds the polyhedron */
    void operator()(HDS& hds) {
      SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON,
                     "Build_surface::operator() start\n");
      // Postcondition: `hds' is a valid polyhedral surface.
      CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
      typedef typename CGAL::Polyhedron_incremental_builder_3<HDS>::size_type
        size_type;
      const Coord_array* coord_array = m_polyhedron->get_coord_array();
      size_type coord_array_size = coord_array->size();
      unsigned int num_facets = m_polyhedron->get_num_primitives();
      SGAL_TRACE_CODE(SGAL::Trace::POLYHEDRON,
                      std::cout << "Vertices: " << coord_array_size
                      << ", Faces: " << num_facets << std::endl;);
      B.begin_surface(coord_array_size, num_facets);
      typedef typename HDS::Vertex Vertex;
      typedef typename Vertex::Point Point;
      // Add the points:
      Uint i;
      for (i = 0; i < coord_array_size; i++) {
        const Vector3f& v = (*coord_array)[i];
        B.add_vertex(Point(v[0], v[1], v[2]));
      }
      
      // Add the faces:
      Uint j = 0;
      for (i = 0; i < num_facets; i++) {
        B.begin_facet();
        for (; m_polyhedron->get_coord_index(j) != (Uint) -1; j++) {
          B.add_vertex_to_facet(m_polyhedron->get_coord_index(j));
        }
        j++;
        B.end_facet();
      }
      B.end_surface();
      SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON,
                     "Build_surface::operator() end\n");
    }

    /*! sets a pointer to the geometry */
    void set_polyhedron(Polyhedron_geo* p)
    { m_polyhedron = p; }

  private:
    /*! The Geometry node */
    Polyhedron_geo* m_polyhedron;
  };
  
public:
  /*! Fields */
  enum {
    FIRST = Mesh_set::LAST - 1,
    LAST
  };

  /*! Constructor */
  Polyhedron_geo(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  ~Polyhedron_geo();

  /*! Constructs the prototype */
  static Polyhedron_geo* prototype() { return new Polyhedron_geo(SGAL_TRUE); }

  /*! Clone */
  virtual SGAL::Container* clone() { return new Polyhedron_geo(); }

  virtual void cull(SGAL::Cull_context& cull_context);
  virtual void isect(SGAL::Isect_action* action);
  virtual bool clean_sphere_bound();

  /*! Set the attributes of this node */
  virtual void set_attributes(SGAL::Element* elem);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual SGAL::Container_proto* get_prototype();

  /*! Obtain the polyhedron data-structure */
  Polyhedron& get_polyhedron();
  
protected:
  /*! The actual polyhedron object */
  Polyhedron m_polyhedron;

  /*! Clean the representation */
  virtual void clean();

  /*! Clear the internal representation */
  virtual void clear();

  /*! Is the internal representation is empty? */
  virtual bool is_empty() const { return m_polyhedron.empty(); }

  /*! Draw the internal representation */
  virtual void draw_geometry(SGAL::Draw_action* action);

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static SGAL::Container_proto* m_prototype;

  /*! The builder */
  Build_surface<HalfedgeDS> m_surface;
};

SGAL_END_NAMESPACE

#endif
