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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CONF_OPTION_PARSER_HPP
#define SGAL_CONF_OPTION_PARSER_HPP

#include <boost/program_options.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class Configuration;

class Conf_option_parser {
public:
  /*! Constructor */
  Conf_option_parser();

  /*! Destructor */
  virtual ~Conf_option_parser();

  /*! Apply the options
   * \param variable_map
   */
  void apply(po::variables_map & variable_map);
  
  /*! Obtain the conf-option description */
  const po::options_description & get_conf_opts() const { return m_conf_opts; }

  /*! Configure */
  void configure(po::variables_map & variable_map, Configuration * conf);
  
  /*! Is the accumulate option set? */
  Boolean get_accumulate() const { return m_accumulate; }
  
  /*! Is the vertex_array option set? */
  Boolean get_use_vertex_array() const { return m_use_vertex_array; }
  
  /*! Is the vertex_buffer_object option set? */
  Boolean get_use_vertex_buffer_object() const
  { return m_use_vertex_buffer_object; }

  /*! Is the map_texture option set? */
  Boolean get_map_texture() const { return m_map_texture; }

  /*! Is the display_fps option set? */
  Boolean get_display_fps(Boolean & flag);
  
protected:
  /*! The conf options */
  po::options_description m_conf_opts;

private:
  /*! Indicates whether to accumulate */
  Boolean m_accumulate;

  /*! Indicates whether to use openGl vertex-array */
  Boolean m_use_vertex_array;

  /*! Indicates whether to use openGl vertex-buffer-object */
  Boolean m_use_vertex_buffer_object;

  /*! Indicates whether to apply texture mapping */
  Boolean m_map_texture;

  /*! Indicate whether to display Frames-Per-Seconds (FPS) */
  Boolean m_display_fps;
};

SGAL_END_NAMESPACE

#endif
