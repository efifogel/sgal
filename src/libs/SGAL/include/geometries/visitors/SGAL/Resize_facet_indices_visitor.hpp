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

#ifndef SGAL_RESIZE_FACET_INDICES_VISITOR_HPP
#define SGAL_RESIZE_FACET_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Resize facet indices visitor */
class Resize_facet_indices_visitor : public boost::static_visitor<> {
private:
  Facet_indices& m_target;

public:
  Resize_facet_indices_visitor(Facet_indices& target) : m_target(target) {}

  void operator()(const Triangle_indices& source)
  {
    m_target = Triangle_indices();
    auto& indices = boost::get<Triangle_indices>(m_target);
    indices.resize(source.size());
  }

  void operator()(const Quad_indices& source)
  {
    m_target = Quad_indices();
    auto& indices = boost::get<Quad_indices>(m_target);
    indices.resize(source.size());
  }

  void operator()(const Polygon_indices& source)
  {
    m_target = Polygon_indices();
    auto& indices = boost::get<Polygon_indices>(m_target);
    indices.resize(source.size());
    auto tit = indices.begin();
    for (auto sit = source.begin(); sit != source.end(); ++sit, ++tit)
      tit->resize(sit->size());
  }

  void operator()(const Flat_indices& source)
  {
    m_target = Flat_indices();
    auto& indices = boost::get<Flat_indices>(m_target);
    indices.resize(source.size());
  }
};

SGAL_END_NAMESPACE

#endif
