// Copyright (c) 2004  Israel.
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
// $Source: $
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <stdio.h>
#include <string.h>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Trace.hpp"

#ifndef GL_GLEXT_PROTOTYPES

// ARB_vertex_buffer_object
PFNGLBINDBUFFERARBPROC              glBindBufferARB              = NULL;
PFNGLDELETEBUFFERSARBPROC           glDeleteBuffersARB           = NULL;
PFNGLGENBUFFERSARBPROC              glGenBuffersARB              = NULL;
PFNGLISBUFFERARBPROC                glIsBufferARB                = NULL;
PFNGLBUFFERDATAARBPROC              glBufferDataARB              = NULL;
PFNGLBUFFERSUBDATAARBPROC           glBufferSubDataARB           = NULL;
PFNGLGETBUFFERSUBDATAARBPROC        glGetBufferSubDataARB        = NULL;
PFNGLMAPBUFFERARBPROC               glMapBufferARB               = NULL;
PFNGLUNMAPBUFFERARBPROC             glUnmapBufferARB             = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC    glGetBufferParameterivARB    = NULL;
PFNGLGETBUFFERPOINTERVARBPROC       glGetBufferPointervARB       = NULL;

PROC                                wglGetExtString              = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB      = NULL;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;

#endif

SGAL_BEGIN_NAMESPACE

Gfx_conf* Gfx_conf::m_instance = 0;
//! \todo Gfx_conf::Gfx_Conf_destroyer Gfx_conf::m_Gfx_conf_destroyer;

// !!!! NOTE !!!!
// If you add any new entries the one of the follwoing arrays, make sure you update the 
// enums in the Gfx_conf class. The enums and the arrays must be syncronized.
// !!!! NOTE !!!!

const char* Gfx_conf::s_vendors[] = {
  "Unknown",
  "NVIDIA Corporation",
  "ATI",
  "3Dfx",
  "MATROX",
  "Microsoft Corporation"
};

const char* Gfx_conf::s_renderers[] = {
  "Unknown",
  "GeForce2 GTS/AGP/SSE",
  "GeForce4 420 Go/AGP/SSE2",
  "GeForce 6200 LE/PCI/SSE2",
  "GeForce 7600 GT/PCI/SSE2/3DNOW",
  "RIVA TNT2",
  "RAGE 128 A21 AGP 2x x86/SSE",
  "RADEON 9700 PRO x86/SSE2",
  "RADEON IGP 340M DDR x86/SSE2",
  "MOBILITY RADEON X300 x86/SSE2", 
  "3Dfx/Voodoo3 (tm)/2 TMUs/16 MB",
  "Matrox G400",
  "Matrox G200",
  "GDI Generic",
};

const char* Gfx_conf::s_versions[] = {
  "Unknown",
  "1.2.0",
  "1.1.1334 Win2000 Release",
  "1.1.0",
  "1.1.3",
  "1.1.0"
};

Gfx_conf* Gfx_conf::get_instance() 
{
  if (!m_instance) {
    m_instance = new Gfx_conf();
    //! \todo m_Gfx_confDestroyer.SetSingleton(m_instance);
  }
  return m_instance;
}

Gfx_conf::Gfx_conf() :
  m_vendor(veUnknown), 
  m_renderer(reUnknown), 
  m_bump_map_supported(true),
  m_vertex_buffer_object_supported(true),
  m_packed_depth_stencil_supported(true),
  m_multisample_supported(true),
  m_seamless_cube_map_supported(true)
{ init(); }

/*! Destructor */
Gfx_conf::~Gfx_conf() {}

void Gfx_conf::init()
{
  int i;
    
  const Uchar* vendor = glGetString(GL_VENDOR);
  for (i = 0; i < veNum; ++i) {
    if (strncmp(s_vendors[i], reinterpret_cast<const char *>(vendor),
                strlen(s_vendors[i])) == 0)
      break;
  }
  if (i != veNum) m_vendor = static_cast<Vendor>(i);  
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "Vendor: " << vendor << " (" << m_vendor << ")"
                  << std::endl;);
  const Uchar* renderer = glGetString(GL_RENDERER);
  for (i = 0; i < reNum; ++i) {
    if (strncmp(s_renderers[i], reinterpret_cast<const char *>(renderer),
                strlen(s_renderers[i])) == 0)
      break;
  }
  if (i != reNum) m_renderer = static_cast<Renderer>(i);  
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "Renderer: " << renderer << " ("
                  << m_renderer << ")"
                  << std::endl;);

  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  const Uchar* version = glGetString(GL_VERSION);
                  std::cout << "Version: " << version << std::endl;);

