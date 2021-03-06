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

#ifndef DXF_SOLID_ENTITY_HPP
#define DXF_SOLID_ENTITY_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_solid_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  /// Record members
  //@{
  double m_corner1[3];  // First corner
  double m_corner2[3];  // Second corner
  double m_corner3[3];  // Third corner
  double m_corner4[3];  // Fourth corner
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)

  //@}

  /*! Construct (set default values).
   */
  Dxf_solid_entity() :
    m_extrusion_direction{0, 0, 1}
  {}

};

DXF_END_NAMESPACE

#endif
