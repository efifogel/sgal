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

#ifndef SGAL_DXF_MLINESTYLE_OBJECT_HPP
#define SGAL_DXF_MLINESTYLE_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_mlinestyle_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  /// Record members
  //@{

  String m_style_name;  // Mline style name
  int16_t m_flags;      // Flags (bit-coded):
                        // 1 = Fill on
                        // 2 = Display miters
                        // 16 = Start square end (line) cap
                        // 32 = Start inner arcs cap
                        // 64 = Start round (outer arcs) cap
                        // 256 = End square (line) cap
                        // 512 = End inner arcs cap
                        // 1024 = End round (outer arcs) cap
  String m_style_description; // Style description (string, 255 characters max)
  int16_t m_fill_color; //  Fill color (integer, default = 256)
  double m_start_angle; // Start angle (real, default is 90 degrees)
  double m_end_angle;   // End angle (real, default is 90 degrees)
  // 71, Number of elements
  std::vector<double> m_element_offsets; // Element offset (real, no default).
                        // Multiple entries can exist; one entry for each
  std::vector<int16_t> m_element_color; // Element color (integer, default = 0).
                        // Multiple entries can exist; one entry for each
  std::vector<String> m_element_line_style; // Element linetype (string,
                        // default = BYLAYER). Multiple entries can exist;
                        // one entry for each element

  //@}

  /// Record handles
  //@{

  /*! Handle number of elements.
   */
  void handle_elements_num(int16_t size)
  {
    m_element_offsets.reserve(size);
    m_element_color.reserve(size);
    m_element_line_style.reserve(size);
  }

  /*! Handle element offset.
   */
  void handle_element_offset(double offset)
  {
    SGAL_assertion(! m_element_offsets.empty());
    m_element_offsets.back() = offset;
  }

  /*! Handle element color.
   */
  void handle_element_color(int16_t color)
  {
    SGAL_assertion(! m_element_color.empty());
    m_element_color.back() = color;
  }

  /*! Handle element line-style.
   */
  void handle_element_line_style(const String& style)
  {
    SGAL_assertion(! m_element_line_style.empty());
    m_element_line_style.back() = style;
  }

  //@}
};

SGAL_END_NAMESPACE

#endif
