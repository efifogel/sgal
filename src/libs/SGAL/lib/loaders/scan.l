%option yyclass="wrlFlexLexer"
%option never-interactive

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
#include "wrlFlexLexer.hpp"

// For older versions of flex (less than 2.5.31)
#if SGAL_FLEX_INT_VER <= 2533
extern "C" int yywrap(void);
#endif

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

#include "parse.h"

wrlFlexLexer * wrlFlexLexer::s_instance = NULL;

#ifdef _DEBUG
#define RET(x) \
{ \
  if (yy_flex_debug) printf("lex %hu [%s]\n", (int) x, YYText()); \
  return(x); \
}
#else
#define RET(x) return(x)
#endif

%}

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

%%

<INITIAL>"#VRML V2.0"[^\n]*	{ BEGIN(vrml); }

<vrml>"/*"		{ comment(); }
<vrml>"#"		{ comment_to_eol(); }

<vrml>"TRUE"		{ RET(K_TRUE); }
<vrml>"FALSE"		{ RET(K_FALSE); }
<vrml>"DEF"		{ RET(K_DEF); }
<vrml>"USE"		{ RET(K_USE); }
<vrml>"PROTO"		{ RET(K_PROTO); }
<vrml>"EXTERNPROTO"	{ RET(K_EXTERNPROTO); }
<vrml>"TO"		{ RET(K_TO); }
<vrml>"ROUTE"		{ RET(K_ROUTE); }
<vrml>"IS"		{ RET(K_IS); }
<vrml>"NULL"		{ RET(K_NULL); }

<vrml>"eventIn"		{ RET(K_EVENTIN); }
<vrml>"eventOut"	{ RET(K_EVENTOUT); }
<vrml>"exposedField"	{ RET(K_EXPOSEDFIELD); }
<vrml>"field"		{ RET(K_FIELD); }

<vrml>"Script"		{ RET(K_SCRIPT); }

<vrml>"MFColor"		{ RET(MFColor); }
<vrml>"MFFloat"		{ RET(MFFloat); }
<vrml>"MFInt32"		{ RET(MFInt32); }
<vrml>"MFNode"		{ RET(MFNode); }
<vrml>"MFRotation"	{ RET(MFRotation); }
<vrml>"MFString"	{ RET(MFString); }
<vrml>"MFTime"		{ RET(MFTime); }
<vrml>"MFVec2f"		{ RET(MFVec2f); }
<vrml>"MFVec3f"		{ RET(MFVec3f); }
<vrml>"SFBool"		{ RET(SFBool); }
<vrml>"SFColor"		{ RET(SFColor); }
<vrml>"SFFloat"		{ RET(SFFloat); }
<vrml>"SFImage"		{ RET(SFImage); }
<vrml>"SFInt32"		{ RET(SFInt32); }
<vrml>"SFNode"		{ RET(SFNode); }
<vrml>"SFRotation"	{ RET(SFRotation); }
<vrml>"SFString"	{ RET(SFString); }
<vrml>"SFTime"		{ RET(SFTime); }
<vrml>"SFVec2f"		{ RET(SFVec2f); }
<vrml>"SFVec3f"		{ RET(SFVec3f); }

<vrml>{quotion}		{ yylval.text = new std::string(YYText()); RET(NUMBER); }
<vrml>{anynumber}	{ yylval.text = new std::string(YYText()); RET(NUMBER); }

<vrml>{idStartChar}{idRestChar}*	{ yylval.text = new std::string(YYText()); RET(IDENTIFIER); }

<vrml>\"(\\.|[^\\"])*\"	{ yylval.text = new std::string(YYText()); RET(STRING_LITERAL); }
<vrml>\'(\\.|[^\\'])*\'	{ yylval.text = new std::string(YYText()); RET(STRING_LITERAL); }


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

"<<"			{ RET(LS); }
">>"			{ RET(RS); }
"@@"			{ RET(AT); }
"&&"            	{ RET(AND); }
"||"            	{ RET(OR); }
"<="            	{ RET(LE); }
">="            	{ RET(GE); }
"=="            	{ RET(EQ); }
"!="            	{ RET(NE); }

"\n\r"			{ m_lineno++; }		/* new line */ 
"\r\n"			{ m_lineno++; }		/* new line */ 
\n			{ m_lineno++; }		/* new line */ 
[ \t\v\f,]		{ ; }		    	/* skip blanks and tabs */
.			{ RET(yytext[0]); }  	/* everything else */
%%

#if SGAL_FLEX_INT_VER <= 2533
extern "C" int yywrap()
#else
int yyFlexLexer::yywrap()
#endif
{
  return(1);
}


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
