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

#ifndef SGAL_EMPTY_POLYHEDRON_VISITOR_HPP
#define SGAL_EMPTY_POLYHEDRON_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Empty polyhedron visitor. */
class Empty_polyhedron_visitor : public boost::static_visitor<Boolean> {
public:
  template <typename Polyhedron_>
  Boolean operator()(const Polyhedron_& polyhedron) const
  { return polyhedron.empty(); }
};

SGAL_END_NAMESPACE

#endif
