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

#ifndef SGAL_IMAGE_READER_HPP
#define SGAL_IMAGE_READER_HPP

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

class SGAL_SGAL_DECL Image_reader : public Image {
public:
  typedef std::list<fi::path>                   Path_list;
  typedef Path_list::iterator                   Path_iter;

  enum {
    FIRST = Image::LAST - 1,
    URL,
    LAST
  };

  /*! Constructor */
  Image_reader(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_reader();

  /*! Construct the prototype. */
  static Image_reader* prototype();

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
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Add a directory to the directory-search list. */
  void add_dir(const fi::path& dir);

  /*! Set the directory-search structure. */
  void set_dirs(const Path_list& dirs);

  /*! Obtain the directory-search structure. */
  const Path_list& get_dirs() const;

  /*! Obtain the image width. */
  virtual Uint get_width();

  /*! Obtain the image height. */
  virtual Uint get_height();

  /*! Obtain the image format. */
  virtual Image::Format get_format();

  /*! Obtain the image pixel data. */
  virtual void* get_pixels();

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

  /*! A collection of directories to search files in. */
  Path_list m_dirs;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Image_reader* Image_reader::prototype() { return new Image_reader(true); }

//! \brief clones.
inline Container* Image_reader::clone() { return new Image_reader(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_reader::get_tag() const { return s_tag; }

//! \brief obtains the URL.
inline const std::string Image_reader::get_url() const { return m_url; }

//! \brief adds a directory to the directory-search list.
inline void Image_reader::add_dir(const fi::path& dir)
{ m_dirs.push_back(dir); }

//! \brief sets the directory-search structure.
inline void Image_reader::set_dirs(const Image_reader::Path_list& dirs)
{ m_dirs = dirs; }

//! \brief obtains the directory-search structure.
inline const Image_reader::Path_list& Image_reader::get_dirs() const
{ return m_dirs; }

SGAL_END_NAMESPACE

#endif
