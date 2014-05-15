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

#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Tex_coord_array_3d.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

void Indexed_face_set::draw_invalid()
{ SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "invalid\n"); }

/****************      Polygons      ****************/

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPV_TENO_MOPO_VANO\n");

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glNormal3fv(get_by_coord_index(*m_normal_array, j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPV_TENO_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPV_TENO_MOPO_VANO\n");

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glNormal3fv(get_by_normal_index(*m_normal_array, j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPV_TENO_MOPO_VANO\n");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPT_TENO_MOPO_VANO\n");

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glNormal3fv(get(*m_normal_array,i));
    for (; m_coord_indices[j] != (Uint) -1; ++j)
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPT_TENO_MOPO_VANO\n");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPT_TENO_MOPO_VANO\n");

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glNormal3fv(get_by_normal_index(*m_normal_array, i));
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPT_TENO_MOPO_VANO\n");

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glColor3fv(get_by_coord_index(*m_color_array, i));
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - polygons
 */
 void Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPM_TENO_MOPO_VANO\n");

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glVertex3fv(get_by_coord_index(*m_coord_array,j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPM_TENO_MOPO_VANO\n");
}

// void draw_FSNO_FIYE_FAPM_TENO_MOPO_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TENO_MOPO_VANO();         invalid

// Texture enabled:
/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glNormal3fv(get_by_coord_index(*m_normal_array, j));
      glTexCoord2fv(get_by_coord_index(*tex_coord_array, j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO\n");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glNormal3fv(get_by_normal_index(*m_normal_array, j));
      glTexCoord2fv(get_by_coord_index(*tex_coord_array, j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glNormal3fv(get(*m_normal_array, i));
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glTexCoord2fv(get_by_coord_index(*tex_coord_array, j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glNormal3fv(get_by_normal_index(*m_normal_array, i));
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glTexCoord2fv(get_by_coord_index(*tex_coord_array, j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
  Fragment source      - normal
  Fragment indexed     - true
  Fragment attach      - per mesh
  Textute enabled      - true
  Texture indexed      - false
*/
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
  Fragment source      - color
  Fragment indexed     - true
  Fragment attach      - per mesh
  Textute enabled      - true
  Texture indexed      - false
*/
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VANO();         invalid

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glNormal3fv(get_by_coord_index(*m_normal_array,j));
      glTexCoord2fv(get_by_tex_coord_index(*tex_coord_array,j));
      glVertex3fv(get_by_coord_index(*m_coord_array, j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glNormal3fv(get_by_normal_index(*m_normal_array,j));
      glTexCoord2fv(get_by_tex_coord_index(*tex_coord_array,j));
      glVertex3fv(get_by_coord_index(*m_coord_array,j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glNormal3fv(get(*m_normal_array,i));
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glTexCoord2fv(get_by_tex_coord_index(*tex_coord_array,j));
      glVertex3fv(get_by_coord_index(*m_coord_array,j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; i++) {
    glBegin(GL_POLYGON);
    glNormal3fv(get_by_normal_index(*m_normal_array,i));
    for (; m_coord_indices[j] != (Uint) -1; ++j) {
      glTexCoord2fv(get_by_tex_coord_index(*tex_coord_array,j));
      glVertex3fv(get_by_coord_index(*m_coord_array,j));
    }
    glEnd();
    ++j;
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - polygons
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO();         invalid

/****************    Triangles    ****************/
/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPV_TENO_MOTR_VANO\n");

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPV_TENO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPV_TENO_MOTR_VANO\n");
  // std::cout << "FSNO_FIYE_FAPV_TENO_MOTR_VANO" << std::endl;

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    // std::cout << *((Vector3f*)(get_by_normal_index(*m_normal_array, j+0))) << ", "
    //           << *((Vector3f*)(get_by_normal_index(*m_normal_array, j+1))) << ", "
    //           << *((Vector3f*)(get_by_normal_index(*m_normal_array, j+2))) << std::endl;

    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPV_TENO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPT_TENO_MOTR_VANO\n");

  SGAL_assertion(m_normal_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(*m_normal_array, i));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPT_TENO_MOTR_VANO\n");

  SGAL_assertion(m_color_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get(*m_color_array, i));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPT_TENO_MOTR_VANO\n");

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array,i));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPT_TENO_MOTR_VANO\n");

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glColor3fv(get_by_flat_color_index(*m_color_array,i));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - triangles
 */
 void Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPM_TENO_MOTR_VANO\n");

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPM_TENO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TENO_MOTR_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TENO_MOTR_VANO();         invalid

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(*m_normal_array, i));

    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, i));

    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VANO();         invalid

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array,j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array,j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array,j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(*m_normal_array,i));

    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array,j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array,j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array,j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array,j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array,j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array,j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO\n");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_TRIANGLES);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, i));

    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - triangles
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO\n");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO();         invalid

/****************      Quads      ****************/

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPV_TENO_MOQU_VANO\n");

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPV_TENO_MOQU_VANO");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPV_TENO_MOQU_VANO");

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPV_TENO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPT_TENO_MOQU_VANO");

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(*m_normal_array,i));

    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPT_TENO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPT_TENO_MOQU_VANO");

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array,i));

    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPT_TENO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - quads
 */
 void Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPM_TENO_MOQU_VANO");

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPM_TENO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TENO_MOQU_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TENO_MOQU_VANO();         invalid

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(*m_normal_array,i));

    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, i));

    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VANO();         invalid

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_coord_index(*m_normal_array,j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, j));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get(*m_normal_array, i));

    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  Uint j = 0;
  glBegin(GL_QUADS);
  for (Uint i = 0; i < m_num_primitives; ++i) {
    glNormal3fv(get_by_flat_normal_index(*m_normal_array, i));

    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, j));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
  }
  glEnd();
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - quads
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO();         invalid

