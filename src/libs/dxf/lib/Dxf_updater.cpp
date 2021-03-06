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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array_3d.hpp"
#include "SGAL/Color_array_4d.hpp"
#include "SGAL/to_hex_string.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_updater.hpp"
#include "dxf/Dxf_line_entity.hpp"

DXF_BEGIN_NAMESPACE

//! \brief constructs.
Dxf_updater::Dxf_updater(Dxf_data& data) :
  m_data(data)
{}

//! \brief handles a node.
void Dxf_updater::operator()(Shared_container node)
{
  auto shape = boost::dynamic_pointer_cast<SGAL::Shape>(node);
  if (shape) {
    // std::cout << "Processing Shape" << std::endl;
    auto geometry = shape->get_geometry();
    auto ils = boost::dynamic_pointer_cast<SGAL::Indexed_line_set>(geometry);
    if (ils) {
      // std::cout << "Processing IndexedLineSet" << std::endl;
      if (SGAL::Geo_set::PT_LINES == ils->get_primitive_type()) {
        // std::cout << "Processing LINES " << ils->get_num_primitives()
        //           << std::endl;
        auto& indices = ils->line_coord_indices();
        auto cda = ils->get_coord_array();
        auto coords= boost::dynamic_pointer_cast<SGAL::Coord_array_3d>(cda);

        int color(-1);
        auto attach = ils->get_color_attachment();
        auto colors = ils->get_color_array();
        if (colors && ! colors->empty()) {
          if (SGAL::Geo_set::AT_PER_MESH == attach) {
            auto ca = boost::dynamic_pointer_cast<SGAL::Color_array_3d>(colors);
            if (ca) color = compact_color(ca, 0);
            else {
              auto ca =
                boost::dynamic_pointer_cast<SGAL::Color_array_4d>(colors);
              SGAL_assertion(ca);
              color = compact_color(ca, 0);
            }
          }
        }

        for (size_t i = 0; i < ils->get_num_primitives(); ++ i) {
          auto* line_entity = new Dxf_line_entity;
          m_data.m_entities.push_back(line_entity);

          line_entity->m_handle = m_data.m_handle++;
          line_entity->m_owner_handle =
            SGAL::to_hex_string(Dxf_data::s_block_record_model_space_handle);
          line_entity->m_layer = Dxf_data::s_def_layer;
          line_entity->m_color_index = 0;
          if (SGAL::Geo_set::AT_PER_PRIMITIVE == attach) {
            auto ca = boost::dynamic_pointer_cast<SGAL::Color_array_3d>(colors);
            if (ca) color = compact_color(ca, i);
            else {
              auto ca =
                boost::dynamic_pointer_cast<SGAL::Color_array_4d>(colors);
              SGAL_assertion(ca);
              color = compact_color(ca, i);
            }
          }
          if (-1 != color) line_entity->m_color = color;

          auto& start = line_entity->m_start;
          auto s = indices[i][0];
          start[0] = (*coords)[s][0];
          start[1] = (*coords)[s][1];
          start[2] = (*coords)[s][2];

          auto& end = line_entity->m_end;
          auto e = indices[i][1];
          end[0] = (*coords)[e][0];
          end[1] = (*coords)[e][1];
          end[2] = (*coords)[e][2];
        }
        return;
      }
    }
  }
}

DXF_END_NAMESPACE
