/* Copyright (c) 2004 Israel.
 * All rights reserved.
 *
 * This file is part of SGAL; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 2.1 of the
 * License. See the file LICENSE.LGPL distributed with SGAL.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
 * THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * $Id: $
 * $Revision: 13559 $
 *
 * Author(s)     : Efi Fogel
 */

%{

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <stdio.h>
//! \tbd remove malloc.h ?
#include <malloc.h>
#include <stdlib.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"

#include "parse.hpp"
#include "wrlFlexLexer.hpp"

SGAL_BEGIN_NAMESPACE

typedef Element::Str_attr	Str_attr;
typedef Element::Cont_list	Cont_list; 

SGAL_END_NAMESPACE

using std::cin;
using std::cerr;
using std::cout;
using std::ostream;

using namespace SGAL;

class Element;
class Container;
class Node;
class Group;
class Transform;

typedef SGAL::Vrml_parser::token	token;

wrlFlexLexer* wrlFlexLexer::s_instance = NULL;

#ifdef _DEBUG
#define RET(x) \
{ \
  if (yy_flex_debug) printf("lex %hu [%s]\n", (int) x, YYText()); \
  return(x); \
}
#else
#define RET(x) return(x)
#endif

// The location of the current token.
static SGAL::location loc;
%}

%option debug 
%option yyclass="wrlFlexLexer"
%option never-interactive
%option noyywrap 
%option c++

I			[1-9]
D			[0-9]
O			[0-7]
L			[a-zA-Z]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

decimal			(0|({I}{D}*))
mantissa		(({decimal}\.{D}*)|(\.{D}+)|({decimal}))

number 			{mantissa}{E}?
hexnumber 		(0[xX]{H}+)
octnumber 		(0{O}+)
integer 		({decimal}|{hexnumber}|{octnumber})
quotion 		([+-]?{integer}[/][+-]?{integer})
anynumber 		([+-]?{number}|{hexnumber}|{octnumber})

idStartChar		([a-zA-Z\$_\x80-\xFF])
idRestChar      	({idStartChar}|[0-9]|\-)

%s vrml js

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

<INITIAL>"#VRML V2.0"[^\n]*	{ BEGIN(vrml); }

<vrml>"/*"		{ comment(); }
<vrml>"#"		{ comment_to_eol(); }

<vrml>"TRUE"		{ RET(token::K_TRUE); }
<vrml>"FALSE"		{ RET(token::K_FALSE); }
<vrml>"DEF"		{ RET(token::K_DEF); }
<vrml>"USE"		{ RET(token::K_USE); }
<vrml>"PROTO"		{ RET(token::K_PROTO); }
<vrml>"EXTERNPROTO"	{ RET(token::K_EXTERNPROTO); }
<vrml>"TO"		{ RET(token::K_TO); }
<vrml>"ROUTE"		{ RET(token::K_ROUTE); }
<vrml>"IS"		{ RET(token::K_IS); }
<vrml>"NULL"		{ RET(token::K_NULL); }

<vrml>"eventIn"		{ RET(token::K_EVENTIN); }
<vrml>"eventOut"	{ RET(token::K_EVENTOUT); }
<vrml>"exposedField"	{ RET(token::K_EXPOSEDFIELD); }
<vrml>"field"		{ RET(token::K_FIELD); }

<vrml>"Script"		{ RET(token::K_SCRIPT); }

