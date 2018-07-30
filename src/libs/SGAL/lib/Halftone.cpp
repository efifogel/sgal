// Copyright (c) 2011 Israel.
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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Halftone.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

/*! Halftone stipple pattern for backfacing elements */
Ubyte Halftone::s_def_pattern[] = {
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55
};

Container_proto* Halftone::s_prototype(nullptr);
const std::string Halftone::s_tag = "Halftone";

/*! Constructor */
Halftone::Halftone(Boolean proto) :
  Container(proto),
  m_dirty(true),
  m_image(nullptr),
  m_pattern(s_def_pattern),
  m_dirty_pattern(false)
{}

//! \brief destructs.
Halftone::~Halftone() {}

//! \brief draws the halftone.
void Halftone::draw(Context*)
{
  if (!m_dirty) return;
  clean();
}

/*! Clean the object using the new decoded data */
void Halftone::clean()
{
  glPolygonStipple(get_pattern());
  m_dirty = false;
}

//! \brief sets the attributes of the object extracted from an input stream.
void Halftone::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    // const std::string & value = elem->get_value(ai);
    if (name == "image") {
      std::cerr << "Not implemented" << std::endl;
      continue;
    }
    if (name == "pattern") {
      std::cerr << "Not implemented" << std::endl;
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief initializes the node prototype.
void Halftone::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();
}

//! \brief deletes the node prototype.
void Halftone::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Halftone::get_prototype()
{
  if (!s_prototype) Halftone::init_prototype();
  return s_prototype;
}

//! \brief
Boolean Halftone::attach_context(Context* context)
{
  Boolean result = Container::attach_context(context);
  return result;
}

//! \brief
Boolean Halftone::detach_context(Context* context)
{
  Boolean result = Container::detach_context(context);
  // next draw with another context - recreate the halftone
  m_dirty = true;
  return result;
}

//! \brief cleans the pattern.
void Halftone::clean_pattern()
{
  if (m_image) {
    if (m_image->is_dirty()) m_image->clean();
    m_pattern = static_cast<Ubyte*>(m_image->get_pixels());
  }
  m_dirty_pattern = false;
}

//! \brief sets the halftone pixel-data.
void Halftone::set_pattern(Ubyte* pattern)
{
  m_dirty_pattern = false;
  m_dirty = true;
  m_pattern = pattern;
}

//! \brief obtains the halftone pixel-data.
Ubyte* Halftone::get_pattern()
{
  if (m_dirty_pattern) clean_pattern();
  return m_pattern;
}

//! \brief sets the halftone image.
inline void Halftone::set_image(Image* image)
{
  m_image = image;
  m_dirty_pattern = true;
}

SGAL_END_NAMESPACE
