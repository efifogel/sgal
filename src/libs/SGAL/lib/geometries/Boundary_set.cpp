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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <tuple>

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
#include "SGAL/Color_array.hpp"
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
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/Bounding_box.hpp"

SGAL_BEGIN_NAMESPACE

//! \todo #include "Model_stats.h"

Container_proto* Boundary_set::s_prototype(nullptr);

// Default values:
const Boolean Boundary_set::s_def_normal_per_vertex(false);
const Boolean Boundary_set::s_def_color_per_vertex(false);

void (Boundary_set::*Boundary_set::m_draws[SGAL_NUM_BO_DRAWS])();
Boolean Boundary_set::m_draws_initialized(false);

REGISTER_TO_FACTORY(Boundary_set, "Boundary_set");

//! \brief constructor.
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
  m_dirty_normals(true),
  m_normals_cleaned(false),
  m_dirty_tex_coords(true),
  m_tex_coords_cleaned(false),
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
  for (auto i = 0; i < SGAL_NUM_BO_DRAWS; ++i)
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

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOTR_VAYE] =
    &Boundary_set::draw_FSNO_FINO_FAPT_TENO_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TENO_MOTR_VANO;
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
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
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
    &Boundary_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Boundary_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
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

//! \brief destructor.
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

//! \brief claculates the normals in case they are invalidated.
void Boundary_set::clean_normals()
{
  if (m_normal_attachment == AT_PER_VERTEX)
    calculate_single_normal_per_vertex();
  else calculate_normal_per_polygon();
  m_dirty_normals = false;
  m_normals_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief computes the normalized normal to a triangle.
void Boundary_set::compute_triangle_normal(Uint j, Vector3f& n) const
{
  const Vector3f& v0 = get_coord_3d(m_flat_coord_indices[j]);
  const Vector3f& v1 = get_coord_3d(m_flat_coord_indices[j+1]);
  const Vector3f& v2 = get_coord_3d(m_flat_coord_indices[j+2]);
  if (is_ccw()) n.normal(v0, v1, v2);
  else n.normal(v2, v1, v0);
}

//! \brief computes the center point of a triangle.
void Boundary_set::compute_triangle_center(Uint j, Vector3f& center) const
{
  center.add(get_coord_3d(m_flat_coord_indices[j+0]));
  center.add(get_coord_3d(m_flat_coord_indices[j+1]));
  center.add(get_coord_3d(m_flat_coord_indices[j+2]));
  center.scale(1.0f / 3);
}

/*! \brief computes the vertex information for the three vertices of a
 * triangule.
 */
void Boundary_set::
compute_triangle_vertex_info(Uint j, Uint facet_index,
                             const Vector3f& center,
                             Vertices_info& vertices_info) const
{
  Uint vertex_index = m_flat_coord_indices[j+0];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
  vertex_index = m_flat_coord_indices[j+1];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
  vertex_index = m_flat_coord_indices[j+2];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
}

/*! \brief computes the normalized normal to a quadrilateral.
 * This implementation assumes that 3 (out of the 4) vertices might be
 * collinear.
 */
void Boundary_set::compute_quad_normal(Uint j, Vector3f& n) const
{
  const Vector3f& v0 = get_coord_3d(m_flat_coord_indices[j]);
  const Vector3f& v1 = get_coord_3d(m_flat_coord_indices[j+1]);
  const Vector3f& v2 = get_coord_3d(m_flat_coord_indices[j+2]);
  if (!v0.collinear(v0, v1, v2)) {
    if (is_ccw()) n.normal(v0, v1, v2);
    else n.normal(v2, v1, v0);
    return;
  }
  const Vector3f& v3 = get_coord_3d(m_flat_coord_indices[j+3]);
  SGAL_assertion(!v0.collinear(v0, v1, v3));
  if (is_ccw()) n.normal(v0, v1, v3);
  else n.normal(v3, v1, v0);
}

//! \brief computes the center point of a quadrilateral.
void Boundary_set::compute_quad_center(Uint j, Vector3f& center) const
{
  center.add(get_coord_3d(m_flat_coord_indices[j+0]));
  center.add(get_coord_3d(m_flat_coord_indices[j+1]));
  center.add(get_coord_3d(m_flat_coord_indices[j+2]));
  center.add(get_coord_3d(m_flat_coord_indices[j+3]));
  center.scale(1.0f / 4);
}

/*! \brief computes the vertex information for the four vertices of a
 * quadrilateral.
 */
void
Boundary_set::compute_quad_vertex_info(Uint j, Uint facet_index,
                                           const Vector3f& center,
                                           Vertices_info& vertices_info) const
{
  Uint vertex_index = m_flat_coord_indices[j+0];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
  vertex_index = m_flat_coord_indices[j+1];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
  vertex_index = m_flat_coord_indices[j+2];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
  vertex_index = m_flat_coord_indices[j+3];
  compute_vertex_info(vertex_index, facet_index, center,
                      std::back_inserter(vertices_info[vertex_index]));
}

/*! \brief computes the normalized normal to a polygon.
 * This implementation assumes that consecuitive vertices might be collinear.
 */
void Boundary_set::compute_polygon_normal(Uint j, Vector3f& n) const
{
  const Vector3f& v0 = get_coord_3d(m_coord_indices[j]);
  const Vector3f& v1 = get_coord_3d(m_coord_indices[j+1]);
  for (Uint k = 2; m_coord_indices[j+k] != (Uint) -1; ++k) {
    const Vector3f& v2 = get_coord_3d(m_coord_indices[j+k]);
    if (v0.collinear(v0, v1, v2)) continue;
    if (is_ccw()) n.normal(v0, v1, v2);
    else n.normal(v2, v1, v0);
    return;
  }
  SGAL_assertion_msg(0, "All vertices are collinear!");
}

//! \brief computes the center point of a polygon.
Uint Boundary_set::compute_polygon_center(Uint j, Vector3f& center) const
{
  Uint k;
  for (k = 0; m_coord_indices[j+k] != (Uint) -1; ++k)
    center.add(get_coord_3d(m_coord_indices[j+k]));
  center.scale(1.0f / k);
  return k;
}

//! \brief computes the vertex information for the all vertices of a polygon.
void Boundary_set::
compute_polygon_vertex_info(Uint j, Uint facet_index, Uint k,
                            const Vector3f& center,
                            Vertices_info& vertices_info) const
{
  for (Uint l = 0; l < k; ++l) {
    Uint vertex_index = m_coord_indices[j+l];
    compute_vertex_info(vertex_index, facet_index, center,
                        back_inserter(vertices_info[vertex_index]));
  }
}

//! \brief calculates vertex information.
void Boundary_set::calculate_vertices_info(Vertices_info& vertices_info)
{
  SGAL_assertion(m_coord_array);

  vertices_info.resize(m_coord_array->size());

  Uint i, j = 0;
  // Assume that the facet is planar, and compute its normal:
  switch (m_primitive_type) {
   case PT_TRIANGLES:
    for (i = 0; i < m_num_primitives; ++i) {
      Vector3f center;
      compute_triangle_center(j, center);
      compute_triangle_vertex_info(j, i, center, vertices_info);
      j += 3;
    }
    break;

   case PT_QUADS:
    for (i = 0; i < m_num_primitives; ++i) {
      Vector3f center;
      compute_quad_center(j, center);
      compute_quad_vertex_info(j, i, center, vertices_info);
      j += 4;
    }
    break;

   case PT_POLYGONS:
    for (i = 0; i < m_num_primitives; ++i) {
      Vector3f center;
      Uint k = compute_polygon_center(j, center);
      compute_polygon_vertex_info(j, i, k, center, vertices_info);
      j += k + 1;       // skip the end-of-face marker
    }
    break;

   case PT_TRIANGLE_STRIP:
   case PT_TRIANGLE_FAN:
   case PT_QUAD_STRIP:
   default: SGAL_assertion(0); break;
  }
}

//! \brief calculates a single normal per vertex for all vertices.
void Boundary_set::calculate_single_normal_per_vertex()
{
  SGAL_assertion(m_coord_array);
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array(m_coord_array->size()));
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->resize(m_coord_array->size());
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

//! \brief calculates a single normal per polygon for all polygons.
void Boundary_set::calculate_normal_per_polygon(Normal_array& normals)
{
  SGAL_assertion(normals.size() == m_num_primitives);
  Uint j = 0;
  for (Uint i = 0; i < m_num_primitives; ++i) {
    Vector3f normal;

    switch (m_primitive_type) {
     case PT_TRIANGLES:
      compute_triangle_normal(j, normals[i]);
      j += 3;
      break;

     case PT_QUADS:
      compute_quad_normal(j, normals[i]);
      j += 4;
      break;

     case PT_POLYGONS:
      compute_polygon_normal(j, normals[i]);
      for (; m_coord_indices[j] != (Uint) -1; ++j);     // advance to end
      ++j;                                      // skip the end-of-face marker
      break;

     case PT_TRIANGLE_STRIP: break;
     case PT_TRIANGLE_FAN: break;
     case PT_QUAD_STRIP:
     default: SGAL_assertion(0); break;
    }
  }
  clear_flat_normal_indices();
}

//! \brief calculates a single normal per polygon for all polygons.
void Boundary_set::calculate_normal_per_polygon()
{
  SGAL_assertion(m_coord_array);
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array(m_num_primitives));
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->resize(m_num_primitives);
  calculate_normal_per_polygon(*m_normal_array);
  set_normal_per_vertex(false);
}

