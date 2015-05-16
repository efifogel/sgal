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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Gl_wrapper* Gl_wrapper::s_instance = NULL;

/*! Constructor */
Gl_wrapper::Gl_wrapper()
{
  m_gl_token_names[0xffffffff] = "-1";
  m_gl_token_names[GL_VERSION_1_1] = "GL_VERSION_1_1";

  /* AccumOp */
  m_gl_token_names[GL_ACCUM] = "GL_ACCUM";
  m_gl_token_names[GL_LOAD] = "GL_LOAD";
  m_gl_token_names[GL_RETURN] = "GL_RETURN";
  m_gl_token_names[GL_MULT] = "GL_MULT";
  m_gl_token_names[GL_ADD] = "GL_ADD";

  /* AlphaFunction */
  m_gl_token_names[GL_NEVER] = "GL_NEVER";
  m_gl_token_names[GL_LESS] = "GL_LESS";
  m_gl_token_names[GL_EQUAL] = "GL_EQUAL";
  m_gl_token_names[GL_LEQUAL] = "GL_LEQUAL";
  m_gl_token_names[GL_GREATER] = "GL_GREATER";
  m_gl_token_names[GL_NOTEQUAL] = "GL_NOTEQUAL";
  m_gl_token_names[GL_GEQUAL] = "GL_GEQUAL";
  m_gl_token_names[GL_ALWAYS] = "GL_ALWAYS";

  /* AttribMask */
  m_gl_token_names[GL_CURRENT_BIT] = "GL_CURRENT_BIT";
  m_gl_token_names[GL_POINT_BIT] = "GL_POINT_BIT";
  m_gl_token_names[GL_LINE_BIT] = "GL_LINE_BIT";
  m_gl_token_names[GL_POLYGON_BIT] = "GL_POLYGON_BIT";
  m_gl_token_names[GL_POLYGON_STIPPLE_BIT] = "GL_POLYGON_STIPPLE_BIT";
  m_gl_token_names[GL_PIXEL_MODE_BIT] = "GL_PIXEL_MODE_BIT";
  m_gl_token_names[GL_LIGHTING_BIT] = "GL_LIGHTING_BIT";
  m_gl_token_names[GL_FOG_BIT] = "GL_FOG_BIT";
  m_gl_token_names[GL_DEPTH_BUFFER_BIT] = "GL_DEPTH_BUFFER_BIT";
  m_gl_token_names[GL_ACCUM_BUFFER_BIT] = "GL_ACCUM_BUFFER_BIT";
  m_gl_token_names[GL_STENCIL_BUFFER_BIT] = "GL_STENCIL_BUFFER_BIT";
  m_gl_token_names[GL_VIEWPORT_BIT] = "GL_VIEWPORT_BIT";
  m_gl_token_names[GL_TRANSFORM_BIT] = "GL_TRANSFORM_BIT";
  m_gl_token_names[GL_ENABLE_BIT] = "GL_ENABLE_BIT";
  m_gl_token_names[GL_COLOR_BUFFER_BIT] = "GL_COLOR_BUFFER_BIT";
  m_gl_token_names[GL_HINT_BIT] = "GL_HINT_BIT";
  m_gl_token_names[GL_EVAL_BIT] = "GL_EVAL_BIT";
  m_gl_token_names[GL_LIST_BIT] = "GL_LIST_BIT";
  m_gl_token_names[GL_TEXTURE_BIT] = "GL_TEXTURE_BIT";
  m_gl_token_names[GL_SCISSOR_BIT] = "GL_SCISSOR_BIT";
  m_gl_token_names[GL_ALL_ATTRIB_BITS] = "GL_ALL_ATTRIB_BITS";

  /* BeginMode */
  m_gl_token_names[GL_POINTS] = "GL_POINTS";
  m_gl_token_names[GL_LINES] = "GL_LINES";
  m_gl_token_names[GL_LINE_LOOP] = "GL_LINE_LOOP";
  m_gl_token_names[GL_LINE_STRIP] = "GL_LINE_STRIP";
  m_gl_token_names[GL_TRIANGLES] = "GL_TRIANGLES";
  m_gl_token_names[GL_TRIANGLE_STRIP] = "GL_TRIANGLE_STRIP";
  m_gl_token_names[GL_TRIANGLE_FAN] = "GL_TRIANGLE_FAN";
  m_gl_token_names[GL_QUADS] = "GL_QUADS";
  m_gl_token_names[GL_QUAD_STRIP] = "GL_QUAD_STRIP";
  m_gl_token_names[GL_POLYGON] = "GL_POLYGON";

  /* BlendingFactorDest */
  // m_gl_token_names[GL_ZERO] = "GL_ZERO";     // GL_FALSE
  // m_gl_token_names[GL_ONE] = "GL_ONE";       // GL_ONE
  m_gl_token_names[GL_SRC_COLOR] = "GL_SRC_COLOR";
  m_gl_token_names[GL_ONE_MINUS_SRC_COLOR] = "GL_ONE_MINUS_SRC_COLOR";
  m_gl_token_names[GL_SRC_ALPHA] = "GL_SRC_ALPHA";
  m_gl_token_names[GL_ONE_MINUS_SRC_ALPHA] = "GL_ONE_MINUS_SRC_ALPHA";
  m_gl_token_names[GL_DST_ALPHA] = "GL_DST_ALPHA";
  m_gl_token_names[GL_ONE_MINUS_DST_ALPHA] = "GL_ONE_MINUS_DST_ALPHA";

  /* BlendingFactorSrc */
  /*      GL_ZERO */
  /*      GL_ONE */
  m_gl_token_names[GL_DST_COLOR] = "GL_DST_COLOR";
  m_gl_token_names[GL_ONE_MINUS_DST_COLOR] = "GL_ONE_MINUS_DST_COLOR";
  m_gl_token_names[GL_SRC_ALPHA_SATURATE] = "GL_SRC_ALPHA_SATURATE";
  /*      GL_SRC_ALPHA */
  /*      GL_ONE_MINUS_SRC_ALPHA */
  /*      GL_DST_ALPHA */
  /*      GL_ONE_MINUS_DST_ALPHA */

  /* Boolean */
  /*      GL_TRUE */
  /*      GL_FALSE */

  /* ClearBufferMask */
  /*      GL_COLOR_BUFFER_BIT */
  /*      GL_ACCUM_BUFFER_BIT */
  /*      GL_STENCIL_BUFFER_BIT */
  /*      GL_DEPTH_BUFFER_BIT */

  /* ClientArrayType */
  /*      GL_VERTEX_ARRAY */
  /*      GL_NORMAL_ARRAY */
  /*      GL_COLOR_ARRAY */
  /*      GL_INDEX_ARRAY */
  /*      GL_TEXTURE_COORD_ARRAY */
  /*      GL_EDGE_FLAG_ARRAY */

  /* ClipPlaneName */
  m_gl_token_names[GL_CLIP_PLANE0] = "GL_CLIP_PLANE0";
  m_gl_token_names[GL_CLIP_PLANE1] = "GL_CLIP_PLANE1";
  m_gl_token_names[GL_CLIP_PLANE2] = "GL_CLIP_PLANE2";
  m_gl_token_names[GL_CLIP_PLANE3] = "GL_CLIP_PLANE3";
  m_gl_token_names[GL_CLIP_PLANE4] = "GL_CLIP_PLANE4";
  m_gl_token_names[GL_CLIP_PLANE5] = "GL_CLIP_PLANE5";

  /* ColorMaterialFace */
  /*      GL_FRONT */
  /*      GL_BACK */
  /*      GL_FRONT_AND_BACK */

  /* ColorMaterialParameter */
  /*      GL_AMBIENT */
  /*      GL_DIFFUSE */
  /*      GL_SPECULAR */
  /*      GL_EMISSION */
  /*      GL_AMBIENT_AND_DIFFUSE */

  /* ColorPointerType */
  /*      GL_BYTE */
  /*      GL_UNSIGNED_BYTE */
  /*      GL_SHORT */
  /*      GL_UNSIGNED_SHORT */
  /*      GL_INT */
  /*      GL_UNSIGNED_INT */
  /*      GL_FLOAT */
  /*      GL_DOUBLE */

  /* CullFaceMode */
  /*      GL_FRONT */
  /*      GL_BACK */
  /*      GL_FRONT_AND_BACK */

  /* DataType */
  m_gl_token_names[GL_BYTE] = "GL_BYTE";
  m_gl_token_names[GL_UNSIGNED_BYTE] = "GL_UNSIGNED_BYTE";
  m_gl_token_names[GL_SHORT] = "GL_SHORT";
  m_gl_token_names[GL_UNSIGNED_SHORT] = "GL_UNSIGNED_SHORT";
  m_gl_token_names[GL_INT] = "GL_INT";
  m_gl_token_names[GL_UNSIGNED_INT] = "GL_UNSIGNED_INT";
  m_gl_token_names[GL_FLOAT] = "GL_FLOAT";
  m_gl_token_names[GL_2_BYTES] = "GL_2_BYTES";
  m_gl_token_names[GL_3_BYTES] = "GL_3_BYTES";
  m_gl_token_names[GL_4_BYTES] = "GL_4_BYTES";
  m_gl_token_names[GL_DOUBLE] = "GL_DOUBLE";

  /* DepthFunction */
  /*      GL_NEVER */
  /*      GL_LESS */
  /*      GL_EQUAL */
  /*      GL_LEQUAL */
  /*      GL_GREATER */
  /*      GL_NOTEQUAL */
  /*      GL_GEQUAL */
  /*      GL_ALWAYS */

  /* DrawBufferMode */
  m_gl_token_names[GL_NONE] = "GL_NONE";
  m_gl_token_names[GL_FRONT_LEFT] = "GL_FRONT_LEFT";
  m_gl_token_names[GL_FRONT_RIGHT] = "GL_FRONT_RIGHT";
  m_gl_token_names[GL_BACK_LEFT] = "GL_BACK_LEFT";
  m_gl_token_names[GL_BACK_RIGHT] = "GL_BACK_RIGHT";
  m_gl_token_names[GL_FRONT] = "GL_FRONT";
  m_gl_token_names[GL_BACK] = "GL_BACK";
  m_gl_token_names[GL_LEFT] = "GL_LEFT";
  m_gl_token_names[GL_RIGHT] = "GL_RIGHT";
  m_gl_token_names[GL_FRONT_AND_BACK] = "GL_FRONT_AND_BACK";
  m_gl_token_names[GL_AUX0] = "GL_AUX0";
  m_gl_token_names[GL_AUX1] = "GL_AUX1";
  m_gl_token_names[GL_AUX2] = "GL_AUX2";
  m_gl_token_names[GL_AUX3] = "GL_AUX3";

  /* Enable */
  /*      GL_FOG */
  /*      GL_LIGHTING */
  /*      GL_TEXTURE_1D */
  /*      GL_TEXTURE_2D */
  /*      GL_LINE_STIPPLE */
  /*      GL_POLYGON_STIPPLE */
  /*      GL_CULL_FACE */
  /*      GL_ALPHA_TEST */
  /*      GL_BLEND */
  /*      GL_INDEX_LOGIC_OP */
  /*      GL_COLOR_LOGIC_OP */
  /*      GL_DITHER */
  /*      GL_STENCIL_TEST */
  /*      GL_DEPTH_TEST */
  /*      GL_CLIP_PLANE0 */
  /*      GL_CLIP_PLANE1 */
  /*      GL_CLIP_PLANE2 */
  /*      GL_CLIP_PLANE3 */
  /*      GL_CLIP_PLANE4 */
  /*      GL_CLIP_PLANE5 */
  /*      GL_LIGHT0 */
  /*      GL_LIGHT1 */
  /*      GL_LIGHT2 */
  /*      GL_LIGHT3 */
  /*      GL_LIGHT4 */
  /*      GL_LIGHT5 */
  /*      GL_LIGHT6 */
  /*      GL_LIGHT7 */
  /*      GL_TEXTURE_GEN_S */
  /*      GL_TEXTURE_GEN_T */
  /*      GL_TEXTURE_GEN_R */
  /*      GL_TEXTURE_GEN_Q */
  /*      GL_MAP1_VERTEX_3 */
  /*      GL_MAP1_VERTEX_4 */
  /*      GL_MAP1_COLOR_4 */
  /*      GL_MAP1_INDEX */
  /*      GL_MAP1_NORMAL */
  /*      GL_MAP1_TEXTURE_COORD_1 */
  /*      GL_MAP1_TEXTURE_COORD_2 */
  /*      GL_MAP1_TEXTURE_COORD_3 */
  /*      GL_MAP1_TEXTURE_COORD_4 */
  /*      GL_MAP2_VERTEX_3 */
  /*      GL_MAP2_VERTEX_4 */
  /*      GL_MAP2_COLOR_4 */
  /*      GL_MAP2_INDEX */
  /*      GL_MAP2_NORMAL */
  /*      GL_MAP2_TEXTURE_COORD_1 */
  /*      GL_MAP2_TEXTURE_COORD_2 */
  /*      GL_MAP2_TEXTURE_COORD_3 */
  /*      GL_MAP2_TEXTURE_COORD_4 */
  /*      GL_POINT_SMOOTH */
  /*      GL_LINE_SMOOTH */
  /*      GL_POLYGON_SMOOTH */
  /*      GL_SCISSOR_TEST */
  /*      GL_COLOR_MATERIAL */
  /*      GL_NORMALIZE */
  /*      GL_AUTO_NORMAL */
  /*      GL_VERTEX_ARRAY */
  /*      GL_NORMAL_ARRAY */
  /*      GL_COLOR_ARRAY */
  /*      GL_INDEX_ARRAY */
  /*      GL_TEXTURE_COORD_ARRAY */
  /*      GL_EDGE_FLAG_ARRAY */
  /*      GL_POLYGON_OFFSET_POINT */
  /*      GL_POLYGON_OFFSET_LINE */
  /*      GL_POLYGON_OFFSET_FILL */

  /* ErrorCode */
  m_gl_token_names[GL_NO_ERROR] = "GL_NO_ERROR";
  m_gl_token_names[GL_INVALID_ENUM] = "GL_INVALID_ENUM";
  m_gl_token_names[GL_INVALID_VALUE] = "GL_INVALID_VALUE";
  m_gl_token_names[GL_INVALID_OPERATION] = "GL_INVALID_OPERATION";
  m_gl_token_names[GL_STACK_OVERFLOW] = "GL_STACK_OVERFLOW";
  m_gl_token_names[GL_STACK_UNDERFLOW] = "GL_STACK_UNDERFLOW";
  m_gl_token_names[GL_OUT_OF_MEMORY] = "GL_OUT_OF_MEMORY";

  /* FeedBackMode */
  m_gl_token_names[GL_2D] = "GL_2D";
  m_gl_token_names[GL_3D] = "GL_3D";
  m_gl_token_names[GL_3D_COLOR] = "GL_3D_COLOR";
  m_gl_token_names[GL_3D_COLOR_TEXTURE] = "GL_3D_COLOR_TEXTURE";
  m_gl_token_names[GL_4D_COLOR_TEXTURE] = "GL_4D_COLOR_TEXTURE";

  /* FeedBackToken */
  m_gl_token_names[GL_PASS_THROUGH_TOKEN] = "GL_PASS_THROUGH_TOKEN";
  m_gl_token_names[GL_POINT_TOKEN] = "GL_POINT_TOKEN";
  m_gl_token_names[GL_LINE_TOKEN] = "GL_LINE_TOKEN";
  m_gl_token_names[GL_POLYGON_TOKEN] = "GL_POLYGON_TOKEN";
  m_gl_token_names[GL_BITMAP_TOKEN] = "GL_BITMAP_TOKEN";
  m_gl_token_names[GL_DRAW_PIXEL_TOKEN] = "GL_DRAW_PIXEL_TOKEN";
  m_gl_token_names[GL_COPY_PIXEL_TOKEN] = "GL_COPY_PIXEL_TOKEN";
  m_gl_token_names[GL_LINE_RESET_TOKEN] = "GL_LINE_RESET_TOKEN";

  /* FogMode */
  /*      GL_LINEAR */
  m_gl_token_names[GL_EXP] = "GL_EXP";
  m_gl_token_names[GL_EXP2] = "GL_EXP2";


  /* FogParameter */
  /*      GL_FOG_COLOR */
  /*      GL_FOG_DENSITY */
  /*      GL_FOG_END */
  /*      GL_FOG_INDEX */
  /*      GL_FOG_MODE */
  /*      GL_FOG_START */

  /* FrontFaceDirection */
  m_gl_token_names[GL_CW] = "GL_CW";
  m_gl_token_names[GL_CCW] = "GL_CCW";

  /* GetMapTarget */
  m_gl_token_names[GL_COEFF] = "GL_COEFF";
  m_gl_token_names[GL_ORDER] = "GL_ORDER";
  m_gl_token_names[GL_DOMAIN] = "GL_DOMAIN";

  /* GetPixelMap */
  /*      GL_PIXEL_MAP_I_TO_I */
  /*      GL_PIXEL_MAP_S_TO_S */
  /*      GL_PIXEL_MAP_I_TO_R */
  /*      GL_PIXEL_MAP_I_TO_G */
  /*      GL_PIXEL_MAP_I_TO_B */
  /*      GL_PIXEL_MAP_I_TO_A */
  /*      GL_PIXEL_MAP_R_TO_R */
  /*      GL_PIXEL_MAP_G_TO_G */
  /*      GL_PIXEL_MAP_B_TO_B */
  /*      GL_PIXEL_MAP_A_TO_A */

  /* GetPointerTarget */
  /*      GL_VERTEX_ARRAY_POINTER */
  /*      GL_NORMAL_ARRAY_POINTER */
  /*      GL_COLOR_ARRAY_POINTER */
  /*      GL_INDEX_ARRAY_POINTER */
  /*      GL_TEXTURE_COORD_ARRAY_POINTER */
  /*      GL_EDGE_FLAG_ARRAY_POINTER */

  /* GetTarget */
  m_gl_token_names[GL_CURRENT_COLOR] = "GL_CURRENT_COLOR";
  m_gl_token_names[GL_CURRENT_INDEX] = "GL_CURRENT_INDEX";
  m_gl_token_names[GL_CURRENT_NORMAL] = "GL_CURRENT_NORMAL";
  m_gl_token_names[GL_CURRENT_TEXTURE_COORDS] = "GL_CURRENT_TEXTURE_COORDS";
  m_gl_token_names[GL_CURRENT_RASTER_COLOR] = "GL_CURRENT_RASTER_COLOR";
  m_gl_token_names[GL_CURRENT_RASTER_INDEX] = "GL_CURRENT_RASTER_INDEX";
  m_gl_token_names[GL_CURRENT_RASTER_TEXTURE_COORDS] =
    "GL_CURRENT_RASTER_TEXTURE_COORDS";
  m_gl_token_names[GL_CURRENT_RASTER_POSITION] = "GL_CURRENT_RASTER_POSITION";
  m_gl_token_names[GL_CURRENT_RASTER_POSITION_VALID] =
    "GL_CURRENT_RASTER_POSITION_VALID";
  m_gl_token_names[GL_CURRENT_RASTER_DISTANCE] = "GL_CURRENT_RASTER_DISTANCE";
  m_gl_token_names[GL_POINT_SMOOTH] = "GL_POINT_SMOOTH";
  m_gl_token_names[GL_POINT_SIZE] = "GL_POINT_SIZE";
  m_gl_token_names[GL_POINT_SIZE_RANGE] = "GL_POINT_SIZE_RANGE";
  m_gl_token_names[GL_POINT_SIZE_GRANULARITY] = "GL_POINT_SIZE_GRANULARITY";
  m_gl_token_names[GL_LINE_SMOOTH] = "GL_LINE_SMOOTH";
  m_gl_token_names[GL_LINE_WIDTH] = "GL_LINE_WIDTH";
  m_gl_token_names[GL_LINE_WIDTH_RANGE] = "GL_LINE_WIDTH_RANGE";
  m_gl_token_names[GL_LINE_WIDTH_GRANULARITY] = "GL_LINE_WIDTH_GRANULARITY";
  m_gl_token_names[GL_LINE_STIPPLE] = "GL_LINE_STIPPLE";
  m_gl_token_names[GL_LINE_STIPPLE_PATTERN] = "GL_LINE_STIPPLE_PATTERN";
  m_gl_token_names[GL_LINE_STIPPLE_REPEAT] = "GL_LINE_STIPPLE_REPEAT";
  m_gl_token_names[GL_LIST_MODE] = "GL_LIST_MODE";
  m_gl_token_names[GL_MAX_LIST_NESTING] = "GL_MAX_LIST_NESTING";
  m_gl_token_names[GL_LIST_BASE] = "GL_LIST_BASE";
  m_gl_token_names[GL_LIST_INDEX] = "GL_LIST_INDEX";
  m_gl_token_names[GL_POLYGON_MODE] = "GL_POLYGON_MODE";
  m_gl_token_names[GL_POLYGON_SMOOTH] = "GL_POLYGON_SMOOTH";
  m_gl_token_names[GL_POLYGON_STIPPLE] = "GL_POLYGON_STIPPLE";
  m_gl_token_names[GL_EDGE_FLAG] = "GL_EDGE_FLAG";
  m_gl_token_names[GL_CULL_FACE] = "GL_CULL_FACE";
  m_gl_token_names[GL_CULL_FACE_MODE] = "GL_CULL_FACE_MODE";
  m_gl_token_names[GL_FRONT_FACE] = "GL_FRONT_FACE";
  m_gl_token_names[GL_LIGHTING] = "GL_LIGHTING";
  m_gl_token_names[GL_LIGHT_MODEL_LOCAL_VIEWER] =
    "GL_LIGHT_MODEL_LOCAL_VIEWER";
  m_gl_token_names[GL_LIGHT_MODEL_TWO_SIDE] = "GL_LIGHT_MODEL_TWO_SIDE";
  m_gl_token_names[GL_LIGHT_MODEL_AMBIENT] = "GL_LIGHT_MODEL_AMBIENT";
  m_gl_token_names[GL_SHADE_MODEL] = "GL_SHADE_MODEL";
  m_gl_token_names[GL_COLOR_MATERIAL_FACE] = "GL_COLOR_MATERIAL_FACE";
  m_gl_token_names[GL_COLOR_MATERIAL_PARAMETER] =
    "GL_COLOR_MATERIAL_PARAMETER";
  m_gl_token_names[GL_COLOR_MATERIAL] = "GL_COLOR_MATERIAL";
  m_gl_token_names[GL_FOG] = "GL_FOG";
  m_gl_token_names[GL_FOG_INDEX] = "GL_FOG_INDEX";
  m_gl_token_names[GL_FOG_DENSITY] = "GL_FOG_DENSITY";
  m_gl_token_names[GL_FOG_START] = "GL_FOG_START";
  m_gl_token_names[GL_FOG_END] = "GL_FOG_END";
  m_gl_token_names[GL_FOG_MODE] = "GL_FOG_MODE";
  m_gl_token_names[GL_FOG_COLOR] = "GL_FOG_COLOR";
  m_gl_token_names[GL_DEPTH_RANGE] = "GL_DEPTH_RANGE";
  m_gl_token_names[GL_DEPTH_TEST] = "GL_DEPTH_TEST";
  m_gl_token_names[GL_DEPTH_WRITEMASK] = "GL_DEPTH_WRITEMASK";
  m_gl_token_names[GL_DEPTH_CLEAR_VALUE] = "GL_DEPTH_CLEAR_VALUE";
  m_gl_token_names[GL_DEPTH_FUNC] = "GL_DEPTH_FUNC";
  m_gl_token_names[GL_ACCUM_CLEAR_VALUE] = "GL_ACCUM_CLEAR_VALUE";
  m_gl_token_names[GL_STENCIL_TEST] = "GL_STENCIL_TEST";
  m_gl_token_names[GL_STENCIL_CLEAR_VALUE] = "GL_STENCIL_CLEAR_VALUE";
  m_gl_token_names[GL_STENCIL_FUNC] = "GL_STENCIL_FUNC";
  m_gl_token_names[GL_STENCIL_VALUE_MASK] = "GL_STENCIL_VALUE_MASK";
  m_gl_token_names[GL_STENCIL_FAIL] = "GL_STENCIL_FAIL";
  m_gl_token_names[GL_STENCIL_PASS_DEPTH_FAIL] = "GL_STENCIL_PASS_DEPTH_FAIL";
  m_gl_token_names[GL_STENCIL_PASS_DEPTH_PASS] = "GL_STENCIL_PASS_DEPTH_PASS";
  m_gl_token_names[GL_STENCIL_REF] = "GL_STENCIL_REF";
  m_gl_token_names[GL_STENCIL_WRITEMASK] = "GL_STENCIL_WRITEMASK";
  m_gl_token_names[GL_MATRIX_MODE] = "GL_MATRIX_MODE";
  m_gl_token_names[GL_NORMALIZE] = "GL_NORMALIZE";
  m_gl_token_names[GL_VIEWPORT] = "GL_VIEWPORT";
  m_gl_token_names[GL_MODELVIEW_STACK_DEPTH] = "GL_MODELVIEW_STACK_DEPTH";
  m_gl_token_names[GL_PROJECTION_STACK_DEPTH] = "GL_PROJECTION_STACK_DEPTH";
  m_gl_token_names[GL_TEXTURE_STACK_DEPTH] = "GL_TEXTURE_STACK_DEPTH";
  m_gl_token_names[GL_MODELVIEW_MATRIX] = "GL_MODELVIEW_MATRIX";
  m_gl_token_names[GL_PROJECTION_MATRIX] = "GL_PROJECTION_MATRIX";
  m_gl_token_names[GL_TEXTURE_MATRIX] = "GL_TEXTURE_MATRIX";
  m_gl_token_names[GL_ATTRIB_STACK_DEPTH] = "GL_ATTRIB_STACK_DEPTH";
  m_gl_token_names[GL_CLIENT_ATTRIB_STACK_DEPTH] =
    "GL_CLIENT_ATTRIB_STACK_DEPTH";
  m_gl_token_names[GL_ALPHA_TEST] = "GL_ALPHA_TEST";
  m_gl_token_names[GL_ALPHA_TEST_FUNC] = "GL_ALPHA_TEST_FUNC";
  m_gl_token_names[GL_ALPHA_TEST_REF] = "GL_ALPHA_TEST_REF";
  m_gl_token_names[GL_DITHER] = "GL_DITHER";
  m_gl_token_names[GL_BLEND_DST] = "GL_BLEND_DST";
  m_gl_token_names[GL_BLEND_SRC] = "GL_BLEND_SRC";
  m_gl_token_names[GL_BLEND] = "GL_BLEND";
  m_gl_token_names[GL_LOGIC_OP_MODE] = "GL_LOGIC_OP_MODE";
  m_gl_token_names[GL_INDEX_LOGIC_OP] = "GL_INDEX_LOGIC_OP";
  m_gl_token_names[GL_COLOR_LOGIC_OP] = "GL_COLOR_LOGIC_OP";
  m_gl_token_names[GL_AUX_BUFFERS] = "GL_AUX_BUFFERS";
  m_gl_token_names[GL_DRAW_BUFFER] = "GL_DRAW_BUFFER";
  m_gl_token_names[GL_READ_BUFFER] = "GL_READ_BUFFER";
  m_gl_token_names[GL_SCISSOR_BOX] = "GL_SCISSOR_BOX";
  m_gl_token_names[GL_SCISSOR_TEST] = "GL_SCISSOR_TEST";
  m_gl_token_names[GL_INDEX_CLEAR_VALUE] = "GL_INDEX_CLEAR_VALUE";
  m_gl_token_names[GL_INDEX_WRITEMASK] = "GL_INDEX_WRITEMASK";
  m_gl_token_names[GL_COLOR_CLEAR_VALUE] = "GL_COLOR_CLEAR_VALUE";
  m_gl_token_names[GL_COLOR_WRITEMASK] = "GL_COLOR_WRITEMASK";
  m_gl_token_names[GL_INDEX_MODE] = "GL_INDEX_MODE";
  m_gl_token_names[GL_RGBA_MODE] = "GL_RGBA_MODE";
  m_gl_token_names[GL_DOUBLEBUFFER] = "GL_DOUBLEBUFFER";
  m_gl_token_names[GL_STEREO] = "GL_STEREO";
  m_gl_token_names[GL_RENDER_MODE] = "GL_RENDER_MODE";
  m_gl_token_names[GL_PERSPECTIVE_CORRECTION_HINT] =
    "GL_PERSPECTIVE_CORRECTION_HINT";
  m_gl_token_names[GL_POINT_SMOOTH_HINT] = "GL_POINT_SMOOTH_HINT";
  m_gl_token_names[GL_LINE_SMOOTH_HINT] = "GL_LINE_SMOOTH_HINT";
  m_gl_token_names[GL_POLYGON_SMOOTH_HINT] = "GL_POLYGON_SMOOTH_HINT";
  m_gl_token_names[GL_FOG_HINT] = "GL_FOG_HINT";
  m_gl_token_names[GL_TEXTURE_GEN_S] = "GL_TEXTURE_GEN_S";
  m_gl_token_names[GL_TEXTURE_GEN_T] = "GL_TEXTURE_GEN_T";
  m_gl_token_names[GL_TEXTURE_GEN_R] = "GL_TEXTURE_GEN_R";
  m_gl_token_names[GL_TEXTURE_GEN_Q] = "GL_TEXTURE_GEN_Q";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_I] = "GL_PIXEL_MAP_I_TO_I";
  m_gl_token_names[GL_PIXEL_MAP_S_TO_S] = "GL_PIXEL_MAP_S_TO_S";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_R] = "GL_PIXEL_MAP_I_TO_R";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_G] = "GL_PIXEL_MAP_I_TO_G";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_B] = "GL_PIXEL_MAP_I_TO_B";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_A] = "GL_PIXEL_MAP_I_TO_A";
  m_gl_token_names[GL_PIXEL_MAP_R_TO_R] = "GL_PIXEL_MAP_R_TO_R";
  m_gl_token_names[GL_PIXEL_MAP_G_TO_G] = "GL_PIXEL_MAP_G_TO_G";
  m_gl_token_names[GL_PIXEL_MAP_B_TO_B] = "GL_PIXEL_MAP_B_TO_B";
  m_gl_token_names[GL_PIXEL_MAP_A_TO_A] = "GL_PIXEL_MAP_A_TO_A";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_I_SIZE] = "GL_PIXEL_MAP_I_TO_I_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_S_TO_S_SIZE] = "GL_PIXEL_MAP_S_TO_S_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_R_SIZE] = "GL_PIXEL_MAP_I_TO_R_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_G_SIZE] = "GL_PIXEL_MAP_I_TO_G_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_B_SIZE] = "GL_PIXEL_MAP_I_TO_B_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_I_TO_A_SIZE] = "GL_PIXEL_MAP_I_TO_A_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_R_TO_R_SIZE] = "GL_PIXEL_MAP_R_TO_R_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_G_TO_G_SIZE] = "GL_PIXEL_MAP_G_TO_G_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_B_TO_B_SIZE] = "GL_PIXEL_MAP_B_TO_B_SIZE";
  m_gl_token_names[GL_PIXEL_MAP_A_TO_A_SIZE] = "GL_PIXEL_MAP_A_TO_A_SIZE";
  m_gl_token_names[GL_UNPACK_SWAP_BYTES] = "GL_UNPACK_SWAP_BYTES";
  m_gl_token_names[GL_UNPACK_LSB_FIRST] = "GL_UNPACK_LSB_FIRST";
  m_gl_token_names[GL_UNPACK_ROW_LENGTH] = "GL_UNPACK_ROW_LENGTH";
  m_gl_token_names[GL_UNPACK_SKIP_ROWS] = "GL_UNPACK_SKIP_ROWS";
  m_gl_token_names[GL_UNPACK_SKIP_PIXELS] = "GL_UNPACK_SKIP_PIXELS";
  m_gl_token_names[GL_UNPACK_ALIGNMENT] = "GL_UNPACK_ALIGNMENT";
  m_gl_token_names[GL_PACK_SWAP_BYTES] = "GL_PACK_SWAP_BYTES";
  m_gl_token_names[GL_PACK_LSB_FIRST] = "GL_PACK_LSB_FIRST";
  m_gl_token_names[GL_PACK_ROW_LENGTH] = "GL_PACK_ROW_LENGTH";
  m_gl_token_names[GL_PACK_SKIP_ROWS] = "GL_PACK_SKIP_ROWS";
  m_gl_token_names[GL_PACK_SKIP_PIXELS] = "GL_PACK_SKIP_PIXELS";
  m_gl_token_names[GL_PACK_ALIGNMENT] = "GL_PACK_ALIGNMENT";
  m_gl_token_names[GL_MAP_COLOR] = "GL_MAP_COLOR";
  m_gl_token_names[GL_MAP_STENCIL] = "GL_MAP_STENCIL";
  m_gl_token_names[GL_INDEX_SHIFT] = "GL_INDEX_SHIFT";
  m_gl_token_names[GL_INDEX_OFFSET] = "GL_INDEX_OFFSET";
  m_gl_token_names[GL_RED_SCALE] = "GL_RED_SCALE";
  m_gl_token_names[GL_RED_BIAS] = "GL_RED_BIAS";
  m_gl_token_names[GL_ZOOM_X] = "GL_ZOOM_X";
  m_gl_token_names[GL_ZOOM_Y] = "GL_ZOOM_Y";
  m_gl_token_names[GL_GREEN_SCALE] = "GL_GREEN_SCALE";
  m_gl_token_names[GL_GREEN_BIAS] = "GL_GREEN_BIAS";
  m_gl_token_names[GL_BLUE_SCALE] = "GL_BLUE_SCALE";
  m_gl_token_names[GL_BLUE_BIAS] = "GL_BLUE_BIAS";
  m_gl_token_names[GL_ALPHA_SCALE] = "GL_ALPHA_SCALE";
  m_gl_token_names[GL_ALPHA_BIAS] = "GL_ALPHA_BIAS";
  m_gl_token_names[GL_DEPTH_SCALE] = "GL_DEPTH_SCALE";
  m_gl_token_names[GL_DEPTH_BIAS] = "GL_DEPTH_BIAS";
  m_gl_token_names[GL_MAX_EVAL_ORDER] = "GL_MAX_EVAL_ORDER";
  m_gl_token_names[GL_MAX_LIGHTS] = "GL_MAX_LIGHTS";
  m_gl_token_names[GL_MAX_CLIP_PLANES] = "GL_MAX_CLIP_PLANES";
  m_gl_token_names[GL_MAX_TEXTURE_SIZE] = "GL_MAX_TEXTURE_SIZE";
  m_gl_token_names[GL_MAX_PIXEL_MAP_TABLE] = "GL_MAX_PIXEL_MAP_TABLE";
  m_gl_token_names[GL_MAX_ATTRIB_STACK_DEPTH] = "GL_MAX_ATTRIB_STACK_DEPTH";
  m_gl_token_names[GL_MAX_MODELVIEW_STACK_DEPTH] =
    "GL_MAX_MODELVIEW_STACK_DEPTH";
  m_gl_token_names[GL_MAX_NAME_STACK_DEPTH] = "GL_MAX_NAME_STACK_DEPTH";
  m_gl_token_names[GL_MAX_PROJECTION_STACK_DEPTH] =
    "GL_MAX_PROJECTION_STACK_DEPTH";
  m_gl_token_names[GL_MAX_TEXTURE_STACK_DEPTH] = "GL_MAX_TEXTURE_STACK_DEPTH";
  m_gl_token_names[GL_MAX_VIEWPORT_DIMS] = "GL_MAX_VIEWPORT_DIMS";
  m_gl_token_names[GL_MAX_CLIENT_ATTRIB_STACK_DEPTH] =
    "GL_MAX_CLIENT_ATTRIB_STACK_DEPTH";
  m_gl_token_names[GL_SUBPIXEL_BITS] = "GL_SUBPIXEL_BITS";
  m_gl_token_names[GL_INDEX_BITS] = "GL_INDEX_BITS";
  m_gl_token_names[GL_RED_BITS] = "GL_RED_BITS";
  m_gl_token_names[GL_GREEN_BITS] = "GL_GREEN_BITS";
  m_gl_token_names[GL_BLUE_BITS] = "GL_BLUE_BITS";
  m_gl_token_names[GL_ALPHA_BITS] = "GL_ALPHA_BITS";
  m_gl_token_names[GL_DEPTH_BITS] = "GL_DEPTH_BITS";
  m_gl_token_names[GL_STENCIL_BITS] = "GL_STENCIL_BITS";
  m_gl_token_names[GL_ACCUM_RED_BITS] = "GL_ACCUM_RED_BITS";
  m_gl_token_names[GL_ACCUM_GREEN_BITS] = "GL_ACCUM_GREEN_BITS";
  m_gl_token_names[GL_ACCUM_BLUE_BITS] = "GL_ACCUM_BLUE_BITS";
  m_gl_token_names[GL_ACCUM_ALPHA_BITS] = "GL_ACCUM_ALPHA_BITS";
  m_gl_token_names[GL_SAMPLES] = "GL_SAMPLES";
  m_gl_token_names[GL_NAME_STACK_DEPTH] = "GL_NAME_STACK_DEPTH";
  m_gl_token_names[GL_AUTO_NORMAL] = "GL_AUTO_NORMAL";
  m_gl_token_names[GL_MAP1_COLOR_4] = "GL_MAP1_COLOR_4";
  m_gl_token_names[GL_MAP1_INDEX] = "GL_MAP1_INDEX";
  m_gl_token_names[GL_MAP1_NORMAL] = "GL_MAP1_NORMAL";
  m_gl_token_names[GL_MAP1_TEXTURE_COORD_1] = "GL_MAP1_TEXTURE_COORD_1";
  m_gl_token_names[GL_MAP1_TEXTURE_COORD_2] = "GL_MAP1_TEXTURE_COORD_2";
  m_gl_token_names[GL_MAP1_TEXTURE_COORD_3] = "GL_MAP1_TEXTURE_COORD_3";
  m_gl_token_names[GL_MAP1_TEXTURE_COORD_4] = "GL_MAP1_TEXTURE_COORD_4";
  m_gl_token_names[GL_MAP1_VERTEX_3] = "GL_MAP1_VERTEX_3";
  m_gl_token_names[GL_MAP1_VERTEX_4] = "GL_MAP1_VERTEX_4";
  m_gl_token_names[GL_MAP2_COLOR_4] = "GL_MAP2_COLOR_4";
  m_gl_token_names[GL_MAP2_INDEX] = "GL_MAP2_INDEX";
  m_gl_token_names[GL_MAP2_NORMAL] = "GL_MAP2_NORMAL";
  m_gl_token_names[GL_MAP2_TEXTURE_COORD_1] = "GL_MAP2_TEXTURE_COORD_1";
  m_gl_token_names[GL_MAP2_TEXTURE_COORD_2] = "GL_MAP2_TEXTURE_COORD_2";
  m_gl_token_names[GL_MAP2_TEXTURE_COORD_3] = "GL_MAP2_TEXTURE_COORD_3";
  m_gl_token_names[GL_MAP2_TEXTURE_COORD_4] = "GL_MAP2_TEXTURE_COORD_4";
  m_gl_token_names[GL_MAP2_VERTEX_3] = "GL_MAP2_VERTEX_3";
  m_gl_token_names[GL_MAP2_VERTEX_4] = "GL_MAP2_VERTEX_4";
  m_gl_token_names[GL_MAP1_GRID_DOMAIN] = "GL_MAP1_GRID_DOMAIN";
  m_gl_token_names[GL_MAP1_GRID_SEGMENTS] = "GL_MAP1_GRID_SEGMENTS";
  m_gl_token_names[GL_MAP2_GRID_DOMAIN] = "GL_MAP2_GRID_DOMAIN";
  m_gl_token_names[GL_MAP2_GRID_SEGMENTS] = "GL_MAP2_GRID_SEGMENTS";
  m_gl_token_names[GL_TEXTURE_1D] = "GL_TEXTURE_1D";
  m_gl_token_names[GL_TEXTURE_2D] = "GL_TEXTURE_2D";
  m_gl_token_names[GL_FEEDBACK_BUFFER_POINTER] = "GL_FEEDBACK_BUFFER_POINTER";
  m_gl_token_names[GL_FEEDBACK_BUFFER_SIZE] = "GL_FEEDBACK_BUFFER_SIZE";
  m_gl_token_names[GL_FEEDBACK_BUFFER_TYPE] = "GL_FEEDBACK_BUFFER_TYPE";
  m_gl_token_names[GL_SELECTION_BUFFER_POINTER] =
    "GL_SELECTION_BUFFER_POINTER";
  m_gl_token_names[GL_SELECTION_BUFFER_SIZE] = "GL_SELECTION_BUFFER_SIZE";
  /*      GL_TEXTURE_BINDING_1D */
  /*      GL_TEXTURE_BINDING_2D */
  /*      GL_VERTEX_ARRAY */
  /*      GL_NORMAL_ARRAY */
  /*      GL_COLOR_ARRAY */
  /*      GL_INDEX_ARRAY */
  /*      GL_TEXTURE_COORD_ARRAY */
  /*      GL_EDGE_FLAG_ARRAY */
  /*      GL_VERTEX_ARRAY_SIZE */
  /*      GL_VERTEX_ARRAY_TYPE */
  /*      GL_VERTEX_ARRAY_STRIDE */
  /*      GL_NORMAL_ARRAY_TYPE */
  /*      GL_NORMAL_ARRAY_STRIDE */
  /*      GL_COLOR_ARRAY_SIZE */
  /*      GL_COLOR_ARRAY_TYPE */
  /*      GL_COLOR_ARRAY_STRIDE */
  /*      GL_INDEX_ARRAY_TYPE */
  /*      GL_INDEX_ARRAY_STRIDE */
  /*      GL_TEXTURE_COORD_ARRAY_SIZE */
  /*      GL_TEXTURE_COORD_ARRAY_TYPE */
  /*      GL_TEXTURE_COORD_ARRAY_STRIDE */
  /*      GL_EDGE_FLAG_ARRAY_STRIDE */
  /*      GL_POLYGON_OFFSET_FACTOR */
  /*      GL_POLYGON_OFFSET_UNITS */

  /* GetTextureParameter */
  /*      GL_TEXTURE_MAG_FILTER */
  /*      GL_TEXTURE_MIN_FILTER */
  /*      GL_TEXTURE_WRAP_S */
  /*      GL_TEXTURE_WRAP_T */
  m_gl_token_names[GL_TEXTURE_WIDTH] = "GL_TEXTURE_WIDTH";
  m_gl_token_names[GL_TEXTURE_HEIGHT] = "GL_TEXTURE_HEIGHT";
  m_gl_token_names[GL_TEXTURE_INTERNAL_FORMAT] = "GL_TEXTURE_INTERNAL_FORMAT";
  m_gl_token_names[GL_TEXTURE_BORDER_COLOR] = "GL_TEXTURE_BORDER_COLOR";
  m_gl_token_names[GL_TEXTURE_BORDER] = "GL_TEXTURE_BORDER";
  /*      GL_TEXTURE_RED_SIZE */
  /*      GL_TEXTURE_GREEN_SIZE */
  /*      GL_TEXTURE_BLUE_SIZE */
  /*      GL_TEXTURE_ALPHA_SIZE */
  /*      GL_TEXTURE_LUMINANCE_SIZE */
  /*      GL_TEXTURE_INTENSITY_SIZE */
  /*      GL_TEXTURE_PRIORITY */
  /*      GL_TEXTURE_RESIDENT */

  /* HintMode */
  m_gl_token_names[GL_DONT_CARE] = "GL_DONT_CARE";
  m_gl_token_names[GL_FASTEST] = "GL_FASTEST";
  m_gl_token_names[GL_NICEST] = "GL_NICEST";

  /* HintTarget */
  /*      GL_PERSPECTIVE_CORRECTION_HINT */
  /*      GL_POINT_SMOOTH_HINT */
  /*      GL_LINE_SMOOTH_HINT */
  /*      GL_POLYGON_SMOOTH_HINT */
  /*      GL_FOG_HINT */
  /*      GL_PHONG_HINT */

  /* IndexPointerType */
  /*      GL_SHORT */
  /*      GL_INT */
  /*      GL_FLOAT */
  /*      GL_DOUBLE */

  /* LightModelParameter */
  /*      GL_LIGHT_MODEL_AMBIENT */
  /*      GL_LIGHT_MODEL_LOCAL_VIEWER */
  /*      GL_LIGHT_MODEL_TWO_SIDE */

  /* LightName */
  m_gl_token_names[GL_LIGHT0] = "GL_LIGHT0";
  m_gl_token_names[GL_LIGHT1] = "GL_LIGHT1";
  m_gl_token_names[GL_LIGHT2] = "GL_LIGHT2";
  m_gl_token_names[GL_LIGHT3] = "GL_LIGHT3";
  m_gl_token_names[GL_LIGHT4] = "GL_LIGHT4";
  m_gl_token_names[GL_LIGHT5] = "GL_LIGHT5";
  m_gl_token_names[GL_LIGHT6] = "GL_LIGHT6";
  m_gl_token_names[GL_LIGHT7] = "GL_LIGHT7";

  /* LightParameter */
  m_gl_token_names[GL_AMBIENT] = "GL_AMBIENT";
  m_gl_token_names[GL_DIFFUSE] = "GL_DIFFUSE";
  m_gl_token_names[GL_SPECULAR] = "GL_SPECULAR";
  m_gl_token_names[GL_POSITION] = "GL_POSITION";
  m_gl_token_names[GL_SPOT_DIRECTION] = "GL_SPOT_DIRECTION";
  m_gl_token_names[GL_SPOT_EXPONENT] = "GL_SPOT_EXPONENT";
  m_gl_token_names[GL_SPOT_CUTOFF] = "GL_SPOT_CUTOFF";
  m_gl_token_names[GL_CONSTANT_ATTENUATION] = "GL_CONSTANT_ATTENUATION";
  m_gl_token_names[GL_LINEAR_ATTENUATION] = "GL_LINEAR_ATTENUATION";
  m_gl_token_names[GL_QUADRATIC_ATTENUATION] = "GL_QUADRATIC_ATTENUATION";

  /* InterleavedArrays */
  /*      GL_V2F */
  /*      GL_V3F */
  /*      GL_C4UB_V2F */
  /*      GL_C4UB_V3F */
  /*      GL_C3F_V3F */
  /*      GL_N3F_V3F */
  /*      GL_C4F_N3F_V3F */
  /*      GL_T2F_V3F */
  /*      GL_T4F_V4F */
  /*      GL_T2F_C4UB_V3F */
  /*      GL_T2F_C3F_V3F */
  /*      GL_T2F_N3F_V3F */
  /*      GL_T2F_C4F_N3F_V3F */
  /*      GL_T4F_C4F_N3F_V4F */

  /* ListMode */
  m_gl_token_names[GL_COMPILE] = "GL_COMPILE";
  m_gl_token_names[GL_COMPILE_AND_EXECUTE] = "GL_COMPILE_AND_EXECUTE";

  /* ListNameType */
  /*      GL_BYTE */
  /*      GL_UNSIGNED_BYTE */
  /*      GL_SHORT */
  /*      GL_UNSIGNED_SHORT */
  /*      GL_INT */
  /*      GL_UNSIGNED_INT */
  /*      GL_FLOAT */
  /*      GL_2_BYTES */
  /*      GL_3_BYTES */
  /*      GL_4_BYTES */

  /* LogicOp */
  m_gl_token_names[GL_CLEAR] = "GL_CLEAR";
  m_gl_token_names[GL_AND] = "GL_AND";
  m_gl_token_names[GL_AND_REVERSE] = "GL_AND_REVERSE";
  m_gl_token_names[GL_COPY] = "GL_COPY";
  m_gl_token_names[GL_AND_INVERTED] = "GL_AND_INVERTED";
  m_gl_token_names[GL_NOOP] = "GL_NOOP";
  m_gl_token_names[GL_XOR] = "GL_XOR";
  m_gl_token_names[GL_OR] = "GL_OR";
  m_gl_token_names[GL_NOR] = "GL_NOR";
  m_gl_token_names[GL_EQUIV] = "GL_EQUIV";
  m_gl_token_names[GL_INVERT] = "GL_INVERT";
  m_gl_token_names[GL_OR_REVERSE] = "GL_OR_REVERSE";
  m_gl_token_names[GL_COPY_INVERTED] = "GL_COPY_INVERTED";
  m_gl_token_names[GL_OR_INVERTED] = "GL_OR_INVERTED";
  m_gl_token_names[GL_NAND] = "GL_NAND";
  m_gl_token_names[GL_SET] = "GL_SET";

  /* MapTarget */
  /*      GL_MAP1_COLOR_4 */
  /*      GL_MAP1_INDEX */
  /*      GL_MAP1_NORMAL */
  /*      GL_MAP1_TEXTURE_COORD_1 */
  /*      GL_MAP1_TEXTURE_COORD_2 */
  /*      GL_MAP1_TEXTURE_COORD_3 */
  /*      GL_MAP1_TEXTURE_COORD_4 */
  /*      GL_MAP1_VERTEX_3 */
  /*      GL_MAP1_VERTEX_4 */
  /*      GL_MAP2_COLOR_4 */
  /*      GL_MAP2_INDEX */
  /*      GL_MAP2_NORMAL */
  /*      GL_MAP2_TEXTURE_COORD_1 */
  /*      GL_MAP2_TEXTURE_COORD_2 */
  /*      GL_MAP2_TEXTURE_COORD_3 */
  /*      GL_MAP2_TEXTURE_COORD_4 */
  /*      GL_MAP2_VERTEX_3 */
  /*      GL_MAP2_VERTEX_4 */

  /* MaterialFace */
  /*      GL_FRONT */
  /*      GL_BACK */
  /*      GL_FRONT_AND_BACK */

  /* MaterialParameter */
  m_gl_token_names[GL_EMISSION] = "GL_EMISSION";
  m_gl_token_names[GL_SHININESS] = "GL_SHININESS";
  m_gl_token_names[GL_AMBIENT_AND_DIFFUSE] = "GL_AMBIENT_AND_DIFFUSE";
  m_gl_token_names[GL_COLOR_INDEXES] = "GL_COLOR_INDEXES";
  /*      GL_AMBIENT */
  /*      GL_DIFFUSE */
  /*      GL_SPECULAR */

  /* MatrixMode */
  m_gl_token_names[GL_MODELVIEW] = "GL_MODELVIEW";
  m_gl_token_names[GL_PROJECTION] = "GL_PROJECTION";
  m_gl_token_names[GL_TEXTURE] = "GL_TEXTURE";

  /* MeshMode1 */
  /*      GL_POINT */
  /*      GL_LINE */

  /* MeshMode2 */
  /*      GL_POINT */
  /*      GL_LINE */
  /*      GL_FILL */

  /* NormalPointerType */
  /*      GL_BYTE */
  /*      GL_SHORT */
  /*      GL_INT */
  /*      GL_FLOAT */
  /*      GL_DOUBLE */

  /* PixelCopyType */
  m_gl_token_names[GL_COLOR] = "GL_COLOR";
  m_gl_token_names[GL_DEPTH] = "GL_DEPTH";
  m_gl_token_names[GL_STENCIL] = "GL_STENCIL";

  /* PixelFormat */
  m_gl_token_names[GL_COLOR_INDEX] = "GL_COLOR_INDEX";
  m_gl_token_names[GL_STENCIL_INDEX] = "GL_STENCIL_INDEX";
  m_gl_token_names[GL_DEPTH_COMPONENT] = "GL_DEPTH_COMPONENT";
  m_gl_token_names[GL_RED] = "GL_RED";
  m_gl_token_names[GL_GREEN] = "GL_GREEN";
  m_gl_token_names[GL_BLUE] = "GL_BLUE";
  m_gl_token_names[GL_ALPHA] = "GL_ALPHA";
  m_gl_token_names[GL_RGB] = "GL_RGB";
  m_gl_token_names[GL_RGBA] = "GL_RGBA";
  m_gl_token_names[GL_LUMINANCE] = "GL_LUMINANCE";
  m_gl_token_names[GL_LUMINANCE_ALPHA] = "GL_LUMINANCE_ALPHA";

  /* PixelMap */
  /*      GL_PIXEL_MAP_I_TO_I */
  /*      GL_PIXEL_MAP_S_TO_S */
  /*      GL_PIXEL_MAP_I_TO_R */
  /*      GL_PIXEL_MAP_I_TO_G */
  /*      GL_PIXEL_MAP_I_TO_B */
  /*      GL_PIXEL_MAP_I_TO_A */
  /*      GL_PIXEL_MAP_R_TO_R */
  /*      GL_PIXEL_MAP_G_TO_G */
  /*      GL_PIXEL_MAP_B_TO_B */
  /*      GL_PIXEL_MAP_A_TO_A */

  /* PixelStore */
  /*      GL_UNPACK_SWAP_BYTES */
  /*      GL_UNPACK_LSB_FIRST */
  /*      GL_UNPACK_ROW_LENGTH */
  /*      GL_UNPACK_SKIP_ROWS */
  /*      GL_UNPACK_SKIP_PIXELS */
  /*      GL_UNPACK_ALIGNMENT */
  /*      GL_PACK_SWAP_BYTES */
  /*      GL_PACK_LSB_FIRST */
  /*      GL_PACK_ROW_LENGTH */
  /*      GL_PACK_SKIP_ROWS */
  /*      GL_PACK_SKIP_PIXELS */
  /*      GL_PACK_ALIGNMENT */

  /* PixelTransfer */
  /*      GL_MAP_COLOR */
  /*      GL_MAP_STENCIL */
  /*      GL_INDEX_SHIFT */
  /*      GL_INDEX_OFFSET */
  /*      GL_RED_SCALE */
  /*      GL_RED_BIAS */
  /*      GL_GREEN_SCALE */
  /*      GL_GREEN_BIAS */
  /*      GL_BLUE_SCALE */
  /*      GL_BLUE_BIAS */
  /*      GL_ALPHA_SCALE */
  /*      GL_ALPHA_BIAS */
  /*      GL_DEPTH_SCALE */
  /*      GL_DEPTH_BIAS */

  /* PixelType */
  m_gl_token_names[GL_BITMAP] = "GL_BITMAP";
  /*      GL_BYTE */
  /*      GL_UNSIGNED_BYTE */
  /*      GL_SHORT */
  /*      GL_UNSIGNED_SHORT */
  /*      GL_INT */
  /*      GL_UNSIGNED_INT */
  /*      GL_FLOAT */

  /* PolygonMode */
  m_gl_token_names[GL_POINT] = "GL_POINT";
  m_gl_token_names[GL_LINE] = "GL_LINE";
  m_gl_token_names[GL_FILL] = "GL_FILL";

  /* ReadBufferMode */
  /*      GL_FRONT_LEFT */
  /*      GL_FRONT_RIGHT */
  /*      GL_BACK_LEFT */
  /*      GL_BACK_RIGHT */
  /*      GL_FRONT */
  /*      GL_BACK */
  /*      GL_LEFT */
  /*      GL_RIGHT */
  /*      GL_AUX0 */
  /*      GL_AUX1 */
  /*      GL_AUX2 */
  /*      GL_AUX3 */

  /* RenderingMode */
  m_gl_token_names[GL_RENDER] = "GL_RENDER";
  m_gl_token_names[GL_FEEDBACK] = "GL_FEEDBACK";
  m_gl_token_names[GL_SELECT] = "GL_SELECT";

  /* ShadingModel */
  m_gl_token_names[GL_FLAT] = "GL_FLAT";
  m_gl_token_names[GL_SMOOTH] = "GL_SMOOTH";


  /* StencilFunction */
  /*      GL_NEVER */
  /*      GL_LESS */
  /*      GL_EQUAL */
  /*      GL_LEQUAL */
  /*      GL_GREATER */
  /*      GL_NOTEQUAL */
  /*      GL_GEQUAL */
  /*      GL_ALWAYS */

  /* StencilOp */
  /*      GL_ZERO */
  m_gl_token_names[GL_KEEP] = "GL_KEEP";
  m_gl_token_names[GL_REPLACE] = "GL_REPLACE";
  m_gl_token_names[GL_INCR] = "GL_INCR";
  m_gl_token_names[GL_DECR] = "GL_DECR";
  /*      GL_INVERT */

  /* StringName */
  m_gl_token_names[GL_VENDOR] = "GL_VENDOR";
  m_gl_token_names[GL_RENDERER] = "GL_RENDERER";
  m_gl_token_names[GL_VERSION] = "GL_VERSION";
  m_gl_token_names[GL_EXTENSIONS] = "GL_EXTENSIONS";

  /* TextureCoordName */
  m_gl_token_names[GL_S] = "GL_S";
  m_gl_token_names[GL_T] = "GL_T";
  m_gl_token_names[GL_R] = "GL_R";
  m_gl_token_names[GL_Q] = "GL_Q";

  /* TexCoordPointerType */
  /*      GL_SHORT */
  /*      GL_INT */
  /*      GL_FLOAT */
  /*      GL_DOUBLE */

  /* TextureEnvMode */
  m_gl_token_names[GL_MODULATE] = "GL_MODULATE";
  m_gl_token_names[GL_DECAL] = "GL_DECAL";
  /*      GL_BLEND */
  /*      GL_REPLACE */

  /* TextureEnvParameter */
  m_gl_token_names[GL_TEXTURE_ENV_MODE] = "GL_TEXTURE_ENV_MODE";
  m_gl_token_names[GL_TEXTURE_ENV_COLOR] = "GL_TEXTURE_ENV_COLOR";

  /* TextureEnvTarget */
  m_gl_token_names[GL_TEXTURE_ENV] = "GL_TEXTURE_ENV";

  /* TextureGenMode */
  m_gl_token_names[GL_EYE_LINEAR] = "GL_EYE_LINEAR";
  m_gl_token_names[GL_OBJECT_LINEAR] = "GL_OBJECT_LINEAR";
  m_gl_token_names[GL_SPHERE_MAP] = "GL_SPHERE_MAP";
  m_gl_token_names[GL_NORMAL_MAP] = "GL_NORMAL_MAP";
  m_gl_token_names[GL_REFLECTION_MAP] = "GL_REFLECTION_MAP";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP] = "GL_TEXTURE_CUBE_MAP";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP_SEAMLESS] =
    "GL_TEXTURE_CUBE_MAP_SEAMLESS";

  m_gl_token_names[GL_TEXTURE_CUBE_MAP_POSITIVE_X] =
    "GL_TEXTURE_CUBE_MAP_POSITIVE_X";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP_NEGATIVE_X] =
    "GL_TEXTURE_CUBE_MAP_NEGATIVE_X";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP_POSITIVE_Y] =
    "GL_TEXTURE_CUBE_MAP_POSITIVE_Y";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y] =
    "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP_POSITIVE_Z] =
    "GL_TEXTURE_CUBE_MAP_POSITIVE_Z";
  m_gl_token_names[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z] =
    "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z";

  /* TextureGenParameter */
  m_gl_token_names[GL_TEXTURE_GEN_MODE] = "GL_TEXTURE_GEN_MODE";
  m_gl_token_names[GL_OBJECT_PLANE] = "GL_OBJECT_PLANE";
  m_gl_token_names[GL_EYE_PLANE] = "GL_EYE_PLANE";

  /* TextureMagFilter */
  m_gl_token_names[GL_NEAREST] = "GL_NEAREST";
  m_gl_token_names[GL_LINEAR] = "GL_LINEAR";

  /* TextureMinFilter */
  /*      GL_NEAREST */
  /*      GL_LINEAR */
  m_gl_token_names[GL_NEAREST_MIPMAP_NEAREST] = "GL_NEAREST_MIPMAP_NEAREST";
  m_gl_token_names[GL_LINEAR_MIPMAP_NEAREST] = "GL_LINEAR_MIPMAP_NEAREST";
  m_gl_token_names[GL_NEAREST_MIPMAP_LINEAR] = "GL_NEAREST_MIPMAP_LINEAR";
  m_gl_token_names[GL_LINEAR_MIPMAP_LINEAR] = "GL_LINEAR_MIPMAP_LINEAR";

  /* TextureParameterName */
  m_gl_token_names[GL_TEXTURE_MAG_FILTER] = "GL_TEXTURE_MAG_FILTER";
  m_gl_token_names[GL_TEXTURE_MIN_FILTER] = "GL_TEXTURE_MIN_FILTER";
  m_gl_token_names[GL_TEXTURE_WRAP_S] = "GL_TEXTURE_WRAP_S";
  m_gl_token_names[GL_TEXTURE_WRAP_T] = "GL_TEXTURE_WRAP_T";
  /*      GL_TEXTURE_BORDER_COLOR */
  /*      GL_TEXTURE_PRIORITY */

  /* TextureTarget */
  /*      GL_TEXTURE_1D */
  /*      GL_TEXTURE_2D */
  /*      GL_PROXY_TEXTURE_1D */
  /*      GL_PROXY_TEXTURE_2D */

  /* TextureWrapMode */
  m_gl_token_names[GL_CLAMP] = "GL_CLAMP";
  m_gl_token_names[GL_REPEAT] = "GL_REPEAT";

  /* VertexPointerType */
  /*      GL_SHORT */
  /*      GL_INT */
  /*      GL_FLOAT */
  /*      GL_DOUBLE */

  /* ClientAttribMask */
  /*      GL_CLIENT_PIXEL_STORE_BIT */
  /*      GL_CLIENT_VERTEX_ARRAY_BIT */
  /*      GL_CLIENT_ALL_ATTRIB_BITS */

  /* polygon_offset */
  m_gl_token_names[GL_POLYGON_OFFSET_FACTOR] = "GL_POLYGON_OFFSET_FACTOR";
  m_gl_token_names[GL_POLYGON_OFFSET_UNITS] = "GL_POLYGON_OFFSET_UNITS";
  m_gl_token_names[GL_POLYGON_OFFSET_POINT] = "GL_POLYGON_OFFSET_POINT";
  m_gl_token_names[GL_POLYGON_OFFSET_LINE] = "GL_POLYGON_OFFSET_LINE";
  m_gl_token_names[GL_POLYGON_OFFSET_FILL] = "GL_POLYGON_OFFSET_FILL";

  /* texture */
  m_gl_token_names[GL_ALPHA4] = "GL_ALPHA4";
  m_gl_token_names[GL_ALPHA8] = "GL_ALPHA8";
  m_gl_token_names[GL_ALPHA12] = "GL_ALPHA12";
  m_gl_token_names[GL_ALPHA16] = "GL_ALPHA16";
  m_gl_token_names[GL_LUMINANCE4] = "GL_LUMINANCE4";
  m_gl_token_names[GL_LUMINANCE8] = "GL_LUMINANCE8";
  m_gl_token_names[GL_LUMINANCE12] = "GL_LUMINANCE12";
  m_gl_token_names[GL_LUMINANCE16] = "GL_LUMINANCE16";
  m_gl_token_names[GL_LUMINANCE4_ALPHA4] = "GL_LUMINANCE4_ALPHA4";
  m_gl_token_names[GL_LUMINANCE6_ALPHA2] = "GL_LUMINANCE6_ALPHA2";
  m_gl_token_names[GL_LUMINANCE8_ALPHA8] = "GL_LUMINANCE8_ALPHA8";
  m_gl_token_names[GL_LUMINANCE12_ALPHA4] = "GL_LUMINANCE12_ALPHA4";
  m_gl_token_names[GL_LUMINANCE12_ALPHA12] = "GL_LUMINANCE12_ALPHA12";
  m_gl_token_names[GL_LUMINANCE16_ALPHA16] = "GL_LUMINANCE16_ALPHA16";
  m_gl_token_names[GL_INTENSITY] = "GL_INTENSITY";
  m_gl_token_names[GL_INTENSITY4] = "GL_INTENSITY4";
  m_gl_token_names[GL_INTENSITY8] = "GL_INTENSITY8";
  m_gl_token_names[GL_INTENSITY12] = "GL_INTENSITY12";
  m_gl_token_names[GL_INTENSITY16] = "GL_INTENSITY16";
  m_gl_token_names[GL_R3_G3_B2] = "GL_R3_G3_B2";
  m_gl_token_names[GL_RGB4] = "GL_RGB4";
  m_gl_token_names[GL_RGB5] = "GL_RGB5";
  m_gl_token_names[GL_RGB8] = "GL_RGB8";
  m_gl_token_names[GL_RGB10] = "GL_RGB10";
  m_gl_token_names[GL_RGB12] = "GL_RGB12";
  m_gl_token_names[GL_RGB16] = "GL_RGB16";
  m_gl_token_names[GL_RGBA2] = "GL_RGBA2";
  m_gl_token_names[GL_RGBA4] = "GL_RGBA4";
  m_gl_token_names[GL_RGB5_A1] = "GL_RGB5_A1";
  m_gl_token_names[GL_RGBA8] = "GL_RGBA8";
  m_gl_token_names[GL_RGB10_A2] = "GL_RGB10_A2";
  m_gl_token_names[GL_RGBA12] = "GL_RGBA12";
  m_gl_token_names[GL_RGBA16] = "GL_RGBA16";
  m_gl_token_names[GL_TEXTURE_RED_SIZE] = "GL_TEXTURE_RED_SIZE";
  m_gl_token_names[GL_TEXTURE_GREEN_SIZE] = "GL_TEXTURE_GREEN_SIZE";
  m_gl_token_names[GL_TEXTURE_BLUE_SIZE] = "GL_TEXTURE_BLUE_SIZE";
  m_gl_token_names[GL_TEXTURE_ALPHA_SIZE] = "GL_TEXTURE_ALPHA_SIZE";
  m_gl_token_names[GL_TEXTURE_LUMINANCE_SIZE] = "GL_TEXTURE_LUMINANCE_SIZE";
  m_gl_token_names[GL_TEXTURE_INTENSITY_SIZE] = "GL_TEXTURE_INTENSITY_SIZE";
  m_gl_token_names[GL_PROXY_TEXTURE_1D] = "GL_PROXY_TEXTURE_1D";
  m_gl_token_names[GL_PROXY_TEXTURE_2D] = "GL_PROXY_TEXTURE_2D";

  /* texture_object */
  m_gl_token_names[GL_TEXTURE_PRIORITY] = "GL_TEXTURE_PRIORITY";
  m_gl_token_names[GL_TEXTURE_RESIDENT] = "GL_TEXTURE_RESIDENT";
  m_gl_token_names[GL_TEXTURE_BINDING_1D] = "GL_TEXTURE_BINDING_1D";
  m_gl_token_names[GL_TEXTURE_BINDING_2D] = "GL_TEXTURE_BINDING_2D";

  /* vertex_array */
  m_gl_token_names[GL_VERTEX_ARRAY] = "GL_VERTEX_ARRAY";
  m_gl_token_names[GL_NORMAL_ARRAY] = "GL_NORMAL_ARRAY";
  m_gl_token_names[GL_COLOR_ARRAY] = "GL_COLOR_ARRAY";
  m_gl_token_names[GL_INDEX_ARRAY] = "GL_INDEX_ARRAY";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY] = "GL_TEXTURE_COORD_ARRAY";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY] = "GL_EDGE_FLAG_ARRAY";
  m_gl_token_names[GL_VERTEX_ARRAY_SIZE] = "GL_VERTEX_ARRAY_SIZE";
  m_gl_token_names[GL_VERTEX_ARRAY_TYPE] = "GL_VERTEX_ARRAY_TYPE";
  m_gl_token_names[GL_VERTEX_ARRAY_STRIDE] = "GL_VERTEX_ARRAY_STRIDE";
  m_gl_token_names[GL_NORMAL_ARRAY_TYPE] = "GL_NORMAL_ARRAY_TYPE";
  m_gl_token_names[GL_NORMAL_ARRAY_STRIDE] = "GL_NORMAL_ARRAY_STRIDE";
  m_gl_token_names[GL_COLOR_ARRAY_SIZE] = "GL_COLOR_ARRAY_SIZE";
  m_gl_token_names[GL_COLOR_ARRAY_TYPE] = "GL_COLOR_ARRAY_TYPE";
  m_gl_token_names[GL_COLOR_ARRAY_STRIDE] = "GL_COLOR_ARRAY_STRIDE";
  m_gl_token_names[GL_INDEX_ARRAY_TYPE] = "GL_INDEX_ARRAY_TYPE";
  m_gl_token_names[GL_INDEX_ARRAY_STRIDE] = "GL_INDEX_ARRAY_STRIDE";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_SIZE] =
    "GL_TEXTURE_COORD_ARRAY_SIZE";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_TYPE] =
    "GL_TEXTURE_COORD_ARRAY_TYPE";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_STRIDE] =
    "GL_TEXTURE_COORD_ARRAY_STRIDE";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_STRIDE] = "GL_EDGE_FLAG_ARRAY_STRIDE";
  m_gl_token_names[GL_VERTEX_ARRAY_POINTER] = "GL_VERTEX_ARRAY_POINTER";
  m_gl_token_names[GL_NORMAL_ARRAY_POINTER] = "GL_NORMAL_ARRAY_POINTER";
  m_gl_token_names[GL_COLOR_ARRAY_POINTER] = "GL_COLOR_ARRAY_POINTER";
  m_gl_token_names[GL_INDEX_ARRAY_POINTER] = "GL_INDEX_ARRAY_POINTER";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_POINTER] =
    "GL_TEXTURE_COORD_ARRAY_POINTER";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_POINTER] = "GL_EDGE_FLAG_ARRAY_POINTER";
  m_gl_token_names[GL_V2F] = "GL_V2F";
  m_gl_token_names[GL_V3F] = "GL_V3F";
  m_gl_token_names[GL_C4UB_V2F] = "GL_C4UB_V2F";
  m_gl_token_names[GL_C4UB_V3F] = "GL_C4UB_V3F";
  m_gl_token_names[GL_C3F_V3F] = "GL_C3F_V3F";
  m_gl_token_names[GL_N3F_V3F] = "GL_N3F_V3F";
  m_gl_token_names[GL_C4F_N3F_V3F] = "GL_C4F_N3F_V3F";
  m_gl_token_names[GL_T2F_V3F] = "GL_T2F_V3F";
  m_gl_token_names[GL_T4F_V4F] = "GL_T4F_V4F";
  m_gl_token_names[GL_T2F_C4UB_V3F] = "GL_T2F_C4UB_V3F";
  m_gl_token_names[GL_T2F_C3F_V3F] = "GL_T2F_C3F_V3F";
  m_gl_token_names[GL_T2F_N3F_V3F] = "GL_T2F_N3F_V3F";
  m_gl_token_names[GL_T2F_C4F_N3F_V3F] = "GL_T2F_C4F_N3F_V3F";
  m_gl_token_names[GL_T4F_C4F_N3F_V4F] = "GL_T4F_C4F_N3F_V4F";

  /* Extensions */
  m_gl_token_names[GL_EXT_vertex_array] = "GL_EXT_vertex_array";
  m_gl_token_names[GL_EXT_bgra] = "GL_EXT_bgra";
  m_gl_token_names[GL_EXT_paletted_texture] = "GL_EXT_paletted_texture";

