// Copyright (c) 2012 Israel.
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

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4390 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <boost/extension/extension.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"

#include "SEGO/Ego.hpp"
#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxels_filler.hpp"
#include "SEGO/Ego_voxels_layer_filler.hpp"

SGAL_BEGIN_NAMESPACE

extern "C" void BOOST_EXTENSION_EXPORT_DECL sego_init()
{
  Container_factory::get_instance()->doregister(Ego::prototype());
  Container_factory::get_instance()->doregister(Ego_brick::prototype());
  Container_factory::get_instance()->doregister(Ego_voxels_filler::prototype());
  Container_factory::get_instance()->doregister(Ego_voxels_layer_filler::prototype());
}

SGAL_END_NAMESPACE
