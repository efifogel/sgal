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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/IO_option_parser.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Image_format.hpp"
#include "SGAL/Geometry_format.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
IO_option_parser::IO_option_parser() :
  m_io_opts("SGAL IO options"),
  m_snapshot(false),
  m_export(false),
  m_interactive(true),
  m_binary(true),
  m_export_scene_root(2),
  m_export_non_visible(false)
{
  typedef std::vector<std::string> vs;

  // Extract geometry file format options:
  auto* gf = Geometry_format::get_instance();
  std::string gff_msg("geometry file formats:\n");
  for (auto it = gf->names_begin(); it != gf->names_end(); ++it)
    if (! it->empty()) gff_msg += "  " + *it + "\n";

  // Extract image file format options:
  auto* if_ = Image_format::get_instance();
  std::string iff_msg("image file formats:\n");
  for (auto it = if_->names_begin(); it != if_->names_end(); ++it)
    if (! it->empty()) iff_msg += "  " + *it + "\n";

  // Options allowed on the command line, config file, or env. variables
  m_io_opts.add_options()
    ("input-path", po::value<vs>()->composing(), "input path")
    ("image-format", po::value<File_formats>(&m_image_formats)->multitoken(),
     iff_msg.c_str())
    ("geometry-format,f", po::value<File_formats>(&m_geometry_formats)->multitoken(),
     gff_msg.c_str())
    ("snapshot,S", po::value<Boolean>(&m_snapshot)->default_value(false),
     "snapshot")
    ("export,E", po::value<Boolean>(&m_export)->default_value(false), "export")
    ("interactive,I", po::value<Boolean>(&m_interactive)->default_value(true),
     "run in interactive mode.")
    ("output-file", po::value<std::string>(&m_output_file), "output file")
    ("output-path", po::value<std::string>(&m_output_path)->default_value("."),
     "output path")
    ("binary", po::value<Boolean>(&m_binary)->default_value(true), "binary")
    ("export-scene-root", po::value<Uint>(&m_export_scene_root),
     "The root of the scene to export; (0, 1, ...")
    ("export-non-visible", po::value<Boolean>(&m_export_non_visible),
     "export non-visible geometries")
    ;
}

//! \brief Destructor.
IO_option_parser::~IO_option_parser() {}

//! \brief applies the options.
void IO_option_parser::apply() {}

//! \brief sets the Configuration node.
void IO_option_parser::configure(Configuration* conf)
{
  if (!conf) return;

  const auto& var_map = get_variable_map();

  if (var_map.count("export-scene-root"))
    conf->set_export_scene_root(var_map["export-scene-root"].as<Uint>());

  if (var_map.count("export-non-visible")) {
    conf->set_export_non_visible(var_map["export-non-visible"].as<Boolean>());
    conf->set_override_export_non_visible(true);
  }
}

SGAL_END_NAMESPACE
