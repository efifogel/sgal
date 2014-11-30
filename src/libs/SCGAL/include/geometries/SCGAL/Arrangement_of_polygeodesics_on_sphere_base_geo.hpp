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

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_surface_geo.hpp"
#include "SCGAL/Exact_coord_array_3d.hpp"
#include "SCGAL/Exact_normal_array.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"

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


protected:
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
    const auto* seg_traits = traits->segment_traits_2();
    auto cmp_endpoints = traits->compare_endpoints_xy_2_object();
    auto ctr_min_vertex = traits->construct_min_vertex_2_object();
    auto ctr_max_vertex = traits->construct_max_vertex_2_object();
    auto ctr_cross_product = seg_traits->construct_cross_product_vector_3_object();
    Edge_const_iterator hei;
    for (hei = aos->edges_begin(); hei != aos->edges_end(); ++hei) {
      const X_monotone_curve& curve = hei->curve();
      // Vector3f src = to_vector3f(curve.source());
      // Vector3f trg = to_vector3f(curve.target());
      // Vector3f normal = to_vector3f(curve.normal());
      auto flag = (cmp_endpoints(curve) == CGAL::SMALLER);
      const auto& source =
        (flag) ? ctr_min_vertex(curve) : ctr_max_vertex(curve);
      const auto& target =
        (flag) ? ctr_max_vertex(curve) : ctr_min_vertex(curve);
      auto normal = ctr_cross_product(source.vector(), target.vector());
      Vector3f src = to_vector3f(source);
      Vector3f trg = to_vector3f(target);
      Vector3f nrm = to_vector3f(normal);
      src.normalize();
      trg.normalize();
      nrm.normalize();
      draw_aos_edge(action, src, trg, nrm);
    }
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

    Shared_exact_coord_array_3d exact_coord_array =
      boost::dynamic_pointer_cast<Exact_coord_array_3d>(m_coord_array);
    Shared_exact_normal_array exact_normal_array =
      boost::dynamic_pointer_cast<Exact_normal_array>(m_normal_array);

    if (exact_coord_array && (exact_coord_array->size() > 0)) {
      const Geom_traits* traits = aos->geometry_traits();
      const auto* seg_traits = traits->segment_traits_2();
      Exact_kernel::Construct_direction_3 ctr_direction =
        seg_traits->construct_direction_3_object();
      Exact_kernel::Construct_vector_3 ctr_vector =
        seg_traits->construct_vector_3_object();

      // Insert the x-monotone curves:
      if (! m_x_monotone_curve_indices.empty()) {
        if (m_insertion_strategy == AGGREGATE) {
          std::list<X_monotone_curve> xcurves;
          for (auto it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it)
          {
            std::list<Point> points;
            while (static_cast<Uint>(-1) != *it) {
              Exact_point_3& p = (*exact_coord_array)[*it];
              Exact_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
              Exact_direction_3 d = ctr_direction(v);
              Point q(d);
              points.push_back(q);
              ++it;
            }
            X_monotone_curve xcv =
              traits->construct_x_monotone_curve_2_object()(points.begin(),
                                                            points.end());
            xcurves.push_back(xcv);
          }

//             if (exact_normal_array && (m_normal_indices.size() > i)) {
//               Uint index = m_normal_indices[i];
//               if (index != static_cast<Uint>(-1)) {
//                 Exact_vector_3& v = (*exact_normal_array)[index];
//                 Exact_direction_3 normal = ctr_direction(v);
//                 vec[i++] = X_monotone_curve(q1, q2, normal);
//                 continue;
//               }
//             }
//             vec[i++] = X_monotone_curve(q1, q2);
//           }
          insert(*aos, xcurves.begin(), xcurves.end());
          xcurves.clear();
        }
        else if (m_insertion_strategy == INCREMENT) {
          for (auto it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it)
          {
            std::list<Point> points;
            while (static_cast<Uint>(-1) != *it) {
              Exact_point_3& p = (*exact_coord_array)[*it];
              Exact_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
              Exact_direction_3 d = ctr_direction(v);
              Point q(d);
              points.push_back(q);
              ++it;
            }
            X_monotone_curve xcv =
              traits->construct_x_monotone_curve_2_object()(points.begin(),
                                                            points.end());
            insert(*aos, xcv);

//             if (exact_normal_array && (m_normal_indices.size() > i)) {
//               Uint index = m_normal_indices[i];
//               if (index != static_cast<Uint>(-1)) {
//                 Exact_vector_3& v = (*exact_normal_array)[index];
//                 Exact_direction_3 normal = ctr_direction(v);
//                 X_monotone_curve xc(q1, q2, normal);
//                 insert(*aos, xc);
//                 ++i;
//                 continue;
//               }
//             }
          }
        }
      }

      // Insert the general curves:
      std::cout << "m_curve_indices.size(): "
                << m_curve_indices.size() << std::endl;
      if (!m_curve_indices.empty()) {
        if (m_insertion_strategy == AGGREGATE) {
          std::list<Curve> curves;
          for (auto it = m_curve_indices.begin();
               it != m_curve_indices.end(); ++it)
          {
            std::list<Point> points;
            while (static_cast<Uint>(-1) != *it) {
              Exact_point_3& p = (*exact_coord_array)[*it];
              Exact_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
              Exact_direction_3 d = ctr_direction(v);
              Point q(d);
              points.push_back(q);
              ++it;
            }
            Curve cv =
              traits->construct_curve_2_object()(points.begin(), points.end());
            curves.push_back(cv);
          }

          // if (exact_normal_array && (m_normal_indices.size() > i)) {
          //   Uint index = m_normal_indices[i];
          //   if (index != static_cast<Uint>(-1)) {
          //     Exact_vector_3& v = (*exact_normal_array)[index];
          //     Exact_direction_3 normal = ctr_direction(v);
          //     vec[i++] = X_monotone_curve(q1, q2, normal);
          //     continue;
          //   }
          // }

          insert(*aos, curves.begin(), curves.end());
          curves.clear();
        }
        else if (m_insertion_strategy == INCREMENT) {
          for (auto it = m_curve_indices.begin();
               it != m_curve_indices.end(); ++it)
          {
            std::list<Point> points;
            while (static_cast<Uint>(-1) != *it) {
              Exact_point_3& p = (*exact_coord_array)[*it];
              Exact_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
              Exact_direction_3 d = ctr_direction(v);
              Point q(d);
              points.push_back(q);
              ++it;
            }
            Curve cv =
              traits->construct_curve_2_object()(points.begin(), points.end());

            // if (exact_normal_array && (m_normal_indices.size() > i)) {
            //   Uint index = m_normal_indices[i];
            //   if (index != static_cast<Uint>(-1)) {
            //     Exact_vector_3& v = (*exact_normal_array)[index];
            //     Exact_direction_3 normal = ctr_direction(v);
            //     X_monotone_curve xc(q1, q2, normal);
            //     insert(*aos, xc);
            //     ++i;
            //     continue;
            //   }
            // }

            insert(*aos, cv);
          }
        }
      }

      // Insert the points:
      for (auto it = m_point_indices.begin(); it != m_point_indices.end();
           ++it)
      {
        Exact_point_3& p = (*exact_coord_array)[*it];
        Exact_vector_3 v = ctr_vector(CGAL::ORIGIN, p);
        Exact_direction_3 d = ctr_direction(v);
        insert_point(*aos, d);
      }
    }
  }

private:
  /*! The container prototype. */
  static Container_proto* s_prototype;

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
