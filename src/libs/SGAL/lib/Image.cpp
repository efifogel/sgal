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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Image::s_prototype(nullptr);
const std::string Image::s_tag("Image");

/*! A map from format to number of bits */
Uint Image::s_format_sizes[] = {
    0,                  // kIllegal
    1,                  // kFormat1
    2,                  // kFormat2
    4,                  // kFormat4
    8,                  // kFormat8
    12,                 // kFormat12
    16,                 // kFormat16
    32,                 // kFormat32
    1,                  // kPalette1
    2,                  // kPalette2
    4,                  // kPalette4
    8,                  // kPalette8
    12,                 // kPalette12
    16,                 // kPalette16
    4,                  // kAlpha4
    8,                  // kAlpha8
    12,                 // kAlpha12
    16,                 // kAlpha16
    4,                  // kLuminance4
    8,                  // kLuminance8
    12,                 // kLuminance12
    16,                 // kLuminance16
    4,                  // kIntensity4
    8,                  // kIntensity8
    12,                 // kIntensity12
    16,                 // kIntensity16
    16,                 // kAlphaPalette8_8
    8,                  // kLuminanceAlpha4_4
    8,                  // kLuminanceAlpha6_2
    16,                 // kLuminanceAlpha8_8
    16,                 // kLuminanceAlpha12_4
    24,                 // kLuminanceAlpha12_12
    32,                 // kLuminanceAlpha16_16
    8,                  // kAlphaLuminance4_4
    16,                 // kAlphaLuminance8_8
    8,                  // kRGB3_3_2
    12,                 // kRGB4_4_4
    15,                 // kRGB5_5_5
    16,                 // kRGB5_6_5
    24,                 // kRGB8_8_8
    30,                 // kRGB10_10_10
    36,                 // kRGB12_12_12
    48,                 // kRGB16_16_16
    8,                  // kBGR2_3_3
    12,                 // kBGR4_4_4
    15,                 // kBGR5_5_5
    16,                 // kBGR5_6_5
    24,                 // kBGR8_8_8
    30,                 // kBGR10_10_10
    36,                 // kBGR12_12_12
    48,                 // kBGR16_16_16
    8,                  // kYIQ4_2_2
    8,                  // kRGBA2_2_2_2
    16,                 // kRGBA4_4_4_4
    16,                 // kRGBA3_3_2_8
    16,                 // kRGBA5_5_5_1
    32,                 // kRGBA8_8_8_8
    32,                 // kRGBA10_10_10_2
    48,                 // kRGBA12_12_12_12
    64,                 // kRGBA16_16_16_16
    8,                  // kARGB2_2_2_2,
    16,                 // kARGB4_4_4_4
    16,                 // kARGB8_3_3_2
    16,                 // kARGB1_5_5_5
    32,                 // kARGB8_8_8_8
    32,                 // kARGB10_10_10_2
    48,                 // kkARGB12_12_12_12
    64,                 // kARGB16_16_16_16
    8,                  // kABGR2_2_2_2
    16,                 // kABGR4_4_4_4
    16,                 // kABGR8_2_3_3
    16,                 // kABGR1_5_5_5
    32,                 // kABGR8_8_8_8
    32,                 // kABGR2_10_10_10
    48,                 // kABGR12_12_12_12
    64,                 // kABGR16_16_16_16
    16,                 // kAYIQ8_4_2_2
    16,                 // kDepth16
    32,                 // kDepth32
    1,                  // kStencil
    32                  // kDepthStencil24_8
};

/*! A map from format to OpenGl format
 * The OpenGL format of the kIntensity<x> and kAlpha<x> formats is
 * GL_LUMINANCE<x>
 */
