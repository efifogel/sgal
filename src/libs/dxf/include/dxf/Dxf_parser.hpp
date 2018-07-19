// Copyright (c) 2018 Israel.
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

#ifndef DXF_PARSER_HPP
#define DXF_PARSER_HPP

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <array>
#include <type_traits>

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tti/has_member_function.hpp>
#include <boost/tti/has_type.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Loader_code.hpp"
#include "SGAL/Base_loader.hpp"
#include "SGAL/Trace.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_importer.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_class.hpp"
#include "dxf/Dxf_table.hpp"
#include "dxf/Dxf_appid_entry.hpp"
#include "dxf/Dxf_block_record_entry.hpp"
#include "dxf/Dxf_dimstyle_entry.hpp"
#include "dxf/Dxf_layer_entry.hpp"
#include "dxf/Dxf_ltype_entry.hpp"
#include "dxf/Dxf_style_entry.hpp"
#include "dxf/Dxf_ucs_entry.hpp"
#include "dxf/Dxf_view_entry.hpp"
#include "dxf/Dxf_vport_entry.hpp"
#include "dxf/Dxf_block.hpp"
#include "dxf/Dxf_base_entity.hpp"
#include "dxf/Dxf_base_object.hpp"
#include "dxf/Dxf_simple_record_wrapper.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_extended_data.hpp"
#include "dxf/Dxf_line_edge.hpp"
#include "dxf/Dxf_circle_edge.hpp"
#include "dxf/Dxf_ellipse_edge.hpp"
#include "dxf/Dxf_spline_edge.hpp"

// Entities
#include "dxf/Dxf_3dface_entity.hpp"
#include "dxf/Dxf_3dsolid_entity.hpp"
#include "dxf/Dxf_acad_proxy_entity.hpp"
#include "dxf/Dxf_arc_entity.hpp"
#include "dxf/Dxf_arcalignedtext_entity.hpp"
#include "dxf/Dxf_attdef_entity.hpp"
#include "dxf/Dxf_attrib_entity.hpp"
#include "dxf/Dxf_body_entity.hpp"
#include "dxf/Dxf_circle_entity.hpp"
#include "dxf/Dxf_dimension_entity.hpp"
#include "dxf/Dxf_ellipse_entity.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_image_entity.hpp"
#include "dxf/Dxf_insert_entity.hpp"
#include "dxf/Dxf_leader_entity.hpp"
#include "dxf/Dxf_line_entity.hpp"
#include "dxf/Dxf_lwpolyline_entity.hpp"
#include "dxf/Dxf_mline_entity.hpp"
#include "dxf/Dxf_mtext_entity.hpp"
#include "dxf/Dxf_oleframe_entity.hpp"
#include "dxf/Dxf_ole2frame_entity.hpp"
#include "dxf/Dxf_point_entity.hpp"
#include "dxf/Dxf_polyline_entity.hpp"
#include "dxf/Dxf_ray_entity.hpp"
#include "dxf/Dxf_region_entity.hpp"
#include "dxf/Dxf_rtext_entity.hpp"
#include "dxf/Dxf_seqend_entity.hpp"
#include "dxf/Dxf_shape_entity.hpp"
#include "dxf/Dxf_solid_entity.hpp"
#include "dxf/Dxf_spline_entity.hpp"
#include "dxf/Dxf_text_entity.hpp"
#include "dxf/Dxf_tolerance_entity.hpp"
#include "dxf/Dxf_trace_entity.hpp"
#include "dxf/Dxf_vertex_entity.hpp"
#include "dxf/Dxf_viewport_entity.hpp"
#include "dxf/Dxf_wipeout_entity.hpp"
#include "dxf/Dxf_xline_entity.hpp"
#include "dxf/Dxf_user_entity.hpp"

// Objects
#include "dxf/Dxf_acad_proxy_object.hpp"
#include "dxf/Dxf_acdbdictionarywdflt_object.hpp"
#include "dxf/Dxf_acdbnavisworksmodeldef_object.hpp"
#include "dxf/Dxf_acdbplaceholder_object.hpp"
#include "dxf/Dxf_datatable_object.hpp"
#include "dxf/Dxf_dictionary_object.hpp"
#include "dxf/Dxf_dictionaryvar_object.hpp"
#include "dxf/Dxf_dimassoc_object.hpp"
#include "dxf/Dxf_field_object.hpp"
#include "dxf/Dxf_geodata_object.hpp"
#include "dxf/Dxf_group_object.hpp"
#include "dxf/Dxf_idbuffer_object.hpp"
#include "dxf/Dxf_imagedef_object.hpp"
#include "dxf/Dxf_imagedef_reactor_object.hpp"
#include "dxf/Dxf_layer_filter_object.hpp"
#include "dxf/Dxf_layer_index_object.hpp"
#include "dxf/Dxf_layout_object.hpp"
#include "dxf/Dxf_lightlist_object.hpp"
#include "dxf/Dxf_material_object.hpp"
#include "dxf/Dxf_mlinestyle_object.hpp"
#include "dxf/Dxf_object_ptr_object.hpp"
#include "dxf/Dxf_plotsettings_object.hpp"
#include "dxf/Dxf_rastervariables_object.hpp"
#include "dxf/Dxf_render_object.hpp"
#include "dxf/Dxf_section_object.hpp"
#include "dxf/Dxf_sortentstable_object.hpp"
#include "dxf/Dxf_spatial_filter_object.hpp"
#include "dxf/Dxf_spatial_index_object.hpp"
#include "dxf/Dxf_sunstudy_object.hpp"
#include "dxf/Dxf_tablestyle_object.hpp"
#include "dxf/Dxf_underlaydefinition_object.hpp"
#include "dxf/Dxf_vba_project_object.hpp"
#include "dxf/Dxf_visualstyle_object.hpp"
#include "dxf/Dxf_wipeoutvariables_object.hpp"
#include "dxf/Dxf_xrecord_object.hpp"
#include "dxf/Dxf_user_object.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Group;

