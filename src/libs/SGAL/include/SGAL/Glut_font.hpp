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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GLUT_FONT_HPP
#define SGAL_GLUT_FONT_HPP

/*! \file
 *
 */

#include "SGAL/Font.hpp"

SGAL_BEGIN_NAMESPACE

class Glut_font : public Font {
public:
  /*! Constructor */
  Glut_font(const std::string & family, Uint size = 8, Uint style = ANTIALIAS);

  /*! Destructor */
  virtual ~Glut_font();

  /*! Draw one string */
  virtual void draw_string(const std::string & str);

  /*! Obtain the width and height of the string */
  virtual void get_string_size(const std::string & str,
                               Uint & width, Uint & height) const;

  /*! Initialize the font */
  virtual void init();

  /*! Clear the font */
  virtual void clear();

private:
  void * m_glut_font;
};

SGAL_END_NAMESPACE

#endif
