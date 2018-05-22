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
#include <type_traits>

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tti/has_member_function.hpp>

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
#include "SGAL/Dxf_base_entity.hpp"
#include "SGAL/Dxf_base_object.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Loader_code.hpp"
#include "SGAL/Trace.hpp"

// Entities
#include "SGAL/Dxf_3dface_entity.hpp"
#include "SGAL/Dxf_3dsolid_entity.hpp"
#include "SGAL/Dxf_acad_proxy_entity.hpp"
#include "SGAL/Dxf_arc_entity.hpp"
#include "SGAL/Dxf_arcalignedtext_entity.hpp"
#include "SGAL/Dxf_attdef_entity.hpp"
#include "SGAL/Dxf_attrib_entity.hpp"
#include "SGAL/Dxf_body_entity.hpp"
#include "SGAL/Dxf_circle_entity.hpp"
#include "SGAL/Dxf_dimension_entity.hpp"
#include "SGAL/Dxf_ellipse_entity.hpp"
#include "SGAL/Dxf_hatch_entity.hpp"
#include "SGAL/Dxf_image_entity.hpp"
#include "SGAL/Dxf_insert_entity.hpp"
#include "SGAL/Dxf_leader_entity.hpp"
#include "SGAL/Dxf_line_entity.hpp"
#include "SGAL/Dxf_lwpolyline_entity.hpp"
#include "SGAL/Dxf_mline_entity.hpp"
#include "SGAL/Dxf_mtext_entity.hpp"
#include "SGAL/Dxf_oleframe_entity.hpp"
#include "SGAL/Dxf_ole2frame_entity.hpp"
#include "SGAL/Dxf_point_entity.hpp"
#include "SGAL/Dxf_polyline_entity.hpp"
#include "SGAL/Dxf_ray_entity.hpp"
#include "SGAL/Dxf_region_entity.hpp"
#include "SGAL/Dxf_rtext_entity.hpp"
#include "SGAL/Dxf_seqend_entity.hpp"
#include "SGAL/Dxf_shape_entity.hpp"
#include "SGAL/Dxf_solid_entity.hpp"
#include "SGAL/Dxf_spline_entity.hpp"
#include "SGAL/Dxf_text_entity.hpp"
#include "SGAL/Dxf_tolerance_entity.hpp"
#include "SGAL/Dxf_trace_entity.hpp"
#include "SGAL/Dxf_vertex_entity.hpp"
#include "SGAL/Dxf_viewport_entity.hpp"
#include "SGAL/Dxf_wipeout_entity.hpp"
#include "SGAL/Dxf_xline_entity.hpp"

