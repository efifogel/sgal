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

#ifndef SGAL_DXF_STYLE_ENTRY_HPP
#define SGAL_DXF_STYLE_ENTRY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_table_entry.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_style_entry : Dxf_table_entry {
  String m_name;        // Style name
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
  double m_text_height; // Fixed text height; 0 if not fixed
  double m_width_factor; // Width factor
  double m_oblique_angle; // Oblique angle
  int16_t m_text_generation_flags;// Text generation flags:
                        // 2 = Text is backward (mirrored in X)
                        // 4 = Text is upside down (mirrored in Y)
  double m_last_height_used; // Last height used
  String m_primary_font_file_name; // Primary font file name
  String m_big_font_file_name; // Bigfont file name; blank if none
  int32_t m_font_flags; // A truetype font's pitch and family, charset, and
                        // italic and bold flags
};

SGAL_END_NAMESPACE

#endif
