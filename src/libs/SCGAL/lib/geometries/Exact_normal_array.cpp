// Copyright (c) 2013 Israel.
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

#include "SCGAL/Exact_normal_array.hpp"

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

SGAL_BEGIN_NAMESPACE

const std::string Exact_normal_array::s_tag = "ExactNormal";
Container_proto* Exact_normal_array::s_prototype(nullptr);

REGISTER_TO_FACTORY(Exact_normal_array, "Exact_normal_array");

//! \brief initializes the node prototype.
void Exact_normal_array::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Normal_array::get_prototype());
}

//! \brief deletes the node prototype.
void Exact_normal_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Exact_normal_array::get_prototype()
{
  if (!s_prototype) Exact_normal_array::init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 */
void Exact_normal_array::set_attributes(Element* elem)
{
  // We clear the array now and resize it later when we need it.
  m_array.clear();

  Normal_array::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "fltVector") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 3;
      std::istringstream svalue(value, std::istringstream::in);
      int old_size = m_array.size();
      m_array.resize(old_size + size);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; i++) {
        float x, y, z;
        svalue >> x >> y >> z;
#if SCGAL_KERNEL == SCGAL_EXACT_PREDICATES_EXACT_CONSTRUCTIONS_KERNEL
        CGAL::Gmpq ex((int)(x * 1000), 1000);
        CGAL::Gmpq ey((int)(y * 1000), 1000);
        CGAL::Gmpq ez((int)(z * 1000), 1000);
#elif SCGAL_NT == SCGAL_CGAL_GMPZ_NT
        Exact_number_type ex(((int)(x * 1000)) / 1000);
        Exact_number_type ey(((int)(y * 1000)) / 1000);
        Exact_number_type ez(((int)(z * 1000)) / 1000);
#elif SCGAL_NT == SCGAL_DOUBLE_NT
        not implemented
#else
        Exact_number_type ex((int)(x * 1000), 1000);
        Exact_number_type ey((int)(y * 1000), 1000);
        Exact_number_type ez((int)(z * 1000), 1000);
#endif
        Exact_FT fx(ex);
        Exact_FT fy(ey);
        Exact_FT fz(ez);
        m_array[old_size + i] = Exact_vector_3(fx,fy,fz);
      }
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }

    if (name == "ratVector") {
      Uint num_values = get_num_values<Exact_FT>(value);
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

  if (size() > 0) {
    // Convert the exact vectors to approximate:
    Normal_array* normal_array = this;
    normal_array->resize(size());
    Uint i = 0;
    CGAL::To_double<Exact_FT> todouble;
    for (Exact_vector_iter eit = begin(); eit != end(); ++eit) {
      Exact_vector_3& d = *eit;
      (*normal_array)[i++].set(static_cast<float>(todouble(d.x())),
                               static_cast<float>(todouble(d.y())),
                               static_cast<float>(todouble(d.z())));
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Exact_normal_array::get_attributes()
{
  Attribute_list attrs;
  attrs = Normal_array::get_attributes();
  return attrs;
}
#endif

SGAL_END_NAMESPACE
