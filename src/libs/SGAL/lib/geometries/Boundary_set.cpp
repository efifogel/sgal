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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <sstream>
#include <algorithm>

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include "SGAL/basic.hpp"
#include "SGAL/Boundary_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array_3d.hpp"
#include "SGAL/Color_array_4d.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Tex_coord_array_3d.hpp"
#include "SGAL/Tex_coord_array_4d.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/Bounding_box.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include <SGAL/to_boolean.hpp>

SGAL_BEGIN_NAMESPACE

//! \todo #include "Model_stats.h"

Container_proto* Boundary_set::s_prototype(nullptr);

// Default values:
const Boolean Boundary_set::s_def_normal_per_vertex(false);
const Boolean Boundary_set::s_def_color_per_vertex(false);

void (Boundary_set::*Boundary_set::m_draws[BO_NUM_DRAWS])();
Boolean Boundary_set::m_draws_initialized(false);

REGISTER_TO_FACTORY(Boundary_set, "Boundary_set");

//! \brief constructs.
Boundary_set::Boundary_set(Boolean proto) :
  Mesh_set(proto),
  m_normal_per_vertex(s_def_normal_per_vertex),
  m_color_per_vertex(s_def_color_per_vertex),
  m_tri_strip_lengths(0),
  m_display_list_id(0),
  m_coord_buffer_id(0),
  m_color_buffer_id(0),
  m_normal_buffer_id(0),
  m_tex_coord_buffer_id(0),
  m_drawing_mode(Configuration::s_def_geometry_drawing_mode),
  m_use_vertex_buffer_object
    (Configuration::s_def_use_vertex_buffer_object),
  m_dirty_normal_array(true),
  m_normal_array_cleaned(false),
  m_dirty_tex_coord_array(true),
  m_tex_coord_array_cleaned(false),
  m_dirty_coord_buffer(true),
  m_dirty_normal_buffer(true),
  m_dirty_color_buffer(true),
  m_dirty_tex_coord_buffer(true),
  m_dirty_local_vertex_buffers(true),
  m_dirty_center(true)
{
  if (proto) return;

  m_primitive_type = PT_POLYGONS;

  if (m_draws_initialized) return;
  m_draws_initialized = true;

  // Initialize static draws[] array:
  for (auto i = 0; i < BO_NUM_DRAWS; ++i)
    m_draws[i] = &Boundary_set::draw_invalid;

  // Standard (no vertex array):
  // Polygons:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TENO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TENO_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOPO_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOPO_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO;

  // Triangles
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TENO_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TENO_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TENO_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TENO_MOTR_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOTR_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOTR_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO;

  // Quads
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TENO_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TENO_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TENO_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TENO_MOQU_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOQU_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOQU_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO;

  // Triangle Strips:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TENO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TENO_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOTS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOTS_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO;

  // Vertex array:
  // Polygons:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TENO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TENO_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOPO_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOPO_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO;

  // Triangles:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOTR_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOTR_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOTR_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOTR_VAYE] = &Boundary_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOTR_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOTR_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOTR_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOTR_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO;

  // Quads:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOQU_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOQU_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOQU_VAYE] = &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOQU_VAYE] = &Boundary_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOQU_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOQU_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO;

  // Triangle Strips:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TENO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TENO_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TENO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TENO_MOTS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TENO_MOTS_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO;
}

//! \brief destructs.
Boundary_set::~Boundary_set()
{
  clear_local_vertex_buffers();
  destroy_display_list();
  destroy_vertex_buffers();
  m_dirty_local_vertex_buffers = true;
}

/* \brief sets the flag that indicates whether normals are bound per vertex or
 * per face.
 */
void Boundary_set::set_normal_per_vertex(Boolean normal_per_vertex)
{
  m_normal_per_vertex = normal_per_vertex;
  m_normal_attachment = (normal_per_vertex) ? AT_PER_VERTEX : AT_PER_PRIMITIVE;
}

/* \brief sets the flag that indicates whether colors are bound per vertex or
 * per face.
 */
void Boundary_set::set_color_per_vertex(Boolean color_per_vertex)
{
  m_color_per_vertex = color_per_vertex;
  m_color_attachment = (color_per_vertex) ? AT_PER_VERTEX : AT_PER_PRIMITIVE;
}

//! \brief responds to a change in the coordinate array.
void Boundary_set::coord_content_changed(const Field_info* field_info)
{
  destroy_display_list();
  m_dirty_coord_buffer = true;
  m_dirty_local_vertex_buffers = true;
  if (m_normal_array_cleaned || !m_normal_array) m_dirty_normal_array = true;
  if (m_tex_coord_array_cleaned || !m_tex_coord_array)
    m_dirty_tex_coord_array = true;
  Mesh_set::coord_content_changed(field_info);
}

//! \brief responds to a change in the normal array.
void Boundary_set::normal_content_changed(const Field_info* field_info)
{
  destroy_display_list();
  m_dirty_normal_buffer = true;
  m_dirty_local_vertex_buffers = true;
  m_dirty_normal_array = !m_normal_array;
  m_normal_array_cleaned = false;
  Mesh_set::normal_content_changed(field_info);
}

//! \brief responds to a change in the color array.
void Boundary_set::color_content_changed(const Field_info* field_info)
{
  destroy_display_list();
  m_dirty_color_buffer = true;
  m_dirty_local_vertex_buffers = true;
  Mesh_set::color_content_changed(field_info);
}

//! \brief responds to a change in the texture coordinate array.
void Boundary_set::tex_coord_content_changed(const Field_info* field_info)
{
  destroy_display_list();
  m_dirty_tex_coord_buffer = true;
  m_dirty_local_vertex_buffers = true;
  m_dirty_tex_coord_array = !m_tex_coord_array;
  m_tex_coord_array_cleaned = false;
  Mesh_set::tex_coord_content_changed(field_info);
}

