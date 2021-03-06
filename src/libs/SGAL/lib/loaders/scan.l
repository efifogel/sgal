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
 * Author(s)     : Efi Fogel
 */

%{

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4127 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4146 )
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"

#undef YY_DECL
#define YY_DECL SGAL::Vrml_parser::symbol_type SGAL::Vrml_scanner::mylex()

#include "parse.hpp"
#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

typedef Element::Shared_string		Shared_string;
typedef Element::Str_attr		Str_attr;
typedef Element::Cont_list		Cont_list;

#undef yywrap
#define yywrap() 1

SGAL_END_NAMESPACE

#define YY_NO_UNISTD_H
#define MAX_STR_CONST 			1025
%}

%option debug
%option yyclass="SGAL::Vrml_scanner"
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
idAnyChar    		({idRestChar}|\.|,)

%s vrml js stl obj objname mtl

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position.
 */
%{
#define YY_USER_ACTION  loc.columns(yyleng);
%}

%x str

%% /*** Regular Expressions Part ***/

                        std::string string_literal;

%{
loc.step();		// reset location
%}

<mtl>"Ns"		{ return Vrml_parser::make_K_SPECULAR_EXPONENT(loc); }
<mtl>"Ka"		{ return Vrml_parser::make_K_AMBIENT_COLOR(loc); }
<mtl>"Kd"		{ return Vrml_parser::make_K_DIFFUSE_COLOR(loc); }
<mtl>"Ks"		{ return Vrml_parser::make_K_SPECULAR_COLOR(loc); }
<mtl>"Ke"		{ return Vrml_parser::make_K_EMISSIVE_COLOR(loc); }
<mtl>"Ni"		{ return Vrml_parser::make_K_OPTICAL_DENSITY(loc); }
<mtl>"d"		{ return Vrml_parser::make_K_DISSOLVE(loc); }
<mtl>"Tr"		{ return Vrml_parser::make_K_TRANSPARENCY(loc); }
<mtl>"illum"		{ return Vrml_parser::make_K_ILLUMINATION(loc); }

<obj,mtl>"#"		{ comment_to_eol(); }
<obj>"vt"               { return Vrml_parser::make_K_TEXTURE_COORDINATE(loc); }
<obj>"vn"               { return Vrml_parser::make_K_NORMAL(loc); }
<obj>"v"                { return Vrml_parser::make_K_VERTEX(loc); }
<obj>"f"                { return Vrml_parser::make_K_FACET(loc); }
<obj>"s"                { return Vrml_parser::make_K_SMOOTH(loc); }
<obj>"mtllib"           { yy_push_state(objname); return Vrml_parser::make_K_MATERIAL_LIB(loc); }
<mtl>"newmtl"           { yy_push_state(objname); return Vrml_parser::make_K_NEW_MATERIAL(loc); }
<obj>"usemtl"           { yy_push_state(objname); return Vrml_parser::make_K_USE_MATERIAL(loc); }
<obj,mtl>"\n"           { loc.lines(yyleng); loc.step(); return Vrml_parser::make_K_LINE_END(loc); }

<INITIAL>"#VRML V2.0"[^\n]*	{ BEGIN(vrml); return Vrml_parser::make_VRML(loc); }

<INITIAL>"solid"[^\n]*	{ BEGIN(stl); return Vrml_parser::make_K_SOLID(loc); }

<stl>"endsolid"[^\n]*	{ return Vrml_parser::make_K_SOLID_END(loc); }
<stl>"facet"		{ return Vrml_parser::make_K_FACET(loc); }
<stl>"endfacet"		{ return Vrml_parser::make_K_FACET_END(loc); }
<stl>"normal"		{ return Vrml_parser::make_K_NORMAL(loc); }
<stl>"vertex"		{ return Vrml_parser::make_K_VERTEX(loc); }
<stl>"outer loop"	{ return Vrml_parser::make_K_LOOP(loc); }
<stl>"endloop"	        { return Vrml_parser::make_K_LOOP_END(loc); }

<vrml>"/*"		{ comment(); }
<vrml>"#"		{ comment_to_eol(); }

