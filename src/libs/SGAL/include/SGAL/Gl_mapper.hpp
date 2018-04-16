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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GL_MAPPER_HPP
#define SGAL_GL_MAPPER_HPP

#if defined(_WIN32)
#include <windows.h>
#endif
#include <map>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Gl_mapper {
private:
  typedef std::map<GLenum, const char*> Name_map;
  typedef Name_map::const_iterator      Name_const_iter;

  /*! The singletone. */
  static Gl_mapper* s_instance;

  /*! The search data structure. */
  Name_map m_gl_token_names;

  /*! Private Constructor. */
  Gl_mapper();

  /*! Find the name of a token. */
  const char* find_name(GLenum num);

  /*! Find the name of a Boolean token. */
  const char* find_boolean_name(GLboolean flag);

  /*! Check whether openGl errors have occured. */
  void check_error();

public:
  /*! Obtain the singletone. */
  static Gl_mapper* get_instance();

  /*! Destructor. */
  ~Gl_mapper();

  /*! Find the name of a token. */
  static const char* find(GLenum num);

  /*! Find the name of a token. */
  static const char* find_boolean(GLboolean flag);

  /*! Check for GL errors. */
  static void check();
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief finds the name of a Boolean token.
inline const char* Gl_mapper::find_boolean_name(GLboolean flag)
{ return (flag == GL_FALSE) ? "GL_FALSE" : "GL_TRUE"; }

//! \brief finds the name of a token.
inline const char* Gl_mapper::find(GLenum num)
{ return get_instance()->find_name(num); }

//! \brief finds the name of a token.
inline const char* Gl_mapper::find_boolean(GLboolean flag)
{ return get_instance()->find_boolean_name(flag); }

//! \brief checks for GL errors.
inline void Gl_mapper::check() { get_instance()->check_error(); }

SGAL_END_NAMESPACE

#endif
