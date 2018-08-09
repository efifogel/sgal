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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_OPTION_MAPPER_HPP
#define SGAL_OPTION_MAPPER_HPP

#include <map>
#include <iterator>

#include <boost/icl/interval_set.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Option_mapper {
private:
  typedef boost::icl::interval_set<size_t>      set_t;
  typedef set_t::interval_type                  ival;

  //! Free codes arranged in a set of intervals.
  set_t m_free_codes;

protected:
  typedef std::map<String, size_t>                              Map;

  /*! Set the options.
   * \pre options must contain the INVALID code.
   */
  void set_options(Map&& options);

  /*! Obtain the options container.
   */
  const Map& get_options();

  //! The options.
  Map m_options;

public:
  //! A pre-defined code for the invalid option:
  enum Base_code { INVALID = 0, START_CODE };

  /*! Construct.
   */
  Option_mapper();

  /*! Destruct.
   */
  virtual ~Option_mapper();

  /*! Obtain the number of options.
   */
  size_t options_size() const;

  /*! Find the code of an option.
   * \param[in] name the option name.
   * \return the code.
   */
  size_t find_code(const String& name) const;

  /*! Find the name of the option of a code.
   * \param[in] code the code.
   * \return the option.
   */
  const String& find_name(size_t code) const;

  /*! Register a option.
   * \param[in] name the new option name.
   * \return the code allocated for the external option.
   */
  size_t register_option(const String& name);

  /*! Unregister a option.
   * \param[in] name the name of the option to remove.
   */
  void unregister_option(const String& name);

  /*! Iterator to the options.
   */
  class Name_const_iterator :
    public std::iterator<std::forward_iterator_tag, const String>
  {
  private:
    typedef Name_const_iterator       iterator;

    //! The internal itertor.
    Map::const_iterator m_it;

  public:
    Name_const_iterator() {}
    Name_const_iterator(Map::const_iterator it) : m_it(it) {}
    ~Name_const_iterator() {}

    iterator& operator++() /* prefix */ { ++m_it; return *this; }
    reference operator* () const { return m_it->first; }
    pointer operator->() const { return &(m_it->first); }
    bool operator==(const iterator& rhs) const { return m_it == rhs.m_it; }
    bool operator!=(const iterator& rhs) const { return m_it != rhs.m_it; }
  };

  /*! Obtain the begin iterator of option names.
   */
  Name_const_iterator names_begin() const
  { return Name_const_iterator(m_options.begin()); }

  /*! Obtain the past-the-end iterator of option names.
   */
  Name_const_iterator names_end() const
  { return Name_const_iterator(m_options.end()); }
};

SGAL_END_NAMESPACE

#endif
