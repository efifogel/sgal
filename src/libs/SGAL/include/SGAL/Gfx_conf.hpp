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

#ifndef SGAL_GFX_CONF_HPP
#define SGAL_GFX_CONF_HPP

/*! \file
 *
 */

#if (defined _MSC_VER)
#include <windows.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#else
#include <GL/gl.h>
#endif
#include <GL/glext.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
//! \todo #include "Singleton_destroyer.h"

#ifndef GL_GLEXT_PROTOTYPES

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

/* ARB_multitexture command function pointers */
SGAL_SGAL_DECL extern PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB;
SGAL_SGAL_DECL extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
SGAL_SGAL_DECL extern PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB;
SGAL_SGAL_DECL extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

/* NV_register_combiners command function pointers */
SGAL_SGAL_DECL extern PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;
SGAL_SGAL_DECL extern PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV;
SGAL_SGAL_DECL extern PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV;
SGAL_SGAL_DECL extern PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV;
SGAL_SGAL_DECL extern PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
SGAL_SGAL_DECL extern PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
SGAL_SGAL_DECL extern PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
SGAL_SGAL_DECL extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC
  glGetCombinerInputParameterfvNV;
SGAL_SGAL_DECL extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC
  glGetCombinerInputParameterivNV;
SGAL_SGAL_DECL extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC
  glGetCombinerOutputParameterfvNV;
SGAL_SGAL_DECL extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC
  glGetCombinerOutputParameterivNV;
SGAL_SGAL_DECL extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC
  glGetFinalCombinerInputfvNV;
SGAL_SGAL_DECL extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC
  glGetFinalCombinerInputivNV;

/* WGL_EXT_swap_control command function pointers */
// Not supported in glext.h:
// extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
// extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

// ARB_vertex_buffer_object
SGAL_SGAL_DECL extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
SGAL_SGAL_DECL extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
SGAL_SGAL_DECL extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
SGAL_SGAL_DECL extern PFNGLISBUFFERARBPROC glIsBufferARB;
SGAL_SGAL_DECL extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
SGAL_SGAL_DECL extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
SGAL_SGAL_DECL extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
SGAL_SGAL_DECL extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
SGAL_SGAL_DECL extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
SGAL_SGAL_DECL extern PFNGLGETBUFFERPARAMETERIVARBPROC
  glGetBufferParameterivARB;
SGAL_SGAL_DECL extern PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;

// Framebuffer object
SGAL_SGAL_DECL extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
SGAL_SGAL_DECL extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
SGAL_SGAL_DECL extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
SGAL_SGAL_DECL extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
SGAL_SGAL_DECL extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC
  glGetFramebufferAttachmentParameteriv;
SGAL_SGAL_DECL extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
SGAL_SGAL_DECL extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
SGAL_SGAL_DECL extern PFNGLFRAMEBUFFERRENDERBUFFERPROC
  glFramebufferRenderbuffer;
SGAL_SGAL_DECL extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
SGAL_SGAL_DECL extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
SGAL_SGAL_DECL extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
SGAL_SGAL_DECL extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
SGAL_SGAL_DECL extern PFNGLGETRENDERBUFFERPARAMETERIVPROC
  glGetRenderbufferParameteriv;
SGAL_SGAL_DECL extern PFNGLISRENDERBUFFERPROC glIsRenderbuffer;

// wgl extensions:
SGAL_SGAL_DECL extern PROC wglGetExtString;
SGAL_SGAL_DECL extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
SGAL_SGAL_DECL extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC
  wglGetPixelFormatAttribivARB;

#ifdef __cplusplus
}
#endif

