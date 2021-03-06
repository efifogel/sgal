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

#ifndef DXF_LINE_ENTITY_HPP
#define DXF_LINE_ENTITY_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_line_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;
  /// Record members
  //@{
  double m_thickness;   // Thickness (optional; default = 0)
  double m_start[3];    // Start point
  double m_end[3];      // End point
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)
  //@}

  /*! Construct (set default values).
   */
  Dxf_line_entity();

  //! Default values
  static const double s_def_thickness;
  static const double s_def_extrusion_direction[3];
};

DXF_END_NAMESPACE

#endif
