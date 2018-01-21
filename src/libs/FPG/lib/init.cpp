// Copyright (c) 2018 Israel.
// All rights reserved.
//
// This file is private property of Xenia Optimal Ltd.
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

#include "FPG/Store.hpp"
#include "FPG/Wall.hpp"
#include "FPG/Column.hpp"

SGAL_BEGIN_NAMESPACE

extern "C" void BOOST_EXTENSION_EXPORT_DECL fpg_init()
{
  Container_factory::get_instance()->doregister(Store::prototype());
  Container_factory::get_instance()->doregister(Wall::prototype());
  Container_factory::get_instance()->doregister(Column::prototype());
}

SGAL_END_NAMESPACE