#endif

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Gfx_conf {
public:
  // !!!! NOTE !!!!
  // If you add any new entries the one of the follwoing enums, make sure you
  // update the arrays in Gfx_conf.cpp. The enums and the arrays must be
  // syncronized.
  // !!!! NOTE !!!!
  enum Vendor {
    veUnknown = 0,
    veNVIDIA,
    veATI,
    ve3Dfx,
    veMatrox,
    veMicrosoft,
    veNum
  };

  enum Renderer {
    reUnknown = 0,
    reGEFORCE2,
    reGEFORCE4,
    reGEFORCE_6200,
    reGEFORCE_7600,
    reRIVATNT2,
    reRAGE,
    reRADEON9700,
    reRADEON_IGP,
    reMOBILITY_RADEON_X300,
    reVoodoo3,
    reG400,
    reG200,
    reGeneric,
    reNum
  };

  /*! Obtain the pointer to the singleton instance */
  static Gfx_conf* get_instance();

  /*! Destructor */
  ~Gfx_conf();

  /*! Initialize the structure. First, Obtain the following:
   * 1. The name of the vendor of the GPU (renderer)
   * 2. The name of the renderer (GPU)
   * 3. The version number of the OpenGL driver
   * 4. The string that contains the name of all extensions supported by the
   * configuration of renderer and the OpenGL driver
   * Then, process the extension list and update the boolean flags that
   * indicate whether specific extensions are supported or not.
   * Finally, For OpenGl on Windows, obtain the address of OpenGL extension
   * functions for use with the current OpenGL rendering context.
   */
  void init();

  /*! Obtain the vendor name.
   * \return the vendor enumeration.
   */
  Vendor get_vendor() const;

  /*! Obtain the renderer (GPU) name.
   * \return the renderer enumeration
   */
  Renderer get_renderer() const;

  /*! Obtain the vendor name
   * \return the vendor name.
   */
  const char* get_vendor_name() const;

  /*! Obtain the renderer name.
   * \return the renderer name
   */
  const char* get_renderer_name() const;

  /*! Is bump_map_ extension supported?
   * \return true iff bump_map_ extension is supported.
   */
  inline Boolean is_bump_map_supported() const;

  /*! Is vertex-buffer extension supported?
   * \return true iff vertex-buffer extension is supported.
   */
  inline Boolean is_vertex_buffer_object_supported() const;

  /*! Is packed_depth_stencil extension supported?
   * \return true iff packed_depth_stencil extension is supported.
   */
  inline Boolean is_packed_depth_stencil_supported() const;

  /*! Is multisample extension supported?
   * \return true iff multisample extension is supported.
   */
  inline Boolean is_multisample_supported() const;

  /*! Determine whether seamless cube map is supported. */
  inline Boolean is_seamless_cube_map_supported() const;

  /*! Disable vertex-buffer support */
  void disable_vertex_buffer_object_support();

private:
  /*! The vendor of the graphics-acceleration system */
  Vendor m_vendor;

  /*! The renderer (card) of the graphics-acceleration system */
  Renderer m_renderer;

  /*! Indicates whether bump mapping is supported */
  Boolean m_bump_map_supported;

  /*! Indicates whether vertex buffer object is supported */
  Boolean m_vertex_buffer_object_supported;

  /*! Indicates whether packed depth-stencil is supported */
  Boolean m_packed_depth_stencil_supported;

  /*! Indicates whether multisample is supported */
  Boolean m_multisample_supported;

  /*! Indicates whether seamless cube map is supported. */
  Boolean m_seamless_cube_map_supported;

  /*! The instance of the Graphics configuration record. */
  static Gfx_conf* s_instance;

  /*! The vendor names */
  static const char* s_vendors[];

  /*! The renderer names */
  static const char* s_renderers[];

  /*! The OpenGL versions */
  static const char* s_versions[];

  // this is related to singleton distruction
  //! \todo typedef Singleton_destroyer<Gfx_conf> Gfx_conf_destroyer;
  //! \todo static Gfx_conf_destroyer m_gfx_conf_destroyer;

  //! \todo friend class Singleton_destroyer<Gfx_conf>;

  /*! Constructor */
  Gfx_conf();

  /*! Is the the extension given by a name supported? */
  Boolean is_extension_supported(const Uchar* extensions,
                                 const char* target_extension);
};

/*! \brief returns true iff bump_map_ extension is supported */
inline Boolean Gfx_conf::is_bump_map_supported() const
{ return m_bump_map_supported; }

/*! \brief returns true iff vertex-buffer extension is supported */
inline Boolean Gfx_conf::is_vertex_buffer_object_supported() const
{ return m_vertex_buffer_object_supported; }

/* \brief returns true iff packed_depth_stencil extension is supported */
inline Boolean Gfx_conf::is_packed_depth_stencil_supported() const
{ return m_packed_depth_stencil_supported; }

/* \brief returns true iff multisample extension is supported */
inline Boolean Gfx_conf::is_multisample_supported() const
{ return m_multisample_supported; }

/*! \brief disables vertex-buffer support */
inline void Gfx_conf::disable_vertex_buffer_object_support()
{ m_vertex_buffer_object_supported = false; }

inline Gfx_conf::Vendor Gfx_conf::get_vendor() const { return m_vendor; }
inline Gfx_conf::Renderer Gfx_conf::get_renderer() const { return m_renderer; }

inline const char* Gfx_conf::get_vendor_name() const
{ return s_vendors[m_vendor]; }

inline const char* Gfx_conf::get_renderer_name() const
{ return s_renderers[m_renderer]; }

/*! \brief determines whether seamless cube map is supported. */
inline Boolean Gfx_conf::is_seamless_cube_map_supported() const
{ return m_seamless_cube_map_supported; }

SGAL_END_NAMESPACE

#endif
