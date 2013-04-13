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
// $Revision: 13559 $
//
// Author(s)     : Efi Fogel

// %debug
%require "2.7"
%skeleton "lalr1.cc"
 // %debug
%error-verbose
%defines
%define api.namespace "SGAL"
%language "C++"
%define parser_class_name "Vrml_parser"
 // %define api.token.constructor
%define api.value.type variant
%locations

%code requires // *.hh
{

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Route.hpp"

SGAL_BEGIN_NAMESPACE

typedef Element::Str_attr               Str_attr;
typedef Element::Str_attr_list          Str_attr_list;
typedef Element::Str_attr_iter          Str_attr_iter;

typedef Element::Cont_attr              Cont_attr;
typedef Element::Cont_attr_list         Cont_attr_list;
typedef Element::Cont_attr_iter         Cont_attr_iter;

typedef Element::Cont_list              Cont_list; 
typedef Element::Multi_cont_attr        Multi_cont_attr;
typedef Element::Multi_cont_attr_list   Multi_cont_attr_list;
typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;

class Vrml_scanner;
 
typedef boost::shared_ptr<Container>    Shared_container;
typedef boost::shared_ptr<Node>         Shared_node;
typedef boost::shared_ptr<Group>        Shared_group;
typedef boost::shared_ptr<Transform>    Shared_transform;
typedef boost::shared_ptr<Route>        Shared_route;

SGAL_END_NAMESPACE
}

%lex-param { Vrml_scanner& scanner }
%parse-param { Vrml_scanner& scanner }
%parse-param { Scene_graph* scene_graph }

%code // *.cc
{
  static int yylex(SGAL::Vrml_parser::semantic_type* yylval,
                   SGAL::Vrml_parser::location_type* yylloc,
                   SGAL::Vrml_scanner& scanner);  
}

/* %union { */
/*   std::string* text; */
/*   SGAL::Element* element; */
/*   SGAL::Str_attr* attribute; */
/*   SGAL::Container* container; */
/*   SGAL::Cont_list* cont_list; */
/*   SGAL::Node* node; */
/*   SGAL::Group* group; */
/*   SGAL::Transform* transform; */
/* } */

%type <Shared_group> vrmlScene
%type <Shared_transform> statements
%type <Shared_node> statement
%type <Shared_container> nodeStatement protoStatement routeStatement
%type <Shared_container> node sfnodeValue
%type <Shared_container> proto externproto

%type <std::string*> NUMBER STRING_LITERAL
%type <std::string*> IDENTIFIER
%type <std::string*> Id nodeTypeId fieldId nodeNameId eventInId eventOutId
%type <std::string*> mfValue sfValues sfValue
%type <std::string*> sfstringValue sfstringValues mfstringValue URLList
%type <std::string*> sfint32Values
%type <std::string*> sfboolValue
%type <std::string*> fieldValue
%type <std::string*> fieldType
%type <Cont_list*> nodeStatements
%type <Element*> nodeBody
%type <Str_attr*> scriptBodyAttribute

%token IDENTIFIER
%token STRING_LITERAL
%token NUMBER
%token K_TRUE K_FALSE

%token K_DEF K_USE
%token K_PROTO K_EXTERNPROTO
%token K_TO K_ROUTE
%token K_IS
%token K_NULL

%token K_EVENTIN
%token K_EVENTOUT
%token K_EXPOSEDFIELD
%token K_FIELD

%token K_SCRIPT

%token MFColor
%token MFFloat
%token MFInt32
%token MFNode
%token MFRotation
%token MFString
%token MFTime
%token MFVec2f
%token MFVec3f
%token SFBool
%token SFColor
%token SFFloat
%token SFImage
%token SFInt32
%token SFNode
%token SFRotation
%token SFString
%token SFTime
%token SFVec2f
%token SFVec3f

%token LS
%token RS
%token AT
%token AND
%token OR
%token LE
%token GE
%token EQ
%token NE

%start Start

%%

/* General: */

Start           : vrmlScene { scene_graph->set_root($1); } ;

vrmlScene       : statements
                {
                  $$ = Shared_group(new Group);
                  $$->add_child($1);
                } ; 

statements      : /* empty */
                {
                  $$ = Shared_transform(new Transform);
                  scene_graph->add_container($$, g_navigation_root_name);
                  scene_graph->set_navigation_root($$);
                }
                | statements statement { $$ = $1; if ($2) $$->add_child($2); }
                ;

statement       : nodeStatement { $$ = boost::dynamic_pointer_cast<Node>($1); }
                | protoStatement { $$ = boost::dynamic_pointer_cast<Node>($1); }
                | routeStatement { $$ = boost::dynamic_pointer_cast<Node>($1); }
                ;

nodeStatement   : node { scene_graph->add_container($1); $$ = $1; }
                | K_DEF nodeNameId node
                { scene_graph->add_container($3, *($2)); $$ = $3; }
                | K_USE nodeNameId { $$ = scene_graph->get_container(*($2)); }
                ;

