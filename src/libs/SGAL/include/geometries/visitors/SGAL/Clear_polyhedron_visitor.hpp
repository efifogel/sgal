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

#ifndef SGAL_CLEAR_POLYHEDRON_VISITOR_HPP
#define SGAL_CLEAR_POLYHEDRON_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clear polyhedron visitor. */
class Clear_polyhedron_visitor : public boost::static_visitor<> {
public:
  template <typename Polyhedron_>
  void operator()(Polyhedron_& polyhedron) const { polyhedron.clear(); }
};

SGAL_END_NAMESPACE

#endif
