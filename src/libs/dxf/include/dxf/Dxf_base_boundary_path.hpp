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

#ifndef DXF_BASE_BOUNDARY_PATH_HPP
#define DXF_BASE_BOUNDARY_PATH_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_base_boundary_path {
  enum Path_type {
    DEFAULT = 0x0,
    EXTERNAL = 0x1,
    POLYLINE = 0x2,
    DERIVED = 0x4,
    TEXTBOX = 0x8,
    OUTERMOST = 0x10
  };

  /*! Desctruct.
   * This vrtual destructor is necessary to make this struct polymorphic.
   * Note, that it is used as a base class for several boundary-path types
   * dynamically allocated.
   */
  virtual ~Dxf_base_boundary_path() = default;

  /// Record members
  //@{

  int32_t m_flags;      // Boundary path type flag (bit coded):
                        // 0 = Default
                        // 1 = External
                        // 2 = Polyline
                        // 4 = Derived
                        // 8 = Textbox
                        // 16 = Outermost
  // varies Polyline boundary type data (only if boundary = polyline).
                        // See Polyline boundary data table below
  // Number of source boundary objects
  std::vector<SGAL::String> m_source_objects; // Reference to source boundary
                        // objects (multiple entries)

  //@}

  /// Record handlers
  //@{
  void handle_source_objects_num(int32_t size);
  void handle_source_object(const SGAL::String& handler);
  //@}
};

DXF_END_NAMESPACE

#endif
