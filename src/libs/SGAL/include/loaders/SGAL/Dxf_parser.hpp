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
#include <array>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_header.hpp"
#include "SGAL/Dxf_class.hpp"
#include "SGAL/Dxf_table.hpp"
#include "SGAL/Dxf_appid_entry.hpp"
#include "SGAL/Dxf_block_record_entry.hpp"
#include "SGAL/Dxf_dimstyle_entry.hpp"
#include "SGAL/Dxf_layer_entry.hpp"
#include "SGAL/Dxf_ltype_entry.hpp"
#include "SGAL/Dxf_style_entry.hpp"
#include "SGAL/Dxf_ucs_entry.hpp"
#include "SGAL/Dxf_view_entry.hpp"
#include "SGAL/Dxf_vport_entry.hpp"
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

  // Sec tion parsers:
  void parse_header();
  void parse_classes();
  void parse_tables();
  void parse_blocks();
  void parse_entities();
  void parse_objects();
  void parse_thumbnailimage();

  // Table entry parsers
  void parse_appid_table();
  void parse_block_record_table();
  void parse_dimstyle_table();
  void parse_layer_table();
  void parse_ltype_table();
  void parse_style_table();
  void parse_ucs_table();
  void parse_view_table();
  void parse_vport_table();

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

  //! The type of a section parser member function.
  typedef void(Dxf_parser::*Section_parser)(void);

  /// \name Header types
  //@{

  // Header data member types.
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

  //! The variant type of handle to all types of HEADER data members.
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
  //@}

  /// \name Class types
  //@{

  //! Class data member types.
  typedef String Dxf_class::*           String_class;
  typedef int32_t Dxf_class::*          Int32_class;
  typedef int8_t Dxf_class::*           Int8_class;

  //! The variant type of handle to all types of class data members.
  typedef boost::variant<String_class, Int8_class, Int32_class>
                                        Class_variable_type;

  //@}

  /// \name Table types
  //@{

  //! The type of a table parser member function.
  typedef void(Dxf_parser::*Table_parser)(void);

  //@}

  //! The text input stream to parse.
  std::istream& m_is;

  //! The scene graph.
  Scene_graph* m_scene_graph;

  //! Header data.
  Dxf_header m_header;

  // Classes.
  std::list<Dxf_class> m_classes;

  // Tables
  typedef Dxf_table<Dxf_appid_entry>            Appid_table;
  typedef Dxf_table<Dxf_block_record_entry>     Block_record_table;
  typedef Dxf_table<Dxf_dimstyle_entry>         Dimstyle_table;
  typedef Dxf_table<Dxf_layer_entry>            Layer_table;
  typedef Dxf_table<Dxf_ltype_entry>            Ltype_table;
  typedef Dxf_table<Dxf_style_entry>            Style_table;
  typedef Dxf_table<Dxf_ucs_entry>              Ucs_table;
  typedef Dxf_table<Dxf_view_entry>             View_table;
  typedef Dxf_table<Dxf_vport_entry>            Vport_table;

  Appid_table m_appid_table;
  Block_record_table m_block_record_table;
  Dimstyle_table m_dimstyle_table;
  Layer_table m_layer_table;
  Ltype_table m_ltype_table;
  Style_table m_style_table;
  Ucs_table m_ucs_table;
  View_table m_view_table;
  Vport_table m_vport_table;

  // Blocks.
  std::list<Dxf_block> m_block;

  /*! Obtain the type of a code
   * \param code the given code.
   */
  static Code_type code_type(int code);

  /*! Read a value from the input string and verify that it matches a given
   * code.
   * \return the type of the code.
   */
  Code_type read_code(int code);

  /*! Import a value to a header variable (of the same type, naturally).
   * \param[i] handle the handle to the variable.
   */
  template <typename T, typename VariableType, typename Target>
  void import_variable(VariableType handle, Target& target)
  {
    m_is >> target.*(boost::get<T>(handle));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_variable() value: "
                    << target.*(boost::get<T>(handle))
                    << std::endl;);
  }

  /*! Import a string value to a string header variable.
   * \param[i] handle the handle to the string variable.
   */
  template <typename T, typename VariableType, typename Target>
  void import_string_variable(VariableType handle, Target& target)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline consumes
    // the endline character left on the input stream by operator>>, and returns
    // immediately. Ignore all leftover characters.
    m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(m_is, target.*(boost::get<T>(handle)));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout
                    << "Dxf_parser::import_string_variable() value: "
                    << target.*(boost::get<T>(handle))
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

  /*! Parse common table section, which aplies to all table types.
   * \return maximum number of entries in table.
   */
  int parse_base_table(Dxf_base_table& table);

  /*! Parse a table.
   */
  template <typename Table>
  void parse_table(Table& table, const std::string& name)
  {
    typedef typename Table::String_entry        String_entry;;
    typedef typename Table::Bool_entry          Bool_entry;;
    typedef typename Table::Int8_entry          Int8_entry;;
    typedef typename Table::Int16_entry         Int16_entry;;
    typedef typename Table::Int32_entry         Int32_entry;;
    typedef typename Table::Double_entry        Double_entry;;
    typedef typename Table::Uint_entry          Uint_entry;;

    auto num = parse_base_table(table);
    auto& entries = table.m_entries;
    entries.resize(num);

    for (auto& entry : entries) {
      std::string str;
      m_is >> str;
      if ("ENDTAB" == str) return;

      SGAL_assertion(str == name);

      bool done(false);
      while (!done) {
        int code;
        m_is >> code;
        SGAL_TRACE_CODE(Trace::DXF,
                        std::cout << "Dxf_parser::parse_table() code: "
                        << code << std::endl;);
        if (0 == code) {
          done = true;
          break;
        }

        auto it = table.s_entry_variables.find(code);
        SGAL_assertion(it != table.s_entry_variables.end());
        auto handle = it->second;
        switch (code_type(code)) {
         STRING: import_string_variable<String_entry>(handle, entry); break;
         BOOL: import_variable<Bool_entry>(handle, entry); break;
         INT8: import_variable<Int8_entry>(handle, entry); break;
         INT16: import_variable<Int16_entry>(handle, entry); break;
         INT32: import_variable<Int32_entry>(handle, entry); break;
         DOUBLE: import_variable<Double_entry>(handle, entry); break;
         default: SGAL_error();
        }
      }
    }
  }

  static const std::map<String, Section_parser> s_sections;
  static const std::map<String, Header_variable> s_header_variables;
  static const std::vector<Code_range> s_code_ranges;
  static const std::array<String, 8> s_code_type_names;
  static const std::map<int, Class_variable_type> s_class_variables;
  static const std::map<String, Table_parser> s_tables;

  // Table-entry variable mappings
  //static const std::map<int, Appid_entry_variable_type> s_appid_entry_variables;
  //static const std::map<int, Block_record_entry_variable_type>
  //  s_block_record_entry_variables;
  //static const std::map<int, Dimstyle_entry_variable_type>
  //  s_dimstyle_entry_variables;
  //static const std::map<int, Layer_entry_variable_type> s_layer_entry_variables;
  //static const std::map<int, Ltype_entry_variable_type> s_ltype_entry_variables;
  //static const std::map<int, Style_entry_variable_type> s_style_entry_variables;
  //static const std::map<int, Ucs_entry_variable_type> s_ucs_entry_variables;
  //static const std::map<int, View_entry_variable_type> s_view_entry_variables;
  // static const std::map<int, Vport_entry_variable_type> s_vport_entry_variables;
};

SGAL_END_NAMESPACE

#endif
