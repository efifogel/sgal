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

#ifndef SGAL_EQUAL_FACET_INDICES_VISITOR_HPP
#define SGAL_EQUAL_FACET_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Equal facet indices visitor */
class Equal_facet_indices_visitor : public boost::static_visitor<Boolean> {
public:
  template <typename T, typename U>
  Boolean operator()(const T&, const U&) const { return false; }

  template <typename T>
  Boolean operator()(const T& lhs, const T& rhs) const { return lhs == rhs; }
};

SGAL_END_NAMESPACE

#endif
