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

#ifndef SGAL_FACES_COUNTER_VISITOR_HPP
#define SGAL_FACES_COUNTER_VISITOR_HPP

#include <algorithm>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! A visitor that counts the number of facets of a polyhedron. */
class Faces_counter_visitor : public boost::static_visitor<Size> {
public:
  template <typename Indices_>
  Size operator()(const Indices_& indices) const { return indices.size(); }

  Size operator()(const Flat_indices& indices) const
  { return std::count(indices.begin(), indices.end(), -1); }
};

SGAL_END_NAMESPACE

#endif
