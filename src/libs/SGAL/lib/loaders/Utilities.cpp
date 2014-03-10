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

#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief Compares the input string to "TRUE" or "FALSE" */
Boolean compare_to_true(const std::string & str)
{
  const char * true_str = "true";
  return (std::lexicographical_compare(str.begin(), str.end(),
                                       true_str,
                                       true_str + strlen(true_str),
                                       lt_nocase()) == 0);
}

/*! \brief Strips embedding double quotes */
std::string strip_double_quotes(std::string str)
{
  if (str[0] == '\"') str.erase(0, 1);
  Uint n = str.size() - 1;
  if (str[n] == '\"') str.resize(n);
  return str;
}

/*! \brief Obtains the number of tokens in a string
 */
Uint get_num_tokens(const std::string & str)
{
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  tokenizer tokens(str, sep);
  Uint size = 0;
  for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
    ++size;
  return size;
}

SGAL_END_NAMESPACE
