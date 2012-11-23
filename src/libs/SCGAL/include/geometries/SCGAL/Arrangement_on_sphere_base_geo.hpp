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
// $Id: Arrangement_on_sphere_base_geo.hpp 6666 2008-07-24 10:07:46Z efif $
// $Revision: 6666 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#ifndef SGAL_ARRANGEMENT_ON_SPHERE_BASE_GEO_HPP
#define SGAL_ARRANGEMENT_ON_SPHERE_BASE_GEO_HPP

#include <CGAL/basic.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"

#include "SCGAL/Arrangement_on_surface_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"
#include "SCGAL/Exact_coord_array.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */
class SGAL_CLASSDEF Arrangement_on_sphere_base_geo :
  public Arrangement_on_surface_geo
{
public:
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
  /*! Constructor */
  Arrangement_on_sphere_base_geo(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Arrangement_on_sphere_base_geo();

  /*! Clone */
  virtual Container * clone() = 0;

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype */
  virtual Container_proto * get_prototype();

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! */
  virtual void cull(Cull_context & cull_context);

  /*! */
  virtual void isect(Isect_action * action);

  /*! */
  virtual Boolean calculate_sphere_bound();

  /*! Clear the internal representation and auxiliary data structures */
  virtual void clear() = 0;

  /*! Set the coordinate array */
  void set_coord_array(Coord_array * coord_array);

  /*! Obtain the coordinate array */
  Coord_array * get_coord_array() const;

  /*! Obtain the x-monotone curve index array */
  const std::vector<Uint> & get_x_monotone_curve_indices() const;

  /*! Obtain the curve index array */
  const std::vector<Uint> & get_curve_indices() const;

  /*! Obtain the point index array */
  const std::vector<Uint> & get_point_indices() const;

  /*! Obtain the i-th x-monotone curve index */
  Uint get_x_monotone_curve_index(Uint i) const;

  /*! Obtain the i-th curve index */
  Uint get_curve_index(Uint i) const;

  /*! Obtain the i-th point index */
  Uint get_point_index(Uint i) const;

  /*! Obtain the point location index array */
  const std::vector<Uint> & get_point_location_indices() const;

  /*! Obtain the i-th point index */
  Uint get_point_location_index(Uint i) const;
    
  /*! Draw an arrangement on surface vertex
   * \param action
   * \param center the vertex center
   */
  virtual void draw_aos_vertex(Draw_action * action, Vector3f & center);

  /*! Draw an arrangement on surface isolated vertex
   * \param action
   * \param center the vertex center
   */
  virtual void draw_aos_isolated_vertex(Draw_action * action,
                                        Vector3f & center);
  
  /*! Draw an arrangement on surface boundary_vertex
   * \param action
   * \param center the vertex center
   */
  virtual void draw_aos_boundary_vertex(Draw_action * action,
                                        Vector3f & center);
  
  /*! Draw an arrangement on surface edge
   * \param action
   * \param source the edge source point
   * \param target the edge target point
   */
  virtual void draw_aos_edge(Draw_action * action,
                             Vector3f & source, Vector3f & target);
  
    
protected:
  typedef SGAL::Sphere_renderer            Surface_renderer;
  typedef SGAL::Colored_sphere_renderer    Colored_surface_renderer;
  typedef SGAL::Stencil_sphere_renderer    Stencil_surface_renderer;

  /*! An array of direction ccordinates */
  Coord_array * m_coord_array;

  /*! An array of indices into the coordinate array for general curves */
  std::vector<Uint> m_curve_indices;

  /*! An array of indices into the coordinate array for x-monotone curves */
  std::vector<Uint> m_x_monotone_curve_indices;

  /*! An array of indices into the coordinate array for isolated points */
  std::vector<Uint> m_point_indices;

  /*! An array of indices into the coordinate array for point queries */
  std::vector<Uint> m_point_location_indices;
  
  /*! Insert the points, curves, and x-monotone curves into the givem
   * representation
   */
  template <typename Aos>
  void insert_all(Aos * aos)
  {
    typedef typename Aos::Geometry_traits_2             Geom_traits;
    typedef typename Geom_traits::Point_2               Point;
    typedef typename Geom_traits::X_monotone_curve_2    X_monotone_curve;
    typedef typename Geom_traits::Curve_2               Curve;

    if (!m_coord_array) return;
    
    Exact_coord_array * exact_coord_array =
      dynamic_cast<Exact_coord_array *>(m_coord_array);
    if (exact_coord_array && (exact_coord_array->size() > 0)) {
      std::vector<Uint>::iterator it;
      Exact_kernel kernel;

      // Insert the x-monotone curves:
      if (m_x_monotone_curve_indices.size() != 0) {
        if (m_insertion_strategy == AGGREGATE) {
          std::vector<X_monotone_curve>
            vec(m_x_monotone_curve_indices.size() / 2);
          unsigned int i = 0;
          for (it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it)
          {
            Exact_point_3 & p1 = (*exact_coord_array)[*it];
            Exact_vector_3 v1 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p1);
            Exact_direction_3 d1 = kernel.construct_direction_3_object()(v1);
            Point q1(d1);
            ++it;
            
            Exact_point_3 & p2 = (*exact_coord_array)[*it];
            Exact_vector_3 v2 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p2);
            Exact_direction_3 d2 = kernel.construct_direction_3_object()(v2);
            Point q2(d2);

            vec[i++] = X_monotone_curve(q1, q2);
          }
          insert(*aos, vec.begin(), vec.end());
          vec.clear();
        } else if (m_insertion_strategy == INCREMENT) {
          for (it = m_x_monotone_curve_indices.begin();
               it != m_x_monotone_curve_indices.end(); ++it) {
            Exact_point_3 & p1 = (*exact_coord_array)[*it];
            Exact_vector_3 v1 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p1);
            Exact_direction_3 d1 = kernel.construct_direction_3_object()(v1);
            Point q1(d1);
            ++it;
          
            Exact_point_3 & p2 = (*exact_coord_array)[*it];
            Exact_vector_3 v2 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p2);
            Exact_direction_3 d2 = kernel.construct_direction_3_object()(v2);
            Point q2(d2);

            X_monotone_curve xc(q1, q2);
            insert(*aos, xc);
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
            Exact_point_3 & p1 = (*exact_coord_array)[*it];
            Exact_vector_3 v1 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p1);
            Exact_direction_3 d1 = kernel.construct_direction_3_object()(v1);
            ++it;
          
            Exact_point_3 & p2 = (*exact_coord_array)[*it];
            Exact_vector_3 v2 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p2);
            Exact_direction_3 d2 = kernel.construct_direction_3_object()(v2);

            vec[i++] = Curve(d1, d2);
          }
          insert(*aos, vec.begin(), vec.end());
          vec.clear();
        } else if (m_insertion_strategy == INCREMENT) {
          for (it = m_curve_indices.begin(); it != m_curve_indices.end(); ++it)
          {
            Exact_point_3 & p1 = (*exact_coord_array)[*it];
            Exact_vector_3 v1 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p1);
            Exact_direction_3 d1 = kernel.construct_direction_3_object()(v1);
            ++it;
          
            Exact_point_3 & p2 = (*exact_coord_array)[*it];
            Exact_vector_3 v2 =
              kernel.construct_vector_3_object()(CGAL::ORIGIN, p2);
            Exact_direction_3 d2 = kernel.construct_direction_3_object()(v2);

            Curve cv(d1, d2);
            insert(*aos, cv);
          }
        }
      }

      // Insert the points:
      for (it = m_point_indices.begin(); it != m_point_indices.end(); ++it) {
        Exact_point_3 & p = (*exact_coord_array)[*it];
        Exact_vector_3 v = kernel.construct_vector_3_object()(CGAL::ORIGIN, p);
        Exact_direction_3 d = kernel.construct_direction_3_object()(v);
        insert_point(*aos, d);
      }
    }
  }

