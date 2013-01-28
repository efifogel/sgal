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
// $Revision: 5647 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FONT_HPP
#define SGAL_FONT_HPP

/*! \file
 *
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Context;

class Font {
public:
  /*! Constructor */
  Font(const std::string& name, Boolean antialias = true,
       Boolean left_to_right = true,
       Boolean top_to_bottom = true);
  
  /*! Destructor */
  virtual ~Font() {}

  /*! Initialize the representation. */
  virtual void init() = 0;
  
  /*! Clear the font. */
  virtual void clear() {}

  /*! Draw one string.
   * \param str the string.
   * \param size the nominal height, in the local coordinate system.
   */
  virtual void draw_string(Context* context, const std::string& str,
                           Float size) = 0;

  /*! Obtain the width and height of the string. */
  virtual void get_string_size(const std::string& str,
                               Uint& width, Uint& height) = 0;

  /*! Obtain the flag that indicates whether the font is antialiased.
   * \return the flag that indicates whether the font is antialiased.
   */
  Boolean get_antialias() const { return m_antialias; }

  /*! Obtain the flag that indicates the text horizontal direction.
   * \return the flag that indicates the text horizontal direction.
   */
  Boolean get_left_to_right() const { return m_left_to_right; }

  /*! Obtain the flag that indicates the text vertical direction.
   * \return the flag that indicates the text vertical direction.
   */
  Boolean get_top_to_bottom() const { return m_top_to_bottom; }
  
protected:
  /*! The font name. */ 
  std::string m_name;

  /*! Indicates whether the font is antialiased. */
  Boolean m_antialias;

  /*! Indicates the horizontal direction of the text. */
  Boolean m_left_to_right;

  /*! Indicates the vertical direction of the text. */
  Boolean m_top_to_bottom;
};

SGAL_END_NAMESPACE

#endif
