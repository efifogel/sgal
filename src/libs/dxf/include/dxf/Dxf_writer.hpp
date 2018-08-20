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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef DXF_WRITER_HPP
#define DXF_WRITER_HPP

#include <string>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Base_writer.hpp"

#include "dxf/basic.hpp"
#include "dxf/Code_type.hpp"
#include "dxf/Dxf_exporter.hpp"
#include "dxf/Dxf_dimstyle_entry.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_data;
struct Dxf_base_table;
struct Dxf_base_entry;
struct Dxf_block;
struct Dxf_endblk;
struct Dxf_base_entity;
struct Dxf_base_object;

// Table entry types
struct Dxf_appid_entry;
struct Dxf_block_record_entry;
struct Dxf_dimstyle_entry;
struct Dxf_layer_entry;
struct Dxf_ltype_entry;
struct Dxf_style_entry;
struct Dxf_ucs_entry;
struct Dxf_view_entry;
struct Dxf_vport_entry;

// Entity types
struct Dxf_line_entity;

// Object types
struct Dxf_dictionary_object;

class SGAL_SGAL_DECL Dxf_writer : public SGAL::Base_writer {
public:
  /*! Construct
   */
  Dxf_writer();

  /*! Destruct.
   */
  virtual ~Dxf_writer();

  /*! Write the scene to a file.
   * \param[in] scene_graph the scene graph that represents the scene.
   * \param[in] os the output stream.
   * \param[in] filename the output filename.
   */
  virtual void operator()(SGAL::Scene_graph* scene_graph,
                          std::ostream& os, const std::string& filename,
                          bool is_binary = false);

  /*! Export a section
   */
  template <typename Function>
  void export_section(Function fnc);

  /*! Write the HEADER section.
   */
  void export_header(Dxf_data* data);

  /*! Write the CLASSES section.
   */
  void export_classes(Dxf_data* data);

  /*! Write the TABLES section.
   */
  void export_tables(Dxf_data* data);

  /*! Write the BLOCKS section.
   */
  void export_blocks(Dxf_data* data);

  /*! Write the ENTITIES section.
   */
  void export_entities(Dxf_data* data);

  /*! Write the OBJECTS section.
   */
  void export_objects(Dxf_data* data);

  /*! Write the THUMBNAILIMAGE section.
   */
  void export_thumbnailimage(Dxf_data* data);

  /*! Write the ACDSDATA section.
   */
  void export_acdsdata(Dxf_data* data);

  /*! Export a given table.
   */
  template <typename Table>
  void export_table(const Table& table, const std::string& name);

  /*! Export a given base_table.
   */
  void export_base_table(const Dxf_base_table& base_table);

  /*! Export a generic entry.
   */
  template <typename Entry>
  void export_entry(const Entry& entry, const std::string& name);

  /*! Export a DIMSTYLE entry.
   */
  void export_entry(const Dxf_dimstyle_entry& entry, const std::string& name);

  /*! Export a given base entry.
   */
  void export_base_entry(const Dxf_base_entry& base_entry);

  /*! Export a given DIMSTYLE base entry.
   */
  void export_dimstyle_base_entry(const Dxf_base_entry& base_entry);

  /*! Export a given entry.
   */
  void export_entry(const Dxf_appid_entry& entry);
  void export_entry(const Dxf_block_record_entry& entry);
  void export_entry(const Dxf_dimstyle_entry& entry);
  void export_entry(const Dxf_layer_entry& entry);
  void export_entry(const Dxf_ltype_entry& entry);
  void export_entry(const Dxf_style_entry& entry);
  void export_entry(const Dxf_ucs_entry& entry);
  void export_entry(const Dxf_view_entry& entry);
  void export_entry(const Dxf_vport_entry& entry);

  /*! Export a BLOCK record.
   */
  void export_block(const Dxf_block& block);

  /*! Export an ENDBLK record.
   */
  void export_endblk(const Dxf_endblk& block);

  /// Entity exporters
  //@{

  /*! Export a base entity record.
   */
  void export_base_entity(const Dxf_base_entity& base_entity);

  /*! Export a LINE entity record.
   */
  void export_entity(const Dxf_line_entity& entity);

  //@}

  /// Object exporters
  //@{