#if defined(GL_WIN_swap_hint)
  m_gl_token_names[GL_WIN_swap_hint] = "GL_WIN_swap_hint";
#endif

#if defined(GL_WIN_draw_range_elements)
  m_gl_token_names[GL_WIN_draw_range_elements] = "GL_WIN_draw_range_elements";
#endif
  // #define GL_WIN_phong_shading              1
  // #define GL_WIN_specular_fog               1

  /* EXT_vertex_array */
  m_gl_token_names[GL_VERTEX_ARRAY_EXT] = "GL_VERTEX_ARRAY_EXT";
  m_gl_token_names[GL_NORMAL_ARRAY_EXT] = "GL_NORMAL_ARRAY_EXT";
  m_gl_token_names[GL_COLOR_ARRAY_EXT] = "GL_COLOR_ARRAY_EXT";
  m_gl_token_names[GL_INDEX_ARRAY_EXT] = "GL_INDEX_ARRAY_EXT";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_EXT] = "GL_TEXTURE_COORD_ARRAY_EXT";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_EXT] = "GL_EDGE_FLAG_ARRAY_EXT";
  m_gl_token_names[GL_VERTEX_ARRAY_SIZE_EXT] = "GL_VERTEX_ARRAY_SIZE_EXT";
  m_gl_token_names[GL_VERTEX_ARRAY_TYPE_EXT] = "GL_VERTEX_ARRAY_TYPE_EXT";
  m_gl_token_names[GL_VERTEX_ARRAY_STRIDE_EXT] = "GL_VERTEX_ARRAY_STRIDE_EXT";
  m_gl_token_names[GL_VERTEX_ARRAY_COUNT_EXT] = "GL_VERTEX_ARRAY_COUNT_EXT";
  m_gl_token_names[GL_NORMAL_ARRAY_TYPE_EXT] = "GL_NORMAL_ARRAY_TYPE_EXT";
  m_gl_token_names[GL_NORMAL_ARRAY_STRIDE_EXT] = "GL_NORMAL_ARRAY_STRIDE_EXT";
  m_gl_token_names[GL_NORMAL_ARRAY_COUNT_EXT] = "GL_NORMAL_ARRAY_COUNT_EXT";
  m_gl_token_names[GL_COLOR_ARRAY_SIZE_EXT] = "GL_COLOR_ARRAY_SIZE_EXT";
  m_gl_token_names[GL_COLOR_ARRAY_TYPE_EXT] = "GL_COLOR_ARRAY_TYPE_EXT";
  m_gl_token_names[GL_COLOR_ARRAY_STRIDE_EXT] = "GL_COLOR_ARRAY_STRIDE_EXT";
  m_gl_token_names[GL_COLOR_ARRAY_COUNT_EXT] = "GL_COLOR_ARRAY_COUNT_EXT";
  m_gl_token_names[GL_INDEX_ARRAY_TYPE_EXT] = "GL_INDEX_ARRAY_TYPE_EXT";
  m_gl_token_names[GL_INDEX_ARRAY_STRIDE_EXT] = "GL_INDEX_ARRAY_STRIDE_EXT";
  m_gl_token_names[GL_INDEX_ARRAY_COUNT_EXT] = "GL_INDEX_ARRAY_COUNT_EXT";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_SIZE_EXT] =
    "GL_TEXTURE_COORD_ARRAY_SIZE_EXT";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_TYPE_EXT] =
    "GL_TEXTURE_COORD_ARRAY_TYPE_EXT";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_STRIDE_EXT] =
    "GL_TEXTURE_COORD_ARRAY_STRIDE_EXT";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_COUNT_EXT] =
    "GL_TEXTURE_COORD_ARRAY_COUNT_EXT";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_STRIDE_EXT] =
    "GL_EDGE_FLAG_ARRAY_STRIDE_EXT";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_COUNT_EXT] =
    "GL_EDGE_FLAG_ARRAY_COUNT_EXT";
  m_gl_token_names[GL_VERTEX_ARRAY_POINTER_EXT] =
    "GL_VERTEX_ARRAY_POINTER_EXT";
  m_gl_token_names[GL_NORMAL_ARRAY_POINTER_EXT] =
    "GL_NORMAL_ARRAY_POINTER_EXT";
  m_gl_token_names[GL_COLOR_ARRAY_POINTER_EXT] = "GL_COLOR_ARRAY_POINTER_EXT";
  m_gl_token_names[GL_INDEX_ARRAY_POINTER_EXT] = "GL_INDEX_ARRAY_POINTER_EXT";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_POINTER_EXT] =
    "GL_TEXTURE_COORD_ARRAY_POINTER_EXT";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_POINTER_EXT] =
    "GL_EDGE_FLAG_ARRAY_POINTER_EXT";

