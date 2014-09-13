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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Execution_coordinator.hpp"
#if 0
#include "SGAL/Text_texture.hpp"
#endif
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Texture_2d::s_tag = "Texture2d";
Container_proto* Texture_2d::s_prototype(nullptr);

REGISTER_TO_FACTORY(Texture_2d, "Texture_2d");

//! Constructor
Texture_2d::Texture_2d(Boolean proto) :
  Texture(proto),
  m_image(nullptr)
#if 0
  m_text(0)
#endif
{ set_target(TEXTURE_2D); }

//! Destructor
Texture_2d::~Texture_2d() {}

//! \brief draws the texture.
void Texture_2d::draw(Context* context)
{
#if 0
  if (m_text) m_text->draw(context);
#endif
  Texture::draw(context);
}

//! \brief cleans the object using the new decoded data.
void Texture_2d::clean()
{
  Texture::clean();
  if (m_image->is_dirty()) m_image->clean();
  load_color_map(m_image, s_targets[m_target]);
#if 0
  if (m_text) m_text->clean();
#endif
}

//! \brief determines whether texture image is empty.
Boolean Texture_2d::empty()
{
  if (!m_image) return true;
  if (m_image->is_dirty()) m_image->clean();
  return m_image->empty();
}

//! \brief obtains the texture number of components.
Uint Texture_2d::get_component_count() const
{
  if (!m_image) return 0;
  if (m_image->is_dirty()) m_image->clean();
  return m_image->get_component_count();
}

//! \brief sets the attributes of the texture.
void Texture_2d::set_attributes(Element* elem)
{ Texture::set_attributes(elem); }

//! \brief initializes the node prototype.
void Texture_2d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Texture::get_prototype());
}

//! \brief deletes the node prototype.
void Texture_2d::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Texture_2d::get_prototype()
{
  if (!s_prototype) Texture_2d::init_prototype();
  return s_prototype;
}

//! \brief adds the container to a given scene.
void Texture_2d::add_to_scene(Scene_graph* scene_graph)
{ m_image->set_dirs(scene_graph->get_data_dirs()); }

//! \brief prints texture information.
void Texture_2d::print_info()
{
  int width = get_width();
  int height = get_height();
  Image::Format format = get_format();
  unsigned int size = Image::get_size(width, height, format);
  std::cout << "Texture size: " << size << std::endl;
}

SGAL_END_NAMESPACE