//! \brief cleans the normal array and the normal indices.
void Boundary_set::clean_normals()
{
  if (m_normal_attachment == AT_PER_VERTEX)
    calculate_single_normal_per_vertex();
  else calculate_normal_per_facet();
  m_dirty_normal_array = false;
  m_normal_array_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief obtains the normal array.
Boundary_set::Shared_normal_array Boundary_set::get_normal_array()
{
  if (m_dirty_normal_array) clean_normals();
  return m_normal_array;
}

//! \brief Calculate vertex information per triangle for all triangles.
void
Boundary_set::calculate_vertex_info_per_triangle(Vertices_info& vertices_info)
{
  const auto& tris = triangle_coord_indices();
  for (size_t i = 0; i < m_num_primitives; ++i) {
    auto index0 = tris[i][0];
    auto index1 = tris[i][1];
    auto index2 = tris[i][2];
    Vector3f center;
    center.add(get_coord_3d(index0));
    center.add(get_coord_3d(index1));
    center.add(get_coord_3d(index2));
    center.scale(1.0f / 3);
    compute_vertex_info(index0, i, center,
                        std::back_inserter(vertices_info[index0]));
    compute_vertex_info(index1, i, center,
                        std::back_inserter(vertices_info[index1]));
    compute_vertex_info(index2, i, center,
                        std::back_inserter(vertices_info[index2]));
  }
}

//! \brief Calculate vertex information per quad for all quads.
void Boundary_set::calculate_vertex_info_per_quad(Vertices_info& vertices_info)
{
  const auto& quads = quad_coord_indices();
  for (size_t i = 0; i < m_num_primitives; ++i) {
    auto index0 = quads[i][0];
    auto index1 = quads[i][1];
    auto index2 = quads[i][2];
    auto index3 = quads[i][3];
    Vector3f center;
    center.add(get_coord_3d(index0));
    center.add(get_coord_3d(index1));
    center.add(get_coord_3d(index2));
    center.add(get_coord_3d(index3));
    center.scale(1.0f / 4);
    compute_vertex_info(index0, i, center,
                        std::back_inserter(vertices_info[index0]));
    compute_vertex_info(index1, i, center,
                        std::back_inserter(vertices_info[index1]));
    compute_vertex_info(index2, i, center,
                        std::back_inserter(vertices_info[index2]));
    compute_vertex_info(index3, i, center,
                        std::back_inserter(vertices_info[index3]));
  }
}

//! \brief Calculate vertex information per polygon for all polygons.
void
Boundary_set::calculate_vertex_info_per_polygon(Vertices_info& vertices_info)
{
  const auto& polygons = polygon_coord_indices();
  for (size_t i = 0; i < m_num_primitives; ++i) {
    Vector3f center;
    size_t j;
    for (j = 0; j < polygons[i].size(); ++j)
      center.add(get_coord_3d(polygons[i][j]));
    center.scale(1.0f / polygons[i].size());

    for (j = 0; j < polygons[i].size(); ++j) {
      Uint index = polygons[i][j];
      compute_vertex_info(index, i, center,
                          back_inserter(vertices_info[index]));
    }
  }
}

//! \brief calculates vertex information.
void Boundary_set::calculate_vertices_info(Vertices_info& vertices_info)
{
  auto coords = get_coord_array();
  SGAL_assertion(coords);
  vertices_info.resize(coords->size());

  Uint i, j = 0;
  // Assume that the facet is planar, and compute its normal:
  switch (m_primitive_type) {
   case PT_TRIANGLES: calculate_vertex_info_per_triangle(vertices_info); break;
   case PT_QUADS: calculate_vertex_info_per_quad(vertices_info); break;
   case PT_POLYGONS: calculate_vertex_info_per_polygon(vertices_info); break;

   case PT_TRIANGLE_STRIP:
   case PT_TRIANGLE_FAN:
   case PT_QUAD_STRIP:
   default: SGAL_assertion(0); break;
  }
}

//! \brief calculates a single normal per vertex for all vertices.
void Boundary_set::calculate_single_normal_per_vertex()
{
  auto coords = get_coord_array();
  SGAL_assertion(coords);
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array(coords->size()));
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->resize(coords->size());
  calculate_single_normal_per_vertex(m_normal_array);
  set_normal_per_vertex(true);
}

//! \brief determines whether the surface is smooth.
Boolean Boundary_set::is_smooth(const Vector3f& normal1,
                                    const Vector3f& normal2) const
{
  float angle = acosf(normal1.dot(normal2));
  return (angle > m_crease_angle);
}

//! \brief calculates a single normal per triangle for all triangles.
void Boundary_set::calculate_normal_per_triangle(Normal_array& normals)
{
  const auto& tris = triangle_coord_indices();
  for (size_t i = 0; i < m_num_primitives; ++i) {
    const Vector3f& v0 = get_coord_3d(tris[i][0]);
    const Vector3f& v1 = get_coord_3d(tris[i][1]);
    const Vector3f& v2 = get_coord_3d(tris[i][2]);
    if (is_ccw()) normals[i].normal(v0, v1, v2);
    else normals[i].normal(v2, v1, v0);
  }
}

//! \brief calculates a single normal per quad for all quads.
void Boundary_set::calculate_normal_per_quad(Normal_array& normals)
{
  const auto& quads = quad_coord_indices();
  for (size_t i = 0; i < m_num_primitives; ++i) {
    const Vector3f& v0 = get_coord_3d(quads[i][0]);
    const Vector3f& v1 = get_coord_3d(quads[i][1]);
    const Vector3f& v2 = get_coord_3d(quads[i][2]);

    if (!v0.collinear(v0, v1, v2)) {
      if (is_ccw()) normals[i].normal(v0, v1, v2);
      else normals[i].normal(v2, v1, v0);
      continue;
    }
    const Vector3f& v3 = get_coord_3d(quads[i][3]);
    SGAL_assertion(!v0.collinear(v0, v1, v3));
    if (is_ccw()) normals[i].normal(v0, v1, v3);
    else normals[i].normal(v3, v1, v0);
  }
}

//! \brief Calculate a single normal per polygon for all polygons.
void Boundary_set::calculate_normal_per_polygon(Normal_array& normals)
{
  const auto& polygons = polygon_coord_indices();
  for (size_t i = 0; i < m_num_primitives; ++i) {
    const Vector3f& v0 = get_coord_3d(polygons[i][0]);
    const Vector3f& v1 = get_coord_3d(polygons[i][1]);
    Boolean collinear(true);
    for (size_t j = 2; j < polygons[i].size(); ++j) {
      const Vector3f& v2 = get_coord_3d(polygons[i][j]);
      if (v0.collinear(v0, v1, v2)) continue;
      collinear = false;
      if (is_ccw()) normals[i].normal(v0, v1, v2);
      else normals[i].normal(v2, v1, v0);
      break;
    }
    SGAL_assertion_msg(!collinear, "All vertices are collinear!");
  }
}

//! \brief calculates a single normal per polygon for all polygons.
void Boundary_set::calculate_normal_per_facet(Normal_array& normals)
{
  SGAL_assertion(normals.size() == m_num_primitives);
  switch (m_primitive_type) {
   case PT_TRIANGLES: calculate_normal_per_triangle(normals); break;
   case PT_QUADS: calculate_normal_per_quad(normals); break;
   case PT_POLYGONS: calculate_normal_per_polygon(normals); break;

   case PT_TRIANGLE_STRIP: break;
   case PT_TRIANGLE_FAN: break;
   case PT_QUAD_STRIP:
   default: SGAL_assertion(0); break;
  }

  clear_facet_normal_indices();
}

//! \brief calculates a single normal per polygon for all polygons.
void Boundary_set::calculate_normal_per_facet()
{
  auto coords = get_coord_array();
  SGAL_assertion(coords);
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array(m_num_primitives));
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->resize(m_num_primitives);
  calculate_normal_per_facet(*m_normal_array);
  set_normal_per_vertex(false);
}

//! \brief obtains the texture-coordinate array.
Boundary_set::Shared_tex_coord_array Boundary_set::get_tex_coord_array()
{
  // We assume 2D texture coordinate, but it is an arbitrary choice.
  if (m_dirty_tex_coord_array) clean_tex_coords(Texture::TEXTURE_2D_ARRAY);
  return m_tex_coord_array;
}

