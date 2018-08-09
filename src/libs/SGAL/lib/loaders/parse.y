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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel

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
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_type.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Route.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Attribute_error.hpp"
#include "SGAL/Proto.hpp"
#include "SGAL/Container_proto.hpp"
#ifdef SGAL_USE_V8
  #include "SGAL/Script.hpp"
#endif

SGAL_BEGIN_NAMESPACE

typedef Element::Str_attr               Str_attr;
typedef Element::Str_attr_list          Str_attr_list;
typedef Element::Str_attr_iter          Str_attr_iter;

typedef Element::Str_list               Str_list;
typedef Element::Multi_str_attr         Multi_str_attr;
typedef Element::Multi_str_attr_list    Multi_str_attr_list;
typedef Element::Multi_str_attr_iter    Multi_str_attr_iter;

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

typedef Element::Field_multi_str_attr           Field_multi_str_attr;
typedef Element::Field_multi_str_attr_list      Field_multi_str_attr_list;
typedef Element::Field_multi_str_attr_iter      Field_multi_str_attr_iter;

class Vrml_scanner;
class Proto;

typedef boost::shared_ptr<std::string>        Shared_string;
typedef boost::shared_ptr<Container>          Shared_container;
typedef boost::shared_ptr<Node>               Shared_node;
typedef boost::shared_ptr<Group>              Shared_group;
typedef boost::shared_ptr<Route>              Shared_route;
typedef boost::shared_ptr<Shape>              Shared_shape;
typedef boost::shared_ptr<Coord_array_3d>     Shared_coord_array_3d;
typedef boost::shared_ptr<Indexed_face_set>   Shared_indexed_face_set;

SGAL_END_NAMESPACE
}

%lex-param { Vrml_scanner& scanner }
%parse-param { Vrml_scanner& scanner }
%parse-param { Scene_graph* scene_graph }
%parse-param { Group* root }
%parse-param { bool& maybe_binary_stl }

%code // *.cc
{
  static SGAL::Vrml_parser::symbol_type yylex(SGAL::Vrml_scanner& scanner);
}

%type <Group*> statements
%type <Shared_container> statement
%type <Shared_container> nodeStatement routeStatement
%type <Shared_container> node sfnodeValue
%type <Proto*> protoRooting

%type <Shared_string> NUMBER STRING_LITERAL
%type <Shared_string> IDENTIFIER
%type <Shared_string> Id nodeTypeId fieldId nodeNameId eventInId eventOutId
%type <Shared_string> mfValue sfValues sfValue
%type <Shared_string> sfstringValue
%type <Shared_string> sfboolValues
%type <Shared_string> sfint32Values
%type <Shared_string> sfboolValue
%type <Shared_string> fieldValue
%type <Field_type> fieldType
%type <Str_list*> sfstringValues mfstringValue URLList
%type <Cont_list*> nodeStatements
%type <Element*> nodeBody
%type <Element*> scriptBody
%type <Element*> scriptBodyElement
%type <Element*> restrictedInterfaceDeclaration
%type <Element*> interfaceDeclaration interfaceDeclarations

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
%token K_TEXTURE_COORDINATE
%token K_SMOOTH
%token K_MATERIAL_LIB
%token K_NEW_MATERIAL
%token K_USE_MATERIAL
%token K_LINE_END
%token K_SPECULAR_EXPONENT
%token K_AMBIENT_COLOR
%token K_DIFFUSE_COLOR
%token K_SPECULAR_COLOR
%token K_EMISSIVE_COLOR
%token K_OPTICAL_DENSITY
%token K_DISSOLVE
%token K_TRANSPARENCY
%token K_ILLUMINATION

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
                  /* WRL */
                }
                | K_SOLID facets K_SOLID_END
                {
                  /* STL */
                  /* Add Shape */
                  Shared_shape shape(new Shape);
                  SGAL_assertion(shape);
                  shape->add_to_scene(scene_graph);
                  scene_graph->add_container(shape);
                  root->add_child(shape);

                  /* Add IndexedFaceSet */
                  Shared_indexed_face_set ifs = $2.first;
                  ifs->set_coord_indices_from_range($2.second->begin(),
                                                    $2.second->end());
                  ifs->collapse_identical_coordinates();
                  ifs->add_to_scene(scene_graph);
                  shape->set_geometry(ifs);

                  /* Clear */
                  $2.second->clear();
                  delete $2.second;
                  $2.second = nullptr;
                }
                  /* It is possible that even though the file starts with the
                   * token "solid", the file is in the binary stl format.
                   */
                | K_SOLID facets { maybe_binary_stl = true; } error { YYERROR; }
                | K_SOLID { maybe_binary_stl = true; } error { YYERROR; }
                ;