#if defined(GL_DOUBLE_EXT)
  m_gl_token_names[GL_DOUBLE_EXT] = "GL_DOUBLE_EXT";
#endif

  /* EXT_bgra */
  m_gl_token_names[GL_BGR_EXT] = "GL_BGR_EXT";
  m_gl_token_names[GL_BGRA_EXT] = "GL_BGRA_EXT";

  /* EXT_paletted_texture */

  /* These must match the GL_COLOR_TABLE_*_SGI enumerants */
#if defined(GL_COLOR_TABLE_FORMAT_EXT)
  m_gl_token_names[GL_COLOR_TABLE_FORMAT_EXT] = "GL_COLOR_TABLE_FORMAT_EXT";
#endif

#if defined(GL_COLOR_TABLE_WIDTH_EXT)
  m_gl_token_names[GL_COLOR_TABLE_WIDTH_EXT] = "GL_COLOR_TABLE_WIDTH_EXT";
#endif

#if defined(GL_COLOR_TABLE_RED_SIZE_EXT)
  m_gl_token_names[GL_COLOR_TABLE_RED_SIZE_EXT] =
    "GL_COLOR_TABLE_RED_SIZE_EXT";
#endif

#if defined(GL_COLOR_TABLE_GREEN_SIZE_EXT)
  m_gl_token_names[GL_COLOR_TABLE_GREEN_SIZE_EXT] =
    "GL_COLOR_TABLE_GREEN_SIZE_EXT";
