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
// $Id: Exact_coord2_array.cpp 7204 2009-01-24 21:43:15Z efif $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <CGAL/basic.h>
#include <CGAL/number_utils_classes.h>

#include "SCGAL/Exact_coord2_array.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/Geo_set.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_coord2_array::s_tag = "ExactCoordinate2";
Container_proto * Exact_coord2_array::s_prototype(NULL);

REGISTER_TO_FACTORY(Exact_coord2_array, "Exact_coord2_array");

/*! \brief initializes the node prototype */
void Exact_coord2_array::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());
}

/*! \brief deletes the node prototype */
void Exact_coord2_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype */
Container_proto * Exact_coord2_array::get_prototype()
{
  if (s_prototype == NULL) Exact_coord2_array::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from an input file */
void Exact_coord2_array::set_attributes(Element * elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "fltPoint") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 2;
      std::istringstream svalue(value, std::istringstream::in);
      m_array.resize(size);
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
        m_array[i] = Exact_point_2(fx,fy);
      }
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }

    if (name == "rationalPoint") {
      Uint num_values = get_num_values<Exact_number_type>(value);
      Uint size = num_values / 2;
      m_array.resize(size);
      std::istringstream svalue(value, std::istringstream::in);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; ++i) svalue >> m_array[i];
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Exact_coord2_array::get_attributes()
{
  Attribute_list attrs;
  attrs = Container::get_attributes();
  return attrs;
}
#endif

SGAL_END_NAMESPACE
