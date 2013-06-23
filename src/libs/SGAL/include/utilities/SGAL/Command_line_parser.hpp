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
// $Id $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COMMAND_LINE_PARSER_HPP
#define SGAL_COMMAND_LINE_PARSER_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <set>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! 
 */
class SGAL_SGAL_DECL Command_line_parser {
public:
  /*! Constructor */
  Command_line_parser(int argc, char* argv[]);

  /*! Parameter-less constructor */
  Command_line_parser(void);

  /*! Destructor */
  virtual ~Command_line_parser(void) {}
  
  /*! Print an informative message to standard output */
  virtual void print_help(void) {}

  /*! Obtain the options string */
  virtual char* get_option_str(void) const = 0;
  
  /*! Parse the command-line */
  virtual int operator()(void) = 0;

  /*! Obtain the program name */
  static const char* get_prog_name(void) { return s_prog_name; }

  /*! Register the given parser to parse the command line */
  void doregister(Command_line_parser* cl);

  /*! Return true if the command line was syntacticaly valid */
  Boolean is_valid(void) const;
  
  /*! Return true if the program should be terminated immediately once parsing
   * is complete
   */
  Boolean is_done(void) const { return m_done; }

protected:
  /*! The number of command-line strings */
  static int s_argc;

  /*! The command-line strings */
  static char** s_argv;

  /*! The program name */
  static const char* s_prog_name;

  /*! A set of command line parsers to apply */
  static std::set<Command_line_parser*> s_set;

  /*! Indicates that the program should be terminated immediately once parsing
   * is complete
   */
  Boolean m_done;
};

SGAL_END_NAMESPACE

#endif
