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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Image_texture.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

template <>
Container_proto * Image_texture_template<Texture>::s_prototype = NULL;

template <>
std::string Image_texture_template<Texture>::s_tag = "ImageTexture";

REGISTER_TO_FACTORY(Image_texture_template<Texture>, "Image_texture");

SGAL_END_NAMESPACE