/*! \brief calculates the default texture-mapping oordinates in case they are
 * dirty using the shape bounding-box.
 */
void Boundary_set::clean_tex_coords(Texture::Target target)
{
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
  m_dirty_tex_coords = false;
  m_tex_coords_cleaned = true;
  m_dirty_tex_coord_buffer = true;
}

void Boundary_set::compute_flat_tex_coords_2d(size_t num_verts) {}
void Boundary_set::compute_polygon_tex_coords_2d() {}

//! \brief calculates the default 2D texture-mapping oordinates.
void Boundary_set::clean_tex_coords_2d()
{
  SGAL_assertion(m_coord_array);

  // Compute bounding box
  Bounding_box bbox = bounding_box();;

  // Allocate space for texture coordinates and texture cordinate indices
  Uint size = (m_coord_indices_flat) ?
    m_flat_coord_indices.size() : m_coord_indices.size() - m_num_primitives;

  if (m_tex_coord_array) m_tex_coord_array->resize(size);
  else {
    m_tex_coord_array.reset(new Tex_coord_array_2d(size));
    SGAL_assertion(m_tex_coord_array);
  }
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);

  // Generate indices
  if (m_coord_indices_flat) {
    Uint k(0);
    m_flat_tex_coord_indices.resize(size);
    auto it = m_flat_tex_coord_indices.begin();
    for (; it != m_flat_tex_coord_indices.end(); ++it) *it = k++;
    m_tex_coord_indices_flat = true;
    m_dirty_flat_tex_coord_indices = false;
    m_dirty_tex_coord_indices = true;
  }
  else {
    m_tex_coord_indices.resize(size + m_num_primitives);
    Uint j(0);
    Uint k(0);
    for (Uint i = 0; i < m_num_primitives; ++i) {
      while (m_coord_indices[j] != (Uint) -1)
        m_tex_coord_indices[j++] = k++;
      m_tex_coord_indices[j++] = (Uint) -1;
    }
    m_tex_coord_indices_flat = false;
    m_dirty_flat_tex_coord_indices = true;
    m_dirty_tex_coord_indices = false;
  }

  // Generate coordinates
  auto dims = bbox.get_longest_dimensions();
  auto d0 = boost::get<0>(dims);
  auto d1 = boost::get<1>(dims);
  Vector3f range(bbox.xmax() - bbox.xmin(),
                 bbox.ymax() - bbox.ymin(),
                 bbox.zmax() - bbox.zmin());
  size_t j(0);
  size_t k(0);
  switch (m_primitive_type) {
   case PT_TRIANGLES:
    for (Uint i = 0; i < m_num_primitives; ++i) {
      for (size_t l = 0; l < 3; ++l) {
        const Vector3f& v = get_coord_3d(m_flat_coord_indices[j++]);
        auto s = (v[d0] - bbox.min(d0)) / range[d0];
        auto t = (v[d1] - bbox.min(d1)) / range[d0];
        Vector2f tex_vec(s, t);
        (*tex_coord_array)[k++] = tex_vec;
      }
    }
    break;

   case PT_QUADS:
    for (Uint i = 0; i < m_num_primitives; ++i) {
      for (size_t l = 0; l < 4; ++l) {
        const Vector3f& v = get_coord_3d(m_flat_coord_indices[j++]);
        auto s = (v[d0] - bbox.min(d0)) / range[d0];
        auto t = (v[d1] - bbox.min(d1)) / range[d0];
        Vector2f tex_vec(s, t);
        (*tex_coord_array)[k++] = tex_vec;
      }
    }
    break;

   case PT_POLYGONS:
    for (Uint i = 0; i < m_num_primitives; ++i) {
      while (m_coord_indices[j] != (Uint) -1) {
        const Vector3f& v = get_coord_3d(m_coord_indices[j++]);
        auto s = (v[d0] - bbox.min(d0)) / range[d0];
        auto t = (v[d1] - bbox.min(d1)) / range[d0];
        Vector2f tex_vec(s, t);
        (*tex_coord_array)[k++] = tex_vec;
      }
      ++j;
    }
    break;

   case PT_TRIANGLE_STRIP:
   case PT_TRIANGLE_FAN:
   case PT_QUAD_STRIP:
   default: SGAL_assertion(0); break;
  }
}

