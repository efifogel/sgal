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

Container_proto* Halftone::s_prototype(NULL);
const std::string Halftone::s_tag = "Halftone";

/*! Constructor */
Halftone::Halftone(Boolean proto) :
  Container(proto),
  m_dirty(true),
  m_image(NULL),
  m_pattern(s_def_pattern),
  m_dirty_pattern(true)
{
}

/*! Destructor */
Halftone::~Halftone() {}

/*! Draw the halftone */
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

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Halftone::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter        Str_attr_iter;

  Container::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
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

/*! Initializes the node prototype */
void Halftone::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();
}

/*! */
void Halftone::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Halftone::get_prototype()
{
  if (!s_prototype) Halftone::init_prototype();
  return s_prototype;
}

/*!
 */
Boolean Halftone::attach_context(Context* context)
{
  Boolean result = Container::attach_context(context);
  return result;
}

/*!
 */
Boolean Halftone::detach_context(Context* context)
{
  Boolean result = Container::detach_context(context);
  // next draw with another context - recreate the halftone
  m_dirty = true;
  return result;
}

/*! Obtain the halftone pixel-data */
Ubyte* Halftone::get_pattern()
{
  if (m_dirty_pattern) {
    if (m_image) {
      if (m_image->is_dirty()) m_image->clean();
      m_pattern = static_cast<Ubyte*>(m_image->get_pixels());
    }
  }
  m_dirty_pattern = false;
  return m_pattern;
}

SGAL_END_NAMESPACE
