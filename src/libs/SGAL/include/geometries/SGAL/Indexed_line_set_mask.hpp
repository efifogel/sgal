// Copyright (c) 2014 Israel.
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

#ifndef SGAL_INDEXED_LINE_SET_MASK_HPP
#define SGAL_INDEXED_LINE_SET_MASK_HPP

#include "SGAL/basic.hpp"
#include "SGAL/set_bits.hpp"

SGAL_BEGIN_NAMESPACE

// General lines:
// Texture mapping disabled:
// Standard (no vertex buffer):

// Fragment attachment, none:
#define SGAL_FANO_TENO_MOLI_VANO                                    \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,              \
      set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,                 \
        set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))

// Fragment attachment:
#define SGAL_FSNO_FINO_FAPV_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPL_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPL_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPL_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPL_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

// #define SGAL_FSNO_FINO_FAPM_TENO_MOLI_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOLI_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

// #define SGAL_FSNO_FIYE_FAPM_TENO_MOLI_VANO
// #define SGAL_FSCO_FIYE_FAPM_TENO_MOLI_VANO

// Texture enabled:
// Texture not indexed:

// Fragment attachment, none:
#define SGAL_FANO_TEYE_TINO_MOLI_VANO                               \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,              \
      set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,            \
        set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,               \
          set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))

// Fragment attachment:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOLI_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOLI_VANO
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO

// Texture indexed

// Fragment attachment, none
#define SGAL_FANO_TEYE_TIYE_MOLI_VANO                               \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,              \
      set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,            \
        set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,               \
                 set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))

#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOLI_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO

// line strips:
// Texture disabled:

// Fragment attachment, none:
#define SGAL_FANO_TENO_MOLS_VANO                                    \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,              \
      set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,           \
        set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))

// Fragment attachment:
#define SGAL_FSNO_FINO_FAPV_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOLS_VANO                              \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPL_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPL_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPL_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPL_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

// #define SGAL_FSNO_FINO_FAPM_TENO_MOLS_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOLS_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

// #define SGAL_FSNO_FIYE_FAPM_TENO_MOLS_VANO
// #define SGAL_FSCO_FIYE_FAPM_TENO_MOLS_VANO

// Texture enabled:
// Texture not indexed:

// Fragment attachment, none:
#define SGAL_FANO_TEYE_TINO_MOLS_VANO                               \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,              \
      set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,            \
        set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,         \
          set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))

#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOLS_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOLS_VANO
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO

// Texture indexed

// Fragment attachment, none:
#define SGAL_FANO_TEYE_TIYE_MOLS_VANO                               \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,              \
      set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,            \
        set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,         \
          set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))

// Fragment attachment, none:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOLS_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO

// line loops:
// Texture disabled:

// Fragment attachment, none:
#define SGAL_FANO_TENO_MOLL_VANO                                    \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,              \
      set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,            \
        set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))

// Fragment attachment:
#define SGAL_FSNO_FINO_FAPV_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPL_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPL_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPL_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPL_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

// #define SGAL_FSNO_FINO_FAPM_TENO_MOLL_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOLL_VANO                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0))))))

// #define SGAL_FSNO_FIYE_FAPM_TENO_MOLL_VANO
// #define SGAL_FSCO_FIYE_FAPM_TENO_MOLL_VANO

// Texture enabled:
// Texture not indexed:

// Fragment attachment, none:
#define SGAL_FANO_TEYE_TINO_MOLL_VANO                               \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,              \
      set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,            \
        set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,          \
          set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))

// Fragment attachment:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOLL_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOLL_VANO
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO

// Texture indexed:

// Fragment attachment, none:
#define SGAL_FANO_TEYE_TIYE_MOLL_VANO                               \
  set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_NONE,          \
    set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,              \
      set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,            \
        set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,          \
          set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))

// Fragment attachment:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOLL_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,0,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO

// Vertex buffer:
// General lines:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPL_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPL_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPL_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPL_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

// #define SGAL_FSNO_FINO_FAPM_TENO_MOLI_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOLI_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,             \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

// #define SGAL_FSNO_FIYE_FAPM_TENO_MOLI_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TENO_MOLI_VAYE

// Texture enabled:
// Texture not indexed:

#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOLI_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOLI_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VAYE

// Texture indexed
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOLI_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINES,           \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOLI_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VAYE

// line strips:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPL_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPL_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPL_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPL_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

// #define SGAL_FSNO_FINO_FAPM_TENO_MOLS_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOLS_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,       \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

// #define SGAL_FSNO_FIYE_FAPM_TENO_MOLS_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TENO_MOLS_VAYE

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOLS_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOLS_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VAYE

    // Texture indexed
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOLS_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_STRIPS,     \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOLS_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VAYE

// line loops:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPL_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPL_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPL_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPL_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

// #define SGAL_FSNO_FINO_FAPM_TENO_MOLL_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOLL_VAYE                          \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,0,          \
          set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,        \
            set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0))))))

// #define SGAL_FSNO_FIYE_FAPM_TENO_MOLL_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TENO_MOLL_VAYE

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOLL_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,0,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOLL_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VAYE

// Texture indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPL_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPL_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPL_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPL_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,1,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOLL_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VAYE                     \
  set_bits(LI_FRAG_SOURCE,LI_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(LI_FRAG_INDEXED,LI_FRAG_INDEXED_,0,                    \
      set_bits(LI_FRAG_ATTACHMENT,LI_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(LI_TEXTURE_ENABLED,LI_TEXTURE_ENABLED_,1,          \
          set_bits(LI_TEXTURE_INDEXED,LI_TEXTURE_INDEXED_,1,        \
            set_bits(LI_PRIM_TYPE,LI_PRIM_TYPE_,PT_LINE_LOOPS,      \
              set_bits(LI_VERTEX_ARRAY,LI_VERTEX_ARRAY_,1,0x0)))))))

// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOLL_VAYE
// #define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VAYE

SGAL_END_NAMESPACE

#endif
