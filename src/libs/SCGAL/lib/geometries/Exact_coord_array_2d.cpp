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

#include <CGAL/basic.h>
#include <CGAL/number_utils_classes.h>

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/Geo_set.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

#include "SCGAL/Exact_coord_array_2d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_coord_array_2d::s_tag = "ExactCoordinate2D";

//! The node prototype.
Container_proto* Exact_coord_array_2d::s_prototype(nullptr);

//! Register to the container factory.
REGISTER_TO_FACTORY(Exact_coord_array_2d, "Exact_coord_array_2d");

//! \brief initializes the node prototype.
void Exact_coord_array_2d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Coord_array::get_prototype());
}

//! \brief deletes the node prototype.
void Exact_coord_array_2d::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! Obtain the node prototype.
Container_proto* Exact_coord_array_2d::get_prototype()
{
  if (!s_prototype) Exact_coord_array_2d::init_prototype();
  return s_prototype;
}

//! Sets the attributes of the object extracted from the VRML or X3D file.
void Exact_coord_array_2d::set_attributes(Element* elem)
{
  m_array.clear();

  Coord_array::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai = elem->str_attrs_begin();
  for (; ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "fltPoint") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 3;
      std::istringstream svalue(value, std::istringstream::in);
      int old_size = m_array.size();
      m_array.resize(old_size + size);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; i++) {
        float x, y, z;
        svalue >> x >> y >> z;
#if SCGAL_NT == SCGAL_CGAL_GMPZ_NT
        Exact_number_type ex(((int)(x * 1000)) / 1000);
        Exact_number_type ey(((int)(y * 1000)) / 1000);
#elif SCGAL_NT == SCGAL_DOUBLE_NT
        not implemented
#else
        Exact_number_type ex((int)(x * 1000), 1000);
        Exact_number_type ey((int)(y * 1000), 1000);
#endif
        Exact_FT fx(ex);
        Exact_FT fy(ey);
        m_array[old_size + i] = Exact_point_2(fx,fy);
      }
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }

    if (name == "fltPolarPoint") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 2;
      std::istringstream svalue(value, std::istringstream::in);
      int old_size = m_array.size();
      m_array.resize(old_size + size);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; i++) {
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
        Exact_number_type eu(static_cast<int>(utanhalf * 1000), 1000);
        Exact_FT ut(eu);
        Exact_FT ut_square = CGAL::square(ut);
        Exact_FT cosu = (1 - ut_square) / (1 + ut_square);
        Exact_FT sinu = 2 * ut / (1 + ut_square);

        float vtanhalf = tanf(v_rad * 0.5f);
        Exact_number_type ev(static_cast<int>(vtanhalf * 1000), 1000);
        Exact_FT vt(ev);
        Exact_FT vt_square = CGAL::square(vt);
        Exact_FT cosv = (1 - vt_square) / (1 + vt_square);
        Exact_FT sinv = 2 * vt / (1 + vt_square);

        Exact_FT fx = cosu * cosv;
        Exact_FT fy = cosu * sinv;
#endif

        m_array[old_size + i] = Exact_point_2(fx,fy);
      }
      elem->mark_delete(ai);
    }

    if (name == "ratPoint") {
      Uint num_values = get_num_values<Exact_number_type>(value);
      Uint size = num_values / 3;
      int old_size = m_array.size();
      m_array.resize(old_size + size);
      std::istringstream svalue(value, std::istringstream::in);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; ++i) svalue >> m_array[old_size + i];
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! Obtain the data size.
Uint Exact_coord_array_2d::data_size() const
{ return size() * sizeof(Vector2f); }

// \brief obtains the data.
const GLfloat* Exact_coord_array_2d::data() const
{
  if (m_dirty_data) clean_data();
  return (GLfloat*)(&(*(m_data.begin())));
}

//! \brief clean the raw data.
void Exact_coord_array_2d::clean_data() const
{
  m_dirty_data = false;
  m_data.resize(size());

  // Convert the exact points to approximate:
  std::vector<Vector2f>::iterator it = m_data.begin();
  CGAL::To_double<Exact_FT> todouble;
  for (Exact_point_const_iter eit = begin(); eit != end(); ++eit) {
    const Exact_point_2& p = *eit;
    it->set(static_cast<Float>(todouble(p.x())),
            static_cast<Float>(todouble(p.y())));
  }
}

//! \brief clears the array.
void Exact_coord_array_2d::clear()
{
  m_array.clear();
  m_data.clear();
  m_dirty_data = true;
}

SGAL_END_NAMESPACE