// Objects
#include "SGAL/Dxf_acad_proxy_object.hpp"
#include "SGAL/Dxf_acdbdictionarywdflt_object.hpp"
#include "SGAL/Dxf_acdbnavisworksmodeldef_object.hpp"
#include "SGAL/Dxf_acdbplaceholder_object.hpp"
#include "SGAL/Dxf_datatable_object.hpp"
#include "SGAL/Dxf_dictionary_object.hpp"
#include "SGAL/Dxf_dictionaryvar_object.hpp"
#include "SGAL/Dxf_dimassoc_object.hpp"
#include "SGAL/Dxf_field_object.hpp"
#include "SGAL/Dxf_geodata_object.hpp"
#include "SGAL/Dxf_group_object.hpp"
#include "SGAL/Dxf_idbuffer_object.hpp"
#include "SGAL/Dxf_imagedef_object.hpp"
#include "SGAL/Dxf_imagedef_reactor_object.hpp"
#include "SGAL/Dxf_layer_filter_object.hpp"
#include "SGAL/Dxf_layer_index_object.hpp"
#include "SGAL/Dxf_layout_object.hpp"
#include "SGAL/Dxf_lightlist_object.hpp"
#include "SGAL/Dxf_material_object.hpp"
#include "SGAL/Dxf_mlinestyle_object.hpp"
#include "SGAL/Dxf_object_ptr_object.hpp"
#include "SGAL/Dxf_plotsettings_object.hpp"
#include "SGAL/Dxf_rastervariables_object.hpp"
#include "SGAL/Dxf_render_object.hpp"
#include "SGAL/Dxf_section_object.hpp"
#include "SGAL/Dxf_sortentstable_object.hpp"
#include "SGAL/Dxf_spatial_filter_object.hpp"
#include "SGAL/Dxf_spatial_index_object.hpp"
#include "SGAL/Dxf_sunstudy_object.hpp"
#include "SGAL/Dxf_tablestyle_object.hpp"
#include "SGAL/Dxf_underlaydefinition_object.hpp"
#include "SGAL/Dxf_vba_project_object.hpp"
#include "SGAL/Dxf_visualstyle_object.hpp"
#include "SGAL/Dxf_wipeoutvariables_object.hpp"
#include "SGAL/Dxf_xrecord_object.hpp"

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
  typedef String Dxf_class::*           String_class;
  typedef int32_t Dxf_class::*          Int32_class;
  typedef int8_t Dxf_class::*           Int8_class;

  //! The variant type of handle to all types of class data members.
  typedef boost::variant<String_class, Int8_class, Int32_class>
                                        Class_member_type;

  //@}

  /// \name Block types
  //@{

  //! Class data member types.
  typedef String Dxf_block::*           String_block;
  typedef Uint Dxf_block::*             Uint_block;
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

  typedef String Dxf_table_entry::*             String_base_entry;
  typedef Uint Dxf_table_entry::*               Uint_base_entry;
  typedef boost::variant<String_base_entry, Uint_base_entry>
                                                Base_entry_member_type;


  typedef String Dxf_base_table::*             String_base_table;
  typedef Uint Dxf_base_table::*               Uint_base_table;
  typedef boost::variant<String_base_table, Uint_base_table>
                                                Base_table_member_type;
  //@}

  /// \name Base entity types
  //@{

  //! The type of an entity parser member function.
  typedef void(Dxf_parser::*Entity_parser)(void);

  typedef String Dxf_base_entity::*                  String_base_entity;
  typedef Uint Dxf_base_entity::*                    Uint_base_entity;
  typedef int8_t Dxf_base_entity::*                  Int8_base_entity;
  typedef int16_t Dxf_base_entity::*                 Int16_base_entity;
  typedef int32_t Dxf_base_entity::*                 Int32_base_entity;
  typedef double Dxf_base_entity::*                  Double_base_entity;
  typedef boost::variant<String_base_entity,
                         Uint_base_entity,
                         Int8_base_entity,
                         Int16_base_entity,
                         Int32_base_entity,
                         Double_base_entity>         Base_entity_type;

  /*! Parse 3dface entity. */
  void parse_3dface_entity();

  /*! Parse 3dsolid entity. */
  void parse_3dsolid_entity();

  /*! Parse acad_proxy entity. */
  void parse_acad_proxy_entity();

  /*! Parse arc entity. */
  void parse_arc_entity();

  /*! Parse arcalignedtext entity. */
  void parse_arcalignedtext_entity();

  /*! Parse attdef entity. */
  void parse_attdef_entity();

  /*! Parse attrib entity. */
  void parse_attrib_entity();

  /*! Parse body entity. */
  void parse_body_entity();

  /*! Parse circle entity. */
  void parse_circle_entity();

  /*! Parse dimension entity. */
  void parse_dimension_entity();

  /*! Parse ellipse entity. */
  void parse_ellipse_entity();

  /*! Parse hatch entity. */
  void parse_hatch_entity();

  /*! Parse image entity. */
  void parse_image_entity();

  /*! Parse insert entity. */
  void parse_insert_entity();

  /*! Parse leader entity. */
  void parse_leader_entity();

  /*! Parse line entity. */
  void parse_line_entity();

  /*! Parse lwpolyline entity. */
  void parse_lwpolyline_entity();

  /*! Parse mline entity. */
  void parse_mline_entity();

  /*! Parse mtext entity. */
  void parse_mtext_entity();

  /*! Parse oleframe entity. */
  void parse_oleframe_entity();

  /*! Parse ole2frame entity. */
  void parse_ole2frame_entity();

  /*! Parse point entity. */
  void parse_point_entity();

  /*! Parse polyline entity. */
  void parse_polyline_entity();

  /*! Parse ray entity. */
  void parse_ray_entity();

  /*! Parse region entity. */
  void parse_region_entity();

  /*! Parse rtext entity. */
  void parse_rtext_entity();

  /*! Parse seqend entity. */
  void parse_seqend_entity();

  /*! Parse shape entity. */
  void parse_shape_entity();

  /*! Parse solid entity. */
  void parse_solid_entity();

  /*! Parse spline entity. */
  void parse_spline_entity();

  /*! Parse text entity. */
  void parse_text_entity();

  /*! Parse tolerance entity. */
  void parse_tolerance_entity();

  /*! Parse trace entity. */
  void parse_trace_entity();

  /*! Parse vertex entity. */
  void parse_vertex_entity();

  /*! Parse viewport entity. */
  void parse_viewport_entity();

  /*! Parse wipeout entity. */
  void parse_wipeout_entity();

  /*! Parse xline entity. */
  void parse_xline_entity();

  //@}

  /// \name Object types
  //@{

  //! The type of an object parser member function.
  typedef void(Dxf_parser::*Object_parser)(void);

  typedef String Dxf_base_object::*                  String_base_object;
  typedef Uint Dxf_base_object::*                    Uint_base_object;
  typedef int8_t Dxf_base_object::*                  Int8_base_object;
  typedef int16_t Dxf_base_object::*                 Int16_base_object;
  typedef int32_t Dxf_base_object::*                 Int32_base_object;
  typedef double Dxf_base_object::*                  Double_base_object;
  typedef boost::variant<String_base_object,
                         Uint_base_object,
                         Int8_base_object,
                         Int16_base_object,
                         Int32_base_object,
                         Double_base_object>         Base_object_type;

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

  void parse_dummy_object();
  //@}

  /*! Parse base record. */
  template <typename Handle, typename BaseRecord>
  void read_base_record_value(Code_type ct, Handle handle, BaseRecord& record)
  {
    typedef BaseRecord                                  Base_record;

    typedef String Base_record::*              String_record;
    typedef Uint Base_record::*                Uint_record;
    typedef int8_t Base_record::*              Int8_record;
    typedef int16_t Base_record::*             Int16_record;
    typedef int32_t Base_record::*             Int32_record;
    typedef double Base_record::*              Double_record;

    switch (ct) {
     case STRING: import_string_member<String_record>(handle, record); break;
     case UINT: import_uint_member<Uint_record>(handle, record);  break;
     case INT8: import_int8_member<Int8_record>(handle, record);  break;
     case INT16: import_member<Int16_record>(handle, record);  break;
     case INT32: import_member<Int32_record>(handle, record);  break;
     case DOUBLE: import_member<Double_record>(handle, record);  break;
     default: break;
    }
  }

  /*! Handle special.
   */
  template <bool what, typename Record>
  void handle_int16_value(int code, int16_t value, Record& record,
                          char (*)[what] = 0)
  { record.handle_value(code, value); }

  /*! Store extended data.
   */
  template <bool what, typename Record>
  void handle_int16_value(int code, int16_t value, Record& record,
                          char (*)[!what] = 0)
  {}
  /////////////////////////////////////////////////////////////////

  BOOST_TTI_HAS_MEMBER_FUNCTION(handle_value);

  /*! Read a value of a record that requires special handling
   */
  template <typename Record>
  void read_record_special_value(int code, Record& record)
  {
    auto ct = code_type(code);
    int16_t i16;
    switch (ct) {
     case INT16:
      m_is >> i16;
      // Call the read_record_special_value() only if Record has a member
      // function called "handle_value" with the signature:
      //   bool (Record::*)(int, int16)
      if (handle_int16_value<has_member_function_handle_value
          <bool (Record::*)(int, int16_t)>::
          value>(code, i16, record))
        return;
      // issue warning
    }
    // issue warning
  }

  /*! Parse record. */
  template <typename RecordWrapper, typename BaseMembers>
  void parse_record(RecordWrapper& record_wrapper, BaseMembers& base_members)
  {
    typedef RecordWrapper                               Record_wrapper;
    typedef typename Record_wrapper::Record             Record;
    //typedef typename Record_wrapper::Base_record        Base_record;
    typedef typename Record::Base                       Base_record;

    auto& record = record_wrapper.m_record;
    while (true) {
      int code;
      import_code(code);
      if (0 == code) break;

      if (100 == code) {
        import_string_value(m_marker);
        continue;
      }

      if ((102 == code) || (1002 == code)) {
        read_xdata_block(code, record);
        continue;
      }

      auto ct = code_type(code);
      auto bit = base_members.find(code);
      if (bit != base_members.end()) {
        auto handle = bit->second;
        auto& base_record = static_cast<Base_record&>(record);
        read_base_record_value(ct, handle, base_record);
        continue;
      }

      auto& members = record_wrapper.s_record_members;
      auto it = members.find(code);
      if (it != members.end()) {
        auto handle = it->second.m_handle;
        auto size = it->second.m_size;
        auto index = it->second.m_index;
        read_record_value<Record_wrapper>(ct, size, handle, record, index);
        continue;
      }

      read_record_special_value(code, record);
      continue;

      // read_unrecognized(code);
    }
  }

  //! The text input stream to parse.
  std::istream& m_is;

  //! The scene graph.
  Scene_graph* m_scene_graph;

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

  // Blocks
  std::list<Dxf_block> m_blocks;

  // Entities
  Dxf_record_wrapper<Dxf_3dface_entity> m_3dface_entity;
  Dxf_record_wrapper<Dxf_3dsolid_entity> m_3dsolid_entity;
  Dxf_record_wrapper<Dxf_acad_proxy_entity> m_acad_proxy_entity;
  Dxf_record_wrapper<Dxf_arc_entity> m_arc_entity;
  Dxf_record_wrapper<Dxf_arcalignedtext_entity> m_arcalignedtext_entity;
  Dxf_record_wrapper<Dxf_attdef_entity> m_attdef_entity;
  Dxf_record_wrapper<Dxf_attrib_entity> m_attrib_entity;
  Dxf_record_wrapper<Dxf_body_entity> m_body_entity;
  Dxf_record_wrapper<Dxf_circle_entity> m_circle_entity;
  Dxf_record_wrapper<Dxf_dimension_entity> m_dimension_entity;
  Dxf_record_wrapper<Dxf_ellipse_entity> m_ellipse_entity;
  Dxf_record_wrapper<Dxf_hatch_entity> m_hatch_entity;
  Dxf_record_wrapper<Dxf_image_entity> m_image_entity;
  Dxf_record_wrapper<Dxf_insert_entity> m_insert_entity;
  Dxf_record_wrapper<Dxf_leader_entity> m_leader_entity;
  Dxf_record_wrapper<Dxf_line_entity> m_line_entity;
  Dxf_record_wrapper<Dxf_lwpolyline_entity> m_lwpolyline_entity;
  Dxf_record_wrapper<Dxf_mline_entity> m_mline_entity;
  Dxf_record_wrapper<Dxf_mtext_entity> m_mtext_entity;
  Dxf_record_wrapper<Dxf_oleframe_entity> m_oleframe_entity;
  Dxf_record_wrapper<Dxf_ole2frame_entity> m_ole2frame_entity;
  Dxf_record_wrapper<Dxf_point_entity> m_point_entity;
  Dxf_record_wrapper<Dxf_polyline_entity> m_polyline_entity;
  Dxf_record_wrapper<Dxf_ray_entity> m_ray_entity;
  Dxf_record_wrapper<Dxf_region_entity> m_region_entity;
  Dxf_record_wrapper<Dxf_rtext_entity> m_rtext_entity;
  Dxf_record_wrapper<Dxf_seqend_entity> m_seqend_entity;
  Dxf_record_wrapper<Dxf_shape_entity> m_shape_entity;
  Dxf_record_wrapper<Dxf_solid_entity> m_solid_entity;
  Dxf_record_wrapper<Dxf_spline_entity> m_spline_entity;
  Dxf_record_wrapper<Dxf_text_entity> m_text_entity;
  Dxf_record_wrapper<Dxf_tolerance_entity> m_tolerance_entity;
  Dxf_record_wrapper<Dxf_trace_entity> m_trace_entity;
  Dxf_record_wrapper<Dxf_vertex_entity> m_vertex_entity;
  Dxf_record_wrapper<Dxf_viewport_entity> m_viewport_entity;
  Dxf_record_wrapper<Dxf_wipeout_entity> m_wipeout_entity;
  Dxf_record_wrapper<Dxf_xline_entity> m_xline_entity;

  // Objects
  Dxf_record_wrapper<Dxf_acad_proxy_object> m_acad_proxy_object;
  Dxf_record_wrapper<Dxf_acdbdictionarywdflt_object> m_acdbdictionarywdflt_object;
  Dxf_record_wrapper<Dxf_acdbplaceholder_object> m_acdbplaceholder_object;
  Dxf_record_wrapper<Dxf_acdbnavisworksmodeldef_object> m_acdbnavisworksmodeldef_object;
  Dxf_record_wrapper<Dxf_datatable_object> m_datatable_object;
  Dxf_record_wrapper<Dxf_dictionary_object> m_dictionary_object;
  Dxf_record_wrapper<Dxf_dictionaryvar_object> m_dictionaryvar_object;
  Dxf_record_wrapper<Dxf_dimassoc_object> m_dimassoc_object;
  Dxf_record_wrapper<Dxf_field_object> m_field_object;
  Dxf_record_wrapper<Dxf_geodata_object> m_geodata_object;
  Dxf_record_wrapper<Dxf_group_object> m_group_object;
  Dxf_record_wrapper<Dxf_idbuffer_object> m_idbuffer_object;
  Dxf_record_wrapper<Dxf_imagedef_object> m_imagedef_object;
  Dxf_record_wrapper<Dxf_imagedef_reactor_object> m_imagedef_reactor_object;
  Dxf_record_wrapper<Dxf_layer_index_object> m_layer_index_object;
  Dxf_record_wrapper<Dxf_layer_filter_object> m_layer_filter_object;
  Dxf_record_wrapper<Dxf_layout_object> m_layout_object;
  Dxf_record_wrapper<Dxf_lightlist_object> m_lightlist_object;
  Dxf_record_wrapper<Dxf_material_object> m_material_object;
  Dxf_record_wrapper<Dxf_mlinestyle_object> m_mlinestyle_object;
  Dxf_record_wrapper<Dxf_object_ptr_object> m_object_ptr_object;
  Dxf_record_wrapper<Dxf_plotsettings_object> m_plotsettings_object;
  Dxf_record_wrapper<Dxf_rastervariables_object> m_rastervariables_object;
  Dxf_record_wrapper<Dxf_render_object> m_render_object;
  Dxf_record_wrapper<Dxf_section_object> m_section_object;
  Dxf_record_wrapper<Dxf_spatial_index_object> m_spatial_index_object;
  Dxf_record_wrapper<Dxf_spatial_filter_object> m_spatial_filter_object;
  Dxf_record_wrapper<Dxf_sortentstable_object> m_sortentstable_object;
  Dxf_record_wrapper<Dxf_sunstudy_object> m_sunstudy_object;
  Dxf_record_wrapper<Dxf_tablestyle_object> m_tablestyle_object;
  Dxf_record_wrapper<Dxf_underlaydefinition_object> m_underlaydefinition_object;
  Dxf_record_wrapper<Dxf_visualstyle_object> m_visualstyle_object;
  Dxf_record_wrapper<Dxf_vba_project_object> m_vba_project_object;
  Dxf_record_wrapper<Dxf_wipeoutvariables_object> m_wipeoutvariables_object;
  Dxf_record_wrapper<Dxf_xrecord_object> m_xrecord_object;

