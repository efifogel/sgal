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
// $Revision: 1821 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VERSION_HPP
#define SGAL_VERSION_HPP

/*! \file
 * Caution, this is the only SGAL header that is guarenteed
 * to change with every SGAL release, including this header
 * will cause a recompile every time a new SGAL version is
 * released.
 *
 * SGAL_VERSION % 100 is the sub-minor version
 * SGAL_VERSION / 100 % 1000 is the minor version
 * SGAL_VERSION / 100000 is the major version
 */

#define SGAL_VERSION 100001

//
//  SGAL_LIB_VERSION must be defined to be the same as SGAL_VERSION
//  but as a *string* in the form "x_y" where x is the major version
//  number and y is the minor version number.  This is used by 
//  <config/auto_link.hpp> to select which library version to link to.

#define SGAL_LIB_VERSION "1_01"

#endif
