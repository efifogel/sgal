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
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vertex_shader.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Vertex_shader::s_prototype(NULL);
const std::string Vertex_shader::s_tag = "VertexShader";

/*! Constructor */
Vertex_shader::Vertex_shader(Boolean proto) :  Shader(proto) { }

/*! Destructor */
Vertex_shader::~Vertex_shader() {}

/*! \brief applies the shader. */
void Vertex_shader::draw(Context* ctx)
{
  if (m_dirty) clean();

  // Do stuff
}

/*! \brief cleans (compiles, link, and download) the shader. */
void Vertex_shader::clean()
{
  // Do stuff

  m_dirty = false;
}

/*! \brief sets the attributes of the shader node. */
void Vertex_shader::set_attributes(Element* elem)
{
  Shader::set_attributes(elem);

  // Remove all the marked attributes:
  elem->delete_marked();
}

/*! \brief initializes the shader node prototype. */
void Vertex_shader::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Shader::get_prototype());
}

/*! \brief deletes the shader node prototype. */
void Vertex_shader::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the shader node prototype. */
Container_proto* Vertex_shader::get_prototype()
{
  if (!s_prototype) Vertex_shader::init_prototype();
  return s_prototype;
}

Boolean Vertex_shader::empty() { return true; }

SGAL_END_NAMESPACE
