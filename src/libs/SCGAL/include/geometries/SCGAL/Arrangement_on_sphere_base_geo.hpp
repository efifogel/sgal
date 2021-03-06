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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#ifndef SCGAL_ARRANGEMENT_ON_SPHERE_BASE_GEO_HPP
#define SCGAL_ARRANGEMENT_ON_SPHERE_BASE_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"
#include "SGAL/Epec_normal_array.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_surface_geo.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */
class SGAL_SCGAL_DECL Arrangement_on_sphere_base_geo :
  public Arrangement_on_surface_geo
{
public:
  // Shared pointers
  typedef boost::shared_ptr<Coord_array>          Shared_coord_array;
  typedef boost::shared_ptr<Normal_array>         Shared_normal_array;
  typedef boost::shared_ptr<Epec_coord_array_3d> Shared_exact_coord_array_3d;
  typedef boost::shared_ptr<Epec_normal_array>   Shared_exact_normal_array;

  enum {
    FIRST = Arrangement_on_surface_geo::LAST - 1,
    COORD_ARRAY,
    CURVE_INDEX_ARRAY,
    X_MONOTONE_CURVE_INDEX_ARRAY,
    POINT_INDEX_ARRAY,
    POINT_LOCATION_INDEX_ARRAY,
    LAST
  };

public:
  /*! Constructor. */
  Arrangement_on_sphere_base_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Arrangement_on_sphere_base_geo();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! Clean the bounding sphere of the arrangement node. */
  virtual void clean_bounding_sphere();

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear() = 0;

  /*! Set the coordinate array. */
  void set_coord_array(Shared_coord_array coord_array);

  /*! Obtain the coordinate array. */
  Shared_coord_array get_coord_array() const;

  /*! Set the normal array. */
  void set_normal_array(Shared_normal_array normal_array);

  /*! Obtain the normal array. */
  Shared_normal_array get_normal_array() const;

  /*! Obtain the x-monotone curve index array. */
  const std::vector<Uint>& get_x_monotone_curve_indices() const;

  /*! Obtain the curve index array. */
  const std::vector<Uint>& get_curve_indices() const;

  /*! Obtain the normal index array. */
  const std::vector<Uint>& get_normal_indices() const;

  /*! Obtain the point index array. */
  const std::vector<Uint>& get_point_indices() const;

  /*! Obtain the ith x-monotone curve index. */
  Uint get_x_monotone_curve_index(Uint i) const;

  /*! Obtain the ith curve index. */
  Uint get_curve_index(Uint i) const;

  /*! Obtain the ith normal index. */
  Uint get_normal_index(Uint i) const;

  /*! Obtain the ith point index. */
  Uint get_point_index(Uint i) const;

  /*! Obtain the point location index array. */
  const std::vector<Uint>& get_point_location_indices() const;

  /*! Obtain the ith point index. */
  Uint get_point_location_index(Uint i) const;

  /*! Draw an arrangement on surface vertex.
   * \param action
   * \param center the vertex center.
   */
  virtual void draw_aos_vertex(Draw_action* action, Vector3f& center);

  /*! Draw an arrangement on surface isolated vertex.
   * \param action
   * \param center the vertex center.
   */
  virtual void draw_aos_isolated_vertex(Draw_action* action,
                                        Vector3f& center);

  /*! Draw an arrangement on surface boundary_vertex.
   * \param action
   * \param center the vertex center.
   */
  virtual void draw_aos_boundary_vertex(Draw_action* action,
                                        Vector3f& center);

  /*! Draw an arrangement on surface edge.
   * \param action
   * \param source the edge source point.
   * \param target the edge target point.
   * \param normal the normal to the plane containing the edge.
   */
  virtual void draw_aos_edge(Draw_action* action,
                             Vector3f& source, Vector3f& target,
                             Vector3f& normal);


protected:
  /*! Draw the vertices of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_vertices(Aos* aos, Draw_action* action)
  {
    typedef typename Aos::Vertex_const_iterator Vertex_const_iterator;
    typedef typename Aos::Geometry_traits_2     Geom_traits;
    typedef typename Geom_traits::Point_2       Point;

    Vertex_const_iterator vi;
    for (vi = aos->vertices_begin(); vi != aos->vertices_end(); ++vi) {
      if (vi->is_isolated()) continue;
      Vector3f center = to_vector3f(vi->point());
      center.normalize();
      draw_aos_vertex(action, center);
    }
  }

  /*! Draw the isolated vertices of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_isolated_vertices(Aos* aos, Draw_action* action)
  {
    typedef typename Aos::Vertex_const_iterator Vertex_const_iterator;
    typedef typename Aos::Geometry_traits_2     Geom_traits;
    typedef typename Geom_traits::Point_2       Point;

    Vertex_const_iterator vi;
    for (vi = aos->vertices_begin(); vi != aos->vertices_end(); ++vi) {
      if (!(vi->is_isolated())) continue;
      Vector3f center = to_vector3f(vi->point());
      center.normalize();
      draw_aos_isolated_vertex(action, center);
    }
  }

  /*! Draw the edges of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_edges(Aos* aos, Draw_action* action)
  {
    typedef typename Aos::Edge_const_iterator           Edge_const_iterator;
    typedef typename Aos::Geometry_traits_2             Geom_traits;
    typedef typename Geom_traits::Point_2               Point;
    typedef typename Geom_traits::X_monotone_curve_2    X_monotone_curve;

    const Geom_traits* traits = aos->geometry_traits();
    auto cmp_endpoints = traits->compare_endpoints_xy_2_object();
    auto ctr_min_vertex = traits->construct_min_vertex_2_object();
    auto ctr_max_vertex = traits->construct_max_vertex_2_object();
    auto ctr_cross_product = traits->construct_cross_product_vector_3_object();
    Edge_const_iterator hei;
    for (hei = aos->edges_begin(); hei != aos->edges_end(); ++hei) {
      const X_monotone_curve& curve = hei->curve();
      /*! The following code uses member functions of the curve type that are
       * not in the concept, and thus are not guaranteed to be supported.
       * However, since there is only one traits that supports geodesics on
       * the sphere, and the curve type of this traits does support these
       * member functions, the code uses them after all, as they are more
       * efficient. For different traits classes use instead:
       */
      // auto flag = (cmp_endpoints(curve) == CGAL::SMALLER);
      // const auto& source =
      //   (flag) ? ctr_min_vertex(curve) : ctr_max_vertex(curve);
      // const auto& target =
      //   (flag) ? ctr_max_vertex(curve) : ctr_min_vertex(curve);
      // auto normal = ctr_cross_product(source.vector(), target.vector());
      // Vector3f src = to_vector3f(source);
      // Vector3f trg = to_vector3f(target);
      // Vector3f nrm = to_vector3f(normal);
      // nrm.normalize();

      Vector3f src = to_vector3f(curve.source());
      Vector3f trg = to_vector3f(curve.target());
      Vector3f nrm = to_vector3f(curve.normal());
      src.normalize();
      trg.normalize();
      draw_aos_edge(action, src, trg, nrm);
    }
  }

  /*! An array of coordinates. */
  Shared_coord_array m_coord_array;

  /*! An array of normals. */
  Shared_normal_array m_normal_array;

  /*! An array of indices into the coordinate array for general curves. */
  std::vector<Uint> m_curve_indices;

  /*! An array of indices into the coordinate array for x-monotone curves. */
  std::vector<Uint> m_x_monotone_curve_indices;

  /*! An array of indices into the coordinate array for isolated points. */
  std::vector<Uint> m_point_indices;

  /*! An array of indices into the coordinate array for point queries. */
  std::vector<Uint> m_point_location_indices;

  /*! An array of indices into the normal array for curves. */
  std::vector<Uint> m_normal_indices;

  /*! Insert the points, curves, and x-monotone curves into the givem
   * representation.
   */
  template <typename Aos>
  void insert_all(Aos* aos)
  {
    typedef typename Aos::Geometry_traits_2             Geom_traits;
    typedef typename Geom_traits::Point_2               Point;
    typedef typename Geom_traits::X_monotone_curve_2    X_monotone_curve;
    typedef typename Geom_traits::Curve_2               Curve;

    if (!m_coord_array) return;

    auto exact_coord_array =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
    auto exact_normal_array =
      boost::dynamic_pointer_cast<Epec_normal_array>(m_normal_array);

    if (exact_coord_array && (exact_coord_array->size() > 0)) {
      std::vector<Uint>::iterator it;
      Epec_kernel kernel;
      auto ctr_direction = kernel.construct_direction_3_object();
      auto ctr_vector = kernel.construct_vector_3_object();

      // Insert the x-monotone curves:
      if (m_x_monotone_curve_indices.size() != 0) {
        if (m_insertion_strategy == AGGREGATE) {
          std::vector<X_monotone_curve>
            vec(m_x_monotone_curve_indices.size() / 2);
          unsigned int i = 0;
          for (it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it)
          {
            Epec_point_3& p1 = (*exact_coord_array)[*it];
            Epec_vector_3 v1 = ctr_vector(CGAL::ORIGIN, p1);
            Epec_direction_3 d1 = ctr_direction(v1);
            Point q1(d1);
            ++it;

            Epec_point_3& p2 = (*exact_coord_array)[*it];
            Epec_vector_3 v2 = ctr_vector(CGAL::ORIGIN, p2);
            Epec_direction_3 d2 = ctr_direction(v2);
            Point q2(d2);

            if (exact_normal_array && (m_normal_indices.size() > i)) {
              Uint index = m_normal_indices[i];
              if (index != static_cast<Uint>(-1)) {
                Epec_vector_3& v = (*exact_normal_array)[index];
                Epec_direction_3 normal = ctr_direction(v);
                vec[i++] = X_monotone_curve(q1, q2, normal);
                continue;
              }
            }
            vec[i++] = X_monotone_curve(q1, q2);
          }
          insert(*aos, vec.begin(), vec.end());
          vec.clear();
        }
        else if (m_insertion_strategy == INCREMENT) {
          unsigned int i = 0;
          for (it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it) {
            Epec_point_3& p1 = (*exact_coord_array)[*it];
            Epec_vector_3 v1 = ctr_vector(CGAL::ORIGIN, p1);
            Epec_direction_3 d1 = ctr_direction(v1);
            Point q1(d1);
            ++it;

            Epec_point_3& p2 = (*exact_coord_array)[*it];
            Epec_vector_3 v2 = ctr_vector(CGAL::ORIGIN, p2);
            Epec_direction_3 d2 = ctr_direction(v2);
            Point q2(d2);

            if (exact_normal_array && (m_normal_indices.size() > i)) {
              Uint index = m_normal_indices[i];
              if (index != static_cast<Uint>(-1)) {
                Epec_vector_3& v = (*exact_normal_array)[index];
                Epec_direction_3 normal = ctr_direction(v);
                X_monotone_curve xc(q1, q2, normal);
                insert(*aos, xc);
                ++i;
                continue;
              }
            }
            X_monotone_curve xc(q1, q2);
            insert(*aos, xc);
            ++i;
          }
        }
      }

      // Insert the general curves:
      if (m_curve_indices.size() != 0) {
        if (m_insertion_strategy == AGGREGATE) {
          std::vector<Curve> vec(m_curve_indices.size()/2);
          unsigned int i = 0;
          for (it = m_curve_indices.begin(); it != m_curve_indices.end(); ++it)
          {
            Epec_point_3& p1 = (*exact_coord_array)[*it];
            Epec_vector_3 v1 = ctr_vector(CGAL::ORIGIN, p1);
            Epec_direction_3 d1 = ctr_direction(v1);
            ++it;

            Epec_point_3& p2 = (*exact_coord_array)[*it];
            Epec_vector_3 v2 = ctr_vector(CGAL::ORIGIN, p2);
            Epec_direction_3 d2 = ctr_direction(v2);

            if (exact_normal_array && (m_normal_indices.size() > i)) {
              Uint index = m_normal_indices[i];
              if (index != static_cast<Uint>(-1)) {
                Epec_vector_3& v = (*exact_normal_array)[index];
                Epec_direction_3 normal = ctr_direction(v);
                vec[i++] = Curve(d1, d2, normal);
                continue;
              }
            }
            vec[i++] = Curve(d1, d2);
          }
          insert(*aos, vec.begin(), vec.end());
          vec.clear();
        }
        else if (m_insertion_strategy == INCREMENT) {
          unsigned int i = 0;
          for (it = m_curve_indices.begin(); it != m_curve_indices.end(); ++it)
          {
            Epec_point_3& p1 = (*exact_coord_array)[*it];
            Epec_vector_3 v1 = ctr_vector(CGAL::ORIGIN, p1);
            Epec_direction_3 d1 = ctr_direction(v1);
            ++it;

            Epec_point_3& p2 = (*exact_coord_array)[*it];
            Epec_vector_3 v2 = ctr_vector(CGAL::ORIGIN, p2);
            Epec_direction_3 d2 = ctr_direction(v2);

            if (exact_normal_array && (m_normal_indices.size() > i)) {
              Uint index = m_normal_indices[i];
              if (index != static_cast<Uint>(-1)) {
                Epec_vector_3& v = (*exact_normal_array)[index];
                Epec_direction_3 normal = ctr_direction(v);
                Curve cv(d1, d2, normal);
                insert(*aos, cv);
                ++i;
                continue;
              }
            }
            Curve cv(d1, d2);
            insert(*aos, cv);
            ++i;
          }
        }
      }

      // Insert the points:
      for (it = m_point_indices.begin(); it != m_point_indices.end(); ++it) {
        Epec_point_3& p = (*exact_coord_array)[*it];
        Epec_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
        Epec_direction_3 d = ctr_direction(v);
        insert_point(*aos, d);
      }
    }
  }

  /*! Draw the arrangement on sphere vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action) = 0;

  /*! Draw the arrangement on sphere edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action) = 0;

  /*! Create the renderers. */
  void create_renderers();