//! \brief cleans the texture-mapping coordinate array and coordinate indices.
void Boundary_set::clean_tex_coords(Texture::Target target)
{
  m_dirty_tex_coord_array = false;
  m_tex_coord_array_cleaned = true;
  m_dirty_tex_coord_buffer = true;
  if (!m_generate_tex_coord) return;

  switch (target) {
   case Texture::TEXTURE_1D:
   case Texture::TEXTURE_1D_ARRAY: SGAL_error_msg("Not supported yet!"); break;

   case Texture::TEXTURE_2D:
   case Texture::TEXTURE_2D_ARRAY:
   case Texture::TEXTURE_2D_MULTISAMPLE:
   case Texture::TEXTURE_2D_MULTISAMPLE_ARRAY:
   case Texture::TEXTURE_RECTANGLE: clean_tex_coords_2d(); break;

   case Texture::TEXTURE_3D:
   case Texture::TEXTURE_CUBE_MAP:
   case Texture::TEXTURE_CUBE_MAP_ARRAY: clean_tex_coords_3d(); break;

   case Texture::TEXTURE_BUFFER: SGAL_error_msg("Not supported yet!"); break;
   default: SGAL_error();
  }
}

void Boundary_set::compute_flat_tex_coords_2d(size_t num_verts) {}
void Boundary_set::compute_polygon_tex_coords_2d() {}

//! \brief cleans the 2D texture-mapping coordinate array and coordinate indices.
void Boundary_set::clean_tex_coords_2d()
{
  auto coords = get_coord_array();
  SGAL_assertion(coords);

  // Compute bounding box
  Bounding_box bbox = bounding_box();

  // Allocate space for texture coordinates and texture cordinate indices
  auto size = size_facet_indices(m_facet_coord_indices);

  if (m_tex_coord_array) m_tex_coord_array->resize(size);
  else {
    m_tex_coord_array.reset(new Tex_coord_array_2d(size));
    SGAL_assertion(m_tex_coord_array);
  }
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);

  // Generate indices from 0 to size-1
  resize_facet_indices(m_facet_tex_coord_indices, m_facet_coord_indices);
  sequence_facet_indices(m_facet_tex_coord_indices);

  m_dirty_facet_tex_coord_indices = false;
  m_dirty_tex_coord_indices = true;

  // Generate coordinates
  auto dims = bbox.get_longest_dimensions();
  auto d0 = boost::get<0>(dims);
  auto d1 = boost::get<1>(dims);
  Vector3f range(bbox.xmax() - bbox.xmin(),
                 bbox.ymax() - bbox.ymin(),
                 bbox.zmax() - bbox.zmin());
  switch (m_primitive_type) {
   case PT_TRIANGLES:
    {
     const auto& tris = triangle_coord_indices();
     size_t k(0);
     for (size_t i = 0; i < m_num_primitives; ++i) {
       for (size_t j = 0; j < 3; ++j) {
         const Vector3f& v = get_coord_3d(tris[i][j]);
         auto s = (v[d0] - bbox.min(d0)) / range[d0];
         auto t = (v[d1] - bbox.min(d1)) / range[d0];
         Vector2f tex_vec(s, t);
         (*tex_coord_array)[k++] = tex_vec;
       }
     }
    }
    break;

   case PT_QUADS:
    {
     const auto& quads = quad_coord_indices();
     size_t k(0);
     for (Uint i = 0; i < m_num_primitives; ++i) {
       for (size_t j = 0; j < 4; ++j) {
         const Vector3f& v = get_coord_3d(quads[i][j]);
         auto s = (v[d0] - bbox.min(d0)) / range[d0];
         auto t = (v[d1] - bbox.min(d1)) / range[d0];
         Vector2f tex_vec(s, t);
         (*tex_coord_array)[k++] = tex_vec;
       }
     }
    }
    break;

   case PT_POLYGONS:
    {
     const auto& polys = polygon_coord_indices();
     size_t k(0);
     for (auto pit = polys.begin(); pit != polys.end(); ++pit) {
       for (auto vit = pit->begin(); vit != pit->end(); ++vit) {
         const Vector3f& v = get_coord_3d(*vit);
         auto s = (v[d0] - bbox.min(d0)) / range[d0];
         auto t = (v[d1] - bbox.min(d1)) / range[d0];
         Vector2f tex_vec(s, t);
         (*tex_coord_array)[k++] = tex_vec;
       }
     }
    }
    break;

   case PT_TRIANGLE_STRIP:
   case PT_TRIANGLE_FAN:
   case PT_QUAD_STRIP:
   default: SGAL_assertion(0); break;
  }
}

//! \brief cleans the 3D texture-mapping coordinate array and coordinate indices.
void Boundary_set::clean_tex_coords_3d()
{
  auto coords = get_coord_array();
  SGAL_assertion(coords);
  auto num_coords = coords->size();
  if (m_tex_coord_array) m_tex_coord_array->resize(num_coords);
  else {
    m_tex_coord_array.reset(new Tex_coord_array_3d(num_coords));
    SGAL_assertion(m_tex_coord_array);
  }
  boost::shared_ptr<Tex_coord_array_3d> tex_coords =
    boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
  SGAL_assertion(tex_coords);

  for (auto k = 0; k < tex_coords->size(); ++k) {
    (*tex_coords)[k].sub(get_coord_3d(k), m_center);
    (*tex_coords)[k].normalize();
  }
}

//! \brief draws the mesh conditionaly.
void Boundary_set::draw(Draw_action* action)
{
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (empty_facet_indices(m_facet_coord_indices)) return;
  if (!m_coord_array || m_coord_array->empty()) return;

  // Clean the center
  if (is_dirty_center()) clean_center();

  // Clean the normals or the colors.
  //! \todo clean the nromals only if lighting is enabled.
  if (resolve_fragment_source() == FS_NORMAL) {
    if (m_dirty_normal_array) clean_normals();
    if (is_dirty_facet_normal_indices()) clean_facet_normal_indices();
  }
  else {
    if (is_dirty_facet_color_indices()) clean_facet_color_indices();
  }

  // Clean the tex coordinates.
  Context* context = action->get_context();
  if (context->get_tex_enable() && !(context->get_tex_gen_enable())) {
    if (m_dirty_tex_coord_array) {
      const Context* context = action->get_context();
      boost::shared_ptr<Texture> texture = context->get_texture();
      Texture::Target target = Texture::TEXTURE_2D_ARRAY;
      if (texture) target = texture->get_target();
      // If texture-mapping is enabled and automatic (opengl) texture
      // coordinate generation is not enabled, generate the texture
      // coordinate as last resort.
      clean_tex_coords(target);
    }
    if (is_dirty_facet_tex_coord_indices()) clean_facet_tex_coord_indices();
  }

  draw_mesh(action);
}

/*! \brief draws the geometry.
 * For efficiency reasons, differenrt methods were written to
 * draw geometries with different kinds of data (texture/normal/color).
 */
