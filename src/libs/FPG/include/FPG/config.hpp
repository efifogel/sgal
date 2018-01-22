// Copyright (c) 2004 Israel.
// All rights reserved.
//
// This file is private property of Xenia Optimal Ltd.
//
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef FPG_CONFIG_HPP
#define FPG_CONFIG_HPP

#include <boost/config.hpp>

// #include "SGAL/version.hpp"

///////////////////////////////////////////////////////////////////////////////
// Windows DLL suport
#ifdef BOOST_HAS_DECLSPEC
#if defined(SGAL_ALL_DYN_LINK) || defined(SGAL_SGAL_DYN_LINK)
// export if this is our own source, otherwise import:
#ifdef SGAL_FPG_SOURCE
# define SGAL_FPG_DECL __declspec(dllexport)
#else
# define SGAL_FPG_DECL __declspec(dllimport)
#endif  // SGAL_FPG_SOURCE
#endif  // DYN_LINK
#endif  // BOOST_HAS_DECLSPEC

#ifndef SGAL_FPG_DECL
#define SGAL_FPG_DECL
#endif

#endif
