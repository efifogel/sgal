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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GL_WRAP_HPP
#define SGAL_GL_WRAP_HPP

#if defined(_WIN32)
#include <windows.h>
#endif
#include <map>
#include <GL/gl.h>
#include <GL/glext.h>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Gl_wrapper {
private:
  typedef std::map<GLenum, const char*> Name_map;
  typedef Name_map::const_iterator      Name_const_iter;
  
  /*! The singletone */
  static Gl_wrapper* s_instance;

  /*! The search data structure */
  Name_map m_gl_token_names;

  /*! Private Constructor */
  Gl_wrapper();

  /*! Find the name of a token */
  const char* find_name(GLenum num);

public:
  /*! Obtain the singletone */
  static Gl_wrapper* get_instance();

  /*! Destructor */
  ~Gl_wrapper();

  /*! Find the name of a token */
  static const char* find(GLenum num)
  { return get_instance()->find_name(num); }
};

// Free wrapper functions:

/*! glAccum wrapper */
inline void glAccum(GLenum op, GLfloat value) {
  ::glAccum(op, value);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glAccum(" << Gl_wrapper::find(op)
             << value << ");"
             << std::endl;);
}

/*! glAlphaFunc wrapper */
inline void glAlphaFunc(GLenum func, GLclampf ref) {
  ::glAlphaFunc(func, ref);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glAlphaFunc("
             << Gl_wrapper::find(func) << ", " << ref << ");"
             << std::endl;);
}

/*! glAreTexturesResident wrapper */
inline GLboolean glAreTexturesResident(GLsizei n, const GLuint* textures,
                                    GLboolean* residences) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glAreTexturesResident(" << n
             << ");"
             << std::endl;);
  return ::glAreTexturesResident(n, textures, residences);
}

/*! glArrayElement wrapper */
inline void glArrayElement(GLint i) {
  ::glArrayElement(i);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glArrayElement(" << i << ");"
             << std::endl;);
}

/*! glBegin wrapper */
inline void glBegin(GLenum mode) {
  ::glBegin(mode);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glBegin(" << Gl_wrapper::find(mode) << ");"
             << std::endl;);
}

/*! glBindTexture wrapper */
inline void glBindTexture(GLenum target, GLuint texture) {
  ::glBindTexture(target,texture );
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glBindTexture("
             << Gl_wrapper::find(target) << ", " << texture << ");"
             << std::endl;);
}

/*! glBitmap wrapper */
inline void glBitmap(GLsizei width, GLsizei height, GLfloat xorig,
                     GLfloat yorig, GLfloat xmove, GLfloat ymove,
                     const GLubyte* bitmap) {
  ::glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glBitmap("
             << width << ", " << height << ", " << xorig << ", " << yorig
             << ", " << xmove << ", " << ymove << ");"
             << std::endl;);
}

/*! glBlendFunc wrapper */
inline void glBlendFunc(GLenum sfactor, GLenum dfactor) {
  ::glBlendFunc(sfactor, dfactor);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glBlendFunc("
             << Gl_wrapper::find(sfactor) << Gl_wrapper::find(dfactor) << ");"
             << std::endl;);
}

/*! glCallList wrapper */
inline void glCallList(GLuint list) {
  ::glCallList(list);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCallList(" << list << ");"
             << std::endl;);
}

/*! glCallLists wrapper */
inline void glCallLists(GLsizei n, GLenum type, const GLvoid* lists) {
  ::glCallLists(n, type, lists);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCallLists(" << n << ", " 
             << Gl_wrapper::find(type) << ");"
             << std::endl;);
}

