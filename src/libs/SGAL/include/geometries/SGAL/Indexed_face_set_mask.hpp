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

#ifndef SGAL_INDEXED_FACE_SET_MASK_HPP
#define SGAL_INDEXED_FACE_SET_MASK_HPP

#include "SGAL/basic.hpp"
#include "SGAL/set_bits.hpp"

SGAL_BEGIN_NAMESPACE

// Standard (no vertex buffer):
// General polygons:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOPO_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOPO_VANO

#define SGAL_FSCO_FIYE_FAPM_TENO_MOPO_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:

#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Texture indexed
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Triangles
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOTR_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOTR_VANO

#define SGAL_FSCO_FIYE_FAPM_TENO_MOTR_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

    // Texture indexed
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Quads:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOQU_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOQU_VANO

#define SGAL_FSCO_FIYE_FAPM_TENO_MOQU_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Texture indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Triangle Strips:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOTS_VANO

#define SGAL_FSCO_FINO_FAPM_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOTS_VANO

#define SGAL_FSCO_FIYE_FAPM_TENO_MOTS_VANO                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Texture indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,0,0x0)))))))

// Vertex buffer:
// General polygons:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOPO_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOPO_VAYE

#define SGAL_FSCO_FIYE_FAPM_TENO_MOPO_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,          \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:

#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// Texture indexed
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_POLYGONS,        \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// Triangles
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOTR_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOTR_VAYE

#define SGAL_FSCO_FIYE_FAPM_TENO_MOTR_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,         \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

    // Texture indexed
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLES,       \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// Quads:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOQU_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOQU_VAYE

#define SGAL_FSCO_FIYE_FAPM_TENO_MOQU_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,             \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// Texture indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_QUADS,           \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// Triangle Strips:
// Texture disabled:
#define SGAL_FSNO_FINO_FAPV_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPV_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPV_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPV_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FINO_FAPT_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FINO_FAPT_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSNO_FIYE_FAPT_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

#define SGAL_FSCO_FIYE_FAPT_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TENO_MOTS_VAYE

#define SGAL_FSCO_FINO_FAPM_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TENO_MOTS_VAYE

#define SGAL_FSCO_FIYE_FAPM_TENO_MOTS_VAYE                          \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,0,          \
          set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,    \
            set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,0,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// Texture indexed:
#define SGAL_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_VERTEX,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_NORMAL,                \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

#define SGAL_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_PRIMITIVE,\
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE

#define SGAL_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,0,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

// invalid
// #define SGAL_FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VAYE

#define SGAL_FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VAYE                     \
  set_bits(BO_FRAG_SOURCE,BO_FRAG_SOURCE_,FS_COLOR,                 \
    set_bits(BO_FRAG_INDEXED,BO_FRAG_INDEXED_,1,                    \
      set_bits(BO_FRAG_ATTACHMENT,BO_FRAG_ATTACHMENT_,AT_PER_MESH,  \
        set_bits(BO_TEXTURE_ENABLED,BO_TEXTURE_ENABLED_,1,          \
          set_bits(BO_TEXTURE_INDEXED,BO_TEXTURE_INDEXED_,1,        \
            set_bits(BO_PRIM_TYPE,BO_PRIM_TYPE_,PT_TRIANGLE_STRIP,  \
              set_bits(BO_VERTEX_ARRAY,BO_VERTEX_ARRAY_,1,0x0)))))))

SGAL_END_NAMESPACE

#endif