SGAL_END_NAMESPACE

DXF_BEGIN_NAMESPACE

class Dxf_boundary_path;
class Dxf_polyline_boundary_path;
class Dxf_pattern_data;

//!
class SGAL_SGAL_DECL Dxf_parser : public SGAL::Base_loader {
public:

  friend Dxf_hatch_entity;
  friend Dxf_boundary_path;
  friend Dxf_polyline_boundary_path;
  friend Dxf_pattern_data;
  friend Dxf_layout_object;

  /*! Construct.
   */
  Dxf_parser();

  /*! Parse.
   */
  virtual SGAL::Loader_code operator()(std::istream& is, SGAL::Scene_graph* sg,
                                       const SGAL::String& filename);

  /*! Set the flag that determines whether to report unrecognized code.
   */
  void set_report_unrecognized_code(bool flag);

protected:
  // Section parsers:
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

  /*! Parse one class.
   */
  void parse_class();

  /*! Parse one BLOCK.
   */
  void parse_block();

  /*! Parse one ENDBLK.
   */
  void parse_endblk();

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

  enum Code_type { STRING, DOUBLE, INT8, INT16, INT32, UINT, BOOL };

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
  typedef SGAL::String Dxf_header::*    String_header;
  typedef float Dxf_header::*           Float_header;
  typedef double Dxf_header::*          Double_header;
  typedef double (Dxf_header::*Double_2d_header)[2];
  typedef double (Dxf_header::*Double_3d_header)[3];
  typedef int8_t Dxf_header::*          Int8_header;
  typedef int16_t Dxf_header::*         Int16_header;
  typedef int32_t Dxf_header::*         Int32_header;
  typedef SGAL::Uint Dxf_header::*      Uint_header;
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

  //! Information of a header member.
  struct Header_member {
    Header_member(Header_member_type handle, std::list<int> codes) :
      m_handle(handle), m_codes(codes)
    {}
    Header_member_type m_handle;
    std::list<int> m_codes;
  };

  //@}

  /// \name Class types
  //@{

  //! Class data member types.
  typedef SGAL::String Dxf_class::*     String_class;
  typedef int32_t Dxf_class::*          Int32_class;
  typedef int8_t Dxf_class::*           Int8_class;

  //! The variant type of handle to all types of class data members.
  typedef boost::variant<String_class, Int8_class, Int32_class>
                                        Class_member_type;

  //@}

  /// \name Block types
  //@{

  //! Class data member types.
  typedef SGAL::String Dxf_block::*     String_block;
  typedef SGAL::Uint Dxf_block::*       Uint_block;
  typedef int16_t Dxf_block::*          Int16_block;
  typedef double (Dxf_block::*Double_3d_block)[3];

  //! The variant type of handle to all types of block data members.
  typedef boost::variant<String_block,
                         Uint_block,
                         Int16_block,
                         Double_3d_block>       Block_member_type;

  //! Information of a header member.
  struct Block_member {
    Block_member(Block_member_type handle, int size, int index) :
      m_handle(handle), m_size(size), m_index(index)
    {}

    Block_member_type m_handle;
    int m_size;
    int m_index;
  };

  //@}

  /// \name Table types
  //@{

  //! The type of a table parser member function.
  typedef void(Dxf_parser::*Table_parser)(void);

  //@}

  /// \name Base entity types
  //@{

  //! The type of an entity parser member function.
  typedef void(Dxf_parser::*Entity_parser)(void);

  /*! Parse a 3dface entity. */
  void parse_3dface_entity();

  /*! Parse a 3dsolid entity. */
  void parse_3dsolid_entity();

  /*! Parse an acad_proxy entity. */
  void parse_acad_proxy_entity();

  /*! Parse an arc entity. */
  void parse_arc_entity();

  /*! Parse an arcalignedtext entity. */
  void parse_arcalignedtext_entity();

  /*! Parse an attdef entity. */
  void parse_attdef_entity();

  /*! Parse an attrib entity. */
  void parse_attrib_entity();

  /*! Parse a body entity. */
  void parse_body_entity();

  /*! Parse a circle entity. */
  void parse_circle_entity();

  /*! Parse a dimension entity. */
  void parse_dimension_entity();

  /*! Parse an ellipse entity. */
  void parse_ellipse_entity();

  /*! Parse a hatch entity. */
  void parse_hatch_entity();

  /*! Parse an image entity. */
  void parse_image_entity();

  /*! Parse an insert entity. */
  void parse_insert_entity();

  /*! Parse a leader entity. */
  void parse_leader_entity();

  /*! Parse a line entity. */
  void parse_line_entity();

