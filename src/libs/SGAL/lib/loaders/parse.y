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
// Author(s)     : Efi Fogel

// %debug
%require "2.7"
%skeleton "lalr1.cc"
%error-verbose
%defines
%define api.token.constructor
%define api.namespace {SGAL}
%define api.value.type variant
%language "C++"
%define parser_class_name {Vrml_parser}
%locations

%code requires // *.hh
{

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4127 )
#pragma warning ( disable : 4065 )
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4512 )
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

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
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"
#ifdef USE_V8
  #include "SGAL/Script.hpp"
#endif

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

typedef Element::Field_attr             Field_attr;
typedef Element::Field_attr_list        Field_attr_list;
typedef Element::Field_attr_iter        Field_attr_iter;

class Vrml_scanner;

typedef boost::shared_ptr<Container>          Shared_container;
typedef boost::shared_ptr<Node>               Shared_node;
typedef boost::shared_ptr<Group>              Shared_group;
typedef boost::shared_ptr<Transform>          Shared_transform;
typedef boost::shared_ptr<Route>              Shared_route;
typedef boost::shared_ptr<Shape>              Shared_shape;
typedef boost::shared_ptr<Coord_array_3d>     Shared_coord_array_3d;
typedef boost::shared_ptr<Indexed_face_set>   Shared_indexed_face_set;

SGAL_END_NAMESPACE
}

%lex-param { Vrml_scanner& scanner }
%parse-param { Vrml_scanner& scanner }
%parse-param { Scene_graph* scene_graph }
%parse-param { bool& maybe_binary_stl }

