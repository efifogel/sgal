// Copyright (c) 2004  Israel.
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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Texture_transform.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Texture_transform::s_prototype(NULL);
const std::string Texture_transform::s_tag = "TextureTransform";

REGISTER_TO_FACTORY(Texture_transform, "Textture_transform");

/*! Constructor */
Texture_transform::Texture_transform(Boolean proto) : Container(proto) {}

/*! Destructor */
Texture_transform::~Texture_transform() {}

/*! Initializes the node prototype */
void Texture_transform::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());
}

/*! Deletes the node prototype */
void Texture_transform::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtains the node prototype */
Container_proto* Texture_transform::get_prototype()
{
  if (!s_prototype) Texture_transform::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this container. */
void Texture_transform::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
};

SGAL_END_NAMESPACE
