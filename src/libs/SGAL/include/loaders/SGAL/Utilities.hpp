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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_UTILITIES_HPP
#define SGAL_UTILITIES_HPP

#include <boost/tokenizer.hpp>

#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Compare a string to "TRUE" or "FALSE".
 * \param str (in) the input string.
 * \return true if str is equal to "TRUE" and false otherwise.
 */
SGAL_SGAL_DECL Boolean compare_to_true(const std::string& str);

/*! Compare a string range to "TRUE" or "FALSE".
 * \param begin (in) the input string begin iterator.
 * \param end (in) the input string past-the-end iterator.
 * \return true if str is equal to "TRUE" and false otherwise.
 */
SGAL_SGAL_DECL Boolean compare_to_true(std::string::const_iterator begin,
                                       std::string::const_iterator end);

/*! Obtain the number of tokens separated by while characters in a string.
 * \param str (in) the input string.
 * \return the number of tokens.
 */
SGAL_SGAL_DECL size_t get_num_tokens(const std::string& str);

/*! \todo replace this code */
template <typename T>
inline size_t get_num_values(const std::string& value)
{
  std::istringstream svalue(value, std::istringstream::in);
  size_t num_values;
  T tmp;
  for (num_values = 0; !svalue.eof(); svalue >> tmp, num_values++);
  return num_values;
}

/*! Strip embedding double quotes.
 * \param str (in) the input string.
 */
SGAL_SGAL_DECL std::string strip_double_quotes(std::string str);

SGAL_END_NAMESPACE

#endif
