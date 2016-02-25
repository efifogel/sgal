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
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Epec_plane_array.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Epec_plane_array::s_tag = "ExactPlane";
Container_proto* Epec_plane_array::s_prototype(nullptr);

REGISTER_TO_FACTORY(Epec_plane_array, "Epec_plane_array");

//! \brief construct.
Epec_plane_array::Epec_plane_array(Boolean proto) : Container(proto) {}

//! \brief constructs.
Epec_plane_array::Epec_plane_array(Uint n) { m_array.resize(n); }

//! \brief destructors.
Epec_plane_array::~Epec_plane_array() { clear(); }

//! \brief initializes the node prototype.
void Epec_plane_array::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());
}

//! \brief deletes the node prototype.
void Epec_plane_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Epec_plane_array::get_prototype()
{
  if (!s_prototype) Epec_plane_array::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Epec_plane_array::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "fltPlane") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 4;
      std::istringstream svalue(value, std::istringstream::in);
      m_array.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; i++) {
        float a, b, c, d;
        svalue >> a >> b >> c >> d;
#if SCGAL_KERNEL == SCGAL_EXACT_PREDICATES_EXACT_CONSTRUCTIONS_KERNEL
        CGAL::Gmpq ea((int)(a * 1000), 1000);
        CGAL::Gmpq eb((int)(b * 1000), 1000);
        CGAL::Gmpq ec((int)(c * 1000), 1000);
        CGAL::Gmpq ed((int)(d * 1000), 1000);
#elif SCGAL_NT == SCGAL_CGAL_GMPZ_NT
        Exact_number_type ea(((int)(a * 1000)) / 1000);
        Exact_number_type eb(((int)(b * 1000)) / 1000);
        Exact_number_type ec(((int)(c * 1000)) / 1000);
        Exact_number_type ed(((int)(d * 1000)) / 1000);
#elif SCGAL_NT == SCGAL_DOUBLE_NT
        not implemented
#else
        Exact_number_type ea((int)(a * 1000), 1000);
        Exact_number_type eb((int)(b * 1000), 1000);
        Exact_number_type ec((int)(c * 1000), 1000);
        Exact_number_type ed((int)(d * 1000), 1000);
#endif
        Epec_FT fa(ea);
        Epec_FT fb(eb);
        Epec_FT fc(ec);
        Epec_FT fd(ed);
        m_array[i] = Epec_plane_3(fa,fb,fc,fd);
      }
      //! \todo sg->get_stats().AddNumVertices(size);
      elem->mark_delete(ai);
    }
    if (name == "ratPlane") {
      Uint num_values = get_num_values<Epec_FT>(value);
      Uint size = num_values / 4;
      m_array.resize(size);
      std::istringstream svalue(value, std::istringstream::in);
      //! svalue.seekg(0); why this doesn't work?
      for (Uint i = 0; i < size; ++i) svalue >> m_array[i];
      elem->mark_delete(ai);
    }
  }

#if 0
  if (size() > 0) {
    // Convert the exact planes to approximate:
    Coord_array* coord_array = this;
    coord_array->resize(size());
    Uint i = 0;
    for (auto eit = begin(); eit != end(); ++eit) {
      Exact_plane_3& p = *eit;
      (*coord_array)[i++].set(static_cast<float>(CGAL::to_double(p.x())),
                              static_cast<float>(CGAL::to_double(p.y())),
                              static_cast<float>(CGAL::to_double(p.z())));
    }
  }
#endif

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Epec_plane_array::get_attributes()
{
  Attribute_list attrs;
  attrs = Plane_array::get_attributes();
  return attrs;
}
#endif

SGAL_END_NAMESPACE