//! \brief calculates the default 2D texture-mapping oordinates.
void Boundary_set::clean_tex_coords_3d()
{
  SGAL_assertion(m_coord_array);
  Uint num_coords = m_coord_array->size();
  if (m_tex_coord_array) m_tex_coord_array->resize(num_coords);
  else {
    m_tex_coord_array.reset(new Tex_coord_array_3d(num_coords));
    SGAL_assertion(m_tex_coord_array);
  }
  boost::shared_ptr<Tex_coord_array_3d> tex_coords =
    boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
  SGAL_assertion(tex_coords);

  for (Uint k = 0; k < tex_coords->size(); ++k) {
    (*tex_coords)[k].sub(get_coord_3d(k), m_center);
    (*tex_coords)[k].normalize();
  }
}

//! \brief draws the mesh conditionaly.
void Boundary_set::draw(Draw_action* action)
{
  if (is_dirty()) clean();
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_empty()) return;

  // Clean the normals or the colors.
  //! \todo clean the nromals only if lighting is enabled.
  if (resolve_fragment_source() == FS_NORMAL) {
    if (m_dirty_normals) clean_normals();
    if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  }
  else {
    if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  }

  // Clean the tex coordinates.
  Context* context = action->get_context();
  if (context->get_tex_enable() && !(context->get_tex_gen_enable())) {
    if (m_dirty_tex_coords) {
      const Context* context = action->get_context();
      boost::shared_ptr<Texture> texture = context->get_texture();
      Texture::Target target = Texture::TEXTURE_2D_ARRAY;
      if (texture) target = texture->get_target();
      // If texture-mapping is enabled and automatic (opengl) texture
      // coordinate generation is not enabled, generate the texture
      // coordinate as last resort.
      clean_tex_coords(target);
    }
    if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
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
    if (use_vertex_array() &&
        (Gfx_conf::get_instance()->is_vertex_buffer_object_supported()))
    {
      bool color_normal_condition = (resolve_fragment_source() == FS_COLOR) ?
        (m_flat_color_indices.empty() ||
         (std::equal(m_flat_coord_indices.begin(),
                     m_flat_coord_indices.end(),
                     m_flat_color_indices.begin()))) :
        (m_flat_normal_indices.empty() ||
         (std::equal(m_flat_coord_indices.begin(),
                     m_flat_coord_indices.end(),
                     m_flat_normal_indices.begin())));
      bool tex_coord_condition =
        (m_flat_tex_coord_indices.empty() ||
         (std::equal(m_flat_coord_indices.begin(),
                     m_flat_coord_indices.end(),
                     m_flat_tex_coord_indices.begin())));

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
              else {
                Uint num_tex_coords = m_tex_coord_array->num_coordinates();
                switch (num_tex_coords) {
                 case 2: clean_local_cnct2_vertex_buffers(); break;
                 case 3: clean_local_cnct3_vertex_buffers(); break;
                 case 4: clean_local_cnct4_vertex_buffers(); break;
                }
              }
            }
            else {
              if (!m_tex_coord_array) clean_local_cn_vertex_buffers();
              else {
                Uint num_tex_coords = m_tex_coord_array->num_coordinates();
                switch (num_tex_coords) {
                 case 2: clean_local_cnt2_vertex_buffers(); break;
                 case 3: clean_local_cnt3_vertex_buffers(); break;
                 case 4: clean_local_cnt4_vertex_buffers(); break;
                }
              }
            }
          }
          else {
            if (m_color_array) {
              if (!m_tex_coord_array) clean_local_cc_vertex_buffers();
              else {
                Uint num_tex_coords = m_tex_coord_array->num_coordinates();
                switch (num_tex_coords) {
                 case 2: clean_local_cct2_vertex_buffers(); break;
                 case 3: clean_local_cct3_vertex_buffers(); break;
                 case 4: clean_local_cct4_vertex_buffers(); break;
                }
              }
            }
            else {
              Uint num_tex_coords = m_tex_coord_array->num_coordinates();
              switch (num_tex_coords) {
               case 2: clean_local_ct2_vertex_buffers(); break;
               case 3: clean_local_ct3_vertex_buffers(); break;
               case 4: clean_local_ct4_vertex_buffers(); break;
              }
            }
          }
        }

        // Clean OpenGL vertex array buffers:
        if (m_dirty_coord_buffer && !m_local_coord_buffer.empty()) {
          Uint size = m_local_coord_buffer.size() * sizeof(Vector3f);
          clean_vertex_coord_buffer(size, local_coord_data());
        }
        if (m_dirty_normal_buffer && !m_local_normal_buffer.empty()) {
          Uint size = m_local_normal_buffer.size() * sizeof(Vector3f);
          clean_vertex_normal_buffer(size, local_normal_data());
        }
        if (m_dirty_color_buffer && !m_local_color_buffer.empty()) {
          Uint size = m_local_color_buffer.size() * sizeof(Vector3f);
          clean_vertex_color_buffer(size, local_color_data());
        }
        if (m_dirty_tex_coord_buffer && m_tex_coord_array) {
          Uint size = tex_coord_data_size();
          clean_vertex_tex_coord_buffer(size, tex_coord_data());
        }
      }
    }

   case Configuration::GDM_DIRECT:
    draw_dispatch(action);
    break;
  }
}

