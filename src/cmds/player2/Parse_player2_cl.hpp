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
// $Revision: 1314 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef PARSE_PLAYER2_CL_HPP
#define PARSE_PLAYER2_CL_HPP

#include "Parse_player_cl.hpp"

/*! Scene command-line parser */
class Parse_player2_cl : public Parse_player_cl {
public:
  enum Viewport_options {
    VO_SCALE = 0,
    VO_MARGIN,
    NUM_VIEWPORT_OPTS
  };
  
  /*! Constructor */
  Parse_player2_cl();
  
  /*! Destructor */
  virtual ~Parse_player2_cl() {}

  /*! \brief prints help message */
  virtual void print_help();

  /*! \brief parses the command line */
  virtual SGAL::Int operator()();

  /*! Obtain the viewport-scale factor */
  SGAL::Float get_viewport_scale() const { return m_viewport_scale; }

  /*! Obtain the viewport margin */
  SGAL::Int get_viewport_margin() const { return m_viewport_margin; }
  
private:
  /*! Viewport-option names */
  static char * s_viewport_opts[];

  /*! The viewport scale factor */
  SGAL::Float m_viewport_scale;

  /*! The viewport margin */
  SGAL::Int m_viewport_margin;

/*! Parse the viewport options */
  SGAL::Int parse_viewport_option(char * optarg);
};

#endif