  /*! Parse a lwpolyline entity. */
  void parse_lwpolyline_entity();

  /*! Parse an mline entity. */
  void parse_mline_entity();

  /*! Parse an mtext entity. */
  void parse_mtext_entity();

  /*! Parse an oleframe entity. */
  void parse_oleframe_entity();

  /*! Parse an ole2frame entity. */
  void parse_ole2frame_entity();

  /*! Parse a point entity. */
  void parse_point_entity();

  /*! Parse a polyline entity. */
  void parse_polyline_entity();

  /*! Parse a ray entity. */
  void parse_ray_entity();

  /*! Parse a region entity. */
  void parse_region_entity();

  /*! Parse a rtext entity. */
  void parse_rtext_entity();

  /*! Parse a seqend entity. */
  void parse_seqend_entity();

  /*! Parse a shape entity. */
  void parse_shape_entity();

  /*! Parse a solid entity. */
  void parse_solid_entity();

  /*! Parse a spline entity. */
  void parse_spline_entity();

  /*! Parse a text entity. */
  void parse_text_entity();

  /*! Parse a tolerance entity. */
  void parse_tolerance_entity();

  /*! Parse a trace entity. */
  void parse_trace_entity();

  /*! Parse a vertex entity. */
  void parse_vertex_entity();

  /*! Parse a viewport entity. */
  void parse_viewport_entity();

  /*! Parse a wipeout entity. */
  void parse_wipeout_entity();

  /*! Parse an xline entity. */
  void parse_xline_entity();

  //@}

  /// \name Object types
  //@{

  //! The type of an object parser member function.
  typedef void(Dxf_parser::*Object_parser)(void);

  void parse_acad_proxy_object();
  void parse_acdbdictionarywdflt_object();
  void parse_acdbplaceholder_object();
  void parse_acdbnavisworksmodeldef_object();
  void parse_datatable_object();
  void parse_dictionary_object();
  void parse_dictionaryvar_object();
  void parse_dimassoc_object();
  void parse_field_object();
  void parse_geodata_object();
  void parse_group_object();
  void parse_idbuffer_object();
  void parse_imagedef_object();
  void parse_imagedef_reactor_object();
  void parse_layer_index_object();
  void parse_layer_filter_object();
  void parse_layout_object();
  void parse_lightlist_object();
  void parse_material_object();
  void parse_mlinestyle_object();
  void parse_object_ptr_object();
  void parse_plotsettings_object();
  void parse_rastervariables_object();
  void parse_render_object();
  void parse_section_object();
  void parse_spatial_index_object();
  void parse_spatial_filter_object();
  void parse_sortentstable_object();
  void parse_sunstudy_object();
  void parse_tablestyle_object();
  void parse_underlaydefinition_object();
  void parse_visualstyle_object();
  void parse_vba_project_object();
  void parse_wipeoutvariables_object();
  void parse_xrecord_object();
  //@}

  /*! Import a value and assign it to a record member.
   */
  template <typename Handle, typename Record_>
  void assign_simple_record_value(Code_type ct, Handle handle, Record_& record)
  {
    typedef Record_                             Record;

    typedef SGAL::String Record::*              String_record;
    typedef SGAL::Uint Record::*                Uint_record;
    typedef int8_t Record::*                    Int8_record;
    typedef int16_t Record::*                   Int16_record;
    typedef int32_t Record::*                   Int32_record;
    typedef double Record::*                    Double_record;

    switch (ct) {
     case STRING: assign_member<String_record>(handle, record); break;
     case UINT: assign_member<Uint_record>(handle, record);  break;
     case INT8: assign_member<Int8_record>(handle, record);  break;
     case INT16: assign_member<Int16_record>(handle, record);  break;
     case INT32: assign_member<Int32_record>(handle, record);  break;
     case DOUBLE: assign_member<Double_record>(handle, record);  break;
     default: break;
    }
  }

  /*! Assign a simple record value if listed.
   * \return true if listed (implies that a value was read) false otherwise.
   */
  template <typename Record, typename Members>
  bool assign_simple_record_value(int code, Record& record, Members& members)
  {
    auto bit = members.find(code);
    if (bit == members.end()) return false;

    auto ct = code_type(code);
    auto handle = bit->second;
    assign_simple_record_value(ct, handle, record);
    return true;
  }

  /*! Import a value and assign it to a record member.
   * Also handle arrays of doubles.
   */
  template <typename MemberVariant, typename Record_>
  void assign_record_value(Code_type ct, int size, MemberVariant handle,
                           Record_& record, int index)
  {
    typedef Record_                             Record;

    typedef SGAL::String Record::*              String_record;
    typedef bool Record::*                      Bool_record;
    typedef int8_t Record::*                    Int8_record;
    typedef int16_t Record::*                   Int16_record;
    typedef int32_t Record::*                   Int32_record;
    typedef double Record::*                    Double_record;
    typedef SGAL::Uint Record::*                Uint_record;
    typedef double (Record::*Double_2d_record)[2];
    typedef double (Record::*Double_3d_record)[3];

    switch (ct) {
     case STRING: assign_member<String_record>(handle, record); break;
     case BOOL: assign_member<Bool_record>(handle, record); break;
     case INT8: assign_member<Int8_record>(handle, record); break;
     case INT16: assign_member<Int16_record>(handle, record); break;
     case INT32: assign_member<Int32_record>(handle, record); break;
     case UINT: assign_member<Uint_record>(handle, record); break;

     case DOUBLE:
      switch (size) {
       case 1: assign_member<Double_record>(handle, record); break;
       case 2: assign_member<Double_2d_record>(handle, record, index); break;
       case 3: assign_member<Double_3d_record>(handle, record, index); break;
      }
      break;

     default: SGAL_error();
    }
  }