%code // *.cc
{
  static SGAL::Vrml_parser::symbol_type yylex(SGAL::Vrml_scanner& scanner);
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

%type <std::string> NUMBER STRING_LITERAL
%type <std::string> IDENTIFIER
%type <std::string> Id nodeTypeId fieldId nodeNameId eventInId eventOutId
%type <std::string> mfValue sfValues sfValue
%type <std::string> sfstringValue sfstringValues mfstringValue URLList
%type <std::string> sfboolValues
%type <std::string> sfint32Values
%type <std::string> sfboolValue
%type <std::string> fieldValue
%type <Field_type> fieldType
%type <Cont_list*> nodeStatements
%type <Element*> nodeBody
%type <Element*> scriptBody
%type <Element*> scriptBodyElement
%type <Element*> restrictedInterfaceDeclaration

%type <Vector3f> normal
%type <Vector3f> vertex
%type <std::vector<Vector3f>*> vertices
%type <std::vector<Vector3f>*> vertexLoop
%type <std::vector<Vector3f>*> facet
%type <std::pair<Shared_indexed_face_set, std::vector<Uint>* > > facets

%define api.token.prefix {TOK_}

%token VRML
%token K_SOLID
%token K_SOLID_END
%token K_FACET
%token K_FACET_END
%token K_NORMAL
%token K_LOOP
%token K_LOOP_END
%token K_VERTEX

%token
  END               0   "end of file"
  SEMICOLON         ";"
  OPEN_PARENTHESES  "("
  CLOSE_PARENTHESES ")"
  COLON             ":"
  BAR               "|"
  OPEN_BRACES       "{"
  CLOSE_BRACES      "}"
  OPEN_BRACKETS     "["
  CLOSE_BRACKETS    "]"
  QUESTION          "?"
  EQUAL             "="
  STAR              "*"
  SLASH             "/"
  PLUS              "+"
  MINUS             "-"
  TILDE             "~"
  AT                "@"
  AMPERSAND         "&"
  HAT               "^"
  PROCENT           "%"
  DOT               "."
;

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

%token MFBool
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
%token AND
%token OR
%token LE
%token GE
%token EQ
%token NE

%start Start

%%

/* General: */

Start           : VRML vrmlScene
                {
                  scene_graph->set_input_format_id(File_format::ID_WRL);
                  scene_graph->set_root($2);
                }
                | K_SOLID IDENTIFIER facets K_SOLID_END IDENTIFIER
                {
                  /* STL */
                  scene_graph->set_input_format_id(File_format::ID_STL);
                  if ($2.compare($5) != 0)
                    error(yyla.location,
                          std::string("Non matching solid names \"") +
                          $2 + "\" and \"" + $5 + "\"!");
                  Group* group = new Group;
                  scene_graph->set_root(Shared_group(group));
                  Transform* transform = new Transform;
                  Shared_transform shared_transform(transform);
                  scene_graph->add_container(shared_transform,
                                             g_navigation_root_name);
                  scene_graph->set_navigation_root(shared_transform);
                  group->add_child(shared_transform);
                  Shape* shape = new Shape;
                  Shared_shape shared_shape(shape);
                  transform->add_child(shared_shape);
                  Shared_indexed_face_set shared_ifs = $3.first;
                  shared_ifs->set_coord_indices_from_range($3.second->begin(),
                                                           $3.second->end());
                  shared_ifs->collapse_identical_coordinates();
                  shape->set_geometry(shared_ifs);
                  shape->add_to_scene(scene_graph);
                  scene_graph->add_container(shared_shape);

                  /* Clear */
                  $3.second->clear();
                  delete $3.second;
                  $3.second = NULL;
                }
                | K_SOLID error
                  /* It is possible that even though the file starts with the
                   * token "solid", the file is in the binary stl format.
                   */
                {  maybe_binary_stl = true; }
                ;

facets          : /* empty */
                {
                  Indexed_face_set* indexed_face_set = new Indexed_face_set;
                  Shared_indexed_face_set shared_ifs(indexed_face_set);
                  std::vector<Uint>* num_verts = new std::vector<Uint>;
                  $$ = std::make_pair(shared_ifs, num_verts);
                  Coord_array_3d* coords = new Coord_array_3d;
                  Shared_coord_array_3d shared_coords(coords);
                  indexed_face_set->set_coord_array(shared_coords);
                }
                | facets facet
                {
                  std::swap($$, $1);

                  /* Splice coordinates */
                  Shared_coord_array_3d coords =
                    boost::dynamic_pointer_cast<Coord_array_3d>($$.first->get_coord_array());
                  std::vector<Vector3f>& vertices = *$2;
                  Uint size = coords->size();
                  coords->resize(size + vertices.size());
                  std::copy(vertices.begin(), vertices.end(),
                            coords->begin() + size);

                  /* Insert vertex number */
                  $$.second->push_back(vertices.size());

                  /* Clear */
                  $2->clear();
                  delete $2;
                  $2 = NULL;
                }
                ;

facet           : K_FACET normal vertexLoop K_FACET_END { std::swap($$, $3); }
                ;

normal          : K_NORMAL NUMBER NUMBER NUMBER
                { $$ = Vector3f(boost::lexical_cast<Float>($2),
                                boost::lexical_cast<Float>($3),
                                boost::lexical_cast<Float>($4)); }
                ;

vertexLoop      : K_LOOP vertices K_LOOP_END { std::swap($$, $2); }
                ;

vertices        : /* empty */
                { $$ = new std::vector<Vector3f>; }
                | vertices vertex
                {
                  std::swap($$, $1);
                  $$->push_back($2);
                }
                ;

vertex          : K_VERTEX NUMBER NUMBER NUMBER
                { $$ = Vector3f(boost::lexical_cast<Float>($2),
                                boost::lexical_cast<Float>($3),
                                boost::lexical_cast<Float>($4)); }
                ;

vrmlScene       : statements
                {
                  Group* group = new Group;
                  $$ = Shared_group(group);
                  $$->add_child($1);
                } ;

statements      : /* empty */
                {
                  Transform* transform = new Transform;
                  $$ = Shared_transform(transform);
                  scene_graph->add_container($$, g_navigation_root_name);
                  scene_graph->set_navigation_root($$);
                }
                | statements statement
                { std::swap($$, $1); if ($2) $$->add_child($2); }
                ;

statement       : nodeStatement { $$ = boost::dynamic_pointer_cast<Node>($1); }
                | protoStatement { $$ = boost::dynamic_pointer_cast<Node>($1); }
                | routeStatement { $$ = boost::dynamic_pointer_cast<Node>($1); }
                ;

nodeStatement   : node { scene_graph->add_container($1); std::swap($$, $1); }
                | K_DEF nodeNameId node
                { scene_graph->add_container($3, $2); $$ = $3; }
                | K_USE nodeNameId { $$ = scene_graph->get_container($2); }
                ;

protoStatement  : proto { std::swap($$, $1); }
                | externproto { std::swap($$, $1); }
                ;

proto           : K_PROTO nodeTypeId "[" interfaceDeclarations "]" "{" statements "}" { $$ = Shared_container(); /*! \todo */ }
                ;

interfaceDeclarations   : /* empty */ { /*! \todo */ }
                | interfaceDeclarations interfaceDeclaration { /*! \todo */ }
                ;

restrictedInterfaceDeclaration : K_EVENTIN fieldType eventInId
                {
                  $$ = new Element;
                  Field_attr attr(new std::string($3),
                                  std::make_tuple(RULE_IN, $2, new std::string("")));
                  $$->add_attribute(attr);
                }
                | K_EVENTOUT fieldType eventOutId
                {
                  $$ = new Element;
                  Field_attr attr(new std::string($3),
                                  std::make_tuple(RULE_OUT, $2, new std::string("")));
                  $$->add_attribute(attr);
                }
                | K_FIELD fieldType fieldId fieldValue
                {
                  $$ = new Element;
                  Field_attr attr(new std::string($3),
                                  std::make_tuple(RULE_FIELD, $2, new std::string($4)));
                  $$->add_attribute(attr);
                }
                ;

interfaceDeclaration : restrictedInterfaceDeclaration { /*! \todo */ }
                | K_EXPOSEDFIELD fieldType fieldId fieldValue { /*! \todo */ }
                | K_EXPOSEDFIELD fieldType fieldId sfnodeValue { /*! \todo */ }
                ;

externproto     : K_EXTERNPROTO nodeTypeId "[" externInterfaceDeclarations "]" URLList { $$ = Shared_container(); /*! \todo */ }
                ;

externInterfaceDeclarations     : /* empty */ { /*! \todo */ }
                | externInterfaceDeclarations externInterfaceDeclaration { /*! \todo */ }
                ;

externInterfaceDeclaration      : K_EVENTIN fieldType eventInId { /*! \todo */ }
                | K_EVENTOUT fieldType eventOutId { /*! \todo */ }
                | K_FIELD fieldType fieldId { /*! \todo */ }
                | K_EXPOSEDFIELD fieldType fieldId { /*! \todo */ }
                ;

routeStatement  : K_ROUTE nodeNameId "." eventOutId K_TO nodeNameId "." eventInId
                {
                  Shared_route route = Shared_route(new Route);
                  if (!scene_graph->route($2, $4, $6, $8, &*route)) {
                    error(yyla.location, std::string("Cannot route"));
                    YYERROR;
                  }
                  scene_graph->add_container(route);
                  $$ = route;
                }
                ;

URLList         : mfstringValue { std::swap($$, $1); }
                ;

mfstringValue   : sfstringValue { std::swap($$, $1); }
                | "[" "]" { ; }
                | "[" sfstringValues "]" { std::swap($$, $2); }
                ;

/* nodes: */

node            : nodeTypeId "{" nodeBody "}"
                {
                  $$ = Container_factory::get_instance()->create($1);
                  if (!$$) {
                    error(yyla.location,
                          std::string("Unknown node type \"") + $1 + "\"");
                    YYERROR;
                  }
                  else {
                    $$->set_attributes($3);
                    $$->add_to_scene(scene_graph);
                  }
                }
                | K_SCRIPT "{" scriptBody "}"
                {
#ifdef USE_V8
                  /* Every Script node has a unique type; thus, the type of the
                   * Script node is not registered in the container factory.
                   * Instead every Script node is explicitly created.
                   */
                  SGAL::Script* script = new SGAL::Script;
                  $$ = Shared_container(script);
                  if (!$$) {
                    error(yyla.location,
                          std::string("Unknown node type \"Script\""));
                    YYERROR;
                  }
                  else {
                    $$->set_attributes($3);
                    $$->add_to_scene(scene_graph);
                  }
#endif
                }
                ;

nodeBody        : /* empty */ { $$ = new Element; }
                | nodeBody fieldId sfValue
                {
                  std::swap($$, $1);
                  Str_attr str_attr(new std::string($2), new std::string($3));
                  $$->add_attribute(str_attr);
                }
                | nodeBody fieldId "[" sfValues "]"
                {
                  std::swap($$, $1);
                  Str_attr attr(new std::string($2), new std::string($4));
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId sfnodeValue
                {
                  std::swap($$, $1);
                  Cont_attr attr(new std::string($2), $3);
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId "[" "]"
                {
                  std::swap($$, $1);
                  Multi_cont_attr attr(new std::string($2), new Cont_list);
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId "[" nodeStatements "]"
                {
                  std::swap($$, $1);
                  Multi_cont_attr attr(new std::string($2), $4);
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId K_IS fieldId { $$ = 0; /*! \todo */ }
                /* | nodeBody eventInId K_IS eventInId */
                /* | nodeBody eventOutId K_IS eventOutId */
                | nodeBody routeStatement { std::swap($$, $1); /*! \todo */ }
                | nodeBody protoStatement { std::swap($$, $1); /*! \todo */ }
                ;


scriptBody      : /* empty */ { $$ = new Element; }
                | scriptBody scriptBodyElement
                {
                  std::swap($$, $1);
                  $$->splice(*$2);
                }
                ;

scriptBodyElement : fieldId sfValue
                {
                  $$ = new Element;
                  Str_attr attr(new std::string($1), new std::string($2));
                  $$->add_attribute(attr);
                }

                | restrictedInterfaceDeclaration { std::swap($$, $1); }
                /*! | K_EVENTIN fieldType eventInId K_IS eventInId { \todo } */
                /*! | K_EVENTOUT fieldType eventOutId K_IS eventOutId { \todo } */
                /*! | K_FIELD fieldType fieldId K_IS fieldId { \todo } */
                ;

nodeNameId      : Id { std::swap($$, $1); }
                ;

nodeTypeId      : Id { std::swap($$, $1); }
                ;

fieldId         : Id { std::swap($$, $1); }
                ;

eventInId       : Id { std::swap($$, $1); }
                ;

eventOutId      : Id { std::swap($$, $1); }
                ;

Id              : IDENTIFIER { std::swap($$, $1); }
                ;

/*
Id              : IdFirstChar
                | IdFirstChar IdRestChars
                ;

IdFirstChar     : Any ISO-10646 character encoded using UTF-8 except: 0x30-0x39, 0x0-0x20, 0x22, 0x23, 0x27, 0x2b, 0x2c, 0x2d, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ;

IdRestChars     : Any number of ISO-10646 characters except: 0x0-0x20, 0x22, 0x23, 0x27, 0x2c, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ;
*/

/* Fields: */

fieldType       : MFBool { $$ = MF_BOOL; }
                | MFColor { $$ = MF_COLOR; }
                | MFFloat { $$ = MF_FLOAT; }
                | MFInt32 { $$ = MF_INT32; }
                | MFNode { $$ = MF_SHARED_CONTAINER; }
                | MFRotation { $$ = MF_ROTATION; }
                | MFString { $$ = MF_STR; }
                | MFTime { $$ = MF_TIME; }
                | MFVec2f { $$ = MF_VEC2F; }
                | MFVec3f { $$ = MF_VEC3F; }
                | SFBool { $$ = SF_BOOL; }
                | SFColor { $$ = SF_COLOR; }
                | SFFloat { $$ = SF_FLOAT; }
                | SFImage { $$ = SF_IMAGE; }
                | SFInt32 { $$ = SF_INT32; }
                | SFNode { $$ = SF_SHARED_CONTAINER; }
                | SFRotation { $$ = SF_ROTATION; }
                | SFString { $$ = SF_STR; }
                | SFTime { $$ = SF_TIME; }
                | SFVec2f { $$ = SF_VEC2F; }
                | SFVec3f { $$ = SF_VEC3F; }
                ;

fieldValue      : sfValue { std::swap($$, $1); }
                | mfValue { std::swap($$, $1); }
                ;

sfValue         : sfint32Values { std::swap($$, $1); }
                | sfboolValue { std::swap($$, $1); }
                | sfstringValue { std::swap($$, $1); }
                ;

sfboolValue     : K_TRUE { $$ = std::string("TRUE"); }
                | K_FALSE { $$ = std::string("FALSE"); }
                ;

sfnodeValue     : nodeStatement { std::swap($$, $1); }
                | K_NULL { $$ = Shared_container(); /*! \todo */ }
                ;

sfstringValue   : STRING_LITERAL { std::swap($$, $1); }
                ;

sfboolValues   : sfboolValue { std::swap($$, $1); }
                | sfboolValues sfboolValue { $1 += " " + $2; std::swap($$, $1); }
                | sfboolValues "," sfboolValue { $1 += " " + $3; std::swap($$, $1); }
                ;

sfint32Values   : NUMBER { std::swap($$, $1); }
                | sfint32Values NUMBER { $1 += " " + $2; std::swap($$, $1); }
                | sfint32Values "," NUMBER { $1 += " " + $3; std::swap($$, $1); }
                ;

sfstringValues  : STRING_LITERAL { std::swap($$, $1); }
                | sfstringValues STRING_LITERAL { $1 += $2; std::swap($$, $1); }
                | sfstringValues "," STRING_LITERAL { $1 += $3; std::swap($$, $1); }
                ;

nodeStatements  : nodeStatement { $$ = new Cont_list; $$->push_back($1); }
                | nodeStatements nodeStatement { std::swap($$, $1); $$->push_back($2); }
                ;

mfValue         : "[" "]" { ; }
                | "[" sfValues "]" { std::swap($$, $2); }
                ;

sfValues        : sfboolValues { std::swap($$, $1); }
                | sfstringValues { std::swap($$, $1); }
                | sfint32Values { std::swap($$, $1); }
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
static SGAL::Vrml_parser::symbol_type yylex(SGAL::Vrml_scanner& scanner)
{ return scanner.mylex(); }
