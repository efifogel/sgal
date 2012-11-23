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
// $Revision: 1309 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <string>
#if (defined _MSC_VER)
#include "getopt.h"
#else
#include <stdlib.h>
#include <getopt.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Command_line_parser.hpp"

SGAL_BEGIN_NAMESPACE

int Command_line_parser::s_argc = 0;
char ** Command_line_parser::s_argv = 0;
const char * Command_line_parser::s_prog_name;
std::set<Command_line_parser *> Command_line_parser::s_set;

/*! Constructor */
Command_line_parser::Command_line_parser(int argc, char * argv[]) :
  m_done(SGAL_FALSE)
{
  s_argc = argc;
  s_argv = argv;
  s_prog_name = strrchr(argv[0], '\\');
  s_prog_name = (s_prog_name) ? s_prog_name+1 : argv[0];
}

/*! Parameter-less constructor */
Command_line_parser::Command_line_parser(void) : m_done(false)
{
}

/*! Register the given command-line parser. Insert the given parser to the
 * set of command-line parsers.
 */
void Command_line_parser::doregister(Command_line_parser * cl)
{ s_set.insert(cl); }

/*! Return true if the command line was syntacticaly valid */
Boolean Command_line_parser::is_valid(void) const
{
  std::string str;
  std::set<Command_line_parser *>::const_iterator it;
  for (it = s_set.begin(); it != s_set.end(); it++) {
    str += (*it)->get_option_str();
  }

  // std::cout << "Command_line_parser::is_valid: "
  //           << "options: " << str.c_str() << std::endl;
  
  optind = 1;
  int c;
  while ((c = getopt(s_argc, s_argv, str.c_str())) != EOF) {
    switch (c) {
     case '?':
      std::cerr << s_prog_name << ": invalid option -- "
                << (char) optopt << std::endl;
      std::cerr << "Try `" << s_prog_name << " -h' for more information."
                << std::endl;
      return false;
    }
  }
  return true;
}

SGAL_END_NAMESPACE
