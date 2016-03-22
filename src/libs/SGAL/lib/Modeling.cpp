// Copyright (c) 2016 Israel.
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

#include "SGAL/basic.hpp"
#include "SGAL/Modeling.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Modeling::s_tag = "Modeling";
Container_proto* Modeling::s_prototype(nullptr);

// Default values
const Boolean Modeling::s_def_make_consistent(false);
const Boolean Modeling::s_def_triangulate_holes(false);
const Boolean Modeling::s_def_refine(false);
const Boolean Modeling::s_def_fair(false);
const Boolean Modeling::s_def_split_ccs(false);
const Boolean Modeling::s_def_remove_degeneracies(false);
const Boolean Modeling::s_def_repair_orientation(false);

//! \brief constructs.
Modeling::Modeling(Boolean proto) :
  Container(proto),
  m_make_consistent(s_def_make_consistent),
  m_triangulate_holes(s_def_triangulate_holes),
  m_refine(s_def_refine),
  m_fair(s_def_fair),
  m_split_ccs(s_def_split_ccs),
  m_remove_degeneracies(s_def_remove_degeneracies),
  m_repair_orientation(s_def_repair_orientation)
{}

//! \brief initializes the node prototype.
void Modeling::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);

  // makeConsistent
  auto make_consistent_func =
    static_cast<Boolean_handle_function>(&Modeling::make_consistent_handle);
  s_prototype->add_field_info(new SF_bool(MAKE_CONSISTENT, "makeConsistent",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          make_consistent_func,
                                          s_def_make_consistent,
                                          exec_func));

  // triangulateHoles
  auto triangulate_holes_func =
    static_cast<Boolean_handle_function>(&Modeling::triangulate_holes_handle);
  s_prototype->add_field_info(new SF_bool(TRIANGULATE_HOLES, "triangulateHoles",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          triangulate_holes_func,
                                          s_def_triangulate_holes,
                                          exec_func));

  // refine
  auto refine_func =
    static_cast<Boolean_handle_function>(&Modeling::refine_handle);
  s_prototype->add_field_info(new SF_bool(REFINE, "refine",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          refine_func, s_def_refine,
                                          exec_func));

  // fair
  auto fair_func =
    static_cast<Boolean_handle_function>(&Modeling::fair_handle);
  s_prototype->add_field_info(new SF_bool(FAIR, "fair",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          fair_func, s_def_fair,
                                          exec_func));

  // splitCcs
  auto split_ccs_func =
    static_cast<Boolean_handle_function>(&Modeling::split_ccs_handle);
  s_prototype->add_field_info(new SF_bool(SPLIT_CCS, "splitCcs",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          split_ccs_func, s_def_split_ccs,
                                          exec_func));

  // removeDegeneracies
  auto remove_degeneracies_func =
    static_cast<Boolean_handle_function>(&Modeling::remove_degeneracies_handle);
  s_prototype->add_field_info(new SF_bool(REMOVE_DEGENERACIES,
                                          "removeDegeneracies",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          remove_degeneracies_func,
                                          s_def_remove_degeneracies,
                                          exec_func));

  // repairOrientation
  auto repair_orientation_func =
    static_cast<Boolean_handle_function>(&Modeling::repair_orientation_handle);
  s_prototype->add_field_info(new SF_bool(REPAIR_ORIENTATION,
                                          "repairOrientation",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          repair_orientation_func,
                                          s_def_repair_orientation,
                                          exec_func));
}

//! \brief deletes the node prototype.
void Modeling::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Modeling::get_prototype()
{
  if (!s_prototype) Modeling::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Modeling::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);

    if (name == "makeConsistent") {
      set_make_consistent(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "triangulateHoles") {
      set_triangulate_holes(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "refine") {
      set_refine(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fair") {
      set_fair(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "splitCcs") {
      set_split_ccs(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "removeDegeneracies") {
      set_remove_degeneracies(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "repairOrientation") {
      set_repair_orientation(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief sets defualt values.
void Modeling::reset(Boolean def_make_consistent, Boolean def_triangulate_holes,
                     Boolean def_refine, Boolean def_fair,
                     Boolean def_split_ccs, Boolean def_remove_degeneracies,
                     Boolean def_repair_orientation)
{
  m_make_consistent = def_make_consistent;
  m_triangulate_holes = def_triangulate_holes;
  m_refine = def_refine;
  m_fair = def_fair;
  m_split_ccs = def_split_ccs;
  m_remove_degeneracies = def_remove_degeneracies;
  m_repair_orientation = def_repair_orientation;
}
SGAL_END_NAMESPACE
