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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_WRL_FLEXLEXER_HPP
#define SGAL_WRL_FLEXLEXER_HPP

#include <assert.h>
#include <iostream>
#include <FlexLexer.h>

#include "parse.hpp"

SGAL_BEGIN_NAMESPACE

class wrlFlexLexer : public yyFlexLexer {
public:
  wrlFlexLexer(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0) :
    yyFlexLexer(arg_yyin, arg_yyout)
  { assert(s_instance == NULL); s_instance = this; }

  static wrlFlexLexer* instance(void)
  { assert(s_instance); return(s_instance); }

  int yylex(Vrml_parser::semantic_type* lval)
  {
    yylval = lval;
    return yylex();
  }
  
  void yyerror(const char* message, int cur_token);

  int m_lineno;
    
private:
  Vrml_parser::semantic_type* yylval;
  static wrlFlexLexer* s_instance;

  virtual int yylex();
  void comment_to_eol(void);
  void comment(void);
};

SGAL_END_NAMESPACE

#endif
