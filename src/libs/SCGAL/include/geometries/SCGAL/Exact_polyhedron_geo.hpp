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
class Coord_array;
class Color_array;
class Draw_action;
class Scene_graph;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Exact_polyhedron_geo Exact_polyhedron_geo.hpp
 * Exact_polyhedron_geo represents a node in the scene graph that maintains a
 * polyhedron defined by exact (rational) coordinates. A halfedge data
 * structure is used by the polyhedron to maintain the incidence relations
 * between the polyhedron cells. The cells of the polyhedron are extended to
 * expedite the building of the polyhedron. A polyhedron can be built in
 * several ways:
 * 1. Given a coordinate array and a coordinate indices array a polyhedral
 *    surface builder is applied to construct the polyhedron.
 * 2. Given a coordinate array the convex hull of the coordinate is computed.
 * 3. The polyhedron is set by an external agent.
 *
 * The Exact_polyhedron_geo derives from the Boundary_set representation. The
 * later provides the mechanism for efficiently drawing the polyhedron.
 *
 * Exact_polyhedron_geo respects the creaseAngle field; it computes the
 * normals of the vertices based on the creaseAngle value (similar to
 * IndexedFaceSet, which uses a polyhedron with inexact coordinates). As such
 * it extends the drawing capabilities of the Boundary_set base class.
 */

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

  /*! Clean the representation. */
  virtual void clean();

  /*! Determine whether the representation is empty.
   * \return true if the representation is empty and false otherwise.
   */
  virtual Boolean is_empty() const;

  /*! Calculate the sphere bound.
   */
  virtual Boolean clean_sphere_bound();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! Clears the internal representation. */
  virtual void clear();

  /*! Process change of field.
   * \param field_info The information record of the field that changed.
   * Notice that Mesh::coord_changed() is invoked when a new coordinate field
   * is set, while field_changed() is invoked either when a new coordinate
   * field is set or when the content of the current coordinate field changes.
   */
  virtual void field_changed(const Field_info* field_info);

  /*! Set the polyhedron data-structure. */
  void set_polyhedron(Exact_polyhedron& polyhedron);

  /*! Obtain the polyhedron data-structure.
   * \param with_planes (in) indicates whether to clean the planes (as well
   *        as the polyhedron itself.
   * \return the polyhedron data-structure.
   */
  const Exact_polyhedron& get_polyhedron(Boolean with_planes = false);

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
  /*! A functor that calculates the normal of a given (half)edge. */
  struct Edge_normal_calculator {
    /*! The crease angle. */
    Float m_crease_angle;

    /*! Indicates whether all edges are creased. */
    Boolean m_creased;

    /*! Indicates whether all edges are smooth. */
    Boolean m_smooth;

    /*! Constructor. */
    Edge_normal_calculator(Float crease_angle) :
      m_crease_angle(crease_angle),
      m_creased(true),
      m_smooth(true)
    {}

    /*! Calculate the normal of a given (half)edge.
     * \param edge (in) the given (half)edge.
     */
    template <typename Halfedge>
    void operator()(Halfedge& edge)
    {
      if (edge.is_border_edge()) return;
      Vector3f normal1 = edge.facet()->m_normal;
      Vector3f normal2 = edge.opposite()->facet()->m_normal;
      Float angle = arccosf(normal1.dot(normal2));  // inner product
      if (abs(angle) > m_crease_angle) {
        edge.m_creased = true;
        edge.opposite()->m_creased = true;
        m_smooth = false;
        return;
      }
      edge.m_creased = false;
      edge.opposite()->m_creased = false;
      m_creased = false;
    }
  };

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Clean the polyhedron. */
  virtual void clean_polyhedron();

  /*! Calculate the normals in case they are invalidated.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  virtual void clean_normals();

  /*! Obtain the ith 3D coordinate.
   */
  virtual const Vector3f& get_coord_3d(Uint i) const;

  /*! Clean the polyhedron edges.
   */
  virtual void clean_polyhedron_edges();

  /*! Clean the polyhedron facets.
   */
  virtual void clean_polyhedron_facets();

  /*! Clean the coordinate array. */
  void clean_coord_array();

  /*! Clean the coordinate indices. */
  void clean_coord_indices();

  /*! Determine whether the polyhedron representation is empty.
   */
  bool is_polyhedron_empty() const;

  /*! Calculate multiple normals per vertex for all vertices.
   * If the angle between the geometric normals of two adjacent faces is less
   * than the crease angle, calculate the normals so that the facets are
   * smooth-shaded across the edge. Otherwise, calculate the normals so that
   * the facets are faceted.
   */
  void calculate_multiple_normals_per_vertex();

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

  // /*! Convert a point in exact number type to approximate. */
  // struct Point_to_vector {
  //   void operator()(Exact_polyhedron::Vertex& vertex)
  //   { vertex.m_vertex = to_vector3f(vertex.point()); }
  // };

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

  /*! Search each vertex in the polyhedron in the given range, compute its
   * index in the range, and assign it to polyhedron-vertex record.
   */
  template <typename InputIterator>
  void clean_vertices(InputIterator begin, InputIterator end)
  {
    for (auto vit = m_polyhedron.vertices_begin();
         vit != m_polyhedron.vertices_end(); ++vit)
    {
      // find the index
      Uint index = 0;
      InputIterator cit;
      for (cit = begin; cit != end; ++cit, ++index)
        if (vit->point() == *cit) break;
      SGAL_assertion(cit != end);
      vit->m_index = index;
    }
  }

  /*! Computes the convex hull of the coordinate set. */
  void convex_hull();

  /*! The builder. */
  Exact_polyhedron_geo_builder<Exact_polyhedron::HalfedgeDS> m_surface;

  /*! The resulting polyhedron. */
  Exact_polyhedron m_polyhedron;

  /*! Indicates whether to compute the convex hull. */
  bool m_convex_hull;

  /*! Indicates whether the polyhedron is dirty and thus should be cleaned. */
  Boolean m_dirty_polyhedron;

  /*! Indicates whether the polyhedron edges are dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_polyhedron_edges;

  /*! Indicates whether the polyhedron facets are dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_polyhedron_facets;

  /*! Indicates whether the coordinate array is dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_coord_array;

  /*! The time is took to compute the minkowski sum in seconds. */
  float m_time;

  /*! Indicates whether all edges are creased. */
  Boolean m_creased;

  /*! Indicates whether all edges are smooth. */
  Boolean m_smooth;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
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

//! \brief determines whether the polyhedron representation is empty.
inline bool Exact_polyhedron_geo::is_polyhedron_empty() const
{ return m_polyhedron.empty(); }

SGAL_END_NAMESPACE

#endif