  /*! Assign a record value if listed.
   * \return true if listed (implies that a value was read) false otherwise.
   */
  template <typename Record, typename Members>
  bool assign_record_value(int code, Record& record, Members& members)
  {
    auto it = members.find(code);
    if (it == members.end()) return false;

    auto ct = code_type(code);
    auto handle = it->second.m_handle;
    auto size = it->second.m_size;
    auto index = it->second.m_index;
    assign_record_value(ct, size, handle, record, index);
    return true;
  }

  /*! Import a value and pass it to a record handler.
   */
  template <typename Handler, typename Record_>
  void handle_record_value(Code_type ct, Handler handler, Record_& record)
  {
    switch (ct) {
     case STRING: handle_string_item(handler, record);  break;
     case INT8: handle_item<int8_t>(handler, record);  break;
     case INT16: handle_item<int16_t>(handler, record);  break;
     case INT32: handle_item<int32_t>(handler, record);  break;
     case UINT: handle_item<SGAL::Uint>(handler, record);  break;
     case DOUBLE: handle_item<double>(handler, record);  break;
     default: SGAL_error();
    }
  }

  /*! Handle a record value if listed.
   * \return true if listed (implies that a value was read) false otherwise.
   */
  template<typename Record, typename Handlers>
  bool handle_record_value(int code, Record& record, Handlers& handlers)
  {
    auto it = handlers.find(code);
    if (it == handlers.end()) return false;
    auto ct = code_type(code);
    auto handler = it->second;
    handle_record_value(ct, handler, record);
    return true;
  }

  /// Handle marker.
  //@{

  //! Handle a marker in case record.handle_marker() is defined.
  template <bool what, typename Record>
  bool handle_marker(const SGAL::String& marker, Record& record,
                     char (*)[what] = 0)
  { return record.handle_marker(marker); }

  //! Handle a marker in case record.handle_marker() is not defined.
  template <bool what, typename Record>
  bool handle_marker(const SGAL::String& marker, Record& record,
                     char (*)[!what] = 0)
  { return true; }

  // Define a helper class that detects the presence of the member function
  // handle_marker()
  BOOST_TTI_HAS_MEMBER_FUNCTION(handle_marker);
  //@}

  /// Handle special codes.
  //@{

  //!
  template <bool what, typename Record>
  bool handle_value(int code, const SGAL::String& value, Record& record,
                    char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, bool value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, int8_t value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, int16_t value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, int32_t value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, SGAL::Uint value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, float value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, double value, Record& record, char (*)[what] = 0)
  { return record.handle_value(code, value); }

  //@}

  /// Handle special codes place holders.
  //@{

  //!
  template <bool what, typename Record>
  bool handle_value(int code, const SGAL::String& value, Record& record,
                    char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, bool value, Record& record, char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, int8_t value, Record& record, char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, int16_t value, Record& record, char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, int32_t value, Record& record, char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, SGAL::Uint value, Record& record,
                    char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, float value, Record& record, char (*)[!what] = 0)
  { return false; }

  //!
  template <bool what, typename Record>
  bool handle_value(int code, double value, Record& record, char (*)[!what] = 0)
  { return false; }

  //@}

  // Define a helper class that detects the definition of the member function
  // handle_value()
  BOOST_TTI_HAS_MEMBER_FUNCTION(handle_value);

  /*! Read a value of a record that requires special handling
   */
  template <typename Record>
  void handle_record_special_value(int code, Record& record)
  {
    auto ct = code_type(code);

    SGAL::String str;
    bool bool_val;
    int8_t int8_val;
    int16_t int16_val;
    int32_t int32_val;
    SGAL::Uint uint_val;
    float float_val;
    double double_val;

    std::stringstream stream;
    SGAL::String msg("Unrecognized code ");
    msg += std::to_string(code);

    // Below, we call the handle_value() member function only if Record has a
    // member function called "handle_value" with the signature that matches
    // the type of the value, e.g., for a SGAL::String value, we require:
    //    bool (Record::*)(int, const SGAL::String&)
    switch (ct) {
     case STRING:
      import_value(str);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, const SGAL::String&)>::value>(code, str,
                                                               record))
        return;
      msg += ", string value: " + str;
      break;

