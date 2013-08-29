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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

std::string Sphere_environment::s_tag = "sphericalEnvironmentMap";
Container_proto* Sphere_environment::s_prototype(NULL);

// Default values:
const Float Sphere_environment::m_def_alpha(0.5f);
const Boolean Sphere_environment::m_def_quality(false);

/*! Constructor */
Sphere_environment::Sphere_environment(Boolean proto) :
  Texture(proto),
  m_alpha(m_def_alpha),
  m_quality(m_def_quality)
{}

/*! Destructor */
Sphere_environment::~Sphere_environment() {}

/*! \brief initializes the prototype. */
void Sphere_environment::init_prototype()
{
  if (s_prototype != NULL) return;
  s_prototype = new Container_proto(Texture::get_prototype());

  // alpha
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function alpha_func =
    static_cast<Float_handle_function>(&Sphere_environment::alpha_handle);
  s_prototype->add_field_info(new SF_float(ALPHA, "alpha", alpha_func,
                                           exec_func));
}

/*! \brief deletes the prototype. */
void Sphere_environment::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Sphere_environment::get_prototype()
{
  if (s_prototype == NULL) Sphere_environment::init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Sphere_environment::set_attributes(Element* elem)
{
  Texture::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;

  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "alpha") {
      set_alpha(atoff(value.c_str()));
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
Attribute_list Sphere_environment::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  char buf[32];

  attribs = Texture::get_attributes();

  if (m_alpha != m_def_alpha) {
    attrib.first = "alpha";
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
 * @param parent a pointer to the parent object. NULL if the apperance
 * is defined in the top level.
 */
void Sphere_environment::add_to_scene(Scene_graph* sg)
{
  Container::add_to_scene(sg, parent);
  sg->add_container(this);
  if (parent->get_name() == g_navigation_root_name) return;
  Shape* shape = dynamic_cast<Shape *>(parent);
  if (shape) {
    shape->set_env_map(this);
    return;
  }
}
#endif

/*! \brief cleans the object in case it is dirty. */
void Sphere_environment::clean()
{
  //! \todo implement
}

/*! \brief determines whether the texture ios empty. */
Boolean Sphere_environment::empty()
{
  if (!m_images[0].first || !m_images[1].first) return true;
  if (m_images[0].first->is_dirty()) m_images[0].first->clean();
  if (m_images[1].first->is_dirty()) m_images[1].first->clean();
  return (m_images[0].first->empty() || m_images[1].first->empty());
}

/*! \brief obtains the texture number of components. */
Uint Sphere_environment::get_component_count() const
{
  if (!m_images[0].first) return 0;
  if (m_images[0].first->is_dirty()) m_images[0].first->clean();
  return m_images[0].first->get_component_count();
}

/*! \brief adds the container to a given scene. */
void Sphere_environment::add_to_scene(Scene_graph* scene_graph)
{
  if (!m_images[0].first || !m_images[1].first) return;
  m_images[0].first->set_dirs(scene_graph->get_data_dirs());
  m_images[1].first->set_dirs(scene_graph->get_data_dirs());
}

SGAL_END_NAMESPACE