<vrml>"TRUE"		{ return Vrml_parser::make_K_TRUE(loc); }
<vrml>"FALSE"		{ return Vrml_parser::make_K_FALSE(loc); }
<vrml>"DEF"		{ return Vrml_parser::make_K_DEF(loc); }
<vrml>"USE"		{ return Vrml_parser::make_K_USE(loc); }
<vrml>"PROTO"		{ return Vrml_parser::make_K_PROTO(loc); }
<vrml>"EXTERNPROTO"	{ return Vrml_parser::make_K_EXTERNPROTO(loc); }
<vrml>"TO"		{ return Vrml_parser::make_K_TO(loc); }
<vrml>"ROUTE"		{ return Vrml_parser::make_K_ROUTE(loc); }
<vrml>"IS"		{ return Vrml_parser::make_K_IS(loc); }
<vrml>"NULL"		{ return Vrml_parser::make_K_NULL(loc); }

<vrml>"eventIn"		{ return Vrml_parser::make_K_EVENTIN(loc); }
<vrml>"eventOut"	{ return Vrml_parser::make_K_EVENTOUT(loc); }
<vrml>"exposedField"	{ return Vrml_parser::make_K_EXPOSEDFIELD(loc); }
<vrml>"field"		{ return Vrml_parser::make_K_FIELD(loc); }

<vrml>"Script"		{ return Vrml_parser::make_K_SCRIPT(loc); }

<vrml>"MFBool"		{ return Vrml_parser::make_MFBool(loc); }
<vrml>"MFColor"		{ return Vrml_parser::make_MFColor(loc); }
<vrml>"MFFloat"		{ return Vrml_parser::make_MFFloat(loc); }
<vrml>"MFInt32"		{ return Vrml_parser::make_MFInt32(loc); }
<vrml>"MFNode"		{ return Vrml_parser::make_MFNode(loc); }
<vrml>"MFRotation"	{ return Vrml_parser::make_MFRotation(loc); }
<vrml>"MFString"	{ return Vrml_parser::make_MFString(loc); }
<vrml>"MFTime"		{ return Vrml_parser::make_MFTime(loc); }
<vrml>"MFVec2f"		{ return Vrml_parser::make_MFVec2f(loc); }
<vrml>"MFVec3f"		{ return Vrml_parser::make_MFVec3f(loc); }
<vrml>"SFBool"		{ return Vrml_parser::make_SFBool(loc); }
<vrml>"SFColor"		{ return Vrml_parser::make_SFColor(loc); }
<vrml>"SFFloat"		{ return Vrml_parser::make_SFFloat(loc); }
<vrml>"SFImage"		{ return Vrml_parser::make_SFImage(loc); }
<vrml>"SFInt32"		{ return Vrml_parser::make_SFInt32(loc); }
<vrml>"SFNode"		{ return Vrml_parser::make_SFNode(loc); }
<vrml>"SFRotation"	{ return Vrml_parser::make_SFRotation(loc); }
<vrml>"SFString"	{ return Vrml_parser::make_SFString(loc); }
<vrml>"SFTime"		{ return Vrml_parser::make_SFTime(loc); }
<vrml>"SFVec2f"		{ return Vrml_parser::make_SFVec2f(loc); }
<vrml>"SFVec3f"		{ return Vrml_parser::make_SFVec3f(loc); }

<vrml,stl>{quotion}     {
                          Shared_string yylval(new std::string(YYText()));
                          return Vrml_parser::make_NUMBER(yylval, loc);
                        }
<vrml,stl,obj,mtl>{anynumber}   {
                          Shared_string yylval(new std::string(YYText()));
                          return Vrml_parser::make_NUMBER(yylval, loc);
                        }

<vrml,stl>{idStartChar}{idRestChar}* {
                          Shared_string yylval(new std::string(YYText()));
                          return Vrml_parser::make_IDENTIFIER(yylval, loc);
                        }

<objname>{idAnyChar}+ {
                          Shared_string yylval(new std::string(YYText()));
                          yy_pop_state(); return Vrml_parser::make_IDENTIFIER(yylval, loc);
                        }

 /* Comments must not start at the beginning og the line in the rule section. */
 /* <vrml>\"(\\.|[^\\"])*\" { */
 /*   Shared_string yylval(new std::string(YYText())); */
 /*   return Vrml_parser::make_STRING_LITERAL(yylval, loc); */
 /* } */

 /* <vrml>\'(\\.|[^\\'])*\' { */
 /*   Shared_string yylval(new std::string(YYText())); */
 /*   return Vrml_parser::make_STRING_LITERAL(yylval, loc); */
 /* } */

