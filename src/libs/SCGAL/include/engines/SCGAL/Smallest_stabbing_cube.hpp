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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SMALLEST_STABBING_CUBE_HPP
#define SGAL_SMALLEST_STABBING_CUBE_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Rotation.hpp"

//CGAL includes
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_traits.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_arr_dcel.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

SGAL_BEGIN_NAMESPACE

typedef CGAL::Cartesian<CGAL::Gmpq> Kernel;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;
typedef CGAL::Arr_polyhedral_sgm_traits<Kernel> Sgm_geometry_traits_2;
typedef CGAL::Arr_polyhedral_sgm<Sgm_geometry_traits_2,	CGAL::Arr_polyhedral_sgm_arr_dcel> Spherical_gaussian_map_for_ssc;
typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Spherical_gaussian_map_for_ssc, Kernel> Polyhedron;
typedef CGAL::Point_3<Kernel> Point_3;
typedef Polyhedron::Plane_3 Plane_3;
typedef CGAL::Arr_polyhedral_sgm_initializer<Spherical_gaussian_map_for_ssc, Polyhedron> Sgm_initializer_for_ssc;
typedef CGAL::Arr_polyhedral_sgm_overlay<Spherical_gaussian_map_for_ssc> Sgm_overlay_traits_for_ssc;
typedef CGAL::Quadratic_program<CGAL::Gmpq> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpq> Solution;

class Draw_action;
class Element;
class Container_proto;
class Coord_array;
class Coord_transformer;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Smallest_stabbing_cube : public Container 
{

public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    EXECUTE,
    CHANGED,
    COORD_CHANGED,
    GEOMETRIES,
    LAST
  };

  typedef boost::shared_ptr<Coord_array> Shared_coord_array;
  typedef std::vector<boost::shared_ptr<Coord_transformer>> Coord_node_array;

  /*! Constructor. */
  Smallest_stabbing_cube(Boolean proto = false);

  /* Construct the prototype. */
  static Smallest_stabbing_cube* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this node.
   * \param elem lists of attribute names and values extracted from the input
   * file.
   */
  virtual void set_attributes(Element* elem);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* enabled_handle(const Field_info*) { return &m_enabled; }
  Boolean* changed_handle(const Field_info*) { return &m_changed; }
  Boolean* execute_handle(const Field_info*) { return &m_execute; }
  Shared_coord_array* result_handle(Field_info*) { return &m_result; };
  Coord_node_array* coord_nodes_handle(Field_info*) { return &m_coord_nodes; }
  //@}

  /*! Transform the input vertices. */
  void execute(Field_info* field_info = NULL);

protected:

  /*! Determines whether the node is enabled. */
  Boolean m_enabled;

  /*! Indicates that the transformation has been applied. */
  Boolean m_changed;

  /*! Indicates that the operation should be executed. */
  Boolean m_execute;

  /*! The output vertices. */
  Coord_node_array m_coord_nodes;

  // default values.
  static Boolean s_def_enabled;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  
  Polyhedron calculateSum(const Polyhedron& p1, const Polyhedron& p2) const;
  void addConstraints(const Polyhedron& minkCube1, const Polyhedron& minkCube2, Program& lp, int& count);

  Shared_coord_array m_result;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
