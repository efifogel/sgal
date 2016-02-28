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

#include <CGAL/basic.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Boundary_set.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Epec_polyhedron.hpp"
#include "SGAL/Epic_coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Min_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class formatter;
class Container_proto;
class Draw_action;
class Isect_action;
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
    VOLUME,
    SURFACE_AREA,
    LAST
  };

  typedef Epec_polyhedron              Polyhedron;
  typedef Epec_kernel                  Kernel;

  /*! Constructor.
   * \param[in] proto determines whether to construct a prototype.
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
  Float* volume_handle(const Field_info*) { return &m_volume; }
  Float* surface_area_handle(const Field_info*) { return &m_surface_area; }
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  /*! Draw the polygons.
   */
  virtual void draw(Draw_action* action);

  /*! Export this container.
   * \param[in] formatter The formatter to use for the exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Draw the polygons for selection.
   */
  virtual void isect(Isect_action* action);

  /*! Clean the sphere bound.
   */
  virtual void clean_bounding_sphere();

  /*! Clean the coordinate array and the coordinate indices.
   */
  virtual void clean_coords();

  /*! Obtain the coordinate array.
   * \return the coordinate array.
   */
  virtual Shared_coord_array get_coord_array();

  /*! Clear the coordinates.
   */
  void clear_coord_array();

  /*! Determine whether the coordinates have been invalidated, and thus been
   * must be cleaned.
   */
  Boolean is_dirty_coord_array() const;

  /*! Clean the normal array and the normal indices.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  virtual void clean_normals();

  /*! Clean the polyhedron. */
  virtual void clean_polyhedron();

  /*! Clears the polyhedron.
   */
  virtual void clear_polyhedron();

  /*! Determine whether the polyhedron have been invalidated, and thus been
   * must be cleaned.
   */
  Boolean is_dirty_polyhedron() const;

  /*! Set the polyhedron data-structure. */
  void set_polyhedron(Epec_polyhedron& polyhedron);

  /*! Obtain the polyhedron data-structure.
   * \param[in] with_planes indicates whether to clean the planes (as well
   *            as the polyhedron itself.
   * \return the polyhedron data-structure.
   */
  const Epec_polyhedron& get_polyhedron(Boolean with_planes = false);

  /*! Determine whether the polyhedron representation is empty.
   */
  bool is_polyhedron_empty() const;

  /*! Determine whether there are no border edges.
   * \return true if there are no border edges, and false otherwise.
   */
  Boolean is_closed();

  /*! Obtain the number of border edges.
   */
  size_t get_number_of_border_edges();

  /*! Initialize the border edges.
   * \param[in] value the Boolean value to initialize the general-purpose flags
   *                  of the border edges.
   */
  void init_border_edges(Boolean value = false);

  /// \name Change Recators
  //@{
  /*! Respond to a change in the coordinate array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_content_changed(const Field_info* field_info);
  //@}

  /*! Obtain the flag that indicates whether to compute the convex hull
   * of the coordinate set.
   */
  Boolean is_convex_hull() const;

  /*! Set the flag that indicates whether to compute the convex hull
   * of the coordinate set.
   */
  void set_convex_hull(Boolean flag);

  /*! Compute the orientation of a point relative to the polyhedron.
   */
  CGAL::Oriented_side oriented_side(const Epec_point_3& p);

  /*! Compute the volume of the convex hull of the polyhedron.
   */
  Float volume_of_convex_hull();

  /*! Compute the volume of the polyhedron.
   */
  Float volume();

  /*! Compute the surface area of the polyhedron.
   */
  Float surface_area();

  /*! Determine wheather the mesh is consistent.
   * \return true if the the mesh is consistent and false otherwise.
   */
  Boolean is_consistent();

  /*! Determine whether the mesh has singular vertices.
    * \return true if the the mesh has singular vertices and false otherwise.
   */
  Boolean has_singular_vertices();

  /*! Set the flag that determine whether the mesh has singular vertices.
   * \param[in] flag the flag that determine whether the mesh has singular
   *            vertices.
   */
  Boolean set_has_singular_vertices(Boolean flag);

  /*! Print statistics.
   */
  void print_stat();

  /*! Orient polygon soup visitor. */
  class Orient_polygon_soup_visitor : public boost::static_visitor<Boolean> {
  private:
    const Shared_coord_array m_coord_array;

  public:
    //! Construct
    Orient_polygon_soup_visitor(Shared_coord_array coord_array) :
      m_coord_array(coord_array) {}

    template <typename Indices>
    Boolean operator()(Indices& indices) const
    {
      auto* field_info = m_coord_array->get_field_info(Coord_array::POINT);
      auto exact_coords =
        boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
      if (exact_coords) {
        auto& points = *(exact_coords->array_handle(field_info));
        auto has_singular_vertices =
          !CGAL::Polygon_mesh_processing::orient_polygon_soup(points, indices);
        return has_singular_vertices;
      }
      auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
      if (coords) {
        auto& points = *(coords->array_handle(field_info));
        auto has_singular_vertices =
          !CGAL::Polygon_mesh_processing::orient_polygon_soup(points, indices);
        return has_singular_vertices;
      }
      SGAL_error();
    }

    Boolean operator()(Flat_indices& indices) const
    { SGAL_error(); return false; }
  };

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
     * \param[in] edge the given (half)edge.
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

  /*! Obtain the ith 3D coordinate.
   */
  virtual const Vector3f& get_coord_3d(Uint i) const;

  /*! Clean the polyhedron edges.
   */
  virtual void clean_polyhedron_edges();

  /*! Clean the polyhedron facets.
   */
  virtual void clean_polyhedron_facets();

  /*! Calculate multiple normals per vertex for all vertices.
   * If the angle between the geometric normals of two adjacent faces is less
   * than the crease angle, calculate the normals so that the facets are
   * smooth-shaded across the edge. Otherwise, calculate the normals so that
   * the facets are faceted.
   */
  void calculate_multiple_normals_per_vertex();

  /*! Clean (compute) the volume.
   */
  void clean_volume();

  /*! Clean (compute) the surface area.
   */
  void clean_surface_area();

  /*! Indicates whether to compute the convex hull. */
  bool m_convex_hull;

  /*! The volume of the polyhedron. */
  Float m_volume;

  /*! The surface area of the polyhedron. */
  Float m_surface_area;

  /*! Indicates whether the volume is dirty and thus must be cleaned. */
  Boolean m_dirty_volume;

  /*! Indicates whether the surface area is dirty and thus must be cleaned. */
  Boolean m_dirty_surface_area;

  /*! Indicates whether the coordinate array is dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_coord_array;

  /*! The resulting polyhedron. */
  Epec_polyhedron m_polyhedron;

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

  /*! Indicates wheather the mesh is consistent.
   * \return true if the mesh is consistent and false otherwise. An mesh is
   * inconsistent iff the construction of the polyderal surface failed.
   */
  Boolean m_consistent;

  /*! Indicates wheather the mesh has_singular vertices. */
  Boolean m_has_singular_vertices;