/*! \brief dispatches the appropriate drawing routine. */
void Boundary_set::draw_dispatch(Draw_action* /* action */)
{
   // When using vertex array, the index arrays must be flat:
  Boolean va = use_vertex_array();
  SGAL_assertion(!va || m_coord_indices_flat);

  Fragment_source fragment_source = resolve_fragment_source();
  Boolean fragment_indexed = va ? false :
    ((fragment_source == FS_NORMAL) ?
     (m_flat_normal_indices.size() ? true : false) :
     (m_flat_color_indices.size() ? true : false));
  Attachment fragment_attached = (fragment_source == FS_NORMAL) ?
    m_normal_attachment  : m_color_attachment;
  Boolean texture_enbaled = m_tex_coord_array ? true : false;
  Boolean texture_indexed = va ? false :
    (!m_flat_tex_coord_indices.empty() || !m_tex_coord_indices.empty());

  // std::cout << "fragment_source: " << fragment_source << std::endl;
  // std::cout << "fragment_indexed: " << fragment_indexed << std::endl;
  // std::cout << "fragment_attached: " << fragment_attached << std::endl;
  // std::cout << "texture_indexed: " << texture_indexed << std::endl;
  // std::cout << "m_primitive_type: " << m_primitive_type << std::endl;
  // std::cout << "va: " << va << std::endl;

  Uint mask =
    SGAL_SET(SGAL_BO_FRAG_SOURCE,SGAL_BO_FRAG_SOURCE_,fragment_source,
      SGAL_SET(SGAL_BO_FRAG_INDEXED,SGAL_BO_FRAG_INDEXED_,fragment_indexed,
        SGAL_SET(SGAL_BO_FRAG_ATTACHMENT,SGAL_BO_FRAG_ATTACHMENT_,fragment_attached,
          SGAL_SET(SGAL_BO_TEXTURE_ENABLED,SGAL_BO_TEXTURE_ENABLED_,texture_enbaled,
            SGAL_SET(SGAL_BO_TEXTURE_INDEXED,SGAL_BO_TEXTURE_INDEXED_,texture_indexed,
              SGAL_SET(SGAL_BO_PRIM_TYPE,SGAL_BO_PRIM_TYPE_,m_primitive_type,
                SGAL_SET(SGAL_BO_VERTEX_ARRAY,SGAL_BO_VERTEX_ARRAY_,va,0x0)))))));

  (this->*m_draws[mask])();
}