/*! glClear wrapper */
inline void glClear(GLbitfield mask) {
  ::glClear(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClear(" << std::hex << mask
             << ");"
             << std::endl;);
}

/*! glClearAccum wrapper */
inline void glClearAccum(GLfloat red, GLfloat green, GLfloat blue,
                         GLfloat alpha) {
  ::glClearAccum(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearAccum("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glClearColor wrapper */
inline void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha) {
  ::glClearColor(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearColor("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glClearDepth wrapper */
inline void glClearDepth(GLclampd depth) {
  ::glClearDepth(depth);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearDepth(" << depth << ");"
             << std::endl;);
}

/*! glClearIndex wrapper */
inline void glClearIndex(GLfloat c) {
  ::glClearIndex(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearIndex(" << c << ");"
             << std::endl;);
}

/*! glClearStencil wrapper */
inline void glClearStencil(GLint s) {
  ::glClearStencil(s);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearStencil(" << s << ");"
             << std::endl;);
}

/*! glClipPlane wrapper */
inline void glClipPlane(GLenum plane, const GLdouble* equation) {
  ::glClipPlane(plane, equation);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glClipPlane("
             << Gl_wrapper::find(plane) << ");"
             << std::endl;);
}

/*! glColor3b wrapper */
inline void glColor3b(GLbyte red, GLbyte green, GLbyte blue) {
  ::glColor3b(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3b("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3bv wrapper */
inline void glColor3bv(const GLbyte* v) {
  ::glColor3bv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3bv(" << ");"
             << std::endl;);
}

/*! glColor3d wrapper */
inline void glColor3d(GLdouble red, GLdouble green, GLdouble blue) {
  ::glColor3d(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3d("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3dv wrapper */
inline void glColor3dv(const GLdouble* v) {
  ::glColor3dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3dv(" << ");"
             << std::endl;);
}

/*! glColor3f wrapper */
inline void glColor3f(GLfloat red, GLfloat green, GLfloat blue) {
  ::glColor3f(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3f(" 
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! color wrapper */
inline void glColor3fv(const GLfloat* v) {
  ::glColor3fv(v);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glColor3fv(" << *((Vector3f *) v) << ");"
             << std::endl;);
}

/*! glColor3i wrapper */
inline void glColor3i(GLint red, GLint green, GLint blue) {
  ::glColor3i(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3i("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3iv wrapper */
inline void glColor3iv(const GLint* v) {
  ::glColor3iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3iv(" << ");"
             << std::endl;);
}

/*! glColor3s wrapper */
inline void glColor3s(GLshort red, GLshort green, GLshort blue) {
  ::glColor3s(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3s("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3sv wrapper */
inline void glColor3sv(const GLshort* v) {
  ::glColor3sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3sv(" << ");"
             << std::endl;);
}

/*! glColor3ub wrapper */
inline void glColor3ub(GLubyte red, GLubyte green, GLubyte blue) {
  ::glColor3ub(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3ub("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3ubv wrapper */
inline void glColor3ubv(const GLubyte* v) {
  ::glColor3ubv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3ubv(" << ");"
             << std::endl;);
}

/*! glColor3ui wrapper */
inline void glColor3ui(GLuint red, GLuint green, GLuint blue) {
  ::glColor3ui(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3ui("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3uiv wrapper */
inline void glColor3uiv(const GLuint* v) {
  ::glColor3uiv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3uiv(" << ");"
             << std::endl;);
}

/*! glColor3us wrapper */
inline void glColor3us(GLushort red, GLushort green, GLushort blue) {
  ::glColor3us(red, green, blue);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3us("
             << red << ", " << green << ", " << blue << ");"
             << std::endl;);
}

/*! glColor3usv wrapper */
inline void glColor3usv(const GLushort* v) {
  ::glColor3usv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3usv(" << ");"
             << std::endl;);
}

/*! glColor4b wrapper */
inline void glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha) {
  ::glColor4b(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4b("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4bv wrapper */
inline void glColor4bv(const GLbyte* v) {
  ::glColor4bv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4bv(" << ");"
             << std::endl;);
}

/*! glColor4d wrapper */
inline void glColor4d(GLdouble red, GLdouble green, GLdouble blue,
                      GLdouble alpha) {
  ::glColor4d(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4d("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4dv wrapper */
inline void glColor4dv(const GLdouble* v) {
  ::glColor4dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4dv(" << ");"
             << std::endl;);
}

/*! glColor4f wrapper */
inline void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  ::glColor4f(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4f("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4fv wrapper */
inline void glColor4fv(const GLfloat* v) {
  ::glColor4fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4fv(" << ");"
             << std::endl;);
}

/*! glColor4i wrapper */
inline void glColor4i(GLint red, GLint green, GLint blue, GLint alpha) {
  ::glColor4i(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4i("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4iv wrapper */
inline void glColor4iv(const GLint* v) {
  ::glColor4iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4iv(" << ");"
             << std::endl;);
}

/*! glColor4s wrapper */
inline void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
  ::glColor4s(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4s("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4sv wrapper */
inline void glColor4sv(const GLshort* v) {
  ::glColor4sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4sv(" << ");"
             << std::endl;);
}

/*! glColor4ub wrapper */
inline void glColor4ub(GLubyte red, GLubyte green, GLubyte blue,
                       GLubyte alpha) {
  ::glColor4ub(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4ub("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4ubv wrapper */
inline void glColor4ubv(const GLubyte* v) {
  ::glColor4ubv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4ubv(" << ");"
             << std::endl;);
}

/*! glColor4ui wrapper */
inline void glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha) {
  ::glColor4ui(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4ui("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4uiv wrapper */
inline void glColor4uiv(const GLuint* v) {
  ::glColor4uiv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4uiv(" << ");"
             << std::endl;);
}

/*! glColor4us wrapper */
inline void glColor4us(GLushort red, GLushort green, GLushort blue,
                       GLushort alpha) {
  ::glColor4us(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4us("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColor4usv wrapper */
inline void glColor4usv(const GLushort* v) {
  ::glColor4usv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4usv(" << ");"
             << std::endl;);
}

/*! glColorMask wrapper */
inline void glColorMask(GLboolean red, GLboolean green, GLboolean blue,
                        GLboolean alpha) {
  ::glColorMask(red, green, blue, alpha);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColorMask("
             << red << ", " << green << ", " << blue << ", " << alpha << ");"
             << std::endl;);
}

/*! glColorMaterial wrapper */
inline void glColorMaterial(GLenum face, GLenum mode) {
  ::glColorMaterial(face, mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColorMaterial(" << ");"
             << std::endl;);
}

/*! glColorPointer wrapper */
inline void glColorPointer(GLint size, GLenum type, GLsizei stride,
                           const GLvoid* pointer) {
  ::glColorPointer(size, type, stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glColorPointer(" << size << ", "
             << Gl_wrapper::find(type) << ", " << stride << ");"
             << std::endl;);
}

/*! glCopyPixels wrapper */
inline void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                         GLenum type) {
  ::glCopyPixels(x, y, width, height, type);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyPixels("
             << x << ", " << y << ", " << width << ",  " << height
             << Gl_wrapper::find(type) << ");"
             << std::endl;);
}

/*! glCopyTexImage1D wrapper */
inline void glCopyTexImage1D(GLenum target, GLint level,
                             GLenum internalFormat, GLint x, GLint y,
                             GLsizei width, GLint border) {
  ::glCopyTexImage1D(target, level, internalFormat, x, y, width, border);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexImage1D("
             << Gl_wrapper::find(target) << ", " << level << ", "
             << Gl_wrapper::find(internalFormat) << ", "
             << x << ", " << y << ", " << width << ", " << border << ");"
             << std::endl;);
}

/*! glCopyTexImage2D wrapper */
inline void glCopyTexImage2D(GLenum target, GLint level,
                             GLenum internalFormat, GLint x, GLint y,
                             GLsizei width, GLsizei height, GLint border) {
  ::glCopyTexImage2D(target, level, internalFormat, x, y, width, height,
                     border);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexImage2D("
             << Gl_wrapper::find(target) << ", " << level << ", "
             << Gl_wrapper::find(internalFormat) << ", "
             << x << ", " << y << ", "
             << width << ", " << height << ", " << border << ");"
             << std::endl;);
}

/*! glCopyTexSubImage1D wrapper */
inline void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset,
                                GLint x, GLint y, GLsizei width) {
  ::glCopyTexSubImage1D(target, level, xoffset, x, y, width);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexSubImage1D("
             << Gl_wrapper::find(target) << ", " << level << ");"
             << std::endl;);
}

/*! glCopyTexSubImage2D wrapper */
inline void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                                GLint yoffset, GLint x, GLint y,
                                GLsizei width, GLsizei height) {
  ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexSubImage2D("
             << Gl_wrapper::find(target) << ", " << level << ");"
             << std::endl;);
}

/*! glCullFace wrapper */
inline void glCullFace(GLenum mode) {
  ::glCullFace(mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glCullFace("
             << Gl_wrapper::find(mode) << ");"
             << std::endl;);
}

/*! glDeleteLists wrapper */
inline void glDeleteLists(GLuint list, GLsizei range) {
  ::glDeleteLists(list, range);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDeleteLists(" << list << ");"
             << std::endl;);
}

/*! glDeleteTextures wrapper */
inline void glDeleteTextures(GLsizei n, const GLuint* textures) {
  ::glDeleteTextures(n, textures);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDeleteTextures(" << n << ");"
             << std::endl;);
}

/*! glDepthFunc wrapper */
inline void glDepthFunc(GLenum func) {
  ::glDepthFunc(func);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDepthFunc("
             << Gl_wrapper::find(func) << ");"
             << std::endl;);
}

/*! glDepthMask wrapper */
inline void glDepthMask(GLboolean flag) {
  ::glDepthMask(flag);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDepthMask(" << flag << ");"
             << std::endl;);
}

/*! glDepthRange wrapper */
inline void glDepthRange(GLclampd zNear, GLclampd zFar) {
  ::glDepthRange(zNear, zFar);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDepthRange("
             << zNear << ", " << zFar << ");"
             << std::endl;);
}

/*! glDisable wrapper */
inline void glDisable(GLenum cap) {
  ::glDisable(cap);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDisable("
             << Gl_wrapper::find(cap) << ");\n";);
}

/*! glDisableClientState wrapper */
inline void glDisableClientState(GLenum array) {
  ::glDisableClientState(array);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDisableClientState("
             << Gl_wrapper::find(array) << ");"
             << std::endl;);
}

/*! glDrawArrays wrapper */
inline void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
  ::glDrawArrays(mode, first, count);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawArrays("
             << Gl_wrapper::find(mode) << ");"
             << std::endl;);
}

/*! glDrawBuffer wrapper */
inline void glDrawBuffer(GLenum mode) {
  ::glDrawBuffer(mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawBuffer("
             << Gl_wrapper::find(mode) << ");"
             << std::endl;);
}

/*! glDrawElements wrapper */
inline void glDrawElements(GLenum mode, GLsizei count, GLenum type,
                           const GLvoid* indices) {
  ::glDrawElements(mode, count, type, indices);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawElements("
             << Gl_wrapper::find(mode)
             << "," << std::dec << count
             << "," << Gl_wrapper::find(type)
             << "," << "indices" << ");"
             << std::endl;);
}

/*! glDrawPixels wrapper */
inline void glDrawPixels(GLsizei width, GLsizei height, GLenum format,
                         GLenum type, const GLvoid* pixels) {
  ::glDrawPixels(width, height, format, type, pixels);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawPixels("
             << width << ", " << height << ","
             << Gl_wrapper::find(format) << ", "
             << Gl_wrapper::find(type) << ");"
             << std::endl;);
}

/*! glEdgeFlag wrapper */
inline void glEdgeFlag(GLboolean flag) {
  ::glEdgeFlag(flag);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEdgeFlag(" << flag << ");"
             << std::endl;);
}

/*! glEdgeFlagPointer wrapper */
inline void glEdgeFlagPointer(GLsizei stride, const GLvoid* pointer) {
  ::glEdgeFlagPointer(stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEdgeFlagPointer(" << ");"
             << std::endl;);
}

/*! glEdgeFlagv wrapper */
inline void glEdgeFlagv(const GLboolean* flag) {
  ::glEdgeFlagv(flag);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEdgeFlagv(" << flag << ");"
             << std::endl;);
}

/*! glEnable wrapper */
inline void glEnable(GLenum cap) {
  ::glEnable(cap);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEnable("
             << Gl_wrapper::find(cap) << ");"
             << std::endl;);
}

/*! glEnableClientState wrapper */
inline void glEnableClientState(GLenum array) {
  ::glEnableClientState(array);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEnableClientState("
             << Gl_wrapper::find(array) << ");"
             << std::endl;);
}

/*! end wrapper */
inline void glEnd(void) {
  ::glEnd();
  TRACE_MSG(Trace::GRAPHICS, "glEnd();\n");
}

/*! glEndList wrapper */
inline void glEndList(void) {
  ::glEndList();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEndList();"
             << std::endl;);
}

/*! glEvalCoord1d wrapper */
inline void glEvalCoord1d(GLdouble u) {
  ::glEvalCoord1d(u);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1d(" << u << ");"
             << std::endl;);
}

/*! glEvalCoord1dv wrapper */
inline void glEvalCoord1dv(const GLdouble* u) {
  ::glEvalCoord1dv(u);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1dv(" << ");"
             << std::endl;);
}

/*! glEvalCoord1f wrapper */
inline void glEvalCoord1f(GLfloat u) {
  ::glEvalCoord1f(u);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1f(" << u << ");"
             << std::endl;);
}

/*! glEvalCoord1fv wrapper */
inline void glEvalCoord1fv(const GLfloat* u) {
  ::glEvalCoord1fv(u);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1fv(" << ");"
             << std::endl;);
}

/*! glEvalCoord2d wrapper */
inline void glEvalCoord2d(GLdouble u, GLdouble v) {
  ::glEvalCoord2d(u, v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2d("
             << u << ", " << v << ");"
             << std::endl;);
}

/*! glEvalCoord2dv wrapper */
inline void glEvalCoord2dv(const GLdouble* u) {
  ::glEvalCoord2dv(u);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2dv(" << ");"
             << std::endl;);
}

/*! glEvalCoord2f wrapper */
inline void glEvalCoord2f(GLfloat u, GLfloat v) {
  ::glEvalCoord2f(u, v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2f("
             << u << ", " << v << ");"
             << std::endl;);
}

/*! glEvalCoord2fv wrapper */
inline void glEvalCoord2fv(const GLfloat* u) {
  ::glEvalCoord2fv(u);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2fv(" << ");"
             << std::endl;);
}

/*! glEvalMesh1 wrapper */
inline void glEvalMesh1(GLenum mode, GLint i1, GLint i2) {
  ::glEvalMesh1(mode, i1, i2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalMesh1("
             << Gl_wrapper::find(mode) << ", " << i1 << ", " << i2 << ");"
             << std::endl;);
}

/*! glEvalMesh2 wrapper */
inline void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) {
  ::glEvalMesh2(mode, i1, i2, j1, j2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalMesh2("
             << Gl_wrapper::find(mode) << ", " << i1 << ", " << i2
             << j1 << ", " << j2 << ");"
             << std::endl;);
}

/*! glEvalPoint1 wrapper */
inline void glEvalPoint1(GLint i) {
  ::glEvalPoint1(i);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalPoint1(" << i << ");"
             << std::endl;);
}

/*! glEvalPoint2 wrapper */
inline void glEvalPoint2(GLint i, GLint j) {
  ::glEvalPoint2(i, j);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalPoint2("
             << i << ", " << j << ");"
             << std::endl;);
}

/*! glFeedbackBuffer wrapper */
inline void glFeedbackBuffer(GLsizei size, GLenum type, GLfloat* buffer) {
  ::glFeedbackBuffer(size, type, buffer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFeedbackBuffer(" << size << ", "
             << Gl_wrapper::find(type) << ");"
             << std::endl;);
}

/*! glFinish wrapper */
inline void glFinish(void) {
  ::glFinish();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFinish();"
             << std::endl;);
}

/*! glFlush wrapper */
inline void glFlush(void) {
  ::glFlush();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFlush();"
             << std::endl;);
}

/*! glFogf wrapper */
inline void glFogf(GLenum pname, GLfloat param) {
  ::glFogf(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogf("
             << Gl_wrapper::find(pname) << ", " << param << ");"
             << std::endl;);
}

/*! glFogfv wrapper */
inline void glFogfv(GLenum pname, const GLfloat* params) {
  ::glFogfv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogfv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glFogi wrapper */
inline void glFogi(GLenum pname, GLint param) {
  ::glFogi(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogi("
             << Gl_wrapper::find(pname) << ", " << Gl_wrapper::find(param)
             << ");"
             << std::endl;);
}

/*! glFogiv wrapper */
inline void glFogiv(GLenum pname, const GLint* params) {
  ::glFogiv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogiv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}


/*! glFrontFace wrapper */
inline void glFrontFace(GLenum mode) {
  ::glFrontFace(mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFrontFace("
             << Gl_wrapper::find(mode) << ");"
             << std::endl;);
}

/*! glFrustum wrapper */
inline void glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
                      GLdouble top, GLdouble zNear, GLdouble zFar) {
  ::glFrustum(left, right, bottom, top, zNear, zFar);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glFrustum("
             << left << ", " << right << ", " << bottom << ", "
             << top << ", " << zNear << ", " << zFar << ");"
             << std::endl;);
}

/*! GLuint wrapper */
inline GLuint glGenLists(GLsizei range) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGenLists(" << range << ");"
             << std::endl;);
  return ::glGenLists(range);
}

/*! glGenTextures wrapper */
inline void glGenTextures(GLsizei n, GLuint* textures) {
  ::glGenTextures(n, textures);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGenTextures(" << n << ");"
             << std::endl;);
}

/*! glGetBooleanv wrapper */
inline void glGetBooleanv(GLenum pname, GLboolean* params) {
  ::glGetBooleanv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBooleanv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetClipPlane wrapper */
inline void glGetClipPlane(GLenum plane, GLdouble* equation) {
  ::glGetClipPlane(plane, equation);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetClipPlane(" << ");"
             << std::endl;);
}

/*! glGetDoublev wrapper */
inline void glGetDoublev(GLenum pname, GLdouble* params) {
  ::glGetDoublev(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetDoublev("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! GLenum wrapper */
inline GLenum glGetError(void) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLenum();"
             << std::endl;);
  return ::glGetError();
}

/*! glGetFloatv wrapper */
inline void glGetFloatv(GLenum pname, GLfloat* params) {
  ::glGetFloatv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetFloatv("
             << Gl_wrapper::find(pname) << ", params);"
             << std::endl;);
}

/*! glGetIntegerv wrapper */
inline void glGetIntegerv(GLenum pname, GLint* params) {
  ::glGetIntegerv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetIntegerv("
             << Gl_wrapper::find(pname) << ", params);"
             << std::endl;);
}

/*! glGetLightfv wrapper */
inline void glGetLightfv(GLenum light, GLenum pname, GLfloat* params) {
  ::glGetLightfv(light, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetLightfv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetLightiv wrapper */
inline void glGetLightiv(GLenum light, GLenum pname, GLint* params) {
  ::glGetLightiv(light, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetLightiv("
             << Gl_wrapper::find(pname) << ", params);"
             << std::endl;);
}

/*! glGetMapdv wrapper */
inline void glGetMapdv(GLenum target, GLenum query, GLdouble* v) {
  ::glGetMapdv(target, query, v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMapdv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(query) << ");"
             << std::endl;);
}

/*! glGetMapfv wrapper */
inline void glGetMapfv(GLenum target, GLenum query, GLfloat* v) {
  ::glGetMapfv(target, query, v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMapfv("
             << Gl_wrapper::find(target) << ", " << query << ");"
             << std::endl;);
}

/*! glGetMapiv wrapper */
inline void glGetMapiv(GLenum target, GLenum query, GLint* v) {
  ::glGetMapiv(target, query, v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMapiv("
             << Gl_wrapper::find(target) << ", " << query << ");"
             << std::endl;);
}

/*! glGetMaterialfv wrapper */
inline void glGetMaterialfv(GLenum face, GLenum pname, GLfloat* params) {
  ::glGetMaterialfv(face, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMaterialfv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetMaterialiv wrapper */
inline void glGetMaterialiv(GLenum face, GLenum pname, GLint* params) {
  ::glGetMaterialiv(face, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMaterialiv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetPixelMapfv wrapper */
inline void glGetPixelMapfv(GLenum map, GLfloat* values) {
  ::glGetPixelMapfv(map, values);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPixelMapfv(" << ");"
             << std::endl;);
}

/*! glGetPixelMapuiv wrapper */
inline void glGetPixelMapuiv(GLenum map, GLuint* values) {
  ::glGetPixelMapuiv(map, values);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPixelMapuiv(" << ");"
             << std::endl;);
}

/*! glGetPixelMapusv wrapper */
inline void glGetPixelMapusv(GLenum map, GLushort* values) {
  ::glGetPixelMapusv(map, values);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPixelMapusv(" << ");"
             << std::endl;);
}

/*! glGetPointerv wrapper */
inline void glGetPointerv(GLenum pname, GLvoid ** params) {
  ::glGetPointerv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPointerv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetPolygonStipple wrapper */
inline void glGetPolygonStipple(GLubyte* mask) {
  ::glGetPolygonStipple(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPolygonStipple(" << ");"
             << std::endl;);
}

/*! GLubyte wrapper */
inline const GLubyte* glGetString(GLenum name) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLubyte(" << ");"
             << std::endl;);
  return ::glGetString(name);
}

/*! glGetTexEnvfv wrapper */
inline void glGetTexEnvfv(GLenum target, GLenum pname, GLfloat* params) {
  ::glGetTexEnvfv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexEnvfv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexEnviv wrapper */
inline void glGetTexEnviv(GLenum target, GLenum pname, GLint* params) {
  ::glGetTexEnviv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexEnviv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexGendv wrapper */
inline void glGetTexGendv(GLenum coord, GLenum pname, GLdouble* params) {
  ::glGetTexGendv(coord, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexGendv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexGenfv wrapper */
inline void glGetTexGenfv(GLenum coord, GLenum pname, GLfloat* params) {
  ::glGetTexGenfv(coord, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexGenfv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexGeniv wrapper */
inline void glGetTexGeniv(GLenum coord, GLenum pname, GLint* params) {
  ::glGetTexGeniv(coord, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexGeniv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexImage wrapper */
inline void glGetTexImage(GLenum target, GLint level, GLenum format,
                          GLenum type, GLvoid* pixels) {
  ::glGetTexImage(target, level, format, type, pixels);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexImage("
             << Gl_wrapper::find(target) << ", " << level << ");"
             << std::endl;);
}

/*! glGetTexLevelParameterfv wrapper */
inline void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname,
                                     GLfloat* params) {
  ::glGetTexLevelParameterfv(target, level, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexLevelParameterfv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexLevelParameteriv wrapper */
inline void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname,
                                     GLint* params) {
  ::glGetTexLevelParameteriv(target, level, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexLevelParameteriv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexParameterfv wrapper */
inline void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
  ::glGetTexParameterfv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexParameterfv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glGetTexParameteriv wrapper */
inline void glGetTexParameteriv(GLenum target, GLenum pname, GLint* params) {
  ::glGetTexParameteriv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexParameteriv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glHint wrapper */
inline void glHint(GLenum target, GLenum mode) {
  ::glHint(target, mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glHint(" << ");"
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(mode) << std::endl;);
}

/*! glIndexMask wrapper */
inline void glIndexMask(GLuint mask) {
  ::glIndexMask(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexMask(" << mask << ");"
             << std::endl;);
}

/*! glIndexPointer wrapper */
inline void glIndexPointer(GLenum type, GLsizei stride,
                           const GLvoid* pointer) {
  ::glIndexPointer(type, stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexPointer(" << ");"
             << std::endl;);
}

/*! glIndexd wrapper */
inline void glIndexd(GLdouble c) {
  ::glIndexd(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexd(" << c << ");"
             << std::endl;);
}

/*! glIndexdv wrapper */
inline void glIndexdv(const GLdouble* c) {
  ::glIndexdv(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexdv(" << ");"
             << std::endl;);
}

/*! glIndexf wrapper */
inline void glIndexf(GLfloat c) {
  ::glIndexf(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexf(" << c << ");"
             << std::endl;);
}

/*! glIndexfv wrapper */
inline void glIndexfv(const GLfloat* c) {
  ::glIndexfv(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexfv(" << ");"
             << std::endl;);
}

/*! glIndexi wrapper */
inline void glIndexi(GLint c) {
  ::glIndexi(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexi(" << c << ");"
             << std::endl;);
}

/*! glIndexiv wrapper */
inline void glIndexiv(const GLint* c) {
  ::glIndexiv(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexiv(" << ");"
             << std::endl;);
}

/*! glIndexs wrapper */
inline void glIndexs(GLshort c) {
  ::glIndexs(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexs(" << c << ");"
             << std::endl;);
}

/*! glIndexsv wrapper */
inline void glIndexsv(const GLshort* c) {
  ::glIndexsv(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexsv(" << ");"
             << std::endl;);
}

/*! glIndexub wrapper */
inline void glIndexub(GLubyte c) {
  ::glIndexub(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexub(" << c << ");"
             << std::endl;);
}

/*! glIndexubv wrapper */
inline void glIndexubv(const GLubyte* c) {
  ::glIndexubv(c);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexubv(" << ");"
             << std::endl;);
}

/*! glInitNames wrapper */
inline void glInitNames(void) {
  ::glInitNames();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glInitNames();"
             << std::endl;);
}

/*! glInterleavedArrays wrapper */
inline void glInterleavedArrays(GLenum format, GLsizei stride,
                                const GLvoid* pointer) {
  ::glInterleavedArrays(format, stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glInterleavedArrays(" << ");"
             << std::endl;);
}

/*! GLboolean wrapper */
inline GLboolean glIsEnabled(GLenum cap) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLboolean(" << ");"
             << std::endl;);
  return ::glIsEnabled(cap);
}

/*! GLboolean wrapper */
inline GLboolean glIsList(GLuint list) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLboolean(" << list << ");"
             << std::endl;);
  return ::glIsList(list);
}

/*! GLboolean wrapper */
inline GLboolean glIsTexture(GLuint texture) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLboolean(" << texture << ");"
             << std::endl;);
  return ::glIsTexture(texture);
}

/*! glLightModelf wrapper */
inline void glLightModelf(GLenum pname, GLfloat param) {
  ::glLightModelf(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModelf("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLightModelfv wrapper */
inline void glLightModelfv(GLenum pname, const GLfloat* params) {
  ::glLightModelfv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModelfv("
             << Gl_wrapper::find(pname) << ", params);"
             << std::endl;);
}

/*! glLightModeli wrapper */
inline void glLightModeli(GLenum pname, GLint param) {
  ::glLightModeli(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModeli("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLightModeliv wrapper */
inline void glLightModeliv(GLenum pname, const GLint* params) {
  ::glLightModeliv(pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModeliv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLightf wrapper */
inline void glLightf(GLenum light, GLenum pname, GLfloat param) {
  ::glLightf(light, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightf("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLightfv wrapper */
inline void glLightfv(GLenum light, GLenum pname, const GLfloat* params) {
  ::glLightfv(light, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightfv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLighti wrapper */
inline void glLighti(GLenum light, GLenum pname, GLint param) {
  ::glLighti(light, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLighti("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLightiv wrapper */
inline void glLightiv(GLenum light, GLenum pname, const GLint* params) {
  ::glLightiv(light, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightiv("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glLineStipple wrapper */
inline void glLineStipple(GLint factor, GLushort pattern) {
  ::glLineStipple(factor, pattern);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLineStipple(" << ");"
             << std::endl;);
}

/*! glLineWidth wrapper */
inline void glLineWidth(GLfloat width) {
  ::glLineWidth(width);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLineWidth(" << width << ");"
             << std::endl;);
}

/*! glListBase wrapper */
inline void glListBase(GLuint base) {
  ::glListBase(base);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glListBase(" << base << ");"
             << std::endl;);
}

/*! glLoadIdentity wrapper */
inline void glLoadIdentity() {
  ::glLoadIdentity();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLoadIdentity();\n";);
}

/*! glLoadMatrixd wrapper */
inline void glLoadMatrixd(const GLdouble* m) {
  ::glLoadMatrixd(m);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLoadMatrixd(" << ");"
             << std::endl;);
}

/*! glLoadMatrixf wrapper */
inline void glLoadMatrixf(const GLfloat* m) {
  ::glLoadMatrixf(m);
#if !defined(NDEBUG)
  if(SGAL::TRACE(Trace::GRAPHICS)) {
    std::cout << "glLoadMatrixf(";
    for(Uint i = 0; i < 16; ++i) {
      std::cout << m[i];
      if((i % 4) != 3) std::cout << ", ";
      else std::cout
        << std::endl;
    }
    std::cout << ");\n";
  }
#endif
}

/*! glLoadName wrapper */
inline void glLoadName(GLuint name) {
  ::glLoadName(name);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLuint(" << name << ");"
             << std::endl;);
}

/*! glLogicOp wrapper */
inline void glLogicOp(GLenum opcode) {
  ::glLogicOp(opcode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glLogicOp(" << ");"
             << std::endl;);
}

/*! glMap1d wrapper */
inline void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride,
                    GLint order, const GLdouble* points) {
  ::glMap1d(target, u1, u2, stride, order, points);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap1d("
             << Gl_wrapper::find(target) << ", " << u1 << ", " << u2 << ", "
             << stride << ", " << order << ");"
             << std::endl;);
}

/*! glMap1f wrapper */
inline void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride,
                    GLint order, const GLfloat* points) {
  ::glMap1f(target, u1, u2, stride, order, points);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap1f("
             << Gl_wrapper::find(target) << ", " << u1 << ", " << u2 << ", "
             << stride << ", " << order << ");"
             << std::endl;);
}

/*! glMap2d wrapper */
inline void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride,
                    GLint uorder, GLdouble v1, GLdouble v2, GLint vstride,
                    GLint vorder, const GLdouble* points) {
  ::glMap2d(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap2d("
             << Gl_wrapper::find(target) << ", " << u1 << ", " << u2 << ", "
             << ustride << ", " << uorder << ", " << v1 << ", " << v2 << ", "
             << vstride << ", " << vorder << ");"
             << std::endl;);
}

/*! glMap2f wrapper */
inline void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride,
                    GLint uorder, GLfloat v1, GLfloat v2, GLint vstride,
                    GLint vorder, const GLfloat* points) {
  ::glMap2f(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap2f("
             << Gl_wrapper::find(target) << ", " << u1 << ", " << u2 << ", "
             << ustride << ", " << uorder << ", " << v1 << ", " << v2 << ", "
             << vstride << ", " << vorder << ");"
             << std::endl;);
}

/*! glMapGrid1d wrapper */
inline void glMapGrid1d(GLint un, GLdouble u1, GLdouble u2) {
  ::glMapGrid1d(un, u1, u2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid1d("
             << un << ", " << u1 << ", " << u2 << ");"
             << std::endl;);
}

/*! glMapGrid1f wrapper */
inline void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2) {
  ::glMapGrid1f(un, u1, u2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid1f("
             << un << ", " << u1 << ", " << u2 << ");"
             << std::endl;);
}

/*! glMapGrid2d wrapper */
inline void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2,
                        GLint vn, GLdouble v1, GLdouble v2) {
  ::glMapGrid2d(un, u1, u2, vn, v1, v2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid2d("
             << un << ", " << u1 << ", " << u2 << ", "
             << vn << ", " << v1 << ", " << v2 << ");"
             << std::endl;);
}

/*! glMapGrid2f wrapper */
inline void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn,
                        GLfloat v1, GLfloat v2) {
  ::glMapGrid2f(un, u1, u2, vn, v1, v2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid2f("
             << un << ", " << u1 << ", " << u2 << ", "
             << vn << ", " << v1 << ", " << v2 << ");"
             << std::endl;);
}

/*! glMaterialf wrapper */
inline void glMaterialf(GLenum face, GLenum pname, GLfloat param) {
  ::glMaterialf(face, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMaterialf("
             << Gl_wrapper::find(face) << ", "
             << Gl_wrapper::find(pname) << ", " << param << ");"
             << std::endl;);
}

/*! glMaterialfv wrapper */
inline void glMaterialfv(GLenum face, GLenum pname, const GLfloat* params) {
  ::glMaterialfv(face, pname, params);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "float params[] = {" << std::endl << "  ";
             switch(pname) {
              case GL_AMBIENT:
              case GL_DIFFUSE:
              case GL_SPECULAR:
              case GL_EMISSION:
              case GL_AMBIENT_AND_DIFFUSE:
               std::cout << params[0] << "," << params[1] << ","
                         << params[2] << "," << params[3]
                         << std::endl;
               break;
               
              case GL_SHININESS: std::cout << params[0] << std::endl; break;
               
              case GL_COLOR_INDEXES:
               std::cout << params[0] << "," << params[1] << "," << params[2]
                         << std::endl;
             }
             std::cout << "}" << std::endl;
             std::cout << "glMaterialfv("
             << Gl_wrapper::find(face) << ", "
             << Gl_wrapper::find(pname) << ", params);"
             << std::endl;);
}

/*! glMateriali wrapper */
inline void glMateriali(GLenum face, GLenum pname, GLint param) {
  ::glMateriali(face, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMateriali("
             << Gl_wrapper::find(face) << ", "
             << Gl_wrapper::find(pname) << ", " << param << ");"
             << std::endl;);
}

/*! glMaterialiv wrapper */
inline void glMaterialiv(GLenum face, GLenum pname, const GLint* params) {
  ::glMaterialiv(face, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMaterialiv("
             << Gl_wrapper::find(face) << ", "
             << Gl_wrapper::find(pname) << ", params);"
             << std::endl;);
}

/*! glMatrixMode wrapper */
inline void glMatrixMode(GLenum mode) {
  ::glMatrixMode(mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMatrixMode("
             << Gl_wrapper::find(mode) << ");"
             << std::endl;);
}

/*! glMultMatrixd wrapper */
inline void glMultMatrixd(const GLdouble* m) {
  ::glMultMatrixd(m);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMultMatrixd(" << ");"
             << std::endl;);
}

/*! glMultMatrixf wrapper */
inline void glMultMatrixf(const GLfloat* m) {
  ::glMultMatrixf(m);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMultMatrixf(" << ");"
             << std::endl;);
}

/*! glNewList wrapper */
inline void glNewList(GLuint list, GLenum mode) {
  ::glNewList(list, mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNewList(" << ");"
             << std::endl;);
}

/*! glNormal3b wrapper */
inline void glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz) {
  ::glNormal3b(nx, ny, nz);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3b(" << ");"
             << std::endl;);
}

/*! glNormal3bv wrapper */
inline void glNormal3bv(const GLbyte* v) {
  ::glNormal3bv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3bv(" << ");"
             << std::endl;);
}

/*! glNormal3d wrapper */
inline void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz) {
  ::glNormal3d(nx, ny, nz);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3d("
             << nx << ", " << ny << ", " << nz << ");"
             << std::endl;);
}

/*! glNormal3dv wrapper */
inline void glNormal3dv(const GLdouble* v) {
  ::glNormal3dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3dv(" << ");"
             << std::endl;);
}

/*! glNormal3f wrapper */
inline void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz) {
  ::glNormal3f(nx, ny, nz);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3f("
             << nx << ", " << ny << ", " << nz << ");"
             << std::endl;);
}

/*! normal wrapper */
inline void glNormal3fv(const GLfloat* v) {
  ::glNormal3fv(v);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glNormal3fv(" << *((Vector3f *) v) << ");"
             << std::endl;
             );
}

/*! glNormal3i wrapper */
inline void glNormal3i(GLint nx, GLint ny, GLint nz) {
  ::glNormal3i(nx, ny, nz);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "("
             << nx << ", " << ny << ", " << nz << ");"
             << std::endl;);
}

/*! glNormal3iv wrapper */
inline void glNormal3iv(const GLint* v) {
  ::glNormal3iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3iv(" << ");"
             << std::endl;);
}

/*! glNormal3s wrapper */
inline void glNormal3s(GLshort nx, GLshort ny, GLshort nz) {
  ::glNormal3s(nx, ny, nz);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3s("
             << nx << ", " << ny << ", " << nz << ");"
             << std::endl;);
}

/*! glNormal3sv wrapper */
inline void glNormal3sv(const GLshort* v) {
  ::glNormal3sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3sv(" << ");"
             << std::endl;);
}

/*! glNormalPointer wrapper */
inline void glNormalPointer(GLenum type, GLsizei stride,
                            const GLvoid* pointer) {
  ::glNormalPointer(type, stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormalPointer("
             << Gl_wrapper::find(type) << ", "
             << Gl_wrapper::find(stride) << ", "
             << "pointer);"
             << std::endl;);
}

/*! glOrtho wrapper */
inline void glOrtho(GLdouble left, GLdouble right, GLdouble bottom,
                    GLdouble top, GLdouble zNear, GLdouble zFar) {
  ::glOrtho(left, right, bottom, top, zNear, zFar);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glOrtho("
             << left << ", " << right << ", "
             << bottom << ", " << top << ", " << zNear << ", " << zFar << ");"
             << std::endl;);
}

/*! glPassThrough wrapper */
inline void glPassThrough(GLfloat token) {
  ::glPassThrough(token);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPassThrough(" << token << ");"
             << std::endl;);
}

/*! glPixelMapfv wrapper */
inline void glPixelMapfv(GLenum map, GLsizei mapsize, const GLfloat* values) {
  ::glPixelMapfv(map, mapsize, values);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelMapfv(" << ");"
             << std::endl;);
}

/*! glPixelMapuiv wrapper */
inline void glPixelMapuiv(GLenum map, GLsizei mapsize, const GLuint* values) {
  ::glPixelMapuiv(map, mapsize, values);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelMapuiv(" << ");"
             << std::endl;);
}

/*! glPixelMapusv wrapper */
inline void glPixelMapusv(GLenum map, GLsizei mapsize,
                          const GLushort* values) {
  ::glPixelMapusv(map, mapsize, values);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelMapusv(" << ");"
             << std::endl;);
}

/*! glPixelStoref wrapper */
inline void glPixelStoref(GLenum pname, GLfloat param) {
  ::glPixelStoref(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelStoref("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glPixelStorei wrapper */
inline void glPixelStorei(GLenum pname, GLint param) {
  ::glPixelStorei(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelStorei("
             << Gl_wrapper::find(pname) << ", " << param << ");"
             << std::endl;);
}

/*! glPixelTransferf wrapper */
inline void glPixelTransferf(GLenum pname, GLfloat param) {
  ::glPixelTransferf(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelTransferf("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glPixelTransferi wrapper */
inline void glPixelTransferi(GLenum pname, GLint param) {
  ::glPixelTransferi(pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelTransferi("
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glPixelZoom wrapper */
inline void glPixelZoom(GLfloat xfactor, GLfloat yfactor) {
  ::glPixelZoom(xfactor, yfactor);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelZoom(" << ");"
             << std::endl;);
}

/*! glPointSize wrapper */
inline void glPointSize(GLfloat size) {
  ::glPointSize(size);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPointSize(" << size << ");"
             << std::endl;);
}

/*! glPolygonMode wrapper */
inline void glPolygonMode(GLenum face, GLenum mode) {
  ::glPolygonMode(face, mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPolygonMode(" << ");"
             << std::endl;);
}

/*! glPolygonOffset wrapper */
inline void glPolygonOffset(GLfloat factor, GLfloat units) {
  ::glPolygonOffset(factor, units);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPolygonOffset(" << ");"
             << std::endl;);
}

/*! glPolygonStipple wrapper */
inline void glPolygonStipple(const GLubyte* mask) {
  ::glPolygonStipple(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPolygonStipple(" << ");"
             << std::endl;);
}

/*! glPopAttrib wrapper */
inline void glPopAttrib(void) {
  ::glPopAttrib();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopAttrib();"
             << std::endl;);
}

/*! glPopClientAttrib wrapper */
inline void glPopClientAttrib(void) {
  ::glPopClientAttrib();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopClientAttrib();"
             << std::endl;);
}

/*! glPopMatrix wrapper */
inline void glPopMatrix(void) {
  ::glPopMatrix();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopMatrix();"
             << std::endl;);
}

/*! glPopName wrapper */
inline void glPopName(void) {
  ::glPopName();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopName();"
             << std::endl;);
}

/*! glPrioritizeTextures wrapper */
inline void glPrioritizeTextures(GLsizei n, const GLuint* textures,
                                 const GLclampf* priorities) {
  ::glPrioritizeTextures(n, textures, priorities);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPrioritizeTextures(" << ");"
             << std::endl;);
}

/*! glPushAttrib wrapper */
inline void glPushAttrib(GLbitfield mask) {
  ::glPushAttrib(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushAttrib(" << ");"
             << std::endl;);
}

/*! glPushClientAttrib wrapper */
inline void glPushClientAttrib(GLbitfield mask) {
  ::glPushClientAttrib(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushClientAttrib(" << ");"
             << std::endl;);
}

/*! glPushMatrix wrapper */
inline void glPushMatrix(void) {
  ::glPushMatrix();
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushMatrix();\n";);
}

/*! glPushName wrapper */
inline void glPushName(GLuint name) {
  ::glPushName(name);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushMatrix(" << name << ");"
             << std::endl;);
}

/*! glRasterPos2d wrapper */
inline void glRasterPos2d(GLdouble x, GLdouble y) {
  ::glRasterPos2d(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2d("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glRasterPos2dv wrapper */
inline void glRasterPos2dv(const GLdouble* v) {
  ::glRasterPos2dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2dv(" << ");"
             << std::endl;);
}

/*! glRasterPos2f wrapper */
inline void glRasterPos2f(GLfloat x, GLfloat y) {
  ::glRasterPos2f(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2f("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glRasterPos2fv wrapper */
inline void glRasterPos2fv(const GLfloat* v) {
  ::glRasterPos2fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2fv(" << ");"
             << std::endl;);
}

/*! glRasterPos2i wrapper */
inline void glRasterPos2i(GLint x, GLint y) {
  ::glRasterPos2i(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2i("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glRasterPos2iv wrapper */
inline void glRasterPos2iv(const GLint* v) {
  ::glRasterPos2iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2iv(" << ");"
             << std::endl;);
}

/*! glRasterPos2s wrapper */
inline void glRasterPos2s(GLshort x, GLshort y) {
  ::glRasterPos2s(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2s("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glRasterPos2sv wrapper */
inline void glRasterPos2sv(const GLshort* v) {
  ::glRasterPos2sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2sv(" << ");"
             << std::endl;);
}

/*! glRasterPos3d wrapper */
inline void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z) {
  ::glRasterPos3d(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3d("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glRasterPos3dv wrapper */
inline void glRasterPos3dv(const GLdouble* v) {
  ::glRasterPos3dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3dv(" << ");"
             << std::endl;);
}

/*! glRasterPos3f wrapper */
inline void glRasterPos3f(GLfloat x, GLfloat y, GLfloat z) {
  ::glRasterPos3f(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3f("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glRasterPos3fv wrapper */
inline void glRasterPos3fv(const GLfloat* v) {
  ::glRasterPos3fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3fv(" << ");"
             << std::endl;);
}

/*! glRasterPos3i wrapper */
inline void glRasterPos3i(GLint x, GLint y, GLint z) {
  ::glRasterPos3i(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3i("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glRasterPos3iv wrapper */
inline void glRasterPos3iv(const GLint* v) {
  ::glRasterPos3iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3iv(" << ");"
             << std::endl;);
}

/*! glRasterPos3s wrapper */
inline void glRasterPos3s(GLshort x, GLshort y, GLshort z) {
  ::glRasterPos3s(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3s("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glRasterPos3sv wrapper */
inline void glRasterPos3sv(const GLshort* v) {
  ::glRasterPos3sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3sv(" << ");"
             << std::endl;);
}

/*! glRasterPos4d wrapper */
inline void glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  ::glRasterPos4d(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4d("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glRasterPos4dv wrapper */
inline void glRasterPos4dv(const GLdouble* v) {
  ::glRasterPos4dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4dv(" << ");"
             << std::endl;);
}

/*! glRasterPos4f wrapper */
inline void glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  ::glRasterPos4f(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4f("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glRasterPos4fv wrapper */
inline void glRasterPos4fv(const GLfloat* v) {
  ::glRasterPos4fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4fv(" << ");"
             << std::endl;);
}

/*! glRasterPos4i wrapper */
inline void glRasterPos4i(GLint x, GLint y, GLint z, GLint w) {
  ::glRasterPos4i(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4i("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glRasterPos4iv wrapper */
inline void glRasterPos4iv(const GLint* v) {
  ::glRasterPos4iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4iv(" << ");"
             << std::endl;);
}

/*! glRasterPos4s wrapper */
inline void glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w) {
  ::glRasterPos4s(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4s("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glRasterPos4sv wrapper */
inline void glRasterPos4sv(const GLshort* v) {
  ::glRasterPos4sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4sv(" << ");"
             << std::endl;);
}

/*! glReadBuffer wrapper */
inline void glReadBuffer(GLenum mode) {
  ::glReadBuffer(mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glReadBuffer(" << ");"
             << std::endl;);
}

/*! glReadPixels wrapper */
inline void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                         GLenum format, GLenum type, GLvoid* pixels) {
  ::glReadPixels(x, y, width, height, format, type, pixels);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glReadPixels("
             << x << ", " << y << ", " << width << ", " << height << ", "
             << Gl_wrapper::find(format) << ","
             << Gl_wrapper::find(type) << ", pixels);"
             << std::endl;);
}

/*! glRectd wrapper */
inline void glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {
  ::glRectd(x1, y1, x2, y2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectd("
             << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
             << std::endl;);
}

/*! glRectdv wrapper */
inline void glRectdv(const GLdouble* v1, const GLdouble* v2) {
  ::glRectdv(v1, v2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectdv(" << ");"
             << std::endl;);
}

/*! glRectf wrapper */
inline void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
  ::glRectf(x1, y1, x2, y2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectf("
             << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
             << std::endl;);
}

/*! glRectfv wrapper */
inline void glRectfv(const GLfloat* v1, const GLfloat* v2) {
  ::glRectfv(v1, v2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectfv(" << ");"
             << std::endl;);
}

/*! glRecti wrapper */
inline void glRecti(GLint x1, GLint y1, GLint x2, GLint y2) {
  ::glRecti(x1, y1, x2, y2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRecti("
             << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
             << std::endl;);
}

/*! glRectiv wrapper */
inline void glRectiv(const GLint* v1, const GLint* v2) {
  ::glRectiv(v1, v2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectiv(" << ");"
             << std::endl;);
}

/*! glRects wrapper */
inline void glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2) {
  ::glRects(x1, y1, x2, y2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRects("
             << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
             << std::endl;);
}

/*! glRectsv wrapper */
inline void glRectsv(const GLshort* v1, const GLshort* v2) {
  ::glRectsv(v1, v2);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectsv(" << ");"
             << std::endl;);
}

/*! GLint wrapper */
inline GLint glRenderMode(GLenum mode) {
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRenderMode(" << ");"
             << std::endl;);
  return ::glRenderMode(mode);
}

/*! glRotated wrapper */
inline void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {
  ::glRotated(angle, x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRotated(" << ");"
             << std::endl;);
}

/*! glRotatef wrapper */
inline void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
  ::glRotatef(angle, x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glRotatef("
             << angle << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glScaled wrapper */
inline void glScaled(GLdouble x, GLdouble y, GLdouble z) {
  ::glScaled(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glScaled("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glScalef wrapper */
inline void glScalef(GLfloat x, GLfloat y, GLfloat z) {
  ::glScalef(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glScalef("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glScissor wrapper */
inline void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
  ::glScissor(x, y, width, height);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glScissor(" << x << ", " << y
             << width << ", " << height << ");"
             << std::endl;);
}

/*! glSelectBuffer wrapper */
inline void glSelectBuffer(GLsizei size, GLuint* buffer) {
  ::glSelectBuffer(size, buffer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glSelectBuffer(" << ");"
             << std::endl;);
}

/*! glShadeModel wrapper */
inline void glShadeModel(GLenum mode) {
  ::glShadeModel(mode);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glShadeModel(" << ");"
             << std::endl;);
}

/*! glStencilFunc wrapper */
inline void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
  ::glStencilFunc(func, ref, mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glStencilFunc(" << ");"
             << std::endl;);
}

/*! glStencilMask wrapper */
inline void glStencilMask(GLuint mask) {
  ::glStencilMask(mask);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glStencilMask(" << ");"
             << std::endl;);
}

/*! glStencilOp wrapper */
inline void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
  ::glStencilOp(fail, zfail, zpass);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glStencilOp(" << ");"
             << std::endl;);
}

/*! glTexCoord1d wrapper */
inline void glTexCoord1d(GLdouble s) {
  ::glTexCoord1d(s);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1d(" << s << ");"
             << std::endl;);
}

/*! glTexCoord1dv wrapper */
inline void glTexCoord1dv(const GLdouble* v) {
  ::glTexCoord1dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1dv(" << ");"
             << std::endl;);
}

/*! glTexCoord1f wrapper */
inline void glTexCoord1f(GLfloat s) {
  ::glTexCoord1f(s);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1f(" << s << ");"
             << std::endl;);
}

/*! glTexCoord1fv wrapper */
inline void glTexCoord1fv(const GLfloat* v) {
  ::glTexCoord1fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1fv(" << ");"
             << std::endl;);
}

/*! glTexCoord1i wrapper */
inline void glTexCoord1i(GLint s) {
  ::glTexCoord1i(s);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1i(" << s << ");"
             << std::endl;);
}

/*! glTexCoord1iv wrapper */
inline void glTexCoord1iv(const GLint* v) {
  ::glTexCoord1iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1iv(" << ");"
             << std::endl;);
}

/*! glTexCoord1s wrapper */
inline void glTexCoord1s(GLshort s) {
  ::glTexCoord1s(s);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1s(" << s << ");"
             << std::endl;);
}

/*! glTexCoord1sv wrapper */
inline void glTexCoord1sv(const GLshort* v) {
  ::glTexCoord1sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1sv(" << ");"
             << std::endl;);
}

/*! glTexCoord2d wrapper */
inline void glTexCoord2d(GLdouble s, GLdouble t) {
  ::glTexCoord2d(s, t);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2d("
             << s << ", " << t << ");"
             << std::endl;);
}

/*! glTexCoord2dv wrapper */
inline void glTexCoord2dv(const GLdouble* v) {
  ::glTexCoord2dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2dv(" << ");"
             << std::endl;);
}

/*! tex-coord wrapper */
inline void glTexCoord2f(const GLfloat v1, const GLfloat v2) {
  ::glTexCoord2f(v1, v2);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glTexCoord2f(" << v1 << "," << v2 << ");"
             << std::endl;);
}

/*! tex-coord wrapper */
inline void glTexCoord2fv(const GLfloat* v) {
  ::glTexCoord2fv(v);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glTexCoord2fv(" << *((Vector2f *) v) << ");"
             << std::endl;);
}

/*! glTexCoord2i wrapper */
inline void glTexCoord2i(GLint s, GLint t) {
  ::glTexCoord2i(s, t);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2i("
             << s << ", " << t << ");"
             << std::endl;);
}

/*! glTexCoord2iv wrapper */
inline void glTexCoord2iv(const GLint* v) {
  ::glTexCoord2iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2iv(" << ");"
             << std::endl;);
}

/*! glTexCoord2s wrapper */
inline void glTexCoord2s(GLshort s, GLshort t) {
  ::glTexCoord2s(s, t);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2s(" << s << ", " << t << ");"
             << std::endl;);
}

/*! glTexCoord2sv wrapper */
inline void glTexCoord2sv(const GLshort* v) {
  ::glTexCoord2sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2sv(" << ");"
             << std::endl;);
}

/*! glTexCoord3d wrapper */
inline void glTexCoord3d(GLdouble s, GLdouble t, GLdouble r) {
  ::glTexCoord3d(s, t, r);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3d("
             << s << ", " << t << ", " << r << ");"
             << std::endl;);
}

/*! glTexCoord3dv wrapper */
inline void glTexCoord3dv(const GLdouble* v) {
  ::glTexCoord3dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3dv(" << ");"
             << std::endl;);
}

/*! glTexCoord3f wrapper */
inline void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r) {
  ::glTexCoord3f(s, t, r);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3f("
             << s << ", " << t << ", " << r << ");"
             << std::endl;);
}

/*! glTexCoord3fv wrapper */
inline void glTexCoord3fv(const GLfloat* v) {
  ::glTexCoord3fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3fv(" << ");"
             << std::endl;);
}

/*! glTexCoord3i wrapper */
inline void glTexCoord3i(GLint s, GLint t, GLint r) {
  ::glTexCoord3i(s, t, r);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3i("
             << s << ", " << t << ", " << r << ");"
             << std::endl;);
}

/*! glTexCoord3iv wrapper */
inline void glTexCoord3iv(const GLint* v) {
  ::glTexCoord3iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3iv(" << ");"
             << std::endl;);
}

/*! glTexCoord3s wrapper */
inline void glTexCoord3s(GLshort s, GLshort t, GLshort r) {
  ::glTexCoord3s(s, t, r);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3s("
             << s << ", " << t << ", " << r << ");"
             << std::endl;);
}

/*! glTexCoord3sv wrapper */
inline void glTexCoord3sv(const GLshort* v) {
  ::glTexCoord3sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3sv(" << ");"
             << std::endl;);
}

/*! glTexCoord4d wrapper */
inline void glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q) {
  ::glTexCoord4d(s, t, r, q);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4d("
             << s << ", " << t << ", " << r << ", " << q << ");"
             << std::endl;);
}

/*! glTexCoord4dv wrapper */
inline void glTexCoord4dv(const GLdouble* v) {
  ::glTexCoord4dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4dv(" << ");"
             << std::endl;);
}

/*! glTexCoord4f wrapper */
inline void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
  ::glTexCoord4f(s, t, r, q);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4f("
             << s << ", " << t << ", " << r << ", " << q << ");"
             << std::endl;);
}

/*! glTexCoord4fv wrapper */
inline void glTexCoord4fv(const GLfloat* v) {
  ::glTexCoord4fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4fv(" << ");"
             << std::endl;);
}

/*! glTexCoord4i wrapper */
inline void glTexCoord4i(GLint s, GLint t, GLint r, GLint q) {
  ::glTexCoord4i(s, t, r, q);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4i("
             << s << ", " << t << ", " << r << ", " << q << ");"
             << std::endl;);
}

/*! glTexCoord4iv wrapper */
inline void glTexCoord4iv(const GLint* v) {
  ::glTexCoord4iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4iv(" << ");"
             << std::endl;);
}

/*! glTexCoord4s wrapper */
inline void glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q) {
  ::glTexCoord4s(s, t, r, q);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4s("
             << s << ", " << t << ", " << r << ", " << q << ");"
             << std::endl;);
}

/*! glTexCoord4sv wrapper */
inline void glTexCoord4sv(const GLshort* v) {
  ::glTexCoord4sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4sv(" << ");"
             << std::endl;);
}

/*! glTexCoordPointer wrapper */
inline void glTexCoordPointer(GLint size, GLenum type, GLsizei stride,
                              const GLvoid* pointer) {
  ::glTexCoordPointer(size, type, stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoordPointer(" << ");"
             << std::endl;);
}

/*! glTexEnvf wrapper */
inline void glTexEnvf(GLenum target, GLenum pname, GLfloat param) {
  ::glTexEnvf(target, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexEnvf("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glTexEnvfv wrapper */
inline void glTexEnvfv(GLenum target, GLenum pname, const GLfloat* params) {
  ::glTexEnvfv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexEnvfv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glTexEnvi wrapper */
inline void glTexEnvi(GLenum target, GLenum pname, GLint param)
{
  ::glTexEnvi(target, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexEnvi("
             << Gl_wrapper::find(target) << Gl_wrapper::find(pname)
             << Gl_wrapper::find(param) << ");"
             << std::endl;);
  
}

/*! glTexEnviv wrapper */
inline void glTexEnviv(GLenum target, GLenum pname, const GLint* params) {
  ::glTexEnviv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLenum("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(*params) << ");"
             << std::endl;);
}

/*! glTexGend wrapper */
inline void glTexGend(GLenum coord, GLenum pname, GLdouble param) {
  ::glTexGend(coord, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGend("
             << Gl_wrapper::find(coord) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(param) << ");"
             << std::endl;);
}

/*! glTexGendv wrapper */
inline void glTexGendv(GLenum coord, GLenum pname, const GLdouble* params) {
  ::glTexGendv(coord, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGendv("
             << Gl_wrapper::find(coord) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(*params) << ");"
             << std::endl;);
}

/*! glTexGenf wrapper */
inline void glTexGenf(GLenum coord, GLenum pname, GLfloat param) {
  ::glTexGenf(coord, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGenf("
             << Gl_wrapper::find(coord) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(param) << ");"
             << std::endl;);
}

/*! glTexGenfv wrapper */
inline void glTexGenfv(GLenum coord, GLenum pname, const GLfloat* params) {
  ::glTexGenfv(coord, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGenfv("
             << Gl_wrapper::find(coord) << ", "
             << Gl_wrapper::find(*params) << ", "
             << ");"
             << std::endl;);
}

/*! glTexGeni wrapper */
inline void glTexGeni(GLenum coord, GLenum pname, GLint param) {
  ::glTexGeni(coord, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGeni("
             << Gl_wrapper::find(coord) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(param) << ");"
             << std::endl;);
}

/*! glTexGeniv wrapper */
inline void glTexGeniv(GLenum coord, GLenum pname, const GLint* params) {
  ::glTexGeniv(coord, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGeniv("
             << Gl_wrapper::find(coord) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(*params) << ");"
             << std::endl;);
}

/*! glTexImage1D wrapper */
inline void glTexImage1D(GLenum target, GLint level, GLint internalformat,
                         GLsizei width, GLint border, GLenum format,
                         GLenum type, const GLvoid* pixels) {
  ::glTexImage1D(target, level, internalformat, width, border, format,  type,
                 pixels);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexImage1D("
             << Gl_wrapper::find(target) << ", " << level << ");"
             << std::endl;);
}

/*! glTexImage2D wrapper */
inline void glTexImage2D(GLenum target, GLint level, GLint internalformat,
                         GLsizei width, GLsizei height, GLint border,
                         GLenum format, GLenum type, const GLvoid* pixels) {
  ::glTexImage2D(target, level, internalformat, width, height, border, format,
                 type, pixels);
  // Is the internal format the number of components?
  if (0 < internalformat && internalformat <= 4) {
    TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexImage2D("
               << Gl_wrapper::find(target) << ", " << level << ", "
               << internalformat << ", "
               << width << ", " << height << ", " << border << ", "
               << Gl_wrapper::find(format) << ", "
               << Gl_wrapper::find(type) << ", pixels);"
               << std::endl;);
  } else {
    TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexImage2D("
               << Gl_wrapper::find(target) << ", " << level << ", "
               << Gl_wrapper::find(internalformat) << ", "
               << width << ", " << height << ", " << border << ", "
               << Gl_wrapper::find(format) << ", "
               << Gl_wrapper::find(type) << ", pixels);"
               << std::endl;);
  }
}

/*! glTexParameterf wrapper */
inline void glTexParameterf(GLenum target, GLenum pname, GLfloat param) {
  ::glTexParameterf(target, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameterf("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glTexParameterfv wrapper */
inline void glTexParameterfv(GLenum target, GLenum pname,
                             const GLfloat* params) {
  ::glTexParameterfv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameterfv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glTexParameteri wrapper */
inline void glTexParameteri(GLenum target, GLenum pname, GLint param) {
  ::glTexParameteri(target, pname, param);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameteri("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ", "
             << Gl_wrapper::find(param) << ");"
             << std::endl;);
}

/*! glTexParameteriv wrapper */
inline void glTexParameteriv(GLenum target, GLenum pname,
                             const GLint* params) {
  ::glTexParameteriv(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameteriv("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ");"
             << std::endl;);
}

/*! glTexSubImage1D wrapper */
inline void glTexSubImage1D(GLenum target, GLint level, GLint xoffset,
                            GLsizei width, GLenum format, GLenum type,
                            const GLvoid* pixels) {
  ::glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexSubImage1D("
             << Gl_wrapper::find(target) << ", " << level << ");"
             << std::endl;);
}

/*! glTexSubImage2D wrapper */
inline void glTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                            GLint yoffset, GLsizei width, GLsizei height,
                            GLenum format, GLenum type, const GLvoid* pixels)
{
  ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
                    type, pixels);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexSubImage2D("
             << Gl_wrapper::find(target) << ", " << level << ");"
             << std::endl;);
}

/*! glTranslated wrapper */
inline void glTranslated(GLdouble x, GLdouble y, GLdouble z) {
  ::glTranslated(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTranslated("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glTranslatef wrapper */
inline void glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
  ::glTranslatef(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glTranslatef("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glVertex2d wrapper */
inline void glVertex2d(GLdouble x, GLdouble y) {
  ::glVertex2d(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2d("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glVertex2dv wrapper */
inline void glVertex2dv(const GLdouble* v) {
  ::glVertex2dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2dv(" << ");"
             << std::endl;);
}

/*! vertex wrapper */
inline void glVertex2f(const GLfloat v1, const GLfloat v2) {
  ::glVertex2f(v1, v2);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glVertex2fGLfloat(" << v1 << "," << v2 << ");"
             << std::endl;);
}

/*! glVertex2fv wrapper */
inline void glVertex2fv(const GLfloat* v) {
  ::glVertex2fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "GLfloat(" << ");"
             << std::endl;);
}

/*! glVertex2i wrapper */
inline void glVertex2i(GLint x, GLint y) {
  ::glVertex2i(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2i("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glVertex2iv wrapper */
inline void glVertex2iv(const GLint* v) {
  ::glVertex2iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2iv(" << ");"
             << std::endl;);
}

/*! glVertex2s wrapper */
inline void glVertex2s(GLshort x, GLshort y) {
  ::glVertex2s(x, y);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2s("
             << x << ", " << y << ");"
             << std::endl;);
}

/*! glVertex2sv wrapper */
inline void glVertex2sv(const GLshort* v) {
  ::glVertex2sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2sv(" << ");"
             << std::endl;);
}

/*! glVertex3d wrapper */
inline void glVertex3d(GLdouble x, GLdouble y, GLdouble z) {
  ::glVertex3d(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3d("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glVertex3dv wrapper */
inline void glVertex3dv(const GLdouble* v) {
  ::glVertex3dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3dv(" << ");"
             << std::endl;);
}

/*! glVertex3f wrapper */
inline void glVertex3f(GLfloat x, GLfloat y, GLfloat z) {
  ::glVertex3f(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3f("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! vertex wrapper */
inline void glVertex3fv(const GLfloat* v) {
  ::glVertex3fv(v);
  TRACE_CODE(Trace::GRAPHICS,
             std::cout << "glVertex3fv(" << *((Vector3f *) v) << ");"
             << std::endl;);
}

/*! glVertex3i wrapper */
inline void glVertex3i(GLint x, GLint y, GLint z) {
  ::glVertex3i(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3i("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glVertex3iv wrapper */
inline void glVertex3iv(const GLint* v) {
  ::glVertex3iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3iv(" << ");"
             << std::endl;);
}

/*! glVertex3s wrapper */
inline void glVertex3s(GLshort x, GLshort y, GLshort z) {
  ::glVertex3s(x, y, z);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3s("
             << x << ", " << y << ", " << z << ");"
             << std::endl;);
}

/*! glVertex3sv wrapper */
inline void glVertex3sv(const GLshort* v) {
  ::glVertex3sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3sv(" << ");"
             << std::endl;);
}

/*! glVertex4d wrapper */
inline void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  ::glVertex4d(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4d("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glVertex4dv wrapper */
inline void glVertex4dv(const GLdouble* v) {
  ::glVertex4dv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4dv(" << ");"
             << std::endl;);
}

/*! glVertex4f wrapper */
inline void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  ::glVertex4f(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4f("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glVertex4fv wrapper */
inline void glVertex4fv(const GLfloat* v) {
  ::glVertex4fv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4fv(" << ");"
             << std::endl;);
}

/*! glVertex4i wrapper */
inline void glVertex4i(GLint x, GLint y, GLint z, GLint w) {
  ::glVertex4i(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4i("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glVertex4iv wrapper */
inline void glVertex4iv(const GLint* v) {
  ::glVertex4iv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4iv(" << ");"
             << std::endl;);
}

/*! glVertex4s wrapper */
inline void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w) {
  ::glVertex4s(x, y, z, w);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4s("
             << x << ", " << y << ", " << z << ", " << w << ");"
             << std::endl;);
}

/*! glVertex4sv wrapper */
inline void glVertex4sv(const GLshort* v) {
  ::glVertex4sv(v);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4sv(" << ");"
             << std::endl;);
}

/*! glVertexPointer wrapper */
inline void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                            const GLvoid* pointer) {
  ::glVertexPointer(size, type, stride, pointer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertexPointer("
             << size
             << "," << Gl_wrapper::find(type)
             << "," << stride
             << "," << "pointer" << ");"
             << std::endl;);
}

/*! glViewport wrapper */
inline void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  ::glViewport(x, y, width, height);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glViewport("
             << x << ", " << y << ", " << width << ", " << height << ");"
             << std::endl;);
}

#if defined(GL_ARB_vertex_buffer_object)
#ifdef GL_GLEXT_PROTOTYPES
inline void glBindBufferARB(GLenum target, GLuint buffer) {
  ::glBindBufferARB(target, buffer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glBindBufferARB("
             << Gl_wrapper::find(target) << ", " << buffer << ");"
             << std::endl;);
}

inline void glDeleteBuffersARB(GLsizei n, GLuint* buffers) {
  ::glDeleteBuffersARB(n, buffers);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glDeleteBuffersARB("
             << n << ", " << "buffers" << ");"
             << std::endl;);
}

inline void glGenBuffersARB(GLsizei n, GLuint* buffers) {
  ::glGenBuffersARB(n, buffers);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGenBuffersARB("
             << n << ", " << "buffers" << ");"
             << std::endl;);
}

inline GLboolean glIsBufferARB(GLuint buffer) {
  return ::glIsBufferARB(buffer);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glIsBufferARB("
             << buffer << ");"
             << std::endl;);
}

inline void glBufferDataARB(GLenum target, GLsizei size, GLvoid* data,
                            GLenum usage) {
  ::glBufferDataARB(target, size, data, usage);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glBufferDataARB("
             << Gl_wrapper::find(target) << ", " << size << ", "
             << "data" << ", "
             << Gl_wrapper::find(usage) << ");"
             << std::endl;);
}

inline void glBufferSubDataARB(GLenum target, GLint offset, GLsizei size,
                               GLvoid* data) {
  ::glBufferSubDataARB(target, offset, size, data);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glBufferSubDataARB("
             << Gl_wrapper::find(target) << ", " << offset << ", "
             << size << ", " << "data" << ");"
             << std::endl;);
}

inline void glGetBufferSubDataARB(GLenum target, GLint offset, GLsizei size,
                                  GLvoid* data) {
  ::glGetBufferSubDataARB(target, offset, size, data);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBufferSubDataARB("
             << Gl_wrapper::find(target) << ", " << offset << ", "
             << size << ", " << "data" << ");"
             << std::endl;);
}

inline GLvoid* glMapBufferARB(GLenum target, GLenum access) {
  return ::glMapBufferARB(target, access);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapBufferARB("
             << Gl_wrapper::find(target) << ", " << Gl_wrapper::find(access)
             << ");"
             << std::endl;);
}

inline GLboolean glUnmapBufferARB(GLenum target) {
  return ::glUnmapBufferARB(target);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glUnmapBufferARB("
             << Gl_wrapper::find(target) << ");"
             << std::endl;);
}

inline void glGetBufferParameterivARB(GLenum target, GLenum pname,
                                      GLint* params) {
  ::glGetBufferParameterivARB(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBufferParameterivARB("
             << Gl_wrapper::find(target) << ", " <<
             Gl_wrapper::find(pname) << ", " << "params" << ");"
             << std::endl;);
}

inline void glGetBufferPointervARB(GLenum target, GLenum pname,
                                   GLvoid ** params) {
  ::glGetBufferPointervARB(target, pname, params);
  TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBufferPointervARB("
             << Gl_wrapper::find(target) << ", "
             << Gl_wrapper::find(pname) << ", " << "params" << ");"
             << std::endl;);
}
#endif
#endif

#if 0
/* EXT_vertex_array */
typedef void glARRAYELEMENTEXTPROC(GLint i);
typedef void glDRAWARRAYSEXTPROC(GLenum mode, GLint first, GLsizei count);
typedef void glVERTEXPOINTEREXTPROC(GLint size, GLenum type, GLsizei stride,
                                    GLsizei count, const GLvoid* pointer);
typedef void glNORMALPOINTEREXTPROC(GLenum type, GLsizei stride,
                                    GLsizei count, const GLvoid* pointer);
typedef void glCOLORPOINTEREXTPROC(GLint size, GLenum type, GLsizei stride,
                                   GLsizei count, const GLvoid* pointer);
typedef void glINDEXPOINTEREXTPROC(GLenum type, GLsizei stride, GLsizei count,
                                   const GLvoid* pointer);
typedef void glTEXCOORDPOINTEREXTPROC(GLint size, GLenum type, GLsizei stride,
                                      GLsizei count, const GLvoid* pointer);
typedef void glEDGEFLAGPOINTEREXTPROC(GLsizei stride, GLsizei count,
                                      const GLboolean* pointer);
typedef void glGETPOINTERVEXTPROC(GLenum pname, GLvoid ** params);
typedef void glARRAYELEMENTARRAYEXTPROC(GLenum mode, GLsizei count,
                                        const GLvoid* pi);

/* WIN_draw_range_elements */
typedef void glDRAWRANGEELEMENTSWINPROC(GLenum mode, GLuint start, GLuint end,
                                        GLsizei count, GLenum type,
                                        const GLvoid* indices);

/* WIN_swap_hint */
typedef void glADDSWAPHINTRECTWINPROC(GLint x, GLint y, GLsizei width,
                                      GLsizei height);

/* EXT_paletted_texture */
typedef void glCOLORTABLEEXTPROC(GLenum target, GLenum internalFormat,
                                 GLsizei width, GLenum format,
                                 GLenum type, const GLvoid* data);
typedef void glCOLORSUBTABLEEXTPROC(GLenum target, GLsizei start,
                                    GLsizei count, GLenum format,
                                    GLenum type, const GLvoid* data);
typedef void glGETCOLORTABLEEXTPROC(GLenum target, GLenum format, GLenum type,
                                    GLvoid* data);
typedef void glGETCOLORTABLEPARAMETERIVEXTPROC(GLenum target, GLenum pname,
                                               GLint* params);
typedef void glGETCOLORTABLEPARAMETERFVEXTPROC(GLenum target, GLenum pname,
                                               GLfloat* params);

#endif

SGAL_END_NAMESPACE

#endif