void Boundary_set::draw_geometry(Draw_action* action)
{
  switch (m_drawing_mode) {
   case Configuration::GDM_DISPLAY_LIST:
    if (m_display_list_id != 0) {
      glCallList(m_display_list_id);
      break;
    }
    draw_dispatch(action);
    break;

   case Configuration::GDM_VERTEX_ARRAY:
    if (use_vertex_array()) {
      bool color_normal_condition = (resolve_fragment_source() == FS_COLOR) ?
        ((m_color_attachment == AT_PER_VERTEX) &&
         (empty_facet_indices(m_facet_color_indices) ||
         (equal_facet_indices(m_facet_coord_indices, m_facet_color_indices)))) :
        ((m_normal_attachment == AT_PER_VERTEX) &&
         (empty_facet_indices(m_facet_normal_indices) ||
         (equal_facet_indices(m_facet_coord_indices, m_facet_normal_indices))));

      bool tex_coord_condition =
        (empty_facet_indices(m_facet_tex_coord_indices) ||
         (equal_facet_indices(m_facet_coord_indices, m_facet_tex_coord_indices)));

      //! \todo Add a try() and catch() to catch errors.
      // If an error is cought, call destroy_vertex_buffers()
      if (color_normal_condition && tex_coord_condition) {
        /* If each of the normal/color indices and the texture coordinate
         * indices is either empty or identical to the coordinate indices,
         * use vertex array with the coordinates, colors/normals, and
         * texture cooridinates as is.
         */
        if (m_dirty_coord_buffer && m_coord_array)
          clean_vertex_coord_buffer(m_coord_array->data_size(),
                                    m_coord_array->data());
        if (m_dirty_normal_buffer && m_normal_array)
          clean_vertex_normal_buffer(m_normal_array->data_size(),
                                     m_normal_array->data());
        if (m_dirty_color_buffer && m_color_array)
          clean_vertex_color_buffer(m_color_array->data_size(),
                                    m_color_array->data());
        if (m_dirty_tex_coord_buffer && m_tex_coord_array)
          clean_vertex_tex_coord_buffer(m_tex_coord_array->data_size(),
                                        m_tex_coord_array->data());
      }
      else {
        /* If each of the normal/color indices and the texture coordinate
         * indices is not empty and not identical to the coordinate indices,
         * create local coordinate, color/normal, texture coordinate, and
         * indices buffers that will serve as mirrors to the OpenGL buffers.
         */
        if (m_dirty_local_vertex_buffers) {
          if (m_normal_array) {
            if (m_color_array) {
              if (!m_tex_coord_array) clean_local_cnc_vertex_buffers();
              else clean_local_cnct_vertex_buffers();
            }
            else {
              if (!m_tex_coord_array) clean_local_cn_vertex_buffers();
              else clean_local_cnt_vertex_buffers();
            }
          }
          else {
            if (m_color_array) {
              if (!m_tex_coord_array) clean_local_cc_vertex_buffers();
              else clean_local_cct_vertex_buffers();
            }
            else clean_local_ct_vertex_buffers();
          }
        }

        // Clean OpenGL vertex array buffers:
        if (m_dirty_coord_buffer && !m_local_coord_buffer.empty()) {
          auto size = m_local_coord_buffer.size() * sizeof(Vector3f);
          clean_vertex_coord_buffer(size, local_coord_data());
        }
        if (m_dirty_normal_buffer && !m_local_normal_buffer.empty()) {
          auto size = m_local_normal_buffer.size() * sizeof(Vector3f);
          clean_vertex_normal_buffer(size, local_normal_data());
        }
        auto color_size = color_data_size();
        if (m_dirty_color_buffer && (0 != color_size))
          clean_vertex_color_buffer(color_size, color_data());

        auto tex_coord_size = tex_coord_data_size();
        if (m_dirty_tex_coord_buffer && (0 != tex_coord_size))
          clean_vertex_tex_coord_buffer(tex_coord_size, tex_coord_data());
      }
    }

   case Configuration::GDM_DIRECT:
    draw_dispatch(action);
    break;
  }
}

//! \brief dispatches the appropriate drawing routine.
void Boundary_set::draw_dispatch(Draw_action* /* action */)
{
  Boolean va = use_vertex_array();

  Fragment_source fragment_source = resolve_fragment_source();
  Boolean fragment_indexed = (fragment_source == FS_NORMAL) ?
    (!empty_facet_indices(m_facet_normal_indices)) :
    (!empty_facet_indices(m_facet_color_indices));
  Attachment fragment_attached = (fragment_source == FS_NORMAL) ?
    m_normal_attachment  : m_color_attachment;
  Boolean texture_enbaled = m_tex_coord_array ? true : false;
  Boolean texture_indexed = !empty_facet_indices(m_facet_tex_coord_indices) ||
    !m_tex_coord_indices.empty();

  // std::cout << "fragment_source: " << fragment_source << std::endl;
  // std::cout << "fragment_indexed: " << fragment_indexed << std::endl;
  // std::cout << "fragment_attached: " << fragment_attached << std::endl;
  // std::cout << "texture_enbaled: " << texture_enbaled << std::endl;
  // std::cout << "texture_indexed: " << texture_indexed << std::endl;
  // std::cout << "m_primitive_type: " << m_primitive_type << std::endl;
  // std::cout << "va: " << va << std::endl;

  if (va) {
    Boundary_set::draw_FAPV_VAYE();
    return;
  }

  Uint mask =
    set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,fragment_source,
      set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,fragment_indexed,
        set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,fragment_attached,
          set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,texture_enbaled,
            set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,texture_indexed,
              set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,m_primitive_type,
                set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,va,0x0)))))));

  (this->*m_draws[mask])();
}

//! \brief isects direct drawing-mode.
void Boundary_set::isect_direct()
{
  if (!m_coord_array || m_coord_array->empty()) return;

  switch (m_primitive_type) {
   case PT_TRIANGLE_STRIP:
    // {
    //   int num_tri_strips = m_tri_strip_lengths[0];
    //   int index = 0;
    //   for (int strip = 0; strip < num_tri_strips; ++strip) {
    //     int tmp = strip + 1;
    //     glBegin(GL_TRIANGLE_STRIP);
    //     for (Uint i = 0 ; i < m_tri_strip_lengths[tmp]; ++i)
    //       glVertex3fv(m_coord_array->datum(m_flat_coord_indices[index++]));
    //     glEnd();
    //   }
    // }
    SGAL_assertion_msg(0, "Not implemented yet!");
    return;

   case PT_TRIANGLES:
    {
     glBegin(GL_TRIANGLES);
     const auto& tris = triangle_coord_indices();
     for (size_t i = 0; i < m_num_primitives; ++i) {
       glVertex3fv(m_coord_array->datum(tris[i][0]));
       glVertex3fv(m_coord_array->datum(tris[i][1]));
       glVertex3fv(m_coord_array->datum(tris[i][2]));
     }
     glEnd();
    }
    return;

   case PT_QUADS:
    {
     glBegin(GL_QUADS);
     const auto& quads = quad_coord_indices();
     for (size_t i = 0; i < m_num_primitives; ++i) {
       glVertex3fv(m_coord_array->datum(quads[i][0]));
       glVertex3fv(m_coord_array->datum(quads[i][1]));
       glVertex3fv(m_coord_array->datum(quads[i][2]));
       glVertex3fv(m_coord_array->datum(quads[i][3]));
     }
     glEnd();
    }
    return;

   case PT_POLYGONS:
    {
     const auto& polygons = polygon_coord_indices();
     for (size_t i = 0; i < m_num_primitives; ++i) {
       glBegin(GL_POLYGON);
       for (size_t j = 0; j < polygons[i].size(); ++j)
         glVertex3fv(m_coord_array->datum(polygons[i][j]));
       glEnd();
     }
    }
    return;

   case PT_TRIANGLE_FAN:
   case PT_QUAD_STRIP:
   default:
    SGAL_assertion_msg(0, "Not implemented yet!");
    return;
  }
}