facets          : %empty
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
                  std::vector<Vector3f>& vertices = *$2;

                  // Determine wwhether the face is collinear
                  Boolean col = true;
                  auto it = vertices.begin();
                  auto v0 = it++;
                  auto v1 = it++;
                  for (; it != vertices.end(); ++it) {
                    auto v2 = it;
                    if (! Vector3f::collinear(*v0, *v1, *v2)) {
                      col = false;
                      break;
                    }
                    v0 = v1;
                    v1 = v2;
                  }

                  // Discard collinear face
                  if (!col) {
                    /* Splice coordinates */
                    Shared_coord_array_3d coords =
                      boost::dynamic_pointer_cast<Coord_array_3d>($$.first->get_coord_array());
                    Uint size = coords->size();
                    coords->resize(size + vertices.size());
                    std::copy(vertices.begin(), vertices.end(),
                              coords->begin() + size);

                    /* Insert vertex number */
                    $$.second->push_back(vertices.size());
                  }

                  /* Clear */
                  $2->clear();
                  delete $2;
                  $2 = nullptr;
                }
                ;

facet           : K_FACET normal vertexLoop K_FACET_END { std::swap($$, $3); }
                ;

normal          : K_NORMAL NUMBER NUMBER NUMBER
                { $$ = Vector3f(boost::lexical_cast<Float>(*$2),
                                boost::lexical_cast<Float>(*$3),
                                boost::lexical_cast<Float>(*$4)); }
                ;

vertexLoop      : K_LOOP vertices K_LOOP_END { std::swap($$, $2); }
                ;

vertices        : %empty
                { $$ = new std::vector<Vector3f>; }
                | vertices vertex
                {
                  std::swap($$, $1);
                  $$->push_back($2);
                }
                ;

vertex          : K_VERTEX NUMBER NUMBER NUMBER
                { $$ = Vector3f(boost::lexical_cast<Float>(*$2),
                                boost::lexical_cast<Float>(*$3),
                                boost::lexical_cast<Float>(*$4)); }
                ;

vrmlScene       : statements
                ;

statements      : %empty { $$ = root; }
                | statements statement
                { std::swap($$, $1); if ($2) $$->add_child($2); }
                ;

statement       : nodeStatement { std::swap($$, $1); }
                | protoStatement { $$ = Shared_container(nullptr); }
                | routeStatement { std::swap($$, $1); }
                ;

nodeStatement   : node { scene_graph->add_container($1); std::swap($$, $1); }
                | K_DEF nodeNameId node
                { scene_graph->add_container($3, *$2); std::swap($$, $3); }
                | K_USE nodeNameId {
                  $$ = scene_graph->get_container(*$2);
                  if (!$$) {
                    error(yyla.location,
                          std::string("Unknown node name \"") + *$2 + "\"");
                    YYERROR;
                  }
                }
                ;

protoStatement  : proto
                | externproto
                ;

protoRooting    : %empty
                {
                  // Construct a new prototype contasiner.
                  root = $$ = Proto::prototype();
                }
                ;

proto           : K_PROTO protoRooting nodeTypeId "[" interfaceDeclarations "]" "{" statements "}"
                {
                  // Set the tag of the new prototype container.
                  $2->set_tag(*$3);
                  auto* factory = Container_factory::get_instance();
                  // Register the new prototype container (using the tag).
                  factory->do_register($2);
                  // Set the field-info records of this container.
                  $2->set_field_infos($5);
                  auto* field_info = $2->get_field_info(Proto::CHILDREN);
                  SGAL_assertion(field_info);
                  // Move the value of the 'children' field to the initial-value
                  // of the associated information-record (of the prototype), so
                  // that it can be accessed directly from the prototype.
                  field_info->move_field_to_initial_value($2);
                }
                ;

interfaceDeclarations : %empty { $$ = new Element; }
                | interfaceDeclarations interfaceDeclaration
                {
                  std::swap($$, $1);
                  $$->splice(*$2);
                }
                ;

