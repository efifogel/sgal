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

#ifndef SGAL_CONF_OPTION_PARSER_HPP
#define SGAL_CONF_OPTION_PARSER_HPP

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
#include "SGAL/Configuration.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class SGAL_SGAL_DECL Conf_option_parser {
public:
  /*! Construct default. */
  Conf_option_parser();

  /*! Destruct. */
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

protected:
  //! The conf options.
  po::options_description m_conf_opts;

private:
  //! Indicates whether to accumulate.
  Boolean m_accumulate;

  //! Indicates whether to use openGl vertex-array.
  Boolean m_use_vertex_array;

  //! Indicates whether to use openGl vertex-buffer-object.
  Boolean m_use_vertex_buffer_object;

  //! Indicates whether to apply texture mapping.
  Boolean m_map_texture;

  //! Indicate whether to display Frames-Per-Seconds (FPS).
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

//! Import 3D (graphics) file formats
template <typename InputStream>
InputStream& operator>>(InputStream& in, Configuration::Viewpoint_mode& mode)
{
  std::string token;
  in >> token;
  for (size_t i = 0; i < Configuration::NUM_VIEWPOINT_MODES; ++i) {
    if (!Configuration::compare_viewpoint_mode_name(i, token.c_str())) continue;
    mode = static_cast<Configuration::Viewpoint_mode>(i);
    return in;
  }
  throw po::validation_error(po::validation_error::invalid_option_value);
  return in;
}

SGAL_END_NAMESPACE

#endif