protoStatement  : proto { $$ = $1; }
                | externproto { $$ = $1; }
                ;

proto           : K_PROTO nodeTypeId '[' interfaceDeclarations ']' '{' statements '}' { delete $2; $$ = Shared_container(); /*! \todo */ }
                ;

interfaceDeclarations   : /* empty */ { /*! \todo */ }
                | interfaceDeclarations interfaceDeclaration { /*! \todo */ }
                ; 

restrictedInterfaceDeclaration : K_EVENTIN fieldType eventInId { /*! \todo */ }
                | K_EVENTOUT fieldType eventOutId { /*! \todo */ }
                | K_FIELD fieldType fieldId fieldValue { /*! \todo */ }
                | K_FIELD fieldType fieldId sfnodeValue { /*! \todo */ }
                ;

interfaceDeclaration    : restrictedInterfaceDeclaration { /*! \todo */ }
                | K_EXPOSEDFIELD fieldType fieldId fieldValue { /*! \todo */ }
                | K_EXPOSEDFIELD fieldType fieldId sfnodeValue { /*! \todo */ }
                ;

externproto     : K_EXTERNPROTO nodeTypeId '[' externInterfaceDeclarations ']' URLList { delete $2; delete $6; $$ = Shared_container(); /*! \todo */ }
                ; 

externInterfaceDeclarations     : /* empty */ { /*! \todo */ }
                | externInterfaceDeclarations externInterfaceDeclaration { /*! \todo */ }
                ;

externInterfaceDeclaration      : K_EVENTIN fieldType eventInId { /*! \todo */ }
                | K_EVENTOUT fieldType eventOutId { /*! \todo */ }
                | K_FIELD fieldType fieldId { /*! \todo */ }
                | K_EXPOSEDFIELD fieldType fieldId { /*! \todo */ }
                ;

routeStatement  : K_ROUTE nodeNameId '.' eventOutId K_TO nodeNameId '.' eventInId
                {
                  Shared_route route = Shared_route(new Route);
                  if (!scene_graph->route(*($2), *($4), *($6), *($8), &*route)) {
                    error(yyla.location, std::string("Cannot route"));
                    YYERROR;
                  }
                  scene_graph->add_container(route);
                  $$ = route;
                }
                ;

URLList         : mfstringValue { $$ = $1; }
                ;

mfstringValue   : sfstringValue { $$ = $1; }
                | '[' ']' { $$ = new std::string; }
                | '[' sfstringValues ']' { $$ = $2; }
                ;

/* nodes: */

node            : nodeTypeId '{' nodeBody '}'
                {
                  $$ = Container_factory::get_instance()->create(*($1));
                  if (!$$) {
                    error(yyla.location,
                          std::string("Unknown node type \"") + *$1 + "\"");
                    YYERROR;
                  }
                  else {
                    $$->set_attributes($3);
                    $$->add_to_scene(scene_graph);
                  }
                }
                | K_SCRIPT '{' scriptBody '}' { $$ = Shared_container(); /*! \todo */ }
                ;

nodeBody        : /* empty */ { $$ = new Element; }
                | nodeBody fieldId sfValue
                {
                  $$ = $1;
                  $$->add_attribute(new Str_attr($2,$3));
                }
                | nodeBody fieldId '[' sfValues ']'
                {
                  $$ = $1;
                  $$->add_attribute(new Str_attr($2,$4));
                }
                | nodeBody fieldId sfnodeValue
                {
                  $$ = $1;
                  $$->add_attribute(new Cont_attr($2,$3));
                }
                | nodeBody fieldId '[' ']'
                {
                  $$ = $1;
                  $$->add_attribute(new Multi_cont_attr($2,new Cont_list));
                }
                | nodeBody fieldId '[' nodeStatements ']'
                {
                  $$ = $1;
                  $$->add_attribute(new Multi_cont_attr($2,$4));
                }
                | nodeBody fieldId K_IS fieldId { $$ = 0; /*! \todo */ }
                /* | nodeBody eventInId K_IS eventInId */
                /* | nodeBody eventOutId K_IS eventOutId */
                | nodeBody routeStatement { $$ = $1; /*! \todo */ }
                | nodeBody protoStatement { $$ = $1; /*! \todo */ }
                ; 


scriptBody      : scriptBodyElement { /*! \todo */ }
                | scriptBody scriptBodyElement { /*! \todo */ }
                | scriptBodyAttribute { /*! \todo */ }
                | scriptBody scriptBodyAttribute { delete $2; /*! \todo */ }
                ; 

