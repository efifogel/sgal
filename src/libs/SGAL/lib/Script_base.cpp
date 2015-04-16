// Copyright (c) 2014 Israel.
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

#include <regex>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Script_base.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Tick_event.hpp"

SGAL_BEGIN_NAMESPACE

const Boolean Script_base::s_def_direct_output(false);
const Boolean Script_base::s_def_must_evaluate(false);

Container_proto* Script_base::s_prototype(nullptr);

//! \brief constructs.
Script_base::Script_base(Boolean proto) :
  Container(proto),
  m_direct_output(s_def_direct_output),
  m_must_evaluate(s_def_must_evaluate),
  m_protocol(PROTOCOL_INVALID)
{ if (!proto) Tick_event::doregister(this); }

//! \brief destructs.
Script_base::~Script_base()
{ Tick_event::unregister(this); }

//! \brief initializes the container prototype.
void Script_base::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  String_handle_function url_func =
    static_cast<String_handle_function>(&Script_base::url_handle);
  s_prototype->add_field_info(new SF_string(URL, "url",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            url_func));

  Boolean_handle_function direct_output_func =
    static_cast<Boolean_handle_function>(&Script_base::direct_output_handle);
  s_prototype->add_field_info(new SF_bool(DIRECT_OUTPUT, "directOutput",
                                          Field_info::RULE_FIELD,
                                          direct_output_func,
                                          s_def_direct_output));

  Boolean_handle_function must_evaluate_func =
    static_cast<Boolean_handle_function>(&Script_base::must_evaluate_handle);
  s_prototype->add_field_info(new SF_bool(MUST_EVALUATE, "mustEvaluate",
                                          Field_info::RULE_FIELD,
                                          must_evaluate_func,
                                          s_def_must_evaluate));
}

//! \brief deletes the container prototype.
void Script_base::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Script_base::get_prototype()
{
  if (!s_prototype) Script_base::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Script_base::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "url") {
      std::string url = strip_double_quotes(value);
      set_url(url);
      url.clear();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "directOutput") {
      if (compare_to_true(value)) set_direct_output();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "mustEvaluate") {
      if (compare_to_true(value)) set_must_evaluate();
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Script_base::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Container::get_attributes();

  attrib.first = "url";
  attrib.second = m_url;
  attribs.push_back(attrib);

  return attribs;
}
#endif

//! \brief handles tick events.
void Script_base::handle(Tick_event* event) { m_time = event->get_sim_time(); }

//! \brief prints an identification message.
void Script_base::identify() { std::cout << "Agent: Script" << std::endl; }

SGAL_END_NAMESPACE