GLenum Image::s_format_formats[] = {
  (Uint) -1,                    // kIllegal
  (Uint) -1,                    // kFormat1
  (Uint) -1,                    // kFormat2
  (Uint) -1,                    // kFormat4
  (Uint) -1,                    // kFormat8
  (Uint) -1,                    // kFormat12
  (Uint) -1,                    // kFormat16
  (Uint) -1,                    // kFormat32
  (Uint) -1,                    // kPalette1
  (Uint) -1,                    // kPalette2
  (Uint) -1,                    // kPalette4
  (Uint) -1,                    // kPalette8
  (Uint) -1,                    // kPalette12
  (Uint) -1,                    // kPalette16
  GL_LUMINANCE4,                // kAlpha4
  GL_LUMINANCE,                 // kAlpha8, works better than GL_ALPHA8
  GL_LUMINANCE12,               // kAlpha12
  GL_LUMINANCE16,               // kAlpha16
  GL_LUMINANCE4,                // kLuminance4
  GL_LUMINANCE,                 // kLuminance8, works better than GL_LUMINANCE8
  GL_LUMINANCE12,               // kLuminance12
  GL_LUMINANCE16,               // kLuminance16
  GL_LUMINANCE4,                // kIntensity4
  GL_LUMINANCE,                 // kIntensity8
  GL_LUMINANCE12,               // kIntensity12
  GL_LUMINANCE16,               // kIntensity16
  (Uint) -1,                    // kAlphaPalette8_8
  GL_LUMINANCE4_ALPHA4,         // kLuminanceAlpha4_4
  GL_LUMINANCE6_ALPHA2,         // kLuminanceAlpha6_2
  GL_LUMINANCE_ALPHA,           // kLuminanceAlpha8_8, GL_LUMINANCE8_ALPHA8
  GL_LUMINANCE12_ALPHA4,        // kLuminanceAlpha12_4
  GL_LUMINANCE12_ALPHA12,       // kLuminanceAlpha12_12
  GL_LUMINANCE16_ALPHA16,       // kLuminanceAlpha16_16
  GL_LUMINANCE4_ALPHA4,         // kAlphaLuminance4_4
  GL_LUMINANCE8_ALPHA8,         // kAlphaLuminance8_8
  GL_R3_G3_B2,                  // kRGB3_3_2
  GL_RGB4,                      // kRGB4_4_4
  (Uint) -1,                    // kRGB5_5_5
  GL_RGB5,                      // kRGB5_6_5
  GL_RGB,                       // kRGB8_8_8, works better than GL_RGB8
  GL_RGB10,                     // kRGB10_10_10
  GL_RGB12,                     // kRGB12_12_12
  GL_RGB16,                     // kRGB16_16_16
  (Uint) -1,                    // kBGR2_3_3
  (Uint) -1,                    // kBGR4_4_4
  (Uint) -1,                    // kBGR5_5_5
  (Uint) -1,                    // kBGR5_6_5
  (Uint) -1,                    // kBGR8_8_8
  (Uint) -1,                    // kBGR10_10_10
  (Uint) -1,                    // kBGR12_12_12
  (Uint) -1,                    // kBGR16_16_16
  (Uint) -1,                    // kYIQ4_2_2
  GL_RGBA2,                     // kRGBA2_2_2_2
  GL_RGBA4,                     // kRGBA4_4_4_4
  (Uint) -1,                    // kRGBA3_3_2_8
  GL_RGB5_A1,                   // kRGBA5_5_5_1
  GL_RGBA,                      // kRGBA8_8_8_8, works better than GL_RGBA8
  GL_RGB10_A2,                  // kRGBA10_10_10_2
  GL_RGBA12,                    // kRGBA12_12_12_12
  GL_RGBA16,                    // kRGBA16_16_16_16
  (Uint) -1,                    // kARGB2_2_2_2
  (Uint) -1,                    // kARGB4_4_4_4
  (Uint) -1,                    // kARGB8_3_3_2
  (Uint) -1,                    // kARGB1_5_5_5
  (Uint) -1,                    // kARGB8_8_8_8
  (Uint) -1,                    // kARGB10_10_10_2
  (Uint) -1,                    // kkARGB12_12_12_12
  (Uint) -1,                    // kARGB16_16_16_16
  (Uint) -1,                    // kABGR2_2_2_2
  (Uint) -1,                    // kABGR4_4_4_4
  (Uint) -1,                    // kABGR8_2_3_3
  (Uint) -1,                    // kABGR1_5_5_5
  (Uint) -1,                    // kABGR8_8_8_8
  (Uint) -1,                    // kABGR2_10_10_10
  (Uint) -1,                    // kABGR12_12_12_12
  (Uint) -1,                    // kABGR16_16_16_16
  (Uint) -1,                    // kAYIQ8_4_2_2
  GL_DEPTH_COMPONENT,           // kDepth16
  GL_DEPTH_COMPONENT,           // kDepth32
  GL_STENCIL_INDEX,             // kStencil
  GL_DEPTH_STENCIL_NV           // kDepthStencil24_8
};

