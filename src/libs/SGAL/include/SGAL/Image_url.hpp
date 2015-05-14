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

#ifndef SGAL_IMAGE_URL_HPP
#define SGAL_IMAGE_URL_HPP

#include <string>
#include <list>

#include <boost/filesystem/path.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Image.hpp"

namespace fi = boost::filesystem;

SGAL_BEGIN_NAMESPACE

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class Scene_graph;

class SGAL_SGAL_DECL Image_url : public Image {
public:
  typedef std::list<fi::path>                   Path_list;
  typedef Path_list::iterator                   Path_iter;

  enum {
    FIRST = Image::LAST - 1,
    URL,
    FLIP,
    ROTATION,
    ALPHA,
    TRANSPARENCY,
    LAST
  };

  /*! Construct */
  Image_url(Boolean proto = false);

  /*! Destruct */
  virtual ~Image_url();

  /*! Construct the prototype. */
  static Image_url* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  std::string* url_handle(const Field_info*) { return &m_url; }
  Boolean* flip_handle(const Field_info*) { return &m_flip; }
  Float* rotation_handle(const Field_info*) { return &m_rotation; }
  Boolean* alpha_handle(const Field_info*) { return &m_alpha; }
  Float* transparency_handle(const Field_info*) { return &m_transparency; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param[in] scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Set the flag that indicates whether the image should be reflected. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether the image should be reflected. */
  Boolean get_flip() const;

  /*! Set the rotation angle. */
  void set_rotation(Float rotation);

  /*! Obtain the rotation angle. */
  Float get_rotation() const;

  /*! Set the flag that determine whether to add (or retain) the alpha
   * channel, or remove it when present.
   */
  void set_alpha(Boolean flag);

  /*! Determine whether to add (or retain) the alpha channel or remove it. */
  Boolean get_alpha() const;

  /*! Set the transparency of the image. */
  void set_transparency(Float transparency);

  /*! Obtain the transparency of the image. */
  Float get_transparency() const;

  /*! Add a directory to the directory-search list. */
  void add_dir(const fi::path& dir);

  /*! Set the directory-search structure. */
  void set_dirs(const Path_list& dirs);

  /*! Obtain the directory-search structure. */
  const Path_list& get_dirs() const;

  /*! Clean the image in case it is dirty. */
  virtual void clean();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The uniform resource locator (url) that refers to the (Internet)
   * resource that contains the image.
   */
  std::string m_url;

  /*! Indicates whether the image should be reflected when read from file. */
  Boolean m_flip;

  /*! The rotation angle. */
  Float m_rotation;

  /*! Indicates whether to add (or retain) the alpha channel or remove it
   * when present.
   */
  Boolean m_alpha;

  /*! The transparency of the image. */
  Float m_transparency;

  /*! A collection of directories to search files in. */
  Path_list m_dirs;

  /// \name Default values
  //@{
  static const Boolean s_def_flip;
  static const Float s_def_rotation;
  static const Boolean s_def_alpha;
  static const Float s_def_transparency;
  //@}

  friend class Image_texture;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Image_url* Image_url::prototype() { return new Image_url(true); }

//! \brief clones.
inline Container* Image_url::clone() { return new Image_url(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_url::get_tag() const { return s_tag; }

//! \brief obtains the URL.
inline const std::string Image_url::get_url() const { return m_url; }

//! \brief sets the flag that indicates whether the image should be reflected.
inline void Image_url::set_flip(Boolean flag) { m_flip = flag; }

/*! \brief obtains the flag that indicates whether the image should be
 * reflected.
 */
inline Boolean Image_url::get_flip() const { return m_flip; }

//! \brief sets the rotation angle.
inline void Image_url::set_rotation(Float rotation) { m_rotation = rotation; }

//! \brief obtains the rotation angle.
inline Float Image_url::get_rotation() const { return m_rotation; }

/*! \brief sets the flag that indicates whether to add (or retain) the alpha
 * channel or remove it.
 */
inline void Image_url::set_alpha(Boolean flag) { m_alpha = flag; }

//! \brief determines whether to add (or retain) the alpha channel or remove it.
inline Boolean Image_url::get_alpha() const { return m_alpha; }

//! \brief set the transparency of the image.
inline void Image_url::set_transparency(Float transparency)
{ m_transparency = transparency; }

//! \brief obtain the transparency of the image.
inline Float Image_url::get_transparency() const { return m_transparency; }

//! \brief adds a directory to the directory-search list.
inline void Image_url::add_dir(const fi::path& dir)
{ m_dirs.push_back(dir); }

//! \brief sets the directory-search structure.
inline void Image_url::set_dirs(const Image_url::Path_list& dirs)
{ m_dirs = dirs; }

//! \brief obtains the directory-search structure.
inline const Image_url::Path_list& Image_url::get_dirs() const
{ return m_dirs; }

SGAL_END_NAMESPACE

#endif
