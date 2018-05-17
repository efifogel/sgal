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
#include <boost/lexical_cast.hpp>

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

  /*! Read a HEADER member.
   */
  void read_header_member();

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
                         Bool_header>   Header_member_type;
  //@}

  /// \name Class types
  //@{

  //! Class data member types.
  typedef String Dxf_class::*           String_class;
  typedef int32_t Dxf_class::*          Int32_class;
  typedef int8_t Dxf_class::*           Int8_class;

  //! The variant type of handle to all types of class data members.
  typedef boost::variant<String_class, Int8_class, Int32_class>
                                        Class_member_type;

  //@}

  /// \name Table types
  //@{

  //! The type of a table parser member function.
  typedef void(Dxf_parser::*Table_parser)(void);

  typedef String Dxf_table_entry::*             String_base_entry;
  typedef Uint Dxf_table_entry::*               Uint_base_entry;
  typedef boost::variant<String_base_entry, Uint_base_entry>
                                                Base_entry_member_type;


  typedef String Dxf_base_table::*             String_base_table;
  typedef Uint Dxf_base_table::*               Uint_base_table;
  typedef boost::variant<String_base_table, Uint_base_table>
                                                Base_table_member_type;
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

  //! Marker
  String m_marker;

  /*! Obtain the type of a code
   * \param code the given code.
   */
  static Code_type code_type(int code);

  /*! Read a value from the input string and verify that it matches a given
   * code.
   * \return the type of the code.
   */
  Code_type read_code(int code);

  /*! Import a datum item.
   */
  template <typename Table, typename MemberType, typename Target>
  void import_table_value(Code_type ct, int size, MemberType handle,
                          Target& target, int index)
  {
    typedef typename Table::String_entry        String_entry;
    typedef typename Table::Bool_entry          Bool_entry;
    typedef typename Table::Int8_entry          Int8_entry;
    typedef typename Table::Int16_entry         Int16_entry;
    typedef typename Table::Int32_entry         Int32_entry;
    typedef typename Table::Double_entry        Double_entry;
    typedef typename Table::Uint_entry          Uint_entry;
    typedef typename Table::Double_2d_entry     Double_2d_entry;
    typedef typename Table::Double_3d_entry     Double_3d_entry;

    switch (ct) {
     case STRING: import_string_member<String_entry>(handle, target); break;
     case BOOL: import_member<Bool_entry>(handle, target); break;
     case INT8: import_int8_member<Int8_entry>(handle, target); break;
     case INT16: import_member<Int16_entry>(handle, target); break;
     case INT32: import_member<Int32_entry>(handle, target); break;
     case UINT: import_uint_member<Uint_entry>(handle, target); break;

     case DOUBLE:
      switch (size) {
       case 1: import_member<Double_entry>(handle, target); break;
       case 2: import_member<Double_2d_entry>(handle, target, index); break;
       case 3: import_member<Double_3d_entry>(handle, target, index); break;
      }
      break;

     default: SGAL_error();
    }
  }

  /*! Import a value.
   * \param[variable] the target variable.
   */
  template <typename T>
  void import_value(T& variable)
  {
    m_is >> variable;
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_value() value: "
                    << variable << std::endl;);
  }

  /*! Import a value to a struct member (of the same type, naturally).
   * \param[handle] handle the handle to the member.
   * \param[target] target the target struct.
   */
  template <typename T, typename MemberType, typename Target>
  void import_member(MemberType handle, Target& target)
  { import_value(target.*(boost::get<T>(handle))); }

  /*! Import a value to an item of an array member (of the same type,
   * naturally).
   * \param[handle] handle the handle to the member.
   * \param[target] target the target struct.
   * \param[index] the index of the array item.
   */
  template <typename T, typename MemberType, typename Target>
  void import_member(MemberType handle, Target& target, int index)
  { import_value((target.*(boost::get<T>(handle)))[index]); }

  /*! Import a string value to a string header member.
   * \param[i] handle the handle to the string member.
   */
  template <typename T, typename MemberType, typename Target>
  void import_string_member(MemberType handle, Target& target)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline consumes
    // the endline character left on the input stream by operator>>, and returns
    // immediately. Ignore all leftover characters.
    m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(m_is, target.*(boost::get<T>(handle)));
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout
                    << "Dxf_parser::import_string_member() value: "
                    << target.*(boost::get<T>(handle))
                    << std::endl;);
  }

  /*! Import an value to ab int8_t member.
   * C/C++ defines int8_t to be 'signed char'; thus, a negative integer cannot
   * be imported directly, since the preceding '-' is interpreted as the (sole)
   * char input.
   * \param[i] handle the handle to the member.
   */
  template <typename T, typename MemberType, typename Target>
  void import_int8_member(MemberType handle, Target& target)
  {
    // First read as an integer; then, cast to int8_t.
    int tmp;
    m_is >> tmp;
    target.*(boost::get<T>(handle)) = (int8_t) tmp;
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_member() value: "
                    << (int)(target.*(boost::get<T>(handle)))
                    << std::endl;);
  }

  /*! Import a hex value to an unsigned int member.
   * \param[i] handle the handle to the member.
   */
  template <typename T, typename MemberType, typename Target>
  void import_uint_member(MemberType handle, Target& target)
  {
    m_is >> std::hex >> target.*(boost::get<T>(handle)) >> std::dec;
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_member() value: "
                    << std::hex << "0x"
                    << target.*(boost::get<T>(handle)) << std::dec
                    << std::endl;);
  }

  //! Information of a header member.
  struct Header_member {
    Header_member(Header_member_type handle, std::list<int> codes) :
      m_handle(handle), m_codes(codes)
    {}
    Header_member_type m_handle;
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
    typedef typename Table::String_entry        String_entry;
    typedef typename Table::Bool_entry          Bool_entry;
    typedef typename Table::Int8_entry          Int8_entry;
    typedef typename Table::Int16_entry         Int16_entry;
    typedef typename Table::Int32_entry         Int32_entry;
    typedef typename Table::Double_entry        Double_entry;
    typedef typename Table::Uint_entry          Uint_entry;
    typedef typename Table::Double_2d_entry     Double_2d_entry;
    typedef typename Table::Double_3d_entry     Double_3d_entry;

    bool exceeded(false);
    auto num = parse_base_table(table);
    auto& entries = table.m_entries;
    entries.resize(num);
    /*! EF: it seems that the max number of entries is not really respected.
     * Instead of simply iterating over the entries and return when either
     * we run out of entries or the max number is reached, we only look for
     * the end code (namely 0). If we exceed the promised max number, we resize
     * the table.
     */
    size_t i(0);
    while (true) {
      std::string str;
      m_is >> str;
      if ("ENDTAB" == str) return;

      SGAL_assertion(str == name);
      if (i == num) {
        SGAL_warning_msg(exceeded, "Maximum number of entries exceeded!");
        exceeded = true;
        ++num;
        entries.resize(num);
      }
      auto& entry = entries[i++];

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

        if (100 == code) {
          import_value(m_marker);
          continue;
        }

        auto ct = code_type(code);
        auto bit = s_base_entry_members.find(code);
        if (bit != s_base_entry_members.end()) {
          auto handle = bit->second;
          auto& base_entry = static_cast<Dxf_table_entry&>(entry);
          switch (ct) {
           case STRING:
            import_string_member<String_base_entry>(handle, base_entry);
            break;

           case UINT: import_uint_member<Uint_base_entry>(handle, base_entry);
            break;
           default: break;
          }
          continue;
        }

        auto it = table.s_entry_members.find(code);
        if (it != table.s_entry_members.end()) {
          auto handle = it->second.m_handle;
          auto size = it->second.m_size;
          auto index = it->second.m_index;
          import_table_value<Table>(ct, size, handle, entry, index);
          continue;
        }

        // Handle an unrecognized code:
        String str;
        bool bval;
        int ival;
        Uint uval;
        String msg("Unrecognized code ");
        msg += std::to_string(code);
        switch (ct) {
         case STRING:
          m_is >> str;
          msg += ", value: " + str;
          break;

         case BOOL:
          m_is >> bval;
          msg += ", value: " + std::to_string(bval);
          break;

         case INT8:
         case INT16:
         case INT32:
          m_is >> ival;
          msg += ", value: " + std::to_string(ival);
          break;

         case UINT:
          msg += ", value: " + std::to_string(uval);
          m_is >> uval; break;

         default: SGAL_error();
        }
        SGAL_warning_msg(0, msg.c_str());
      }
    }
  }

  static const std::map<String, Section_parser> s_sections;
  static const std::map<String, Header_member> s_header_members;
  static const std::vector<Code_range> s_code_ranges;
  static const std::array<String, 8> s_code_type_names;
  static const std::map<int, Class_member_type> s_class_members;
  static const std::map<String, Table_parser> s_tables;
  static const std::map<int, Base_table_member_type> s_base_table_members;
  static const std::map<int, Base_entry_member_type> s_base_entry_members;
};

SGAL_END_NAMESPACE

#endif
