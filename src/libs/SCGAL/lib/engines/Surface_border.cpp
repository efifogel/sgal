// Copyright (c) 2016 Israel.
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

/*! \file
 *
 * Extract the border of a polygonal surface.
 */

#include <list>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Indexed_line_set.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Surface_border.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Surface_border::s_tag = "SurfaceBorder";
Container_proto* Surface_border::s_prototype(nullptr);

// Default values:

REGISTER_TO_FACTORY(Surface_border, "Surface_border");

//! \brief constructs
Surface_border::Surface_border(Boolean proto) :
  Node(proto),
  m_trigger(false)
{}

//! \brief destruct.
Surface_border::~Surface_border() {}

/*! \brief triggers the execution of the engine as a response to change in one
 * of the input fields.
 */
void Surface_border::trigger_changed(const Field_info* /* field_info */)
{ execute(); }

//! \brief initializes the container prototype.
void Surface_border::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func =
    static_cast<Execution_function>(&Surface_border::trigger_changed);

  auto trigger_func =
    static_cast<Boolean_handle_function>(&Surface_border::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          Field_info::RULE_IN,
                                          trigger_func,
                                          exec_func));

  // surface
  auto surface_func = reinterpret_cast<Shared_container_handle_function>
    (&Surface_border::surface_handle);
  s_prototype->add_field_info(new SF_shared_container(SURFACE, "surface",
                                                      Field_info::RULE_IN,
                                                      surface_func,
                                                      exec_func));
  // border
  auto border_func = reinterpret_cast<Shared_container_handle_function>
    (&Surface_border::border_handle);
  s_prototype->add_field_info(new SF_shared_container(BORDER, "border",
                                                      Field_info::RULE_OUT,
                                                      border_func));
}

//! \brief deletes the container prototype.
void Surface_border::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Surface_border::get_prototype()
{
  if (!s_prototype) Surface_border::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Surface_border::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);

    if (name == "surface") {
      auto mesh = boost::dynamic_pointer_cast<Mesh_set>(cont);
      set_surface(mesh);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "border") {
      auto border = boost::dynamic_pointer_cast<Indexed_line_set>(cont);
      set_border(border);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief executes the engine.
void Surface_border::execute()
{
  if (!m_surface) return;

  if (!m_border) m_border = Shared_indexed_line_set(new Indexed_line_set);

  auto geometry = boost::dynamic_pointer_cast<Exact_polyhedron_geo>(m_surface);
  if (geometry) {
    const auto& polyhedron = geometry->get_polyhedron();
    construct_border(polyhedron);
  }
  else {
    auto geometry = boost::dynamic_pointer_cast<Indexed_face_set>(m_surface);
    if (geometry) {
      const auto& polyhedron = geometry->get_polyhedron();
      construct_border(polyhedron);
    }
    else SGAL_error();
  }

  // Cascade the result field:
  auto* field = get_field(BORDER);
  if (field) field->cascade();
}

//! \brief writes this container.
void Surface_border::write(Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) Node::write(formatter);
}

SGAL_END_NAMESPACE
