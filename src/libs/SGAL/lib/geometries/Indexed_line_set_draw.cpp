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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#if (defined _MSC_VER)
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
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

void Indexed_line_set::draw_invalid()
{ SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "invalid\n"); }

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - Line
 */
void Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

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
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

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
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - Line
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

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
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - Line
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

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
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per line segment
 * Textute enabled      - false
 * Mode                 - lines
 */
void Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  Uint k = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_coord_index(m_normal_array, k++));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per line segment
 * Textute enabled      - false
 * Mode                 - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  Uint k = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_coord_index(m_color_array, k++));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per line segment
 * Textute enabled      - false
 * Mode                 - lines
 */
void Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_normal_array);

  Uint j = 0;
  Uint k = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_normal_index(m_normal_array, k++));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per line segment
 * Textute enabled      - false
 * Mode                 - lines
 */
void Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");

  SGAL_assertion(m_coord_array);
  SGAL_assertion(m_color_array);

  Uint j = 0;
  Uint k = 0;
  glBegin(GL_LINES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_color_index(m_color_array, k++));
    glVertex3fv(get_by_coord_index(m_coord_array, j));
    glVertex3fv(get_by_coord_index(m_coord_array, j+1));
    j += 2;
  }
  glEnd();
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TENO_MOLI_VANO(); invalid

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - lines
 */
void Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TENO_MOLI_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// Texture enabled:
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}


void Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOLI_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLI_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLI_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// Line strips:
// Texture disabled:
void Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TENO_MOLS_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TENO_MOLS_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// Texture enabled:
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOLS_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLS_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLS_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// Line loops:
// Texture disabled:
void Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TENO_MOLL_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TENO_MOLL_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// Texture enabled:
void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOLL_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLL_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLL_VANO(); invalid

void Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// void Indexed_line_set::draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO(); invalid

void Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

// Vertex Buffer:
void Indexed_line_set::draw_FAPV_VAYE()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FAPT_VAYE()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

void Indexed_line_set::draw_FAPM_VAYE()
{
  SGAL_TRACE_MSG(Trace::INDEXED_LINE_SET, "\n");
  SGAL_error_msg("Not implemented yet!");
}

SGAL_END_NAMESPACE
