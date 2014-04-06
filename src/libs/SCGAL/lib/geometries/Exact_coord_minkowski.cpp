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

#include "SCGAL/Exact_coord_minkowski.hpp"
#include "SCGAL/Exact_coord_array.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_coord_minkowski::s_tag = "ExactCoordinateMinkowski";
Container_proto* Exact_coord_minkowski::s_prototype(nullptr);

REGISTER_TO_FACTORY(Exact_coord_minkowski, "Exact_coord_minkowski");

//! \brief initializes the node prototype.
void Exact_coord_minkowski::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Coord_minkowski::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Exact_coord_minkowski::execute);

  // exactExecute
  Boolean_handle_function execute_func =
    static_cast<Boolean_handle_function>
    (&Exact_coord_minkowski::execute_handle);
  s_prototype->add_field_info(new SF_bool(EXACT_EXECUTE, "exactExecute",
                                          RULE_EXPOSED_FIELD,
                                          execute_func, exec_func));

  // // exactCoord1
  // Shared_container_handle_function coord_array1_func =
  //   reinterpret_cast<Shared_container_handle_function>
  //   (&Spherical_gaussian_map_marked_geo::coord_array1_handle);
  // s_prototype->add_field_info(new SF_shared_container(EXACT_COORD1,
  //                                                     "exactCoord1",
  //                                                     coord_array1_func,
  //                                                     exec_func));

  // // exactCoord2
  // Shared_container_handle_function coord_array2_func =
  //   reinterpret_cast<Shared_container_handle_function>
  //   (&Spherical_gaussian_map_marked_geo::coord_array2_handle);
  // s_prototype->add_field_info(new SF_shared_container(EXACT_COORD2,
  //                                                     "exactCoord2",
  //                                                     coord_array1_func,
  //                                                     exec_func));
}

//! \brief deletes the node prototype.
void Exact_coord_minkowski::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Exact_coord_minkowski::get_prototype()
{
  if (!s_prototype) Exact_coord_minkowski::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Exact_coord_minkowski::set_attributes(Element* elem)
{ Coord_minkowski::set_attributes(elem); }

#if 0
Attribute_list Exact_coord_minkowski::get_attributes()
{
  Attribute_list attrs;
  attrs = Coord_minkowski::get_attributes();
  return attrs;
}
#endif

//! \brief transforms the input vertices.
void Exact_coord_minkowski::execute(const Field_info* field_info)
{
  if (!m_enabled) return;

  boost::shared_ptr<Exact_coord_array> exact_coord_array1 =
    boost::dynamic_pointer_cast<Exact_coord_array>(m_coord_array1);
  boost::shared_ptr<Exact_coord_array> exact_coord_array2 =
    boost::dynamic_pointer_cast<Exact_coord_array>(m_coord_array2);

  if (!exact_coord_array1 || (exact_coord_array1->size() == 0) ||
      !exact_coord_array2 || (exact_coord_array2->size() == 0))
  {
    Coord_minkowski::execute(field_info);
    return;
  }

  Uint size1 = exact_coord_array1->size();
  Uint size2 = exact_coord_array2->size();
  Uint size = size1 * size2;

  boost::shared_ptr<Exact_coord_array> coord_array_changed;
  if (!m_coord_array_changed) {
    coord_array_changed.reset(new Exact_coord_array(size));
    m_coord_array_changed = coord_array_changed;
  }
  else {
    coord_array_changed =
      boost::dynamic_pointer_cast<Exact_coord_array>(m_coord_array_changed);
    coord_array_changed->resize(size);
  }

  Exact_coord_array::Exact_point_iter pi1;
  for (pi1 = exact_coord_array1->begin(); pi1 != exact_coord_array1->end();
       ++pi1)
  {
    const Exact_point_3& p1 = *pi1;
    Exact_vector_3 v(CGAL::ORIGIN, p1);
    Exact_coord_array::Exact_point_iter pi2;
    for (pi2 = exact_coord_array2->begin(); pi2 != exact_coord_array2->end();
         ++pi2)
    {
      const Exact_point_3& p2 = *pi2;
      coord_array_changed->push_back(p2 + v);
    }
  }

  Field* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();

  m_changed = true;
  Field* changed_field = get_field(CHANGED);
  if (changed_field) changed_field->cascade();
}

SGAL_END_NAMESPACE
