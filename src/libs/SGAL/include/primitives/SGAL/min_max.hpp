// Copyright (c) 2004 Israel.
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
// $Source$
// $Revision: 1311 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>
#ifndef MIN_MAX_H
#define MIN_MAX_H

#if (defined _MSC_VER)
#if _MSC_VER < 1300

namespace std {

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

template <class T>
inline const T& min(const T& a, const T& b)
{
  return b < a ? b : a;
}

template <class T>
inline const T& max(const T& a, const T& b)
{
  return a < b ? b : a;
}

template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp)
{
  return comp(b, a) ? b : a;
}

template <class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp)
{
  return comp(a, b) ? b : a;
}

} // namespace std

#endif // _MSC_VER < 1300
#endif

#endif
