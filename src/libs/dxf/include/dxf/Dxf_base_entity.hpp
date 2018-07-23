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

#ifndef DXF_BASE_ENTITY_HPP
#define DXF_BASE_ENTITY_HPP

#include <string>
#include <vector>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/By.hpp"
#include "dxf/Dxf_extended_data.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_base_entity {

  /// Member records
  //@{

  SGAL::String m_handle; // Handle
  SGAL::String m_owner_handle; // Soft pointer ID/handle to owner dictionary
                        // (optional)
  SGAL::String m_owner_dict; // Hard owner ID/handle to owner dictionary
                        // (optional)
  // SGAL::String m_owner_handle; // Soft-pointer ID/handle to owner
                        // BLOCK_RECORD object
  int16_t m_is_in_paper_space;  // Absent/0 = entity is in model space.
                        // 1 = entity is in paper space (optional). 0
  SGAL::String m_layer; // Layer name
  SGAL::String m_line_type_name; // Linetype name (present if not BYLAYER).
                        // The special name BYBLOCK indicates a floating
                        // linetype (optional). BYLAYER
  int16_t m_color_index;// Color number (present if not BYLAYER);
                        // 0 = the BYBLOCK (floating) color;
                        // 256 = BYLAYER;
                        // a negative value indicates that the layer is turned
                        // off (optional). BYLAYER
  int8_t m_lineweight_enum_value; // Lineweight enum value. Stored and moved
                        // around as a 16-bit integer.
  SGAL::String m_layout_tab_name; // APP: layout tab name
  double m_line_type_scale; // Linetype scale (optional) 1.0
  int16_t m_is_visible; // Object visibility (optional):
                        // 0 = Visible, 1 = Invisible. 0
  int32_t m_image_byte_count; // The number of bytes in the image (and
                        // subsequent binary chunk records) (optional)
  std::vector<SGAL::String> m_preview_image_data; // Preview image data
                        // (multiple lines; 256 charaters max. per line)
                        // (optional)
  int32_t m_color;      // A 24-bit color value that should be dealt with in
                        // terms of bytes with values of 0 to 255. The lowest
                        // byte is the blue value, the middle byte is the green
                        // value, and the third byte is the red value. The top
                        // byte is always 0. The group code cannot be used by
                        // custom entities for their own data because the group
                        // code is reserved for AcDbEntity, class-level color
                        // data and AcDbEntity, class-level transparency data
  SGAL::String m_color_name; // Color name. The group code cannot be used by
                        // custom entities for their own data because the group
                        // code is reserved for AcDbEntity, class-level color
                        // ata data and AcDbEntity, class-level transparency data
  int32_t m_transparency; // Transparency value. The group code cannot be used
                        // by custom entities for their own data because the
                        // group code is reserved for AcDbEntity, class-level
                        // color data and AcDbEntity, class-level transparency
                        // data
  SGAL::String m_plot_style; // Hard-pointer ID/handle to the plot style object
  int8_t m_shadow_mode; // Shadow mode
                        // 0 = Casts and receives shadows
                        // 1 = Casts shadows
                        // 2 = Receives shadows
                        // 3 = Ignores shadows
                        // Note: Starting with AutoCAD 2016-based products, this
                        // property is obsolete but still supported for
                        // backwards compatibility.
  std::map<SGAL::String, std::vector<SGAL::String> > m_xdata; // Begin xdata
                        // "{", "}" (opt.)
  std::vector<Dxf_extended_data> m_extended_data;

  //@}

  /*! Construct.
   * Initialize the color with -1 to indicate that it hasn't been set.
   */
  Dxf_base_entity() :
    m_is_in_paper_space(0),
    m_color_index(static_cast<int16_t>(By::BYLAYER)),
    m_line_type_scale(1.0),
    m_color(static_cast<int32_t>(-1)),
    m_parser(nullptr)
  {}

  /*! Set the parser.
   * \param[in] parser the parser.
   */
  void set_parser(Dxf_parser* parser) { m_parser = parser; }

  /*! Initialize */
  void init() {}

  //! The parser.
  Dxf_parser* m_parser;
};

DXF_END_NAMESPACE

#endif
