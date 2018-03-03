// Copyright (c) 2004  Israel.
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
 * Encapsulates the OpenGL Multisample application. Specifies the
 * parameters needed to apply multisample.
 */

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Multisample.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Multisample::s_tag = "Multisample";
Container_proto* Multisample::s_prototype(NULL);

// Default values:
Boolean Multisample::s_def_enabled(true);
Uint Multisample::s_def_number_of_samples(4);

REGISTER_TO_FACTORY(Multisample, "Multisample");

/*! Constructor */
Multisample::Multisample(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_number_of_samples(s_def_number_of_samples)
{}

/*! brief initializess the node prototype */
void Multisample::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype:
  Execution_function exec_func;

  // enabled
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function enabled_handle =
    static_cast<Boolean_handle_function>(&Multisample::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          enabled_handle,
                                          exec_func));

  // numberOfSamples
  Uint_handle_function number_of_samples_handle =
    static_cast<Uint_handle_function>(&Multisample::number_of_samples_handle);
  s_prototype->add_field_info(new SF_uint(NUMBER_OF_SAMPLES,
                                          "numberOfSamples",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          number_of_samples_handle));

}

/*! \brief deletes the node prototype */
void Multisample::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype */
Container_proto* Multisample::get_prototype()
{
  if (!s_prototype) Multisample::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this node */
void Multisample::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      set_enabled(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "numberOfSamples") {
      set_number_of_samples(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
Attribute_list Multisample::get_attributes()
{
  Attribute_list attrs;
  Attribue attrib;
  char buf[32];

  attrs = Container::get_attributes();

  if (m_enabled != s_def_enabled) {
    attrib.first = "enabled";
    attrib.second = FALSE_STR;
    attrs.push_back(attrib);
  }
  if (m_delay != s_def_delay) {
    attrib.first = "numberOfSamples";
    sprintf(buf, "%d", m_delay);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
}
#endif

SGAL_END_NAMESPACE
