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

#ifndef SGAL_DXF_EDGE_HPP
#define SGAL_DXF_EDGE_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Dxf_parser;

//!
struct Dxf_edge {
  enum Edge__type {
    LINE = 1,
    CIRCULAR_ARC = 2,
    ELLIPTIC_ARC = 3,
    SPLINE = 4
  };

  int16_t m_edge_type;  // Edge type (only if boundary is not a polyline):
                        // 1 = Line
                        // 2 = Circular arc
                        // 3 = Elliptic arc
                        // 4 = Spline

  /*! Set the parser.
   * \param[in] parser the parser.
   */
  void set_parser(Dxf_parser* parser) { m_parser = parser; }

  //! The parser.
  Dxf_parser* m_parser;
};

SGAL_END_NAMESPACE

#endif