public:
  // Dummy object is used to parse unrecognized objects.
  struct Dxf_dummy_object : public Dxf_base_object {
    static constexpr bool has_read_value = false;
    typedef Dxf_base_object                     Base;
  };
  Dxf_record_wrapper<Dxf_dummy_object> m_dummy_object;

private:
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
  Code_type read_verify_code(int code);

  /*! Import a datum item.
   */
  template <typename Record, typename MemberType, typename Target>
  void read_record_value(Code_type ct, int size, MemberType handle,
                         Target& target, int index)
  {
    typedef typename Record::String_record      String_record;
    typedef typename Record::Bool_record        Bool_record;
    typedef typename Record::Int8_record        Int8_record;
    typedef typename Record::Int16_record       Int16_record;
    typedef typename Record::Int32_record       Int32_record;
    typedef typename Record::Double_record      Double_record;
    typedef typename Record::Uint_record        Uint_record;
    typedef typename Record::Double_2d_record   Double_2d_record;
    typedef typename Record::Double_3d_record   Double_3d_record;

    switch (ct) {
     case STRING: import_string_member<String_record>(handle, target); break;
     case BOOL: import_member<Bool_record>(handle, target); break;
     case INT8: import_int8_member<Int8_record>(handle, target); break;
     case INT16: import_member<Int16_record>(handle, target); break;
     case INT32: import_member<Int32_record>(handle, target); break;
     case UINT: import_uint_member<Uint_record>(handle, target); break;

     case DOUBLE:
      switch (size) {
       case 1: import_member<Double_record>(handle, target); break;
       case 2: import_member<Double_2d_record>(handle, target, index); break;
       case 3: import_member<Double_3d_record>(handle, target, index); break;
      }
      break;

     default: SGAL_error();
    }
  }

  /*! Import a code.
   * \param[code] the code variable.
   */
  template <typename T>
  void import_code(T& code)
  {
    std::string tmp;
    m_is >> code;
    m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::import_code(): "
                    << code << std::endl;);
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

  /*! Import a string value.
   * \param[handle] handle the handle to the string member.
   * \param[target] target the target struct.
   */
  template <typename T>
  void import_string_value(T& variable)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline consumes
    // the endline character left on the input stream by operator>>, and returns
    // immediately. Ignore all leftover characters.
    std::getline(m_is, variable);
    variable.erase(variable.find_last_not_of(" \t\n\r\f\v") + 1);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout
                    << "Dxf_parser::import_string_member() value: "
                    << variable << std::endl;);
  }

  /*! Import a string value to a string header member.
   * \param[handle] handle the handle to the string member.
   * \param[target] target the target struct.
   */
  template <typename T, typename MemberType, typename Target>
  void import_string_member(MemberType handle, Target& target)
  { import_string_value(target.*(boost::get<T>(handle))); }

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

  /*! Parse common table section, which aplies to all table types.
   * \return maximum number of entries in table.
   */
  int parse_base_table(Dxf_base_table& table);

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
  void store_xdata(Record& record, const String& name,
                           const String& value, char (*)[what] = 0)
  { record.m_xdata[name].push_back(value); }

  /*! Store extended data.
   */
  template <bool what, typename Record>
  void store_xdata(Record& record, const String& name,
                           const String& value, char (*)[!what] = 0)
  {}

  /*! Read extended data
   */
  template <typename Record>
  void read_xdata_block(int code, Record& record)
  {
    char c;
    m_is >> c;
    if ('{' != c) {
      //! \todo when is this valid and what should we do here?
      String str;
      import_string_value(str);
      return;
    }

    String name;
    if (102 == code) import_string_value(name);
    else m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    String str;
    import_string_value(str);
    while ("}" != str) {
      import_string_value(str);
      store_xdata<Has_xdata<Record>::value>(record, name, str);
    }
  }

  /*! Parse a table.
   */
  template <typename Table>
  void parse_table(Table& table, const std::string& name)
  {
#if 0
    typedef typename Table::String_entry        String_entry;
    typedef typename Table::Bool_entry          Bool_entry;
    typedef typename Table::Int8_entry          Int8_entry;
    typedef typename Table::Int16_entry         Int16_entry;
    typedef typename Table::Int32_entry         Int32_entry;
    typedef typename Table::Double_entry        Double_entry;
    typedef typename Table::Uint_entry          Uint_entry;
    typedef typename Table::Double_2d_entry     Double_2d_entry;
    typedef typename Table::Double_3d_entry     Double_3d_entry;
#endif

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
        import_code(code);
        SGAL_TRACE_CODE(Trace::DXF,
                        std::cout << "Dxf_parser::parse_table() code: "
                        << code << std::endl;);
        if (0 == code) {
          done = true;
          break;
        }

        if (100 == code) {
          import_string_value(m_marker);
          continue;
        }

        if ((102 == code) || (1002 == code)) {
          read_xdata_block(code, entry);
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
          read_record_value<Table>(ct, size, handle, entry, index);
          continue;
        }

        read_unrecognized(code);
      }
    }
  }

  /*! Read a comment line.
   */
  void read_comment();

  /*! Handle an unrecognized code.
   */
  void read_unrecognized(int code);

  static const std::map<String, Section_parser> s_sections;
  static const std::map<String, Header_member> s_header_members;
  static const std::vector<Code_range> s_code_ranges;
  static const std::array<String, 8> s_code_type_names;
  static const std::map<int, Class_member_type> s_class_members;
  static const std::map<String, Table_parser> s_tables;
  static const std::map<int, Base_table_member_type> s_base_table_members;
  static const std::map<int, Base_entry_member_type> s_base_entry_members;
  static const std::map<int, Block_member> s_block_members;
  static const std::map<String, Entity_parser> s_entities;
  static const std::map<int, Base_entity_type> s_base_entity_members;
  static const std::map<String, Object_parser> s_objects;
  static const std::map<int, Base_object_type> s_base_object_members;
};

SGAL_END_NAMESPACE

#endif
