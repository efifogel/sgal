// Copyright (c) 2014 Israel.
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
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/IO_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
IO_option_parser::IO_option_parser() :
  m_io_opts("SGAL IO options")
{
  typedef std::vector<std::string> vs;

  // Options allowed on the command line, config file, or env. variables
  m_io_opts.add_options()
    ("input-path,I", po::value<vs>()->composing(), "input path")
    ;
}

/*! Destructor */
IO_option_parser::~IO_option_parser() {}

/*! \brief applies the options */
void IO_option_parser::apply(po::variables_map& /* variable_map */) {}

// /*! \brief sets the Configuration node */
// void IO_option_parser::configure(po::variables_map& /* variable_map */) {}

SGAL_END_NAMESPACE
