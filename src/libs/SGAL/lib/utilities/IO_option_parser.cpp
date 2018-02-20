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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/IO_option_parser.hpp"
#include "SGAL/File_format_3d.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
IO_option_parser::IO_option_parser() :
  m_io_opts("SGAL IO options"),
  m_snapshot(false),
  m_export(false),
  m_interactive(true),
  m_binary(true)
{
  typedef std::vector<std::string> vs;

  // Options allowed on the command line, config file, or env. variables
  m_io_opts.add_options()
    ("input-path", po::value<vs>()->composing(), "input path")
    ("format-2d", po::value<Formats_2d>(&m_formats_2d)->multitoken(),
       "Image formats\n"
       "  jpg\n"
       "  png")
    ("format-3d,f", po::value<Formats_3d>(&m_formats_3d)->multitoken(),
       "3D formats\n"
       "  wrl\n"
       "  x3d\n"
       "  stl\n"
       "  obj\n"
       "  json")
    ("snapshot,S", po::value<Boolean>(&m_snapshot)->default_value(false),
     "snapshot")
    ("export,E", po::value<Boolean>(&m_export)->default_value(false), "export")
    ("interactive,I", po::value<Boolean>(&m_interactive)->default_value(true),
     "run in interactive mode.")
    ("output-file", po::value<std::string>(&m_output_file), "output file")
    ("output-path", po::value<std::string>(&m_output_path)->default_value("."), "output path")
    ("binary", po::value<Boolean>(&m_binary)->default_value(true), "binary")
    ;
}

//! \brief Destructor.
IO_option_parser::~IO_option_parser() {}

//! \brief applies the options.
void IO_option_parser::apply() {}

SGAL_END_NAMESPACE