/*! A map from format to OpenGl internal format */
GLenum Image::s_format_internal_formats[] = {
  (Uint) -1,                    // kIllegal
  (Uint) -1,                    // kFormat1
  (Uint) -1,                    // kFormat2
  (Uint) -1,                    // kFormat4
  (Uint) -1,                    // kFormat8
  (Uint) -1,                    // kFormat12
  (Uint) -1,                    // kFormat16
  (Uint) -1,                    // kFormat32
  (Uint) -1,                    // kPalette1
  (Uint) -1,                    // kPalette2
  (Uint) -1,                    // kPalette4
  (Uint) -1,                    // kPalette8
  (Uint) -1,                    // kPalette12
  (Uint) -1,                    // kPalette16
  GL_ALPHA4,                    // kAlpha4
  GL_ALPHA,                     // kAlpha8,
  GL_ALPHA12,                   // kAlpha12
  GL_ALPHA16,                   // kAlpha16
  GL_LUMINANCE4,                // kLuminance4
  1,                            // kLuminance8,
  GL_LUMINANCE12,               // kLuminance12
  GL_LUMINANCE16,               // kLuminance16
  GL_INTENSITY4,                // kIntensity4
  GL_INTENSITY,                 // kIntensity8
  GL_INTENSITY12,               // kIntensity12
  GL_INTENSITY16,               // kIntensity16
  (Uint) -1,                    // kAlphaPalette8_8
  GL_LUMINANCE4_ALPHA4,         // kLuminanceAlpha4_4
  GL_LUMINANCE6_ALPHA2,         // kLuminanceAlpha6_2
  2,                            // kLuminanceAlpha8_8, GL_LUMINANCE8_ALPHA8
  GL_LUMINANCE12_ALPHA4,        // kLuminanceAlpha12_4
  GL_LUMINANCE12_ALPHA12,       // kLuminanceAlpha12_12
  GL_LUMINANCE16_ALPHA16,       // kLuminanceAlpha16_16
  GL_LUMINANCE4_ALPHA4,         // kAlphaLuminance4_4
  GL_LUMINANCE8_ALPHA8,         // kAlphaLuminance8_8
  GL_R3_G3_B2,                  // kRGB3_3_2
  GL_RGB4,                      // kRGB4_4_4
  (Uint) -1,                    // kRGB5_5_5
  GL_RGB5,                      // kRGB5_6_5
  GL_RGB8,                      // kRGB8_8_8,
  GL_RGB10,                     // kRGB10_10_10
  GL_RGB12,                     // kRGB12_12_12
  GL_RGB16,                     // kRGB16_16_16
  (Uint) -1,                    // kBGR2_3_3
  (Uint) -1,                    // kBGR4_4_4
  (Uint) -1,                    // kBGR5_5_5
  (Uint) -1,                    // kBGR5_6_5
  (Uint) -1,                    // kBGR8_8_8
  (Uint) -1,                    // kBGR10_10_10
  (Uint) -1,                    // kBGR12_12_12
  (Uint) -1,                    // kBGR16_16_16
  (Uint) -1,                    // kYIQ4_2_2
  GL_RGBA2,                     // kRGBA2_2_2_2
  GL_RGBA4,                     // kRGBA4_4_4_4
  (Uint) -1,                    // kRGBA3_3_2_8
  GL_RGB5_A1,                   // kRGBA5_5_5_1
  GL_RGBA8,                     // kRGBA8_8_8_8, works better than GL_RGBA8 ?
  GL_RGB10_A2,                  // kRGBA10_10_10_2
  GL_RGBA12,                    // kRGBA12_12_12_12
  GL_RGBA16,                    // kRGBA16_16_16_16
  (Uint) -1,                    // kARGB2_2_2_2
  (Uint) -1,                    // kARGB4_4_4_4
  (Uint) -1,                    // kARGB8_3_3_2
  (Uint) -1,                    // kARGB1_5_5_5
  (Uint) -1,                    // kARGB8_8_8_8
  (Uint) -1,                    // kARGB10_10_10_2
  (Uint) -1,                    // kkARGB12_12_12_12
  (Uint) -1,                    // kARGB16_16_16_16
  (Uint) -1,                    // kABGR2_2_2_2
  (Uint) -1,                    // kABGR4_4_4_4
  (Uint) -1,                    // kABGR8_2_3_3
  (Uint) -1,                    // kABGR1_5_5_5
  (Uint) -1,                    // kABGR8_8_8_8
  (Uint) -1,                    // kABGR2_10_10_10
  (Uint) -1,                    // kABGR12_12_12_12
  (Uint) -1,                    // kABGR16_16_16_16
  (Uint) -1,                    // kAYIQ8_4_2_2
  GL_DEPTH_COMPONENT,           // kDepth16
  GL_DEPTH_COMPONENT,           // kDepth32
  GL_STENCIL_INDEX,             // kStencil
  GL_DEPTH_STENCIL_NV           // kDepthStencil24_8
};

