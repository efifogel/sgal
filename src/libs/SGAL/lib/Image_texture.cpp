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

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Image_texture.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Image_url.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Image_texture::s_prototype(nullptr);
const std::string Image_texture::s_tag("ImageTexture");

REGISTER_TO_FACTORY(Image_texture, "Image_texture");

//! Constructor
Image_texture::Image_texture(Boolean proto) :
  Texture_2d(proto),
  m_flip(Image_url::s_def_flip),
  m_dirty_image(true),
  m_dirty_url(false),
  m_dirty_flip(false)
{}

//! \brief destructs.
Image_texture::~Image_texture()
{
  if (m_image) {
    Observer observer(this, get_field_info(IMAGE));
    m_image->unregister_observer(observer);
  }
}

//! \brief initializes the node prototype.
void Image_texture::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Texture_2d::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Image_texture::field_changed);

  // url
  String_handle_function url_func =
    static_cast<String_handle_function>(&Image_url::url_handle);
  s_prototype->add_field_info(new SF_string(URL, "url",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            url_func, exec_func));

  // flip
  Boolean_handle_function flip_func =
    static_cast<Boolean_handle_function>(&Image_url::flip_handle);
  s_prototype->add_field_info(new SF_bool(FLIP, "flip",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          flip_func, Image_url::s_def_flip,
                                          exec_func));
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
      m_url = value;
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

//! \brief adds the container to a given scene.
void Image_texture::add_to_scene(Scene_graph* scene_graph)
{ m_scene_graph = scene_graph; }

//! \brief sets the image.
void Image_texture::set_image(Shared_image image)
{
  Observer observer(this, get_field_info(IMAGE));
  if (m_image) m_image->unregister_observer(observer);
  m_image = image;
  m_image->register_observer(observer);
  m_image->add_to_scene(m_scene_graph);
  m_dirty_image = false;
  m_dirty_url = true;
  m_dirty_flip = true;
}

//! \brief obtains the image.
const Image_texture::Shared_image Image_texture::get_image()
{
  if (m_dirty_image) clean_image();
  return m_image;
}

//! \brief sets the URL.
void Image_texture::set_url(const std::string& url)
{
  m_url = url;
  m_dirty_image = true;
  m_dirty_url = false;
}

//! \brief obtains the URL.
const std::string Image_texture::get_url()
{
  if (m_dirty_url) clean_attributes();
  return m_url;
}

//! \brief sets the flag that indicates whether to reflect the image.
void Image_texture::set_flip(Boolean flag)
{
  m_flip = flag;
  m_dirty_image = true;
  m_dirty_flip = false;
}

//! \brief obtains the flag that indicates whether to reflect the image.
Boolean Image_texture::get_flip()
{
  if (m_dirty_flip) clean_attributes();
  return m_flip;
}

//! \brief cleans the image.
void Image_texture::clean_image()
{
  m_dirty_image = false;
  if (!m_image) {
    Observer observer(this, get_field_info(IMAGE));
    if (m_image) m_image->unregister_observer(observer);
    m_image.reset(new Image_url);
    SGAL_assertion(m_image);
    m_image->register_observer(observer);
    m_image->add_to_scene(m_scene_graph);
  }
  auto image = boost::dynamic_pointer_cast<Image_url>(m_image);
  image->set_url(get_url());
  image->set_flip(get_flip());
}

//! \brief cleans the attributes.
void Image_texture::clean_attributes()
{
  m_dirty_url = false;
  m_dirty_flip = false;
  auto image = boost::dynamic_pointer_cast<Image_url>(m_image);
  if (image) {
    m_url = image->get_url();
    m_flip = image->get_flip();
  }
}

//! \brief processes change of field.
void Image_texture::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case IMAGE:
    m_dirty_image = false;
    m_dirty_url = true;
    m_dirty_flip = true;
    break;

   case URL:
    m_dirty_url = false;
    m_dirty_image = true;
    break;

   case FLIP:
    m_dirty_flip = false;
    m_dirty_image = true;
    break;

   default: break;
  }
  Texture_2d::field_changed(field_info);
}

//! \brief clean the object using the new decoded data.
void Image_texture::clean()
{
  if (m_dirty_image) clean_image();
  Texture_2d::clean();
}

//! \brief determines whether the texture is empty.
Boolean Image_texture::empty()
{
  if (m_dirty_image) clean_image();
  return Texture_2d::empty();
}

//! \brief obtains the texture number of components.
Uint Image_texture::get_component_count()
{
  if (m_dirty_image) clean_image();
  return Texture_2d::get_component_count();
}

SGAL_END_NAMESPACE
