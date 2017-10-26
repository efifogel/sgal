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
  m_make_consistent(false),
  m_triangulate_holes(false),
  m_refine(false),
  m_fair(false),
  m_split_ccs(false),
  m_remove_degeneracies(false),
  m_repair_orientation(false),
  m_repair_normals(false)
{
  m_modeling_opts.add_options()
    ("make-consistent",
     po::value<Boolean>(&m_make_consistent),
     "make the orientation of facets consistent")
    ("triangulate-holes",
     po::value<Boolean>(&m_triangulate_holes), "triangulate holes")
    ("refine", po::value<Boolean>(&m_refine), "refine hole triangulation")
    ("fair", po::value<Boolean>(&m_fair), "fair hole triangulation")
    ("split-ccs", po::value<Boolean>(&m_split_ccs), "split connected components")
    ("remove-degeneracies", po::value<Boolean>(&m_remove_degeneracies),
     "remove shape children the geometry of which is degenerate (zero volume)")
    ("repair-orientation", po::value<Boolean>(&m_repair_orientation),
     "repair the orientation of facets reversing them all")
    ("repair-normals", po::value<Boolean>(&m_repair_normals),
     "repair the normals of facets")
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

  if (var_map.count("make-consistent"))
    modeling->set_make_consistent(var_map["make-consistent"].as<Boolean>());

  if (var_map.count("triangulate-holes"))
    modeling->set_triangulate_holes(var_map["triangulate-holes"].as<Boolean>());

  if (var_map.count("refine"))
    modeling->set_refine(var_map["refine"].as<Boolean>());

  if (var_map.count("fair"))
    modeling->set_fair(var_map["fair"].as<Boolean>());

  if (var_map.count("split-ccs"))
    modeling->set_split_ccs(var_map["split-ccs"].as<Boolean>());

  if (var_map.count("remove-degeneracies"))
    modeling->set_remove_degeneracies(var_map["remove-degeneracies"].as<Boolean>());

  if (var_map.count("repair-orientation"))
    modeling->set_repair_orientation(var_map["repair-orientation"].as<Boolean>());

  if (var_map.count("repair-normals"))
    modeling->set_repair_normals(var_map["repair-normals"].as<Boolean>());
}

SGAL_END_NAMESPACE
