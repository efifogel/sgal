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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_URL_IMAGE_HPP
#define SGAL_URL_IMAGE_HPP

#include <string>
#include <list>

#include <boost/filesystem/path.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Image_base.hpp"

namespace fi = boost::filesystem;

SGAL_BEGIN_NAMESPACE

class SGAL_CLASSDEF Image : public Image_base {
public:
  typedef std::list<fi::path>                   Path_list;
  typedef Path_list::iterator                   Path_iter;

  enum {
    FIRST = Image_base::LAST - 1,
    URL,
    FLIP,
    LAST
  };

  /*! Constructor */
  Image(Boolean proto = false);
  
  /*! Destructor */
  virtual ~Image();

  /*! Construct the prototype. */
  static Image* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

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
  virtual Image_base::Format get_format();

  /*! Obtain the image pixel data. */
  virtual void* get_pixels();

  /*! Clean the image in case it is dirty. */
  virtual void clean();

  /*! Set the flag that indicates whether the image should be reflected. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether the image should be reflected. */
  Boolean get_flip() const;
  
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

  /*! Indicates whether the image should be reflected when read from file. */
  Boolean m_flip;
};

/*! \brief constructs the prototype. */
inline Image* Image::prototype() { return new Image(true); }

/*! \brief clones. */
inline Container* Image::clone() { return new Image(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Image::get_tag() const { return s_tag; }

/*! \brief obtains the URL. */
inline const std::string Image::get_url() const { return m_url; }

/*! \brief adds a directory to the directory-search list. */
inline void Image::add_dir(const fi::path& dir) { m_dirs.push_back(dir); }

/*! \brief sets the directory-search structure. */
inline void Image::set_dirs(const Image::Path_list& dirs) { m_dirs = dirs; }

/*! \brief obtains the directory-search structure. */
inline const Image::Path_list& Image::get_dirs() const { return m_dirs; }

/*! \brief sets the flag that indicates whether the image should be reflected.
 */
inline void Image::set_flip(Boolean flag) { m_flip = flag; }

/*! \brief obtains the flag that indicates whether the image should be
 * reflected.
 */
inline Boolean Image::get_flip() const { return m_flip; }
  
SGAL_END_NAMESPACE

#endif