  /*! Export a base object record.
   */
  void export_base_object(const Dxf_base_object& base_object);

  /*! Export a DICTIONARY object record.
   */
  void export_object(const Dxf_dictionary_object& object);

  //@}

  /// Exporters
  //@{
  /*! Export a given code.
   */
  void export_code(int code);

  /*! Export a given code.
   */
  template <typename T>
  void export_value(const T& value)   { out() << value << std::endl; }

  /*! Export an item if not equal to a given default value.
   */
  template <typename T>
  void export_item(int code, const T& value, const T& default_value);

  /*! Export an item.
   */
  template <typename T>
  void export_item(int code, const T& value);

  /*! Export a string item.
   */
  void export_string(int code, const std::string& str);

  /*! Export a string item if the string is not empty.
   */
  void export_nonempty_string(int code, const std::string& str);

  /*! Export an item.
   * \param[in] code the code to export.
   * \param[in] record the source record.
   * \param[in] handle the handle to the struct member.
   */
  template <typename T, typename Record, typename MemberVariant>
  void export_member(int code, const Record& record, MemberVariant handle);

  /*! Export an element of an array.
   * \param[in] code the code to export.
   * \param[in] record the source struct.
   * \param[in] handle the handle to the member.
   * \param[in] index the index of the array element.
   */
  template <typename T, typename Record, typename MemberVariant>
  void export_member(int code, const Record& record, MemberVariant handle,
                     int index);

  /*! Export an item that is a member of a record.
   */
  template <typename Record, typename Members>
  void export_member(int code, const Record& record, Members& members);

  /*! Export a non-empty item .that is a member of a record.
   * \param[in] code the code to export.
   * \param[in] record the source record.
   * \param[in] handle the handle to the struct member.
   */
  template <typename T, typename Record, typename MemberVariant>
  void export_nonempty_member(int code, const Record& record,
                              MemberVariant handle);

  /*! Export a non-empty item that is a member of a record.
   * \pre code_type(code) must be a handle to SGAL::String.
   */
  template <typename Record, typename Members>
  void export_nonempty_member(int code, const Record& record, Members& members);

  /*! Export an item that is a member of a simple record.
   */
  template <typename Record, typename Members>
  void export_simple_member(int code, const Record& record, Members& members);

  /*! Export a non-empty item that is a member of a simple record.
   * \pre code_type(code) must be a handle to SGAL::String.
   */
  template <typename Record, typename Members>
  void export_nonempty_simple_member(int code, const Record& record,
                                     Members& members);
  //@}

 protected:
  /*! Initialize a DXF data structure with the minimal requirements.
   */
  void init(Dxf_data* data);

  /*! Add the scene to a DXF data structure.
   */
  void update(Dxf_data* data);

  /*! Export a DXF data structure.
   */
  void export_data(Dxf_data* data);

  //! The scene to export.
  SGAL::Scene_graph* m_scene_graph;

  //! A dxf data to export.
  Dxf_data* m_data;

  //! Indicates whether the dxf data is owned, and thus should be deallocated.
  bool m_owned;
};

//! \brief export an item if not equal to a given default value.
template <typename T>
void Dxf_writer::export_item(int code, const T& value, const T& default_value)
{
  if (value == default_value) return;
  export_item(code, value);
}

//! \brief export an item.
template <typename T>
void Dxf_writer::export_item(int code, const T& value)
{
  export_code(code);
  Dxf_exporter<T> exporter(*this);
  exporter(value);
}

//! \brief exports an item.
template <typename T, typename Record, typename MemberVariant>
void Dxf_writer::export_member(int code, const Record& record,
                               MemberVariant handle)
{ export_item(code, record.*(boost::get<T>(handle))); }

//! \brief exports an element of an array.
template <typename T, typename Record, typename MemberVariant>
void Dxf_writer::export_member(int code, const Record& record,
                               MemberVariant handle, int index)
{ export_item(code, (record.*(boost::get<T>(handle)))[index]); }

