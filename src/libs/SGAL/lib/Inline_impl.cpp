// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel        <efifogel@gmail.com>

#include <iostream>
#include <fstream>

#include "SGAL/basic.hpp"
#include "SGAL/Inline.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Loader_errors.hpp"

SGAL_BEGIN_NAMESPACE

/*! Process change of url field.
 */
void Inline::url_changed(const Field_info* field_info)
{
  m_dirty_childs = true;
  field_changed(field_info);
}

//! \brief adds the container to a given scene.
void Inline::add_to_scene(Scene_graph* sg)
{
  Group::add_to_scene(sg);
  if (m_dirty_childs) clean_childs();
}

//! \brief cleans (generate) the children.
void Inline::clean_childs()
{
  Group::clean_childs();
  const auto& filename = m_url[0];
  auto& loader = *(SGAL::Loader::get_instance());
  loader(filename.c_str(), m_scene_graph, this);
}

SGAL_END_NAMESPACE