/*! A map from format to storage type of component */
GLenum Image::s_format_types[] = {
  (Uint) -1,                    // kIllegal
  (Uint) -1,                    // kFormat1
  (Uint) -1,                    // kFormat2
  (Uint) -1,                    // kFormat4
  GL_UNSIGNED_BYTE,             // kFormat8
  (Uint) -1,                    // kFormat12
  GL_UNSIGNED_SHORT,            // kFormat16
  GL_UNSIGNED_INT,              // kFormat32
  (Uint) -1,                    // kPalette1
  (Uint) -1,                    // kPalette2
  (Uint) -1,                    // kPalette4
  GL_UNSIGNED_BYTE,             // kPalette8
  (Uint) -1,                    // kPalette12
  GL_UNSIGNED_SHORT,            // kPalette16
  (Uint) -1,                    // kAlpha4
  GL_UNSIGNED_BYTE,             // kAlpha8
  (Uint) -1,                    // kAlpha12
  GL_UNSIGNED_SHORT,            // kAlpha16
  (Uint) -1,                    // kLuminance4
  GL_UNSIGNED_BYTE,             // kLuminance8
  (Uint) -1,                    // kLuminance12
  GL_UNSIGNED_SHORT,            // kLuminance16
  (Uint) -1,                    // kIntensity4
  GL_UNSIGNED_BYTE,             // kIntensity8
  (Uint) -1,                    // kIntensity12
  GL_UNSIGNED_SHORT,            // kIntensity16
  GL_UNSIGNED_BYTE,             // kAlphaPalette8_8
  (Uint) -1,                    // kLuminanceAlpha4_4
  (Uint) -1,                    // kLuminanceAlpha6_2
  GL_UNSIGNED_BYTE,             // kLuminanceAlpha8_8
  (Uint) -1,                    // kLuminanceAlpha12_4
  (Uint) -1,                    // kLuminanceAlpha12_12
  GL_UNSIGNED_SHORT,            // kLuminanceAlpha16_16
  (Uint) -1,                    // kAlphaLuminance4_4
  GL_UNSIGNED_BYTE,             // kAlphaLuminance8_8
  GL_UNSIGNED_BYTE_3_3_2,       // kRGB3_3_2
  (Uint) -1,                    // kRGB4_4_4
  (Uint) -1,                    // kRGB5_5_5
  GL_UNSIGNED_SHORT_5_6_5,      // kRGB5_6_5
  GL_UNSIGNED_BYTE,             // kRGB8_8_8
  (Uint) -1,                    // kRGB10_10_10
  (Uint) -1,                    // kRGB12_12_12
  GL_UNSIGNED_SHORT,            // kRGB16_16_16
  (Uint) -1,                    // kBGR2_3_3
  (Uint) -1,                    // kBGR4_4_4
  (Uint) -1,                    // kBGR5_5_5
  (Uint) -1,                    // kBGR5_6_5
  (Uint) -1,                    // kBGR8_8_8
  (Uint) -1,                    // kBGR10_10_10
  (Uint) -1,                    // kBGR12_12_12
  (Uint) -1,                    // kBGR16_16_16
  (Uint) -1,                    // kYIQ4_2_2
  (Uint) -1,                    // kRGBA2_2_2_2
  GL_UNSIGNED_SHORT_4_4_4_4,    // kRGBA4_4_4_4
  (Uint) -1,                    // kRGBA3_3_2_8
  GL_UNSIGNED_SHORT_5_5_5_1,    // kRGBA5_5_5_1
  GL_UNSIGNED_BYTE,             // kRGBA8_8_8_8
  (Uint) -1,                    // kRGBA10_10_10_2
  (Uint) -1,                    // kRGBA12_12_12_12
  (Uint) -1,                    // kRGBA16_16_16_16
  (Uint) -1,                    // kARGB2_2_2_2
  GL_UNSIGNED_SHORT_4_4_4_4,    // kARGB4_4_4_4
  (Uint) -1,                    // kARGB8_3_3_2
  GL_UNSIGNED_SHORT_1_5_5_5_REV,// kARGB1_5_5_5
  GL_UNSIGNED_BYTE,             // kARGB8_8_8_8
  (Uint) -1,                    // kARGB10_10_10_2
  (Uint) -1,                    // kkARGB12_12_12_12
  (Uint) -1,                    // kARGB16_16_16_16
  (Uint) -1,                    // kABGR2_2_2_2
  (Uint) -1,                    // kABGR4_4_4_4
  (Uint) -1,                    // kABGR8_2_3_3
  (Uint) -1,                    // kABGR1_5_5_5
  (Uint) -1,                    // kABGR8_8_8_8
  (Uint) -1,                    // kABGR2_10_10_10
  (Uint) -1,                    // kABGR12_12_12_12
  (Uint) -1,                    // kABGR16_16_16_16
  (Uint) -1,                    // kAYIQ8_4_2_2
  GL_UNSIGNED_SHORT,            // kDepth16
  GL_UNSIGNED_INT,              // kDepth32
  GL_UNSIGNED_BYTE,             // kStencil
  GL_UNSIGNED_INT_24_8_NV       // kDepthStencil24_8
};

