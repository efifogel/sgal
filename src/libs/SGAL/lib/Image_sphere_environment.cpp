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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Image_sphere_environment.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Image_sphere_environment::s_prototype(nullptr);
const std::string Image_sphere_environment::s_tag("ImageSphereEnvironment");

REGISTER_TO_FACTORY(Image_sphere_environment, "Image_sphere_environment");

//! \brief construct
Image_sphere_environment::Image_sphere_environment(Boolean proto) :
  Sphere_environment(proto)
{}

//! \brief destruct
Image_sphere_environment::~Image_sphere_environment() {}

//! \brief sets the attributes of this object.
void Image_sphere_environment::set_attributes(Element* elem)
{ Sphere_environment::set_attributes(elem); }

//! \brief initializes the node prototype.
void Image_sphere_environment::init_prototype()
{
  if (s_prototype != nullptr) return;
  s_prototype = new Container_proto(Sphere_environment::get_prototype());
}

//! \brief deletes the node prototype.
void Image_sphere_environment::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Image_sphere_environment::get_prototype()
{
  if (s_prototype == nullptr) Image_sphere_environment::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
