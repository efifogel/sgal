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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include <vector>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Modeling_option_parser.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Modeling.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs default.
Modeling_option_parser::Modeling_option_parser() :
  m_modeling_opts("Modeling options"),
  m_triangulate(false),
  m_refine(false),
  m_fair(false)
{
  m_modeling_opts.add_options()
    ("triangulate", po::value<Boolean>(&m_triangulate)->default_value(false), "triangulate holes")
    ("refine hole triangulation", po::value<Boolean>(&m_triangulate)->default_value(false), "refine")
    ("fair hole triangulation", po::value<Boolean>(&m_triangulate)->default_value(false), "fair")
    ;
}

//! \brief destructs.
Modeling_option_parser::~Modeling_option_parser() {}

//! \brief applies the options.
void Modeling_option_parser::apply() {}

//! \brief sets the Configuration node.
void Modeling_option_parser::configure(Configuration* conf)
{
  if (!conf) return;

  const auto& var_map = get_variable_map();

  auto modeling = conf->get_modeling();
  if (!modeling) {
    modeling.reset(new Modeling);
    SGAL_assertion(modeling);
    conf->set_modeling(modeling);
  }

  if (var_map.count("triangulate"))
    modeling->set_triangulate(var_map["triangulate"].as<Boolean>());

  if (var_map.count("refine"))
    modeling->set_refine(var_map["refine"].as<Boolean>());

  if (var_map.count("fair"))
    modeling->set_fair(var_map["fair"].as<Boolean>());
}

SGAL_END_NAMESPACE