restrictedInterfaceDeclaration : K_EVENTIN fieldType eventInId
                {
                  $$ = new Element;
                  Shared_string empty_str(new std::string(""));
                  Field_attr
                    attr($3, std::make_tuple(Field_rule::RULE_IN, $2, empty_str));
                  $$->add_attribute(attr);
                }
                | K_EVENTOUT fieldType eventOutId
                {
                  $$ = new Element;
                  Shared_string empty_str(new std::string(""));
                  Field_attr
                    attr($3, std::make_tuple(Field_rule::RULE_OUT, $2, empty_str));
                  $$->add_attribute(attr);
                }
                | K_FIELD fieldType fieldId fieldValue
                {
                  $$ = new Element;
                  Field_attr
                    attr($3, std::make_tuple(Field_rule::RULE_FIELD, $2, $4));
                  $$->add_attribute(attr);
                }
                | K_FIELD fieldType fieldId mfstringValue
                {
                  $$ = new Element;
                  Field_multi_str_attr
                    attr($3, std::make_tuple(Field_rule::RULE_FIELD, $2, $4));
                  $$->add_attribute(attr);
                }
                ;

interfaceDeclaration : restrictedInterfaceDeclaration { std::swap($$, $1); }
                | K_EXPOSEDFIELD fieldType fieldId fieldValue
                {
                  $$ = new Element;
                  Field_attr
                    attr($3, std::make_tuple(Field_rule::RULE_EXPOSED_FIELD,
                                             $2, $4));
                  $$->add_attribute(attr);
                }
                | K_EXPOSEDFIELD fieldType fieldId mfstringValue
                {
                  $$ = new Element;
                  Field_multi_str_attr
                    attr($3, std::make_tuple(Field_rule::RULE_EXPOSED_FIELD,
                                             $2, $4));
                  $$->add_attribute(attr);
                }
                | K_EXPOSEDFIELD fieldType fieldId sfnodeValue
                {
                  $$ = new Element;
                  SGAL_error_msg("Not implemented!");
                }
                ;

externproto     : K_EXTERNPROTO nodeTypeId "[" externInterfaceDeclarations "]" URLList
                {
                  SGAL_error_msg("\"EXTERNPROTO\" not implemented yet!");
                  /*! \todo */
                }
                ;

externInterfaceDeclarations : %empty { /*! \todo */ }
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
                  if (!scene_graph->route(*$2, *$4, *$6, *$8, &*route)) {
                    error(yyla.location, std::string("Cannot route"));
                    YYERROR;
                  }
                  scene_graph->add_container(route);
                  $$ = route;
                }
                ;

URLList         : mfstringValue { std::swap($$, $1); }
                ;

mfstringValue   : "[" sfstringValues "]" { std::swap($$, $2); }
                ;

/* nodes: */

node            : nodeTypeId "{" nodeBody "}"
                {
                  $$ = Container_factory::get_instance()->create(*$1);
                  if (!$$) {
                    error(yyla.location,
                          std::string("Unknown node type \"") + *$1 + "\"");
                    YYERROR;
                  }
                  try {
                    $$->set_attributes($3);
                  }
                  catch(Attribute_error& e) {
                    error(yyla.location, e.what());
                  }
                  $$->add_to_scene(scene_graph);
                }
                | K_SCRIPT "{" scriptBody "}"
                {
#ifdef SGAL_USE_V8
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
                    try {
                      $$->set_attributes($3);
                    }
                    catch(Attribute_error& e) {
                      error(yyla.location, e.what());
                    }
                    $$->add_to_scene(scene_graph);
                  }
#endif
                }
                ;

