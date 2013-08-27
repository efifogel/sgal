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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Simulation.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Simulation::s_tag = "Simulation";
Container_proto* Simulation::s_prototype(NULL);

REGISTER_TO_FACTORY(Simulation, "Simulation");

/*! \brief constructor */
Simulation::Simulation(Boolean proto) :
  Container(proto),
  m_start(false)
{}

/*! \brief initializes the node prototype. */
void Simulation::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // start
  s_prototype->add_field_info(new SF_bool(START, "start",
                                          get_member_offset(&m_start)));
}

/*! \brief deletes the node prototype. */
void Simulation::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Simulation::get_prototype()
{
  if (!s_prototype) Simulation::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Simulation::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
}

/*! \brief adds the container to a given scene. */
void Simulation::add_to_scene(Scene_graph* sg) { sg->add_simulation(this); }

/*! Start simulation */
void Simulation::start()
{
  Field* start_field = get_field(START);
  if (start_field) start_field->cascade();
}

SGAL_END_NAMESPACE

