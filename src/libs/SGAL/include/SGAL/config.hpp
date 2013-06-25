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
// $Source$
// $Revision: 1809 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CONFIG_HPP
#define SGAL_CONFIG_HPP

#include <boost/config.hpp>

#include "SGAL/version.hpp"

//----------------------------------------------------------------------//
//             include platform specific workaround flags (SGAL_CFG_...)
//----------------------------------------------------------------------//

// #include <SGAL/compiler_config.h>

//----------------------------------------------------------------------//
//             do some post processing for the flags
//----------------------------------------------------------------------//

#ifdef SGAL_CFG_NO_NAMESPACE
#  define SGAL_USING_NAMESPACE_STD
#  define SGAL_STD
#  define SGAL std
#else
#  define SGAL_USING_NAMESPACE_STD using namespace std;
#  define SGAL_STD std
#  ifndef SGAL_USE_NAMESPACE
#    define SGAL_USE_NAMESPACE 1
#  endif
#endif

#if SGAL_USE_NAMESPACE
#  define SGAL_BEGIN_NAMESPACE namespace SGAL {
#  define SGAL_END_NAMESPACE }
#else
#  define SGAL_BEGIN_NAMESPACE
#  define SGAL_END_NAMESPACE
#endif

#ifdef SGAL_CFG_NO_STDC_NAMESPACE
#  define SGAL_CLIB_STD
#else
#  define SGAL_CLIB_STD std
#endif

#ifndef SGAL_CFG_NO_LONG_LONG
#  define SGAL_USE_LONG_LONG
#endif

// FIXME: what is the problem with Sun 5.5? MATCHING_BUG_4 is not
// triggered, but there are runtime errors, e.g., with Distance_3,
// that do not appear when using the wrapper...
#if defined(SGAL_CFG_MATCHING_BUG_4) || \
  (defined(__sun) && defined(__SUNPRO_CC))
namespace SGAL {
    template < typename T >
    struct Self { typedef T Type; };
}
#  define SGAL_WRAP(K) SGAL::Self<K>::Type
#else
#  define SGAL_WRAP(K) K
#endif

//----------------------------------------------------------------------//
//             include separate workaround files
//----------------------------------------------------------------------//

#ifdef _MSC_VER
#  include "SGAL/MSVC_standard_header_fixes.hpp"
#endif
#if defined(__BORLANDC__) && __BORLANDC__ > 0x520
#include "SGAL/Borland_fixes.hpp"
#endif
#if defined(__sun) && defined(__SUNPRO_CC)
#include "SGAL/Sun_fixes.hpp"
#endif

//--------------------------------------------------------------------//
// This addresses a bug in VC++ 7.0 that (re)defines min(a, b)
// and max(a, b) in windows.h and windef.h 
//-------------------------------------------------------------------//

#ifdef _MSC_VER
#  define NOMINMAX 1
#endif

//-------------------------------------------------------------------//
// When the global min and max are no longer defined (as macros) 
// because of NOMINMAX flag definition, we define our own global 
// min/max functions to make the Microsoft headers compile. (afxtempl.h)
// Users that does not want the global min/max 
// should define SGAL_NOMINMAX
//-------------------------------------------------------------------//
#include <algorithm>
#if defined NOMINMAX && !defined SGAL_NOMINMAX
using std::min;
using std::max;
#endif

// Is Geomview usable ?
#if !defined(__BORLANDC__) && !defined(_MSC_VER) && !defined(__MINGW32__)
#  define SGAL_USE_GEOMVIEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Windows DLL suport
#ifdef BOOST_HAS_DECLSPEC
#if defined(SGAL_ALL_DYN_LINK) || defined(SGAL_SGAL_DYN_LINK)
// export if this is our own source, otherwise import:
#ifdef SGAL_SGAL_SOURCE
# define SGAL_SGAL_DECL __declspec(dllexport)
#else
# define SGAL_SGAL_DECL __declspec(dllimport)
#endif  // SGAL_SGAL_SOURCE
#endif  // DYN_LINK
#endif  // SGAL_HAS_DECLSPEC

#ifndef SGAL_SGAL_DECL
#define SGAL_SGAL_DECL
#endif

#endif