/*! A map from format to number of components */
GLenum Image::s_format_components[] = {
  (Uint) -1,                    // kIllegal
  (Uint) -1,                    // kFormat1
  (Uint) -1,                    // kFormat2
  (Uint) -1,                    // kFormat4
  (Uint) -1,                    // kFormat8
  (Uint) -1,                    // kFormat12
  (Uint) -1,                    // kFormat16
  (Uint) -1,                    // kFormat32
  (Uint) -1,                    // kPalette1
  (Uint) -1,                    // kPalette2
  (Uint) -1,                    // kPalette4
  (Uint) -1,                    // kPalette8
  (Uint) -1,                    // kPalette12
  (Uint) -1,                    // kPalette16
  1,                            // kAlpha4
  1,                            // kAlpha8, works better than GL_ALPHA8
  1,                            // kAlpha12
  1,                            // kAlpha16
  1,                            // kLuminance4
  1,                            // kLuminance8, works better than GL_LUMINANCE8
  1,                            // kLuminance12
  1,                            // kLuminance16
  1,                            // kIntensity4
  1,                            // kIntensity8
  1,                            // kIntensity12
  1,                            // kIntensity16
  (Uint) -1,                    // kAlphaPalette8_8
  2,                            // kLuminanceAlpha4_4
  2,                            // kLuminanceAlpha6_2
  2,                            // kLuminanceAlpha8_8, GL_LUMINANCE8_ALPHA8
  2,                            // kLuminanceAlpha12_4
  2,                            // kLuminanceAlpha12_12
  2,                            // kLuminanceAlpha16_16
  (Uint) -1,                    // kAlphaLuminance4_4
  (Uint) -1,                    // kAlphaLuminance8_8
  3,                            // kRGB3_3_2
  3,                            // kRGB4_4_4
  3,                            // kRGB5_5_5
  3,                            // kRGB5_6_5
  3,                            // kRGB8_8_8, works better than GL_RGB8
  3,                            // kRGB10_10_10
  3,                            // kRGB12_12_12
  3,                            // kRGB16_16_16
  3,                            // BGR2_3_3
  3,                            // BGR4_4_4
  3,                            // BGR5_5_5
  3,                            // BGR5_6_5
  3,                            // BGR8_8_8
  3,                            // BGR10_10_10
  3,                            // BGR12_12_12
  3,                            // BGR16_16_16
  (Uint) -1,                    // kYIQ4_2_2
  4,                            // kRGBA2_2_2_2
  4,                            // kRGBA4_4_4_4
  4,                            // kRGBA3_3_2_8
  4,                            // kRGBA5_5_5_1
  4,                            // kRGBA8_8_8_8, works better than GL_RGBA8
  4,                            // kRGBA10_10_10_2
  4,                            // kRGBA12_12_12_12
  4,                            // kRGBA16_16_16_16
  (Uint) -1,                    // kARGB2_2_2_2
  4,                            // kARGB4_4_4_4
  (Uint) -1,                    // kARGB8_3_3_2
  (Uint) -1,                    // kARGB1_5_5_5
  (Uint) -1,                    // kARGB8_8_8_8
  (Uint) -1,                    // kARGB10_10_10_2
  (Uint) -1,                    // kkARGB12_12_12_12
  (Uint) -1,                    // kARGB16_16_16_16
  (Uint) -1,                    // kABGR2_2_2_2
  (Uint) -1,                    // kABGR4_4_4_4
  (Uint) -1,                    // kABGR8_2_3_3
  (Uint) -1,                    // kABGR1_5_5_5
  (Uint) -1,                    // kABGR8_8_8_8
  (Uint) -1,                    // kABGR2_10_10_10
  (Uint) -1,                    // kABGR12_12_12_12
  (Uint) -1,                    // kABGR16_16_16_16
  (Uint) -1,                    // kAYIQ8_4_2_2
  1,                            // kDepth16
  1,                            // kDepth32
  1,                            // kStencil
  1                             // kDepthStencil24_8
};

