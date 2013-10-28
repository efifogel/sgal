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
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_OPTION_PARSER_HPP
#define SGAL_OPTION_PARSER_HPP

#include <vector>
#include <string>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 )
#endif
#include <boost/program_options.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Generic_option_parser.hpp"
#include "SGAL/Bench_option_parser.hpp"
#include "SGAL/Conf_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class Scene_graph;

class SGAL_SGAL_DECL Option_parser : public Generic_option_parser,
                                     public Bench_option_parser,
                                     public Conf_option_parser
{
public:
  /*! Trace id */
  struct Trace_id {
    Trace_id(Uint id) : m_id(id) {}
    Uint m_id;
  };

  typedef std::vector<Trace_id>         Vector_trace_id;
  typedef Vector_trace_id::iterator     Vector_trace_id_iter;

  struct Input_file_missing_error : public po::error {
    Input_file_missing_error(std::string & str) : error(str) {}
  };

  /*! Constructor */
  Option_parser();

  /*! Parse the options
   * \param argc
   * \param argv
   */
  void operator()(Int argc, Char * argv[]);

  /*! Apply the options
   */
  void apply();

  /*! Configure */
  void configure(Scene_graph * scene_graph);

  /*! Add options to command line options */
  void add_command_line_options(po::options_description & options)
  {
    m_cmd_line_opts.add(options);
  }

  /*! Add options to config file options */
  void add_config_file_options(po::options_description & options)
  {
    m_config_file_opts.add(options);
  }

  /*! Add options to environment variable options */
  void add_environment_options(po::options_description & options)
  {
    m_environment_opts.add(options);
  }

  /*! Add options to visible options */
  void add_visible_options(po::options_description & options)
  {
    m_visible_opts.add(options);
  }

  /* Obtain the 'quite' mode */
  Boolean get_quite_mode() const { return m_quite; }

  /*! Obtain the verbose level */
  Uint get_verbosity_level() const { return m_verbose; }

  /*! Obtain number of trace options */
  static Uint number_trace_opts();

  /*! Compare the i-th option to a given option */
  static Boolean compare_trace_opt(Uint i, const Char * opt)
  { return strcmp(s_trace_opts[i], opt) == 0; }

  /*! Obtain the variable map */
  const po::variables_map & get_variable_map() const { return m_variable_map; }

protected:
  /*! Command line options */
  po::options_description m_cmd_line_opts;

  /*! Config file options */
  po::options_description m_config_file_opts;

  /*! Environment variable options */
  po::options_description m_environment_opts;

  /*! Visible options */
  po::options_description m_visible_opts;

  /*! The configuration option description */
  po::options_description m_config_opts;

  /*! The hidden option description */
  po::options_description m_hidden_opts;

  /*! Positional option description */
  po::positional_options_description m_positional_opts;

  /*! The variable map */
  po::variables_map m_variable_map;

#if 0
  std::pair<Char*,Char*> m_env_var_option_names;

  const std::string & name_mapper<std::string, std::string>(std::string & src)
  {
  }
#endif

  virtual const po::options_description & get_visible_opts() const
  { return m_visible_opts; }

private:
  /*! Trace options */
  static const char * s_trace_opts[];

  /*! Indicates whether to run in quite mode */
  Boolean m_quite;

  /*! Verbose level */
  Uint m_verbose;
};

SGAL_END_NAMESPACE

#endif
