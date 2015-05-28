// Copyright (c) 2004 Israel.
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

#ifndef SCGAL_CONFIG_HPP
#define SCGAL_CONFIG_HPP

#include <boost/config.hpp>

// #include "SCGAL/version.hpp"

///////////////////////////////////////////////////////////////////////////////
// Windows DLL suport
#ifdef BOOST_HAS_DECLSPEC
#if defined(SGAL_ALL_DYN_LINK) || defined(SGAL_SGAL_DYN_LINK)
// export if this is our own source, otherwise import:
#ifdef SGAL_SCGAL_SOURCE
# define SGAL_SCGAL_DECL __declspec(dllexport)
#else
# define SGAL_SCGAL_DECL __declspec(dllimport)
#endif  // SGAL_SCGAL_SOURCE
#endif  // DYN_LINK
#endif  // BOOST_HAS_DECLSPEC

#ifndef SGAL_SCGAL_DECL
#define SGAL_SCGAL_DECL
#endif

#endif