#if 0
  char* dummy;
  search_path(NULL, "opengl32.dll", NULL, 1000, temp, &dummy);
#endif

  const Uchar* extensions = glGetString(GL_EXTENSIONS);
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "Extensions: " << extensions
                  << std::endl;);

  m_bump_map_supported = true;          // Start optimistic

  if (!is_extension_supported(extensions, "GL_EXT_bgra"))
    m_bump_map_supported = false;
  else if (!is_extension_supported(extensions, "GL_ARB_multitexture"))
    m_bump_map_supported = false;
  else if (!is_extension_supported(extensions, "GL_NV_register_combiners"))
    m_bump_map_supported = false;
  else if (!is_extension_supported(extensions, "GL_EXT_texture_cube_map"))
    m_bump_map_supported = false;
  else if (!is_extension_supported(extensions, "WGL_EXT_swap_control"))
    m_bump_map_supported = false;

  m_vertex_buffer_object_supported =
    is_extension_supported(extensions, "GL_ARB_vertex_buffer_object");

  m_packed_depth_stencil_supported =
    is_extension_supported(extensions, "GL_NV_packed_depth_stencil");

  m_multisample_supported =
    is_extension_supported(extensions, "GL_ARB_multisample");

  m_seamless_cube_map_supported =
    is_extension_supported(extensions, "GL_ARB_seamless_cube_map");
  
#if defined(_WIN32)
  glBindBufferARB =
    (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
  glDeleteBuffersARB =
    (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
  glGenBuffersARB =
    (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
  glIsBufferARB =
    (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");
  glBufferDataARB =
    (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
  glBufferSubDataARB =
    (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
  glGetBufferSubDataARB =
    (PFNGLGETBUFFERSUBDATAARBPROC)wglGetProcAddress("glGetBufferSubDataARB");
  glMapBufferARB =
    (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
  glUnmapBufferARB =
    (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
  glGetBufferParameterivARB =
    (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
  glGetBufferPointervARB =
    (PFNGLGETBUFFERPOINTERVARBPROC)wglGetProcAddress("glGetBufferPointervARB");
    
  // Try To Use wglGetExtensionStringARB On Current DC, If Possible
  PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
  const Uchar* wgl_extensions =
    ((Uchar*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

  if (!m_multisample_supported) {
    if (is_extension_supported(wgl_extensions, "WGL_ARB_multisample"))
      m_multisample_supported = true;
  }
  if (m_multisample_supported) {
    wglChoosePixelFormatARB =
      (PFNWGLCHOOSEPIXELFORMATARBPROC)
      wglGetProcAddress("wglChoosePixelFormatARB");
    wglGetPixelFormatAttribivARB =
      (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
      wglGetProcAddress("wglGetPixelFormatAttribivARB");
    if (!wglChoosePixelFormatARB) m_multisample_supported = false;
  }
  std::cout << "m_multisample_supported: "
            << m_multisample_supported << std::endl;

  if (!glBindBufferARB || !glDeleteBuffersARB || !glGenBuffersARB || 
      !glIsBufferARB || !glBufferDataARB || !glBufferSubDataARB || 
      !glGetBufferSubDataARB || !glMapBufferARB || !glUnmapBufferARB || 
      !glGetBufferParameterivARB || !glGetBufferPointervARB)
    m_vertex_buffer_object_supported = false;

  std::cout << "m_vertex_buffer_object_supported: "
            << m_vertex_buffer_object_supported << std::endl;
#endif
}

/*! Is the the extension given by a name supported? */
Boolean Gfx_conf::is_extension_supported(const Uchar* extensions,
                                         const char* target_extension)
{
  // Extension names should not have spaces
  const Uchar* where = (Uchar*) strchr(target_extension, ' ');
  if (where || *target_extension == '\0') return false;

  // Search The Extensions String For An Exact Copy
  const Uchar* start = extensions;
  for (;;) {
    where = (Uchar *) strstr((const char *) start, target_extension);
    if (!where) break;
    const Uchar* terminator = where + strlen(target_extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0') return true;
    start = terminator;
  }
  return false;
}

SGAL_END_NAMESPACE
