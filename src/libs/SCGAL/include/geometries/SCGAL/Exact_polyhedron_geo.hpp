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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EXACT_POLYHEDRON_GEO_HPP
#define SCGAL_EXACT_POLYHEDRON_GEO_HPP

/*! \file
 * A node in the scene graph that maintains a polyhedron.
 * The the point of each vertex and the plane of each facet is in exact
 * representation. Each facet is extended with the normal of the facet-plane
 * in Vector3f (approximate) representation. The later is used for rendering.
 */

#include <CGAL/convex_hull_3.h>
#include <CGAL/enum.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Boundary_set.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Min_sphere.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Exact_polyhedron.hpp"
#include "SCGAL/Exact_polyhedron_geo_builder.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Cull_context;
class Isect_action;
class Coord_array;
class Color_array;
class Draw_action;
class Scene_graph;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Exact_polyhedron_geo : public Boundary_set {
public:
  enum {
    FIRST = Boundary_set::LAST - 1,
    INVALIDATE,
    LAST
  };

  typedef Exact_polyhedron              Polyhedron;
  typedef Exact_kernel                  Kernel;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Exact_polyhedron_geo(Boolean proto = false);

  /*! Destructor. */
  ~Exact_polyhedron_geo();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Exact_polyhedron_geo* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action);

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! */
  virtual bool clean_sphere_bound();

  /*! Write this container.
   * \param formatter (in) the formatter to use; e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Process change of field.
   * \param field_info The information record of the field that changed.
   * Notice that Mesh::coord_changed() is invoked when a new coordinate field
   * is set, while field_changed() is invoked either when a new coordinate
   * field is set or when the content of the current coordinate field changes.
   */
  virtual void field_changed(const Field_info* field_info);

  /*! Set the polyhedron data-structure. */
  void set_polyhedron(Exact_polyhedron& polyhedron);

  /*! Obtain the polyhedron data-structure. */
  Exact_polyhedron& get_polyhedron();

  /*! Obtain the flag that indicates whether to compute the convex hull
   * of the coordinate set.
   */
  Boolean get_convex_hull() const;

  /*! Set the flag that indicates whether to compute the convex hull
   * of the coordinate set.
   */
  void set_convex_hull(Boolean flag);

  /*! Compute the orientation of a point relative to the polyhedron.
   */
  CGAL::Oriented_side oriented_side(const Exact_point_3& p);

  /*! Prints statistics. */
  void print_stat();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Cleans the polyhedron. */
  virtual void clean_polyhedron();

  /*! Cleans the facets. */
  virtual void clean_facets();

  /*! Clears the internal representation. */
  virtual void clear();

  /*! Draws the internal representation. */
  virtual void draw_geometry(Draw_action * action);

  /*! Returns true if the internal representation is empty. */
  virtual bool is_empty() const { return m_polyhedron.empty(); }

private:
  /*! Extracts the approximate point from a polyhedron vertex. */
  struct Convert_approximate_sphere {
    Approximate_sphere_3
    operator()(const Exact_polyhedron::Vertex& vertex) const
    { return to_approximate_sphere(vertex.point()); }
  };

   /*! Convert a point in approximate number type to exact. */
  struct Vector_to_point {
    template <typename Vector3f>
    Exact_point_3 operator()(const Vector3f& vec)
    { return Exact_point_3(vec[0], vec[1], vec[2]); }
  };

   /*! Convert a point in exact number type to approximate. */
  struct Point_to_vector {
    void operator()(Exact_polyhedron::Vertex& vertex)
    { vertex.m_vertex = to_vector3f(vertex.point()); }
  };

  /*! Convert Plane_3 to normal in Vector3f representation. */
  struct Plane_to_normal {
    void operator()(Exact_polyhedron::Facet& facet)
    {
      Vector3f normal = to_vector3f(facet.plane().orthogonal_vector());
      facet.m_normal.set(normal);
      facet.m_normal.normalize();
    }
  };


  /*! Transform a (planar) facet into a plane. */
  struct Plane_equation {
    template <typename Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      typedef typename Facet::Plane_3 Plane;
      return Plane(h->vertex()->point(),
                   h->next()->vertex()->point(),
                   h->next()->next()->vertex()->point());
    }
  };

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The builder. */
  Exact_polyhedron_geo_builder<Exact_polyhedron::HalfedgeDS> m_surface;

  /*! The resulting polyhedron. */
  Exact_polyhedron m_polyhedron;

  /*! Indicates whether to compute the convex hull. */
  bool m_convex_hull;

  /*! Indicates whether the geometry is dirty and thus should be cleaned. */
  Boolean m_dirty_polyhedron;

  /*! Indicates whether the facets are dirty and thus should be cleaned. */
  Boolean m_dirty_facets;

  /*! The time is took to compute the minkowski sum in seconds. */
  float m_time;

  /*! Computes the convex hull of the coordinate set. */
  void convex_hull();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Exact_polyhedron_geo* Exact_polyhedron_geo::prototype()
{ return new Exact_polyhedron_geo(true); }

//! \brief clones.
inline Container* Exact_polyhedron_geo::clone()
{ return new Exact_polyhedron_geo(); }

/*! \brief obtainss the flag that indicates whether to compute the convex hull
 * of the coordinate set.
 */
inline Boolean Exact_polyhedron_geo::get_convex_hull() const
{ return m_convex_hull; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Exact_polyhedron_geo::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
