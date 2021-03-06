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

#ifndef DXF_BOUNDARY_PATH_HPP
#define DXF_BOUNDARY_PATH_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_boundary_path.hpp"
#include "dxf/Dxf_edge.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

//!
struct Dxf_boundary_path : Dxf_base_boundary_path {
  /// Record members
  //@{

  // Number of edges in this boundary path (only if boundary is not a
                        // polyline)
  std::vector<Dxf_edge*> m_edges;
  // varies Edge type data (only if boundary is not a polyline).
                        // See appropriate Edge data table below

  //@}

  /// Record handlers
  //@{
  void handle_edges_num(int32_t size);
  //@}

  /*! Initialize */
  void init();

  /*! Set the parser.
   * \param[in] parser the parser.
   */
  void set_parser(Dxf_parser* parser) { m_parser = parser; }

  //! The parser
  Dxf_parser* m_parser;
};

DXF_END_NAMESPACE

#endif
