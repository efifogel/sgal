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
// $Id: Arrangement_on_sphere_marked_geo.hpp 7205 2009-01-24 22:20:34Z efif $
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>
//                 Ophir Setter      <ophir.setter@gmail.com>

/*! \file
 * A geometry container that represents a general polygon set embedded on a
 * sphere bounded by geodesic arcs.
 */

#ifndef SGAL_POLYGON_SET_ON_SPHERE_GEO_HPP
#define SGAL_POLYGON_SET_ON_SPHERE_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#include <CGAL/Arr_tracing_traits_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>

#include <CGAL/General_polygon_set_on_surface_2.h>
#include <CGAL/Gps_traits_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_marked_geo.hpp"
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
 * great circles embeded on a sphere.
 */
class SGAL_SCGAL_DECL Polygon_set_on_sphere_geo :
  public Arrangement_on_sphere_marked_geo
{
public:
  // Shared pointer
  typedef boost::shared_ptr<Polygon_set_on_sphere_geo>
    Shared_polygon_set_on_sphere_geo;

  /*! Fields 
   * We assume that the Boolean set operations fields are continous and in
   * the same order as s_operation_types.
   */
  enum Fields {
    FIRST = Arrangement_on_sphere_marked_geo::LAST - 1,
    SET_COMPLEMENT,
    SET_INTERSECTION,
    SET_UNION,
    SET_DIFFERENCE,
    SET_SYMMETRIC_DIFFERENCE,
    NUM_OF_OPERATIONS,
    LAST
  };

  typedef Fields                                      Operation_type;

protected:
  typedef Polygon_set_on_sphere_geo                   Self;
  typedef Arrangement_on_sphere_marked_geo            Base;
  
  typedef Base::Aos_geom_traits                       Gps_traits_2;
  typedef CGAL::General_polygon_set_on_surface_2<
    Gps_traits_2, Base::Aos_topol_traits>             Polygon_set;

  typedef Gps_traits_2::Polygon_2                     Polygon_2;

  typedef std::vector<Int>                            Int_container;
  typedef std::vector<
    Aos_geom_traits::X_monotone_curve_2>              Curve_cont;

public:
  /*! Constructor. */
  Polygon_set_on_sphere_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Polygon_set_on_sphere_geo();

  /* Construct the prototype. */
  static Polygon_set_on_sphere_geo* prototype()
  { return new Polygon_set_on_sphere_geo(true); }

  /*! Clone. */
  virtual Container* clone()
  { return new Polygon_set_on_sphere_geo(); }

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation. */
  virtual void clean();

  /*! */
  virtual void cull(Cull_context& cull_context) {}

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();

  //! Returns the type of operation we need to perform (intersection, etc.).
  Operation_type operation_type() const { return m_operation_type; }

  //! Get the polygon set (const version)
  const Polygon_set& polygon_set() const { return m_polygon_set; }

  //! Get the polygon set (non-const version).
  Polygon_set& polygon_set() { return m_polygon_set; }

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  //! Sets the type of operation we need to perform (intersection etc.)
  void set_operation_type(Operation_type o) { m_operation_type = o; }

  /*! Checks whether a string is a name of a legal Boolean operation
   * supported by Polygon_set_on_sphere_geo class.
   */
  bool is_operation_type(Operation_type op) const
  { return ((op >= SET_COMPLEMENT) && (op < NUM_OF_OPERATIONS)); }

  /*! Checks whether a string is a name of a legal Boolean operation
   * supported by Polygon_set_on_sphere_geo class.
   */
  bool is_operation_type_name(const std::string& op_name)
  { return is_operation_type(get_operation_type_by_name(op_name)); }
  
  //! construct_curves
  /*! Construct x-monotone curve that represent the shortest geodesic arc
      between p1 and p2.
      \param p1 First point
      \param p2 Second point
  */
  Curve_cont construct_curves(Exact_point_3* p1, Exact_point_3* p2);

  //! Get the type of operation according to its name
  Operation_type get_operation_type_by_name(const std::string& op_name);

  /*! A polygon set that represent polygon sets bounded by
   * great-circle arcs on a sphere.
   */
  Polygon_set m_polygon_set;

  /*! 
    enum that specifies the operation type.
    Should be equal to one of the following values: SET_COMPLEMENT,
    SET_INTERSECTION, SET_UNION, SET_DIFFERENCE, SET_SYMMETRIC_DIFFERENCE, 
    or NUM_OF_OPERATIONS (which specifies no operation).
   */
  Operation_type m_operation_type;
 
  /*! An array of indices into the vertex-coordinate array */
  Int_container m_poly_indices;

 private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  //! Names of Booleans set operations.
  static std::string s_operation_types[];

  /*! Default values. */
};
  
SGAL_END_NAMESPACE

#endif // SGAL_POLYGON_SET_ON_SPHERE_GEO_HPP