private:
  /*! Extracts the approximate point from a polyhedron vertex. */
  struct Convert_approximate_sphere {
    Inexact_sphere_3
    operator()(const Epec_polyhedron::Vertex& vertex) const
    { return to_inexact_sphere(vertex.point()); }
  };

   /*! Convert a point in approximate number type to exact. */
  struct Vector_to_point {
    template <typename Vector3f>
    Epec_point_3 operator()(const Vector3f& vec)
    { return Epec_point_3(vec[0], vec[1], vec[2]); }
  };

  // /*! Convert a point in exact number type to approximate. */
  // struct Point_to_vector {
  //   void operator()(Epec_polyhedron::Vertex& vertex)
  //   { vertex.m_vertex = to_vector3f(vertex.point()); }
  // };

  /*! Convert Plane_3 to normal in Vector3f representation. */
  struct Plane_to_normal {
    void operator()(Epec_polyhedron::Facet& facet)
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

  /*! Clean the coordinate indices.
   */
  void clean_coord_indices();

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
inline Boolean Exact_polyhedron_geo::is_convex_hull() const
{ return m_convex_hull; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Exact_polyhedron_geo::get_tag() const
{ return s_tag; }

/*! \brief determines whether the coordinates have been invalidated,
 * and thus been must be cleaned.
 */
inline Boolean Exact_polyhedron_geo::is_dirty_coord_array() const
{ return m_dirty_coord_array; }

/*! \brief determine whether the polyhedron have been invalidated,
 * and thus been must be cleaned.
 */
inline Boolean Exact_polyhedron_geo::is_dirty_polyhedron() const
{ return m_dirty_polyhedron; }

//! \brief determines whether the polyhedron representation is empty.
inline bool Exact_polyhedron_geo::is_polyhedron_empty() const
{ return m_polyhedron.empty(); }

//! brief sets the flag that determine whether the mesh has singular vertices.
inline Boolean Exact_polyhedron_geo::set_has_singular_vertices(Boolean flag)
{ m_has_singular_vertices = flag; }

SGAL_END_NAMESPACE

#endif