/**************** Triangle Strips ****************/

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPV_TENO_MOTS_VANO");

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    glBegin(GL_TRIANGLE_STRIP);
    for (Uint i = 0; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_coord_index(*m_normal_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPV_TENO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPV_TENO_MOTS_VANO");

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0] + 1;

  int normalInd = 0;
  int coordInd = 0;
  for (Uint strip = 1; strip < num_tri_strips; ++strip) {
    coordInd +=2;
    glBegin(GL_POLYGON);
    int numTri = m_tri_strip_lengths[strip] - 2;
    for (int i = 0; i < numTri/2; ++i) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      normalInd += 3;

      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      normalInd += 3;
    }
    if (numTri % 2) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      normalInd += 3;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - false
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPV_TENO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPT_TENO_MOTS_VANO");

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint coordInd = 0;
  int nind=0;
  for (Uint strip = 1; strip <= num_tri_strips; ++strip) {
    coordInd +=2;
    glBegin(GL_POLYGON);
    Uint numTri = m_tri_strip_lengths[strip] - 2;
    for (Uint i = 0; i < numTri/2; ++i) {
      glNormal3fv(get(*m_normal_array, nind));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      ++nind;

      glNormal3fv(get(*m_normal_array, nind));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      ++nind;
    }
    if (numTri%2) {
      glNormal3fv(get(*m_normal_array, nind));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      ++nind;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPT_TENO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
 void Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPT_TENO_MOTS_VANO");

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  GLint shade_model;
  glGetIntegerv(GL_SHADE_MODEL, &shade_model);
  glShadeModel(GL_FLAT);
  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    Uint tmp2 = tmp * 2;
    if (m_tri_strip_lengths[tmp] < 3) continue;
    glBegin(GL_TRIANGLE_STRIP);
    // draw the first two vertices first
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index+1));
    index += 2;
    // loop over the rest of the vertices
    for (Uint i = 2; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array,index - tmp2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
  glShadeModel(shade_model);
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FIYE_FAPT_TENO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FINO_FAPM_TENO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per mesh
 * Textute enabled      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSCO_FINO_FAPM_TENO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TENO_MOTS_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TENO_MOTS_VANO();         invalid

// Texture enabled:
/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    glBegin(GL_TRIANGLE_STRIP);
    for (Uint i = 0; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_coord_index(*m_normal_array, index));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0] + 1;
  Uint normalInd = 0;
  Uint coordInd = 0;
  for (Uint strip = 1; strip < num_tri_strips; ++strip) {
    coordInd +=2;
    glBegin(GL_POLYGON);
    Uint numTri = m_tri_strip_lengths[strip]-2;
    for (Uint i = 0; i < numTri/2; ++i) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+1));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      ++coordInd;
      normalInd += 3;

      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+1));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      coordInd++;
      normalInd += 3;
    }
    if (numTri%2) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+1));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, normalInd+2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      coordInd++;
      normalInd += 3;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  // int index = 0;

  Uint coordInd = 0;
  Uint nind = 0;
  for (Uint strip = 1; strip <= num_tri_strips; ++strip) {
    coordInd +=2;
    glBegin(GL_POLYGON);
    Uint numTri = m_tri_strip_lengths[strip] - 2;
    for (Uint i = 0; i < numTri/2; ++i) {
      glNormal3fv(get(*m_normal_array, nind));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      coordInd++;
      nind++;

      glNormal3fv(get(*m_normal_array, nind));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      coordInd++;
      nind++;
    }
    if (numTri % 2) {
      glNormal3fv(get(*m_normal_array, nind));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-2));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd-1));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd-1));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, coordInd));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, coordInd));
      coordInd++;
      nind++;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  GLint shade_model;
  glGetIntegerv(GL_SHADE_MODEL, &shade_model);
  glShadeModel(GL_FLAT);
  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    Uint tmp2 = 2*tmp;
    if (m_tri_strip_lengths[tmp] < 3) continue;
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    ++index;
    glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    ++index;
    for (Uint i = 2; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, index - tmp2));
      glTexCoord2fv(get_by_flat_coord_index(*tex_coord_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
  glShadeModel(shade_model);
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - false
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VANO();         invalid

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    glBegin(GL_TRIANGLE_STRIP);
    for (Uint i = 0; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_coord_index(*m_normal_array, index));
      glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    glBegin(GL_TRIANGLE_STRIP);
    for (Uint i = 0; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, index));
      glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per vertex
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  GLint shade_model;
  glGetIntegerv(GL_SHADE_MODEL, &shade_model);
  glShadeModel(GL_FLAT);
  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    Uint tmp2 = tmp * 2;
    if (m_tri_strip_lengths[tmp] < 3) continue;
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    ++index;
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    ++index;
    for (Uint i = 2; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get(*m_normal_array, index - tmp2));
      glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
  glShadeModel(shade_model);
}

