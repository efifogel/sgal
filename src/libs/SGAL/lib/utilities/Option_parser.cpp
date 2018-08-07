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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <boost/any.hpp>
#include <boost/filesystem.hpp>

#include <iterator>

#include "SGAL/Option_parser.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Configuration.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

//! \brief constructs.
Option_parser::Option_parser() :
  m_config_opts("SGAL configuration options"),
  m_hidden_opts("SGAL hidden options"),
  m_quite(true),
  m_verbose(0)
{
  typedef std::vector<std::string> vs;

  // Extract trace options:
  auto* tracer = Trace::get_instance();
  std::string trace_msg("trace options:\n");
  for (const auto& trace_opt : tracer->get_options())
    trace_msg += "  " + trace_opt.first + "\n";

  // Options allowed on the command line, config file, or env. variables
  // We must keep the trace-options as strings, and NOT convert them on the fly,
  // using, e.g., a validator, to trace codes, because at the time that the
  // program options are parsed, we do not know all trace codes yet. Some trace
  // codes might be registered by a dynamically loaded library, which is loaded
  // after the program options are parsed.
  m_config_opts.add_options()
    ("quite,q", po::value<Boolean>(&m_quite)->default_value(true),
     "quite mode")
    ("verbose,v", po::value<Uint>(&m_verbose)->
     default_value(Configuration::s_def_verbose_level),
     "verbose level")
    ("trace,T", po::value<vs>()->composing(), trace_msg.c_str())
    ("load,L", po::value<vs>()->composing(), "load a library")
    ;

  // Options hidden to the user. Allowed only on the command line:
  m_hidden_opts.add_options()
    ("input-file", po::value<vs>(), "input file")
    ;

  // m_visible_opts.set_name("Allowed options");
  m_config_opts.add(m_conf_opts).add(m_modeling_opts).add(m_dxf_opts);
  m_visible_opts.add(m_generic_opts).add(m_config_opts).add(m_io_opts).add(m_bench_opts);
  m_cmd_line_opts.add(m_visible_opts).add(m_hidden_opts);
  m_config_file_opts.add(m_config_opts).add(m_io_opts).add(m_bench_opts);
  m_environment_opts.add(m_config_opts).add(m_io_opts).add(m_bench_opts);

  m_positional_opts.add("input-file", -1);
}

//! \brief parses the options.
void Option_parser::operator()(Int32 argc, Char* argv[])
{
  std::ifstream ifs;
  po::store(po::command_line_parser(argc, argv).
            options(m_cmd_line_opts).positional(m_positional_opts).run(),
            m_variable_map);

  static std::string config_file_name(".sgal.cfg");
  if (boost::filesystem::exists(config_file_name))
    ifs.open(config_file_name);
  else {
    const char* home = getenv("HOME");
    if (home) {
      std::string home_str(home);
#if (defined _MSC_VER)
      // Convert the HOME from cygwin path to windows path, if relevant:
      std::string cygdrive = home_str.substr(0, 10);
      if (cygdrive == std::string("/cygdrive/")) {
        home_str.erase(0, 10);
        home_str.insert(1, ":");
      }
#endif
      fi::path home_path(home_str);
      home_path /= config_file_name;
      ifs.open(home_path.string());
    }
  }
  if (ifs.is_open()) {
    po::store(po::parse_config_file(ifs, m_config_file_opts), m_variable_map);
  }
  po::notify(m_variable_map);
}

//! \brief applies the options.
void Option_parser::apply()
{
  Generic_option_parser::apply();
  Conf_option_parser::apply();
  Modeling_option_parser::apply();
  Dxf_option_parser::apply();
  IO_option_parser::apply();
  Bench_option_parser::apply();
}

//! \brief configures the scene graph.
void Option_parser::configure(Scene_graph* scene_graph)
{
  typedef std::vector<std::string> vs;
  if (m_variable_map.count("trace")) {
    auto& traces = m_variable_map["trace"].as<vs>();
    for (auto it = traces.begin(); it != traces.end(); ++it) {
      auto code = Trace::get_instance()->find_code(*it);
      if (code == Trace::INVALID) {
        throw po::validation_error(po::validation_error::invalid_option_value,
                                   "--trace", *it);
        continue;
      }
      Trace::get_instance()->enable(code);
    }
  }

  if (!scene_graph) return;
  auto* conf = scene_graph->get_configuration();
  if (conf) {
    conf->set_verbose_level(m_variable_map["verbose"].as<Uint>());

    Conf_option_parser::configure(conf);
    Modeling_option_parser::configure(conf);
    Dxf_option_parser::configure(conf);
    IO_option_parser::configure(conf);
  }
}

SGAL_END_NAMESPACE
