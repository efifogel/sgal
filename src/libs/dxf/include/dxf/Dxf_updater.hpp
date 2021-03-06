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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef DXF_UPDATER_HPP
#define DXF_UPDATER_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

SGAL_END_NAMESPACE

DXF_BEGIN_NAMESPACE

class Dxf_data;

class SGAL_SGAL_DECL Dxf_updater {
public:
  typedef boost::shared_ptr<SGAL::Container>            Shared_container;

  /*! Construct.
   */
  Dxf_updater(Dxf_data& data);

  /*! Handle a node.
   */
  void operator()(Shared_container node);

protected:
  //! Obtain a compact colod.
  // \todo Consider moveing out from here.
  template <typename SharedColorArray>
    int compact_color(SharedColorArray array, size_t index);

  //! The DXF data.
  Dxf_data& m_data;
};

//! \brief obtains a compact colod.
template <typename SharedColorArray>
inline int Dxf_updater::compact_color(SharedColorArray color_array, size_t index)
{
  auto r = static_cast<int>((*color_array)[index][0] * 255.0 + 0.5f);
  auto g = static_cast<int>((*color_array)[index][1] * 255.0 + 0.5f);
  auto b = static_cast<int>((*color_array)[index][2] * 255.0 + 0.5f);
  return (r << 16) | (g << 8) | b;
}

DXF_END_NAMESPACE

#endif