//! \brief isects direct drawing-mode.
void Boundary_set::isect_direct()
{
  Uint i, j;
  switch (m_primitive_type) {
   case PT_TRIANGLE_STRIP:
    if (m_coord_array->size()) {
      int num_tri_strips = m_tri_strip_lengths[0];
      int index = 0;
      for (int strip = 0; strip < num_tri_strips; ++strip) {
        int tmp = strip + 1;
        glBegin(GL_TRIANGLE_STRIP);
        for (Uint i = 0 ; i < m_tri_strip_lengths[tmp]; ++i)
          glVertex3fv(m_coord_array->datum(m_flat_coord_indices[index++]));
        glEnd();
      }
    }
    return;

   case PT_TRIANGLES:
    glBegin(GL_TRIANGLES);
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
    }
    glEnd();
    return;

   case PT_QUADS:
    glBegin(GL_QUADS);
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
      glVertex3fv(m_coord_array->datum(m_flat_coord_indices[j++]));
    }
    glEnd();
    return;

   case PT_POLYGONS:
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glBegin(GL_POLYGON);
      for (; m_coord_indices[j] != (Uint) -1; ++j)
        glVertex3fv(m_coord_array->datum(m_coord_indices[j]));
      glEnd();
      ++j;
    }
    return;

   case PT_TRIANGLE_FAN:
   case PT_QUAD_STRIP:
    SGAL_assertion_msg(0, "Not implemented yet!");
    return;

   default: ;
  }
}

