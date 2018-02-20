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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SUN_FIXES_HPP
#define SGAL_SUN_FIXES_HPP

//----------------------------------------------------------------------//
//             if member templates for stdlib are not enabled           //
//----------------------------------------------------------------------//

/*

For reasons of binary backward compatibility, Sun CC does not enable 
member templates of the STL classes.

An #undef creates runtime errors for some packages, so it is not a 
viable solution. Instead, we have to offer workarounds in SGAL
code, whereever we use this feature.

*/
 
#include <stdcomp.h>

#if defined(RWSTD_NO_MEMBER_TEMPLATES) || defined(_RWSTD_NO_MEMBER_TEMPLATES)

#define SGAL_CFG_RWSTD_NO_MEMBER_TEMPLATES

#endif

//----------------------------------------------------------------------//
//             fake iterator_traits                                     //
//----------------------------------------------------------------------//

#include <iterator>

namespace std {

  template <class Iterator> struct iterator_traits
  {
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
  };

  template <class T> struct iterator_traits<T*>
  {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_access_iterator_tag iterator_category;
  };

  template <class T> struct iterator_traits<const T*>
  {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_access_iterator_tag iterator_category;
  };

  template <class ForwardIterator>
  inline ptrdiff_t
  distance (ForwardIterator first, ForwardIterator last)
  {
    ptrdiff_t n = 0;
    __distance(first, last, n, 
               iterator_traits<ForwardIterator>::iterator_category());
    return n;
  }

} // namespace std

namespace SGAL {

  template < class T >
  inline typename T::value_type*
  __value_type (const T&)
  { return (typename T::value_type*)(0); }

  template < class T >
  inline typename T::difference_type*
  __distance_type(const T&)
  { return (typename T::difference_type*)(0); }

  template < class T >
  inline typename T::iterator_category
  __iterator_category (const T&)
  {
    typename T::iterator_category tmp;
    return tmp;
  }

  template < class T >
  inline T* __value_type(T*)
  { return (T*)(0); }

  template <class T>
  inline std::ptrdiff_t* 
  __distance_type (T*)
  { return (std::ptrdiff_t*)(0); }

  template <class T>
  inline std::random_access_iterator_tag 
  __iterator_category (T*)
  { return std::random_access_iterator_tag(); }

} // namespace SGAL

#endif
