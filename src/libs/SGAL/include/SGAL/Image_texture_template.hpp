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
// $Source$
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_TEXTURE_TEMPLATE_HPP
#define SGAL_IMAGE_TEXTURE_TEMPLATE_HPP

/*! \file
 *
 */

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Url_image.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;

template <class T_Texture>
class SGAL_SGAL_DECL Image_texture_template : public T_Texture {
public:
  typedef Url_image::Path_list                          Path_list;

  /*! Constructor */
  Image_texture_template(Boolean proto = false) : T_Texture(proto)
  { this->m_image = &m_url_image; }

  /*! Destructor */
  virtual ~Image_texture_template() {}

  /*! Construct the prototype. */
  static Image_texture_template* prototype()
  { return new Image_texture_template(true); }

  /*! Clone. */
  virtual Container* clone() { return new Image_texture_template(); }

  /*! Set the attributes of this image textute extracted from the VRML or X3D
   * file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene.
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Set the flag that indicates whether to reflect the image. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether to reflect the image. */
  Boolean get_flip() const;

  /*! Set the directory-search structure. */
  void set_dirs(const Path_list& dirs);

  /*! Obtain the directory-search structure. */
  const Path_list& get_dirs() const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The image. */
  Url_image m_url_image;
};

/*! \brief sets the URL. */
template <class T_Texture>
void Image_texture_template<T_Texture>::set_url(const std::string& url)
{ m_url_image.set_url(url); }

/*! \brief obtains the URL. */
template <class T_Texture>
const std::string Image_texture_template<T_Texture>::get_url() const
{ return m_url_image.get_url(); }

/*! \brief sets the flag that indicates whether to reflect the image. */
template <class T_Texture>
void Image_texture_template<T_Texture>::set_flip(Boolean flag)
{ m_url_image.set_flip(flag); }

/*! \brief obtains the flag that indicates whether to reflect the image. */
template <class T_Texture>
Boolean Image_texture_template<T_Texture>::get_flip() const
{ return m_url_image.get_flip(); }

/*! \brief sets the directory-search structure. */
template <class T_Texture>
void
Image_texture_template<T_Texture>::set_dirs(const Url_image::Path_list& dirs)
{ m_url_image.set_dirs(dirs); }

/*! \brief obtains the directory-search structure. */
template <class T_Texture>
const Url_image::Path_list& Image_texture_template<T_Texture>::get_dirs() const
{ return m_url_image.get_dirs(); }

/*! \brief initializes the node prototype. */
template <class T_Texture>
void Image_texture_template<T_Texture>::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(T_Texture::get_prototype());
}

/*! \brief deletes the node prototype. */
template <class T_Texture>
void Image_texture_template<T_Texture>::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
template <class T_Texture>
Container_proto* Image_texture_template<T_Texture>::get_prototype()
{
  if (!s_prototype) Image_texture_template<T_Texture>::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the image texture. */
template <class T_Texture>
void Image_texture_template<T_Texture>::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter        Str_attr_iter;

  T_Texture::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
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

/*! \brief obtains the tag (type) of the container. */
template <class T_Texture>
const std::string& Image_texture_template<T_Texture>::get_tag() const
{ return s_tag; }

/*! \brief adds the container to a given scene */
template <class T_Texture>
void Image_texture_template<T_Texture>::add_to_scene(Scene_graph* sg)
{ m_url_image.set_dirs(sg->get_data_dirs()); }

#if 0
/*! \brief */
template <class T_Texture>
Attribute_list Image_texture_template<T_Texture>::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Texture::get_attributes();

  attrib.first = "url";
  attrib.second = get_fileName();
  attribs.push_back(attrib);

  return attribs;
}
#endif

SGAL_END_NAMESPACE

#endif