#endif

#if defined(GL_COLOR_TABLE_BLUE_SIZE_EXT)
  m_gl_token_names[GL_COLOR_TABLE_BLUE_SIZE_EXT] =
    "GL_COLOR_TABLE_BLUE_SIZE_EXT";
#endif

#if defined(GL_COLOR_TABLE_ALPHA_SIZE_EXT)
  m_gl_token_names[GL_COLOR_TABLE_ALPHA_SIZE_EXT] =
    "GL_COLOR_TABLE_ALPHA_SIZE_EXT";
#endif

#if defined(GL_COLOR_TABLE_LUMINANCE_SIZE_EXT)
  m_gl_token_names[GL_COLOR_TABLE_LUMINANCE_SIZE_EXT] =
    "GL_COLOR_TABLE_LUMINANCE_SIZE_EXT";
#endif

#if defined(GL_COLOR_TABLE_INTENSITY_SIZE_EXT)
  m_gl_token_names[GL_COLOR_TABLE_INTENSITY_SIZE_EXT] =
    "GL_COLOR_TABLE_INTENSITY_SIZE_EXT";
#endif

  m_gl_token_names[GL_COLOR_INDEX1_EXT] = "GL_COLOR_INDEX1_EXT";
  m_gl_token_names[GL_COLOR_INDEX2_EXT] = "GL_COLOR_INDEX2_EXT";
  m_gl_token_names[GL_COLOR_INDEX4_EXT] = "GL_COLOR_INDEX4_EXT";
  m_gl_token_names[GL_COLOR_INDEX8_EXT] = "GL_COLOR_INDEX8_EXT";
  m_gl_token_names[GL_COLOR_INDEX12_EXT] = "GL_COLOR_INDEX12_EXT";
  m_gl_token_names[GL_COLOR_INDEX16_EXT] = "GL_COLOR_INDEX16_EXT";

  /* WIN_draw_range_elements */
