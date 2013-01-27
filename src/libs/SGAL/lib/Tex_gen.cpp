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
  m_mode_s(OFF),
  m_mode_t(OFF),
  m_mode_r(OFF),
  m_mode_q(OFF)
{}

/*! Destructor */
Tex_gen::~Tex_gen() {}

/*! \brief applies the texture generation attribute. */
void Tex_gen::draw(Context* /* context */) 
{
  // glTexGenfv(GL_S, GL_SPHERE_MAP, 0);
  // glTexGenfv(GL_T, GL_SPHERE_MAP, 0);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
};

/*! \brief sets the attributes of the texture generation attribute. */
void Tex_gen::set_attributes(Element* /* elem */) { SGAL_assertion(0); }

/*! \brief sets the s-texture coordinate generation function. */
void Tex_gen::set_mode_s(Mode mode) { m_mode_s = mode; }

/*! \brief sets the t-texture coordinate generation function. */
void Tex_gen::set_mode_t(Mode mode) { m_mode_t = mode; }

/*! \brief sets the r-texture coordinate generation function. */
void Tex_gen::set_mode_r(Mode mode) { m_mode_r = mode; }

/*! \brief sets the q-texture coordinate generation function. */
void Tex_gen::set_mode_q(Mode mode) { m_mode_q = mode; }

/*! \brief sets the s-texture coordinate generation reference-plane. */
void Tex_gen::set_plane_s(const Plane& plane) { m_plane_s.set(plane); }

/*! \brief sets the t-texture coordinate generation reference-plane. */
void Tex_gen::set_plane_t(const Plane& plane) { m_plane_t.set(plane); }

/*! \brief sets the r-texture coordinate generation reference-plane. */
void Tex_gen::set_plane_r(const Plane& plane) { m_plane_r.set(plane); }

/*! \brief sets the q-texture coordinate generation reference-plane. */
void Tex_gen::set_plane_q(const Plane& plane) { m_plane_q.set(plane); }

SGAL_END_NAMESPACE
