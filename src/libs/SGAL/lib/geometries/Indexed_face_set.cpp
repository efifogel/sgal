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
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Tex_coord_array_3d.hpp"
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

SGAL_BEGIN_NAMESPACE

//! \todo #include "Model_stats.h"

const std::string Indexed_face_set::s_tag = "IndexedFaceSet";
Container_proto* Indexed_face_set::s_prototype(NULL);

// Default values:
const Boolean Indexed_face_set::s_def_normal_per_vertex(true);
const Boolean Indexed_face_set::s_def_color_per_vertex(true);

void (Indexed_face_set::*Indexed_face_set::draws[NUM_DRAWS])();
Boolean Indexed_face_set::m_draws_initialized(false);

REGISTER_TO_FACTORY(Indexed_face_set, "Indexed_face_set");

//! \brief constructor.
Indexed_face_set::Indexed_face_set(Boolean proto) :
  Mesh_set(proto),
  m_max_rank(0),
  m_curr_num_polygons(0),
  m_normal_per_vertex(s_def_normal_per_vertex),
  m_color_per_vertex(s_def_color_per_vertex),
  m_tri_strip_lengths(0),
  m_is_multiple_uv(false),
  m_display_list_id(0),
  m_coord_buffer_id(0),
  m_color_buffer_id(0),
  m_normal_buffer_id(0),
  m_tex_coord_buffer_id(0),
  m_drawing_mode(Configuration::s_def_geometry_drawing_mode),
  m_bb_is_pre_set(false),
  m_is_progressive(false),
  m_dirty_polyhedron(true),
  m_dirty_facets(true),
  m_dirty_normals(true),
  m_normals_cleaned(false),
  m_dirty_tex_coords(true),
  m_tex_coords_cleaned(false),
  m_dirty_coord_buffer(true),
  m_dirty_normal_buffer(true),
  m_dirty_color_buffer(true),
  m_dirty_tex_coord_buffer(true),
  m_dirty_local_vertex_buffers(true)
{
  m_primitive_type = PT_POLYGONS;

  if (m_draws_initialized) return;
  m_draws_initialized = true;

  // Initialize static draws[] array:
  for (int i = 0; i < NUM_DRAWS; ++i)
    draws[i] = &Indexed_face_set::draw_invalid;

  // Standard (no vertex array):
  // Polygons:
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOPO_VANO;
  draws[FSCO_FINO_FAPV_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOPO_VANO;
  draws[FSNO_FIYE_FAPV_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOPO_VANO;
  draws[FSCO_FIYE_FAPV_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOPO_VANO;

  draws[FSNO_FINO_FAPT_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOPO_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOPO_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOPO_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOPO_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOPO_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOPO_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO;

  // Triangles
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOTR_VANO;
  draws[FSCO_FINO_FAPV_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOTR_VANO;
  draws[FSNO_FIYE_FAPV_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOTR_VANO;
  draws[FSCO_FIYE_FAPV_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOTR_VANO;

  draws[FSNO_FINO_FAPT_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOTR_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOTR_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOTR_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOTR_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOTR_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOTR_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO;

  // Quads
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOQU_VANO;
  draws[FSCO_FINO_FAPV_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOQU_VANO;
  draws[FSNO_FIYE_FAPV_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOQU_VANO;
  draws[FSCO_FIYE_FAPV_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOQU_VANO;

  draws[FSNO_FINO_FAPT_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOQU_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOQU_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOQU_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOQU_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOQU_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOQU_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO;

  // Triangle Strips:
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOTS_VANO;
  draws[FSCO_FINO_FAPV_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOTS_VANO;
  draws[FSNO_FIYE_FAPV_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOTS_VANO;
  draws[FSCO_FIYE_FAPV_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOTS_VANO;

  draws[FSNO_FINO_FAPT_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOTS_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOTS_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOTS_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOTS_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOTS_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOTS_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO;

  // Vertex array:
  // Polygons:
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOPO_VANO;
  draws[FSCO_FINO_FAPV_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOPO_VANO;
  draws[FSNO_FIYE_FAPV_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOPO_VANO;
  draws[FSCO_FIYE_FAPV_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOPO_VANO;

  draws[FSNO_FINO_FAPT_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOPO_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOPO_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOPO_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOPO_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOPO_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOPO_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO;

  // Triangles:
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOTR_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FINO_FAPV_TENO_MOTR_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSNO_FIYE_FAPV_TENO_MOTR_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FIYE_FAPV_TENO_MOTR_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;

  draws[FSNO_FINO_FAPT_TENO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOTR_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOTR_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOTR_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOTR_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOTR_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOTR_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO;

  // Quads:
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOQU_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FINO_FAPV_TENO_MOQU_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSNO_FIYE_FAPV_TENO_MOQU_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FIYE_FAPV_TENO_MOQU_VAYE] = &Indexed_face_set::draw_FAPV_VAYE;

  draws[FSNO_FINO_FAPT_TENO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOQU_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOQU_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOQU_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOQU_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOQU_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOQU_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FAPV_VAYE;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO;

  // Triangle Strips:
  // Texture disabled:
  draws[FSNO_FINO_FAPV_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TENO_MOTS_VANO;
  draws[FSCO_FINO_FAPV_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TENO_MOTS_VANO;
  draws[FSNO_FIYE_FAPV_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TENO_MOTS_VANO;
  draws[FSCO_FIYE_FAPV_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TENO_MOTS_VANO;

  draws[FSNO_FINO_FAPT_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TENO_MOTS_VANO;
  draws[FSCO_FINO_FAPT_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TENO_MOTS_VANO;
  draws[FSNO_FIYE_FAPT_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TENO_MOTS_VANO;
  draws[FSCO_FIYE_FAPT_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TENO_MOTS_VANO;

  draws[FSNO_FINO_FAPM_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TENO_MOTS_VANO;
  draws[FSCO_FINO_FAPM_TENO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TENO_MOTS_VANO;

  // Texture enabled:
  draws[FSNO_FINO_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TINO_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO;

  draws[FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO;
  draws[FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO;

  draws[FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO;
  draws[FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO;

  draws[FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO;
  draws[FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE] =
    &Indexed_face_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO;
}

//! \brief destructor.
Indexed_face_set::~Indexed_face_set()
{
  clear_local_vertex_buffers();
  clear_vertex_arrays();
  destroy_display_list();
  destroy_vertex_buffers();
  m_dirty_local_vertex_buffers = true;
}

/* \brief sets the flag that indicates whether normals are bound per vertex or
 * per face.
 */
void Indexed_face_set::set_normal_per_vertex(Boolean normal_per_vertex)
{
  m_normal_per_vertex = normal_per_vertex;
  m_normal_attachment = (normal_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/* \brief sets the flag that indicates whether colors are bound per vertex or
 * per face.
 */
void Indexed_face_set::set_color_per_vertex(Boolean color_per_vertex)
{
  m_color_per_vertex = color_per_vertex;
  m_color_attachment = (color_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

//! \brief claculates the normals in case they are invalidated.
void Indexed_face_set::clean_normals()
{
  if (m_crease_angle >= SGAL_PI) calculate_single_normal_per_vertex();
  else if (m_crease_angle > 0) calculate_multiple_normals_per_vertex();
  else calculate_normal_per_polygon();
  m_dirty_normals = false;
  m_normals_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief computes the normalized normal to a triangle.
void Indexed_face_set::compute_triangle_normal(Uint j, Vector3f& n) const
{
  Vector3f& v0 = (*m_coord_array)[m_flat_coord_indices[j]];
  Vector3f& v1 = (*m_coord_array)[m_flat_coord_indices[j+1]];
  Vector3f& v2 = (*m_coord_array)[m_flat_coord_indices[j+2]];
  SGAL_assertion(!v0.collinear(v0, v1, v2));
  if (is_ccw()) n.normal(v0, v1, v2);
  else n.normal(v2, v1, v0);
}

//! \brief computes the center point of a triangle.
void Indexed_face_set::compute_triangle_center(Uint j, Vector3f& center) const
{
  center.add((*m_coord_array)[m_flat_coord_indices[j+0]]);
  center.add((*m_coord_array)[m_flat_coord_indices[j+1]]);
  center.add((*m_coord_array)[m_flat_coord_indices[j+2]]);
  center.scale(1.0f / 3);
}

/*! \brief computes the vertex information for the three vertices of a
 * triangule.
 */
void Indexed_face_set::
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
void Indexed_face_set::compute_quad_normal(Uint j, Vector3f& n) const
{
  Vector3f& v0 = (*m_coord_array)[m_flat_coord_indices[j]];
  Vector3f& v1 = (*m_coord_array)[m_flat_coord_indices[j+1]];
  Vector3f& v2 = (*m_coord_array)[m_flat_coord_indices[j+2]];
  if (!v0.collinear(v0, v1, v2)) {
    if (is_ccw()) n.normal(v0, v1, v2);
    else n.normal(v2, v1, v0);
    return;
  }
  Vector3f& v3 = (*m_coord_array)[m_flat_coord_indices[j+3]];
  SGAL_assertion(!v0.collinear(v0, v1, v3));
  if (is_ccw()) n.normal(v0, v1, v3);
  else n.normal(v3, v1, v0);
}

//! \brief computes the center point of a quadrilateral.
void Indexed_face_set::compute_quad_center(Uint j, Vector3f& center) const
{
  center.add((*m_coord_array)[m_flat_coord_indices[j+0]]);
  center.add((*m_coord_array)[m_flat_coord_indices[j+1]]);
  center.add((*m_coord_array)[m_flat_coord_indices[j+2]]);
  center.add((*m_coord_array)[m_flat_coord_indices[j+3]]);
  center.scale(1.0f / 4);
}

/*! \brief computes the vertex information for the four vertices of a
 * quadrilateral.
 */
void
Indexed_face_set::compute_quad_vertex_info(Uint j, Uint facet_index,
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
void Indexed_face_set::compute_polygon_normal(Uint j, Vector3f& n) const
{
  Vector3f& v0 = (*m_coord_array)[m_coord_indices[j]];
  Vector3f& v1 = (*m_coord_array)[m_coord_indices[j+1]];
  for (Uint k = 2; m_coord_indices[j+k] != (Uint) -1; ++k) {
    Vector3f& v2 = (*m_coord_array)[m_coord_indices[j+k]];
    if (v0.collinear(v0, v1, v2)) continue;
    if (is_ccw()) n.normal(v0, v1, v2);
    else n.normal(v2, v1, v0);
    return;
  }
  SGAL_assertion_msg(0, "All vertices are collinear!");
}

//! \brief computes the center point of a polygon.
Uint Indexed_face_set::compute_polygon_center(Uint j, Vector3f& center) const
{
  Uint k;
  for (k = 0; m_coord_indices[j+k] != (Uint) -1; ++k)
    center.add((*m_coord_array)[m_coord_indices[j+k]]);
  center.scale(1.0f / k);
  return k;
}

//! \brief computes the vertex information for the all vertices of a polygon.
void Indexed_face_set::
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
void Indexed_face_set::calculate_vertices_info(Vertices_info& vertices_info)
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
void Indexed_face_set::calculate_single_normal_per_vertex()
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
Boolean Indexed_face_set::is_smooth(const Vector3f& normal1,
                                    const Vector3f& normal2) const
{
  float angle = acosf(normal1.dot(normal2));
  return (angle > m_crease_angle);
}

//! \brief calculates multiple normals per vertex for all vertices.
void Indexed_face_set::calculate_multiple_normals_per_vertex()
{ calculate_single_normal_per_vertex(); }

//! \brief calculates a single normal per polygon for all polygons.
void Indexed_face_set::
calculate_normal_per_polygon(Normal_array& normals)
{
  SGAL_assertion(normals.size() == m_num_primitives);
  Uint j = 0;
  for (Uint i = 0 ; i < m_num_primitives; ++i) {
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
}

//! \brief calculates a single normal per polygon for all polygons.
void Indexed_face_set::calculate_normal_per_polygon()
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

/*! \brief calculates the default texture coordinate mapping in case it is
 * not present, using the shape bounding-box.
 */
void Indexed_face_set::clean_tex_coords()
{
  SGAL_assertion(m_coord_array);
  Uint num_coords = m_coord_array->size();
  if (m_tex_coord_array) {
    m_tex_coord_array->resize(num_coords);
  }
  else {
    m_tex_coord_array.reset(new Tex_coord_array_2d(num_coords));
    SGAL_assertion(m_tex_coord_array);
  }
  boost::shared_ptr<Tex_coord_array_2d> shared_tex_coord_array =
    boost::static_pointer_cast<Tex_coord_array_2d>(m_tex_coord_array);
  SGAL_assertion(shared_tex_coord_array);
  Tex_coord_array_2d* tex_coord_array = &*shared_tex_coord_array;

  //! \todo do the right thing!
  const Vector2f t0(0,0);
  const Vector2f t1(1,0);
  const Vector2f t2(1,1);
  const Vector2f t3(0,1);
  for (Uint i = 0 ; i < num_coords; ++i) {
    Uint mask = i & 0x3;
    const Vector2f& t =
      (mask == 0x0) ? t0 : (mask == 0x1) ? t1 : (mask == 0x2) ? t2 : t3;
    (*tex_coord_array)[i] = t;
  }

  m_dirty_tex_coords = false;
  m_tex_coords_cleaned = true;
  m_dirty_tex_coord_buffer = true;
}

//! \brief draws the mesh conditionaly.
void Indexed_face_set::draw(Draw_action* action)
{
  if (is_dirty()) clean();
//   if (m_crease_angle > 0) {
//     if (m_dirty_polyhedron) clean_polyhedron();
//     if (m_dirty_facets) clean_facets();
//   }
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  if (is_empty()) return;

  // Clean the normals:
  if ((resolve_fragment_source() == FS_NORMAL) && m_dirty_normals)
    clean_normals();

  // Clean the tex coordinates:
  Context* context = action->get_context();
  if (context->get_tex_enable() && !(context->get_tex_gen_enable()) &&
      m_dirty_tex_coords)
    clean_tex_coords();

  draw_mesh(action);
}

/*! \brief draws the geometry.
 * For efficiency reasons, differenrt methods were written to
 * draw geometries with different kinds of data (texture/normal/color).
 */
void Indexed_face_set::draw_geometry(Draw_action* action)
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
        if (m_dirty_local_vertex_buffers) clean_local_vertex_buffers();

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
//         if (m_dirty_tex_coord_buffer && m_tex_coord_array) {
//           Uint size =
//           clean_vertex_tex_coord_buffer(size, local_tex_coord_data());
//         }
      }
    }

   case Configuration::GDM_DIRECT:
    draw_dispatch(action);
    break;
  }
}

/*! \brief dispatches the appropriate drawing routine. */
void Indexed_face_set::draw_dispatch(Draw_action* /* action */)
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
    ((m_normal_per_vertex) ? PER_VERTEX : PER_PRIMITIVE) :
    ((m_color_per_vertex) ? PER_VERTEX : PER_PRIMITIVE);
  Boolean texture_enbaled = m_tex_coord_array ? true : false;
  Boolean texture_indexed = va ? false :
    (m_flat_tex_coord_indices.size() ? true : false);

//   std::cout << "fragment_source: " << fragment_source << std::endl;
//   std::cout << "fragment_indexed: " << fragment_indexed << std::endl;
//   std::cout << "fragment_attached: " << fragment_attached << std::endl;
//   std::cout << "texture_indexed: " << texture_indexed << std::endl;
//   std::cout << "m_primitive_type: " << m_primitive_type << std::endl;
//   std::cout << "va: " << va << std::endl;

  Uint mask =
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,fragment_source,
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,fragment_indexed,
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,fragment_attached,
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,texture_enbaled,
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,texture_indexed,
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,m_primitive_type,
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,va,0x0)))))));

  (this->*draws[mask])();
}

//! \brief isects direct drawing-mode.
void Indexed_face_set::isect_direct()
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
        for (Uint i = 0 ; i < m_tri_strip_lengths[tmp]; ++i) {
          Vector3f& v = (*m_coord_array)[m_flat_coord_indices[index++]];
          glVertex3fv((float*)&v);
        }
        glEnd();
      }
    }
    return;

   case PT_TRIANGLES:
    glBegin(GL_TRIANGLES);
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    }
    glEnd();
    return;

   case PT_QUADS:
    glBegin(GL_QUADS);
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
      glVertex3fv(get_by_flat_coord_index(*m_coord_array, j++));
    }
    glEnd();
    return;

   case PT_POLYGONS:
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glBegin(GL_POLYGON);
      for (; m_coord_indices[j] != (Uint) -1; ++j) {
        glVertex3fv(get_by_coord_index(*m_coord_array, j));
      }
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
void Indexed_face_set::isect(Isect_action* action)
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
int Indexed_face_set::create_display_list(Draw_action* action)
{
  int id = glGenLists(1);
  glNewList(id, GL_COMPILE);
  draw_dispatch(action);
  glEndList();
  return id;
}

//! \brief sets the attributes of the object.
void Indexed_face_set::set_attributes(Element* elem)
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
void Indexed_face_set::add_to_scene(Scene_graph* sg)
{
  Configuration* config = sg->get_configuration();
  if (config) m_drawing_mode = config->get_geometry_drawing_mode();
}

#if 0
/*! Obtain a list of atytributes in this object. This method is called only
 * from the Builder side.
 * An Indexed_face_set is always converted to an ProgIndexedTriSet
 * and therefore any information regarding compressed data is not
 * written out (e.g., coordIndex).
 *
 * \return a list of attributes
 */
Attribute_list Indexed_face_set::get_attributes()
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
  if (m_normal_per_vertex != s_def_color_per_vertex) {
    attrib.first = "colorPerVertex";
    attrib.second = get_color_per_vertex() ? "TRUE" : "FALSE";
    attribs.push_back(attrib);
  }
  return attribs;
}

#endif

//! \brief initializes the container prototype.
void Indexed_face_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());

  // Add the field-info records to the prototype:
  // normalPerVertex
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function normal_per_vertex_func =
    static_cast<Boolean_handle_function>
    (&Indexed_face_set::normal_per_vertex_handle);
  s_prototype->add_field_info(new SF_bool(NORMAL_PER_VERTEX,
                                          "normalPerVertex",
                                          RULE_EXPOSED_FIELD,
                                          normal_per_vertex_func,
                                          s_def_normal_per_vertex, exec_func));

  // colorPerVertex
  Boolean_handle_function color_per_vertex_func =
    static_cast<Boolean_handle_function>
    (&Indexed_face_set::color_per_vertex_handle);
  s_prototype->add_field_info(new SF_bool(COLOR_PER_VERTEX,
                                          "colorPerVertex",
                                          RULE_EXPOSED_FIELD,
                                          color_per_vertex_func,
                                          s_def_color_per_vertex, exec_func));
}

//! \brief deletes the container prototype.
void Indexed_face_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Indexed_face_set::get_prototype()
{
  if (s_prototype == NULL) Indexed_face_set::init_prototype();
  return s_prototype;
}

//! \brief destroys the data structure of the vertex buffer object.
void Indexed_face_set::destroy_vertex_buffers()
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
void Indexed_face_set::destroy_display_list()
{
  if (m_display_list_id != 0) {
    glDeleteLists(m_display_list_id, 1);
    m_display_list_id = 0;
  }
}

//! \brief clear the vertex buffers.
void Indexed_face_set::clear_local_vertex_buffers()
{
  m_local_coord_buffer.clear();
  m_local_normal_buffer.clear();
  m_local_color_buffer.clear();
  m_local_tex_coord_buffer_2d.clear();
  m_local_tex_coord_buffer_3d.clear();
  m_local_tex_coord_buffer_4d.clear();
}

//! \brief destroys the vertex arrays.
void Indexed_face_set::clear_vertex_arrays()
{
  if (m_coord_array) m_coord_array->clear();
  if (m_tex_coord_array) m_tex_coord_array->clear();
  if (m_normal_array) m_normal_array->clear();
  if (m_color_array) m_color_array->clear();
}

//! \brief processes change of coordinate points.
void Indexed_face_set::coord_point_changed()
{
  destroy_display_list();
  m_dirty_coord_buffer = true;
  m_dirty_local_vertex_buffers = true;
  if (m_normals_cleaned || !m_normal_array) m_dirty_normals = true;
  if (m_tex_coords_cleaned || !m_tex_coord_array) m_dirty_tex_coords = true;
}

//! \brief returns true if the representation is empty.
Boolean Indexed_face_set::is_empty() const
{
  if (m_drawing_mode == Configuration::GDM_DISPLAY_LIST)
    return (m_display_list_id == 0);

  if (use_vertex_array()) {
    if (Gfx_conf::get_instance()->is_vertex_buffer_object_supported())
      return ((m_coord_buffer_id == 0) && Geo_set::is_empty());
  }

  return Geo_set::is_empty();
}

//! \brief Calculate a single normal per vertex for all vertices.
void Indexed_face_set::
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
}

//! \brief cleans the data structure of the vertex coordinate buffer object.
void Indexed_face_set::clean_vertex_coord_buffer(Uint size, const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_coord_buffer_id == 0) glGenBuffersARB(1, &m_coord_buffer_id);
  SGAL_assertion(m_coord_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_coord_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);

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
void Indexed_face_set::clean_vertex_normal_buffer(Uint size,
                                                  const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_normal_buffer_id == 0) glGenBuffersARB(1, &m_normal_buffer_id);
  SGAL_assertion(m_normal_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_normal_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);

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
void Indexed_face_set::clean_vertex_color_buffer(Uint size, const GLfloat* data)
{
  if (!m_color_array) return;
#if defined(GL_ARB_vertex_buffer_object)
  if (m_color_buffer_id == 0) glGenBuffersARB(1, &m_color_buffer_id);
  SGAL_assertion(m_color_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_color_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);

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
void Indexed_face_set::clean_vertex_tex_coord_buffer(Uint size,
                                                     const GLfloat* data)
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_tex_coord_buffer_id == 0) glGenBuffersARB(1, &m_tex_coord_buffer_id);
  SGAL_assertion(m_tex_coord_buffer_id != 0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_tex_coord_buffer_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);

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
void Indexed_face_set::set_coord_array(Shared_coord_array coord_array)
{
  Mesh_set::set_coord_array(coord_array);
  coord_point_changed();
}

//! \brief sets the normal array.
void Indexed_face_set::set_normal_array(Shared_normal_array normal_array)
{
  Mesh_set::set_normal_array(normal_array);
  destroy_display_list();
  m_dirty_normal_buffer = true;
  m_dirty_local_vertex_buffers = true;
  m_dirty_normals = false;
  m_normals_cleaned = false;
}

//! \brief sets the color field.
void Indexed_face_set::set_color_array(Shared_color_array color_array)
{
  Mesh_set::set_color_array(color_array);
  destroy_display_list();
  m_dirty_color_buffer = true;
}

//! \brief sets the texture-coordinate array.
void
Indexed_face_set::set_tex_coord_array(Shared_tex_coord_array tex_coord_array)
{
  Mesh_set::set_tex_coord_array(tex_coord_array);
  destroy_display_list();
  m_dirty_tex_coord_buffer = true;
  m_dirty_local_vertex_buffers = true;
  m_dirty_tex_coords = false;
  m_tex_coords_cleaned = false;
}

//! \brief Process change of field.
void Indexed_face_set::field_changed(const Field_info* field_info)
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

//! \brief determines whether colors are generated by the geometry.
inline Boolean Indexed_face_set::are_generated_color()
{ return (m_generated_color); }

//! \brief determines whether texture coordinates are generated by the geometry.
inline Boolean Indexed_face_set::are_generated_tex_coord()
{ return (m_generated_tex_coord); }

//! \brief cleans the data structure.
void Indexed_face_set::clean_polyhedron()
{
  m_polyhedron.delegate(m_surface);
#if 0
  if (!m_polyhedron.normalized_border_is_valid()) {
    m_polyhedron.normalize_border();
  }
#else
  m_polyhedron.normalize_border();
#endif

  m_dirty_polyhedron = false;
  m_dirty_facets = true;
}

//! \brief cleans the facets.
void Indexed_face_set::clean_facets()
{
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Normal_vector());
  m_dirty_facets = false;
}

//! \brief cleans the local vertex buffers.
void Indexed_face_set::clean_local_vertex_buffers()
{
  //! \start with coordinates and normals
  m_local_indices.resize(m_flat_coord_indices.size());
  Uint tex_coord_id = 0;
  Uint coord_id = 0;
  Uint normal_id = 0;

  auto cit = m_flat_coord_indices.begin();
  auto nit = m_flat_normal_indices.begin();
  auto tit = m_flat_tex_coord_indices.begin();
  size_t index = 0;
  for (; cit != m_flat_coord_indices.end(); ++cit, ++nit) {
    coord_id = *cit;
    normal_id = *nit;
    Id_map::const_iterator got =
      m_id_map.find(std::make_tuple(coord_id, normal_id, tex_coord_id));
    if (got != m_id_map.end()) {
      m_local_indices[++index] = got->second;
      continue;
    }
    /* The combination of coord_id, normal/color_id, and tex_coord_id
     * hasn't been used yet, introduce it.
     */
    size_t new_id = m_local_coord_buffer.size();
    m_local_coord_buffer.push_back((*m_coord_array)[coord_id]);
    m_local_normal_buffer.push_back((*m_normal_array)[normal_id]);
    m_id_map[std::make_tuple(coord_id, normal_id, tex_coord_id)] = index;
    m_local_indices[++index] = new_id;
  }
  m_dirty_coord_buffer = true;
  m_dirty_normal_buffer = true;
  m_dirty_color_buffer = true;
  m_dirty_tex_coord_buffer = true;

  m_dirty_local_vertex_buffers = false;
}

SGAL_END_NAMESPACE