#if defined(GL_MAX_ELEMENTS_VERTICES_WIN)
  m_gl_token_names[GL_MAX_ELEMENTS_VERTICES_WIN] =
    "GL_MAX_ELEMENTS_VERTICES_WIN";
#endif

#if defined(GL_MAX_ELEMENTS_INDICES_WIN)
  m_gl_token_names[GL_MAX_ELEMENTS_INDICES_WIN] =
    "GL_MAX_ELEMENTS_INDICES_WIN";
#endif


  /* WIN_phong_shading */
  m_gl_token_names[GL_PHONG_WIN] = "GL_PHONG_WIN";
  m_gl_token_names[GL_PHONG_HINT_WIN] = "GL_PHONG_HINT_WIN";

  /* WIN_specular_fog */
  m_gl_token_names[GL_FOG_SPECULAR_TEXTURE_WIN] =
    "GL_FOG_SPECULAR_TEXTURE_WIN";

  /* For compatibility with OpenGL v1.0 */
  m_gl_token_names[GL_LOGIC_OP] = "GL_LOGIC_OP";
  m_gl_token_names[GL_TEXTURE_COMPONENTS] = "GL_TEXTURE_COMPONENTS";

  /*! ARB Extensions */
#if defined(GL_ARB_vertex_buffer_object)
  m_gl_token_names[GL_BUFFER_SIZE_ARB] = "GL_BUFFER_SIZE_ARB";
  m_gl_token_names[GL_BUFFER_USAGE_ARB] = "GL_BUFFER_USAGE_ARB";
  m_gl_token_names[GL_ARRAY_BUFFER_ARB] = "GL_ARRAY_BUFFER_ARB";
  m_gl_token_names[GL_ELEMENT_ARRAY_BUFFER_ARB] = "GL_ELEMENT_ARRAY_BUFFER_ARB";
  m_gl_token_names[GL_ARRAY_BUFFER_BINDING_ARB] = "GL_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB] = "GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_VERTEX_ARRAY_BUFFER_BINDING_ARB] = "GL_VERTEX_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_NORMAL_ARRAY_BUFFER_BINDING_ARB] = "GL_NORMAL_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_COLOR_ARRAY_BUFFER_BINDING_ARB] = "GL_COLOR_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_INDEX_ARRAY_BUFFER_BINDING_ARB] = "GL_INDEX_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB] = "GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB] = "GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB] = "GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB] = "GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB] = "GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB] = "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB";
  m_gl_token_names[GL_READ_ONLY_ARB] = "GL_READ_ONLY_ARB";
  m_gl_token_names[GL_WRITE_ONLY_ARB] = "GL_WRITE_ONLY_ARB";
  m_gl_token_names[GL_READ_WRITE_ARB] = "GL_READ_WRITE_ARB";
  m_gl_token_names[GL_BUFFER_ACCESS_ARB] = "GL_BUFFER_ACCESS_ARB";
  m_gl_token_names[GL_BUFFER_MAPPED_ARB] = "GL_BUFFER_MAPPED_ARB";
  m_gl_token_names[GL_BUFFER_MAP_POINTER_ARB] = "GL_BUFFER_MAP_POINTER_ARB";
  m_gl_token_names[GL_STREAM_DRAW_ARB] = "GL_STREAM_DRAW_ARB";
  m_gl_token_names[GL_STREAM_READ_ARB] = "GL_STREAM_READ_ARB";
  m_gl_token_names[GL_STREAM_COPY_ARB] = "GL_STREAM_COPY_ARB";
  m_gl_token_names[GL_STATIC_DRAW_ARB] = "GL_STATIC_DRAW_ARB";
  m_gl_token_names[GL_STATIC_READ_ARB] = "GL_STATIC_READ_ARB";
  m_gl_token_names[GL_STATIC_COPY_ARB] = "GL_STATIC_COPY_ARB";
  m_gl_token_names[GL_DYNAMIC_DRAW_ARB] = "GL_DYNAMIC_DRAW_ARB";
  m_gl_token_names[GL_DYNAMIC_READ_ARB] = "GL_DYNAMIC_READ_ARB";
  m_gl_token_names[GL_DYNAMIC_COPY_ARB] = "GL_DYNAMIC_COPY_ARB";
