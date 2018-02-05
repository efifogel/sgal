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

#ifndef SGAL_MULTI_ISTREAM_ITERATOR_HPP
#define SGAL_MULTI_ISTREAM_ITERATOR_HPP

#include <iterator>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

template <size_t d>
class multi_istream_iterator :
  public std::iterator<std::input_iterator_tag, String>
{
private:
  std::istream* m_is;
  String m_str;
  void fetch() {
    if (!m_is) return;
    if (! (*m_is >> m_str)) {
      m_is = 0;
      return;
    }
    for (size_t i = 1; i < d; ++i) {
      String tmp;
      if (! (*m_is >> tmp)) {
        m_is = 0;
        return;
      }
      m_str.append(" ").append(tmp);
    }
  }

public:
  multi_istream_iterator(std::istream& is) : m_is(&is) { fetch(); }
  multi_istream_iterator() : m_is(0) {}

  String& operator*() { return m_str; }

  multi_istream_iterator& operator++() {
    fetch();
    return *this;
  }

  bool operator==(const multi_istream_iterator& rhs) const
  { return (!rhs.m_is) == (!m_is); }

  bool operator!=(const multi_istream_iterator& rhs) const
  { return !(rhs == *this); }
};

SGAL_END_NAMESPACE

#endif
