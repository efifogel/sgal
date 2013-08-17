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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>

#include "SGAL/Glut_font.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Glut_font::Glut_font(const std::string & family, Uint size, Uint style) :
  Font(style),
  m_glut_font(NULL)
{
}

/*! Destructor */
Glut_font::~Glut_font()
{
}

/*! Draw one string */
void Glut_font::draw_string(const std::string & str)
{
  // Init:
  GLdouble model_matrix[16];
  GLdouble proj_matrix[16];
  GLint viewport[4];  
  glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
  glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Project vector v(0,0,0) to px,py,pz.
  GLdouble px, py, pz;
  gluProject(0, 0, 0, model_matrix, proj_matrix, viewport, &px, &py, &pz);
  // std::cout << "x,y,z: " << px << "," << py << "," << pz << std::endl;
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(viewport[0], viewport[0] + viewport[2],
          viewport[1], viewport[1] + viewport[3], -1, 1);

  // Set raster position:
  Uint width, height;
  get_string_size(str, width, height);
  glRasterPos2f(px - width * 0.5f, py - height * 0.5f);

  // Draw:
  const char * c = '\0';
  for (c = str.c_str(); *c != '\0'; ++c)
    glutBitmapCharacter(m_glut_font, *c);

  // Clear:
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

/*! Obtain the width and height of the string */
void Glut_font::get_string_size(const std::string & str,
                                Uint & width, Uint & height) const
{
  width = 0;
  const char * c = '\0';
  for (c = str.c_str(); *c != '\0'; ++c)
    width += glutBitmapWidth(m_glut_font, *c);
  height = 0;
}

/*! Initialize the font */
void Glut_font::init()
{
#if 1
  m_glut_font = GLUT_BITMAP_9_BY_15;
  m_glut_font = GLUT_BITMAP_8_BY_13;
  m_glut_font = GLUT_BITMAP_TIMES_ROMAN_10;
  m_glut_font = GLUT_BITMAP_TIMES_ROMAN_24;
  m_glut_font = GLUT_BITMAP_HELVETICA_10;
  m_glut_font = GLUT_BITMAP_HELVETICA_12;
  m_glut_font = GLUT_BITMAP_HELVETICA_18;
#endif
  m_glut_font = GLUT_BITMAP_HELVETICA_18;
}

/*! Clear the font */
void Glut_font::clear()
{
}

SGAL_END_NAMESPACE
