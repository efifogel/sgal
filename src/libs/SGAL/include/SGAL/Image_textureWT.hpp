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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_TEXTURE_WT_HPP
#define SGAL_IMAGE_TEXTURE_WT_HPP

/*! \file
 * Purpose: A class that implements a progressive compressed image texture.
 *
 *  Inherits from Texture
 */

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Texture.hpp"

SGAL_BEGIN_NAMESPACE

class Element;

class SGAL_SGAL_DECL Image_texture_wt : public Texture {
public:
  /*! Constructor */
  Image_texture_wt(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_texture_wt();

  /* Construct the prototype */
  static Image_texture_wt* prototype() { return new Image_texture_wt(true); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();
};

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Image_textureWT::create()
{ return new Image_textureWT(); }

SGAL_END_NAMESPACE

#endif
