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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Implementation of TextScreen class.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Font_texture.hpp"
#include "SGAL/Text_screen.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Text_screen::Text_screen()
{
  m_font = 0;
  m_font = new Font_texture("fixedsys", 8);

  m_appearance = new Appearance();
  m_appearance->set_light_enable(SGAL_FALSE);
  m_appearance->set_depth_enable(SGAL_FALSE);
  m_appearance->set_poly_mode(Gfx::FILL_PMODE);
  m_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
  m_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
  m_material = new Material();
  m_material->set_diffuse_color(1, 1, 1);
  m_appearance->set_material(m_material);
  m_font->set_appearance(m_appearance);
  m_font->init();
}

/*! Destructor */
Text_screen::~Text_screen()
{
  DELETE_OBJECT(m_font);
  DELETE_OBJECT(m_appearance);
  DELETE_OBJECT(m_material);
}

// Put string in specified line, replace string already present on this line.
void Text_screen::put_string(int line, const std::string & str)
{
  SGAL_assertion(line >= 0 && line < 1024);
  if (line >= m_lines.size()) {
    m_lines.resize(line+1);
  }
  m_lines[line] = str;
}

/*!
 */
void Text_screen::clear()
{
  m_lines.clear();
}

/*!
 */
void Text_screen::add_string(const std::string &str)
{
  m_lines.push_back(str);
}

/*! Draw all strings.
 */
void Text_screen::draw(Draw_action * draw_action)
{
  if (m_lines.empty()) return;
  if (!draw_action) return;

  Context * context = draw_action->get_context();
  if (!context) return;

  if (!m_font) return;

  Int vx,vy,vw,vh;
  context->get_viewport(vx,vy,vw,vh);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  GLboolean bCull = glIsEnabled(GL_CULL_FACE);
  if (bCull) glDisable(GL_CULL_FACE);

  glOrtho(vx, vx + vw, vy,vy + vh, -1, 1);
  
  m_font->DrawAppearance(draw_action);

  // Draw text lines.
  float px,py;
  int sx,sy;

  px = 0;
  py = vy + vh;
  m_font->get_text_size("Iy", sx, sy);
  String text;
  for (int i = 0; i < m_lines.size(); i++)
  {
    if (!m_lines[i].empty()) {
      m_font->draw_string(px, py, m_lines[i]);
      py -= sy;
    }
  }

  if (bCull) glEnable(GL_CULL_FACE);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

SGAL_END_NAMESPACE