scriptBodyElement : restrictedInterfaceDeclaration { /*! \todo */ }
                | routeStatement { /*! \todo */ }
                | protoStatement { /*! \todo */ }
                | K_EVENTIN fieldType eventInId K_IS eventInId { /*! \todo */ }
                | K_EVENTOUT fieldType eventOutId K_IS eventOutId { /*! \todo */ }
                | K_FIELD fieldType fieldId K_IS fieldId { /*! \todo */ }
                /* | fieldId sfnodeValue */
                /* | fieldId K_IS fieldId { $1; } */
                /* | eventInId K_IS eventInId */
                /* | eventOutId K_IS eventOutId */
                ;

scriptBodyAttribute : fieldId fieldValue { $$ = new Str_attr($1, $2); }
                ; 

nodeNameId      : Id { $$ = $1; }
                ; 

nodeTypeId      : Id { $$ = $1; }
                ; 

fieldId         : Id { $$ = $1; }
                ; 

eventInId       : Id { $$ = $1; }
                ; 

eventOutId      : Id { $$ = $1; }
                ;

Id              : IDENTIFIER { $$ = $1; }
                ;

/*
Id              : IdFirstChar
                | IdFirstChar IdRestChars
                ; 

IdFirstChar     : Any ISO-10646 character encoded using UTF-8 except: 0x30-0x39, 0x0-0x20, 0x22, 0x23, 0x27, 0x2b, 0x2c, 0x2d, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ; 

IdRestChars     : Any number of ISO-10646 characters except: 0x0-0x20, 0x22, 0x23, 0x27, 0x2c, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ; 
*/

/* Fields: */

fieldType       : MFColor { $$ = new std::string("Colors"); }
                | MFFloat { $$ = new std::string("Floats"); }
                | MFInt32 { $$ = new std::string("Integers"); }
                | MFNode { $$ = new std::string("Nodes"); }
                | MFRotation { $$ = new std::string("Rotations"); }
                | MFString { $$ = new std::string("Strings"); }
                | MFTime { $$ = new std::string("Times"); }
                | MFVec2f { $$ = new std::string("Vector2Floats"); }
                | MFVec3f { $$ = new std::string("Vector2Floats"); }
                | SFBool { $$ = new std::string("Boolean"); }
                | SFColor { $$ = new std::string("Color"); }
                | SFFloat { $$ = new std::string("Float"); }
                | SFImage { $$ = new std::string("Image"); }
                | SFInt32 { $$ = new std::string("Integer"); }
                | SFNode { $$ = new std::string("Node"); }
                | SFRotation { $$ = new std::string("Rotation"); }
                | SFString { $$ = new std::string("String"); }
                | SFTime { $$ = new std::string("Time"); }
                | SFVec2f { $$ = new std::string("Vector2Float"); }
                | SFVec3f { $$ = new std::string("Vector3Float"); }
                ;

fieldValue      : sfValue { $$ = $1; }
                | mfValue { $$ = $1; }
                ;

sfValue         : sfint32Values { $$ = $1; }
                | sfboolValue { $$ = $1; }
                | sfstringValue { $$ = $1; }
                ;

sfboolValue     : K_TRUE { $$ = new std::string("TRUE"); }
                | K_FALSE { $$ = new std::string("FALSE"); }
                ; 

sfnodeValue     : nodeStatement { $$ = $1; }
                | K_NULL { $$ = Shared_container(); /*! \todo */ }
                ; 

sfstringValue   : STRING_LITERAL { $$ = $1; }
                ; 

sfint32Values   : NUMBER { $$ = $1; }
                | sfint32Values NUMBER { (*$1) += " " + (*$2); $$ = $1; }
                | sfint32Values ',' NUMBER { (*$1) += " " + (*$3); $$ = $1; }
                ; 

sfstringValues  : STRING_LITERAL { $$ = $1; }
                | sfstringValues STRING_LITERAL { (*$1) += (*$2); $$ = $1; }
                | sfstringValues ',' STRING_LITERAL { (*$1) += (*$3); $$ = $1; }
                ;

nodeStatements  : nodeStatement { $$ = new Cont_list; $$->push_back($1); }
                | nodeStatements nodeStatement { $$ = $1; $$->push_back($2); }
                ; 

mfValue         : '[' ']' { $$ = new std::string; }
                | '[' sfValues ']' { $$ = $2; }
                ;

sfValues        : sfint32Values { $$ = $1; } 
                | sfstringValues { $$ = $1; }
                ;

%%

SGAL_BEGIN_NAMESPACE

/*! */
void Vrml_parser::error(const Vrml_parser::location_type& l,
                        const std::string& err_message)
{ std::cerr << "Error at " << l << ": " << err_message << std::endl; }

SGAL_END_NAMESPACE

#include "Vrml_scanner.hpp"

/*! */
static int yylex(SGAL::Vrml_parser::semantic_type* yylval,
                 SGAL::Vrml_parser::location_type* yylloc,
                 SGAL::Vrml_scanner& scanner)
{ return scanner.yylex(yylval, yylloc); }
