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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TO_BOOLEAN_HPP
#define SGAL_TO_BOOLEAN_HPP

#include <string>

#include <boost/algorithm/string.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

inline Boolean to_boolean(const SGAL::String& str)
{
  if (boost::iequals(str, "true")) return true;
  if (boost::iequals(str, "false")) return false;
  throw std::invalid_argument(std::string("Error: invalid argument (to_bool(" + str + "))!"));
  return false;
}

SGAL_END_NAMESPACE

#endif