//! \brief exports an item.
template <typename Record_, typename Members>
void Dxf_writer::export_member(int code, const Record_& record, Members& members)
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

  auto it = members.find(code);
  SGAL_assertion(it != members.end());
  auto ct = code_type(code);

  auto handle = it->second.m_handle;
  auto size = it->second.m_size;
  auto index = it->second.m_index;

  switch (ct) {
   case Code_type::STRING: export_member<String_record>(code, record, handle); break;
   case Code_type::BOOL: export_member<Bool_record>(code, record, handle); break;
   case Code_type::INT8: export_member<Int8_record>(code, record, handle); break;
   case Code_type::INT16: export_member<Int16_record>(code, record, handle); break;
   case Code_type::INT32: export_member<Int32_record>(code, record, handle); break;
   case Code_type::UINT: export_member<Uint_record>(code, record, handle); break;

   case Code_type::DOUBLE:
    switch (size) {
     case 1: export_member<Double_record>(code, record, handle); break;
     case 2: export_member<Double_2d_record>(code, record, handle, index); break;
     case 3: export_member<Double_3d_record>(code, record, handle, index); break;
    }
    break;

   default: SGAL_error();
  }
}

//! \brief export an item that is a member of a simple record.
template <typename Record, typename Members>
void Dxf_writer::export_simple_member(int code, const Record& record,
                                      Members& members)
{
  typedef SGAL::String Record::*              String_record;
  typedef bool Record::*                      Bool_record;
  typedef int8_t Record::*                    Int8_record;
  typedef int16_t Record::*                   Int16_record;
  typedef int32_t Record::*                   Int32_record;
  typedef double Record::*                    Double_record;
  typedef SGAL::Uint Record::*                Uint_record;

  auto it = members.find(code);
  SGAL_assertion(it != members.end());
  auto ct = code_type(code);

  auto handle = it->second;

  switch (ct) {
   case Code_type::STRING: export_member<String_record>(code, record, handle); break;
   case Code_type::BOOL: export_member<Bool_record>(code, record, handle); break;
   case Code_type::INT8: export_member<Int8_record>(code, record, handle); break;
   case Code_type::INT16: export_member<Int16_record>(code, record, handle); break;
   case Code_type::INT32: export_member<Int32_record>(code, record, handle); break;
   case Code_type::UINT: export_member<Uint_record>(code, record, handle); break;

   case Code_type::DOUBLE: export_member<Double_record>(code, record, handle); break;

   default: SGAL_error();
  }
}

//! \brief exports a non-empty item.
template <typename T, typename Record, typename MemberVariant>
void Dxf_writer::export_nonempty_member(int code, const Record& record,
                                        MemberVariant handle)
{ export_nonempty_string(code, record.*(boost::get<T>(handle))); }

//! \brief exports a non-empty item.
template <typename Record, typename Members>
void Dxf_writer::export_nonempty_member(int code, const Record& record,
                                        Members& members)
{
  auto it = members.find(code);
  SGAL_assertion(it != members.end());
  SGAL_assertion_code(auto ct = code_type(code));
  SGAL_assertion(ct == Code_type::STRING);
  auto handle = it->second.m_handle;
  export_nonempty_member<SGAL::String Record::*>(code, record, handle);
}

//! \brief exports a non-empty item that is a member of a simple record.
template <typename Record, typename Members>
void Dxf_writer::export_nonempty_simple_member(int code, const Record& record,
                                               Members& members)
{
  auto it = members.find(code);
  SGAL_assertion(it != members.end());
  SGAL_assertion_code(auto ct = code_type(code));
  SGAL_assertion(ct == Code_type::STRING);
  auto handle = it->second;
  export_nonempty_member<SGAL::String Record::*>(code, record, handle);
}

//! \brief exports an object.
template <typename Function>
inline void Dxf_writer::export_section(Function fnc)
{
  export_string(0, "SECTION");
  fnc();
  export_string(0, "ENDSEC");
}

/*! Export a given table.
 */
template <typename Table>
void Dxf_writer::export_table(const Table& table, const std::string& name)
{
  export_string(0, "TABLE");
  export_string(2, name);
  export_base_table(table);
  export_item(70, table.m_entries.size());
  for (auto& entry : table.m_entries) export_entry(entry, name);
  export_string(0, "ENDTAB");
}

//! \brief export a generic entry.
template <typename Entry>
void Dxf_writer::export_entry(const Entry& entry, const std::string& name)
{

  export_string(0, name);
  export_base_entry(entry);
  export_entry(entry);
}

DXF_END_NAMESPACE

#endif