//! \brief draws the mesh in selection mode.
void Boundary_set::isect(Isect_action* action)
{
  if (is_dirty()) clean();
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_empty()) return;

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

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "normalPerVertex") {
      set_normal_per_vertex(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "colorPerVertex") {
      set_color_per_vertex(compare_to_true(value));
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
  Configuration* config = sg->get_configuration();
  if (config) m_drawing_mode = config->get_geometry_drawing_mode();
}

#if 0
/*! Obtain a list of atytributes in this object. This method is called only
 * from the Builder side.
 * An Boundary_set is always converted to an ProgIndexedTriSet
 * and therefore any information regarding compressed data is not
 * written out (e.g., coordIndex).
 *
 * \return a list of attributes
 */
Attribute_list Boundary_set::get_attributes()
{
  Attribute_list attribs;
  Attribute attrib;
  char buf[32];
  Vector3f col;

  attribs = Geometry::get_attributes();

  if (m_normal_per_vertex != s_def_normal_per_vertex) {
    attrib.first = "normalPerVertex";
    attrib.second = get_normal_per_vertex() ? "TRUE" : "FALSE";
    attribs.push_back(attrib);
  }
  if (m_color_per_vertex != s_def_color_per_vertex) {
    attrib.first = "colorPerVertex";
    attrib.second = get_color_per_vertex() ? "TRUE" : "FALSE";
    attribs.push_back(attrib);
  }
  return attribs;
}

#endif

//! \brief initializes the container prototype.
void Boundary_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());

  // Add the field-info records to the prototype:
  // normalPerVertex
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function normal_per_vertex_func =
    static_cast<Boolean_handle_function>
    (&Boundary_set::normal_per_vertex_handle);
  s_prototype->add_field_info(new SF_bool(NORMAL_PER_VERTEX,
                                          "normalPerVertex",
                                          RULE_EXPOSED_FIELD,
                                          normal_per_vertex_func,
                                          s_def_normal_per_vertex, exec_func));

  // colorPerVertex
  Boolean_handle_function color_per_vertex_func =
    static_cast<Boolean_handle_function>
    (&Boundary_set::color_per_vertex_handle);
  s_prototype->add_field_info(new SF_bool(COLOR_PER_VERTEX,
                                          "colorPerVertex",
                                          RULE_EXPOSED_FIELD,
                                          color_per_vertex_func,
                                          s_def_color_per_vertex, exec_func));
}

//! \brief deletes the container prototype.
void Boundary_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Boundary_set::get_prototype()
{
  if (s_prototype == NULL) Boundary_set::init_prototype();
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
  m_local_color_buffer.clear();
  m_local_tex_coord_buffer_2d.clear();
  m_local_tex_coord_buffer_3d.clear();
  m_local_tex_coord_buffer_4d.clear();
}

//! \brief processes change of coordinate points.
void Boundary_set::coord_point_changed()
{
  destroy_display_list();
  m_dirty_coord_buffer = true;
  m_dirty_local_vertex_buffers = true;
  if (m_normals_cleaned || !m_normal_array) m_dirty_normals = true;
  if (m_tex_coords_cleaned || !m_tex_coord_array) m_dirty_tex_coords = true;
}

