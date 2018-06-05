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

#ifndef SGAL_DXF_BLOCK_RECORD_ENTRY_HPP
#define SGAL_DXF_BLOCK_RECORD_ENTRY_HPP

#include <string>
#include <list>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_table_entry.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_block_record_entry : Dxf_table_entry {
  /// Record members
  //@{

  String m_name;        // Block name
  Uint m_layout_handle; // Hard-pointer ID/handle to associated LAYOUT object
  int16_t m_insertion_units; // Block insertion units.
  int8_t m_explodability; // Block explodability
  int8_t m_scalability; // Block scalability
  String m_bitmap_preview_data; // Binary data for bitmap preview (optional)
  int16_t m_design_center_version_number; // Autodesk Design Center version
                        // number
  int16_t m_insert_units; // Insert units:
                        // 0 = Unitless
                        // 1 = Inches
                        // 2 = Feet
                        // 3 = Miles
                        // 4 = Millimeters
                        // 5 = Centimeters
                        // 6 = Meters
                        // 7 = Kilometers
                        // 8 = Microinches
                        // 9 = Mils
                        // 10 = Yards
                        // 11 = Angstroms
                        // 12 = Nanometers
                        // 13 = Microns
                        // 14 = Decimeters
                        // 15 = Decameters
                        // 16 = Hectometers
                        // 17 = Gigameters
                        // 18 = Astronomical units
                        // 19 = Light years
                        // 20 = Parsecs
                        // 21 = US Survey Feet
                        // 22 = US Survey Inch
                        // 23 = US Survey Yard
                        // 24 = US Survey Mile
  String m_application_name; // Xdata application name "ACAD" (optional)
  String m_string_data; // Xdata string data "DesignCenter Data" (optional)
  std::map<String, std::list<String> > m_xdata; // Begin xdata "{", "}" (opt.)

  //@}

  /// Record handles
  //@{

  /*! Handle code 1070.
   */
  void handle_1070(int16_t value);

  //@}

  //! The number of times code 1070 was read.
  size_t m_1070_num;

  /*! Construct */
  Dxf_block_record_entry() : m_1070_num(0) {}

  /*! Initialize. */
  void init();
};

SGAL_END_NAMESPACE

#endif
