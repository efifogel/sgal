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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Composed_shader.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Composed_shader::s_prototype(nullptr);
const std::string Composed_shader::s_tag = "ComposedShader";

/*! Constructor */
Composed_shader::Composed_shader(Boolean proto) :  Container(proto) { }

/*! Destructor */
Composed_shader::~Composed_shader() {}

/*! \brief applies the shader. */
void Composed_shader::draw(Context* /* context */)
{
  if (m_dirty) clean();

  // Do stuff
}

/*! \brief cleans (compiles, link, and download) the shader. */
void Composed_shader::clean()
{
  // Do stuff

  m_dirty = false;
}

/*! \brief sets the attributes of the shader node. */
void Composed_shader::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "language") {
      set_language(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

/*! \brief initializes the shader node prototype. */
void Composed_shader::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  String_handle_function language_func =
    static_cast<String_handle_function>(&Composed_shader::language_handle);
  s_prototype->add_field_info(new SF_string(LANGUAGE, "language",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            language_func));
}

/*! \brief deletes the shader node prototype. */
void Composed_shader::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

/*! \brief obtains the shader node prototype. */
Container_proto* Composed_shader::get_prototype()
{
  if (!s_prototype) Composed_shader::init_prototype();
  return s_prototype;
}

/*! \brief sets the language. */
void Composed_shader::set_language(const std::string& language)
{
  m_language = language;
  m_dirty = true;
}

SGAL_END_NAMESPACE
