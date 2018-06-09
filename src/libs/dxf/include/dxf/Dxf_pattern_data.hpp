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

#ifndef SGAL_DXF_PATTERN_DATA_HPP
#define SGAL_DXF_PATTERN_DATA_HPP

#include <vector>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_pattern_data {
  double m_angle;        // Pattern line angle
  double m_base_point[2]; // Pattern line base point, X component
  double m_offset[2];   // Pattern line offset
  std::vector<double> m_dash_lengths; // Dash length (multiple entries)

  /// Handlers
  //@{
  void handle_size(int16_t size);
  void handle_dash_length(double dash_length);
  //@{
};

SGAL_END_NAMESPACE

#endif
