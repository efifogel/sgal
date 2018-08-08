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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Tracer.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

void Indexed_line_set::draw_invalid()
{ SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "invalid\n"); }

/*!
 * Fragment attach  - none
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FANO_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "draw_FANO_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FINO_FAPV_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glNormal3fv(get_by_coord_index(m_normal_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FINO_FAPV_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glColor3fv(get_by_coord_index(m_color_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FIYE_FAPV_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glNormal3fv(get_by_normal_index(m_normal_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FIYE_FAPV_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glColor3fv(get_by_color_index(m_color_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line segment
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FINO_FAPL_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line segment
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FINO_FAPL_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line segment
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FIYE_FAPL_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line segment
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FIYE_FAPL_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TENO_MOLI_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FINO_FAPM_TENO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TENO_MOLI_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLI_VANO(); invalid

// Texture enabled:

/*!
 * Fragment attach  - none
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FANO_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glNormal3fv(get_by_coord_index(m_normal_array, j+1));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glColor3fv(get_by_coord_index(m_color_array, j+1));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glNormal3fv(get_by_normal_index(m_normal_array, j+1));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glColor3fv(get_by_color_index(m_color_array, j+1));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOLI_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLI_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO(); invalid

/*!
 * Fragment attach  - none
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FANO_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glNormal3fv(get_by_coord_index(m_normal_array, j+1));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glColor3fv(get_by_coord_index(m_color_array, j+1));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glNormal3fv(get_by_normal_index(m_normal_array, j+1));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glColor3fv(get_by_color_index(m_color_array, j+1));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line segment
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLI_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j+1));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO(); invalid

// Line strips:
// Texture disabled:

/*!
 * Fragment attach  - none
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FANO_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);

  Uint j = 0;
  for (auto i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_coord_index(m_normal_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Textute enabled  - false
 * Fragment attach  - per vertex
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "draw_FSCO_FINO_FAPV_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_coord_index(m_color_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_normal_index(m_normal_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_color_index(m_color_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line strip
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(m_normal_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line strip
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get(m_color_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line strip
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line strip
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TENO_MOLS_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPM_TENO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TENO_MOLS_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLS_VANO(); invalid

// Texture enabled:

/*!
 * Fragment attach  - none
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FANO_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_coord_index(m_normal_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_coord_index(m_color_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_normal_index(m_normal_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_color_index(m_color_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOLS_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per mesh
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "draw_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLS_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO(); invalid

/*!
 * Fragment attach  - none
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FANO_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_coord_index(m_normal_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_coord_index(m_color_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_normal_index(m_normal_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_color_index(m_color_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line strip
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLS_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line strips
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_STRIP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO(); invalid

// Line loops:
// Texture disabled:

/*!
 * Fragment attach  - none
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FANO_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);

  Uint j = 0;
  for (auto i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FINO_FAPV_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_coord_index(m_normal_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FINO_FAPV_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_coord_index(m_color_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FIYE_FAPV_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_normal_index(m_normal_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FIYE_FAPV_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_color_index(m_color_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line loop
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FINO_FAPL_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line loop
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FINO_FAPL_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line loop
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSNO_FIYE_FAPL_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line loop
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FIYE_FAPL_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TENO_MOLL_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FSCO_FINO_FAPM_TENO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j)
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TENO_MOLL_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLL_VANO(); invalid

// Texture enabled:

/*!
 * Fragment attach  - none
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FANO_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_coord_index(m_normal_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_coord_index(m_color_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_normal_index(m_normal_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_color_index(m_color_array, j));
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOLL_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - true
 * Texture indexed  - false
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLL_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO(); invalid

/*!
 * Fragment attach  - none
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FANO_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FANO_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_coord_index(m_normal_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_coord_index(m_color_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glNormal3fv(get_by_normal_index(m_normal_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per vertex
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glColor3fv(get_by_color_index(m_color_array, j));
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - false
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FINO_FAPL_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FINO_FAPL_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPL_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPL_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - normal
 * Fragment indexed - true
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSNO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source  - color
 * Fragment indexed - true
 * Fragment attach  - per line loop
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, i));
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLL_VANO(); invalid

/*!
 * Fragment source  - color
 * Fragment indexed - false
 * Fragment attach  - per mesh
 * Textute enabled  - true
 * Texture indexed  - true
 * Mode             - line loops
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);
  SGAL_assertion(m_tex_coord_array);

  glColor3fv(get(m_color_array, 0));
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_LINE_LOOP);
    for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
      glTexCoord2fv(get_by_tex_coord_index(m_tex_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO(); invalid
// void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO(); invalid

// Vertex Buffer:

void Indexed_line_set::draw_FAPV_VAYE()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FAPV_VAYE\n");

  SGAL_assertion(m_coord_array);
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FAPL_VAYE()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FAPL_VAYE\n");

  SGAL_assertion(m_coord_array);
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FAPM_VAYE()
{
  SGAL_TRACE_MSG(Tracer::INDEXED_LINE_SET, "FAPM_VAYE\n");

  SGAL_assertion(m_coord_array);
  SGAL_error_msg("Not implemented yet!");
}

SGAL_END_NAMESPACE
