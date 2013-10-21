// Copyright (c) 2013 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRAY_TYPES_HPP
#define SGAL_ARRAY_TYPES_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Sphere_bound.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

typedef boost::shared_ptr<Container>                   Shared_container;

typedef SGAL::Array<Boolean>                           Boolean_array;
typedef SGAL::Array<Float>                             Float_array;
typedef SGAL::Array<Uint>                              Uint_array;
typedef SGAL::Array<Int>                               Int_array;
typedef SGAL::Array<Scene_time>                        Scene_time_array;
typedef SGAL::Array<Vector2f>                          Vector2f_array;
typedef SGAL::Array<Vector3f>                          Vector3f_array;
typedef SGAL::Array<Vector3f>                          Vector3f_array;
typedef SGAL::Array<Vector4f>                          Vector4f_array;
typedef SGAL::Array<Rotation>                          Rotation_array;
typedef SGAL::Array<Sphere_bound>                      Sphere_bound_array;
typedef SGAL::Array<std::string>                       String_array;
typedef SGAL::Array<Shared_container>                  Shared_container_array;

SGAL_END_NAMESPACE

#endif
