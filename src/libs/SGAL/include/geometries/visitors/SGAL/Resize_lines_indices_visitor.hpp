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

#ifndef SGAL_RESIZE_LINES_INDICES_VISITOR_HPP
#define SGAL_RESIZE_LINES_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Resize lines indices visitor */
class Resize_lines_indices_visitor : public boost::static_visitor<> {
private:
  Lines_indices& m_target;

public:
  Resize_lines_indices_visitor(Lines_indices& target) : m_target(target) {}

  void operator()(const Line_indices& source)
  {
    m_target = Line_indices();
    auto& indices = boost::get<Line_indices>(m_target);
    indices.resize(source.size());
  }

  void operator()(const Polyline_indices& source)
  {
    m_target = Polyline_indices();
    auto& indices = boost::get<Polyline_indices>(m_target);
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