//! \brief Calculate a single normal per vertex for all vertices.
void Boundary_set::
calculate_single_normal_per_vertex(Shared_normal_array normals)
{
  SGAL_assertion(normals->size() == m_coord_array->size());
  // Calculate the normals of all facets.
  Normal_array per_polygon_normals(m_num_primitives);
  calculate_normal_per_polygon(per_polygon_normals);

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

  clear_flat_normal_indices();
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
void Boundary_set::clean_vertex_tex_coord_buffer(Uint size,
                                                     const GLfloat* data)
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

//! \brief sets the coordinate array.
void Boundary_set::set_coord_array(Shared_coord_array coord_array)
{
  Mesh_set::set_coord_array(coord_array);
  coord_point_changed();
}

//! \brief sets the normal array.
void Boundary_set::set_normal_array(Shared_normal_array normal_array)
{
  Mesh_set::set_normal_array(normal_array);
  destroy_display_list();
  m_dirty_normal_buffer = true;
  m_dirty_local_vertex_buffers = true;
  m_dirty_normals = false;
  m_normals_cleaned = false;
}

//! \brief sets the color field.
void Boundary_set::set_color_array(Shared_color_array color_array)
{
  Mesh_set::set_color_array(color_array);
  destroy_display_list();
  m_dirty_color_buffer = true;
}

//! \brief sets the texture-coordinate array.
void
Boundary_set::set_tex_coord_array(Shared_tex_coord_array tex_coord_array)
{
  Mesh_set::set_tex_coord_array(tex_coord_array);
  destroy_display_list();
  m_dirty_tex_coord_buffer = true;
  m_dirty_local_vertex_buffers = true;
  m_dirty_tex_coords = false;
  m_tex_coords_cleaned = false;
}

//! \brief Process change of field.
void Boundary_set::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case COORD_ARRAY: coord_point_changed(); break;

   case NORMAL_ARRAY:
    destroy_display_list();
    m_dirty_normal_buffer = true;
    m_dirty_local_vertex_buffers = true;
    m_dirty_normals = false;
    m_normals_cleaned = false;
    break;

   case COLOR_ARRAY:
    destroy_display_list();
    m_dirty_color_buffer = true;
    m_dirty_local_vertex_buffers = true;
    break;

   case TEX_COORD_ARRAY:
    destroy_display_list();
    m_dirty_tex_coord_buffer = true;
    m_dirty_local_vertex_buffers = true;
    m_dirty_tex_coords = false;
    m_tex_coords_cleaned = false;
    break;

   default: break;
  }
  Mesh_set::field_changed(field_info);
}

/*! \brief cleans the local coordinates, normals, color, and 2d texture
 * coordinates vertex buffers.
 */
void Boundary_set::clean_local_cnct2_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_color_array);
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion((normal_indices_begin != color_indices_begin) ||
                 (color_indices_begin != tex_coord_indices_begin));
  clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                m_color_array, m_local_color_buffer,
                                color_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_2d,
                                tex_coord_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

/*! \brief cleans the local coordinates, normals, color, and 3d texture
 * coordinates vertex buffers.
 */
void Boundary_set::clean_local_cnct3_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_color_array);
  boost::shared_ptr<Tex_coord_array_3d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion((normal_indices_begin != color_indices_begin) ||
                 (color_indices_begin != tex_coord_indices_begin));
  clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                m_color_array, m_local_color_buffer,
                                color_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_3d,
                                tex_coord_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

/*! \brief cleans the local coordinates, normals, color, and 4d texture
 * coordinates vertex buffers.
 */
void Boundary_set::clean_local_cnct4_vertex_buffers()
{
  SGAL_error_msg("clean_local_cnct4_vertex_buffers() not implemented yet!");
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_color_array);
  boost::shared_ptr<Tex_coord_array_4d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion((normal_indices_begin != color_indices_begin) ||
                 (color_indices_begin != tex_coord_indices_begin));
  clean_local_4d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                m_color_array, m_local_color_buffer,
                                color_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_4d,
                                tex_coord_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the local coordinates, normals, and color, vertex buffers.
void Boundary_set::clean_local_cnc_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  SGAL_assertion(m_color_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  SGAL_assertion(normal_indices_begin != color_indices_begin);
  clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                m_color_array, m_local_color_buffer,
                                color_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
}

/*! \brief cleans the local coordinates, normals, and 2d texture coordinates
 * vertex buffers.
 */
void Boundary_set::clean_local_cnt2_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion(normal_indices_begin != tex_coord_indices_begin);
  clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_2d,
                                tex_coord_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

/*! \brief cleans the local coordinates, normals, and 3d texture coordinates
 * vertex buffers.
 */
void Boundary_set::clean_local_cnt3_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  boost::shared_ptr<Tex_coord_array_3d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion(normal_indices_begin != tex_coord_indices_begin);
  clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_3d,
                                tex_coord_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

/*! \brief cleans the local coordinates, normals, and 4d texture coordinates
 * vertex buffers.
 */
void Boundary_set::clean_local_cnt4_vertex_buffers()
{
  SGAL_error_msg("clean_local_cnt4_vertex_buffers() not implemented yet!");
  SGAL_assertion(m_normal_array);
  boost::shared_ptr<Tex_coord_array_4d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion(normal_indices_begin != tex_coord_indices_begin);
  clean_local_3d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_4d,
                                tex_coord_indices_begin);
  m_dirty_normal_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the local coordinates and normals vertex buffers.
