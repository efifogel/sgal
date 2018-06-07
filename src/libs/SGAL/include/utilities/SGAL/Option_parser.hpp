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

#ifndef SGAL_OPTION_PARSER_HPP
#define SGAL_OPTION_PARSER_HPP

#include <vector>
#include <string>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512 )
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
#include "SGAL/Modeling_option_parser.hpp"
#include "SGAL/IO_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class Scene_graph;

class SGAL_SGAL_DECL Option_parser : public Generic_option_parser,
                                     public Bench_option_parser,
                                     public Conf_option_parser,
                                     public IO_option_parser,
                                     public Modeling_option_parser
{
public:
  typedef std::vector<std::string>              Plugin;
  typedef Plugin::const_iterator                Plugin_const_iterator;

  /*! Construct default. */
  Option_parser();

  /*! Parse the options.
   * \param[in] argc
   * \param[in] argv
   */
  void operator()(Int32 argc, Char* argv[]);

  /*! Apply the options.
   */
  void apply();

  /*! Add options to command line options.
   * \param options
   */
  void add_command_line_options(po::options_description& options);

  /*! Add options to config file options.
   * \param options
   */
  void add_config_file_options(po::options_description& options);

  /*! Add options to environment variable options.
   * \param options
   */
  void add_environment_options(po::options_description& options);

  /*! Add options to visible options.
   * \param options
   */
  void add_visible_options(po::options_description& options);

  /*! Configure the scene graph.
   * \param conf The scene graph.
   */
  void configure(Scene_graph* scene_graph);

  /* Obtain the 'quite' mode */
  Boolean get_quite_mode() const { return m_quite; }

  /*! Obtain the verbose level */
  Uint get_verbosity_level() const { return m_verbose; }

  /*! Obtain the variable map */
  virtual const po::variables_map& get_variable_map() const;

  /*! Obtain the begin iterator of plugins.
   */
  Plugin_const_iterator plugins_begin();

  /*! Obtain the pass-the-end iterator of plugins.
   */
  Plugin_const_iterator plugins_end();

  /*! Apply a given function object to all plugins.
   */
  template <typename UnaryFunction>
  UnaryFunction for_each_plugin(UnaryFunction func);

protected:
  //! Command line options.
  po::options_description m_cmd_line_opts;

  //! Config file options.
  po::options_description m_config_file_opts;

  //! Environment variable options.
  po::options_description m_environment_opts;

  //! Visible options.
  po::options_description m_visible_opts;

  //! The configuration option description.
  po::options_description m_config_opts;

  //! The hidden option description.
  po::options_description m_hidden_opts;

  //! Positional option description.
  po::positional_options_description m_positional_opts;

  //! The variable map.
  po::variables_map m_variable_map;

#if 0
  std::pair<Char*, Char*> m_env_var_option_names;

  const std::string& name_mapper<std::string, std::string>(std::string& src) { }
#endif

  /*! Obtain the visible options.
   */
  virtual const po::options_description& get_visible_opts() const
  { return m_visible_opts; }

private:
  /*! Indicates whether to run in quite mode */
  Boolean m_quite;

  /*! Verbose level */
  Uint m_verbose;
};

//! \brief adds options to command line options.
inline
void Option_parser::add_command_line_options(po::options_description& options)
{ m_cmd_line_opts.add(options); }

//! \brief adds options to config file options.
inline
void Option_parser::add_config_file_options(po::options_description& options)
{ m_config_file_opts.add(options); }

//! \brief adds options to environment variable options.
inline
void Option_parser::add_environment_options(po::options_description& options)
{ m_environment_opts.add(options); }

//! \brief adds options to visible options.
inline void Option_parser::add_visible_options(po::options_description& options)
{ m_visible_opts.add(options); }

//! \brief obtains the variable map.
inline const po::variables_map& Option_parser::get_variable_map() const
{ return m_variable_map; }

//! \brief obtains the begin iterator of input paths.
inline Option_parser::Plugin_const_iterator
Option_parser::plugins_begin()
{ return get_variable_map()["load"].as<Plugin>().begin(); }

//! \broef obtains the pass-the-end iterator of input paths.
inline Option_parser::Plugin_const_iterator
Option_parser::plugins_end()
{ return get_variable_map()["load"].as<Plugin>().end(); }

//! \brief applies a given function object to all input paths.
template <typename UnaryFunction>
inline UnaryFunction Option_parser::for_each_plugin(UnaryFunction func)
{
  if (!get_variable_map().count("load")) return func;
  return std::for_each(plugins_begin(), plugins_end(), func);
}

SGAL_END_NAMESPACE

#endif
