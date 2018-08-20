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

#ifndef SGAL_TO_HEX_STRING_HPP
#define SGAL_TO_HEX_STRING_HPP

#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Return a string with the hex representation of val.
 * \param[in] Numerical value.
 */
template <typename T>
std::string to_hex_string(T val)
{
  std::stringstream ss;
  ss << std::hex << val;
  auto tmp = ss.str();
  boost::to_upper(tmp);
  return tmp;
}

SGAL_END_NAMESPACE

#endif
