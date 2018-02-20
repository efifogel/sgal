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
//                 Ophir Setter      <ophir.setter@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4146 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4390 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#endif

//Ignore in debug
#ifndef _DEBUG

/*! \file
 * A geometry container that represents a general polygon set embedded on a
 * sphere bounded by geodesic arcs.
 */

#include <list>
#include <boost/lexical_cast.hpp>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Polygon_set_on_sphere_geo.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Polygon_set_on_sphere_geo::s_tag = "PolygonSetOnSphere";
Container_proto* Polygon_set_on_sphere_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Polygon_set_on_sphere_geo, "Polygon_set_on_sphere_geo");

// Default values
std::string Polygon_set_on_sphere_geo::s_operation_types[] = {
  "complement",
  "intersection",
  "union",
  "difference",
  "symmetric_difference"
};

//! \brief constructor.
Polygon_set_on_sphere_geo::
Polygon_set_on_sphere_geo(Boolean proto) :
  Arrangement_on_sphere_marked_geo(proto), m_operation_type(NUM_OF_OPERATIONS)
{
  m_aos = &m_polygon_set.arrangement();
  m_owned_aos = false;
}

//! \brief destructor.
Polygon_set_on_sphere_geo::~Polygon_set_on_sphere_geo() { clear(); }

//! \brief initializes the container prototype.
void Polygon_set_on_sphere_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_marked_geo::get_prototype());
}

