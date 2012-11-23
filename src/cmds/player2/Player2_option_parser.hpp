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
// $Revision: 1995 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef PLAYER2_OPTION_PARSER_HPP
#define PLAYER2_OPTION_PARSER_HPP

/*! \file
 */

#include "SGAL/basic.hpp"

#include "Player_option_parser.hpp"

class Player2_option_parser : public Player_option_parser {
public:
  /*! Constructor */
  Player2_option_parser();

  /*! Obtain the viewport-scale factor */
  SGAL::Float get_viewport_scale() const { return m_viewport_scale; }

  /*! Obtain the viewport margin */
  SGAL::Int get_viewport_margin() const { return m_viewport_margin; }
  
private:
  /*! The viewport scale factor */
  SGAL::Float m_viewport_scale;

  /*! The viewport margin */
  SGAL::Int m_viewport_margin;

  /*! The player option description */
  boost::program_options::options_description m_player2_opts;
};

#endif