//! \brief draws the mesh in selection mode.
void Boundary_set::isect(Isect_action* action)
{
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (empty_facet_indices(m_facet_coord_indices)) return;

  Context* context = action->get_context();
  if (!m_is_solid && context) context->draw_cull_face(Gfx::NO_CULL);

  /*! \todo OPTIMIZE - maybe we should keep the vertices even if there is a
   * display list, so we can draw here without normals and text coords
   */
  switch (m_drawing_mode) {
   case Configuration::GDM_DIRECT:
    isect_direct();
    break;

   case Configuration::GDM_DISPLAY_LIST:
    glCallList(m_display_list_id);
    break;

   case Configuration::GDM_VERTEX_ARRAY:
    //! \todo Implement isect with vertex-array
    isect_direct();
    break;
  }

  if (!m_is_solid  && context) context->draw_cull_face(Gfx::BACK_CULL);
}

//! \brief creates a new display list. This is called after each update.
int Boundary_set::create_display_list(Draw_action* action)
{
  int id = glGenLists(1);
  glNewList(id, GL_COMPILE);
  draw_dispatch(action);
  glEndList();
  return id;
}

//! \brief sets the attributes of the object.
void Boundary_set::set_attributes(Element* elem)
{
  Mesh_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "normalPerVertex") {
      set_normal_per_vertex(to_boolean(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "colorPerVertex") {
      set_color_per_vertex(to_boolean(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();

  // \todo Need to check that the number of coordIndices is equal to the
  // number of texCoordIndices (and normalIndices and/or colorIndex in
  // case of normalPerVertex or colorPerVertex is true resp,, or
  // normalIndices and/or colorIndex is equal to the number of coords
  // in case normalPerVertex or colorPerVertex is false resp.
}

//! \brief adds the container to a given scene.
void Boundary_set::add_to_scene(Scene_graph* sg)
{
  const auto* config = sg->get_configuration();
  if (config) {
    m_drawing_mode = config->get_geometry_drawing_mode();
    m_use_vertex_buffer_object = config->do_use_vertex_buffer_object();
  }
}

//! \brief initializes the container prototype.
void Boundary_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());

  // Add the field-info records to the prototype:
  // normalPerVertex
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto normal_per_vertex_func =
    static_cast<Boolean_handle_function>
    (&Boundary_set::normal_per_vertex_handle);
  s_prototype->add_field_info(new SF_bool(NORMAL_PER_VERTEX,
                                          "normalPerVertex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          normal_per_vertex_func,
                                          s_def_normal_per_vertex, exec_func));

  // colorPerVertex
  auto color_per_vertex_func =
    static_cast<Boolean_handle_function>
    (&Boundary_set::color_per_vertex_handle);
  s_prototype->add_field_info(new SF_bool(COLOR_PER_VERTEX,
                                          "colorPerVertex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          color_per_vertex_func,
                                          s_def_color_per_vertex, exec_func));
}

//! \brief deletes the container prototype.
void Boundary_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Boundary_set::get_prototype()
{
  if (s_prototype == nullptr) Boundary_set::init_prototype();
  return s_prototype;
}

//! \brief destroys the data structure of the vertex buffer object.
void Boundary_set::destroy_vertex_buffers()
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_coord_buffer_id || m_normal_buffer_id ||
      m_color_buffer_id || m_tex_coord_buffer_id)
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  if (m_coord_buffer_id) {
    glDeleteBuffersARB(1, &m_coord_buffer_id);
    m_coord_buffer_id = 0;
  }
  if (m_normal_buffer_id) {
    glDeleteBuffersARB(1, &m_normal_buffer_id);
    m_normal_buffer_id = 0;
  }
  if (m_color_buffer_id) {
    glDeleteBuffersARB(1, &m_color_buffer_id);
    m_color_buffer_id = 0;
  }
  if (m_tex_coord_buffer_id) {
    glDeleteBuffersARB(1, &m_tex_coord_buffer_id);
    m_tex_coord_buffer_id = 0;
  }
#endif
  m_dirty_coord_buffer = true;
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief destroys the data structure of the display_list.
void Boundary_set::destroy_display_list()
{
  if (m_display_list_id != 0) {
    glDeleteLists(m_display_list_id, 1);
    m_display_list_id = 0;
  }
}

//! \brief clear the vertex buffers.
void Boundary_set::clear_local_vertex_buffers()
{
  m_local_coord_buffer.clear();
  m_local_normal_buffer.clear();
  m_local_color_buffer_3d.clear();
  m_local_color_buffer_4d.clear();
  m_local_tex_coord_buffer_2d.clear();
  m_local_tex_coord_buffer_3d.clear();
  m_local_tex_coord_buffer_4d.clear();
}

//! \brief Calculate a single normal per vertex for all vertices.
void Boundary_set::
calculate_single_normal_per_vertex(Shared_normal_array normals)
{
  SGAL_assertion(get_coord_array());
  SGAL_assertion(normals->size() == get_coord_array()->size());
  // Calculate the normals of all facets.
  Normal_array per_polygon_normals(m_num_primitives);
  calculate_normal_per_facet(per_polygon_normals);

  // Initialize the weights:
  Vertices_info vertices_info;
  calculate_vertices_info(vertices_info);

  // Calculate the weighted normals:
  Uint j;
  for (j = 0; j < vertices_info.size(); ++j) {
    Float weight_sum = 0;
    Vertex_info_const_iter it;
    Vector3f n;
    for (it = vertices_info[j].begin(); it != vertices_info[j].end(); ++it) {
      Uint facet_index = it->first;
      Float weight = it->second;
      const Vector3f& normal = per_polygon_normals[facet_index];

      weight_sum += weight;                     // accumulate the weight
      Vector3f tmp;
      tmp.scale(weight, normal);
      n.add(tmp);
    }
    n.scale(1.0f / weight_sum);
    n.normalize();
    (*normals)[j] = n;
  }

  for (j = 0; j < vertices_info.size(); j++) vertices_info[j].clear();
  vertices_info.clear();

  clear_facet_normal_indices();
}

//! \brief cleans the data structure of the vertex coordinate buffer object.
void Boundary_set::clean_vertex_coord_buffer(Uint size, const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_coord_buffer_id == 0) glGenBuffersARB(1, &m_coord_buffer_id);
  SGAL_assertion(m_coord_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_coord_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, const_cast<GLfloat*>(data),
                  GL_DYNAMIC_DRAW_ARB);

  // Verify that everything is ok:
#if !defined(NDEBUG)
  int param_array_size = 0;
  glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB,
                            &param_array_size);
  SGAL_assertion(param_array_size > 0);