private:
  /*! The container prototype */
  static Container_proto * s_prototype;

  /*! Draw the arrangement on sphere vertices
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action * action) = 0;

  /*! Draw the arrangement on sphere edges
   * \param action
   */
  virtual void draw_aos_edges(Draw_action * action) = 0;

  /*! Create the renderers */
  void create_renderers();
};

/*! \brief obtains the coordinate array */
inline Coord_array * Arrangement_on_sphere_base_geo::get_coord_array() const
{ return m_coord_array; }

/*! \brief obtains the x-monotone curve index array */
inline const std::vector<Uint> &
Arrangement_on_sphere_base_geo::get_x_monotone_curve_indices() const
{ return m_x_monotone_curve_indices; }

/*! \brief obtains the curve index array */
inline const std::vector<Uint> &
Arrangement_on_sphere_base_geo::get_curve_indices() const
{ return m_curve_indices; }

/*! \brief obtains the point index array */
inline const std::vector<Uint> &
Arrangement_on_sphere_base_geo::get_point_indices() const
{ return m_point_indices; }

/*! \brief obtains the i-th x-monotone curve index */
inline Uint
Arrangement_on_sphere_base_geo::get_x_monotone_curve_index(Uint i) const
{ return m_x_monotone_curve_indices[i]; }

/*! \brief obtains the i-th curve index */
inline Uint
Arrangement_on_sphere_base_geo::get_curve_index(Uint i) const
{ return m_curve_indices[i]; }

/*! \brief obtains the i-th point index */
inline Uint
Arrangement_on_sphere_base_geo::get_point_index(Uint i) const
{ return m_point_indices[i]; }

/*! \brief obtains the point location index array */
inline const std::vector<Uint> &
Arrangement_on_sphere_base_geo::get_point_location_indices() const
{ return m_point_location_indices; }

/*! \brief obtains the i-th point location index */
inline Uint
Arrangement_on_sphere_base_geo::get_point_location_index(Uint i) const
{ return m_point_location_indices[i]; }

SGAL_END_NAMESPACE

#endif
