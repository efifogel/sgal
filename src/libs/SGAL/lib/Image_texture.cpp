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

#include "SGAL/basic.hpp"
#include "SGAL/Image_texture.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Image_texture::s_prototype(nullptr);
const std::string Image_texture::s_tag = "ImageTexture";

REGISTER_TO_FACTORY(Image_texture, "Image_texture");

//! Constructor
Image_texture::Image_texture(Boolean proto) : Texture_2d(proto)
{ Texture_2d::set_image(&m_image); }

//! \brief initializes the node prototype.
void Image_texture::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Texture_2d::get_prototype());
}

//! \brief deletes the node prototype.
void Image_texture::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Image_texture::get_prototype()
{
  if (!s_prototype) Image_texture::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the image texture.
void Image_texture::set_attributes(Element* elem)
{
  Texture_2d::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "url") {
      std::string url = strip_double_quotes(value);
      set_url(url);
      url.clear();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "flip") {
      set_flip(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
#if 0
    if (name == "alphaUrl") {         // convertor only !!
      SetGSFileName(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "imageSizeLimit") {
      m_decoderImage.set_value(Image_data::SIZE_LIMIT, value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "imageQuality") {
      m_decoderImage.set_value(Image_data::QUALITY, value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "imageResolutionLevel") {
      m_decoderImage.set_value(Image_data::RESOLUTION_REDUCTION, value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "channelSizeFraction") {
      m_decoderImage.set_value(Image_data::CHANNEL_SIZE_FRACTION, value);
      elem->mark_delete(ai);
    }
#endif
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
//! \brief
Attribute_list Image_texture::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Texture_2d::get_attributes();

  attrib.first = "url";
  attrib.second = get_fileName();
  attribs.push_back(attrib);

  return attribs;
}
#endif

//! \brief adds the container to a given scene.
void Image_texture::add_to_scene(Scene_graph* scene_graph)
{ Texture_2d::add_to_scene(scene_graph); }

SGAL_END_NAMESPACE
