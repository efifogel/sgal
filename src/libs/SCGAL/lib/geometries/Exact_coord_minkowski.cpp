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
// $Source$
// $Revision: 7204 $
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

/*! The node prototype */
Container_proto * Exact_coord_minkowski::s_prototype = NULL;

/*! Register to the container factory */
REGISTER_TO_FACTORY(Exact_coord_minkowski, "Exact_coord_minkowski");

/*! Initialize the node prototype */
void Exact_coord_minkowski::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance:
  s_prototype = new Container_proto(Coord_minkowski::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Exact_coord_minkowski::execute);  

  s_prototype->add_field_info(new SF_bool(EXACT_EXECUTE, "exactExecute",
                                          get_member_offset(&m_execute),
                                          exec_func));

  SF_container * field;
  field = new SF_container(EXACT_COORD1, "exactCoord1",
                           get_member_offset(&m_coord_array1), exec_func);
  s_prototype->add_field_info(field);

  field = new SF_container(EXACT_COORD2, "exactCoord2",
                           get_member_offset(&m_coord_array2), exec_func);
  s_prototype->add_field_info(field);
}

/*! Delete the node prototype */
void Exact_coord_minkowski::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */  
Container_proto * Exact_coord_minkowski::get_prototype()
{
  if (s_prototype == NULL) Exact_coord_minkowski::init_prototype();
  return s_prototype;
}
  
/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 * \param elem lists of attribute names and values extracted from the input file
 * \param sg the scene graph
 */
void Exact_coord_minkowski::set_attributes(Element * elem)
{
  Coord_minkowski::set_attributes(elem);
}

#if 0
Attribute_list Exact_coord_minkowski::get_attributes()
{ 
  Attribute_list attrs;
  attrs = Coord_minkowski::get_attributes();
  return attrs; 
}
#endif

/*! Transform the input vertices and store the results in the output vertices
 * \param field_info the field information record
 */
void Exact_coord_minkowski::execute(Field_info * field_info)
{
  std::cout << "Exact_coord_minkowski::transform " << m_enabled << std::endl;
  
  if (!m_enabled) return;

  Exact_coord_array * exact_coord_array1 =
    dynamic_cast<Exact_coord_array *>(m_coord_array1);
  Exact_coord_array * exact_coord_array2 =
    dynamic_cast<Exact_coord_array *>(m_coord_array2);

  if (!exact_coord_array1 || (exact_coord_array1->size() == 0) ||
      !exact_coord_array2 || (exact_coord_array2->size() == 0))
  {
    Coord_minkowski::execute(field_info);
    return;
  }
  
  Uint size1 = exact_coord_array1->size();
  Uint size2 = exact_coord_array2->size();
  Uint size = size1 * size2;

  Exact_coord_array * coord_array_changed;
  if (!m_coord_array_changed) 
    m_coord_array_changed = coord_array_changed = new Exact_coord_array(size);
  else {
    coord_array_changed =
      dynamic_cast<Exact_coord_array *>(m_coord_array_changed);
    coord_array_changed->resize(size);
  }
  
  Exact_coord_array::Exact_point_iter pi1;
  for (pi1 = exact_coord_array1->begin(); pi1 != exact_coord_array1->end();
       ++pi1)
  {
    const Exact_point_3 & p1 = *pi1;
    Exact_vector_3 v(CGAL::ORIGIN, p1);
    Exact_coord_array::Exact_point_iter pi2;
    for (pi2 = exact_coord_array2->begin(); pi2 != exact_coord_array2->end();
         ++pi2)
    {
      const Exact_point_3 & p2 = *pi2;
      coord_array_changed->push_back(p2 + v);
    }
  }
  
  Field * coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();

  m_changed = true;
  Field * changed_field = get_field(CHANGED);
  if (changed_field) changed_field->cascade();
}

SGAL_END_NAMESPACE
