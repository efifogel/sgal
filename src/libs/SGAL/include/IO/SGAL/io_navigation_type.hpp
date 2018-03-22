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

#ifndef SGAL_IO_NAVIGATION_TYPE_HPP
#define SGAL_IO_NAVIGATION_TYPE_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Navigation_type.hpp"
#include "SGAL/Attribute_error.hpp"

SGAL_BEGIN_NAMESPACE

/*! Export a navigation type to an output stream.
 */
template <typename OutputStream>
inline OutputStream& operator<<(OutputStream& os, const Navigation_type& nt)
{
  os << nt;
  return os;
}

/*! Import a navigation type from an input stream.
 */
template <typename InputStream>
inline InputStream& operator>>(InputStream& is, Navigation_type& nt)
{
  String str;
  is >> str;
  for (size_t j = 0; j < static_cast<size_t>(Navigation_type::NUM_TYPES); ++j) {
    if (s_navigation_types[j] == str) {
      nt = static_cast<Navigation_type>(j);
      return is;
    }
  }

  throw Attribute_error(std::string("Unrecognized navigation type \"").append(str).append("\"!"));

  return is;
}

SGAL_END_NAMESPACE

#endif
