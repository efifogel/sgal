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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_INDEXED_FACE_SET_MASK_HPP
#define SGAL_INDEXED_FACE_SET_MASK_HPP

#define SHIFT(mask,pos,value)       ((value << pos) & mask)
#define SET(mask,pos,value,data)    ((data) & ~mask) | ((value << pos) & mask)

// Standard (no vertex buffer):
// General polygons:
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOPO_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
    
#define FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

// Texture indexed
#define FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

// Triangles
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOTR_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
#define FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

    // Texture indexed
#define FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

// Quads:
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOQU_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                               \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
#define FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

// Texture indexed:
#define FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,                           \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))
  
// Triangle Strips:
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOTS_VANO                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                      \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0))))))

// Texture enabled:
// Texture not indexed:
#define FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

// Texture indexed:
#define FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,                  \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,0,0x0)))))))

// Vertex buffer:
// General polygons:
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOPO_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS,     \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
    
#define FSNO_FINO_FAPV_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

// Texture indexed
#define FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_POLYGONS, \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

// Triangles
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOTR_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,    \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
#define FSNO_FINO_FAPV_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

    // Texture indexed
#define FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLES,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

// Quads:
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOQU_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,        \
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
#define FSNO_FINO_FAPV_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

// Texture indexed:
#define FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_QUADS,    \
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))
  
// Triangle Strips:
// Texture disabled:
#define FSNO_FINO_FAPV_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPV_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPV_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPV_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPT_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPT_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPT_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPT_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FINO_FAPM_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FINO_FAPM_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSNO_FIYE_FAPM_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

#define FSCO_FIYE_FAPM_TENO_MOTS_VAYE                                   \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,0,                 \
                    SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                        SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0))))))

// Texture enabled:
// Texture not indexed:
#define FSNO_FINO_FAPV_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,0,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

// Texture indexed:
#define FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_VERTEX,    \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_PRIMITIVE, \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,0,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_NORMAL,                     \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#define FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VAYE                              \
    SET(FRAGMENT_SOURCE,FRAGMENT_SOURCE_,FS_COLOR,                      \
        SET(FRAGMENT_INDEXED,FRAGMENT_INDEXED_,1,                       \
            SET(FRAGMENT_ATTACHMENT,FRAGMENT_ATTACHMENT_,PER_MESH,      \
                SET(TEXTURE_ENABLED,TEXTURE_ENABLED_,1,                 \
                    SET(TEXTURE_INDEXED,TEXTURE_INDEXED_,1,             \
                        SET(PRIMITIVE_TYPE,PRIMITIVE_TYPE_,PT_TRIANGLE_STRIP,\
                            SET(VERTEX_ARRAY,VERTEX_ARRAY_,1,0x0)))))))

#endif
