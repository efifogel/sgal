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

#ifndef DXF_LINE_WEIGHT_HPP
#define DXF_LINE_WEIGHT_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

enum class Line_weight {
  BY_BLOCK = -2,
  BY_LAYER = -1,
  BY_DEFAULT = -3,
  LW_000 = 0,
  LW_005 = 5,
  LW_009 = 9,
  LW_013 = 13,
  LW_015 = 15,
  LW_018 = 0x12,
  LW_020 = 20,
  LW_025 = 0x19,
  LW_030 = 30,
  LW_035 = 0x23,
  LW_040 = 40,
  LW_050 = 50,
  LW_053 = 0x35,
  LW_060 = 60,
  LW_070 = 70,
  LW_080 = 80,
  LW_090 = 90,
  LW_100 = 100,
  LW_106 = 0x6a,
  LW_120 = 120,
  LW_140 = 140,
  LW_158 = 0x9e,
  LW_200 = 200,
  LW_211 = 0xd3
};

DXF_END_NAMESPACE

#endif
