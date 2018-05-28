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

#ifndef SGAL_DXF_LTYPE_ENTRY_HPP
#define SGAL_DXF_LTYPE_ENTRY_HPP

#include <array>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_table_entry.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_ltype_entry: public Dxf_table_entry {
  String m_name;        // Viewport name
  int16_t m_flags;      // 16 = If set, table entry is externally dependent on
                        //      an xref
                        // 32 = If both this bit and bit 16 are set, the
                        //      externally dependent xref has been successfully
                        // resolved
                        // 64 = If set, the table entry was referenced by at
                        //      least one entity in the drawing the last time
                        //      the drawing was edited. (This flag is for the
                        //      benefit of AutoCAD commands. It can be ignored
                        //      by most programs that read DXF files and does
                        //      not need to be set by programs that write DXF
                        //      files)
  String m_description; // Descriptive text for linetype
  int16_t m_alignment_code; // Alignment code; value is always 65, the ASCII
                        // code for A
  int16_t m_element_count; // The number of linetype elements
  double m_total_pattern_length; // Total pattern length
  double m_dash_dot_space_lengths; // Dash, dot or space length (one entry per
                        // element)
  int16_t m_complex_line_type_element_types; // Complex linetype element type
                        // (one per element). Default is 0 (no embedded
                        // shape/text). The following codes are bit values:
                        // 1 = If set, code 50 specifies an absolute rotation;
                        //     if not set, code 50 specifies a relative rotation
                        // 2 = Embedded element is a text string
                        // 4 = Embedded element is a shape
  int16_t m_shape_numbers; // Shape number (one per element) if code 74
                        // specifies an embedded shape
                        // If code 74 specifies an embedded text string, this
                        // value is set to 0
                        // If code 74 is set to 0, code 75 is omitted
  Uint m_style_pointers; // Pointer to STYLE object (one per element if code
                        // 74 > 0)
  std::vector<double> m_scale_values; // S = Scale value (optional)
  double m_rotation_angles; // R = (relative) or A = (absolute) rotation value
                        // in radians of embedded shape or text; one per element
                        // if code 74 specifies an embedded shape or text string
  std::vector<std::array<double, 2> > m_offsets; // offset values (optional)
  std::vector<String> m_text_strings; // Text string (one per element if code
                        // 74 = 2)

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(int code, double value);
};

SGAL_END_NAMESPACE

#endif