#endif

  // Leave clean state:
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#endif
  m_dirty_coord_buffer = false;
}

//! \brief cleans the data structure of the vertex normal buffer object.
void Boundary_set::clean_vertex_normal_buffer(Uint size,
                                                  const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_normal_buffer_id == 0) glGenBuffersARB(1, &m_normal_buffer_id);
  SGAL_assertion(m_normal_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_normal_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, const_cast<GLfloat*>(data),
                  GL_DYNAMIC_DRAW_ARB);
#if !defined(NDEBUG)
  int param_array_size = 0;
  glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB,
                            &param_array_size);
  SGAL_assertion(param_array_size > 0);
#endif

  // Leave clean state:
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#endif
  m_dirty_normal_buffer = false;
}

//! \brief cleans the data structure of the vertex color buffer object.
void Boundary_set::clean_vertex_color_buffer(Uint size, const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_color_buffer_id == 0) glGenBuffersARB(1, &m_color_buffer_id);
  SGAL_assertion(m_color_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_color_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, const_cast<GLfloat*>(data),
                  GL_DYNAMIC_DRAW_ARB);

#if !defined(NDEBUG)
  int param_array_size = 0;
  glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB,
                            &param_array_size);
  SGAL_assertion(param_array_size > 0);
#endif

  // Leave clean state:
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#endif
  m_dirty_color_buffer = false;
}

/*! \brief cleans the data structure of the vertex texture coordinate buffer
 * object.
 */
void Boundary_set::clean_vertex_tex_coord_buffer(Uint size, const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_tex_coord_buffer_id == 0) glGenBuffersARB(1, &m_tex_coord_buffer_id);
  SGAL_assertion(m_tex_coord_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_tex_coord_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, const_cast<GLfloat*>(data),
                  GL_DYNAMIC_DRAW_ARB);

#if !defined(NDEBUG)
  int param_array_size = 0;
  glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB,
                            &param_array_size);
  SGAL_assertion(param_array_size > 0);
#endif

  // Leave clean state:
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#endif
  m_dirty_tex_coord_buffer = false;
}

//! \brief computes flat indices for the normals or for the colors per-primitive.
void
Boundary_set::compute_flat_indices_per_primitive(const Facet_indices& indices,
                                                 Flat_indices& target)
{
  if (empty_facet_indices(indices)) {
    if (PT_TRIANGLES == m_primitive_type) {
      size_t j(0);
      for (size_t i = 0; i < get_num_primitives(); ++i) {
        target[j++] = i;
        target[j++] = i;
        target[j++] = i;
      }
      return;
    }
    SGAL_assertion(PT_QUADS == m_primitive_type);
    size_t j(0);
    for (size_t i = 0; i < get_num_primitives(); ++i) {
      target[j++] = i;
      target[j++] = i;
      target[j++] = i;
      target[j++] = i;
    }
    return;
  }
  auto& source = boost::get<Flat_indices>(indices);
  if (PT_TRIANGLES == m_primitive_type) {
    size_t j(0);
    for (size_t i = 0; i < get_num_primitives(); ++i) {
      target[j++] = source[i];
      target[j++] = source[i];
      target[j++] = source[i];
    }
    return;
  }
  SGAL_assertion(PT_QUADS == m_primitive_type);
  size_t j(0);
  for (size_t i = 0; i < get_num_primitives(); ++i) {
    target[j++] = source[i];
    target[j++] = source[i];
    target[j++] = source[i];
    target[j++] = source[i];
  }
}

//! \brief computes flat indices for the colors per-mesh.
void Boundary_set::compute_flat_indices_per_mesh(const Facet_indices& indices,
                                                 Flat_indices& target)
{
  if (empty_facet_indices(indices)) {
    if (PT_TRIANGLES == m_primitive_type) {
      size_t j(0);
      for (size_t i = 0; i < get_num_primitives(); ++i) {
        target[j++] = 0;
        target[j++] = 0;
        target[j++] = 0;
      }
      return;
    }
    SGAL_assertion(PT_QUADS == m_primitive_type);
    size_t j(0);
    for (size_t i = 0; i < get_num_primitives(); ++i) {
      target[j++] = 0;
      target[j++] = 0;
      target[j++] = 0;
      target[j++] = 0;
    }
    return;
  }
  auto& source = boost::get<Flat_indices>(indices);
  if (PT_TRIANGLES == m_primitive_type) {
    size_t j(0);
    for (size_t i = 0; i < get_num_primitives(); ++i) {
      target[j++] = source[0];
      target[j++] = source[0];
      target[j++] = source[0];
    }
    return;
  }
  SGAL_assertion(PT_QUADS == m_primitive_type);
  size_t j(0);
  for (size_t i = 0; i < get_num_primitives(); ++i) {
    target[j++] = source[0];
    target[j++] = source[0];
    target[j++] = source[0];
    target[j++] = source[0];
  }
}

//! \brief cleans the local coordinates, normals, and color, vertex buffers.
void Boundary_set::clean_local_cnc_vertex_buffers()
{
  SGAL_assertion(m_normal_array);

  Facet_indices tmp_facet_normal_indices = Flat_indices();
  auto& tmp_normal_indices = boost::get<Flat_indices>(tmp_facet_normal_indices);
  if (m_normal_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_normal_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_normal_indices,
                                       tmp_normal_indices);
  }
  Facet_indices tmp_facet_color_indices = Flat_indices();
  auto& tmp_color_indices = boost::get<Flat_indices>(tmp_facet_color_indices);
  if (m_color_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_color_indices,
                                       tmp_color_indices);
  }
  else if (m_color_attachment == AT_PER_MESH) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_mesh(m_facet_color_indices, tmp_color_indices);
  }

  auto ni_begin = (m_normal_attachment == AT_PER_PRIMITIVE) ?
    begin_facet_indices(tmp_facet_normal_indices) :
    (empty_facet_indices(m_facet_normal_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_normal_indices));
  auto ci_begin =
    ((m_color_attachment == AT_PER_PRIMITIVE) ||
     (m_color_attachment == AT_PER_MESH)) ?
    begin_facet_indices(tmp_facet_color_indices) :
    (empty_facet_indices(m_facet_color_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_color_indices));
  SGAL_assertion(ni_begin != ci_begin);

  auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
  if (ca)
    clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                  ni_begin,
                                  ca, m_local_color_buffer_3d, ci_begin);
  else {
    auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
    SGAL_assertion(ca);
    clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                  ni_begin,
                                  ca, m_local_color_buffer_4d, ci_begin);
  }

  tmp_normal_indices.clear();
  tmp_color_indices.clear();
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
}

/*! \brief cleans the local coordinates, normals, color, and 2d texture
 * coordinates vertex buffers.
 */
