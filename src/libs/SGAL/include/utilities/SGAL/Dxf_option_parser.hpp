// Copyright (c) 2015 Israel.
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

#ifndef SGAL_DXF_OPTION_PARSER_HPP
#define SGAL_DXF_OPTION_PARSER_HPP

#include <boost/program_options.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class Configuration;

class SGAL_SGAL_DECL Dxf_option_parser {
public:
  /*! Construct default.
   */
  Dxf_option_parser();

  /*! Destruct.
   */
  virtual ~Dxf_option_parser();

  /*! Obtain the variable map.
   * \return the variable map.
   */
  virtual const po::variables_map& get_variable_map() const = 0;

  /*! Apply the options
   */
  void apply();

  /*! Obtain the modeling-option description.
   * \return the modeling-option description.
   */
  inline const po::options_description& get_modeling_opts() const;

  /*! Configure. */
  void configure(Configuration* conf);

protected:
  /*! The modeling options. */
  po::options_description m_dxf_opts;
};

SGAL_END_NAMESPACE

#endif
