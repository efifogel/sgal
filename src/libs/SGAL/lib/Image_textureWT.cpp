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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "Image_texture_wt.h"
#include "Element.h"

SGAL_BEGIN_NAMESPACE

std::string Image_texture_wt::s_tag = "enbImageTextureWT";

REGISTER_TO_FACTORY(Image_texture_wt, "Image_texture_wt");

INIT_DEBUG_CLASS(Image_texture_wt, DBG_IMAGETEXTUREWT);

/*! Constructor */
Image_texture_wt::Image_texture_wt(Boolean proto) :
  Texture(proto)
{
}

/*! Destructor */
Image_texture_wt::~Image_texture_wt() {}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Image_texture_wt::set_attributes(Element * elem) 
{
  Texture::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

Attribute_list Image_texture_wt::get_attributes() 
{ 
    Attribute_list attribs; 
    attribs = Texture::get_attributes();
    return attribs; 
}

SGAL_END_NAMESPACE