nodeBody        : %empty { $$ = new Element; }
                | nodeBody fieldId sfValue
                {
                  std::swap($$, $1);
                  Str_attr str_attr($2, $3);
                  $$->add_attribute(str_attr);
                }
                | nodeBody fieldId "[" sfValues "]"
                {
                  std::swap($$, $1);
                  Str_attr attr($2, $4);
                  $$->add_attribute(attr);
                }

                | nodeBody fieldId "[" sfstringValues "]"
                {
                  std::swap($$, $1);
                  Multi_str_attr attr($2, $4);
                  $$->add_attribute(attr);
                }

                | nodeBody fieldId sfnodeValue
                {
                  std::swap($$, $1);
                  Cont_attr attr($2, $3);
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId "[" "]"
                {
                  std::swap($$, $1);
                  Multi_cont_attr attr($2, new Cont_list);
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId "[" nodeStatements "]"
                {
                  std::swap($$, $1);
                  Multi_cont_attr attr($2, $4);
                  $$->add_attribute(attr);
                }
                | nodeBody fieldId K_IS fieldId { std::swap($$, $1); /*! \todo */ }
                /* | nodeBody eventInId K_IS eventInId */
                /* | nodeBody eventOutId K_IS eventOutId */
                | nodeBody routeStatement { std::swap($$, $1); /*! \todo */ }
                | nodeBody protoStatement { std::swap($$, $1); /*! \todo */ }
                ;


scriptBody      : %empty { $$ = new Element; }
                | scriptBody scriptBodyElement
                {
                  std::swap($$, $1);
                  $$->splice(*$2);
                }
                ;

scriptBodyElement : fieldId sfValue
                {
                  $$ = new Element;
                  Str_attr attr($1, $2);
                  $$->add_attribute(attr);
                }
                | fieldId "[" sfstringValues "]"
                {
                  $$ = new Element;
                  Multi_str_attr attr($1, $3);
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

fieldType       : MFBool { $$ = Field_type::MF_BOOL; }
                | MFColor { $$ = Field_type::MF_COLOR; }
                | MFFloat { $$ = Field_type::MF_FLOAT; }
                | MFInt32 { $$ = Field_type::MF_INT32; }
                | MFNode { $$ = Field_type::MF_SHARED_CONTAINER; }
                | MFRotation { $$ = Field_type::MF_ROTATION; }
                | MFString { $$ = Field_type::MF_STR; }
                | MFTime { $$ = Field_type::MF_TIME; }
                | MFVec2f { $$ = Field_type::MF_VEC2F; }
                | MFVec3f { $$ = Field_type::MF_VEC3F; }
                | SFBool { $$ = Field_type::SF_BOOL; }
                | SFColor { $$ = Field_type::SF_COLOR; }
                | SFFloat { $$ = Field_type::SF_FLOAT; }
                | SFImage { $$ = Field_type::SF_IMAGE; }
                | SFInt32 { $$ = Field_type::SF_INT32; }
                | SFNode { $$ = Field_type::SF_SHARED_CONTAINER; }
                | SFRotation { $$ = Field_type::SF_ROTATION; }
                | SFString { $$ = Field_type::SF_STR; }
                | SFTime { $$ = Field_type::SF_TIME; }
                | SFVec2f { $$ = Field_type::SF_VEC2F; }
                | SFVec3f { $$ = Field_type::SF_VEC3F; }
                ;

fieldValue      : sfValue { std::swap($$, $1); }
                | mfValue { std::swap($$, $1); }
                ;

sfValue         : sfint32Values { std::swap($$, $1); }
                | sfboolValue { std::swap($$, $1); }
                | sfstringValue { std::swap($$, $1); }
                ;

sfboolValue     : K_TRUE { $$ = Shared_string(new std::string("TRUE")); }
                | K_FALSE { $$ = Shared_string(new std::string("FALSE")); }
                ;

sfnodeValue     : nodeStatement { std::swap($$, $1); }
                | K_NULL { $$ = Shared_container(); /*! \todo */ }
                ;

sfstringValue   : STRING_LITERAL { std::swap($$, $1); }
                ;

sfboolValues    : sfboolValue { std::swap($$, $1); }
                | sfboolValues sfboolValue { *$1 += " " + *$2; std::swap($$, $1); }
                | sfboolValues "," sfboolValue { *$1 += " " + *$3; std::swap($$, $1); }
                ;

sfint32Values   : NUMBER { std::swap($$, $1); }
                | sfint32Values NUMBER { *$1 += " " + *$2; std::swap($$, $1); }
                | sfint32Values "," NUMBER { *$1 += " " + *$3; std::swap($$, $1); }
                ;

sfstringValues  : sfstringValue { $$ = new Str_list; $$->push_back($1); }
                | sfstringValues sfstringValue { std::swap($$, $1); $$->push_back($2); }
                | sfstringValues "," sfstringValue { std::swap($$, $1); $$->push_back($3); }
                ;

nodeStatements  : nodeStatement { $$ = new Cont_list; $$->push_back($1); }
                | nodeStatements nodeStatement { std::swap($$, $1); $$->push_back($2); }
                ;

mfValue         : "[" "]" { $$ = Shared_string(new std::string("")); }
                | "[" sfValues "]" { std::swap($$, $2); }
                ;

sfValues        : sfboolValues { std::swap($$, $1); }
                | sfint32Values { std::swap($$, $1); }
                ;

%%

#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

/*! */
void Vrml_parser::error(const Vrml_parser::location_type& l,
                        const std::string& err_message)
{
  if (maybe_binary_stl) return;
  std::cerr << scanner.get_filename() << ": " << "Error at " << l
            << ": " << err_message << std::endl;
}

SGAL_END_NAMESPACE

/*! */
static SGAL::Vrml_parser::symbol_type yylex(SGAL::Vrml_scanner& scanner)
{ return scanner.mylex(); }
