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

#include <vector>

#ifndef SGAL_DXF_DIMSTYLE_ENTRY_HPP
#define SGAL_DXF_DIMSTYLE_ENTRY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_table_entry.hpp"
#include "SGAL/Dxf_extended_data.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_dimstyle_entry: public Dxf_table_entry {
  String m_name;        // Dimension style name
  int16_t m_flags;      // Standard flag values (bit-coded values):
                        // 16 = If set, table entry is externally dependent on
                        //      an xref
                        // 32 = If both this bit and bit 16 are set, the
                        //      externally dependent xref has been successfully
                        //      resolved
                        // 64 = If set, the table entry was referenced by at
                        //      least one entity in the drawing the last time
                        //      the drawing was edited. (This flag is for the
                        //      benefit of AutoCAD commands. It can be ignored
                        //      by most programs that read DXF files and need
                        //      not be set by programs that write DXF files)
  String m_dimpost;     // DIMPOST
  String m_dimapost;    // DIMAPOST
  String m_object_id;   //  DIMBLK (obsolete, now object ID)
  String m_object_id1;  //  DIMBLK1 (obsolete, now object ID)
  String m_object_id2;  //  DIMBLK2 (obsolete, now object ID)
  double m_dimensioning_scale_factor; // DIMSCALE
  double m_dimensioning_arrow_size; // DIMASZ
  double m_dimension_extension_line_offset; // DIMEXO
  double m_dimension_line_increment; // DIMDLI
  double m_dimension_extension_line_extension; // DIMEXE
  double m_dimension_distance_rounding_value; // DIMRND
  double m_dimension_line_extension; // DIMDLE
  double m_dimension_plus_tolerance; // DIMTP
  double m_dimension_minus_tolerance; // DIMTM
  double m_dimensioning_text_height; // DIMTXT
  double m_center_mark_size; // DIMCEN
  double m_dimensioning_tick_size; // DIMTSZ
  double m_alternate_dimensioning_scale_factor; // DIMALTF
  double m_dimension_linear_measurement_scale_factor; // DIMLFAC
  double m_dimension_vertical_text_position; // DIMTVP
  double m_dimension_unit_tolerance_decimal_places; // DIMTFAC
  double m_dimension_line_gap; // DIMGAP
  double m_alternate_dimensioning_decimal_places; // DIMALTRND
  int16_t m_generate_dimension_tolerances; // DIMTOL
  int16_t m_generate_dimension_limits; // DIMLIM
  int16_t m_dimension_text_inside_horizontal; // DIMTIH
  int16_t m_dimension_text_outside_horizontal; // DIMTOH
  int16_t m_suppress_first_dimension_extension_line; // DIMSE1
  int16_t m_suppress_second_dimension_extension_line; // DIMSE2
  int16_t m_text_above_dimension_line; // DIMTAD
  int16_t m_dimension_unit_zero_suppression; // DIMZIN
  int16_t m_dimension_angle_zero_suppression; // DIMAZIN
  int16_t m_alternate_dimensioning_unit_rounding; // DIMALT
  int16_t m_use_alternate_dimensioning; // DIMALTD
  int16_t m_dimension_tolerance_displace_scale_factor; // DIMTOFL
  int16_t m_use_separate_arrow_blocks_for_dimensions; // DIMSAH
  int16_t m_force_dimension_text_inside_extensions; // DIMTIX
  int16_t m_suppress_outside_extension_dimension_lines; // DIMSOXD
  int16_t m_dimension_line_color; // DIMCLRD
  int16_t m_dimension_extension_line_color; // DIMCLRE
  int16_t m_dimension_text_color; // DIMCLRT
  int16_t m_angular_dimension_precision; // DIMADEC
  int16_t m_dimension_unit_format; // DIMUNIT (obsolete, now use DIMLUNIT AND DIMFRAC)
  int16_t m_force_dimension_line_extensions_outside_if_text_exists; // DIMDEC
  int16_t m_dimension_tolerace_decimal_places; // DIMTDEC
  int16_t m_alternate_dimensioning_units; // DIMALTU
  int16_t m_alternate_dimensioning_tolerance_decimal_places; // DIMALTTD
  int16_t m_dimensioning_angle_format; // DIMAUNIT
  int16_t m_dimension_precision; // DIMFRAC
  int16_t m_dimension_non_angular_units; // DIMLUNIT
  int16_t m_dimension_decilam_separator_char; //DIMDSEP
  int16_t m_dimension_text_movement_rule; // DIMTMOVE
  int8_t m_dimension_text_justification; // DIMJUST
  // int8_t DIMSD1
  // int8_t DIMSD2
  int8_t m_dimension_tolerance_vertical_justification; // DIMTOLJ
  int8_t m_dimension_tolerance_zero_suppression; // DIMTZIN
  int8_t m_alternate_dimensioning_zero_suppression; // DIMALTZ
  int8_t m_alternate_dimensioning_tolerance_zero_suppression; // DIMALTTZ
  // int8_t DIMFIT (obsolete; now use DIMATFIT and DIMTMOVE)
  int8_t m_dimension_cursor_controls_text_position; // DIMUPT
  int8_t m_dimension_text_and_arrow_placement; // DIMATFIT
  Uint m_dimension_text_style; // DIMTXSTY (handle of referenced STYLE)
  Uint m_dimension_leader_block_name; // DIMLDRBLK (handle of referenced BLOCK)
  Uint m_arrow_block_name; // DIMBLK (handle of referenced BLOCK)
  // Uint DIMBLK1 (handle of referenced BLOCK)
  // Uint DIMBLK2 (handle of referenced BLOCK)
  int8_t m_dimension_line_weight; // DIMLWD (lineweight enum value)
  int8_t m_dimension_extension_line_weight; // DIMLWE (lineweight enum value)

  std::vector<Dxf_extended_data> m_extended_data;
};

SGAL_END_NAMESPACE

#endif
