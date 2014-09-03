// Copyright (c) 2014 Israel.
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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Bounding_box.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief obtains a bounding box of the current bounding box and a given one.
Bounding_box Bounding_box::operator+(const Bounding_box& bb) const
{
  Bounding_box bbox(std::min(xmin(), bb.xmin()),
                    std::min(ymin(), bb.ymin()),
                    std::min(zmin(), bb.zmin()),
                    std::max(xmax(), bb.xmax()),
                    std::max(ymax(), bb.ymax()),
                    std::max(zmax(), bb.zmax()));
  return bbox;
}

//! \brief updates the bounding box with a given one.
Bounding_box& Bounding_box::operator+=(const Bounding_box& bb)
{
  if (bb.xmin() < xmin()) m_x_min = bb.xmin();
  if (bb.ymin() < ymin()) m_y_min = bb.ymin();
  if (bb.zmin() < zmin()) m_z_min = bb.zmin();
  if (bb.xmax() > xmax()) m_x_max = bb.xmax();
  if (bb.ymax() > ymax()) m_y_max = bb.ymax();
  if (bb.zmax() > zmax()) m_z_max = bb.zmax();
  return *this;
}

SGAL_END_NAMESPACE
