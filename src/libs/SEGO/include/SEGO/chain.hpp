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

#ifndef SEGO_CHAIN_HPP
#define SEGO_CHAIN_HPP

/**
 * @file   chain.hpp
 * @author Ophir Setter <ophirset@ophir-desktop>
 * @date   Tue Apr  2 00:35:00 2013
 * 
 * @brief  For some reason, I could not find this functionality in boost.
 *         Although you have the ability to chain function objects using
 *         boost::bind, I did not find a way to actually receive the fucntion
 *         object type itself which is need for some implementations.
 *         c++11 probably solves this complitely.
 *         For more information see: http://boost.2283326.n4.nabble.com/boost-bind-result-type-td2672563.html
 * 
 */

#include <boost/tuple/tuple.hpp>
#include <boost/mpl/bool.hpp>

SGAL_BEGIN_NAMESPACE

namespace chain_ns {

  // These traits are used to skip some of the calculations
  // For example, in logical_and or logical_or.
  
  template <typename T>
  struct chain_traits {
    typedef boost::mpl::false_ can_stop;
  };
  
  template <>
  struct chain_traits<std::logical_and<bool> > {
    typedef boost::mpl::true_ can_stop ;
    BOOST_STATIC_CONSTANT(bool, stop_value = false);
  };
  
  // forward declaration.
  template <int Begin, typename Op, typename Tuple>
  struct Accumulate;

  template <int Begin, typename Op, typename Tuple, size_t i>
  struct Accumulate_internal {};

  template <int Begin, typename Op, typename Tuple>
  struct Accumulate_internal<Begin, Op, Tuple, 0> {
    typedef typename Op::result_type result_type;

    typedef typename 
    boost::tuples::element<Begin,
                           typename Tuple::inherited >::type::argument_type
                                                argument_type;

    Accumulate_internal(Op op, Tuple tuple) : m_op(op), m_tuple(tuple) {}

    result_type operator()(argument_type x, boost::mpl::true_) {
      result_type prev =
        typename Accumulate<Begin + 1, Op, Tuple>::type (m_op, m_tuple) (x);
      
      if (prev == chain_traits<Op>::stop_value)
        return prev;
      
      return m_op(m_tuple.template get<Begin>()(x), prev);
    }

    result_type operator()(argument_type x, boost::mpl::false_) {
      result_type prev =
        typename Accumulate<Begin + 1, Op, Tuple>::type (m_op, m_tuple) (x);
      return m_op(m_tuple.template get<Begin>()(x), prev);
    }

    
    result_type operator()(argument_type x) {
      typedef typename chain_traits<Op>::can_stop can_stop;
      return operator()(x, can_stop());
    }
    
    Op m_op;
    Tuple m_tuple;
  };

  template <int Begin, typename Op, typename Tuple>
  struct Accumulate_internal<Begin, Op, Tuple, 1> {
    typedef typename 
    boost::tuples::element<Begin,
                           typename Tuple::inherited>::type::argument_type
      argument_type;

    typedef typename Op::result_type result_type;

    Accumulate_internal(Op op, Tuple tuple) : m_op(op), m_tuple(tuple) {}

    result_type operator()(argument_type x, boost::mpl::true_) {
      BOOST_STATIC_CONSTANT(int, size = boost::tuples::length<Tuple>::value);
      result_type res = m_tuple.template get<size-1>()(x);
      if (res == chain_traits<Op>::stop_value)
        return res;

      return m_op(m_tuple.template get<size-2>()(x), res);
    }

    result_type operator()(argument_type x, boost::mpl::false_) {
      BOOST_STATIC_CONSTANT(int, size = boost::tuples::length<Tuple>::value);
      return m_op(m_tuple.template get<size-2>()(x),
                  m_tuple.template get<size-1>()(x));
    }
    
    result_type operator()(argument_type x) {
      typedef typename chain_traits<Op>::can_stop can_stop;
      return operator()(x, can_stop());
    }
    
    Op m_op;
    Tuple m_tuple;
  };

  template <int Begin, typename Op, typename Tuple>
  struct Accumulate {
    BOOST_STATIC_CONSTANT(int, size = boost::tuples::length<Tuple>::value);
    enum {I = (Begin == size - 2) ? 1 : 0 };
    
    typedef Accumulate_internal<Begin, Op, Tuple, I> type;
  };
}

// The class uses one binary function object multiple times on 
// multipe unary function objects to receive the final result.
// Similar to accumulate.
template <class BinaryFunc,
          class Un1,
          class Un2,
          class Un3 = boost::tuples::null_type,
          class Un4 = boost::tuples::null_type,
          class Un5 = boost::tuples::null_type,
          class Un6 = boost::tuples::null_type>
class chain : public boost::tuple<Un1, Un2, Un3, Un4, Un5, Un6> {
public:
  typedef typename BinaryFunc::result_type                result_type;
  typedef chain<BinaryFunc, Un1, Un2, Un3, Un4, Un5, Un6> This;
  typedef boost::tuple<Un1, Un2, Un3, Un4, Un5, Un6>      Base;
  
  chain(BinaryFunc nfunc, Un1 un1, Un2 un2)
      : Base(un1, un2), m_binary_func(nfunc) {}

  chain(BinaryFunc nfunc, Un1 un1, Un2 un2, Un3 un3)
      : Base(un1, un2, un3), m_binary_func(nfunc) {}

  chain(BinaryFunc nfunc, Un1 un1, Un2 un2, Un3 un3, Un4 un4)
      : Base(un1, un2, un3, un4), m_binary_func(nfunc) {}

  chain(BinaryFunc nfunc, Un1 un1, Un2 un2, Un3 un3, Un4 un4, Un5 un5)
      : Base(un1, un2, un3, un4, un5), m_binary_func(nfunc) {}

  chain(BinaryFunc nfunc, Un1 un1, Un2 un2, Un3 un3, Un4 un4, Un5 un5, Un6 un6)
      : Base(un1, un2, un3, un4, un5, un6), m_binary_func(nfunc) {}
  
  // We need this for filter_iterator (Why?)
  chain() {}

  // TODO: move to boost::fuse. Specifically, the accumulator function.

  // TODO: add static assert that all argument_type are the same + 
  //       that the argument type of the binary fits...

  typedef typename Un1::argument_type argument_type;

  result_type operator () (argument_type x) {
    return 
      typename chain_ns::Accumulate<0, BinaryFunc, Base>::type (m_binary_func,
                                                                *this) (x);
  }

private:
  BinaryFunc m_binary_func;
};

SGAL_END_NAMESPACE

#endif // SEGO_CHAIN_HPP



