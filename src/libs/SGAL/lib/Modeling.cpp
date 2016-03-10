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
const Boolean Modeling::s_def_triangulate(false);
const Boolean Modeling::s_def_refine(false);
const Boolean Modeling::s_def_fair(false);
const Boolean Modeling::s_def_split_ccs(false);

//! \brief constructs.
Modeling::Modeling(Boolean proto) :
  Container(proto),
  m_triangulate(s_def_triangulate),
  m_refine(s_def_refine),
  m_fair(s_def_fair),
  m_split_ccs(s_def_split_ccs)
{}

//! \brief initializes the node prototype.
void Modeling::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);

  // triangulate
  auto triangulate_func =
    static_cast<Boolean_handle_function>(&Modeling::triangulate_handle);
  s_prototype->add_field_info(new SF_bool(TRIANGULATE, "triangulate",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          triangulate_func, s_def_triangulate,
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

    if (name == "triangulate") {
      set_triangulate(compare_to_true(value));
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
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief sets defualt values.
void Modeling::reset(Boolean def_triangulate, Boolean def_refine,
                     Boolean def_fair, Boolean def_split_ccs)
{
  m_triangulate = def_triangulate;
  m_refine = def_refine;
  m_fair = def_fair;
  m_split_ccs = def_split_ccs;
}
SGAL_END_NAMESPACE
