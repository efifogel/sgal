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
#include "SGAL/Shader.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Shader::s_prototype(nullptr);

/*! Constructor */
Shader::Shader(Boolean proto) :  Container(proto) { }

/*! Destructor */
Shader::~Shader() {}

/*! \brief applies the shader. */
void Shader::draw(Context* /* context */)
{
  if (m_dirty) clean();

  // Do stuff
}

/*! \brief cleans (compiles, link, and download) the shader. */
void Shader::clean()
{
  // Do stuff

  m_dirty = false;
}

/*! \brief sets the attributes of the shader node. */
void Shader::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "url") {
      std::string url = strip_double_quotes(value);
      set_url(url);
      url.clear();
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

/*! \brief initializes the shader node prototype. */
void Shader::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();
}

/*! \brief deletes the shader node prototype. */
void Shader::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;

  // Add the field-info records to the prototype:
  // url
  String_handle_function url_func =
    static_cast<String_handle_function>(&Shader::url_handle);
  s_prototype->add_field_info(new SF_string(URL, "url",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            url_func));
}

/*! \brief obtains the shader node prototype. */
Container_proto* Shader::get_prototype()
{
  if (!s_prototype) Shader::init_prototype();
  return s_prototype;
}

/*! \brief sets the URL. */
void Shader::set_url(const std::string& url)
{
  m_url = url;
  m_dirty = true;
}

SGAL_END_NAMESPACE
