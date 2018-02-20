// Copyright (c) 2014 Israel.
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

#ifndef SCGAL_ARRANGEMENT_OF_POLYGEODESICS_ON_SPHERE_BASE_GEO_HPP
#define SCGAL_ARRANGEMENT_OF_POLYGEODESICS_ON_SPHERE_BASE_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <list>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"
#include "SGAL/Epec_normal_array.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_surface_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"

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
class SGAL_SCGAL_DECL Arrangement_of_polygeodesics_on_sphere_base_geo :
  public Arrangement_on_sphere_base_geo
{
public:
  enum {
    FIRST = Arrangement_on_sphere_base_geo::LAST - 1,
    PG_VERTEX_STYLE,
    PG_VERTEX_STYLE_ID,
    PG_VERTEX_RADIUS,
    PG_VERTEX_POINT_SIZE,
    PG_VERTEX_COLOR,
    LAST
  };

public:
  /*! Constructor. */
  Arrangement_of_polygeodesics_on_sphere_base_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Arrangement_of_polygeodesics_on_sphere_base_geo();

  /*! Clone. */
  virtual Container* clone() = 0;

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vertex_style* pg_vertex_style_handle(const Field_info*)
  { return &m_pg_vertex_style; }
  //@}

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear() = 0;

  /*! Draw an arrangement on surface edge.
   * \param action
   * \param source the edge source point.
   * \param target the edge target point.
   * \param normal the normal to the plane containing the edge.
   */
  virtual void draw_aos_edge(Draw_action* action,
                             Vector3f& source, Vector3f& target,
                             Vector3f& normal);

  /*! Obtain the vertex-shape style.
   * \return the vertex-shape style.
   */
  Vertex_style get_pg_vertex_style() const { return m_pg_vertex_style; }

  /*! Set the vertex-shape style. */
  void set_pg_vertex_style(Vertex_style style) { m_pg_vertex_style = style; }

  /*! Obtain the vertex radius.
   * \return the vertex radius.
   */
  Float get_pg_vertex_radius() const { return m_pg_vertex_radius; }

  /*! Set the vertex radius */
  void set_pg_vertex_radius(Float radius) { m_pg_vertex_radius = radius; }

  /*! Obtain the pg vertex color.
   * \return the vertex color.
   */
  const Vector3f& get_pg_vertex_color() const { return m_pg_vertex_color; }

  /*! Set the pg vertex color.
   */
  void set_pg_vertex_color(const Vector3f& color) { m_pg_vertex_color = color; }

  /*! Obtain the vertex point size */
  Float get_pg_vertex_point_size() const { return m_pg_vertex_point_size; }

  /*! Set the vertex point size */
  void set_pg_vertex_point_size(Float size) { m_pg_vertex_point_size = size; }

protected:
  /*! The vertex shape. */
  Vertex_style m_pg_vertex_style;

  /*! The radius of the disc or ball that represents a vertex. */
  Float m_pg_vertex_radius;

  /*! The size of the point that represents a vertex. */
  Float m_pg_vertex_point_size;

  /*! The color of the vertices. */
  Vector3f m_pg_vertex_color;

  /*! Draw the edges of a given arrangement on surface.
   * \param aos the arrangement on surface.
   * \param action
   */
  template <typename Aos>
  void my_draw_aos_edges(Aos* aos, Draw_action* action)
  {
//     void draw_vertex_on_sphere(Draw_action* action, Vector3f& center,
//                                Vertex_style style,
//                                Float radius, Float delta_angle);

    typedef typename Aos::Edge_const_iterator           Edge_const_iterator;
    typedef typename Aos::Geometry_traits_2             Geom_traits;
    typedef typename Geom_traits::Point_2               Point;
    typedef typename Geom_traits::X_monotone_curve_2    X_monotone_curve;

    const Geom_traits* traits = aos->geometry_traits();
    const auto* seg_traits = traits->subcurve_traits_2();
    auto cmp_endpoints = traits->compare_endpoints_xy_2_object();
    auto ctr_min_vertex = traits->construct_min_vertex_2_object();
    auto ctr_max_vertex = traits->construct_max_vertex_2_object();
    auto ctr_cross_product =
      seg_traits->construct_cross_product_vector_3_object();
    Edge_const_iterator hei;
    for (hei = aos->edges_begin(); hei != aos->edges_end(); ++hei) {
      const X_monotone_curve& curve = hei->curve();
      auto last = curve.subcurves_end();
      --last;
      for (auto it = curve.subcurves_begin(); it != curve.subcurves_end(); ++it)
      {
        const auto& seg = *it;
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

        Vector3f src = to_vector3f(seg.source());
        Vector3f trg = to_vector3f(seg.target());
        Vector3f nrm = to_vector3f(seg.normal());
        src.normalize();
        trg.normalize();
        if (it != last)
          draw_vertex_on_sphere(action, trg, get_pg_vertex_style(),
                                get_pg_vertex_radius(), get_aos_delta_angle());
        draw_aos_edge(action, src, trg, nrm);
      }
    }
  }

  /*! Construct an x-monotone polygeodesic curve.
   * \param exact_coord_array the coordinate array.
   * \param exact_normal_array the normal array.
   * \param i the index of the curve. (Used to access the normal indices.)
   * \param it (in/out) the iterator of the coord indices array.
   * \param traits (in) the traits.
   * \return the newly constructed x-monotone polygeodesic curve.
   */
  template <typename InputIterator, typename GeomTraits>
  typename GeomTraits::X_monotone_curve_2 construct_x_monotone_curve
    (Shared_exact_coord_array_3d exact_coord_array,
     Shared_exact_normal_array exact_normal_array,
     Uint i,
     InputIterator& it,
     const GeomTraits* traits)
  {
    const auto* seg_traits = traits->subcurve_traits_2();
    auto ctr_direction = seg_traits->construct_direction_3_object();
    auto ctr_vector = seg_traits->construct_vector_3_object();

    std::list<typename GeomTraits::Point_2> points;
    while (static_cast<Uint>(-1) != *it) {
      Epec_point_3& p = (*exact_coord_array)[*it];
      Epec_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
      Epec_direction_3 d = ctr_direction(v);
      typename GeomTraits::Point_2 q(d);
      points.push_back(q);
      ++it;
    }

    if (!exact_normal_array || (m_normal_indices.size() <= i))
      return traits->construct_x_monotone_curve_2_object()(points.begin(),
                                                           points.end());

    SGAL_assertion(2 == points.size());
    Uint index = m_normal_indices[i];
    Epec_vector_3& v = (*exact_normal_array)[index];
    Epec_direction_3 normal = ctr_direction(v);
    auto pit = points.begin();
    typename GeomTraits::Segment_traits_2::X_monotone_curve_2
      seg(*pit++, *pit, normal);
    return traits->construct_x_monotone_curve_2_object()(seg);
  }

  /*! Construct a polygeodesic general curve.
   * \param exact_coord_array the coordinate array.
   * \param exact_normal_array the normal array.
   * \param i the index of the curve. (Used to access the normal indices.)
   * \param it (in/out) the iterator of the coord indices array.
   * \param traits (in) the traits.
   * \return the newly constructed polygeodesic curve.
   */
  template <typename InputIterator, typename GeomTraits>
  typename GeomTraits::Curve_2 construct_curve
    (Shared_exact_coord_array_3d exact_coord_array,
     Shared_exact_normal_array exact_normal_array,
     Uint i,
     InputIterator& it,
     const GeomTraits* traits)
  {
    const auto* seg_traits = traits->subcurve_traits_2();
    auto ctr_direction = seg_traits->construct_direction_3_object();
    auto ctr_vector = seg_traits->construct_vector_3_object();

    std::list<typename GeomTraits::Point_2> points;
    while (static_cast<Uint>(-1) != *it) {
      Epec_point_3& p = (*exact_coord_array)[*it];
      Epec_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
      Epec_direction_3 d = ctr_direction(v);
      typename GeomTraits::Point_2 q(d);
      points.push_back(q);
      ++it;
    }

    if (!exact_normal_array || (m_normal_indices.size() <= i))
      return traits->construct_curve_2_object()(points.begin(), points.end());

    SGAL_assertion(2 == points.size());
    Uint index = m_normal_indices[i];
    Epec_vector_3& v = (*exact_normal_array)[index];
    Epec_direction_3 normal = ctr_direction(v);
    auto pit = points.begin();
    typename GeomTraits::Segment_traits_2::Curve_2 seg(*pit++, *pit, normal);
    return traits->construct_curve_2_object()(seg);
  }

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
      const Geom_traits* traits = aos->geometry_traits();
      const auto* seg_traits = traits->subcurve_traits_2();
      auto ctr_direction = seg_traits->construct_direction_3_object();
      auto ctr_vector = seg_traits->construct_vector_3_object();

      // Insert the x-monotone curves:
      if (! m_x_monotone_curve_indices.empty()) {
        if (m_insertion_strategy == AGGREGATE) {
          Uint i(0);
          std::list<X_monotone_curve> xcurves;
          for (auto it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it)
          {
            X_monotone_curve xcv =
              construct_x_monotone_curve(exact_coord_array, exact_normal_array,
                                         i, it, traits);
            xcurves.push_back(xcv);
            ++i;
          }

          insert(*aos, xcurves.begin(), xcurves.end());
          xcurves.clear();
        }
        else if (m_insertion_strategy == INCREMENT) {
          Uint i(0);
          for (auto it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it)
          {
            X_monotone_curve xcv =
              construct_x_monotone_curve(exact_coord_array, exact_normal_array,
                                         i, it, traits);
            insert(*aos, xcv);
            ++i;
          }
        }
      }

      // Insert the general curves:
      if (!m_curve_indices.empty()) {
        if (m_insertion_strategy == AGGREGATE) {
          Uint i(0);
          std::list<Curve> curves;
          for (auto it = m_curve_indices.begin();
               it != m_curve_indices.end(); ++it)
          {
            Curve cv = construct_curve(exact_coord_array, exact_normal_array,
                                       i, it, traits);
            curves.push_back(cv);
            ++i;
          }

          insert(*aos, curves.begin(), curves.end());
          curves.clear();
        }
        else if (m_insertion_strategy == INCREMENT) {
          Uint i(0);
          for (auto it = m_curve_indices.begin();
               it != m_curve_indices.end(); ++it)
          {
            Curve cv = construct_curve(exact_coord_array, exact_normal_array,
                                       i, it, traits);
            insert(*aos, cv);
            ++i;
          }
        }
      }

      // Insert the points:
      for (auto it = m_point_indices.begin(); it != m_point_indices.end();
           ++it)
      {
        Epec_point_3& p = (*exact_coord_array)[*it];
        Epec_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
        Epec_direction_3 d = ctr_direction(v);
        insert_point(*aos, d);
      }
    }
  }

private:
  /*! The container prototype. */
  static Container_proto* s_prototype;

  static const Vertex_style s_def_pg_vertex_style;
  static const Float s_def_pg_vertex_radius;
  static const Float s_def_pg_vertex_point_size;
  static const Vector3f s_def_pg_vertex_color;

  /*! Draw the arrangement on sphere vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action) = 0;

  /*! Draw the arrangement on sphere edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action) = 0;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
