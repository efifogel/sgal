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

#include "SGAL/basic.hpp"
#include "SGAL/Base_writer.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_data;
class Dxf_base_table;
class Dxf_table_entry;
class Dxf_appid_entry;
class Dxf_block_record_entry;
class Dxf_dimstyle_entry;
class Dxf_layer_entry;
class Dxf_ltype_entry;
class Dxf_style_entry;
class Dxf_ucs_entry;
class Dxf_view_entry;
class Dxf_vport_entry;

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
  void export_header();

  /*! Write the CLASSES section.
   */
  void export_classes();

  /*! Write the TABLES section.
   */
  void export_tables();

  /*! Write the BLOCKS section.
   */
  void export_blocks();

  /*! Write the ENTITIES section.
   */
  void export_entities();

  /*! Write the OBJECTS section.
   */
  void export_objects();

  /*! Write the THUMBNAILIMAGE section.
   */
  void export_thumbnailimage();

  /*! Write the ACDSDATA section.
   */
  void export_acdsdata();

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

  /*! Export a given entry.
   */
  void export_base_entry(const Dxf_table_entry& base_entry);

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
  //@}

 protected:
  /*! Initialize with the minimal requirements.
   */
  void init();

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
  export_value(value);
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
