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

#ifndef SGAL_REVERSE_LINES_INDICES_VISITOR_HPP
#define SGAL_REVERSE_LINES_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Reverse lines indices visitor */
class Reverse_lines_indices_visitor : public boost::static_visitor<> {
public:
  template <typename Indices_>
  void operator()(Indices_& indices) const { SGAL_error(); }

  template <typename TargetIndices, typename SourceIndices>
  void operator()(TargetIndices& target, const SourceIndices& source) const
  { SGAL_error(); }

  /*! Reverse triangle indices in place.
   */
  void operator()(Line_indices& indices) const
  {
    for (size_t i = 0; i < indices.size(); ++i)
      std::swap(indices[i][0], indices[i][2]);
  }

  /*! Reverse triangle indices.
   */
  void operator()(Line_indices& target, const Line_indices& source) const
  {
    target.resize(source.size());
    for (size_t i = 0; i < source.size(); ++i) {
      target[i][0] = source[i][2];
      target[i][1] = source[i][1];
    }
  }

  /*! Reverse polyline indices in place.
   */
  void operator()(Polyline_indices& indices) const
  {
    for (auto& polyline : indices) {
      auto first = polyline.begin();
      auto last = polyline.end();
      while ((first == last) || (first == --last)) std::swap(*first, *last++);
    }
  }

  /*! Reverse polyline indices.
   */
  void
  operator()(Polyline_indices& target, const Polyline_indices& source) const
  {
    target.resize(source.size());
    for (size_t i = 0; i < source.size(); ++i) {
      target[i].resize(source[i].size());
      size_t j(0);
      for (auto it = source[i].rbegin(); it != source[i].rend(); ++it)
        target[i][j++] = *it;
    }
  }

  /*! Reverse flat indices in place.
   */
  void operator()(Flat_indices& indices) const
  {
    return;
  }

  /*! Reverse flat indices.
   */
  void operator()(Flat_indices& target, const Flat_indices& source) const
  {
    target.resize(source.size());
    size_t j(0);
    auto it = source.rbegin();
    for (++it; it != source.rend(); ++it) target[j++] = *it;
    target[j] = static_cast<Index_type>(-1);
  }
};

SGAL_END_NAMESPACE

#endif
