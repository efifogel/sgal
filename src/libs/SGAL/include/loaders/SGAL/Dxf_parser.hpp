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
#include "SGAL/Dxf_appid_table.hpp"
#include "SGAL/Dxf_block_record_table.hpp"
#include "SGAL/Dxf_dimstyle_table.hpp"
#include "SGAL/Dxf_layer_table.hpp"
#include "SGAL/Dxf_ltype_table.hpp"
#include "SGAL/Dxf_style_table.hpp"
#include "SGAL/Dxf_ucs_table.hpp"
#include "SGAL/Dxf_view_table.hpp"
#include "SGAL/Dxf_vport_table.hpp"
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

  // Table parsers
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

  /*! Parse one table.
   */
  void parse_table();

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
  std::list<Dxf_table*> m_tables;

  // Blocks.
  std::list<Dxf_block> m_block;

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

  // APPID Table types
  typedef String Dxf_appid_table::*     String_appid_table;
  typedef int16_t Dxf_appid_table::*    Int16_appid_table;

  //! The variant type of handle to all types of APPID table data members.
  typedef boost::variant<String_appid_table, Int16_appid_table>
                                        Appid_table_variable_type;

  // BLOCK_RECORD Table types
  typedef String Dxf_block_record_table::*      String_block_record_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_block_record_table>
                                        Block_record_table_variable_type;

  // DIMSTYLE Table types
  typedef String Dxf_dimstyle_table::*  String_dimstyle_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_dimstyle_table>
                                        Dimstyle_table_variable_type;
  // LAYER Table types
  typedef String Dxf_layer_table::*     String_layer_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_layer_table>
                                        Layer_table_variable_type;
  // LTYPE Table types
  typedef String Dxf_ltype_table::*     String_ltype_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_ltype_table>
                                        Ltype_table_variable_type;
  // STYLE Table types
  typedef String Dxf_style_table::*     String_style_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_style_table>
                                        Style_table_variable_type;
  // UCS Table types
  typedef String Dxf_ucs_table::*       String_ucs_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_ucs_table>
                                        Ucs_table_variable_type;
  // VIEW Table types
  typedef String Dxf_view_table::*      String_view_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_view_table>
                                        View_table_variable_type;
  // VPORT Table types
  typedef String Dxf_vport_table::*      String_vport_table;
  typedef bool Dxf_vport_table::*        Bool_vport_table;
  typedef int8_t Dxf_vport_table::*      Int8_vport_table;
  typedef int16_t Dxf_vport_table::*     Int16_vport_table;
  typedef int32_t Dxf_vport_table::*     Int32_vport_table;
  typedef double Dxf_vport_table::*      Double_vport_table;
  typedef Uint Dxf_vport_table::*        Uint_vport_table;

  //! The variant type of handle to all types of  table data members.
  typedef boost::variant<String_vport_table,
                         Bool_vport_table,
                         Int8_vport_table,
                         Int16_vport_table,
                         Int32_vport_table,
                         Double_vport_table,
                         Uint_vport_table>
                                        Vport_table_variable_type;

  //@}

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
  int parse_common_table(Dxf_table& table, const std::string& type);

  static const std::map<String, Section_parser> s_sections;
  static const std::map<String, Header_variable> s_header_variables;
  static const std::vector<Code_range> s_code_ranges;
  static const std::array<String, 8> s_code_type_names;
  static const std::map<int, Class_variable_type> s_class_variables;
  static const std::map<String, Table_parser> s_tables;

  // Table variable mappings
  static const std::map<int, Appid_table_variable_type> s_appid_table_variables;
  static const std::map<int, Block_record_table_variable_type>
    s_block_record_table_variables;
  static const std::map<int, Dimstyle_table_variable_type>
    s_dimstyle_table_variables;
  static const std::map<int, Layer_table_variable_type> s_layer_table_variables;
  static const std::map<int, Ltype_table_variable_type> s_ltype_table_variables;
  static const std::map<int, Style_table_variable_type> s_style_table_variables;
  static const std::map<int, Ucs_table_variable_type> s_ucs_table_variables;
  static const std::map<int, View_table_variable_type> s_view_table_variables;
  static const std::map<int, Vport_table_variable_type> s_vport_table_variables;
};

SGAL_END_NAMESPACE

#endif
