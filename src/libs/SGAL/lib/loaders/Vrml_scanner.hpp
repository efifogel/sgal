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

#ifndef SGAL_WRL_FLEXLEXER_HPP
#define SGAL_WRL_FLEXLEXER_HPP

#include <assert.h>
#include <iostream>
#undef yyFlexLexer
#include <FlexLexer.h>

#include "parse.hpp"

SGAL_BEGIN_NAMESPACE

class Vrml_scanner : public yyFlexLexer {
public:
  Vrml_scanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0) :
    yyFlexLexer(arg_yyin, arg_yyout)
  {}

  /*! */
  Vrml_parser::symbol_type mylex();

  /*! Set the filename.
   * Used when generating error messages.
   */
  void set_filename(const std::string& name) { filename = name; }

  /*! Obtain the filename.
   * Used when generating error messages.
   */
  const std::string& get_filename() const { return filename; }

  /*! Push the current start condition onto the top of the start condition
   * stack and switches to new_state.
   */
  void push_state(int new_state);

  /*! Pop the top of the stack and switches to it.
   */
  void pop_state();

private:
  Vrml_parser::location_type loc;
  std::string filename;

  /*! */
  void comment_to_eol(void);

  /*! */
  void comment(void);
};

SGAL_END_NAMESPACE

#endif
