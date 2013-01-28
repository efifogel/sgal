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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Text_texture.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Texture::s_tag = "Texture";
Container_proto* Texture::s_prototype = NULL;

REGISTER_TO_FACTORY(Texture, "Texture");

/*! Constructor */
Texture::Texture(Boolean proto) :
  Texture_base(proto),
  m_image(NULL),
  m_dirty(true),
  m_text(0)
{ set_target(TEXTURE_2D); }

/*! Destructor */
Texture::~Texture() {}

/*! \brief draws the texture. */
void Texture::draw(Context* context)
{
  if (m_text) m_text->draw(context);
  Texture_base::draw(context);
}

/*! \brief cleans the object using the new decoded data. */
void Texture::clean()
{
  if (Texture_base::is_dirty()) Texture_base::clean();
  if (m_image->is_dirty()) m_image->clean();
  load_color_map(m_image, s_targets[m_target]);
  if (m_text) m_text->clean();
  m_dirty = false;
}

/* \brief determines whether texture image is empty. */
Boolean Texture::empty()
{
  return ((get_width() == 0) || (get_height() == 0) || (get_pixels() == NULL));
}

/*! \brief sets the attributes of the texture. */
void Texture::set_attributes(Element* elem) 
{ Texture_base::set_attributes(elem); }

/*! \brief initializes the node prototype. */
void Texture::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Texture_base::get_prototype());
}

/*! \brief deletes the node prototype. */
void Texture::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Texture::get_prototype() 
{  
  if (!s_prototype) Texture::init_prototype();
  return s_prototype;
}

/*! \brief prints texture information. */
void Texture::print_info()
{
  int width = get_width();
  int height = get_height();
  Image::Format format = get_format();
  unsigned int size = Image::get_size(width, height, format);
  std::cout << "Texture size: " << size << std::endl;
}

SGAL_END_NAMESPACE
