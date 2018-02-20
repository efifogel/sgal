// Copyright (c) 2004  Israel.
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

#include <string.h>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

static const std::string true_str("true");

//! \brief Compares the input string to "TRUE".
Boolean compare_to_true(const std::string& str)
{ return boost::iequals(str, true_str); }

//! \brief compares a string range to "TRUE" or "FALSE".
Boolean compare_to_true(std::string::const_iterator begin,
                        std::string::const_iterator end)
{ return boost::iequals(std::string(begin, end), true_str); }

//! \brief Obtains the number of tokens in a string.
size_t get_num_tokens(const std::string& str)
{
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  tokenizer tokens(str, sep);
  return std::distance(tokens.begin(), tokens.end());
}

SGAL_END_NAMESPACE
