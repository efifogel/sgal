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
#include "SGAL/Environment_map.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Shape.hpp"
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

std::string Environment_map::s_tag = "EnvironmentMap";
Container_proto* Environment_map::s_prototype(nullptr);

// Default values:
float Environment_map::m_def_alpha(0.5f);
bool Environment_map::m_def_quality(false);

REGISTER_TO_FACTORY(Environment_map, "Environment_map");

//! \brief constructor
Environment_map::Environment_map(Boolean proto) :
  Container(proto),
  m_texture(0),
  m_alpha(m_def_alpha),
  m_quality(m_def_quality)
{}

//! \brief destructor
Environment_map::~Environment_map() {}

//! \brief
void Environment_map::set_alpha(float alpha) { m_alpha = alpha; }

//! \brief
float Environment_map::get_alpha() const { return m_alpha; }

//! \brief
void Environment_map::set_texture(Texture* texture)
{ m_texture = texture; }

//! \brief
Boolean Environment_map::attach_context(Context* context)
{
  Boolean result = Container::attach_context (context);
  Texture* texture = get_texture();
  if (texture) result &= texture->attach_context(context);
  return result;
}

//! \brief
Boolean Environment_map::detach_context(Context * context)
{
  Boolean result = Container::detach_context (context);
  Texture* texture = get_texture();
  if (texture) result &= texture->detach_context(context);
  return result;
}

/*! Initialize the prototype for
 * all the node instances of Appearance in the scene graph.
 * Creates and adds a field info for each potential field.
 */
void Environment_map::init_prototype()
{
  // The prototype shuold be allocated only once for all instances
  if (s_prototype != nullptr) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto();

  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  _handle_function texture_func =
    static_cast<_handle_function>(&Environment_map::texture_handle);
  s_prototype->add_field_info(new SF_shared_container(TEXTURE, "texture",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      texture_func,
                                                      exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function alpha_func =
    static_cast<Float_handle_function>(&Environment_map::alpha_handle);
  s_prototype->add_field_info(new SF_float(ALPHA, "alpha",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           alpha_func,
                                           exec_func));
}

/*! \brief */
void Environment_map::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

/*!
 */
Container_proto * Environment_map::get_prototype()
{
  if (s_prototype == nullptr) Environment_map::init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Environment_map::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter                Str_attr_iter;

  Container::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "sgalAlpha") {
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

#if 0
/*!
 */
Attribute_list Environment_map::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  char buf[32];

  attribs = Container::get_attributes();

  if (m_alpha != m_def_alpha) {
    attrib.first = "sgalAlpha";
    sprintf(buf, "%g", m_alpha);
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_quality != m_def_quality) {
    attrib.first = "quality";
    attrib.second = is_quality() ? "TRUE" : "FALSE";
    attribs.push_back(attrib);
  }

  return attribs;
}

/*! Add the reflection map into the scene graph.
 * There are three ways an reflection map can be added.
 * -# the reflection map is defined as a child of a Shape object. In this case
 * we set the reflection map on the shape to point to this.
 * -# the reflection map is decalred at the top level of the scene graph. In
 * this case no object is assigned with this reflection map.
 *
 * In all cases the reflection map is added to the reflection map pool.
 *
 * @param sg a pointer to the scene graph
 * @param parent a pointer to the parent object. nullptr if the apperance
 * is defined in the top level.
 */
void Environment_map::add_to_scene(Scene_graph* sg, XML_entity* parent)
{
  Container::add_to_scene(sg, parent);
  sg->add_container(this);

  if (parent->get_name() == g_navigation_root_name) {
    return;
  }

  Shape * shape = dynamic_cast<Shape *>(parent);
  if (shape) {
    shape->set_env_map(this);
    return;
  }
}
#endif

SGAL_END_NAMESPACE