/*!
 * Fragment source      - color
 * Fragment indexed     - false
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO");

  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);

  if (!m_coord_array->size() || !m_normal_array->size() ||
      !tex_coord_array->size() || !m_tri_strip_lengths)
    return;

  Uint num_tri_strips = m_tri_strip_lengths[0];
  Uint index = 0;

  GLint shade_model;
  glGetIntegerv(GL_SHADE_MODEL, &shade_model);
  glShadeModel(GL_FLAT);
  for (Uint strip = 0; strip < num_tri_strips; ++strip) {
    Uint tmp = strip+1;
    Uint tmp2 = tmp*2;
    if (m_tri_strip_lengths[tmp] < 3) continue;

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    ++index;
    glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
    glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
    ++index;
    for (Uint i = 2; i < m_tri_strip_lengths[tmp]; ++i) {
      glNormal3fv(get_by_flat_normal_index(*m_normal_array, index - tmp2));
      glTexCoord2fv(get_by_flat_tex_coord_index(*tex_coord_array, index));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, index));
      ++index;
    }
    glEnd();
  }
  glShadeModel(shade_model);
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per triangle
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - normal
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 */
void Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

/*!
 * Fragment source      - color
 * Fragment indexed     - true
 * Fragment attach      - per mesh
 * Textute enabled      - true
 * Texture indexed      - true
 * Mode                 - Triangle strips
 */
void Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET,
                 "FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

// void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO();         invalid
// void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO();         invalid

// Vertex Array:
// Triangles or Quads:
// Texture disabled:
void Indexed_face_set::draw_FAPV_VAYE()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FAPV_VAYE\n");

  Uint tcoords = num_tex_coordinates();

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

  static GLenum s_gl_modes[] = {
    GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
    GL_QUAD_STRIP, GL_QUADS,
    GL_POLYGON
  };

#if defined(GL_ARB_vertex_buffer_object)
  if ((Gfx_conf::get_instance()->is_vertex_buffer_object_supported()) &&
      (m_coord_buffer_id != 0))
  {
    if (resolve_fragment_source() == FS_NORMAL) {
      if (m_normal_buffer_id != 0) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_normal_buffer_id);
        glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
        glEnableClientState(GL_NORMAL_ARRAY);
      }
    }
    else {
      if (m_color_buffer_id != 0) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_color_buffer_id);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
        glEnableClientState(GL_COLOR_ARRAY);
      }
    }
    if ((m_tex_coord_buffer_id != 0) && (tcoords != 0)) {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_tex_coord_buffer_id);
      glTexCoordPointer(tcoords, GL_FLOAT, 0, BUFFER_OFFSET(0));
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_coord_buffer_id);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
  }
  else {
    if (resolve_fragment_source() == FS_NORMAL) {
      glNormalPointer(GL_FLOAT, 0, normal_data());
      glEnableClientState(GL_NORMAL_ARRAY);
    }
    else {
      glColorPointer(3, GL_FLOAT, 0, color_data());
      glEnableClientState(GL_COLOR_ARRAY);
    }
    if (tcoords != 0) {
      glTexCoordPointer(tcoords, GL_FLOAT, 0, tex_coord_data());
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glVertexPointer(3, GL_FLOAT, 0, coord_data());
  }
#else
  if (resolve_fragment_source() == FS_NORMAL) {
    glNormalPointer(GL_FLOAT, 0, normal_data());
    glEnableClientState(GL_NORMAL_ARRAY);
  }
  else {
    glColorPointer(3, GL_FLOAT, 0, color_data());
    glEnableClientState(GL_COLOR_ARRAY);
  }
  if (tcoords != 0) {
    glTexCoordPointer(tcoords, GL_FLOAT, 0, tex_coord_data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  glVertexPointer(3, GL_FLOAT, 0, coord_data());
#endif

  glEnableClientState(GL_VERTEX_ARRAY);

  const GLvoid* indices = indices_data();
  SGAL_assertion(m_primitive_type == PT_TRIANGLES ||
                 m_primitive_type == PT_QUADS);
  Uint count = m_num_primitives * ((m_primitive_type == PT_TRIANGLES) ? 3 : 4);
  glDrawElements(s_gl_modes[m_primitive_type], count, GL_UNSIGNED_INT,
                 indices);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

#if defined(GL_ARB_vertex_buffer_object)
  if (Gfx_conf::get_instance()->is_vertex_buffer_object_supported())
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#endif
}

void Indexed_face_set::draw_FAPT_VAYE()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FAPT_VAYE");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

void Indexed_face_set::draw_FAPM_VAYE()
{
  SGAL_TRACE_MSG(Trace::INDEXED_FACE_SET, "FAPM_VAYE");
  SGAL_assertion_msg(0, "Not implemented yet!");
}

SGAL_END_NAMESPACE