void Boundary_set::clean_local_cnct_vertex_buffers()
{
  SGAL_assertion(m_normal_array);

  Facet_indices tmp_facet_normal_indices = Flat_indices();
  auto& tmp_normal_indices = boost::get<Flat_indices>(tmp_facet_normal_indices);
  if (m_normal_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_normal_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_normal_indices,
                                       tmp_normal_indices);
  }
  Facet_indices tmp_facet_color_indices = Flat_indices();
  auto& tmp_color_indices = boost::get<Flat_indices>(tmp_facet_color_indices);
  if (m_color_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_color_indices,
                                       tmp_color_indices);
  }
  else if (m_color_attachment == AT_PER_MESH) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_mesh(m_facet_color_indices, tmp_color_indices);
  }

  auto ni_begin = (m_normal_attachment == AT_PER_PRIMITIVE) ?
    begin_facet_indices(tmp_facet_normal_indices) :
    (empty_facet_indices(m_facet_normal_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_normal_indices));
  auto ci_begin =
    ((m_color_attachment == AT_PER_PRIMITIVE) ||
     (m_color_attachment == AT_PER_MESH)) ?
    begin_facet_indices(tmp_facet_color_indices) :
    (empty_facet_indices(m_facet_color_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_color_indices));
  auto ti_begin =
    empty_facet_indices(m_facet_tex_coord_indices) ?
    begin_facet_indices(m_facet_coord_indices) :
    begin_facet_indices(m_facet_tex_coord_indices);
  SGAL_assertion((ni_begin != ci_begin) || (ci_begin != ti_begin));

  auto ta = boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  if (ta) {
    auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
    if (ca)
      clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                    ni_begin,
                                    ca, m_local_color_buffer_3d, ci_begin,
                                    ta, m_local_tex_coord_buffer_2d, ti_begin);
    else {
      auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
      SGAL_assertion(ca);
      clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                    ni_begin,
                                    ca, m_local_color_buffer_4d, ci_begin,
                                    ta, m_local_tex_coord_buffer_2d, ti_begin);
    }
  }
  else {
    auto ta = boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
    if (ta) {
      auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
      if (ca)
        clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                      ni_begin,
                                      ca, m_local_color_buffer_3d, ci_begin,
                                      ta, m_local_tex_coord_buffer_3d, ti_begin);
      else {
        auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
        SGAL_assertion(ca);
        clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                      ni_begin,
                                      ca, m_local_color_buffer_4d, ci_begin,
                                      ta, m_local_tex_coord_buffer_3d, ti_begin);
      }
    }
    else {
      auto ta =
        boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
      SGAL_assertion(ta);
      auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
      if (ca)
        clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                      ni_begin,
                                      ca, m_local_color_buffer_3d, ci_begin,
                                      ta, m_local_tex_coord_buffer_4d, ti_begin);
      else {
        auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
        SGAL_assertion(ca);
        clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                      ni_begin,
                                      ca, m_local_color_buffer_4d, ci_begin,
                                      ta, m_local_tex_coord_buffer_4d, ti_begin);
      }
    }
  }
  tmp_normal_indices.clear();
  tmp_color_indices.clear();
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the local coordinates and normals vertex buffers.
void Boundary_set::clean_local_cn_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  Facet_indices tmp_facet_normal_indices = Flat_indices();
  auto& tmp_normal_indices = boost::get<Flat_indices>(tmp_facet_normal_indices);
  if (m_normal_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_normal_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_normal_indices,
                                       tmp_normal_indices);
  }
  auto ni_begin =
    (m_normal_attachment == AT_PER_PRIMITIVE) ?
    begin_facet_indices(tmp_facet_normal_indices) :
    (empty_facet_indices(m_facet_normal_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_normal_indices));
  clean_local_2d_vertex_buffers(m_normal_array, m_local_normal_buffer, ni_begin);
  tmp_normal_indices.clear();
  m_dirty_normal_buffer = true;
}

/*! \brief cleans the local coordinates, normals, and 2d texture coordinates
 * vertex buffers.
 */
void Boundary_set::clean_local_cnt_vertex_buffers()
{
  SGAL_assertion(m_normal_array);

  Facet_indices tmp_facet_normal_indices = Flat_indices();
  auto& tmp_normal_indices = boost::get<Flat_indices>(tmp_facet_normal_indices);
  if (m_normal_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_normal_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_normal_indices,
                                       tmp_normal_indices);
  }

  auto ni_begin = (m_normal_attachment == AT_PER_PRIMITIVE) ?
    begin_facet_indices(tmp_facet_normal_indices) :
    (empty_facet_indices(m_facet_normal_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_normal_indices));
  auto ti_begin =
    empty_facet_indices(m_facet_tex_coord_indices) ?
    begin_facet_indices(m_facet_coord_indices) :
    begin_facet_indices(m_facet_tex_coord_indices);
  SGAL_assertion(ni_begin != ti_begin);

  auto ta = boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  if (ta)
    clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                  ni_begin,
                                  ta, m_local_tex_coord_buffer_2d, ti_begin);
  else {
    auto ta = boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
    if (ta)
      clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                    ni_begin,
                                    ta, m_local_tex_coord_buffer_3d, ti_begin);
    else {
      auto ta =
        boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
      SGAL_assertion(ta);
      clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                    ni_begin,
                                    ta, m_local_tex_coord_buffer_4d, ti_begin);
    }
  }

  tmp_normal_indices.clear();
  m_dirty_normal_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the local coordinates and colors vertex buffers.
void Boundary_set::clean_local_cc_vertex_buffers()
{
  Facet_indices tmp_facet_color_indices = Flat_indices();
  auto& tmp_color_indices = boost::get<Flat_indices>(tmp_facet_color_indices);
  if (m_color_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_color_indices,
                                       tmp_color_indices);
  }
  else if (m_color_attachment == AT_PER_MESH) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_mesh(m_facet_color_indices, tmp_color_indices);
  }

  auto ci_begin =
    ((m_color_attachment == AT_PER_PRIMITIVE) ||
     (m_color_attachment == AT_PER_MESH)) ?
    begin_facet_indices(tmp_facet_color_indices) :
    (empty_facet_indices(m_facet_color_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_color_indices));


  auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
  if (ca)
    clean_local_2d_vertex_buffers(ca, m_local_color_buffer_3d, ci_begin);
  else {
    auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
    clean_local_2d_vertex_buffers(ca, m_local_color_buffer_4d, ci_begin);
  }
  tmp_color_indices.clear();
  m_dirty_color_buffer = true;
}

/*! \brief cleans the local coordinates, colors, and texture coordinates vertex
 * buffers.
 */
