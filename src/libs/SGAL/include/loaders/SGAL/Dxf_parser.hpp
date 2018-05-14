// Copyright (c) 2004,2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DXF_PARSER_HPP
#define SGAL_DXF_PARSER_HPP

#include <iostream>
#include <map>
#include <list>
#include <vector>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_header.hpp"
#include "SGAL/Dxf_class.hpp"
#include "SGAL/Dxf_table.hpp"
#include "SGAL/Dxf_block.hpp"
#include "SGAL/Loader_code.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;

class SGAL_SGAL_DECL Dxf_parser {
public:
  /*! Construct.
   */
  Dxf_parser(std::istream& is, Scene_graph* sg);

  /*! Parse.
   */
  Loader_code operator()();

protected:

  void parse_header();
  void parse_classes();
  void parse_tables();
  void parse_blocks();
  void parse_entities();
  void parse_objects();
  void parse_thumbnailimage();

  /*! Read a HEADER variable.
   */
  void read_header_variable();

  /*! Read a comment line.
   */
  void read_comment();

  /*! Parse one class.
   */
  void parse_class();

private:
  enum Section_type {
    HEADER = 0,
    CLASSES,
    TABLES,
    BLOCKS,
    ENTITIES,
    OBJECTS,
    THUMBNAILIMAGE,
    NUM_SECTIONS
  };

  enum Code_type { STRING, FLOAT, DOUBLE, INT8, INT16, INT32, UINT, BOOL };

  struct Code_range {
    Code_range(int min, int max, Code_type type) :
      m_min(min), m_max(max), m_type(type)
    {}

    int m_min;
    int m_max;
    Code_type m_type;
  };

  //! The text input stream to parse.
  std::istream& m_is;

  //! The scene graph.
  Scene_graph* m_scene_graph;

  //! Header data.
  Dxf_header m_header;

  // Classes.
  std::list<Dxf_class> m_classes;

  // Tables.
  std::list<Dxf_table> m_tables;

  // Blocks.
  std::list<Dxf_block> m_block;

  typedef void(Dxf_parser::*Section_parser)(void);

  //
  typedef String Dxf_header::*          String_header;
  typedef float Dxf_header::*           Float_header;
  typedef double Dxf_header::*          Double_header;
  typedef double (Dxf_header::*Double_2d_header)[2];
  typedef double (Dxf_header::*Double_3d_header)[3];
  typedef int8_t Dxf_header::*          Int8_header;
  typedef int16_t Dxf_header::*         Int16_header;
  typedef int32_t Dxf_header::*         Int32_header;
  typedef unsigned int Dxf_header::*    Uint_header;
  typedef bool Dxf_header::*            Bool_header;

  //! The variant type of handle to all types of data members.
  typedef boost::variant<String_header,
                         Float_header,
                         Double_header,
                         Double_2d_header,
                         Double_3d_header,
                         Int8_header,
                         Int16_header,
                         Int32_header,
                         Uint_header,
                         Bool_header>   Header_variable_type;

  //
  typedef String Dxf_class::*           String_class;
  typedef int32_t Dxf_class::*          Int32_class;
  typedef int8_t Dxf_class::*           Int8_class;

  //! The variant type of handle to all types of data members.
  typedef boost::variant<String_class, Int8_class, Int32_class>
                                        Class_variable_type;

  /*! Read a value from the input string and verify that it matches a given
   * code.
   * \return the type of the code.
   */
  Code_type read_code(int code);

  /*! Import a value to a header variable (of the same type, naturally).
   * \param[i] handle the handle to the variable.
   */
  template <typename T>
  void import_header_variable(Header_variable_type handle)
  {
    m_is >> m_header.*(boost::get<T>(handle));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_header_variable() value: "
                    << m_header.*(boost::get<T>(handle))
                    << std::endl;);
  }

  /*! Import a string value to a string header variable.
   * \param[i] handle the handle to the string variable.
   */
  void import_string_header_variable(Header_variable_type handle)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline consumes
    // the endline character left on the input stream by operator>>, and returns
    // immediately. Ignore all leftover characters.
    m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(m_is, m_header.*(boost::get<String_header>(handle)));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout
                    << "Dxf_parser::import_string_header_variable() value: "
                    << m_header.*(boost::get<String_header>(handle))
                    << std::endl;);
  }

  /*! Import a value to a header variable (of the same type, naturally).
   * \param[i] handle the handle to the variable.
   */
  template <typename T>
    void import_class_variable(Class_variable_type handle,
                               Dxf_class& dxf_class)
  {
    m_is >> dxf_class.*(boost::get<T>(handle));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_class_variable() value: "
                    << dxf_class.*(boost::get<T>(handle))
                    << std::endl;);
  }

  /*! Import a string value to a string class variable.
   * \param[i] handle the handle to the string variable.
   */
  void import_string_class_variable(Class_variable_type handle,
                                    Dxf_class& dxf_class)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline consumes
    // the endline character left on the input stream by operator>>, and returns
    // immediately. Ignore all leftover characters.
    m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(m_is, dxf_class.*(boost::get<String_class>(handle)));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout
                    << "Dxf_parser::import_string_class_variable() value: "
                    << dxf_class.*(boost::get<String_class>(handle))
                    << std::endl;);
  }

  //! Information of a header variable.
  struct Header_variable {
    Header_variable(Header_variable_type handle, std::list<int> codes) :
      m_handle(handle), m_codes(codes)
    {}
    Header_variable_type m_handle;
    std::list<int> m_codes;
  };

  static const std::map<String, Section_parser> s_sections;
  static const std::map<String, Header_variable> s_header_variables;
  static const std::vector<Code_range> s_code_ranges;
  static const std::array<String, 8> s_code_type_names;
  static const std::map<int, Class_variable_type> s_class_variables;
};

SGAL_END_NAMESPACE

#endif
