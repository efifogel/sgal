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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <sstream>
#include <string>

#include <CGAL/basic.h>
#include <CGAL/number_utils_classes.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Epec_coord_array_2d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Epec_coord_array_2d::s_tag = "ExactCoordinate2D";

//! The node prototype.
Container_proto* Epec_coord_array_2d::s_prototype(nullptr);

//! Register to the container factory.
REGISTER_TO_FACTORY(Epec_coord_array_2d, "Epec_coord_array_2d");

//! \brief constructs.
Epec_coord_array_2d::Epec_coord_array_2d(Boolean proto) :
  Coord_array(proto),
  m_dirty_inexact_coords(true)
{}

//! \brief constructs.
Epec_coord_array_2d::Epec_coord_array_2d(Size n) :
  m_dirty_inexact_coords(true)
{ m_array.resize(n); }

//! \brief destructs.
Epec_coord_array_2d::~Epec_coord_array_2d() { clear(); }

//! \brief initializes the node prototype.
void Epec_coord_array_2d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Coord_array::get_prototype());
}

//! \brief deletes the node prototype.
void Epec_coord_array_2d::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! Obtain the node prototype.
Container_proto* Epec_coord_array_2d::get_prototype()
{
  if (!s_prototype) Epec_coord_array_2d::init_prototype();
  return s_prototype;
}

//! Sets the attributes of the object extracted from the VRML or X3D file.
void Epec_coord_array_2d::set_attributes(Element* elem)
{
  m_array.clear();

  Coord_array::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "point") {
      auto num_values = get_num_tokens(value);
      auto size = num_values / 3;
      std::istringstream svalue(value, std::istringstream::in);
      m_array.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      for (auto i = 0; i < size; i++) {
        float x, y, z;
        svalue >> x >> y >> z;

        //! \todo apply the following conditionaly and use an input parameter.
        CGAL::Gmpq ex((int)(x * 1000), 1000);
        CGAL::Gmpq ey((int)(y * 1000), 1000);

        Epec_FT fx(ex);
        Epec_FT fy(ey);
        m_array[i] = Epec_point_2(fx,fy);
      }
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }

    if (name == "polarPoint") {
      auto num_values = get_num_tokens(value);
      auto size = num_values / 2;
      std::istringstream svalue(value, std::istringstream::in);
      m_array.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      for (auto i = 0; i < size; i++) {
        float u, v;
        svalue >> u >> v;

        float u_rad = deg2rad(u);
        float v_rad = deg2rad(v);

#if 0
        float x = cosf(u_rad) * cosf(v_rad);;
        float y = cosf(u_rad) * sinf(v_rad);

        Exact_number_type ex(static_cast<int>(x * 1000), 1000);
        Exact_number_type ey(static_cast<int>(y * 1000), 1000);

        Exact_FT fx(ex);
        Exact_FT fy(ey);
#else
        float utanhalf = tanf(u_rad * 0.5f);
        CGAL::Gmpq eu(static_cast<int>(utanhalf * 1000), 1000);
        Epec_FT ut(eu);
        Epec_FT ut_square = CGAL::square(ut);
        Epec_FT cosu = (1 - ut_square) / (1 + ut_square);
        Epec_FT sinu = 2 * ut / (1 + ut_square);

        float vtanhalf = tanf(v_rad * 0.5f);
        CGAL::Gmpq ev(static_cast<int>(vtanhalf * 1000), 1000);
        Epec_FT vt(ev);
        Epec_FT vt_square = CGAL::square(vt);
        Epec_FT cosv = (1 - vt_square) / (1 + vt_square);
        Epec_FT sinv = 2 * vt / (1 + vt_square);

        Epec_FT fx = cosu * cosv;
        Epec_FT fy = cosu * sinv;
#endif

        m_array[i] = Epec_point_2(fx,fy);
      }
      elem->mark_delete(ai);
    }

    if (name == "exactPoint") {
      auto num_values = get_num_values<Epec_FT>(value);
      auto size = num_values / 2;
      m_array.resize(size);
      std::istringstream svalue(value, std::istringstream::in);
      //! svalue.seekg(0); why this doesn't work?
      for (auto i = 0; i < size; ++i) svalue >> m_array[i];
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! Obtain the data size.
Size Epec_coord_array_2d::data_size() const
{ return size() * sizeof(Vector2f); }

//! \brief obtains the data.
inline const GLfloat* Epec_coord_array_2d::datum(Uint i) const
{
  if (m_dirty_inexact_coords) clean_inexact_coords();
  return (GLfloat*)(&(m_inexact_coords[i]));
}

// \brief obtains the data.
const GLfloat* Epec_coord_array_2d::data() const
{
  if (m_dirty_inexact_coords) clean_inexact_coords();
  return (GLfloat*)(&(*(m_inexact_coords.begin())));
}

//! \brief obtains the inexact coordinates.
const std::vector<Vector2f>& Epec_coord_array_2d::get_inexact_coords() const
{
  if (m_dirty_inexact_coords) clean_inexact_coords();
  return m_inexact_coords;
}

//! \brief clean the raw data.
void Epec_coord_array_2d::clean_inexact_coords() const
{
  m_dirty_inexact_coords = false;
  m_inexact_coords.resize(size());

  // Convert the exact points to inexact points.
  std::vector<Vector2f>::iterator it = m_inexact_coords.begin();
  for (Exact_point_const_iter eit = begin(); eit != end(); ++eit) {
    const auto& p = *eit;
    it->set(static_cast<Float>(CGAL::to_double(p.x())),
            static_cast<Float>(CGAL::to_double(p.y())));
  }
}

//! \brief clears the array.
void Epec_coord_array_2d::clear()
{
  m_array.clear();
  m_inexact_coords.clear();
  m_dirty_inexact_coords = true;
}

SGAL_END_NAMESPACE
