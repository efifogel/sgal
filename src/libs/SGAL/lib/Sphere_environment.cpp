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

#include <algorithm>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Sphere_environment::s_tag = "sphericalEnvironmentMap";
Container_proto* Sphere_environment::s_prototype(nullptr);

// Default values:
const Float Sphere_environment::m_def_alpha(0.5f);
const Boolean Sphere_environment::m_def_quality(false);

//! \brief constructor.
Sphere_environment::Sphere_environment(Boolean proto) :
  Texture(proto),
  m_alpha(m_def_alpha),
  m_quality(m_def_quality)
{}

//! \brief destructor.
Sphere_environment::~Sphere_environment() {}

//! \brief initializes the prototype.
void Sphere_environment::init_prototype()
{
  if (s_prototype != nullptr) return;
  s_prototype = new Container_proto(Texture::get_prototype());

  // alpha
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function alpha_func =
    static_cast<Float_handle_function>(&Sphere_environment::alpha_handle);
  s_prototype->add_field_info(new SF_float(ALPHA,
                                           "alpha",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           alpha_func,
                                           exec_func));
}

//! \brief deletes the prototype.
void Sphere_environment::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Sphere_environment::get_prototype()
{
  if (s_prototype == nullptr) Sphere_environment::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this container.
void Sphere_environment::set_attributes(Element* elem)
{
  Texture::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "alpha") {
      set_alpha(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quality") {
      set_quality(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief cleans the object in case it is dirty.
void Sphere_environment::clean()
{
  //! \todo implement
}

//! \brief determines whether the texture ios empty.
Boolean Sphere_environment::empty()
{
  if (!m_images[0].first || !m_images[1].first) return true;
  if (m_images[0].first->is_dirty()) m_images[0].first->clean();
  if (m_images[1].first->is_dirty()) m_images[1].first->clean();
  return (m_images[0].first->empty() || m_images[1].first->empty());
}

//! \brief obtains the texture number of components.
Uint Sphere_environment::get_component_count() const
{
  if (!m_images[0].first) return 0;
  if (m_images[0].first->is_dirty()) m_images[0].first->clean();
  return m_images[0].first->get_component_count();
}

SGAL_END_NAMESPACE