void Boundary_set::clean_local_cct_vertex_buffers()
{
  Facet_indices tmp_facet_color_indices = Flat_indices();
  auto& tmp_color_indices = boost::get<Flat_indices>(tmp_facet_color_indices);
  if (m_color_attachment == AT_PER_PRIMITIVE) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_primitive(m_facet_color_indices,
                                       tmp_color_indices);
  }
  else if (m_color_attachment == AT_PER_MESH) {
    auto size = size_facet_indices(m_facet_coord_indices);
    tmp_color_indices.resize(size);
    compute_flat_indices_per_mesh(m_facet_color_indices, tmp_color_indices);
  }

  auto ci_begin =
    ((m_color_attachment == AT_PER_PRIMITIVE) ||
     (m_color_attachment == AT_PER_MESH)) ?
    begin_facet_indices(tmp_facet_color_indices) :
    (empty_facet_indices(m_facet_color_indices) ?
     begin_facet_indices(m_facet_coord_indices) :
     begin_facet_indices(m_facet_color_indices));
  auto ti_begin =
    empty_facet_indices(m_facet_tex_coord_indices) ?
    begin_facet_indices(m_facet_coord_indices) :
    begin_facet_indices(m_facet_tex_coord_indices);
  SGAL_assertion(ci_begin != ti_begin);


  auto ta = boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  if (ta) {
    auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
    if (ca)
      clean_local_3d_vertex_buffers(ca, m_local_color_buffer_3d, ci_begin,
                                    ta, m_local_tex_coord_buffer_2d, ti_begin);
    else {
      auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
      SGAL_assertion(ca);
      clean_local_3d_vertex_buffers(ca, m_local_color_buffer_4d, ci_begin,
                                    ta, m_local_tex_coord_buffer_2d, ti_begin);

    }
  }
  else {
    auto ta = boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
    if (ta) {
      auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
      if (ca)
        clean_local_3d_vertex_buffers(ca, m_local_color_buffer_3d, ci_begin,
                                      ta, m_local_tex_coord_buffer_3d, ti_begin);
      else {
        auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
        SGAL_assertion(ca);
        clean_local_3d_vertex_buffers(ca, m_local_color_buffer_4d, ci_begin,
                                      ta, m_local_tex_coord_buffer_3d, ti_begin);
      }
    }
    else {
      auto ta =
        boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
      SGAL_assertion(ta);
      auto ca = boost::dynamic_pointer_cast<Color_array_3d>(m_color_array);
      if (ca)
        clean_local_3d_vertex_buffers(ca, m_local_color_buffer_3d, ci_begin,
                                      ta, m_local_tex_coord_buffer_4d, ti_begin);
      else {
        auto ca = boost::dynamic_pointer_cast<Color_array_4d>(m_color_array);
        SGAL_assertion(ca);
        clean_local_3d_vertex_buffers(ca, m_local_color_buffer_4d, ci_begin,
                                      ta, m_local_tex_coord_buffer_4d, ti_begin);
      }
    }
  }

  tmp_color_indices.clear();
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the local coordinates and texture coordinates vertex buffers.
void Boundary_set::clean_local_ct_vertex_buffers()
{
  auto ti_begin =
    empty_facet_indices(m_facet_tex_coord_indices) ?
    begin_facet_indices(m_facet_coord_indices) :
    begin_facet_indices(m_facet_tex_coord_indices);
  auto ta = boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  if (ta)
    clean_local_2d_vertex_buffers(ta, m_local_tex_coord_buffer_2d, ti_begin);
  else {
    auto ta = boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
    if (ta)
      clean_local_2d_vertex_buffers(ta, m_local_tex_coord_buffer_3d, ti_begin);
    else {
      auto ta =
        boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
      SGAL_assertion(ta);
      clean_local_2d_vertex_buffers(ta, m_local_tex_coord_buffer_4d, ti_begin);
    }
  }
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the center of the geometric object.
void Boundary_set::clean_center()
{
  m_center.set(0, 0, 0);
  m_dirty_center = false;
}

//! \brief sets the center of the geometric object.
void Boundary_set::set_center(const Vector3f& center)
{
  m_center = center;
  m_dirty_center = false;
}

//! \brief obtains the center of the geometric object.
Vector3f& Boundary_set::get_center()
{
  if (m_dirty_center) clean_center();
  return m_center;
}

//! \brief determines whether it is possible using openGl vertex buffer object.
Boolean Boundary_set::use_vertex_buffer_object() const
{
  return (m_use_vertex_buffer_object &&
          Gfx_conf::get_instance()->is_vertex_buffer_object_supported());
}

//! \brief responds to a change in the coordinate-index array.
void Boundary_set::coord_indices_changed(const Field_info* field_info)
{
  m_dirty_coord_buffer = true;
  Mesh_set::coord_indices_changed(field_info);
}

//! \brief responds to a change in the normal-index array.
void Boundary_set::normal_indices_changed(const Field_info* field_info)
{
  m_dirty_normal_buffer = true;
  Mesh_set::normal_indices_changed(field_info);
}

//! \brief responds to a change in the color-index array.
void Boundary_set::color_indices_changed(const Field_info* field_info)
{
  m_dirty_color_buffer = true;
  Mesh_set::color_indices_changed(field_info);
}

//! \brief responds to a change in the texture-coordinate index array.
void Boundary_set::tex_coord_indices_changed(const Field_info* field_info)
{
  m_dirty_tex_coord_buffer = true;
  Mesh_set::tex_coord_indices_changed(field_info);
}

//! \brief responds to a change in the facet coordinate-index array.
void Boundary_set::facet_coord_indices_changed()
{
  m_dirty_coord_buffer = true;
  Mesh_set::facet_coord_indices_changed();
}

//! \brief responds to a change in the facet normal-index array.
void Boundary_set::facet_normal_indices_changed()
{
  m_dirty_normal_buffer = true;
  Mesh_set::facet_normal_indices_changed();
}

//! \brief responds to a change in the facet color-index array.
void Boundary_set::facet_color_indices_changed()
{
  m_dirty_color_buffer = true;
  Mesh_set::facet_color_indices_changed();
}

//! \brief responds to a change in the facet texture-coordinate index array.
void Boundary_set::facet_tex_coord_indices_changed()
{
  m_dirty_coord_buffer = true;
  Mesh_set::facet_tex_coord_indices_changed();
}

//! \brief writes a field of this container.
void Boundary_set::write_field(const Field_info* field_info,
                               Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    // Handle normal attachment:
    if (NORMAL_PER_VERTEX == field_info->get_id()) {
      SGAL_assertion(m_normal_attachment != AT_PER_MESH);
      const auto& name = field_info->get_name();
      vrml_formatter->single_boolean(name, get_normal_per_vertex(), true);
      return;
    }
    if (NORMAL_ATTACHMENT == field_info->get_id()) return;

    // Handle color attachment:
    if (COLOR_PER_VERTEX == field_info->get_id()) {
      if (m_color_attachment == AT_PER_MESH) return;
      const auto& name = field_info->get_name();
      vrml_formatter->single_boolean(name, get_color_per_vertex(), true);
      return;
    }
    if (COLOR_ATTACHMENT == field_info->get_id()) {
      if (m_color_attachment != AT_PER_MESH) return;
      const auto& name = field_info->get_name();
      const auto& value = attachment_name(AT_PER_MESH);
      formatter->single_string(name, value, "");
      return;
    }
  }
  Mesh_set::write_field(field_info, formatter);
}

SGAL_END_NAMESPACE
