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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *  Represents the texture generation methods used.
 *
 */

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/GL_error.hpp"

SGAL_BEGIN_NAMESPACE

std::string Tex_gen::s_tag = "TexGen";

/*! Parameter-less constructor */
Tex_gen::Tex_gen(Boolean proto) :
  Container(proto),
  m_modes(OFF),
  m_modet(OFF),
  m_moder(OFF),
  m_modeq(OFF)
{
}

/*! Destructor */
Tex_gen::~Tex_gen() {}

/*!
 */
void Tex_gen::set_modes(Mode modes) { m_modes = modes; }

/*!
 */
void Tex_gen::set_modet(Mode modet) { m_modet = modet; }

/*!
 */
void Tex_gen::set_moder(Mode moder) { m_moder = moder; }

/*!
 */
void Tex_gen::set_modeq(Mode modeq) { m_modeq = modeq; }

/*! Call the texture generation calls
 */
void Tex_gen::draw(Context* /* context */) 
{
  // glTexGenfv(GL_S, GL_SPHERE_MAP, 0);
  // glTexGenfv(GL_T, GL_SPHERE_MAP, 0);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
};

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Tex_gen::set_attributes(Element* /* elem */)
{
  SGAL_assertion(0);
}

SGAL_END_NAMESPACE