void Boundary_set::clean_local_cn_vertex_buffers()
{
  SGAL_assertion(m_normal_array);
  std::vector<Uint>::const_iterator normal_indices_begin =
    m_flat_normal_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_normal_indices.begin();
  clean_local_2d_vertex_buffers(m_normal_array, m_local_normal_buffer,
                                normal_indices_begin);
  m_dirty_normal_buffer = true;
}

/*! \brief cleans the local coordinates, colors, and texture coordinates vertex
 * buffers.
 */
void Boundary_set::clean_local_cct2_vertex_buffers()
{
  SGAL_assertion(m_color_array);
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion(color_indices_begin != tex_coord_indices_begin);
  clean_local_3d_vertex_buffers(m_color_array, m_local_color_buffer,
                                color_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_2d,
                                tex_coord_indices_begin);
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

void Boundary_set::clean_local_cct3_vertex_buffers()
{
  SGAL_assertion(m_color_array);
  boost::shared_ptr<Tex_coord_array_3d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion(color_indices_begin != tex_coord_indices_begin);
  clean_local_3d_vertex_buffers(m_color_array, m_local_color_buffer,
                                color_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_3d,
                                tex_coord_indices_begin);
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

void Boundary_set::clean_local_cct4_vertex_buffers()
{
  SGAL_error_msg("clean_local_cct4_vertex_buffers() not implemented yet!");
  SGAL_assertion(m_color_array);
  boost::shared_ptr<Tex_coord_array_4d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  SGAL_assertion(color_indices_begin != tex_coord_indices_begin);
  clean_local_3d_vertex_buffers(m_color_array, m_local_color_buffer,
                                color_indices_begin,
                                tex_coord_array, m_local_tex_coord_buffer_4d,
                                tex_coord_indices_begin);
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the local coordinates and colors vertex buffers.
void Boundary_set::clean_local_cc_vertex_buffers()
{
  SGAL_assertion(m_color_array);
  std::vector<Uint>::const_iterator color_indices_begin =
    m_flat_color_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_color_indices.begin();
  clean_local_2d_vertex_buffers(m_color_array, m_local_color_buffer,
                                color_indices_begin);
  m_dirty_color_buffer = true;
}

//! \brief cleans the local coordinates and texture coordinates vertex buffers.
void Boundary_set::clean_local_ct2_vertex_buffers()
{
  boost::shared_ptr<Tex_coord_array_2d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  clean_local_2d_vertex_buffers(tex_coord_array, m_local_tex_coord_buffer_2d,
                                tex_coord_indices_begin);
  m_dirty_tex_coord_buffer = true;
}

void Boundary_set::clean_local_ct3_vertex_buffers()
{
  boost::shared_ptr<Tex_coord_array_3d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_3d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  clean_local_2d_vertex_buffers(tex_coord_array, m_local_tex_coord_buffer_3d,
                                m_flat_tex_coord_indices.begin());
  m_dirty_tex_coord_buffer = true;
}

void Boundary_set::clean_local_ct4_vertex_buffers()
{
  SGAL_error_msg("clean_local_ct4_vertex_buffers() not implemented yet!");
  boost::shared_ptr<Tex_coord_array_4d> tex_coord_array =
    boost::dynamic_pointer_cast<Tex_coord_array_4d>(m_tex_coord_array);
  SGAL_assertion(tex_coord_array);
  std::vector<Uint>::const_iterator tex_coord_indices_begin =
    m_flat_tex_coord_indices.empty() ?
    m_flat_coord_indices.begin() : m_flat_tex_coord_indices.begin();
  clean_local_2d_vertex_buffers(tex_coord_array, m_local_tex_coord_buffer_4d,
                                tex_coord_indices_begin);
  m_dirty_tex_coord_buffer = true;
}

//! \brief cleans the center of the geometric object.
void Boundary_set::clean_center()
{
  m_center.set(0, 0, 0);
  m_dirty_center = false;
}

//! \brief sets the center of the geometric object.
void Boundary_set::set_center(Vector3f& center)
{
  m_center = center;
  m_dirty_center = false;
}

//! \brief obtains the center of the geometric object.
Vector3f& Boundary_set::get_center()
{
  if (is_dirty()) clean();
  return m_center;
}

SGAL_END_NAMESPACE
