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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *
 */

#ifndef SGAL_TEXT_SCREEN_HPP
#define SGAL_TEXT_SCREEN_HPP

#include <string>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Font_texture;
class Appearance;
class Material;

class Text_screen {
public:
  /*! Constructor */
  Text_screen();

  /*! Destructor */
  virtual ~Text_screen();

  // Clear all text lines.
  void clear();

  // Put string in specified line, replace string already present on this line.
  void put_string(int line, const std::string & str);

  // Add string to the end.
  void add_string(const std::string & str);

  // Draw all strings.
  void draw(Draw_action * draw_action);
   
protected:
  // Font used to draw strings.
  Font_texture * m_font;

  // Text lines.
  std::vector<std::string> m_lines;

private:
  Appearance * m_appearance;
  Material * m_material;
};

SGAL_END_NAMESPACE

#endif
