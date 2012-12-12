// Copyright (c) 2012 Israel.
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
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

#ifndef _EGO_MULTI_ITERATOR_HPP_
#define _EGO_MULTI_ITERATOR_HPP_

/**
 * @file   multi_iterator.hpp
 * @author Ophir Setter <ophirset@ophir-desktop>
 * @date   Thu Dec 13 21:48:31 2012
 * 
 * @brief  Multi iterator receive several iterators (currently only 3)
 *         and creates an iterator that iterates through all cartesian
 *         products of the 3 iterators. It returns boost::tuple.
 *         The input iterators have to support multiple iterations (don't
 *         remember the concept.
 */

#include "SGAL/config.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/tuple/tuple.hpp>

SGAL_BEGIN_NAMESPACE

  template <class It1, class It2, class It3>
  class multi_iterator : public boost::iterator_facade<
    multi_iterator<It1, It2, It3>,
    boost::tuple<typename std::iterator_traits<It1>::value_type,
                 typename std::iterator_traits<It2>::value_type,
                 typename std::iterator_traits<It3>::value_type>,
                   boost::forward_traversal_tag,
    boost::tuple<typename std::iterator_traits<It1>::reference,
                 typename std::iterator_traits<It2>::reference,
                 typename std::iterator_traits<It3>::reference> > {
  public:

    // TODO: Why do I need this?
    multi_iterator() {}
    
  /** 
   * The constructor receives ranges of all 3 iterators.
   * 
   * @param it1_b Begin iterator for 1st iterator.
   * @param it1_e End iterator for the 1st iterator.
   * @param it2_b Begin iterator for the 2nd iterator.
   * @param it2_e End iterator for the 2nd iterator.
   * @param it3_b Begin iterator for the 3rd iterator.
   * @param it3_e End iteator of the 3rs iterator.
   */
    multi_iterator(It1 it1_b, It1 it1_e,
                   It2 it2_b, It2 it2_e,
                   It3 it3_b, It3 it3_e)
        : m_it1(it1_b), m_it1end(it1_e), m_it2(it2_b),
          m_it2begin(it2_b), m_it2end(it2_e), m_it3(it3_b),
          m_it3begin(it2_b), m_it3end(it3_e) {}

  protected:
    friend class boost::iterator_core_access;

    typedef multi_iterator<It1, It2, It3>            This;
    typedef boost::iterator_facade<
      This,
      boost::tuple<typename std::iterator_traits<It1>::value_type,
                   typename std::iterator_traits<It2>::value_type,
                   typename std::iterator_traits<It3>::value_type>,
                     boost::forward_traversal_tag,
      boost::tuple<typename std::iterator_traits<It1>::reference,
                   typename std::iterator_traits<It2>::reference,
                   typename std::iterator_traits<It3>::reference> >
    Base;

    typedef Base                                        Facade;

    typename Facade::reference
    dereference() const {
      return typename Facade::reference(*m_it1, *m_it2, *m_it3);
    }

    bool equal(const This& o) const {
      return ((this->m_it1 == o.m_it1) &&
              (this->m_it2 == o.m_it2) &&
              (this->m_it3 == o.m_it3));
    }

    void increment() {
      ++m_it3;

      if (m_it3 == m_it3end && m_it2 != m_it2end) {
        m_it3 = m_it3begin;
        ++m_it2;
      }
      
      if (m_it2 == m_it2end && m_it1 != m_it1end) {
        m_it2 = m_it2begin;
        ++m_it1;
      }
      
      // end
      if (m_it1 == m_it1end) {
        m_it2 = m_it2end;
        m_it3 = m_it3end;
      }
    }

  private:
    It1 m_it1, m_it1end;
    It2 m_it2, m_it2begin, m_it2end;
    It3 m_it3, m_it3begin, m_it3end;
  };

SGAL_END_NAMESPACE

#endif // _EGO_MULTI_ITERATOR_HPP_
