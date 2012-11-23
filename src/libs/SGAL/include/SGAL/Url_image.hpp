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
#include "SGAL/Image.hpp"

namespace fi = boost::filesystem;

SGAL_BEGIN_NAMESPACE

class SGAL_CLASSDEF Url_image : public Image {
public:
  typedef std::list<fi::path>                   Path_list;
  typedef Path_list::iterator                   Path_iter;

  enum {
    FIRST = Image::LAST - 1,
    URL,
    LAST
  };

  /*! Constructor */
  Url_image(Boolean proto = SGAL_FALSE);
  
  /*! Destructor */
  virtual ~Url_image();

  /*! Set the URL */
  void set_url(const std::string& url) { m_url = url; }

  /*! Obtain the URL */
  const std::string get_url() const { return m_url; }

  /*! Add a directory to the directory-search list */
  void add_dir(const fi::path& dir) { m_dirs.push_back(dir); }

  /*! Set the directory-search structure */
  void set_dirs(const Path_list& dirs) { m_dirs = dirs; }

  /*! Obtain the directory-search structure */
  const Path_list& get_dirs() const { return m_dirs; }
  
  /*! Obtain the image width */
  virtual Uint get_width();

  /*! Obtain the image height */
  virtual Uint get_height();

  /*! Obtain the image format */
  virtual Image::Format get_format();

  /*! Obtain the image pixel data */
  virtual void* get_pixels();

  /*! Return true if image hasn't been updated yet and false otherwise */
  virtual Boolean is_dirty() { return m_dirty; }

  /*! Update the image in case it is dirty */
  virtual void update();

  /*! Set the flag that indicates whether the image should be reflected */
  void set_flip(Boolean flag) { m_flip = flag; }
  
private:
  /*! the name of the file that the image was read from */
  std::string m_url;

  /*! Is the image dirty */
  Boolean m_dirty;
  
  /*! Indicates which image is used for rendering. The other  
   * is used by the decoder. This can have the values 0 or 1
   */
  Uint m_current_buffer;
  
  /*! The current image. Point to one of the images in m_images */
  Image* m_current_image;

  /*! the double buffering used to manage decoding and rendering */
  Image m_images[2];

  /*! A collection of directories to search files in */
  Path_list m_dirs;

  /*! Indicates whether the image should be reflected when read from file */
  Boolean m_flip;
  
  /*! Allocate memory to hold the image */
  void allocate(Uint size);

  /*! Deallocate the memory that holds the image */
  void deallocate();

  /*! Swap between the decoded image-buffer and the output-image buffer */
  bool swap_image();
};
  
SGAL_END_NAMESPACE

#endif