/*! A map from format to format names */
const char* Image::s_format_names[] = {
  "Illegal",
  "Format1",
  "Format2",
  "Format4",
  "Format8",
  "Format12",
  "Format16",
  "Format32",
  "Palette1",
  "Palette2",
  "Palette4",
  "Palette8",
  "Palette12",
  "Palette16",
  "Alpha4",
  "Alpha8",
  "Alpha12",
  "Alpha16",
  "Luminance4",
  "Luminance8",
  "Luminance12",
  "Luminance16",
  "Intensity4",
  "Intensity8",
  "Intensity12",
  "Intensity16",
  "AlphaPalette8_8",
  "LuminanceAlpha4_4",
  "LuminanceAlpha6_2",
  "LuminanceAlpha8_8",
  "LuminanceAlpha12_4",
  "LuminanceAlpha12_12",
  "LuminanceAlpha16_16",
  "AlphaLuminance4_4",
  "AlphaLuminance8_8",
  "RGB3_3_2",
  "RGB4_4_4",
  "RGB5_5_5",
  "RGB5_6_5",
  "RGB8_8_8",
  "RGB10_10_10",
  "RGB12_12_12",
  "RGB16_16_16",
  "BGR2_3_3",
  "BGR4_4_4",
  "BGR5_5_5",
  "BGR5_6_5",
  "BGR8_8_8",
  "BGR10_10_10",
  "BGR12_12_12",
  "BGR16_16_16",
  "YIQ4_2_2",
  "RGBA2_2_2_2",
  "RGBA4_4_4_4",
  "RGBA3_3_2_8",
  "RGBA5_5_5_1",
  "RGBA8_8_8_8",
  "RGBA10_10_10_2",
  "RGBA12_12_12_12",
  "RGBA16_16_16_16",
  "ARGB2_2_2_2",
  "ARGB4_4_4_4",
  "ARGB8_3_3_2",
  "ARGB1_5_5_5",
  "ARGB8_8_8_8",
  "ARGB10_10_10_2",
  "kARGB12_12_12_12",
  "ARGB16_16_16_16",
  "ABGR2_2_2_2",
  "ABGR4_4_4_4",
  "ABGR8_2_3_3",
  "ABGR1_5_5_5",
  "ABGR8_8_8_8",
  "ABGR2_10_10_10",
  "ABGR12_12_12_12",
  "ABGR16_16_16_16",
  "AYIQ8_4_2_2",
  "Depth16",
  "Depth32",
  "Stencil",
  "DepthStencil24_8"
};

// Default values
const Uint Image::s_def_width(0);
const Uint Image::s_def_height(0);
const Image::Format Image::s_def_format(Image::kRGB8_8_8);

//! \brief constructor
Image::Image(Boolean proto) :
  Container(proto),
  m_width(s_def_width),
  m_height(s_def_height),
  m_format(s_def_format),
  m_pixels(nullptr),
  m_pack_row_length(0),
  m_dirty(true),
  m_owned_pixels(false),
  m_size(0)
{}

//! \brief destructor
Image::~Image()
{
  if (m_owned_pixels) {
    if (m_pixels) {
      delete [] (char*) m_pixels;
      m_pixels = nullptr;
    }
    m_owned_pixels = false;
    m_size = 0;
  }
}

