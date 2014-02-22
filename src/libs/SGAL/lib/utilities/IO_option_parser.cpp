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
#include "SGAL/File_format.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
IO_option_parser::IO_option_parser() :
  m_io_opts("SGAL IO options")
{
  typedef std::vector<std::string> vs;

  // Options allowed on the command line, config file, or env. variables
  m_io_opts.add_options()
    ("input-path", po::value<vs>()->composing(), "input path")
    ("output-formats,f", po::value<Formats>(&m_formats)->multitoken(),
       "output formats\n"
       "  wrl\n"
       "  x3d\n"
       "  stl\n"
       "  obj")
    ("save,S", po::value<Boolean>(&m_save)->default_value(false), "save")
    ("interactive,I", po::value<Boolean>(&m_interactive)->default_value(true),
     "save")
    ("output-file", po::value<std::string>(), "output file")
    ;
}

/*! Destructor */
IO_option_parser::~IO_option_parser() {}

/*! \brief applies the options */
void IO_option_parser::apply(po::variables_map& /* variable_map */) {}

// /*! \brief sets the Configuration node */
// void IO_option_parser::configure(po::variables_map& /* variable_map */) {}

std::istream& operator>>(std::istream& in, File_format::Id& format)
{
  std::string token;
  in >> token;
  for (Uint i = 0; i < File_format::NUM_IDS; ++i) {
    if (! File_format::compare_name(i, token)) continue;
    format = static_cast<File_format::Id>(i);
    return in;
  }
  throw po::validation_error(po::validation_error::invalid_option_value);
  return in;
}

SGAL_END_NAMESPACE