     case BOOL:
      import_value(bool_val);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, bool)>::value>(code, bool_val, record))
        return;
      msg += ", bool value: " + std::to_string(bool_val);
      break;

     case INT8:
      import_value(int8_val);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, int8_t)>::value>(code, int8_val, record))
        return;
      msg += ", int8_t value: " + std::to_string((int)int8_val);
      break;

     case INT16:
      import_value(int16_val);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, int16_t)>::value>(code, int16_val, record))
        return;
      msg += ", int16_t value: " + std::to_string(int16_val);
      break;

     case INT32:
      import_value(int32_val);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, int32_t)>::value>(code, int32_val, record))
        return;
      msg += ", int32_t value: " + std::to_string(int32_val);
      break;

     case UINT:
      import_value(uint_val);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, SGAL::Uint)>::value>(code, uint_val, record))
        return;
      stream << std::hex << uint_val;
      msg += ", unsigned int value: 0x" + stream.str();
      break;

     case DOUBLE:
      import_value(double_val);
      if (handle_value<has_member_function_handle_value
          <bool (Record::*)(int, double)>::value>(code, double_val, record))
        return;
      msg += ", double value: " + std::to_string(double_val);
      break;

     default: SGAL_error();
    }

    if (m_report_unrecognized_code) {
      msg += ", at line " + std::to_string(m_line);
      SGAL_warning_msg(0, msg.c_str());
    }
  }

  /// Handle base type.
  //@{

  /*! Read a value of a base record.
   * This implementation is invoked only if Record::Base is defined.
   * \return true if a value was read and false otherwise.
   */
  template <bool what, typename Record>
  bool read_base_record(int code, Record& record, char (*)[what] = 0)
  {
    typedef typename Record::Base                       Base_record;
    auto& base_members =
      Dxf_simple_record_wrapper<Base_record>::s_record_members;
    auto& base_record = static_cast<Base_record&>(record);
    if (assign_simple_record_value(code, base_record, base_members)) return true;
    return false;
  }

  /*! Read a value of a base record.
   * This implementation is invoked only if Record::Base is not defined.
   * \return false always.
   */
  template <bool what, typename Record>
  bool read_base_record(int code, Record& record, char (*)[!what] = 0)
  { return false; }

  // Define a helper class that detects the presence of the nested type Base
  BOOST_TTI_HAS_TYPE(Base);

  /*! Read a value of a base record.
   * \return true if a value was read and false otherwise.
   */
  template <typename Record>
  bool read_base_record(int code, Record& record)
  { return read_base_record<has_type_Base<Record>::value>(code, record); }

  //@}

  /*! Parse a record. */
  template <typename Record>
  void parse_record(Record& record)
  {
    record.init();
    record.set_parser(this);

    while (true) {
      int code;
      import_code(code);
      if (0 == code) break;

      if (100 == code) {
        import_value(m_marker);
        // The return value of the marker handler indicates whether the marker
        // is expected for tthis record. If it's not, we silently return.
        // Apparently, some markers (e.g., in plotsettings object inficate the
        // end of the object.
        if (! handle_marker<has_member_function_handle_marker
            <bool (Record::*)(const SGAL::String&)>::value>(m_marker, record))
          break;
        continue;
      }

      if ((102 == code) || (1002 == code)) {
        read_xdata_block(code, record);
        continue;
      }

      if ((1000 <= code) && (code <= 1071)) {
        read_extended_data(code, record);
        continue;
      }

      if (read_base_record(code, record)) continue;

      auto& members = Dxf_record_wrapper<Record>::s_record_members;
      if (assign_record_value(code, record, members)) continue;
      auto& handlers = Dxf_record_wrapper<Record>::s_record_handlers;
      if (handle_record_value(code, record, handlers)) continue;

      handle_record_special_value(code, record);
    }
  }

  //! The pending code, in case there is one.
  int m_pending_code;

  //! Indicates whether there is a code pending.
  bool m_is_pending;

  //! Header data
  Dxf_header m_header;

  // Classes
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

  Dxf_extended_data* m_extended_data;

  // Blocks
  std::list<Dxf_block> m_blocks;

  // Entities
  Dxf_3dface_entity m_3dface_entity;
  Dxf_3dsolid_entity m_3dsolid_entity;
  Dxf_acad_proxy_entity m_acad_proxy_entity;
  Dxf_arc_entity m_arc_entity;
  Dxf_arcalignedtext_entity m_arcalignedtext_entity;
  Dxf_attdef_entity m_attdef_entity;
  Dxf_attrib_entity m_attrib_entity;
  Dxf_body_entity m_body_entity;
  Dxf_circle_entity m_circle_entity;
  Dxf_dimension_entity m_dimension_entity;
  Dxf_ellipse_entity m_ellipse_entity;
  std::vector<Dxf_hatch_entity> m_hatch_entities;
  Dxf_image_entity m_image_entity;
  Dxf_insert_entity m_insert_entity;
  Dxf_leader_entity m_leader_entity;
  std::vector<Dxf_line_entity> m_line_entities;
  Dxf_lwpolyline_entity m_lwpolyline_entity;
  Dxf_mline_entity m_mline_entity;
  Dxf_mtext_entity m_mtext_entity;
  Dxf_oleframe_entity m_oleframe_entity;
  Dxf_ole2frame_entity m_ole2frame_entity;
  Dxf_point_entity m_point_entity;
  std::vector<Dxf_polyline_entity> m_polyline_entities;
  Dxf_ray_entity m_ray_entity;
  Dxf_region_entity m_region_entity;
  Dxf_rtext_entity m_rtext_entity;
  Dxf_seqend_entity m_seqend_entity;
  Dxf_shape_entity m_shape_entity;
  Dxf_solid_entity m_solid_entity;
  std::vector<Dxf_spline_entity> m_spline_entities;
  Dxf_text_entity m_text_entity;
  Dxf_tolerance_entity m_tolerance_entity;
  Dxf_trace_entity m_trace_entity;
  Dxf_vertex_entity m_vertex_entity;
  Dxf_viewport_entity m_viewport_entity;
  Dxf_wipeout_entity m_wipeout_entity;
  Dxf_xline_entity m_xline_entity;

  std::list<Dxf_user_entity> m_user_entities;

  // Objects
  Dxf_acad_proxy_object m_acad_proxy_object;
  Dxf_acdbdictionarywdflt_object m_acdbdictionarywdflt_object;
  Dxf_acdbplaceholder_object m_acdbplaceholder_object;
  Dxf_acdbnavisworksmodeldef_object m_acdbnavisworksmodeldef_object;
  Dxf_datatable_object m_datatable_object;
  Dxf_dictionary_object m_dictionary_object;
  Dxf_dictionaryvar_object m_dictionaryvar_object;
  Dxf_dimassoc_object m_dimassoc_object;
  Dxf_field_object m_field_object;
  Dxf_geodata_object m_geodata_object;
  Dxf_group_object m_group_object;
  Dxf_idbuffer_object m_idbuffer_object;
  Dxf_imagedef_object m_imagedef_object;
  Dxf_imagedef_reactor_object m_imagedef_reactor_object;
  Dxf_layer_index_object m_layer_index_object;
  Dxf_layer_filter_object m_layer_filter_object;
  Dxf_layout_object m_layout_object;
  Dxf_lightlist_object m_lightlist_object;
  std::vector<Dxf_material_object> m_material_objects;
  Dxf_mlinestyle_object m_mlinestyle_object;
  Dxf_object_ptr_object m_object_ptr_object;
  Dxf_plotsettings_object m_plotsettings_object;
  Dxf_rastervariables_object m_rastervariables_object;
  Dxf_render_object m_render_object;
  Dxf_section_object m_section_object;
  Dxf_spatial_index_object m_spatial_index_object;
  Dxf_spatial_filter_object m_spatial_filter_object;
  Dxf_sortentstable_object m_sortentstable_object;
  Dxf_sunstudy_object m_sunstudy_object;
  Dxf_tablestyle_object m_tablestyle_object;
  Dxf_underlaydefinition_object m_underlaydefinition_object;
  Dxf_visualstyle_object m_visualstyle_object;
  Dxf_vba_project_object m_vba_project_object;
  Dxf_wipeoutvariables_object m_wipeoutvariables_object;
  Dxf_xrecord_object m_xrecord_object;

  std::list<Dxf_user_object> m_user_objects;

