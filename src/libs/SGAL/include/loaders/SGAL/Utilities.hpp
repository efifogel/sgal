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
// $Revision: 1658 $
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

/*! */
struct lt_nocase : public std::binary_function<char, char, bool> {
  bool operator()(char x, char y) const
  {
    return ::toupper(static_cast<unsigned char>(x)) <
      ::toupper(static_cast<unsigned char>(y));
  }
};

/*! \brief compares the input string to "TRUE" or "FALSE" */
Boolean compare_to_true(const std::string & str);

/*! \brief obtains the number of tokens in a string */
Uint get_num_tokens(const std::string & str);

/*! \todo replace this code */
template<class T>
inline Uint get_num_values(const std::string & value)
{
  std::istringstream svalue(value, std::istringstream::in);
  Uint num_values;
  T tmp;
  for (num_values = 0; !svalue.eof(); svalue >> tmp, num_values++);
  return num_values;
}

/*! \brief strips embedding double quotes */
std::string strip_double_quotes(std::string str);

SGAL_END_NAMESPACE

#endif
