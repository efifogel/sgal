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
// $Source: $
// $Revision: 12358 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <boost/any.hpp>
  
#include <iterator>

#include "SGAL/Option_parser.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Configuration.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

const Char* Option_parser::s_trace_opts[] = {
  "graphics",
  "vrml_parsing",
  "sim",
  "event",
  "ifs",
  "polyhedron",
  "cgm"
};

/*! Obtain number of trace options */
Uint Option_parser::number_trace_opts()
{
  return sizeof(s_trace_opts) / sizeof(char *);
}

/* Overload the 'validate' function for the user-defined class */
void validate(boost::any& v, const std::vector<std::string> & values,
              SGAL::Option_parser::Vector_trace_id* /* target_type */, int)
{
  typedef Option_parser::Trace_id               Trace_id;
  typedef Option_parser::Vector_trace_id        Vector_trace_id;
  
  for (Uint i = 0; i < Option_parser::number_trace_opts(); ++i) {
    if (Option_parser::compare_trace_opt(i, values[0].c_str())) {
      if (v.empty()) {
        Vector_trace_id vec;
        vec.push_back(Trace_id(i));
        v = boost::any(vec);
      } else {
        Vector_trace_id vec = boost::any_cast<Vector_trace_id>(v);
        vec.push_back(Trace_id(i));
        v = boost::any(vec);
      }
      return;
    }
  }
#if BOOST_VERSION >= 104200
  throw po::validation_error(po::validation_error::invalid_option_value);
#else
  throw po::validation_error("invalid value");
#endif
}

/*! Constructor */
Option_parser::Option_parser() :
  m_config_opts("SGAL configuration options"),
  m_hidden_opts("SGAL hidden options"),
  m_quite(true),
  m_verbose(0)
{
  typedef std::vector<std::string> vs;

  // Options allowed on the command line, config file, or env. variables
  m_config_opts.add_options()
    ("quite,q", po::value<Boolean>(&m_quite)->default_value(true),
     "quite mode")
    ("verbose,v", po::value<Uint>(&m_verbose)->default_value(0),
     "verbose level")
    ("input-path,I", po::value<vs>()->composing(), "input path")
    ("trace,T", po::value<std::vector<Trace_id> >()->composing(),
     "trace options\n"
     "  graphics\n"
     "  vrml_parsing\n"
     "  sim\n"
     "  event\n"
     "  ifs\n"
     "  polyhedron\n"
     "  cgm\n"
     )
    ;

  // Options hidden to the user. Allowed only on the command line:
  m_hidden_opts.add_options()
    ("input-file", po::value<vs>(), "input file")
    ;

  // m_visible_opts.set_name("Allowed options");
  m_config_opts.add(m_conf_opts);
  m_visible_opts.add(m_generic_opts).add(m_config_opts).add(m_bench_opts);
  m_cmd_line_opts.add(m_visible_opts).add(m_hidden_opts);
  m_config_file_opts.add(m_config_opts).add(m_bench_opts);
  m_environment_opts.add(m_config_opts).add(m_bench_opts);

  m_positional_opts.add("input-file", -1);
}

/*! \brief parses the options */
void Option_parser::operator()(Int argc, Char* argv[])
{
  po::store(po::command_line_parser(argc, argv).
            options(m_cmd_line_opts).positional(m_positional_opts).run(),
            m_variable_map);
  
  std::ifstream ifs(".sgal.cfg");
  po::store(po::parse_config_file(ifs, m_config_file_opts), m_variable_map);
  po::notify(m_variable_map);

  // std::pair<Char*,Char*> Option_parser::s_env_var_option_names[] = {
  // {"SGAL_QUITE", "quite"},
  // {"SGAL_VERBOSE", "verbose"}
  // };
}

/*! \brief applies the options */
void Option_parser::apply()
{
  Generic_option_parser::apply(m_variable_map);
  Bench_option_parser::apply(m_variable_map);
  Conf_option_parser::apply(m_variable_map);

  if (m_variable_map.count("trace")) {
    Vector_trace_id traces = m_variable_map["trace"].as<Vector_trace_id>();
    for (Vector_trace_id_iter it = traces.begin(); it != traces.end(); ++it) {
      Trace::get_instance()->enable(static_cast<Trace::Code>((*it).m_id));
    }
  }
}

/*! Configure */
void Option_parser::configure(Scene_graph* scene_graph)
{
  if (!scene_graph) return;
  
  SGAL::Configuration* conf = scene_graph->get_configuration();
  Conf_option_parser::configure(m_variable_map, conf);
}

SGAL_END_NAMESPACE
