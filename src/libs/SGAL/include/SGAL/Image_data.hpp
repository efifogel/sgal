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
// $Revision: 1308 $
//
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_DATA_HPP
#define SGAL_IMAGE_DATA_HPP

/*! \file
 * A class that holds image data that can be passed between the decoder
 * and the scene graph.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Item_primitive.hpp"

SGAL_BEGIN_NAMESPACE

class Image_data : public Item_primitive {
public:
  /*! Constructor */
  Image_data() : m_pixels(0)
  {
    m_attributes.push_back("0");        // width
    m_attributes.push_back("0");        // height
    m_attributes.push_back("3");        // number of color components
    m_attributes.push_back("");         // minimum size of image.
    m_attributes.push_back("");         // image quality.
    m_attributes.push_back("");         // resolution level of image.
    m_attributes.push_back("");         // channels size fraction
  }

  /*! Destructor */
  virtual ~Image_data() {}

  enum {
    WIDTH = 0,
    HEIGHT,
    COMPONENET_NUM,
    SIZE_LIMIT,
    RESOLUTION_REDUCTION,
    CHANNEL_SIZE_FRACTION,
    QUALITY,
    LAST
  };

  /*! returns a pointer to the pixels array. */
  void * get_pixels() { return m_pixels; }

  /*! sets the pointer to the pixels/ */
  void set_pixels(void * pixels) { m_pixels = pixels; }

private:
  /*! A pointer to the actual pixel data */
  void * m_pixels;
};

SGAL_END_NAMESPACE

#endif