//! \brief deletes the container prototype.
void Polygon_set_on_sphere_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Polygon_set_on_sphere_geo::get_prototype()
{
  if (!s_prototype) Polygon_set_on_sphere_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
void Polygon_set_on_sphere_geo::set_attributes(Element* elem)
{
  Arrangement_on_sphere_marked_geo::set_attributes(elem);
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);

    if (name == "polyIndex") {
      tokenizer tokens(value, sep);
      auto size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_poly_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_poly_indices.resize(size);
      Uint i = 0;
      for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        m_poly_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<Uint>(*it));
      }
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (is_operation_type_name(name)) {
      set_operation_type(get_operation_type_by_name(name));
      auto aos_geo =
        boost::dynamic_pointer_cast<Polygon_set_on_sphere_geo>(cont);
      if (aos_geo) add_aos_geo(aos_geo);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  for (auto mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const auto& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (is_operation_type_name(name)) {
      set_operation_type(get_operation_type_by_name(name));
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Shared_container cont = *ci;
        Shared_polygon_set_on_sphere_geo aos_geo =
          boost::dynamic_pointer_cast<Polygon_set_on_sphere_geo>(cont);
        if (aos_geo) add_aos_geo(aos_geo);
      }
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief cleans the representation.
void Polygon_set_on_sphere_geo::clean()
{
  // If you, for some reason, want to overlay the arrangements, insert
  // some curves, or do a point location, you can use the base class function.
  // Bare in mind, though, that the base class BY DEFAULT, overlay the
  // arrangements which is a redunt operation if you want to do Boolean set
  // operations. You should take care that the base function does not overlay
  // for nothing.

  SGAL_assertion(is_operation_type(m_operation_type));
  SGAL_assertion(m_aos != nullptr);

  m_dirty = false;
  this->polygon_set().clear();

  if (m_poly_indices.empty() == false) {
    auto exact_coord_array =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
    auto it = m_poly_indices.begin();
    while (it != m_poly_indices.end()) {
      // create the polygons according to the indices and add them to the
      // polygon set.
      typedef std::vector<CGAL::Object>                           Obj_cont;

      Curve_cont curves;
      Epec_point_3* first = &(*exact_coord_array)[*it];
      Epec_point_3* curr = nullptr;
      Epec_point_3* prev = nullptr;
      while ((it != m_poly_indices.end()) && (*it != -1)) {
        prev = curr;

        if (*it >= (int)(*exact_coord_array).size()) {
          std::cerr << "Error! Index too big " << std::endl;
          ++it;
          continue;
        }
        curr = &(*exact_coord_array)[*it];

        if (prev != nullptr) {
          if (*prev == *curr) {
            std::cerr << "Error! Degenerate segment of polygon "
              "polygon_set_on_sphere_geo" << std::endl;
            ++it;
            continue;
          }
          Curve_cont temp = construct_curves(prev, curr);
          curves.insert(curves.end(), temp.begin(), temp.end());
        }
        ++it;
      }
      // add the geodesic segment between the first and the last points.
      if (curr != nullptr) {
        if (*first == *curr) {
          std::cerr << "Error! Degenerate segment of polygon "
            "polygon_set_on_sphere_geo" << std::endl;
        } else {
          Curve_cont temp_curs = construct_curves(curr, first);
          curves.insert(curves.end(), temp_curs.begin(), temp_curs.end());
        }
      }

      // create the polygon and add it to the set.
      Polygon_2 cur_pgn;
      if (curves.empty() == false) {
        this->m_aos->geometry_traits()->
          construct_polygon_2_object() (curves.begin(), curves.end(), cur_pgn);

        this->polygon_set().insert(cur_pgn);
      }

      // in case that the last one is -1.
      if ((it != m_poly_indices.end()) && (*it == -1))
        ++it;
    }

    return;
  }

  // For now, we compute the operation between the first and the last
  // polygons.
  if (m_aoses.size() >= 2) {
    auto first =
      boost::dynamic_pointer_cast<Polygon_set_on_sphere_geo>(m_aoses.front());
    auto last =
      boost::dynamic_pointer_cast<Polygon_set_on_sphere_geo>(m_aoses.back());

    // TODO: Prevent infinite recursion!
    first->clean();
    last->clean();

    switch (operation_type()) {
     case SET_INTERSECTION:
      polygon_set().intersection(first->polygon_set(), last->polygon_set());
      return;
     case SET_UNION:
      polygon_set().join(first->polygon_set(), last->polygon_set());
      return;
     case SET_DIFFERENCE:
      polygon_set().difference(first->polygon_set(), last->polygon_set());
      return;
     case SET_SYMMETRIC_DIFFERENCE:
      polygon_set().symmetric_difference(first->polygon_set(),
                                         last->polygon_set());
      return;
    default:
      break;
    }
  }

  // Complement uses the first polygon.
  if (m_aoses.size() >= 1) {
    Shared_polygon_set_on_sphere_geo poly_set =
      boost::dynamic_pointer_cast<Polygon_set_on_sphere_geo>(m_aoses.front());

    // TODO: prevent infinite recursion.
    // For some reason, I was not able to create infinite recursion.
    // My guess is that the parser some how solves it.
    poly_set->clean();

    if (operation_type() == SET_COMPLEMENT)
      polygon_set().complement(poly_set->polygon_set());
  }
}

//! \brief clears the internal representation and auxiliary data structures.
void Polygon_set_on_sphere_geo::clear()
{
  Base::clear();
}

//! Get the type of operation according to its name
Polygon_set_on_sphere_geo::Operation_type
Polygon_set_on_sphere_geo::get_operation_type_by_name(const std::string& op_name)
{

  std::string* p = std::find(&s_operation_types[0],
                             &s_operation_types[NUM_OF_OPERATIONS],
                             op_name);

  if (p == &s_operation_types[NUM_OF_OPERATIONS])
    return LAST;   // which is NOT an operation type.

  return static_cast<Operation_type>(SET_COMPLEMENT +
                                     (p - &s_operation_types[0]));
}

//! construct_curves
/*! Construct x-monotone curve that represent the shortest geodesic arc
 * between p1 and p2.
 * \param p1 First point
 * \param p2 Second point
 */
Polygon_set_on_sphere_geo::Curve_cont
Polygon_set_on_sphere_geo::construct_curves(Epec_point_3* p1, Epec_point_3* p2)
{
  SGAL_precondition(p1 != nullptr);
  SGAL_precondition(p2 != nullptr);
  SGAL_precondition(*p1 != *p2);

  typedef Aos_geom_traits::Curve_2 Curve_2;
  typedef Aos_geom_traits::X_monotone_curve_2 X_monotone_curve_2;

  // converting points to directions, needed by the geodesic traits class.
  Epec_kernel k;
  Epec_vector_3 vec1 = k.construct_vector_3_object()(CGAL::ORIGIN, *p1);
  Epec_vector_3 vec2 = k.construct_vector_3_object()(CGAL::ORIGIN, *p2);
  Epec_direction_3 dir1 = k.construct_direction_3_object()(vec1);
  Epec_direction_3 dir2 = k.construct_direction_3_object()(vec2);
  Curve_2 new_cv(dir1, dir2);

  std::list<CGAL::Object> objects;
  m_aos->geometry_traits()->make_x_monotone_2_object()
    (new_cv , std::back_inserter(objects));

  Curve_cont res;
  for (auto it = objects.begin(); it != objects.end(); ++it) {
    const X_monotone_curve_2* x_mono_cv =
      CGAL::object_cast<X_monotone_curve_2>(&(*it));
    SGAL_assertion_msg(x_mono_cv != nullptr,
                       "We should receive only x-monotone curves.");
    res.push_back(*x_mono_cv);
  }

  return res;
}

SGAL_END_NAMESPACE

#endif
