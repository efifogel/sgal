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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRAY_TYPES_HPP
#define SGAL_ARRAY_TYPES_HPP

#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Bounding_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

typedef boost::shared_ptr<Container>                    Shared_container;

typedef std::vector<Boolean>                            Boolean_array;
typedef std::vector<Float>                              Float_array;
typedef std::vector<Uint>                               Uint_array;
typedef std::vector<Int32>                              Int32_array;
typedef std::vector<Scene_time>                         Scene_time_array;
typedef std::vector<Vector2f>                           Vector2f_array;
typedef std::vector<Vector3f>                           Vector3f_array;
typedef std::vector<Vector3f>                           Vector3f_array;
typedef std::vector<Vector4f>                           Vector4f_array;
typedef std::vector<Rotation>                           Rotation_array;
typedef std::vector<Bounding_sphere>                    Bounding_sphere_array;
typedef std::vector<String>                             String_array;
typedef std::vector<Image>                              Image_array;
typedef std::vector<Shared_container>                   Shared_container_array;

SGAL_END_NAMESPACE

#endif