#endif

#if defined(GL_VERSION_1_2)
  m_gl_token_names[GL_UNSIGNED_BYTE_3_3_2] = "GL_UNSIGNED_BYTE_3_3_2";
  m_gl_token_names[GL_UNSIGNED_SHORT_4_4_4_4] = "GL_UNSIGNED_SHORT_4_4_4_4";
  m_gl_token_names[GL_UNSIGNED_SHORT_5_5_5_1] = "GL_UNSIGNED_SHORT_5_5_5_1";
  m_gl_token_names[GL_UNSIGNED_INT_8_8_8_8] = "GL_UNSIGNED_INT_8_8_8_8";
  m_gl_token_names[GL_UNSIGNED_INT_10_10_10_2] = "GL_UNSIGNED_INT_10_10_10_2";
  m_gl_token_names[GL_RESCALE_NORMAL] = "GL_RESCALE_NORMAL";
  m_gl_token_names[GL_TEXTURE_BINDING_3D] = "GL_TEXTURE_BINDING_3D";
  m_gl_token_names[GL_PACK_SKIP_IMAGES] = "GL_PACK_SKIP_IMAGES";
  m_gl_token_names[GL_PACK_IMAGE_HEIGHT] = "GL_PACK_IMAGE_HEIGHT";
  m_gl_token_names[GL_UNPACK_SKIP_IMAGES] = "GL_UNPACK_SKIP_IMAGES";
  m_gl_token_names[GL_UNPACK_IMAGE_HEIGHT] = "GL_UNPACK_IMAGE_HEIGHT";
  m_gl_token_names[GL_TEXTURE_3D] = "GL_TEXTURE_3D";
  m_gl_token_names[GL_PROXY_TEXTURE_3D] = "GL_PROXY_TEXTURE_3D";
  m_gl_token_names[GL_TEXTURE_DEPTH] = "GL_TEXTURE_DEPTH";
  m_gl_token_names[GL_TEXTURE_WRAP_R] = "GL_TEXTURE_WRAP_R";
  m_gl_token_names[GL_MAX_3D_TEXTURE_SIZE] = "GL_MAX_3D_TEXTURE_SIZE";
  m_gl_token_names[GL_UNSIGNED_BYTE_2_3_3_REV] = "GL_UNSIGNED_BYTE_2_3_3_REV";
  m_gl_token_names[GL_UNSIGNED_SHORT_5_6_5] = "GL_UNSIGNED_SHORT_5_6_5";
  m_gl_token_names[GL_UNSIGNED_SHORT_5_6_5_REV] = "GL_UNSIGNED_SHORT_5_6_5_REV";
  m_gl_token_names[GL_UNSIGNED_SHORT_4_4_4_4_REV] = "GL_UNSIGNED_SHORT_4_4_4_4_REV";
  m_gl_token_names[GL_UNSIGNED_SHORT_1_5_5_5_REV] = "GL_UNSIGNED_SHORT_1_5_5_5_REV";
  m_gl_token_names[GL_UNSIGNED_INT_8_8_8_8_REV] = "GL_UNSIGNED_INT_8_8_8_8_REV";
  m_gl_token_names[GL_UNSIGNED_INT_2_10_10_10_REV] = "GL_UNSIGNED_INT_2_10_10_10_REV";
  m_gl_token_names[GL_BGR] = "GL_BGR";
  m_gl_token_names[GL_BGRA] = "GL_BGRA";
  m_gl_token_names[GL_MAX_ELEMENTS_VERTICES] = "GL_MAX_ELEMENTS_VERTICES";
  m_gl_token_names[GL_MAX_ELEMENTS_INDICES] = "GL_MAX_ELEMENTS_INDICES";
  m_gl_token_names[GL_CLAMP_TO_EDGE] = "GL_CLAMP_TO_EDGE";
  m_gl_token_names[GL_TEXTURE_MIN_LOD] = "GL_TEXTURE_MIN_LOD";
  m_gl_token_names[GL_TEXTURE_MAX_LOD] = "GL_TEXTURE_MAX_LOD";
  m_gl_token_names[GL_TEXTURE_BASE_LEVEL] = "GL_TEXTURE_BASE_LEVEL";
  m_gl_token_names[GL_TEXTURE_MAX_LEVEL] = "GL_TEXTURE_MAX_LEVEL";
  m_gl_token_names[GL_LIGHT_MODEL_COLOR_CONTROL] = "GL_LIGHT_MODEL_COLOR_CONTROL";
  m_gl_token_names[GL_SINGLE_COLOR] = "GL_SINGLE_COLOR";
  m_gl_token_names[GL_SEPARATE_SPECULAR_COLOR] = "GL_SEPARATE_SPECULAR_COLOR";
  m_gl_token_names[GL_SMOOTH_POINT_SIZE_RANGE] = "GL_SMOOTH_POINT_SIZE_RANGE";
  m_gl_token_names[GL_SMOOTH_POINT_SIZE_GRANULARITY] = "GL_SMOOTH_POINT_SIZE_GRANULARITY";
  m_gl_token_names[GL_SMOOTH_LINE_WIDTH_RANGE] = "GL_SMOOTH_LINE_WIDTH_RANGE";
  m_gl_token_names[GL_SMOOTH_LINE_WIDTH_GRANULARITY] = "GL_SMOOTH_LINE_WIDTH_GRANULARITY";
  m_gl_token_names[GL_ALIASED_POINT_SIZE_RANGE] = "GL_ALIASED_POINT_SIZE_RANGE";
  m_gl_token_names[GL_ALIASED_LINE_WIDTH_RANGE] = "GL_ALIASED_LINE_WIDTH_RANGE";
