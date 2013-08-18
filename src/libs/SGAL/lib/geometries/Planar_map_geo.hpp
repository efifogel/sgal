// Copyright (c) 2004  Israel.
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
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_PLANAR_MAP_GEO_HPP
#define SGAL_PLANAR_MAP_GEO_HPP

/*! \file
 * A node in the scene graph that maintains a planar map.
 */

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Pm_default_dcel.h>
#include <CGAL/Arr_segment_cached_traits_2.h>
#include <CGAL/Planar_map_2.h>
#include <CGAL/Pm_with_intersections.h>

#include "SGAL/Group.hpp"
#include "SGAL/Scene_graph_defs.hpp"
#include "SGAL/Geometry.hpp"

#include "SCGAL/Exact_kernel.hpp"

class Container_proto;
class Cull_context;
class Draw_action;
class Isect_action;
class Scene_graph;

typedef CGAL::Arr_segment_cached_traits_2<Exact_kernel>     Traits;
typedef Traits::Point_2                                     Point_2;
typedef Traits::X_monotone_curve_2                          X_monotone_curve_2;
typedef CGAL::Pm_default_dcel<Traits>                       Dcel;
typedef CGAL::Planar_map_2<Dcel, Traits>                    Planar_map_2;
typedef CGAL::Planar_map_with_intersections_2<Planar_map_2> Pmwx;

class SCENE_GRAPH_CLASSDEF Planar_map_geo : public Geometry {
public:
  Planar_map_geo();
  ~Planar_map_geo();

  virtual void cull(Cull_context & cull_context);
  virtual void draw(Draw_action* action);
  virtual void isect(Isect_action* action);
  virtual Boolean calculate_sphere_bound();

  enum {
    FIRST = Group::LAST - 1,
    LAST
  };

  /*! Constructor */
  static Planar_map_geo* new_object() { return new Planar_map_geo(); }

  /*! Clone */
  virtual Container* clone() { return new Planar_map_geo(); }

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

private:
  static Container_proto* s_prototype;

  Pmwx m_pm;
};

#endif
