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

#ifndef SGAL_CONF_OPTION_PARSER_HPP
#define SGAL_CONF_OPTION_PARSER_HPP

#include <string>
#include <vector>
#include <algorithm>

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

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class Configuration;

class SGAL_SGAL_DECL Conf_option_parser {
public:
  typedef std::vector<std::string>      Plugin;
  typedef Plugin::const_iterator        Plugin_const_iterator;

  /*! Constructor. */
  Conf_option_parser();

  /*! Destructor. */
  virtual ~Conf_option_parser();

  /*! Obtain the variable map.
   * \return the variable map.
   */
  virtual const po::variables_map& get_variable_map() const = 0;

  /*! Apply the options.
   */
  void apply();

  /*! Obtain the conf-option description.
   */
  const po::options_description& get_conf_opts() const;

  /*! Configure. */
  void configure(Configuration* conf);

  /*! Determine whether the accumulate option is set.
   */
  Boolean get_accumulate() const;

  /*! Determine whether the vertex_array option set.
   */
  Boolean get_use_vertex_array() const;

  /*! Determine whether the vertex_buffer_object option set.
   */
  Boolean get_use_vertex_buffer_object() const;

  /*! Determine whether the map_texture option set.
   */
  Boolean get_map_texture() const;

  /*! Determine whether the display_fps option set.
   */
  Boolean get_display_fps(Boolean& flag);

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
  /*! The conf options. */
  po::options_description m_conf_opts;

private:
  /*! Indicates whether to accumulate. */
  Boolean m_accumulate;

  /*! Indicates whether to use openGl vertex-array. */
  Boolean m_use_vertex_array;

  /*! Indicates whether to use openGl vertex-buffer-object. */
  Boolean m_use_vertex_buffer_object;

  /*! Indicates whether to apply texture mapping. */
  Boolean m_map_texture;

  /*! Indicate whether to display Frames-Per-Seconds (FPS). */
  Boolean m_display_fps;

  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  Conf_option_parser& operator=(const Conf_option_parser&);
  Conf_option_parser(const Conf_option_parser&);

  // In C++11, VC2013, the following is supported:
  // Conf_option_parser& operator=(const Conf_option_parser&) = delete;
  // Conf_option_parser(const Conf_option_parser&) = delete;
};

//! \brief obtains the conf-option description.
inline const po::options_description&
Conf_option_parser::get_conf_opts() const { return m_conf_opts; }

//! \brief determines whether the accumulate option is set.
inline Boolean Conf_option_parser::get_accumulate() const
{ return m_accumulate; }

//! \brief determines whether the vertex_array option set.
inline Boolean Conf_option_parser::get_use_vertex_array() const
{ return m_use_vertex_array; }

//! \brief determines whether the vertex_buffer_object option set.
inline Boolean Conf_option_parser::get_use_vertex_buffer_object() const
{ return m_use_vertex_buffer_object; }

//! \brief determines whether the map_texture option set.
inline Boolean Conf_option_parser::get_map_texture() const
{ return m_map_texture; }

//! \brief obtains the begin iterator of input paths.
inline Conf_option_parser::Plugin_const_iterator
Conf_option_parser::plugins_begin()
{ return get_variable_map()["load"].as<Plugin>().begin(); }

//! \broef obtains the pass-the-end iterator of input paths.
inline Conf_option_parser::Plugin_const_iterator
Conf_option_parser::plugins_end()
{ return get_variable_map()["load"].as<Plugin>().end(); }

//! \brief applies a given function object to all input paths.
template <typename UnaryFunction>
inline UnaryFunction Conf_option_parser::for_each_plugin(UnaryFunction func)
{
  if (!get_variable_map().count("load")) return func;
  return std::for_each(plugins_begin(), plugins_end(), func);
}

SGAL_END_NAMESPACE

#endif
