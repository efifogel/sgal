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
// $Revision: 14223 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <sstream>

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
#include "SGAL/Tex_coord_array.hpp"
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

const std::string Indexed_face_set::s_tag = "sgalIndexedFaceSet";
Container_proto* Indexed_face_set::s_prototype = 0;

// Default values:
const Boolean Indexed_face_set::s_def_normal_per_vertex(true);
const Boolean Indexed_face_set::s_def_color_per_vertex(true);

void (Indexed_face_set::*Indexed_face_set::draws[NUM_DRAWS])();
Boolean Indexed_face_set::m_draws_initialized = false;

REGISTER_TO_FACTORY(Indexed_face_set, "Indexed_face_set");

/*! A parameter-less constructor */
Indexed_face_set::Indexed_face_set(Boolean proto) :
  Mesh_set(proto),
  m_max_rank(0),
  m_curr_num_polygons(0),
  m_normal_per_vertex(s_def_normal_per_vertex),
  m_color_per_vertex(s_def_color_per_vertex),
  m_tri_strip_lengths(0),
  m_is_multiple_uv(false),
  m_display_list_id(0),
  m_vertex_coord_id(0),
  m_vertex_color_id(0),
  m_vertex_normal_id(0),
  m_vertex_tex_coord_id(0),
  m_drawing_mode(Configuration::s_def_geometry_drawing_mode),
  m_bb_is_pre_set(false),
  m_is_progressive(false),
  m_own_normal_array(false),
  m_own_tex_coord_array(false),
  m_dirty_normals(true),
  m_dirty_tex_coords(true),
  m_vertex_buffer_object_created(false)
{
  m_primitive_type = PT_POLYGONS;
  m_flatten_indices = true;
  
  if (m_draws_initialized) return;
  m_draws_initialized = true;
    
  // Initialize static draws[] array:
  for (int i = 0; i < NUM_DRAWS; i++)
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

/*! Destructor */
Indexed_face_set::~Indexed_face_set()
{
  clear();
  if (m_own_normal_array && m_normal_array) delete m_normal_array;
  if (m_own_tex_coord_array && m_tex_coord_array) delete m_tex_coord_array;
}

/* Set the flag that indicates whether normals are bound per vertex or per face.
 * @param normal_per_vertex true if normals are bound per vertex
 */
void Indexed_face_set::set_normal_per_vertex(Boolean normal_per_vertex)
{
  m_normal_per_vertex = normal_per_vertex;
  m_normal_attachment = (normal_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/* Set the flag that indicates whether colors are bound per vertex or per face.
 * @param color_per_vertex true if normals are bound per vertex
 */
void Indexed_face_set::set_color_per_vertex(Boolean color_per_vertex)
{
  m_color_per_vertex = color_per_vertex;
  m_color_attachment = (color_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/*! \brief allocates the normal array in case it is not present */
void Indexed_face_set::allocate_normals()
{
  SGAL_assertion(!m_normal_array);
  SGAL_assertion(m_coord_array);
  Uint size = (m_crease_angle > 0) ? m_coord_array->size() : m_num_primitives;
  m_normal_array = new Normal_array(size);
  m_own_normal_array = true;
}

/*! \brief claculates the normals in case they are invalidated */
void Indexed_face_set::clean_normals()
{
  SGAL_assertion(m_normal_array);
  if (m_crease_angle > 0) calculate_normals_per_vertex();
  else calculate_normals_per_polygon();
  m_dirty_normals = false;
}

/*! \brief calculates a single normal per vertex for all vertices */
void Indexed_face_set::calculate_normals_per_vertex()
{
  m_normal_array->resize(m_coord_array->size());

  // Initialize the weights:

  // For each vertex we compute the weighted normal based on the normals of
  // the vertex incident facets and the receiprocal of the square distance
  // from the facet center to the vertex (an alternative could be the facet
  // area).
  typedef std::pair<Float, Vector3f>    Weight_normal_pair;
  typedef std::list<Weight_normal_pair> Vertex_info;
  typedef Vertex_info::const_iterator   Vertex_const_iter;
  typedef std::vector<Vertex_info>      Vertices_info;
    
  Vertices_info vertices_info;
  vertices_info.resize(m_coord_array->size());
  
  Uint k, i, j = 0;
  for (i = 0; i < m_num_primitives; ++i) {
    // Assume that the facet is planar, and compute its normal:
    Vector3f& v0 = (*m_coord_array)[m_coord_indices[j]];
    Vector3f& v1 = (*m_coord_array)[m_coord_indices[j+1]];
    Vector3f& v2 = (*m_coord_array)[m_coord_indices[j+2]];

    Vector3f l1, l2;
    l1.sub(v1, v0);
    l2.sub(v2, v1);
    Vector3f n;
    if (m_is_ccw) {
      n.cross(l1, l2); 
    } else {
      n.cross(l2, l1);
    }
    n.normalize();

    Vector3f center;

    switch (m_primitive_type) {
     case PT_TRIANGLES:
     case PT_QUADS:
     case PT_POLYGONS:
      // Compute the receiprocal of the square distance from the facet center
      for (k = 0; m_coord_indices[j+k] != (Uint) -1; ++k) {
        if ((m_primitive_type == PT_TRIANGLES) && (k == 3)) break;
        if ((m_primitive_type == PT_QUADS) && (k == 4)) break;
        center.add((*m_coord_array)[m_coord_indices[j+k]]);
      }
      center.scale(1.0f / k);

      // Assign the value:
      for (; k > 0; --k, ++j) {
        Uint vertex_index = m_coord_indices[j];
        Vector3f& v = (*m_coord_array)[vertex_index];
        Float weight = 1.0f / v.sqr_distance(center);
        vertices_info[vertex_index].push_back(Weight_normal_pair(weight, n));
      }
      if (m_primitive_type == PT_POLYGONS) ++j; // skip the end-of-face marker
      break;
      
     case PT_TRIANGLE_STRIP: break;
     case PT_TRIANGLE_FAN: break;
     case PT_QUAD_STRIP:
     default: SGAL_assertion(0); break;
    }
  }

  // Calculate the weighted normals:
  for (j = 0; j < vertices_info.size(); ++j) {
    Float weight_sum = 0;
    Vertex_const_iter it;
    Vector3f n;
    for (it = vertices_info[j].begin(); it != vertices_info[j].end(); ++it) {
      Float weight = it->first;
      const Vector3f& normal = it->second;

      weight_sum += weight;                     // accumulate the weight
      Vector3f tmp;
      tmp.scale(weight, normal);
      n.add(tmp);
    }
    n.scale(1.0f / weight_sum);
    n.normalize();
    (*m_normal_array)[j] = n;
  }
  
  for (j = 0; j < vertices_info.size(); j++) vertices_info[j].clear();
  vertices_info.clear();

  set_normal_per_vertex(true);
}

/*! \brief calculates a single normal per polygon for all polygons */
void Indexed_face_set::calculate_normals_per_polygon()
{
  m_normal_array->resize(m_num_primitives);

  Uint j = 0;
  for (Uint i = 0 ; i < m_num_primitives ; i++) {
    Vector3f& v1 = (*m_coord_array)[m_coord_indices[j]];
    Vector3f& v2 = (*m_coord_array)[m_coord_indices[j+1]];
    Vector3f& v3 = (*m_coord_array)[m_coord_indices[j+2]];

    Vector3f l1, l2;
    l1.sub(v2, v1);
    l2.sub(v3, v2);
    Vector3f n;
    if (m_is_ccw) {
      n.cross(l1, l2); 
    } else {
      n.cross(l2, l1); 
    }
    n.normalize();
    (*m_normal_array)[i] = n;

    // Advance to next primitive:
    switch (m_primitive_type) {
     case PT_TRIANGLES: j += 3; break;
     case PT_QUADS: j += 4; break;

     case PT_POLYGONS:
      for (; m_coord_indices[j] != (Uint) -1; ++j);     // advance to end
      ++j;                                      // skip the end-of-face marker
      break;
      
     case PT_TRIANGLE_STRIP: break;
     case PT_TRIANGLE_FAN: break;
     case PT_QUAD_STRIP:
     default: SGAL_assertion(0); break;
    }
  }
  set_normal_per_vertex(false);
}

/*! Allocate the texture coordinate mapping in case it is not present */
void Indexed_face_set::allocate_tex_coords()
{
  SGAL_assertion(!m_tex_coord_array);
  SGAL_assertion(m_coord_array);
  m_tex_coord_array = new Tex_coord_array(m_coord_array->size());
  m_own_tex_coord_array = true;
}

/*! Calculate the default texture coordinate mapping in case it is not present,
 *  using the shape bounding-box.
 */
void Indexed_face_set::clean_tex_coords()
{
  SGAL_assertion(m_tex_coord_array);
  SGAL_assertion(m_coord_array);

  Uint num_coords = m_coord_array->size();

  //! \todo do the right thing!
  const Vector2f t0(0,0);
  const Vector2f t1(1,0);
  const Vector2f t2(1,1);
  const Vector2f t3(0,1);
  for (Uint i = 0 ; i < num_coords; ++i) {
    Uint mask = i & 0x3;
    const Vector2f& t =
      (mask == 0x0) ? t0 : (mask == 0x1) ? t1 : (mask == 0x2) ? t2 : t3;
    (*m_tex_coord_array)[i] = t;
  }

  m_dirty_tex_coords = false;
}

/*! \brief draws the mesh conditionaly */
void Indexed_face_set::draw(Draw_action* action)
{
  if (is_dirty()) clean();
  if (is_empty()) return;
  
  // Clean the normals:
  if ((resolve_fragment_source() == FS_NORMAL) && m_dirty_normals) {
    if (!m_normal_array) allocate_normals();
    clean_normals();
  }

  // Clean the tex coordinates:
  //! add vertex buffer object for the tex-coords
  Context* context = action->get_context();
  if (context->get_tex_enable() && !(context->get_tex_gen()) &&
      m_dirty_tex_coords)
  {
    if (!m_tex_coord_array) allocate_tex_coords();
    clean_tex_coords();
  }

  draw_mesh(action);
}

/*! draw() draws the geometry.
 * For efficiency reasons, differenrt methods were written to 
 * draw geometries with different kinds of data (texture/normal/color).
 * @param action action.
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
    if (!m_vertex_buffer_object_created) {
      if (use_vertex_array() &&
          (Gfx_conf::get_instance()->is_vertex_buffer_object_supported()))
        create_vertex_buffer_object();
    }
   case Configuration::GDM_DIRECT:
    draw_dispatch(action);
    break;
  }
}

/*! Dispatch the appropriate drawing routine */
void Indexed_face_set::draw_dispatch(Draw_action* /* action */) 
{
  // When using vertex array, the index arrays must be flat:
  SGAL_assertion_code(Boolean uva = use_vertex_array(););
  SGAL_assertion(!uva || (uva && m_are_indices_flat));
  
  Fragment_source fragment_source = resolve_fragment_source();
  Boolean fragment_indexed = (fragment_source == FS_NORMAL) ?
    (m_normal_indices.size() ? true : false) :
    (m_color_indices.size() ? true : false);
  Attachment fragment_attached = (fragment_source == FS_NORMAL) ?
    ((m_normal_per_vertex) ? PER_VERTEX : PER_PRIMITIVE) :
    ((m_color_per_vertex) ? PER_VERTEX : PER_PRIMITIVE);
  Boolean texture_enbaled = m_tex_coord_array ? true : false;
  Boolean texture_indexed = m_tex_coord_indices.size() ? true : false;
  Boolean va = m_drawing_mode == Configuration::GDM_VERTEX_ARRAY;

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

/*! Isect direct drawing-mode */
void Indexed_face_set::isect_direct()
{
  Uint i, j;
  switch (m_primitive_type) {
   case PT_TRIANGLE_STRIP:
    if (m_coord_array->size()) {
      int num_tri_strips = m_tri_strip_lengths[0];
      int index = 0;
      for (int strip = 0 ; strip < num_tri_strips ; strip++) {
        int tmp = strip + 1;
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0 ; i < m_tri_strip_lengths[tmp] ; i++) {
          Vector3f& v = (*m_coord_array)[m_coord_indices[index]];
          glVertex3fv((float*)&v);
          index++;
        }
        glEnd();
      }
    }
    return;

   case PT_TRIANGLES:
    glBegin(GL_TRIANGLES);
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glVertex3fv(get_by_ci(*m_coord_array, j++));
      glVertex3fv(get_by_ci(*m_coord_array, j++));
      glVertex3fv(get_by_ci(*m_coord_array, j++));
    }
    glEnd();
    return;

   case PT_QUADS:
    glBegin(GL_QUADS);
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glVertex3fv(get_by_ci(*m_coord_array, j++));
      glVertex3fv(get_by_ci(*m_coord_array, j++));
      glVertex3fv(get_by_ci(*m_coord_array, j++));
      glVertex3fv(get_by_ci(*m_coord_array, j++));
    }
    glEnd();
    return;

   case PT_POLYGONS:
    for (i = 0, j = 0; i < m_num_primitives; ++i) {
      glBegin(GL_POLYGON);
      for (; m_coord_indices[j] != (Uint) -1; ++j) {
        glVertex3fv(get_by_ci(*m_coord_array, j));
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

/*!
 */
void Indexed_face_set::isect(Isect_action* action)
{
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

/*! Create a new display list. This is called after each update */
int Indexed_face_set::create_display_list(Draw_action* action)
{
  int id = glGenLists(1);
  glNewList(id, GL_COMPILE);
  draw_dispatch(action);
  glEndList();
  return id;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Indexed_face_set::set_attributes(Element* elem) 
{ 
  Mesh_set::set_attributes(elem);

  //! \todo sg->get_stats().add_num_mesh();

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
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

/*! \brief adds the container to a given scene */  
void Indexed_face_set::add_to_scene(Scene_graph* sg)
{
  Configuration* config = sg->get_configuration();
  if (config) m_drawing_mode = config->get_geometry_drawing_mode();
}

#if 0
/*! Get a list of atytributes in this object. This method is called only 
 * from the Builder side. 
 * An Indexed_face_set is always converted to an ProgIndexedTriSet 
 * and therefore any information regarding compressed data is not 
 * written out (e.g., coordIndex).
 *
 * @return a list of attributes 
 */
Attribute_list Indexed_face_set::get_attributes()
{ 
  Attribute_list attribs; 
  Attribute attrib;
  char buf[32];
  Vector3f col;

  attribs = Geometry::get_attributes();

  if(m_normal_per_vertex != s_def_normal_per_vertex) {
    attrib.first = "normalPerVertex";
    attrib.second = get_normal_per_vertex() ? "TRUE" : "FALSE";
    attribs.push_back(attrib);
  }
  if(m_normal_per_vertex != s_def_color_per_vertex) {
    attrib.first = "colorPerVertex";
    attrib.second = get_color_per_vertex() ? "TRUE" : "FALSE";
    attribs.push_back(attrib);
  }
  return attribs; 
}

#endif

/*! Initialize the container prototype */
void Indexed_face_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->
    add_field_info(new SF_bool(NORMAL_PER_VERTEX, "normalPerVertex",
                               get_member_offset(&m_normal_per_vertex),
                               exec_func));

  s_prototype->
    add_field_info(new SF_bool(COLOR_PER_VERTEX, "colorPerVertex",
                               get_member_offset(&m_color_per_vertex),
                               exec_func));
}

/*! Delete the container prototype */
void Indexed_face_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the container prototype */
Container_proto* Indexed_face_set::get_prototype() 
{  
  if (s_prototype == NULL) Indexed_face_set::init_prototype();
  return s_prototype;
}

/*! Create the data structure of the vertex buffer object */
void Indexed_face_set::create_vertex_buffer_object()
{
#if defined(GL_ARB_vertex_buffer_object)
  std::cout << "Using vertex buffer object extension!" << std::endl;
  
  destroy_vertex_buffer_object();
  Boolean error_occured = false;
  
  // Create the vertex coord buffer context */
  if (m_coord_array) {
    glGenBuffersARB(1, &m_vertex_coord_id);
    if (m_vertex_coord_id == 0) {
      std::cerr << "glGenBuffersARB failed to generate vertex coord buffer!"
                << std::endl;
      error_occured = true;
    } else {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_coord_id);
      Uint size = m_coord_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_coord_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
  }
  
  // Create the vertex normal buffer context */
  if (!error_occured && m_normal_array) {
    glGenBuffersARB(1, &m_vertex_normal_id);
    if (m_vertex_normal_id == 0) {
      std::cerr << "glGenBuffersARB failed to generate vertex normal buffer!"
                << std::endl;
      error_occured = true;
    } else {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_normal_id);
      Uint size = m_normal_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_normal_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
  }

  // Create the vertex color buffer context */
  if (!error_occured && m_color_array) {
    glGenBuffersARB(1, &m_vertex_color_id);
    if (m_vertex_color_id == 0) {
      std::cerr << "glGenBuffersARB failed to generate vertex color buffer!"
                << std::endl;
      error_occured = true;
    } else {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_color_id);
      Uint size = m_color_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_color_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
  }

  // Create the vertex texture-coordinate buffer context */
  if (!error_occured && m_tex_coord_array) {
    glGenBuffersARB(1, &m_vertex_tex_coord_id);
    if (m_vertex_tex_coord_id == 0) {
      std::cerr << "glGenBuffersARB failed to generate vertex tex_coord buffer!"
                << std::endl;
      error_occured = true;
    } else {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_tex_coord_id);
      Uint size = m_tex_coord_array->size()* sizeof(Vector2f);
      GLfloat* data = (GLfloat*) m_tex_coord_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
  }
  
  // Verify that everything is ok:
  if (!error_occured) {
    int param_array_size = 0;
    glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB,
                              &param_array_size);
    
    if (param_array_size <= 0) {
      destroy_vertex_buffer_object();
      std::cerr << "glBufferDataARB failed to allocate any memory!"
                << std::endl;
      error_occured = true;
    }
  }

  // Leave clean state:
  if (!error_occured) {
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    /*! \todo add a flag the user can set to indicate that the vertex arrays
     * can be cleared. It cannot be determined automatically.
     */
    // clear_vertex_arrays();
  } else {
    destroy_vertex_buffer_object();
  }
#endif
  m_vertex_buffer_object_created = true;
}

/*! Destroy the data structure of the vertex buffer object */
void Indexed_face_set::destroy_vertex_buffer_object()
{
#if defined(GL_ARB_vertex_buffer_object)
  if (m_vertex_coord_id || m_vertex_normal_id ||
      m_vertex_color_id || m_vertex_tex_coord_id)
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  if (m_vertex_coord_id) {
    glDeleteBuffersARB(1, &m_vertex_coord_id);
    m_vertex_coord_id = 0;
  }
  if (m_vertex_normal_id) {
    glDeleteBuffersARB(1, &m_vertex_normal_id);
    m_vertex_normal_id = 0;
  }
  if (m_vertex_color_id) {
    glDeleteBuffersARB(1, &m_vertex_color_id);
    m_vertex_color_id = 0;
  }
  if (m_vertex_tex_coord_id) {
    glDeleteBuffersARB(1, &m_vertex_tex_coord_id);
    m_vertex_tex_coord_id = 0;
  }
#endif
}

/*! Destroy the data structure of the display_list */
void Indexed_face_set::destroy_display_list()
{
  if (m_display_list_id != 0) {
    glDeleteLists(m_display_list_id, 1);
    m_display_list_id = 0;
  }
}

/*! Destroy the vertex arrays */
void Indexed_face_set::clear_vertex_arrays()
{
  if (m_coord_array) m_coord_array->clear();
  if (m_tex_coord_array) m_tex_coord_array->clear();
  if (m_normal_array) m_normal_array->clear();
  if (m_color_array) m_color_array->clear();
}

/*! Destroy the vertex-index arrays */
void Indexed_face_set::destroy_vertex_index_arrays()
{
  m_coord_indices.clear();
  m_tex_coord_indices.clear();
  m_normal_indices.clear();
  m_color_indices.clear();
}

/*! \brief clears the representation */
void Indexed_face_set::clear()
{
  std::cout << "Indexed_face_set::clear()" << std::endl;
  clear_vertex_arrays();
  destroy_vertex_index_arrays();
  destroy_display_list();
  destroy_vertex_buffer_object();

  Mesh_set::clear();
}
  
/*! Return true if the representation is empty */
Boolean Indexed_face_set::is_empty() const
{
  if (m_drawing_mode == Configuration::GDM_DISPLAY_LIST)
    return (m_display_list_id == 0);

  if (use_vertex_array()) {
    if (Gfx_conf::get_instance()->is_vertex_buffer_object_supported())
      return ((m_vertex_coord_id == 0) && Geo_set::is_empty());
  }
  
  return Geo_set::is_empty();
}

/*! \brief processes change of coordinates
 * EFEF: It is not absolutely necessary to destroy the vertex buffer.
 *       Instead, we can simply update it as done in field_changed().
 *       In any case, this is not the place to do it. We should mark
 *       the change needed here, and apply it in the appropriate *clean()
 *       function (which is invoked from the drawing routine.
 */
void Indexed_face_set::coord_changed(SGAL::Field_info* /* field_info */)
{
  if (m_own_normal_array) m_dirty_normals = true;
  if (m_own_tex_coord_array) m_dirty_tex_coords = true;
  destroy_display_list();
  destroy_vertex_buffer_object();
  Mesh_set::clear();
}

/*! \brief Process change of field
 * EFEF: This is wrong for two reasons:
 *       1. A change to the coord_array may imply a change in the normal, 
 *       color, or text_coord arrays.
 *       2. This is not the place to update the arrays. We should mark
 *       the change needed here, and apply it in the appropriate *clean()
 *       function (which is invoked from the drawing routine.
 */
void Indexed_face_set::field_changed(Field_info* field_info)
{
  std::cout << "Indexed_face_set::field_changed()" << std::endl;
  switch (field_info->get_id()) {
   case COORD_ARRAY:
#if defined(GL_ARB_vertex_buffer_object)
    if (!m_vertex_buffer_object_created) break;
    if (m_vertex_coord_id == 0) break;
    {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_coord_id);
      Uint size = m_coord_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_coord_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
#endif
    break;

   case NORMAL_ARRAY:
#if defined(GL_ARB_vertex_buffer_object)
    if (!m_vertex_buffer_object_created) break;
    if (m_vertex_normal_id == 0) break;
    {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_normal_id);
      Uint size = m_normal_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_normal_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
#endif
    break;

   case COLOR_ARRAY:
#if defined(GL_ARB_vertex_buffer_object)
    if (!m_vertex_buffer_object_created) break;
    if (m_vertex_color_id == 0) break;
    {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_color_id);
      Uint size = m_color_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_color_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
#endif
    break;

   case TEX_COORD_ARRAY:
#if defined(GL_ARB_vertex_buffer_object)
    if (!m_vertex_buffer_object_created) break;
    if (m_vertex_tex_coord_id == 0) break;
    {
      glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_tex_coord_id);
      Uint size = m_tex_coord_array->size()* sizeof(Vector3f);
      GLfloat* data = (GLfloat*) m_tex_coord_array->get_vector();
      glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, GL_DYNAMIC_DRAW_ARB);
    }
#endif
    break;
  }
  Container::field_changed(field_info);
}

SGAL_END_NAMESPACE
