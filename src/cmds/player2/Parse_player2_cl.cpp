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

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"

#include "Parse_player2_cl.hpp"

char * Parse_player2_cl::s_viewport_opts[] = { "scale", "margin" };

/*! Constructor */
Parse_player2_cl::Parse_player2_cl() :
  m_viewport_scale(1),
  m_viewport_margin(0)
{}

/*! Print help message */
void Parse_player2_cl::print_help(void)
{
  Parse_player_cl::print_help();

  std::cout <<  "  -V <options>\tset viewport options\n"
            << "\t\t\t\tscale=<scale>\n"
            << "\t\t\t\tmargin=<margin>\n"
            << std::endl;
  
}

/*! Parse the viewport options */
SGAL::Int Parse_player2_cl::parse_viewport_option(char * optarg)
{
  char * options = optarg;
  char * value = 0;
  if (*options == '\0') return 0;
  while (*options != '\0') {
    int code = getsubopt(&options, s_viewport_opts, &value);
    if (code < 0 || code >= NUM_VIEWPORT_OPTS) {
      std::cerr << "Unrecognized trace code option '" << optarg << "'!"
                << std::endl;
      return -1;
    }
    switch (code) {
     case VO_SCALE: m_viewport_scale = SGAL::atoff(value); break;
     case VO_MARGIN: m_viewport_margin = atoi(value); break;
     default: break;
    }
  }
  return 0;
}

/*! Parse the command line */
SGAL::Int Parse_player2_cl::operator()()
{
  SGAL::Int rc = Parse_player_cl::operator()();
  if (rc != 0) return rc;

  optind = 1;
  opterr = 0;

  int c;
  while ((c = getopt(s_argc, s_argv, "V:")) != EOF) {
    switch (c) {
     case 'V': parse_viewport_option(optarg); break;

     default: break;
    }
  }
  
  return 0;
}
