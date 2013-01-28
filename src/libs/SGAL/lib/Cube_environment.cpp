// Copyright (c) 2013 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

std::string Cube_environment::s_tag = "CubeEnvironment";
Container_proto* Cube_environment::s_prototype = NULL;

// Default values:

/*! Constructor */
Cube_environment::Cube_environment(Boolean proto) :
  Texture(proto)
{ set_target(TEXTURE_CUBE_MAP); }

/*! Destructor */
Cube_environment::~Cube_environment() {}

/*! \brief initializes the prototype. */
void Cube_environment::init_prototype()
{
  // The prototype shuold be allocated only once for all instances
  if (s_prototype != NULL) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto(Texture::get_prototype());
}

/*! \brief deletes the prototype. */
void Cube_environment::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Cube_environment::get_prototype() 
{  
  if (s_prototype == NULL) Cube_environment::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the cubical environment map. */
void Cube_environment::set_attributes(Element* elem)
{
  Texture::set_attributes(elem);

  // typedef Element::Str_attr_iter          Str_attr_iter;
  // Str_attr_iter ai;
  // for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
  //   const std::string& name = elem->get_name(ai);
  //   const std::string& value = elem->get_value(ai);
  // }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "leftImage") {
      Image* image = dynamic_cast<Image*>(cont);
      set_left_image(image);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "rightImage") {
      Image* image = dynamic_cast<Image*>(cont);
      set_right_image(image);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "frontImage") {
      Image* image = dynamic_cast<Image*>(cont);
      set_front_image(image);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "backImage") {
      Image* image = dynamic_cast<Image*>(cont);
      set_back_image(image);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "bottomImage") {
      Image* image = dynamic_cast<Image*>(cont);
      set_bottom_image(image);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "topImage") {
      Image* image = dynamic_cast<Image*>(cont);
      set_top_image(image);
      elem->mark_delete(cai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene. */  
void Cube_environment::add_to_scene(Scene_graph* scene_graph)
{
  for (Uint i = 0; i < NUM_IMAGES; ++i) {
    if (!m_images[i].first) return;
    m_images[i].first->set_dirs(scene_graph->get_data_dirs());
  }
}

/*! \brief cleans the cube environment object. */
void Cube_environment::clean()
{
  static GLenum targets[6] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  };
  if (Texture::is_dirty()) Texture::clean();
  for (Uint i = 0; i < NUM_IMAGES; ++i) {
    if (m_images[i].first->is_dirty()) m_images[i].first->clean();
    load_color_map(m_images[i].first, targets[i]);
  }
  m_dirty = false;
}

/*! \brief determines whether the texture ios empty. */
Boolean Cube_environment::empty()
{
  for (Uint i = 0; i < NUM_IMAGES; ++i) {
    if (!m_images[i].first) return true;
    if (m_images[i].first->is_dirty()) m_images[i].first->clean();
    if (m_images[i].first->empty()) return true;
  }
  return false;
}

/*! \brief obtains the texture number of components. */
Uint Cube_environment::get_component_count() const
{
  if (!m_images[0].first) return 0;
  if (m_images[0].first->is_dirty()) m_images[0].first->clean();
  return m_images[0].first->get_component_count();
}

/*! \brief sets the left image. */
void Cube_environment::set_left_image(Image* image, Boolean owned)
{
  if (m_images[LEFT_IMAGE].second && m_images[LEFT_IMAGE].first)
    delete m_images[LEFT_IMAGE].first;
  m_images[LEFT_IMAGE].first = image;
  m_images[LEFT_IMAGE].second = owned; 
}  

/*! \brief sets the right image. */
void Cube_environment::set_right_image(Image* image, Boolean owned)
{
  if (m_images[RIGHT_IMAGE].second && m_images[RIGHT_IMAGE].first)
    delete m_images[RIGHT_IMAGE].first;
  m_images[RIGHT_IMAGE].first = image;
  m_images[RIGHT_IMAGE].second = owned; 
}  

/*! \brief sets the front image. */
void Cube_environment::set_front_image(Image* image, Boolean owned)
{
  if (m_images[FRONT_IMAGE].second && m_images[FRONT_IMAGE].first)
    delete m_images[FRONT_IMAGE].first;
  m_images[FRONT_IMAGE].first = image;
  m_images[FRONT_IMAGE].second = owned; 
}  

/*! \brief sets the back image. */
void Cube_environment::set_back_image(Image* image, Boolean owned)
{
  if (m_images[BACK_IMAGE].second && m_images[BACK_IMAGE].first)
    delete m_images[BACK_IMAGE].first;
  m_images[BACK_IMAGE].first = image;
  m_images[BACK_IMAGE].second = owned; 
}  

/*! \brief sets the bottom image. */
void Cube_environment::set_bottom_image(Image* image, Boolean owned)
{
  if (m_images[BOTTOM_IMAGE].second && m_images[BOTTOM_IMAGE].first)
    delete m_images[BOTTOM_IMAGE].first;
  m_images[BOTTOM_IMAGE].first = image;
  m_images[BOTTOM_IMAGE].second = owned; 
}  

/*! \brief sets the top image. */
void Cube_environment::set_top_image(Image* image, Boolean owned)
{
  if (m_images[TOP_IMAGE].second && m_images[TOP_IMAGE].first)
    delete m_images[TOP_IMAGE].first;
  m_images[TOP_IMAGE].first = image;
  m_images[TOP_IMAGE].second = owned; 
}  

SGAL_END_NAMESPACE
