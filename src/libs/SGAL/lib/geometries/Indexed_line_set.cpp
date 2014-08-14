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
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Indexed_line_set_mask.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Indexed_line_set::s_tag = "IndexedLineSet";
Container_proto* Indexed_line_set::s_prototype(nullptr);

// Default values:
const Boolean Indexed_line_set::m_def_color_per_vertex(true);
const Boolean Indexed_line_set::m_def_normal_per_vertex(true);
const Float Indexed_line_set::s_def_line_width(1);

void (Indexed_line_set::*Indexed_line_set::m_draws[SGAL_NUM_LI_DRAWS])();
Boolean Indexed_line_set::m_draws_initialized(false);

REGISTER_TO_FACTORY(Indexed_line_set, "Indexed_line_set");

//! \brief constructor.
Indexed_line_set::Indexed_line_set(Boolean proto) :
  Geo_set(proto),
  m_color_per_vertex(m_def_color_per_vertex),
  m_normal_per_vertex(m_def_normal_per_vertex),
  m_line_width(s_def_line_width),
  m_elliminate_hiden(true),
  m_has_texture(false),
  m_bb_is_pre_set(false),
  m_use_display_list(false),
  m_display_list_id(-1)
{
  m_primitive_type = PT_LINE_STRIPS;

  if (m_draws_initialized) return;
  m_draws_initialized = true;

  // Initialize static draws[] array:
  for (auto i = 0; i < SGAL_NUM_LI_DRAWS; ++i)
    m_draws[i] = &Indexed_line_set::draw_invalid;

  // Standard (no vertex array):
  // Lines:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLI_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TENO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLI_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLI_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO;

  // Line strips:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TENO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLS_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO;

  // Line loops:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLL_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLL_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TENO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLL_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLL_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO;


  // Vertex array:
  // Lines:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TENO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TENO_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLI_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TENO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLI_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLI_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLI_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO;

  // Line strips:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TENO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLS_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLS_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO;

  // Line loops:
  // Texture disabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TENO_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TENO_MOLL_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TENO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TENO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TENO_MOLL_VANO;

  // Texture enabled:
  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FAPV_VAYE;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TINO_MOLL_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO;

  m_draws[SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO;

  m_draws[SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FINO_FAPT_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPT_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOLL_VANO;

  m_draws[SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO;
  m_draws[SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VAYE] =
    &Indexed_line_set::draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO;
}

//! \brief destructor.
Indexed_line_set::~Indexed_line_set() {}

/* \brief sets the flag that indicates whether normals are bound per vertex
 * or per line.
 */
void Indexed_line_set::set_normal_per_vertex(Boolean normal_per_vertex)
{
  m_normal_per_vertex = normal_per_vertex;
  m_normal_attachment = (normal_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/* \brief sets the flag that indicates whether colors are bound per vertex or
 * per line.
 */
void Indexed_line_set::set_color_per_vertex(Boolean color_per_vertex)
{
  m_color_per_vertex = color_per_vertex;
  m_color_attachment = (color_per_vertex) ? PER_VERTEX : PER_PRIMITIVE;
}

/*! \brief draws the geometry.
 * For efficiency reasons, differenrt methods were written to
 * draw geometries with different kinds of data (texture/normal/color).
 */
void Indexed_line_set::draw(Draw_action* action)
{
  Context* context = action->get_context();
  if (!m_elliminate_hiden) {
    context->draw_depth_mask(false);
    context->draw_depth_enable(false);
  }
  context->draw_line_width(m_line_width);

  Fragment_source fragment_source = resolve_fragment_source();
  Attachment fragment_attached = (fragment_source == FS_NORMAL) ?
    m_normal_attachment : m_color_attachment;

  if (fragment_source == FS_COLOR)
    if (fragment_attached == PER_MESH) glColor3fv(get(m_color_array, 0));

  if (m_primitive_type == PT_LINES) {
    Uint j = 0;
    Uint k = 0;
    glBegin(GL_LINES);

    for (Uint i = 0; i < m_num_primitives; ++i) {
      if ((fragment_attached == PER_VERTEX) ||
          ((fragment_attached == PER_PRIMITIVE) && ((j & 0x1) == 0x0)))
      {
        if (fragment_source == FS_COLOR) {
          if (m_color_array) glColor3fv(get_by_coord_index(m_color_array, k));
        }
        else {
          if (m_normal_array)
            glNormal3fv(get_by_coord_index(m_normal_array, k));
        }
        ++k;
      }
      glVertex3fv(get_by_coord_index(m_coord_array, j));
      glVertex3fv(get_by_coord_index(m_coord_array, j+1));
      j += 2;
    }
    glEnd();
  }
  else if (m_primitive_type == PT_LINE_STRIPS) {
    Uint j = 0;
    for (Uint i = 0; i < m_num_primitives; ++i) {
      if (fragment_attached == PER_PRIMITIVE) {
        if (fragment_source == FS_COLOR) {
          if (m_color_array) glColor3fv(get_by_coord_index(m_color_array, j));
        }
        else {
          if (m_normal_array)
            glNormal3fv(get_by_coord_index(m_normal_array, j));
        }
      }
      glBegin(GL_LINE_STRIP);
      for (; m_coord_indices[j] != static_cast<Uint>(-1); ++j) {
        if (fragment_attached == PER_VERTEX) {
          if (fragment_source == FS_COLOR) {
            if (m_color_array)
              glColor3fv(get_by_coord_index(m_color_array, j));
          }
          else {
            if (m_normal_array)
              glNormal3fv(get_by_coord_index(m_normal_array, j));
          }
        }
        glVertex3fv(get_by_coord_index(m_coord_array, j));
      }
      glEnd();
      ++j;
    }
  }

  context->draw_line_width(1.0f);
  if (!m_elliminate_hiden) {
    context->draw_depth_enable(true);
    context->draw_depth_mask(true);
  }
}

//! \brief
void Indexed_line_set::isect(Isect_action* /* action */) { }

/*! \brief calculates the sphere bound of the lines. Returns true if the BS has
 * changed since lst time this was called.
 */
bool Indexed_line_set::clean_sphere_bound()
{
  if (!m_bb_is_pre_set && m_coord_array) {
    boost::shared_ptr<Coord_array_3d> coord_array =
      boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
    SGAL_assertion(coord_array);
    m_sphere_bound.set_around(coord_array->begin(), coord_array->end());
  }
  m_dirty_sphere_bound = false;
  return true;
}

//! \brief setss the attributes of the geometry object.
void Indexed_line_set::set_attributes(Element* elem)
{
  Geo_set::set_attributes(elem);

  std::string normal_indices_string;
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "colorPerVertex") {
      set_color_per_vertex(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "normalPerVertex") {
      set_normal_per_vertex(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "lineWidth") {
      set_line_width(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "elliminateHiden") {
      m_elliminate_hiden = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief sets the attributes of this node.
void Indexed_line_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geo_set::get_prototype());

  Float_handle_function line_width_func =
    static_cast<Float_handle_function>(&Indexed_line_set::line_width_handle);
  s_prototype->add_field_info(new SF_float(LINE_WIDTH, "lineWidth",
                                           RULE_EXPOSED_FIELD,
                                           line_width_func));
}

//! \brief deletes the prototype.
void Indexed_line_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Indexed_line_set::get_prototype()
{
  if (!s_prototype) Indexed_line_set::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