//! \brief initializess the node prototype.
void Image::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto();

  Uint_handle_function width_func =
    static_cast<Uint_handle_function>(&Image::width_handle);
  s_prototype->add_field_info(new SF_uint(WIDTH, "width",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          width_func,
                                          s_def_width));

  Uint_handle_function height_func =
    static_cast<Uint_handle_function>(&Image::height_handle);
  s_prototype->add_field_info(new SF_uint(HEIGHT, "height",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          height_func,
                                          s_def_height));

  Uint_handle_function format_func =
    reinterpret_cast<Uint_handle_function>(&Image::format_handle);
  s_prototype->add_field_info(new SF_uint(FORMAT, "format",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          format_func,
                                          s_def_format));
}

//! \brief deletes the prototype.
void Image::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Image::get_prototype()
{
  if (!s_prototype) Image::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the image.
void Image::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "format") {
      const auto** found =
        std::find(s_format_names, &s_format_names[kNumFormats], value);
      auto index = found - s_format_names;
      if (index >= kNumFormats) {
        std::cerr << "Illegal format name (" << value.c_str() << ")!"
                  << std::endl;
      }
      m_format = static_cast<Format>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "width") {
      set_width(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "height") {
      set_height(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief draws the image.
void Image::draw()
{
  if ((m_format <= kIllegal) || (m_format >= kNumFormats)) {
    std::cerr << "Illegal image format " << m_format << "!"
              << std::endl;
    return;
  }

  if ((m_format ==  kDepth16) ||
      (m_format ==  kDepth32) ||
      (m_format ==  kDepthStencil24_8))
  {
    std::cerr << "Image of Format " << m_format << " cannot be drawn!"
              << std::endl;
    return;
  }

  if (m_pack_row_length != 0)
    glPixelStorei(GL_PACK_ROW_LENGTH, m_pack_row_length);
  glDrawPixels(m_width, m_height,
               s_format_formats[m_format], s_format_types[m_format],
               (GLvoid *) m_pixels);
}

//! \brief obtains the memory that is used by the image (in bytes).
Uint Image::get_size() const
{
  if (m_pack_row_length == 0) return get_size(m_width, m_height, m_format);
  else return m_pack_row_length * m_height;
}

//! \brief allocates space for the image.
void Image::allocate_space()
{
  auto width = get_width();
  auto height = get_height();
  auto format = get_format();

  if ((width == 0) || (height == 0)) return;
  Uint size = Image::get_size(width, height, format);
  if (size <= m_size) return;

  auto* pixels = (Uchar*) get_pixels();
  if (pixels && m_owned_pixels) delete [] pixels;
  m_pixels = new Uchar[size];
  m_owned_pixels = true;
  SGAL_assertion(m_pixels);
  m_size = size;
}

//! \brief determines whether the image is empty.
Boolean Image::empty()
{
  return ((get_width() == 0) || (get_height() == 0) || (get_pixels() == nullptr));
}

//! \brief obtain the texture number of components.
Uint Image::get_component_count()
{ return Image::get_format_components(get_format()); }

//! \brief sets the image width.
void Image::set_width(Uint width)
{
  m_width = width;
  m_dirty = true;
}

//! \brief obtains the image width.
Uint Image::get_width()
{
  if (m_dirty) clean();
  return m_width;
}

//! \brief sets the image height.
void Image::set_height(Uint height)
{
  m_height = height;
  m_dirty = true;
}

//! \brief obtains the image height.
Uint Image::get_height()
{
  if (m_dirty) clean();
  return m_height;
}

//! \brief sets the image format.
void Image::set_format(Format format)
{
  m_format = format;
  m_dirty = true;
}

//! \brief obtains the image format.
Image::Format Image::get_format()
{
  if (m_dirty) clean();
  return m_format;
}

//! \brief sets the image pixel data.
void Image::set_pixels(void* pixels)
{
  if (m_owned_pixels) {
    if (m_pixels) delete [] (char*) m_pixels;
    m_owned_pixels = false;
  }
  m_pixels = pixels;
  m_size = 0;
  m_dirty = false;
}

//! \brief obtains the image pixel data.
void* Image::get_pixels()
{
  if (m_dirty) clean();
  return m_pixels;
}

//! \brief obtains the openGl internal format.
// Apparently, when compiling with VC, the symbol of this member is
// unresolved if made inline, so keep it here.
GLenum Image::get_format_internal_format(Format format)
{ return s_format_internal_formats[format]; }

SGAL_END_NAMESPACE