#endif

#if defined(GL_VERSION_4_3)
  m_gl_token_names[GL_MAX_COLOR_ATTACHMENTS] = "GL_MAX_COLOR_ATTACHMENTS";
  m_gl_token_names[GL_COLOR_ATTACHMENT0] = "GL_COLOR_ATTACHMENT0";
  m_gl_token_names[GL_COLOR_ATTACHMENT1] = "GL_COLOR_ATTACHMENT1";
  m_gl_token_names[GL_COLOR_ATTACHMENT2] = "GL_COLOR_ATTACHMENT2";
  m_gl_token_names[GL_COLOR_ATTACHMENT3] = "GL_COLOR_ATTACHMENT3";
  m_gl_token_names[GL_COLOR_ATTACHMENT4] = "GL_COLOR_ATTACHMENT4";
  m_gl_token_names[GL_COLOR_ATTACHMENT5] = "GL_COLOR_ATTACHMENT5";
  m_gl_token_names[GL_COLOR_ATTACHMENT6] = "GL_COLOR_ATTACHMENT6";
  m_gl_token_names[GL_COLOR_ATTACHMENT7] = "GL_COLOR_ATTACHMENT7";
  m_gl_token_names[GL_COLOR_ATTACHMENT8] = "GL_COLOR_ATTACHMENT8";
  m_gl_token_names[GL_COLOR_ATTACHMENT9] = "GL_COLOR_ATTACHMENT9";
  m_gl_token_names[GL_COLOR_ATTACHMENT10] = "GL_COLOR_ATTACHMENT10";
  m_gl_token_names[GL_COLOR_ATTACHMENT11] = "GL_COLOR_ATTACHMENT11";
  m_gl_token_names[GL_COLOR_ATTACHMENT12] = "GL_COLOR_ATTACHMENT12";
  m_gl_token_names[GL_COLOR_ATTACHMENT13] = "GL_COLOR_ATTACHMENT13";
  m_gl_token_names[GL_COLOR_ATTACHMENT14] = "GL_COLOR_ATTACHMENT14";
  m_gl_token_names[GL_COLOR_ATTACHMENT15] = "GL_COLOR_ATTACHMENT15";
#endif
}

/*! Destructor */
Gl_wrapper::~Gl_wrapper()
{
  m_gl_token_names.clear();
}

/*! Obtain the singletone */
Gl_wrapper* Gl_wrapper::get_instance()
{
  if (s_instance == NULL) s_instance = new Gl_wrapper();
  return s_instance;
}

/*! Find the name of a token */
const char* Gl_wrapper::find_name(GLenum num)
{
  Name_const_iter it = m_gl_token_names.find(num);
  if (it == m_gl_token_names.end()) return "";
  else return (*it).second;
}

/*! \brief checks whether openGl errors have occured. */
void Gl_wrapper::check_error()
{
  GLenum err = glGetError();
  while (err != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << gluErrorString(err) << std::endl;
    break;
    // err = glGetError();
  }
}

SGAL_END_NAMESPACE