private:
  /*! The container prototype. */
  static Container_proto* s_prototype;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the coordinate array.
inline Arrangement_on_sphere_base_geo::Shared_coord_array
Arrangement_on_sphere_base_geo::get_coord_array() const
{ return m_coord_array; }

//! \brief obtains the x-monotone curve index array.
inline const std::vector<Uint>&
Arrangement_on_sphere_base_geo::get_x_monotone_curve_indices() const
{ return m_x_monotone_curve_indices; }

//! \brief obtains the curve index array.
inline const std::vector<Uint>&
Arrangement_on_sphere_base_geo::get_curve_indices() const
{ return m_curve_indices; }

//! \brief obtains the normal index array.
inline const std::vector<Uint>&
Arrangement_on_sphere_base_geo::get_normal_indices() const
{ return m_normal_indices; }

//! \brief obtains the point index array.
inline const std::vector<Uint>&
Arrangement_on_sphere_base_geo::get_point_indices() const
{ return m_point_indices; }

//! \brief obtains the ith x-monotone curve index.
inline Uint
Arrangement_on_sphere_base_geo::get_x_monotone_curve_index(Uint i) const
{ return m_x_monotone_curve_indices[i]; }

//! \brief obtains the ith curve index.
inline Uint
Arrangement_on_sphere_base_geo::get_curve_index(Uint i) const
{ return m_curve_indices[i]; }

//! \brief obtains the ith normal index.
inline Uint
Arrangement_on_sphere_base_geo::get_normal_index(Uint i) const
{ return m_normal_indices[i]; }

//! \brief obtains the ith point index.
inline Uint
Arrangement_on_sphere_base_geo::get_point_index(Uint i) const
{ return m_point_indices[i]; }

//! \brief obtains the point location index array.
inline const std::vector<Uint>&
Arrangement_on_sphere_base_geo::get_point_location_indices() const
{ return m_point_location_indices; }

//! \brief obtains the ith point location index.
inline Uint
Arrangement_on_sphere_base_geo::get_point_location_index(Uint i) const
{ return m_point_location_indices[i]; }

SGAL_END_NAMESPACE

#endif