private:
  //! Indicates whether to report unrecognized codes.
  bool m_report_unrecognized_code;

  //! Number of polylines.
  size_t m_polylines_num;

  //! Marker
  SGAL::String m_marker;

  /*! Clear the parser. Deallocate data structure and prepare for reuse.
   */
  void clear();

  /*! Obtain the type of a code
   * \param code the given code.
   */
  Code_type code_type(int code);

  /*! Export a code
   */
  void export_code(int code)
  {
    m_pending_code = code;
    m_is_pending = true;
  }

  /*! Import a code.
   * \param[code] the code variable.
   */
  template <typename T>
  void import_code(T& code)
  {
    if (m_is_pending) {
      code = m_pending_code;
      m_is_pending = false;
    }
    else {
      (*m_is) >> code;
      m_is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      ++m_line;
    }
    SGAL_TRACE_CODE(m_trace_code,
                    std::cout << "[" << std::to_string(m_line) << "] "
                    << "Dxf_parser::import_code(): "
                    << code << std::endl;);
  }

  /*! Import a value.
   * \param[variable] the target variable.
   */
  template <typename T>
  void import_value(T& variable)
  {
    Dxf_importer<T> importer(*this);
    importer(variable);
  }

  /*! Import a value and assign it to a struct member (of the same type,
   * naturally).
   * \param[handle] handle the handle to the struct member.
   * \param[target] target the target struct.
   */
  template <typename T, typename MemberVariant, typename Target>
  void assign_member(MemberVariant handle, Target& target)
  { import_value(target.*(boost::get<T>(handle))); }

  /*! Import a value and assign it to an item of an array struct-member (of the
   * same type, naturally).
   * \param[handle] handle the handle to the member.
   * \param[target] target the target struct.
   * \param[index] the index of the array item.
   */
  template <typename T, typename MemberVariant, typename Target>
  void assign_member(MemberVariant handle, Target& target, int index)
  { import_value((target.*(boost::get<T>(handle)))[index]); }

  /// Import to handlers
  //@{
  /*! Import a value and pass it to a handler (that handles the same type,
   * naturally).
   * \param[handler] handler the handler variant struct member-function.
   * \param[target] target the target struct.
   */
  template <typename T, typename HandrelVariant, typename Record>
  void handle_item(HandrelVariant handler, Record& record)
  {
    Dxf_importer<T> importer(*this);

    typedef void(Record::*Handler)(T);
    T value;
    importer(value);
    (record.*(boost::get<Handler>(handler)))(value);
  }

  /*! Import a string value and pass it to a handler (that handles a string
   * value).
   * We need a dedicated template function because the type of the formal
   * argument of the handle, namely 'const SGAL::String&' is different than the
   * type of the local variable, namely 'SGAL::String', used to temporary store
   * the value.
   * \param[handler] handler the handler struct member-function.
   * \param[target] target the target struct.
   */
  template <typename HandrelVariant, typename Record>
  void handle_string_item(HandrelVariant handler, Record& record)
  {
    typedef void(Record::*Handler)(const SGAL::String&);
    SGAL::String value;
    import_value(value);
    (record.*(boost::get<Handler>(handler)))(value);
  }
  //@}

  /*! Has_xdata is a generic-template-struct service that evaluates as follows:
   * If A has a data member called x_data, then Has_xdata<A>::value == true,
   * else (A doesn't have a data member called x_data)
   * Has_xdata<B>::value == false.
   */
  template <typename T, typename = int>
  struct Has_xdata : std::false_type {};

  template <typename T>
  struct Has_xdata<T, decltype((void) T::m_xdata, 0)> : std::true_type {};

  /*! Store extended data.
   * There are two implementations; one is dispatched when x_data is a data
   * member of "Record" and the other is dispatched otherwise. We employ the
   * dispatching by type mechanism; see
   * http://barendgehrels.blogspot.co.il/2010/10/tag-dispatching-by-type-tag-dispatching.html
   *
   */
  template <bool what, typename Record>
  void store_xdata(Record& record, const SGAL::String& name,
                   const SGAL::String& value, char (*)[what] = 0)
  { record.m_xdata[name].push_back(value); }

  /*! Store extended data.
   */
  template <bool what, typename Record>
  void store_xdata(Record& record, const SGAL::String& name,
                   const SGAL::String& value, char (*)[!what] = 0)
  {}

  /*! Read x data
   */
  template <typename Record>
  void read_xdata_block(int code, Record& record)
  {
    char c;
    (*m_is) >> c;
    if ('{' != c) {
      //! \todo when is this valid and what should we do here?
      SGAL::String str;
      import_value(str);
      return;
    }

    SGAL::String name;
    if (102 == code) import_value(name);
    else {
      m_is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      ++m_line;
    }
    SGAL::String str;
    import_value(str);
    while ("}" != str) {
      import_value(str);
      store_xdata<Has_xdata<Record>::value>(record, name, str);
    }
  }

  /*! Has_xdata is a generic-template-struct service that evaluates as follows:
   * If A has a data member called x_data, then Has_xdata<A>::value == true,
   * else (A doesn't have a data member called x_data)
   * Has_xdata<B>::value == false.
   */
  template <typename T, typename = int>
  struct Has_extended_data : std::false_type {};

  //!
  template <typename T>
  struct Has_extended_data<T, decltype((void) T::m_extended_data, 0)> :
    std::true_type {};

  //!
  template <bool what, typename Record>
  Dxf_extended_data* get_extended_data(const SGAL::String& name, Record& record,
                                       char (*)[what] = 0)
  {
    // We store the extended data at the respcetive record, as the appid
    // table entry hasn't been read yet.
    record.m_extended_data.resize(record.m_extended_data.size() + 1);
    return &(record.m_extended_data.back());
  }

  //!
  template <bool what, typename Record>
  Dxf_extended_data* get_extended_data(const SGAL::String& name, Record& record,
                                       char (*)[!what] = 0)
  { return nullptr; }

  /*! Read extended data
   */
  template <typename Record>
  void read_extended_data(int code, Record& record)
  {
    if (1001 == code) {
      SGAL::String app_name;
      import_value(app_name);
      m_extended_data =
        get_extended_data<Has_extended_data<Record>::value>(app_name, record);
      return;
    }

    if (m_extended_data == nullptr) {
      SGAL_error_msg("Extended data record missing");
      return;
    }

    auto ct = code_type(code);
    auto& members = Dxf_record_wrapper<Dxf_extended_data>::s_record_members;
    auto it = members.find(code);
    SGAL_assertion(it != members.end());
    auto handle = it->second.m_handle;
    auto size = it->second.m_size;
    auto index = it->second.m_index;
    assign_record_value(ct, size, handle, *m_extended_data, index);
  }

  /*! Parse a common table section, which aplies to all table types.
   * We cannot pass (a reference to) just the base table (Dxf_base_table), cause
   * it turn out that in the sub-section where only common-table codes should
   * reside, specific-entry codes reside as well; go figures....
   */
  template <typename Table>
  void parse_base_table(Table& table)
  {
    SGAL_TRACE_CODE(m_trace_code,
                    std::cout << "Dxf_parser::parse_base_table()"
                    << std::endl;);

    typedef typename Table::Entry               Entry;

    Dxf_base_table& base_table = table;
    auto& entries = table.m_entries;
    size_t max_num(0);

    while (true) {
      int code;
      import_code(code);
      SGAL_TRACE_CODE(m_trace_code,
                      std::cout << "Dxf_parser::parse_base_table() code: "
                      << code << std::endl;);
      if (0 == code) break;

      if (100 == code) {
        import_value(m_marker);
        continue;
      }

      if (102 == code) {
        read_xdata_block(code, base_table);
        continue;
      }

      // Code 70 applies to STYLE table records only (pre-R13 field), why?
      if (70 == code) {
        import_value(max_num);
        entries.reserve(max_num);
        continue;
      }

      auto& base_members =
        Dxf_simple_record_wrapper<Dxf_base_table>::s_record_members;
      if (assign_simple_record_value(code, base_table, base_members)) continue;

      // Handle specifc-entry table codes:
      // At this point we don't try to handle common entry code.
      entries.resize(1);
      auto& entry = entries.back();
      auto& members = Dxf_record_wrapper<Entry>::s_record_members;
      if (assign_record_value(code, entry, members)) continue;

      handle_record_special_value(code, entry);
    }
  }

  /*! Parse a table.
   */
  template <typename Table>
  void parse_table(Table& table, const std::string& name)
  {
    typedef typename Table::Entry               Entry;

    bool exceeded(false);
    parse_base_table(table);
    auto& entries = table.m_entries;

    while (true) {
      std::string str;
      import_value(str);
      if ("ENDTAB" == str) return;

      SGAL_assertion(str == name);
      if (entries.size() == entries.capacity()) {
        // It is specifiied that Code 70 applies to STYLE table records only
        // (pre-R13 field), why? What is special about STYLE, and why not make
        // things consistent?
        // Anyhow, Instead of simply iterating over the entries and return when
        // either we run out of entries or the max number is reached, we only
        // look for the end code (namely 0). If we exceed the promised max
        // number, we resize the table.
        if ("STYLE" == str) {
          SGAL_warning_msg(exceeded, "Maximum number of entries exceeded!");
          exceeded = true;
        }
        entries.reserve(entries.size() + 1);
      }
      entries.resize(entries.size() + 1);
      auto& entry = entries.back();
      entry.init();

      while (true) {
        int code;
        import_code(code);
        if (0 == code) break;

        if (100 == code) {
          import_value(m_marker);
          continue;
        }

        if ((102 == code) || (1002 == code)) {
          read_xdata_block(code, entry);
          continue;
        }

        if ((1000 <= code) && (code <= 1071)) {
          read_extended_data(code, entry);
          continue;
        }

        auto& base_members =
          Dxf_simple_record_wrapper<Dxf_table_entry>::s_record_members;
        auto& base_entry = static_cast<Dxf_table_entry&>(entry);
        if (assign_simple_record_value(code, base_entry, base_members)) continue;

        auto& members = Dxf_record_wrapper<Entry>::s_record_members;
        if (assign_record_value(code, entry, members)) continue;
        auto& handlers = Dxf_record_wrapper<Entry>::s_record_handlers;
        if (handle_record_value(code, entry, handlers)) continue;

        handle_record_special_value(code, entry);
      }
    }
  }

  /// Various record parsers.
  //@{

  /*! Parse a regular boundary path.
   */
  void parse_boundary_path(Dxf_boundary_path& path);

  /*! Parse a polyline boundary path.
   */
  void parse_polyline_boundary_path(Dxf_polyline_boundary_path& path);

  /*! Parse a pattern data.
   */
  void parse_pattern_data(Dxf_pattern_data& pattern_data);

  /*! Parse a line edge
   */
  void parse_line_edge(Dxf_line_edge& edge);

  /*! Parse a circle edge
   */
  void parse_circle_edge(Dxf_circle_edge& edge);

  /*! Parse an ellipse edge
   */
  void parse_ellipse_edge(Dxf_ellipse_edge& edge);

  /*! Parse a spline edge
   */
  void parse_spline_edge(Dxf_spline_edge& edge);

  //@}

  /*! Read a comment line.
   */
  void read_comment();

  /*! Handle an unrecognized code.
   */
  void read_unrecognized(int code);

  static const std::map<SGAL::String, Section_parser> s_sections;
  static const std::map<SGAL::String, Header_member> s_header_members;
  static const std::vector<Code_range> s_code_ranges;
  static const std::array<SGAL::String, 8> s_code_type_names;
  static const std::map<int, Class_member_type> s_class_members;
  static const std::map<SGAL::String, Table_parser> s_tables;
  static const std::map<SGAL::String, Entity_parser> s_entities;
  static const std::map<SGAL::String, Object_parser> s_objects;

  /*! Add polylines provided in hatch entities.
   */
  void add_polylines(const Dxf_hatch_entity& hatch, SGAL::Group* root);

  /*! Add lines provided in line entities.
   */
  void add_polylines(const Dxf_line_entity& line, SGAL::Group* root);

  /*! Add polylines provided in spline entities.
   */
  void add_polylines(const Dxf_spline_entity& spline, SGAL::Group* root);

  /*! Add polylines provided in polyline entities.
   */
  void add_polylines(const Dxf_polyline_entity& polyline, SGAL::Group* root);

  /*! Add polylines (without bulge).
   * \param[in] polylines
   * \param[in] root
   * \param[in] closed
   */
  void add_polylines(const Dxf_hatch_entity& hatch_entity,
                     const std::list<Dxf_polyline_boundary_path*>& polylines,
                     SGAL::Group* root, bool closed);

  /*! Add polylines with bulge.
   * \param[in] polylines
   * \param[in] root
   * \param[in] closed
   */
  void add_polylines_with_bulge(const Dxf_hatch_entity& hatch_entity,
                                const std::list<Dxf_polyline_boundary_path*>&
                                polylines,
                                SGAL::Group* root, bool closed);
};

//! \brief sets the flag that determines whether to report unrecognized code.
inline void Dxf_parser::set_report_unrecognized_code(bool flag)
{ m_report_unrecognized_code = flag; }

DXF_END_NAMESPACE

#endif
