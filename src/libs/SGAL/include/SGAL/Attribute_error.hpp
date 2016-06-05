// Copyright (c) 2016 Israel.
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

#ifndef SGAL_ATTRIBUTE_ERROR_HPP
#define SGAL_ATTRIBUTE_ERROR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/errors.hpp"

SGAL_BEGIN_NAMESPACE

/*! Class thrown when an attribute is erroneous. */
class SGAL_SGAL_DECL Attribute_error : public error {
 public:
  Attribute_error(const std::string& message) : error(message) {}
  ~Attribute_error() SGAL_NOTHROW {}
};

SGAL_END_NAMESPACE

#endif