<vrml>\"        { string_literal.clear(); BEGIN(str); }

<str>\"         { /* saw closing quote---all done */
                  BEGIN(vrml);
                  Shared_string yylval(new std::string(string_literal));
                  return Vrml_parser::make_STRING_LITERAL(yylval, loc);
                }

<str>\n         { string_literal+= '\n'; }

<str>\\[0-7]{1,3} {
                  /* octal escape sequence */
                  int result;
                  (void) sscanf(yytext + 1, "%o", &result);
                  if (result > 0xff)
                    throw Vrml_parser::syntax_error(loc, "constant is out-of-bounds");
                  string_literal.append(std::to_string(result));
                }

<str>\\[0-9]+   { throw Vrml_parser::syntax_error(loc, "bad escape sequence"); }

<str>\\n  	{ string_literal+= '\n'; }
<str>\\t  	{ string_literal+= '\t'; }
<str>\\r  	{ string_literal+= '\r'; }
<str>\\b  	{ string_literal+= '\b'; }
<str>\\f  	{ string_literal+= '\f'; }

<str>\\(.|\n)  	{ string_literal+= yytext[1]; }

<str>[^\\\n\"]+ { string_literal.append(yytext); }

";"		{ return Vrml_parser::make_SEMICOLON(loc); }
<vrml>"("	{ return Vrml_parser::make_OPEN_PARENTHESES(loc); }
<vrml>")"	{ return Vrml_parser::make_CLOSE_PARENTHESES(loc); }
":"		{ return Vrml_parser::make_COLON(loc); }
"|"		{ return Vrml_parser::make_BAR(loc); }
<vrml>"{"	{ return Vrml_parser::make_OPEN_BRACES(loc); }
<vrml>"}"	{ return Vrml_parser::make_CLOSE_BRACES(loc); }
<vrml>"["	{ return Vrml_parser::make_OPEN_BRACKETS(loc); }
<vrml>"]"	{ return Vrml_parser::make_CLOSE_BRACKETS(loc); }
"?"		{ return Vrml_parser::make_QUESTION(loc); }
"="		{ return Vrml_parser::make_EQUAL(loc); }
"*"		{ return Vrml_parser::make_STAR(loc); }
"/"		{ return Vrml_parser::make_SLASH(loc); }
"+"		{ return Vrml_parser::make_PLUS(loc); }
"-"		{ return Vrml_parser::make_MINUS(loc); }
"~"		{ return Vrml_parser::make_TILDE(loc); }
"@"		{ return Vrml_parser::make_AT(loc); }
"&"		{ return Vrml_parser::make_AMPERSAND(loc); }
"^"		{ return Vrml_parser::make_HAT(loc); }
"%"		{ return Vrml_parser::make_PROCENT(loc); }
"."		{ return Vrml_parser::make_DOT(loc); }

"<<"		{ return Vrml_parser::make_LS(loc); }
">>"		{ return Vrml_parser::make_RS(loc); }
"&&"            { return Vrml_parser::make_AND(loc); }
"||"            { return Vrml_parser::make_OR(loc); }
"<="            { return Vrml_parser::make_LE(loc); }
">="            { return Vrml_parser::make_GE(loc); }
"=="            { return Vrml_parser::make_EQ(loc); }
"!="            { return Vrml_parser::make_NE(loc); }

[ \t\r\v\f,]+	{ loc.step(); }                    /* skip blanks and tabs */
\n		{ loc.lines(yyleng); loc.step(); } /* new line */
.		{ ; }                              /* everything else */
<<EOF>>         { return Vrml_parser::make_END(loc); }
%%

SGAL_BEGIN_NAMESPACE

void Vrml_scanner::comment()
{
  register int c;

  for ( ; ; ) {
    /* eat up text of comment: */
    while ((c = yyinput()) != '*' && c != EOF) {
      if (c == '\n') {
        loc.lines(yyleng);
        loc.step();
      }
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

void Vrml_scanner::comment_to_eol()
{
  char c;
  while (((c = yyinput()) != '\n') && (c != 0) && (c != EOF)) /* putchar(c) */ ;
  if (c == '\n') unput(c);
  else { /* putchar(c) */; }
}

void Vrml_scanner::push_state(int new_state)
{ yyFlexLexer::yy_push_state(new_state); }

void Vrml_scanner::pop_state() { yyFlexLexer::yy_pop_state(); }

SGAL_END_NAMESPACE