<vrml>"MFColor"		{ RET(token::MFColor); }
<vrml>"MFFloat"		{ RET(token::MFFloat); }
<vrml>"MFInt32"		{ RET(token::MFInt32); }
<vrml>"MFNode"		{ RET(token::MFNode); }
<vrml>"MFRotation"	{ RET(token::MFRotation); }
<vrml>"MFString"	{ RET(token::MFString); }
<vrml>"MFTime"		{ RET(token::MFTime); }
<vrml>"MFVec2f"		{ RET(token::MFVec2f); }
<vrml>"MFVec3f"		{ RET(token::MFVec3f); }
<vrml>"SFBool"		{ RET(token::SFBool); }
<vrml>"SFColor"		{ RET(token::SFColor); }
<vrml>"SFFloat"		{ RET(token::SFFloat); }
<vrml>"SFImage"		{ RET(token::SFImage); }
<vrml>"SFInt32"		{ RET(token::SFInt32); }
<vrml>"SFNode"		{ RET(token::SFNode); }
<vrml>"SFRotation"	{ RET(token::SFRotation); }
<vrml>"SFString"	{ RET(token::SFString); }
<vrml>"SFTime"		{ RET(token::SFTime); }
<vrml>"SFVec2f"		{ RET(token::SFVec2f); }
<vrml>"SFVec3f"		{ RET(token::SFVec3f); }

<vrml>{quotion}		{ yylval->text = new std::string(YYText()); RET(token::NUMBER); }
<vrml>{anynumber}	{ yylval->text = new std::string(YYText()); RET(token::NUMBER); }

<vrml>{idStartChar}{idRestChar}*	{ yylval->text = new std::string(YYText()); RET(token::IDENTIFIER); }

<vrml>\"(\\.|[^\\"])*\"	{ yylval->text = new std::string(YYText()); RET(token::STRING_LITERAL); }
<vrml>\'(\\.|[^\\'])*\'	{ yylval->text = new std::string(YYText()); RET(token::STRING_LITERAL); }


";"			{ RET(';'); }
<vrml>"("		{ RET('('); }
<vrml>")"		{ RET(')'); }
":"			{ RET(':'); }
"|"			{ RET('|'); }
<vrml>"{"		{ RET('{'); }
<vrml>"}"		{ RET('}'); }
<vrml>"["		{ RET('['); }
<vrml>"]"		{ RET(']'); }
"?"			{ RET('?'); }
"="			{ RET('='); }
"*"			{ RET('*'); }
"/"			{ RET('/'); }
"+"			{ RET('+'); }
"-"			{ RET('-'); }
"~"			{ RET('~'); }
"@"			{ RET('@'); }
"&"			{ RET('&'); }
"^"			{ RET('^'); }
"%"			{ RET('%'); }
"."			{ RET('.'); }

"<<"			{ RET(token::LS); }
">>"			{ RET(token::RS); }
"@@"			{ RET(token::AT); }
"&&"            	{ RET(token::AND); }
"||"            	{ RET(token::OR); }
"<="            	{ RET(token::LE); }
">="            	{ RET(token::GE); }
"=="            	{ RET(token::EQ); }
"!="            	{ RET(token::NE); }

"\n\r"			{ m_lineno++; }		/* new line */ 
"\r\n"			{ m_lineno++; }		/* new line */ 
\n			{ m_lineno++; }		/* new line */ 
[ \t\v\f,]		{ ; }		    	/* skip blanks and tabs */
.			{ RET(yytext[0]); }  	/* everything else */
%%

void wrlFlexLexer::comment()
{
  register int c;

  for ( ; ; ) {
    /* eat up text of comment: */
    while ((c = yyinput()) != '*' && c != EOF) {
      if (c == '\n') m_lineno++;
    }

    if (c == '*') {
      while ((c = yyinput()) == '*');
      if (c == '/') break;    /* found the end */
    }

    if (c == EOF) {
      fprintf(stderr, "EOF in comment!\n");
      break;
    }
  }
}

// For older versions of flex (less than 2.5.31)
#ifndef yytext_ptr
#define yytext_ptr yytext
#endif

void wrlFlexLexer::comment_to_eol()
{
  char c;
  while ((c = yyinput()) != '\n' && c != 0) /* putchar(c) */ ;
  if (c == '\n') unput(c);
  else { /* putchar(c) */; }
}

void wrlFlexLexer::yyerror(const char* /* message */, int cur_token)
{
  fprintf(stderr, "Syntax error on line %d token %c\n", m_lineno, cur_token);
  exit(-1);
}
