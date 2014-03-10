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

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include <iostream>
#include <fstream>

#include "SGAL/version.hpp"
#include "SGAL/Generic_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

/*! Constructor */
Generic_option_parser::Generic_option_parser() :
  m_generic_opts("SGAL generic options")
{
  // Options allowed only on the command line
  m_generic_opts.add_options()
    ("author", "print author name(s)")
    ("help", "print help message")
    ("license", "print licence information")
    ("version", "print version string")
    ;
}

/*! Destructor */
Generic_option_parser::~Generic_option_parser() {}

/*! \brief applies the options */
void Generic_option_parser::apply(po::variables_map & variable_map)
{
  if (variable_map.count("help")) {
    std::cout << get_visible_opts() << std::endl;
    throw Generic_option_exception(HELP);
    return;
  }

  if (variable_map.count("version")) {
    std::cout << SGAL_LIB_VERSION << std::endl;
    throw Generic_option_exception(VERSION);
    return;
  }

  if (variable_map.count("author")) {
    std::cout << "Efi Fogel         <efifogel@gmail.com>" << std::endl;
    throw Generic_option_exception(AUTHOR);
    return;
  }

  if (variable_map.count("license")) {
    std::cout << "GNU Lesser General Public License (LGPL)"
              << std::endl;
    throw Generic_option_exception(LICENSE);
    return;
  }
}

SGAL_END_NAMESPACE
