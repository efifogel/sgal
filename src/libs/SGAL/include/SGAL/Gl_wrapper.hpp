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
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Gl_wrapper {
private:
  typedef std::map<GLenum, const char*> Name_map;
  typedef Name_map::const_iterator      Name_const_iter;
  
  /*! The singletone. */
  static Gl_wrapper* s_instance;

  /*! The search data structure. */
  Name_map m_gl_token_names;

  /*! Private Constructor. */
  Gl_wrapper();

  /*! Find the name of a token. */
  const char* find_name(GLenum num);

  /*! Find the name of a Boolean token. */
  const char* find_boolean_name(GLboolean flag);
  
  /*! Check whether openGl errors have occured. */ 
  void check_error();
  
public:
  /*! Obtain the singletone. */
  static Gl_wrapper* get_instance();

  /*! Destructor. */
  ~Gl_wrapper();

  /*! Find the name of a token. */
  static const char* find(GLenum num);

  /*! Find the name of a token. */
  static const char* find_boolean(GLboolean flag);
  
  /*! Check for GL errors. */
  static void check();
};

#if defined(NDEBUG)
#define SGAL_CHECK_GL()
#else
#define SGAL_CHECK_GL() Gl_wrapper::check()
#endif

/*! \brief finds the name of a Boolean token. */
inline const char* Gl_wrapper::find_boolean_name(GLboolean flag)
{ return (flag == GL_FALSE) ? "GL_FALSE" : "GL_TRUE"; }

/*! \brief finds the name of a token. */
inline const char* Gl_wrapper::find(GLenum num)
{ return get_instance()->find_name(num); }

/*! \brief finds the name of a token. */
inline const char* Gl_wrapper::find_boolean(GLboolean flag)
{ return get_instance()->find_boolean_name(flag); }
  
/*! \brief checks for GL errors. */
inline void Gl_wrapper::check() { get_instance()->check_error(); }

// Free function wrappers:

/*! glAccum wrapper */
inline void glAccum(GLenum op, GLfloat value)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glAccum("
                  << Gl_wrapper::find(op)
                  << value << ");"
                  << std::endl;);
  ::glAccum(op, value);
  SGAL_CHECK_GL();
}

/*! glAlphaFunc wrapper */
inline void glAlphaFunc(GLenum func, GLclampf ref)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glAlphaFunc("
                  << Gl_wrapper::find(func) << ", " << ref << ");"
                  << std::endl;);
  ::glAlphaFunc(func, ref);
  SGAL_CHECK_GL();
}

/*! glAreTexturesResident wrapper */
inline GLboolean glAreTexturesResident(GLsizei n, const GLuint* textures,
                                       GLboolean* residences)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glAreTexturesResident(" << n
                  << ");"
                  << std::endl;);
  GLboolean res = ::glAreTexturesResident(n, textures, residences);
  SGAL_CHECK_GL();
  return res;
}

/*! glArrayElement wrapper */
inline void glArrayElement(GLint i)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glArrayElement(" << i << ");"
             << std::endl;);
  ::glArrayElement(i);
  SGAL_CHECK_GL();
}

/*! glBegin wrapper */
inline void glBegin(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glBegin(" << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glBegin(mode);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glBindTexture wrapper */
inline void glBindTexture(GLenum target, GLuint texture)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glBindTexture("
                  << Gl_wrapper::find(target) << ", " << texture << ");"
                  << std::endl;);
  ::glBindTexture(target,texture );
  SGAL_CHECK_GL();
}

/*! glBitmap wrapper */
inline void glBitmap(GLsizei width, GLsizei height, GLfloat xorig,
                     GLfloat yorig, GLfloat xmove, GLfloat ymove,
                     const GLubyte* bitmap)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glBitmap("
                  << width << ", " << height << ", " << xorig << ", " << yorig
                  << ", " << xmove << ", " << ymove << ");"
                  << std::endl;);
  ::glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap);
  SGAL_CHECK_GL();
}

/*! glBlendFunc wrapper */
inline void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glBlendFunc("
                  << Gl_wrapper::find(sfactor) << ", "
                  << Gl_wrapper::find(dfactor) << ");"
                  << std::endl;);
  ::glBlendFunc(sfactor, dfactor);
  SGAL_CHECK_GL();
}

/*! glCallList wrapper */
inline void glCallList(GLuint list)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCallList(" << list << ");"
                  << std::endl;);
  ::glCallList(list);
  SGAL_CHECK_GL();
}

/*! glCallLists wrapper */
inline void glCallLists(GLsizei n, GLenum type, const GLvoid* lists)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCallLists(" << n << ", " 
                  << Gl_wrapper::find(type) << ");"
                  << std::endl;);
  ::glCallLists(n, type, lists);
  SGAL_CHECK_GL();
}

/*! glClear wrapper */
inline void glClear(GLbitfield mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClear(" << std::hex << mask
                  << ");"
                  << std::endl;);
  ::glClear(mask);
  SGAL_CHECK_GL();
}

/*! glClearAccum wrapper */
inline void glClearAccum(GLfloat red, GLfloat green, GLfloat blue,
                         GLfloat alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearAccum("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glClearAccum(red, green, blue, alpha);
  SGAL_CHECK_GL();
}

/*! glClearColor wrapper */
inline void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha)
{
  ::glClearColor(red, green, blue, alpha);
  SGAL_CHECK_GL();
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearColor("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
}

/*! glClearDepth wrapper */
inline void glClearDepth(GLclampd depth)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearDepth(" << depth << ");"
                  << std::endl;);
  ::glClearDepth(depth);
  SGAL_CHECK_GL();
}

/*! glClearIndex wrapper */
inline void glClearIndex(GLfloat c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearIndex(" << c << ");"
                  << std::endl;);
  ::glClearIndex(c);
  SGAL_CHECK_GL();
}

/*! glClearStencil wrapper */
inline void glClearStencil(GLint s)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClearStencil(" << s << ");"
                  << std::endl;);
  ::glClearStencil(s);
  SGAL_CHECK_GL();
}

/*! glClipPlane wrapper */
inline void glClipPlane(GLenum plane, const GLdouble* equation)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glClipPlane("
                  << Gl_wrapper::find(plane) << ");"
                  << std::endl;);
  ::glClipPlane(plane, equation);
  SGAL_CHECK_GL();
}

/*! glColor3b wrapper */
inline void glColor3b(GLbyte red, GLbyte green, GLbyte blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3b("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3b(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3bv wrapper */
inline void glColor3bv(const GLbyte* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3bv(" << ");"
                  << std::endl;);
  ::glColor3bv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3d wrapper */
inline void glColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3d("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3d(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3dv wrapper */
inline void glColor3dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3dv(" << ");"
                  << std::endl;);
  ::glColor3dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3f wrapper */
inline void glColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3f(" 
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3f(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! color wrapper */
inline void glColor3fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glColor3fv(" << *((Vector3f *) v) << ");"
                  << std::endl;);
  ::glColor3fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3i wrapper */
inline void glColor3i(GLint red, GLint green, GLint blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3i("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3i(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3iv wrapper */
inline void glColor3iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3iv(" << ");"
                  << std::endl;);
  ::glColor3iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3s wrapper */
inline void glColor3s(GLshort red, GLshort green, GLshort blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3s("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3s(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3sv wrapper */
inline void glColor3sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3sv(" << ");"
                  << std::endl;);
  ::glColor3sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3ub wrapper */
inline void glColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3ub("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3ub(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3ubv wrapper */
inline void glColor3ubv(const GLubyte* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3ubv(" << ");"
                  << std::endl;);
  ::glColor3ubv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3ui wrapper */
inline void glColor3ui(GLuint red, GLuint green, GLuint blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3ui("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3ui(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3uiv wrapper */
inline void glColor3uiv(const GLuint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3uiv(" << ");"
                  << std::endl;);
  ::glColor3uiv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3us wrapper */
inline void glColor3us(GLushort red, GLushort green, GLushort blue)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3us("
                  << red << ", " << green << ", " << blue << ");"
                  << std::endl;);
  ::glColor3us(red, green, blue);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor3usv wrapper */
inline void glColor3usv(const GLushort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor3usv(" << ");"
                  << std::endl;);
  ::glColor3usv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4b wrapper */
inline void glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4b("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glColor4b(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4bv wrapper */
inline void glColor4bv(const GLbyte* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4bv(" << ");"
                  << std::endl;);
  ::glColor4bv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4d wrapper */
inline void glColor4d(GLdouble red, GLdouble green, GLdouble blue,
                      GLdouble alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4d("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glColor4d(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4dv wrapper */
inline void glColor4dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4dv(" << ");"
                  << std::endl;);
  ::glColor4dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4f wrapper */
inline void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4f("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glColor4f(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4fv wrapper */
inline void glColor4fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4fv(" << ");"
                  << std::endl;);
  ::glColor4fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4i wrapper */
inline void glColor4i(GLint red, GLint green, GLint blue, GLint alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4i("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glColor4i(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4iv wrapper */
inline void glColor4iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4iv(" << ");"
                  << std::endl;);
  ::glColor4iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4s wrapper */
inline void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4s("
                  << red << ", " << green << ", "
                  << blue << ", " << alpha << ");"
                  << std::endl;);
  ::glColor4s(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4sv wrapper */
inline void glColor4sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4sv(" << ");"
                  << std::endl;);
  ::glColor4sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4ub wrapper */
inline void glColor4ub(GLubyte red, GLubyte green, GLubyte blue,
                       GLubyte alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4ub("
                  << (Uint) red << ", " << (Uint) green << ", "
                  << (Uint) blue << ", " << (Uint) alpha << ");"
                  << std::endl;);
  ::glColor4ub(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4ubv wrapper */
inline void glColor4ubv(const GLubyte* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4ubv(" << ");"
                  << std::endl;);
  ::glColor4ubv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4ui wrapper */
inline void glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4ui("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glColor4ui(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4uiv wrapper */
inline void glColor4uiv(const GLuint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4uiv(" << ");"
                  << std::endl;);
  ::glColor4uiv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4us wrapper */
inline void glColor4us(GLushort red, GLushort green, GLushort blue,
                       GLushort alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4us("
                  << red << ", " << green << ", " << blue << ", "
                  << alpha << ");"
                  << std::endl;);
  ::glColor4us(red, green, blue, alpha);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColor4usv wrapper */
inline void glColor4usv(const GLushort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColor4usv(" << ");"
                  << std::endl;);
  ::glColor4usv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glColorMask wrapper */
inline void glColorMask(GLboolean red, GLboolean green, GLboolean blue,
                        GLboolean alpha)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColorMask("
                  << Gl_wrapper::find_boolean(red) << ", "
                  << Gl_wrapper::find_boolean(green) << ", "
                  << Gl_wrapper::find_boolean(blue) << ", "
                  << Gl_wrapper::find_boolean(alpha) << ");"
                  << std::endl;);
  ::glColorMask(red, green, blue, alpha);
  SGAL_CHECK_GL();
}

/*! glColorMaterial wrapper */
inline void glColorMaterial(GLenum face, GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColorMaterial(" << ");"
                  << std::endl;);
  ::glColorMaterial(face, mode);
  SGAL_CHECK_GL();
}

/*! glColorPointer wrapper */
inline void glColorPointer(GLint size, GLenum type, GLsizei stride,
                           const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glColorPointer("
                  << size << ", "
                  << Gl_wrapper::find(type) << ", " << stride << ");"
                  << std::endl;);
  ::glColorPointer(size, type, stride, pointer);
  SGAL_CHECK_GL();
}

/*! glCopyPixels wrapper */
inline void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                         GLenum type)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyPixels("
                  << x << ", " << y << ", " << width << ",  " << height
                  << Gl_wrapper::find(type) << ");"
             << std::endl;);
  ::glCopyPixels(x, y, width, height, type);
  SGAL_CHECK_GL();
}

/*! glCopyTexImage1D wrapper */
inline void glCopyTexImage1D(GLenum target, GLint level,
                             GLenum internalFormat, GLint x, GLint y,
                             GLsizei width, GLint border)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexImage1D("
                  << Gl_wrapper::find(target) << ", " << level << ", "
                  << Gl_wrapper::find(internalFormat) << ", "
                  << x << ", " << y << ", " << width << ", " << border << ");"
                  << std::endl;);
  ::glCopyTexImage1D(target, level, internalFormat, x, y, width, border);
  SGAL_CHECK_GL();
}

/*! glCopyTexImage2D wrapper */
inline void glCopyTexImage2D(GLenum target, GLint level,
                             GLenum internalFormat, GLint x, GLint y,
                             GLsizei width, GLsizei height, GLint border)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexImage2D("
                  << Gl_wrapper::find(target) << ", " << level << ", "
                  << Gl_wrapper::find(internalFormat) << ", "
                  << x << ", " << y << ", "
                  << width << ", " << height << ", " << border << ");"
                  << std::endl;);
  ::glCopyTexImage2D(target, level, internalFormat, x, y, width, height,
                     border);
  SGAL_CHECK_GL();
}

/*! glCopyTexSubImage1D wrapper */
inline void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset,
                                GLint x, GLint y, GLsizei width)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexSubImage1D("
                  << Gl_wrapper::find(target) << ", " << level << ");"
                  << std::endl;);
  ::glCopyTexSubImage1D(target, level, xoffset, x, y, width);
  SGAL_CHECK_GL();
}

/*! glCopyTexSubImage2D wrapper */
inline void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                                GLint yoffset, GLint x, GLint y,
                                GLsizei width, GLsizei height)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCopyTexSubImage2D("
                  << Gl_wrapper::find(target) << ", " << level << ");"
                  << std::endl;);
  ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
  SGAL_CHECK_GL();
}

/*! glCullFace wrapper */
inline void glCullFace(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glCullFace("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glCullFace(mode);
  SGAL_CHECK_GL();
}

/*! glDeleteLists wrapper */
inline void glDeleteLists(GLuint list, GLsizei range)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDeleteLists(" << list << ");"
                  << std::endl;);
  ::glDeleteLists(list, range);
  SGAL_CHECK_GL();
}

/*! glDeleteTextures wrapper */
inline void glDeleteTextures(GLsizei n, const GLuint* textures)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDeleteTextures(" << n << ");"
                  << std::endl;);
  ::glDeleteTextures(n, textures);
  SGAL_CHECK_GL();
}

/*! glDepthFunc wrapper */
inline void glDepthFunc(GLenum func)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDepthFunc("
                  << Gl_wrapper::find(func) << ");"
                  << std::endl;);
  ::glDepthFunc(func);
  SGAL_CHECK_GL();
}

/*! glDepthMask wrapper */
inline void glDepthMask(GLboolean flag)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDepthMask("
                  << Gl_wrapper::find_boolean(flag) << ");"
                  << std::endl;);
  ::glDepthMask(flag);
  SGAL_CHECK_GL();
}

/*! glDepthRange wrapper */
inline void glDepthRange(GLclampd zNear, GLclampd zFar)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDepthRange("
                  << zNear << ", " << zFar << ");"
                  << std::endl;);
  ::glDepthRange(zNear, zFar);
  SGAL_CHECK_GL();
}

/*! glDisable wrapper */
inline void glDisable(GLenum cap)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDisable("
                  << Gl_wrapper::find(cap) << ");\n";);
  ::glDisable(cap);
  SGAL_CHECK_GL();
}

/*! glDisableClientState wrapper */
inline void glDisableClientState(GLenum array)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDisableClientState("
                  << Gl_wrapper::find(array) << ");"
                  << std::endl;);
  ::glDisableClientState(array);
  SGAL_CHECK_GL();
}

/*! glDrawArrays wrapper */
inline void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawArrays("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glDrawArrays(mode, first, count);
  SGAL_CHECK_GL();
}

/*! glDrawBuffer wrapper */
inline void glDrawBuffer(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawBuffer("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glDrawBuffer(mode);
  SGAL_CHECK_GL();
}

/*! glDrawElements wrapper */
inline void glDrawElements(GLenum mode, GLsizei count, GLenum type,
                           const GLvoid* indices)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawElements("
                  << Gl_wrapper::find(mode)
                  << "," << std::dec << count
                  << "," << Gl_wrapper::find(type)
                  << "," << "indices" << ");"
                  << std::endl;);
  ::glDrawElements(mode, count, type, indices);
  SGAL_CHECK_GL();
}

/*! glDrawPixels wrapper */
inline void glDrawPixels(GLsizei width, GLsizei height, GLenum format,
                         GLenum type, const GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDrawPixels("
                  << width << ", " << height << ","
                  << Gl_wrapper::find(format) << ", "
                  << Gl_wrapper::find(type) << ");"
                  << std::endl;);
  ::glDrawPixels(width, height, format, type, pixels);
  SGAL_CHECK_GL();
}

/*! glEdgeFlag wrapper */
inline void glEdgeFlag(GLboolean flag)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEdgeFlag("
                  << Gl_wrapper::find_boolean(flag) << ");"
                  << std::endl;);
  ::glEdgeFlag(flag);
  SGAL_CHECK_GL();
}

/*! glEdgeFlagPointer wrapper */
inline void glEdgeFlagPointer(GLsizei stride, const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEdgeFlagPointer(" << ");"
                  << std::endl;);
  ::glEdgeFlagPointer(stride, pointer);
  SGAL_CHECK_GL();
}

/*! glEdgeFlagv wrapper */
inline void glEdgeFlagv(const GLboolean* flag)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEdgeFlagv("
                  << flag << ");"
                  << std::endl;);
  ::glEdgeFlagv(flag);
  SGAL_CHECK_GL();
}

/*! glEnable wrapper */
inline void glEnable(GLenum cap)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEnable("
                  << Gl_wrapper::find(cap) << ");"
                  << std::endl;);
  ::glEnable(cap);
  SGAL_CHECK_GL();
}

/*! glEnableClientState wrapper */
inline void glEnableClientState(GLenum array)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEnableClientState("
                  << Gl_wrapper::find(array) << ");"
                  << std::endl;);
  ::glEnableClientState(array);
  SGAL_CHECK_GL();
}

/*! end wrapper */
inline void glEnd(void)
{
  SGAL_TRACE_MSG(Trace::GRAPHICS, "glEnd();\n");
  ::glEnd();
  SGAL_CHECK_GL();
}

/*! glEndList wrapper */
inline void glEndList(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEndList();"
                  << std::endl;);
  ::glEndList();
  SGAL_CHECK_GL();
}

/*! glEvalCoord1d wrapper */
inline void glEvalCoord1d(GLdouble u)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1d(" << u << ");"
                  << std::endl;);
  ::glEvalCoord1d(u);
  SGAL_CHECK_GL();
}

/*! glEvalCoord1dv wrapper */
inline void glEvalCoord1dv(const GLdouble* u)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1dv(" << ");"
                  << std::endl;);
  ::glEvalCoord1dv(u);
  SGAL_CHECK_GL();
}

/*! glEvalCoord1f wrapper */
inline void glEvalCoord1f(GLfloat u)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1f(" << u << ");"
                  << std::endl;);
  ::glEvalCoord1f(u);
  SGAL_CHECK_GL();
}

/*! glEvalCoord1fv wrapper */
inline void glEvalCoord1fv(const GLfloat* u)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord1fv(" << ");"
                  << std::endl;);
  ::glEvalCoord1fv(u);
  SGAL_CHECK_GL();
}

/*! glEvalCoord2d wrapper */
inline void glEvalCoord2d(GLdouble u, GLdouble v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2d("
                  << u << ", " << v << ");"
                  << std::endl;);
  ::glEvalCoord2d(u, v);
  SGAL_CHECK_GL();
}

/*! glEvalCoord2dv wrapper */
inline void glEvalCoord2dv(const GLdouble* u)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2dv(" << ");"
                  << std::endl;);
  ::glEvalCoord2dv(u);
  SGAL_CHECK_GL();
}

/*! glEvalCoord2f wrapper */
inline void glEvalCoord2f(GLfloat u, GLfloat v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2f("
                  << u << ", " << v << ");"
                  << std::endl;);
  ::glEvalCoord2f(u, v);
  SGAL_CHECK_GL();
}

/*! glEvalCoord2fv wrapper */
inline void glEvalCoord2fv(const GLfloat* u)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalCoord2fv(" << ");"
                  << std::endl;);
  ::glEvalCoord2fv(u);
  SGAL_CHECK_GL();
}

/*! glEvalMesh1 wrapper */
inline void glEvalMesh1(GLenum mode, GLint i1, GLint i2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalMesh1("
                  << Gl_wrapper::find(mode) << ", " << i1 << ", " << i2 << ");"
                  << std::endl;);
  ::glEvalMesh1(mode, i1, i2);
  SGAL_CHECK_GL();
}

/*! glEvalMesh2 wrapper */
inline void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalMesh2("
                  << Gl_wrapper::find(mode) << ", " << i1 << ", " << i2
                  << j1 << ", " << j2 << ");"
                  << std::endl;);
  ::glEvalMesh2(mode, i1, i2, j1, j2);
  SGAL_CHECK_GL();
}

/*! glEvalPoint1 wrapper */
inline void glEvalPoint1(GLint i)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalPoint1(" << i << ");"
                  << std::endl;);
  ::glEvalPoint1(i);
  SGAL_CHECK_GL();
}

/*! glEvalPoint2 wrapper */
inline void glEvalPoint2(GLint i, GLint j)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glEvalPoint2("
                  << i << ", " << j << ");"
                  << std::endl;);
  ::glEvalPoint2(i, j);
  SGAL_CHECK_GL();
}

/*! glFeedbackBuffer wrapper */
inline void glFeedbackBuffer(GLsizei size, GLenum type, GLfloat* buffer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFeedbackBuffer("
                  << size << ", "
                  << Gl_wrapper::find(type) << ");"
                  << std::endl;);
  ::glFeedbackBuffer(size, type, buffer);
  SGAL_CHECK_GL();
}

/*! glFinish wrapper */
inline void glFinish(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFinish();"
                  << std::endl;);
  ::glFinish();
  SGAL_CHECK_GL();
}

/*! glFlush wrapper */
inline void glFlush(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFlush();"
                  << std::endl;);
  ::glFlush();
  SGAL_CHECK_GL();
}

/*! glFogf wrapper */
inline void glFogf(GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogf("
                  << Gl_wrapper::find(pname) << ", " << param << ");"
                  << std::endl;);
  ::glFogf(pname, param);
  SGAL_CHECK_GL();
}

/*! glFogfv wrapper */
inline void glFogfv(GLenum pname, const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogfv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glFogfv(pname, params);
  SGAL_CHECK_GL();
}

/*! glFogi wrapper */
inline void glFogi(GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogi("
                  << Gl_wrapper::find(pname) << ", " << Gl_wrapper::find(param)
                  << ");"
                  << std::endl;);
  ::glFogi(pname, param);
  SGAL_CHECK_GL();
}

/*! glFogiv wrapper */
inline void glFogiv(GLenum pname, const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFogiv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glFogiv(pname, params);
  SGAL_CHECK_GL();
}


/*! glFrontFace wrapper */
inline void glFrontFace(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFrontFace("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glFrontFace(mode);
  SGAL_CHECK_GL();
}

/*! glFrustum wrapper */
inline void glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
                      GLdouble top, GLdouble zNear, GLdouble zFar)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glFrustum("
                  << left << ", " << right << ", " << bottom << ", "
                  << top << ", " << zNear << ", " << zFar << ");"
                  << std::endl;);
  ::glFrustum(left, right, bottom, top, zNear, zFar);
  SGAL_CHECK_GL();
}

/*! GLuint wrapper */
inline GLuint glGenLists(GLsizei range)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGenLists(" << range << ");"
                  << std::endl;);
  GLuint res = ::glGenLists(range);
  SGAL_CHECK_GL();
  return res;
}

/*! glGenTextures wrapper */
inline void glGenTextures(GLsizei n, GLuint* textures)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGenTextures(" << n << ");"
                  << std::endl;);
  ::glGenTextures(n, textures);
  SGAL_CHECK_GL();
}

/*! glGetBooleanv wrapper */
inline void glGetBooleanv(GLenum pname, GLboolean* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBooleanv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetBooleanv(pname, params);
  SGAL_CHECK_GL();
}

/*! glGetClipPlane wrapper */
inline void glGetClipPlane(GLenum plane, GLdouble* equation)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetClipPlane(" << ");"
                  << std::endl;);
  ::glGetClipPlane(plane, equation);
  SGAL_CHECK_GL();
}

/*! glGetDoublev wrapper */
inline void glGetDoublev(GLenum pname, GLdouble* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetDoublev("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetDoublev(pname, params);
  SGAL_CHECK_GL();
}

/*! GLenum wrapper */
inline GLenum glGetError(void)
{ return ::glGetError(); }

/*! glGetFloatv wrapper */
inline void glGetFloatv(GLenum pname, GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetFloatv("
                  << Gl_wrapper::find(pname) << ", params);"
                  << std::endl;);
  ::glGetFloatv(pname, params);
  SGAL_CHECK_GL();
}

/*! glGetIntegerv wrapper */
inline void glGetIntegerv(GLenum pname, GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetIntegerv("
                  << Gl_wrapper::find(pname) << ", params);"
                  << std::endl;);
  ::glGetIntegerv(pname, params);
  SGAL_CHECK_GL();
}

/*! glGetLightfv wrapper */
inline void glGetLightfv(GLenum light, GLenum pname, GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetLightfv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetLightfv(light, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetLightiv wrapper */
inline void glGetLightiv(GLenum light, GLenum pname, GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetLightiv("
                  << Gl_wrapper::find(pname) << ", params);"
                  << std::endl;);
  ::glGetLightiv(light, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetMapdv wrapper */
inline void glGetMapdv(GLenum target, GLenum query, GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMapdv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(query) << ");"
                  << std::endl;);
  SGAL_CHECK_GL();
  ::glGetMapdv(target, query, v);
}

/*! glGetMapfv wrapper */
inline void glGetMapfv(GLenum target, GLenum query, GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMapfv("
                  << Gl_wrapper::find(target) << ", " << query << ");"
                  << std::endl;);
  ::glGetMapfv(target, query, v);
  SGAL_CHECK_GL();
}

/*! glGetMapiv wrapper */
inline void glGetMapiv(GLenum target, GLenum query, GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMapiv("
                  << Gl_wrapper::find(target) << ", " << query << ");"
                  << std::endl;);
  ::glGetMapiv(target, query, v);
  SGAL_CHECK_GL();
}

/*! glGetMaterialfv wrapper */
inline void glGetMaterialfv(GLenum face, GLenum pname, GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMaterialfv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetMaterialfv(face, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetMaterialiv wrapper */
inline void glGetMaterialiv(GLenum face, GLenum pname, GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetMaterialiv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  SGAL_CHECK_GL();
  ::glGetMaterialiv(face, pname, params);
}

/*! glGetPixelMapfv wrapper */
inline void glGetPixelMapfv(GLenum map, GLfloat* values)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPixelMapfv(" << ");"
                  << std::endl;);
  ::glGetPixelMapfv(map, values);
  SGAL_CHECK_GL();
}

/*! glGetPixelMapuiv wrapper */
inline void glGetPixelMapuiv(GLenum map, GLuint* values)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPixelMapuiv(" << ");"
                  << std::endl;);
  ::glGetPixelMapuiv(map, values);
  SGAL_CHECK_GL();
}

/*! glGetPixelMapusv wrapper */
inline void glGetPixelMapusv(GLenum map, GLushort* values)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPixelMapusv(" << ");"
                  << std::endl;);
  ::glGetPixelMapusv(map, values);
  SGAL_CHECK_GL();
}

/*! glGetPointerv wrapper */
inline void glGetPointerv(GLenum pname, GLvoid** params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPointerv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetPointerv(pname, params);
  SGAL_CHECK_GL();
}

/*! glGetPolygonStipple wrapper */
inline void glGetPolygonStipple(GLubyte* mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetPolygonStipple(" << ");"
                  << std::endl;);
  ::glGetPolygonStipple(mask);
  SGAL_CHECK_GL();
}

/*! GLubyte wrapper */
inline const GLubyte* glGetString(GLenum name)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "GLubyte(" << ");"
                  << std::endl;);
  const GLubyte* res = ::glGetString(name);
  SGAL_CHECK_GL();
  return res;
}

/*! glGetTexEnvfv wrapper */
inline void glGetTexEnvfv(GLenum target, GLenum pname, GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexEnvfv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexEnvfv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexEnviv wrapper */
inline void glGetTexEnviv(GLenum target, GLenum pname, GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexEnviv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexEnviv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexGendv wrapper */
inline void glGetTexGendv(GLenum coord, GLenum pname, GLdouble* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexGendv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexGendv(coord, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexGenfv wrapper */
inline void glGetTexGenfv(GLenum coord, GLenum pname, GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexGenfv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexGenfv(coord, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexGeniv wrapper */
inline void glGetTexGeniv(GLenum coord, GLenum pname, GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexGeniv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexGeniv(coord, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexImage wrapper */
inline void glGetTexImage(GLenum target, GLint level, GLenum format,
                          GLenum type, GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexImage("
                  << Gl_wrapper::find(target) << ", " << level << ");"
                  << std::endl;);
  ::glGetTexImage(target, level, format, type, pixels);
  SGAL_CHECK_GL();
}

/*! glGetTexLevelParameterfv wrapper */
inline void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname,
                                     GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexLevelParameterfv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexLevelParameterfv(target, level, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexLevelParameteriv wrapper */
inline void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname,
                                     GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexLevelParameteriv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexLevelParameteriv(target, level, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexParameterfv wrapper */
inline void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexParameterfv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexParameterfv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glGetTexParameteriv wrapper */
inline void glGetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetTexParameteriv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glGetTexParameteriv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glHint wrapper */
inline void glHint(GLenum target, GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glHint(" << ");"
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(mode) << std::endl;);
  ::glHint(target, mode);
  SGAL_CHECK_GL();
}

/*! glIndexMask wrapper */
inline void glIndexMask(GLuint mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexMask(" << mask << ");"
                  << std::endl;);
  ::glIndexMask(mask);
  SGAL_CHECK_GL();
}

/*! glIndexPointer wrapper */
inline void glIndexPointer(GLenum type, GLsizei stride, const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexPointer(" << ");"
                  << std::endl;);
  ::glIndexPointer(type, stride, pointer);
  SGAL_CHECK_GL();
}

/*! glIndexd wrapper */
inline void glIndexd(GLdouble c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexd(" << c << ");"
                  << std::endl;);
  ::glIndexd(c);
  SGAL_CHECK_GL();
}

/*! glIndexdv wrapper */
inline void glIndexdv(const GLdouble* c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexdv(" << ");"
                  << std::endl;);
  ::glIndexdv(c);
  SGAL_CHECK_GL();
}

/*! glIndexf wrapper */
inline void glIndexf(GLfloat c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexf(" << c << ");"
                  << std::endl;);
  ::glIndexf(c);
  SGAL_CHECK_GL();
}

/*! glIndexfv wrapper */
inline void glIndexfv(const GLfloat* c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexfv(" << ");"
                  << std::endl;);
  ::glIndexfv(c);
  SGAL_CHECK_GL();
}

/*! glIndexi wrapper */
inline void glIndexi(GLint c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexi(" << c << ");"
                  << std::endl;);
  ::glIndexi(c);
  SGAL_CHECK_GL();
}

/*! glIndexiv wrapper */
inline void glIndexiv(const GLint* c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexiv(" << ");"
                  << std::endl;);
  ::glIndexiv(c);
  SGAL_CHECK_GL();
}

/*! glIndexs wrapper */
inline void glIndexs(GLshort c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexs(" << c << ");"
                  << std::endl;);
  ::glIndexs(c);
  SGAL_CHECK_GL();
}

/*! glIndexsv wrapper */
inline void glIndexsv(const GLshort* c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexsv(" << ");"
                  << std::endl;);
  ::glIndexsv(c);
  SGAL_CHECK_GL();
}

/*! glIndexub wrapper */
inline void glIndexub(GLubyte c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexub(" << c << ");"
                  << std::endl;);
  ::glIndexub(c);
  SGAL_CHECK_GL();
}

/*! glIndexubv wrapper */
inline void glIndexubv(const GLubyte* c)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIndexubv(" << ");"
                  << std::endl;);
  ::glIndexubv(c);
  SGAL_CHECK_GL();
}

/*! glInitNames wrapper */
inline void glInitNames(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glInitNames();"
                  << std::endl;);
  ::glInitNames();
  SGAL_CHECK_GL();
}

/*! glInterleavedArrays wrapper */
inline void glInterleavedArrays(GLenum format, GLsizei stride,
                                const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glInterleavedArrays(" << ");"
                  << std::endl;);
  ::glInterleavedArrays(format, stride, pointer);
  SGAL_CHECK_GL();
}

/*! GLboolean wrapper */
inline GLboolean glIsEnabled(GLenum cap)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIsEnabled(" << cap << ");"
                  << std::endl;);
  GLboolean res = ::glIsEnabled(cap);
  SGAL_CHECK_GL();
  return res;
}

/*! GLboolean wrapper */
inline GLboolean glIsList(GLuint list)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIsList(" << list << ");"
                  << std::endl;);
  GLboolean res = ::glIsList(list);
  SGAL_CHECK_GL();
  return res;
}

/*! GLboolean wrapper */
inline GLboolean glIsTexture(GLuint texture)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "GLboolean(" << texture << ");"
                  << std::endl;);
  GLboolean res = ::glIsTexture(texture);
  SGAL_CHECK_GL();
  return res;
}

/*! glLightModelf wrapper */
inline void glLightModelf(GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModelf("
                  << Gl_wrapper::find(pname) << ", " << param << ");"
                  << std::endl;);
  ::glLightModelf(pname, param);
  SGAL_CHECK_GL();
}

/*! glLightModelfv wrapper */
inline void glLightModelfv(GLenum pname, const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModelfv("
                  << Gl_wrapper::find(pname) << ", params);"
                  << std::endl;);
  ::glLightModelfv(pname, params);
  SGAL_CHECK_GL();
}

/*! glLightModeli wrapper */
inline void glLightModeli(GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, 
                  if (pname == GL_LIGHT_MODEL_COLOR_CONTROL) {
                    std::cout << "glLightModeli("
                              << Gl_wrapper::find(pname) << ", "
                              << Gl_wrapper::find(param) << ");"
                              << std::endl;
                  }
                  else {
                    std::cout << "glLightModeli("
                              << Gl_wrapper::find(pname) << ", "
                              << param << ");"
                              << std::endl;
                  });
  ::glLightModeli(pname, param);
  SGAL_CHECK_GL();
}

/*! glLightModeliv wrapper */
inline void glLightModeliv(GLenum pname, const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightModeliv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glLightModeliv(pname, params);
  SGAL_CHECK_GL();
}

/*! glLightf wrapper */
inline void glLightf(GLenum light, GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightf("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glLightf(light, pname, param);
  SGAL_CHECK_GL();
}

/*! glLightfv wrapper */
inline void glLightfv(GLenum light, GLenum pname, const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightfv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glLightfv(light, pname, params);
  SGAL_CHECK_GL();
}

/*! glLighti wrapper */
inline void glLighti(GLenum light, GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLighti("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glLighti(light, pname, param);
  SGAL_CHECK_GL();
}

/*! glLightiv wrapper */
inline void glLightiv(GLenum light, GLenum pname, const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLightiv("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glLightiv(light, pname, params);
  SGAL_CHECK_GL();
}

/*! glLineStipple wrapper */
inline void glLineStipple(GLint factor, GLushort pattern)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLineStipple(" << ");"
                  << std::endl;);
  ::glLineStipple(factor, pattern);
  SGAL_CHECK_GL();
}

/*! glLineWidth wrapper */
inline void glLineWidth(GLfloat width)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLineWidth(" << width << ");"
                  << std::endl;);
  ::glLineWidth(width);
  SGAL_CHECK_GL();
}

/*! glListBase wrapper */
inline void glListBase(GLuint base)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glListBase(" << base << ");"
                  << std::endl;);
  ::glListBase(base);
  SGAL_CHECK_GL();
}

/*! glLoadIdentity wrapper */
inline void glLoadIdentity()
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLoadIdentity();\n";);
  ::glLoadIdentity();
  SGAL_CHECK_GL();
}

/*! glLoadMatrixd wrapper */
inline void glLoadMatrixd(const GLdouble* m)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLoadMatrixd(" << ");"
                  << std::endl;);
  ::glLoadMatrixd(m);
  SGAL_CHECK_GL();
}

/*! glLoadMatrixf wrapper */
inline void glLoadMatrixf(const GLfloat* m)
{
    SGAL_TRACE_CODE(Trace::GRAPHICS, 
                    std::cout << "glLoadMatrixf(";
                    for (Uint i = 0; i < 16; ++i) {
                      std::cout << m[i];
                      if ((i % 4) != 3) std::cout << ", ";
                      else std::cout << std::endl;
                    }
                    std::cout << ");\n";);
  ::glLoadMatrixf(m);
  SGAL_CHECK_GL();
}

/*! glLoadName wrapper */
inline void glLoadName(GLuint name)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "GLuint(" << name << ");"
                  << std::endl;);
  ::glLoadName(name);
  SGAL_CHECK_GL();
}

/*! glLogicOp wrapper */
inline void glLogicOp(GLenum opcode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glLogicOp(" << ");"
                  << std::endl;);
  ::glLogicOp(opcode);
  SGAL_CHECK_GL();
}

/*! glMap1d wrapper */
inline void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride,
                    GLint order, const GLdouble* points)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap1d("
                  << Gl_wrapper::find(target) << ", "
                  << u1 << ", " << u2 << ", "
                  << stride << ", " << order << ");"
                  << std::endl;);
  ::glMap1d(target, u1, u2, stride, order, points);
  SGAL_CHECK_GL();
}

/*! glMap1f wrapper */
inline void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride,
                    GLint order, const GLfloat* points)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap1f("
                  << Gl_wrapper::find(target) << ", "
                  << u1 << ", " << u2 << ", "
                  << stride << ", " << order << ");"
                  << std::endl;);
  ::glMap1f(target, u1, u2, stride, order, points);
  SGAL_CHECK_GL();
}

/*! glMap2d wrapper */
inline void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride,
                    GLint uorder, GLdouble v1, GLdouble v2, GLint vstride,
                    GLint vorder, const GLdouble* points)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap2d("
                  << Gl_wrapper::find(target) << ", "
                  << u1 << ", " << u2 << ", "
                  << ustride << ", " << uorder << ", "
                  << v1 << ", " << v2 << ", "
                  << vstride << ", " << vorder << ");"
                  << std::endl;);
  ::glMap2d(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
  SGAL_CHECK_GL();
}

/*! glMap2f wrapper */
inline void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride,
                    GLint uorder, GLfloat v1, GLfloat v2, GLint vstride,
                    GLint vorder, const GLfloat* points)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMap2f("
                  << Gl_wrapper::find(target) << ", "
                  << u1 << ", " << u2 << ", "
                  << ustride << ", " << uorder << ", "
                  << v1 << ", " << v2 << ", "
                  << vstride << ", " << vorder << ");"
                  << std::endl;);
  ::glMap2f(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
  SGAL_CHECK_GL();
}

/*! glMapGrid1d wrapper */
inline void glMapGrid1d(GLint un, GLdouble u1, GLdouble u2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid1d("
                  << un << ", " << u1 << ", " << u2 << ");"
                  << std::endl;);
  ::glMapGrid1d(un, u1, u2);
  SGAL_CHECK_GL();
}

/*! glMapGrid1f wrapper */
inline void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid1f("
                  << un << ", " << u1 << ", " << u2 << ");"
                  << std::endl;);
  ::glMapGrid1f(un, u1, u2);
  SGAL_CHECK_GL();
}

/*! glMapGrid2d wrapper */
inline void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2,
                        GLint vn, GLdouble v1, GLdouble v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid2d("
                  << un << ", " << u1 << ", " << u2 << ", "
                  << vn << ", " << v1 << ", " << v2 << ");"
                  << std::endl;);
  ::glMapGrid2d(un, u1, u2, vn, v1, v2);
  SGAL_CHECK_GL();
}

/*! glMapGrid2f wrapper */
inline void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn,
                        GLfloat v1, GLfloat v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapGrid2f("
                  << un << ", " << u1 << ", " << u2 << ", "
                  << vn << ", " << v1 << ", " << v2 << ");"
                  << std::endl;);
  ::glMapGrid2f(un, u1, u2, vn, v1, v2);
  SGAL_CHECK_GL();
}

/*! glMaterialf wrapper */
inline void glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMaterialf("
                  << Gl_wrapper::find(face) << ", "
                  << Gl_wrapper::find(pname) << ", " << param << ");"
                  << std::endl;);
  ::glMaterialf(face, pname, param);
  SGAL_CHECK_GL();
}

/*! glMaterialfv wrapper */
inline void glMaterialfv(GLenum face, GLenum pname, const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
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
               
                   case GL_SHININESS:
                    std::cout << params[0] << std::endl;
                    break;
               
                   case GL_COLOR_INDEXES:
                    std::cout << params[0] << "," << params[1] << ","
                              << params[2]
                              << std::endl;
                  }
                  std::cout << "}" << std::endl;
                  std::cout << "glMaterialfv("
                  << Gl_wrapper::find(face) << ", "
                  << Gl_wrapper::find(pname) << ", params);"
                  << std::endl;);
  ::glMaterialfv(face, pname, params);
  SGAL_CHECK_GL();
}

/*! glMateriali wrapper */
inline void glMateriali(GLenum face, GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMateriali("
                  << Gl_wrapper::find(face) << ", "
                  << Gl_wrapper::find(pname) << ", " << param << ");"
                  << std::endl;);
  ::glMateriali(face, pname, param);
  SGAL_CHECK_GL();
}

/*! glMaterialiv wrapper */
inline void glMaterialiv(GLenum face, GLenum pname, const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMaterialiv("
                  << Gl_wrapper::find(face) << ", "
                  << Gl_wrapper::find(pname) << ", params);"
                  << std::endl;);
  ::glMaterialiv(face, pname, params);
  SGAL_CHECK_GL();
}

/*! glMatrixMode wrapper */
inline void glMatrixMode(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMatrixMode("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glMatrixMode(mode);
  SGAL_CHECK_GL();
}

/*! glMultMatrixd wrapper */
inline void glMultMatrixd(const GLdouble* m)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMultMatrixd(" << ");"
                  << std::endl;);
  ::glMultMatrixd(m);
  SGAL_CHECK_GL();
}

/*! glMultMatrixf wrapper */
inline void glMultMatrixf(const GLfloat* m)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMultMatrixf("
                  << "matrix" << ");"
                  << std::endl;);
  ::glMultMatrixf(m);
  SGAL_CHECK_GL();
}

/*! glNewList wrapper */
inline void glNewList(GLuint list, GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNewList(" << ");"
                  << std::endl;);
  ::glNewList(list, mode);
  SGAL_CHECK_GL();
}

/*! glNormal3b wrapper */
inline void glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3b(" << ");"
                  << std::endl;);
  ::glNormal3b(nx, ny, nz);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3bv wrapper */
inline void glNormal3bv(const GLbyte* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3bv(" << ");"
                  << std::endl;);
  ::glNormal3bv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3d wrapper */
inline void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3d("
                  << nx << ", " << ny << ", " << nz << ");"
                  << std::endl;);
  ::glNormal3d(nx, ny, nz);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3dv wrapper */
inline void glNormal3dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3dv(" << ");"
                  << std::endl;);
  ::glNormal3dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3f wrapper */
inline void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3f("
                  << nx << ", " << ny << ", " << nz << ");"
                  << std::endl;);
  ::glNormal3f(nx, ny, nz);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! normal wrapper */
inline void glNormal3fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glNormal3fv(" << *((Vector3f *) v) << ");"
                  << std::endl;
             );
  ::glNormal3fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3i wrapper */
inline void glNormal3i(GLint nx, GLint ny, GLint nz)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "("
                  << nx << ", " << ny << ", " << nz << ");"
                  << std::endl;);
  ::glNormal3i(nx, ny, nz);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3iv wrapper */
inline void glNormal3iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3iv(" << ");"
                  << std::endl;);
  ::glNormal3iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3s wrapper */
inline void glNormal3s(GLshort nx, GLshort ny, GLshort nz)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3s("
                  << nx << ", " << ny << ", " << nz << ");"
                  << std::endl;);
  ::glNormal3s(nx, ny, nz);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormal3sv wrapper */
inline void glNormal3sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormal3sv(" << ");"
                  << std::endl;);
  ::glNormal3sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glNormalPointer wrapper */
inline void glNormalPointer(GLenum type, GLsizei stride,
                            const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glNormalPointer("
                  << Gl_wrapper::find(type) << ", "
                  << stride << ", " << "normals);"
                  << std::endl;);
  ::glNormalPointer(type, stride, pointer);
  SGAL_CHECK_GL();
}

/*! glOrtho wrapper */
inline void glOrtho(GLdouble left, GLdouble right, GLdouble bottom,
                    GLdouble top, GLdouble zNear, GLdouble zFar)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glOrtho("
                  << left << ", " << right << ", "
                  << bottom << ", " << top << ", "
                  << zNear << ", " << zFar << ");"
                  << std::endl;);
  ::glOrtho(left, right, bottom, top, zNear, zFar);
  SGAL_CHECK_GL();
}

/*! glPassThrough wrapper */
inline void glPassThrough(GLfloat token)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPassThrough("
                  << token << ");"
                  << std::endl;);
  ::glPassThrough(token);
  SGAL_CHECK_GL();
}

/*! glPixelMapfv wrapper */
inline void glPixelMapfv(GLenum map, GLsizei mapsize, const GLfloat* values)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelMapfv(" << ");"
                  << std::endl;);
  ::glPixelMapfv(map, mapsize, values);
  SGAL_CHECK_GL();
}

/*! glPixelMapuiv wrapper */
inline void glPixelMapuiv(GLenum map, GLsizei mapsize, const GLuint* values)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelMapuiv(" << ");"
                  << std::endl;);
  ::glPixelMapuiv(map, mapsize, values);
  SGAL_CHECK_GL();
}

/*! glPixelMapusv wrapper */
inline void glPixelMapusv(GLenum map, GLsizei mapsize,
                          const GLushort* values)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelMapusv(" << ");"
                  << std::endl;);
  ::glPixelMapusv(map, mapsize, values);
  SGAL_CHECK_GL();
}

/*! glPixelStoref wrapper */
inline void glPixelStoref(GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelStoref("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glPixelStoref(pname, param);
  SGAL_CHECK_GL();
}

/*! glPixelStorei wrapper */
inline void glPixelStorei(GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelStorei("
                  << Gl_wrapper::find(pname) << ", " << param << ");"
                  << std::endl;);
  ::glPixelStorei(pname, param);
  SGAL_CHECK_GL();
}

/*! glPixelTransferf wrapper */
inline void glPixelTransferf(GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelTransferf("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glPixelTransferf(pname, param);
  SGAL_CHECK_GL();
}

/*! glPixelTransferi wrapper */
inline void glPixelTransferi(GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelTransferi("
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glPixelTransferi(pname, param);
  SGAL_CHECK_GL();
}

/*! glPixelZoom wrapper */
inline void glPixelZoom(GLfloat xfactor, GLfloat yfactor)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPixelZoom(" << ");"
                  << std::endl;);
  ::glPixelZoom(xfactor, yfactor);
  SGAL_CHECK_GL();
}

/*! glPointSize wrapper */
inline void glPointSize(GLfloat size)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPointSize(" << size << ");"
                  << std::endl;);
  ::glPointSize(size);
  SGAL_CHECK_GL();
}

/*! glPolygonMode wrapper */
inline void glPolygonMode(GLenum face, GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPolygonMode("
                  << Gl_wrapper::find(face) << ", "
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glPolygonMode(face, mode);
  SGAL_CHECK_GL();
}

/*! glPolygonOffset wrapper */
inline void glPolygonOffset(GLfloat factor, GLfloat units)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPolygonOffset(" << ");"
                  << std::endl;);
  ::glPolygonOffset(factor, units);
  SGAL_CHECK_GL();
}

/*! glPolygonStipple wrapper */
inline void glPolygonStipple(const GLubyte* mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPolygonStipple(" << ");"
                  << std::endl;);
  ::glPolygonStipple(mask);
  SGAL_CHECK_GL();
}

/*! glPopAttrib wrapper */
inline void glPopAttrib(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopAttrib();"
                  << std::endl;);
  ::glPopAttrib();
  SGAL_CHECK_GL();
}

/*! glPopClientAttrib wrapper */
inline void glPopClientAttrib(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopClientAttrib();"
                  << std::endl;);
  ::glPopClientAttrib();
  SGAL_CHECK_GL();
}

/*! glPopMatrix wrapper */
inline void glPopMatrix(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopMatrix();"
                  << std::endl;);
  ::glPopMatrix();
  SGAL_CHECK_GL();
}

/*! glPopName wrapper */
inline void glPopName(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPopName();" << std::endl;);
  ::glPopName();
  SGAL_CHECK_GL();
}

/*! glPrioritizeTextures wrapper */
inline void glPrioritizeTextures(GLsizei n, const GLuint* textures,
                                 const GLclampf* priorities)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPrioritizeTextures(" << ");"
                  << std::endl;);
  ::glPrioritizeTextures(n, textures, priorities);
  SGAL_CHECK_GL();
}

/*! glPushAttrib wrapper */
inline void glPushAttrib(GLbitfield mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushAttrib(" << ");"
                  << std::endl;);
  ::glPushAttrib(mask);
  SGAL_CHECK_GL();
}

/*! glPushClientAttrib wrapper */
inline void glPushClientAttrib(GLbitfield mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushClientAttrib(" << ");"
                  << std::endl;);
  ::glPushClientAttrib(mask);
  SGAL_CHECK_GL();
}

/*! glPushMatrix wrapper */
inline void glPushMatrix(void)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushMatrix();\n";);
  ::glPushMatrix();
  SGAL_CHECK_GL();
}

/*! glPushName wrapper */
inline void glPushName(GLuint name)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glPushMatrix(" << name << ");"
                  << std::endl;);
  ::glPushName(name);
  SGAL_CHECK_GL();
}

/*! glRasterPos2d wrapper */
inline void glRasterPos2d(GLdouble x, GLdouble y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2d("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glRasterPos2d(x, y);
  SGAL_CHECK_GL();
}

/*! glRasterPos2dv wrapper */
inline void glRasterPos2dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2dv(" << ");"
                  << std::endl;);
  ::glRasterPos2dv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos2f wrapper */
inline void glRasterPos2f(GLfloat x, GLfloat y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2f("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glRasterPos2f(x, y);
  SGAL_CHECK_GL();
}

/*! glRasterPos2fv wrapper */
inline void glRasterPos2fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2fv(" << ");"
                  << std::endl;);
  ::glRasterPos2fv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos2i wrapper */
inline void glRasterPos2i(GLint x, GLint y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2i("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glRasterPos2i(x, y);
  SGAL_CHECK_GL();
}

/*! glRasterPos2iv wrapper */
inline void glRasterPos2iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2iv(" << ");"
                  << std::endl;);
  ::glRasterPos2iv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos2s wrapper */
inline void glRasterPos2s(GLshort x, GLshort y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2s("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glRasterPos2s(x, y);
  SGAL_CHECK_GL();
}

/*! glRasterPos2sv wrapper */
inline void glRasterPos2sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos2sv(" << ");"
                  << std::endl;);
  ::glRasterPos2sv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos3d wrapper */
inline void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3d("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glRasterPos3d(x, y, z);
  SGAL_CHECK_GL();
}

/*! glRasterPos3dv wrapper */
inline void glRasterPos3dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3dv(" << ");"
                  << std::endl;);
  ::glRasterPos3dv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos3f wrapper */
inline void glRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3f("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glRasterPos3f(x, y, z);
  SGAL_CHECK_GL();
}

/*! glRasterPos3fv wrapper */
inline void glRasterPos3fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3fv(" << ");"
                  << std::endl;);
  ::glRasterPos3fv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos3i wrapper */
inline void glRasterPos3i(GLint x, GLint y, GLint z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3i("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glRasterPos3i(x, y, z);
  SGAL_CHECK_GL();
}

/*! glRasterPos3iv wrapper */
inline void glRasterPos3iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3iv(" << ");"
                  << std::endl;);
  ::glRasterPos3iv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos3s wrapper */
inline void glRasterPos3s(GLshort x, GLshort y, GLshort z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3s("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glRasterPos3s(x, y, z);
  SGAL_CHECK_GL();
}

/*! glRasterPos3sv wrapper */
inline void glRasterPos3sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos3sv(" << ");"
                  << std::endl;);
  ::glRasterPos3sv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos4d wrapper */
inline void glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4d("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glRasterPos4d(x, y, z, w);
  SGAL_CHECK_GL();
}

/*! glRasterPos4dv wrapper */
inline void glRasterPos4dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4dv(" << ");"
                  << std::endl;);
  ::glRasterPos4dv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos4f wrapper */
inline void glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4f("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glRasterPos4f(x, y, z, w);
  SGAL_CHECK_GL();
}

/*! glRasterPos4fv wrapper */
inline void glRasterPos4fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4fv(" << ");"
                  << std::endl;);
  ::glRasterPos4fv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos4i wrapper */
inline void glRasterPos4i(GLint x, GLint y, GLint z, GLint w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4i("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glRasterPos4i(x, y, z, w);
  SGAL_CHECK_GL();
}

/*! glRasterPos4iv wrapper */
inline void glRasterPos4iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4iv(" << ");"
                  << std::endl;);
  ::glRasterPos4iv(v);
  SGAL_CHECK_GL();
}

/*! glRasterPos4s wrapper */
inline void glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4s("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glRasterPos4s(x, y, z, w);
  SGAL_CHECK_GL();
}

/*! glRasterPos4sv wrapper */
inline void glRasterPos4sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRasterPos4sv(" << ");"
                  << std::endl;);
  ::glRasterPos4sv(v);
  SGAL_CHECK_GL();
}

/*! glReadBuffer wrapper */
inline void glReadBuffer(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glReadBuffer("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glReadBuffer(mode);
  SGAL_CHECK_GL();
}

/*! glReadPixels wrapper */
inline void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                         GLenum format, GLenum type, GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glReadPixels("
                  << x << ", " << y << ", " << width << ", " << height << ", "
                  << Gl_wrapper::find(format) << ","
                  << Gl_wrapper::find(type) << ", pixels);"
                  << std::endl;);
  ::glReadPixels(x, y, width, height, format, type, pixels);
  SGAL_CHECK_GL();
}

/*! glRectd wrapper */
inline void glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectd("
                  << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
                  << std::endl;);
  ::glRectd(x1, y1, x2, y2);
  SGAL_CHECK_GL();
}

/*! glRectdv wrapper */
inline void glRectdv(const GLdouble* v1, const GLdouble* v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectdv(" << ");"
                  << std::endl;);
  ::glRectdv(v1, v2);
  SGAL_CHECK_GL();
}

/*! glRectf wrapper */
inline void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectf("
                  << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
                  << std::endl;);
  ::glRectf(x1, y1, x2, y2);
  SGAL_CHECK_GL();
}

/*! glRectfv wrapper */
inline void glRectfv(const GLfloat* v1, const GLfloat* v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectfv(" << ");"
                  << std::endl;);
  ::glRectfv(v1, v2);
  SGAL_CHECK_GL();
}

/*! glRecti wrapper */
inline void glRecti(GLint x1, GLint y1, GLint x2, GLint y2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRecti("
                  << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
                  << std::endl;);
  ::glRecti(x1, y1, x2, y2);
  SGAL_CHECK_GL();
}

/*! glRectiv wrapper */
inline void glRectiv(const GLint* v1, const GLint* v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectiv(" << ");"
                  << std::endl;);
  ::glRectiv(v1, v2);
  SGAL_CHECK_GL();
}

/*! glRects wrapper */
inline void glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRects("
                  << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ");"
                  << std::endl;);
  ::glRects(x1, y1, x2, y2);
  SGAL_CHECK_GL();
}

/*! glRectsv wrapper */
inline void glRectsv(const GLshort* v1, const GLshort* v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRectsv(" << ");"
                  << std::endl;);
  ::glRectsv(v1, v2);
  SGAL_CHECK_GL();
}

/*! GLint wrapper */
inline GLint glRenderMode(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRenderMode(" << ");"
                  << std::endl;);
  GLint res = ::glRenderMode(mode);
  return res;
  SGAL_CHECK_GL();
}

/*! glRotated wrapper */
inline void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRotated(" << ");"
                  << std::endl;);
  ::glRotated(angle, x, y, z);
  SGAL_CHECK_GL();
}

/*! glRotatef wrapper */
inline void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glRotatef("
                  << angle << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glRotatef(angle, x, y, z);
  SGAL_CHECK_GL();
}

/*! glScaled wrapper */
inline void glScaled(GLdouble x, GLdouble y, GLdouble z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glScaled("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glScaled(x, y, z);
  SGAL_CHECK_GL();
}

/*! glScalef wrapper */
inline void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glScalef("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glScalef(x, y, z);
  SGAL_CHECK_GL();
}

/*! glScissor wrapper */
inline void glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glScissor(" << x << ", " << y
                  << width << ", " << height << ");"
                  << std::endl;);
  ::glScissor(x, y, width, height);
  SGAL_CHECK_GL();
}

/*! glSelectBuffer wrapper */
inline void glSelectBuffer(GLsizei size, GLuint* buffer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glSelectBuffer(" << ");"
                  << std::endl;);
  ::glSelectBuffer(size, buffer);
  SGAL_CHECK_GL();
}

/*! glShadeModel wrapper */
inline void glShadeModel(GLenum mode)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glShadeModel("
                  << Gl_wrapper::find(mode) << ");"
                  << std::endl;);
  ::glShadeModel(mode);
  SGAL_CHECK_GL();
}

/*! glStencilFunc wrapper */
inline void glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glStencilFunc(" << ");"
                  << std::endl;);
  ::glStencilFunc(func, ref, mask);
  SGAL_CHECK_GL();
}

/*! glStencilMask wrapper */
inline void glStencilMask(GLuint mask)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glStencilMask(" << ");"
                  << std::endl;);
  ::glStencilMask(mask);
  SGAL_CHECK_GL();
}

/*! glStencilOp wrapper */
inline void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glStencilOp(" << ");"
                  << std::endl;);
  ::glStencilOp(fail, zfail, zpass);
  SGAL_CHECK_GL();
}

/*! glTexCoord1d wrapper */
inline void glTexCoord1d(GLdouble s)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1d(" << s << ");"
                  << std::endl;);
  ::glTexCoord1d(s);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1dv wrapper */
inline void glTexCoord1dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1dv(" << ");"
                  << std::endl;);
  ::glTexCoord1dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1f wrapper */
inline void glTexCoord1f(GLfloat s)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1f(" << s << ");"
                  << std::endl;);
  ::glTexCoord1f(s);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1fv wrapper */
inline void glTexCoord1fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1fv(" << ");"
                  << std::endl;);
  ::glTexCoord1fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1i wrapper */
inline void glTexCoord1i(GLint s)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1i(" << s << ");"
                  << std::endl;);
  ::glTexCoord1i(s);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1iv wrapper */
inline void glTexCoord1iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1iv(" << ");"
                  << std::endl;);
  ::glTexCoord1iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1s wrapper */
inline void glTexCoord1s(GLshort s)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1s(" << s << ");"
                  << std::endl;);
  ::glTexCoord1s(s);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord1sv wrapper */
inline void glTexCoord1sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord1sv(" << ");"
                  << std::endl;);
  ::glTexCoord1sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord2d wrapper */
inline void glTexCoord2d(GLdouble s, GLdouble t)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2d("
                  << s << ", " << t << ");"
                  << std::endl;);
  ::glTexCoord2d(s, t);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord2dv wrapper */
inline void glTexCoord2dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2dv(" << ");"
                  << std::endl;);
  ::glTexCoord2dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! tex-coord wrapper */
inline void glTexCoord2f(const GLfloat v1, const GLfloat v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glTexCoord2f(" << v1 << "," << v2 << ");"
                  << std::endl;);
  ::glTexCoord2f(v1, v2);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! tex-coord wrapper */
inline void glTexCoord2fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glTexCoord2fv(" << *((Vector2f *) v) << ");"
                  << std::endl;);
  ::glTexCoord2fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord2i wrapper */
inline void glTexCoord2i(GLint s, GLint t)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2i("
                  << s << ", " << t << ");"
                  << std::endl;);
  ::glTexCoord2i(s, t);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord2iv wrapper */
inline void glTexCoord2iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2iv(" << ");"
                  << std::endl;);
  ::glTexCoord2iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord2s wrapper */
inline void glTexCoord2s(GLshort s, GLshort t)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2s("
                  << s << ", " << t
                  << ");"
                  << std::endl;);
  ::glTexCoord2s(s, t);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord2sv wrapper */
inline void glTexCoord2sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord2sv(" << ");"
                  << std::endl;);
  ::glTexCoord2sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3d wrapper */
inline void glTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3d("
                  << s << ", " << t << ", " << r << ");"
                  << std::endl;);
  ::glTexCoord3d(s, t, r);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3dv wrapper */
inline void glTexCoord3dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3dv(" << ");"
                  << std::endl;);
  ::glTexCoord3dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3f wrapper */
inline void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3f("
                  << s << ", " << t << ", " << r << ");"
                  << std::endl;);
  ::glTexCoord3f(s, t, r);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3fv wrapper */
inline void glTexCoord3fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3fv(" << ");"
                  << std::endl;);
  ::glTexCoord3fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3i wrapper */
inline void glTexCoord3i(GLint s, GLint t, GLint r)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3i("
                  << s << ", " << t << ", " << r << ");"
                  << std::endl;);
  ::glTexCoord3i(s, t, r);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3iv wrapper */
inline void glTexCoord3iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3iv(" << ");"
                  << std::endl;);
  ::glTexCoord3iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3s wrapper */
inline void glTexCoord3s(GLshort s, GLshort t, GLshort r)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3s("
                  << s << ", " << t << ", " << r << ");"
                  << std::endl;);
  ::glTexCoord3s(s, t, r);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord3sv wrapper */
inline void glTexCoord3sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord3sv(" << ");"
                  << std::endl;);
  ::glTexCoord3sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4d wrapper */
inline void glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4d("
                  << s << ", " << t << ", " << r << ", " << q << ");"
                  << std::endl;);
  ::glTexCoord4d(s, t, r, q);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4dv wrapper */
inline void glTexCoord4dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4dv(" << ");"
                  << std::endl;);
  ::glTexCoord4dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4f wrapper */
inline void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4f("
                  << s << ", " << t << ", " << r << ", " << q << ");"
                  << std::endl;);
  SGAL_CHECK_GL();
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4fv wrapper */
inline void glTexCoord4fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4fv(" << ");"
                  << std::endl;);
  ::glTexCoord4fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4i wrapper */
inline void glTexCoord4i(GLint s, GLint t, GLint r, GLint q)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4i("
                  << s << ", " << t << ", " << r << ", " << q << ");"
                  << std::endl;);
  ::glTexCoord4i(s, t, r, q);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4iv wrapper */
inline void glTexCoord4iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4iv(" << ");"
                  << std::endl;);
  ::glTexCoord4iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4s wrapper */
inline void glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4s("
                  << s << ", " << t << ", " << r << ", " << q << ");"
                  << std::endl;);
  ::glTexCoord4s(s, t, r, q);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoord4sv wrapper */
inline void glTexCoord4sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoord4sv(" << ");"
                  << std::endl;);
  ::glTexCoord4sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glTexCoordPointer wrapper */
inline void glTexCoordPointer(GLint size, GLenum type, GLsizei stride,
                              const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexCoordPointer("
                  << size << ", "
                  << Gl_wrapper::find(type) << ", "
                  << stride << ", "
                  << "tex_coords" << ");" << std::endl;);
  ::glTexCoordPointer(size, type, stride, pointer);
  SGAL_CHECK_GL();
}

/*! glTexEnvf wrapper */
inline void glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexEnvf("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glTexEnvf(target, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexEnvfv wrapper */
inline void glTexEnvfv(GLenum target, GLenum pname, const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexEnvfv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glTexEnvfv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexEnvi wrapper */
inline void glTexEnvi(GLenum target, GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexEnvi("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(param) << ");"
                  << std::endl;);
  ::glTexEnvi(target, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexEnviv wrapper */
inline void glTexEnviv(GLenum target, GLenum pname, const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "GLenum("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(*params) << ");"
                  << std::endl;);
  ::glTexEnviv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexGend wrapper */
inline void glTexGend(GLenum coord, GLenum pname, GLdouble param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGend("
                  << Gl_wrapper::find(coord) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(param) << ");"
                  << std::endl;);
  ::glTexGend(coord, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexGendv wrapper */
inline void glTexGendv(GLenum coord, GLenum pname, const GLdouble* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGendv("
                  << Gl_wrapper::find(coord) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(*params) << ");"
                  << std::endl;);
  ::glTexGendv(coord, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexGenf wrapper */
inline void glTexGenf(GLenum coord, GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGenf("
                  << Gl_wrapper::find(coord) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(param) << ");"
                  << std::endl;);
  ::glTexGenf(coord, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexGenfv wrapper */
inline void glTexGenfv(GLenum coord, GLenum pname, const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGenfv("
                  << Gl_wrapper::find(coord) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(*params) << ");"
                  << std::endl;);
  ::glTexGenfv(coord, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexGeni wrapper */
inline void glTexGeni(GLenum coord, GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGeni("
                  << Gl_wrapper::find(coord) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(param) << ");"
                  << std::endl;);
  ::glTexGeni(coord, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexGeniv wrapper */
inline void glTexGeniv(GLenum coord, GLenum pname, const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexGeniv("
                  << Gl_wrapper::find(coord) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(*params) << ");"
                  << std::endl;);
  ::glTexGeniv(coord, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexImage1D wrapper */
inline void glTexImage1D(GLenum target, GLint level, GLint internalformat,
                         GLsizei width, GLint border, GLenum format,
                         GLenum type, const GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexImage1D("
                  << Gl_wrapper::find(target) << ", " << level << ");"
                  << std::endl;);
  ::glTexImage1D(target, level, internalformat, width, border, format,  type,
                 pixels);
  SGAL_CHECK_GL();
}

/*! glTexImage2D wrapper */
inline void glTexImage2D(GLenum target, GLint level, GLint internalformat,
                         GLsizei width, GLsizei height, GLint border,
                         GLenum format, GLenum type, const GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, 
                  // Is the internal format the number of components?
                  if ((0 < internalformat) && (internalformat <= 4)) {
                    std::cout << "glTexImage2D("
                              << Gl_wrapper::find(target) << ", "
                              << level << ", "
                              << internalformat << ", "
                              << width << ", " << height << ", "
                              << border << ", "
                              << Gl_wrapper::find(format) << ", "
                              << Gl_wrapper::find(type) << ", pixels);"
                              << std::endl;
                  }
                  else {
                    std::cout << "glTexImage2D("
                              << Gl_wrapper::find(target) << ", "
                              << level << ", "
                              << Gl_wrapper::find(internalformat) << ", "
                              << width << ", " << height << ", "
                              << border << ", "
                              << Gl_wrapper::find(format) << ", "
                              << Gl_wrapper::find(type) << ", pixels);"
                              << std::endl;
                  });
  ::glTexImage2D(target, level, internalformat, width, height, border, format,
                 type, pixels);
  SGAL_CHECK_GL();
}

/*! glTexParameterf wrapper */
inline void glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameterf("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glTexParameterf(target, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexParameterfv wrapper */
inline void glTexParameterfv(GLenum target, GLenum pname,
                             const GLfloat* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameterfv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glTexParameterfv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexParameteri wrapper */
inline void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameteri("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ", "
                  << Gl_wrapper::find(param) << ");"
                  << std::endl;);
  ::glTexParameteri(target, pname, param);
  SGAL_CHECK_GL();
}

/*! glTexParameteriv wrapper */
inline void glTexParameteriv(GLenum target, GLenum pname,
                             const GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexParameteriv("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ");"
                  << std::endl;);
  ::glTexParameteriv(target, pname, params);
  SGAL_CHECK_GL();
}

/*! glTexSubImage1D wrapper */
inline void glTexSubImage1D(GLenum target, GLint level, GLint xoffset,
                            GLsizei width, GLenum format, GLenum type,
                            const GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexSubImage1D("
                  << Gl_wrapper::find(target) << ", " << level << ");"
                  << std::endl;);
  ::glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
  SGAL_CHECK_GL();
}

/*! glTexSubImage2D wrapper */
inline void glTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                            GLint yoffset, GLsizei width, GLsizei height,
                            GLenum format, GLenum type, const GLvoid* pixels)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTexSubImage2D("
                  << Gl_wrapper::find(target) << ", " << level << ");"
                  << std::endl;);
  ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
                    type, pixels);
  SGAL_CHECK_GL();
}

/*! glTranslated wrapper */
inline void glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTranslated("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glTranslated(x, y, z);
  SGAL_CHECK_GL();
}

/*! glTranslatef wrapper */
inline void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glTranslatef("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glTranslatef(x, y, z);
  SGAL_CHECK_GL();
}

/*! glVertex2d wrapper */
inline void glVertex2d(GLdouble x, GLdouble y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2d("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glVertex2d(x, y);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex2dv wrapper */
inline void glVertex2dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2dv(" << ");"
                  << std::endl;);
  ::glVertex2dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! vertex wrapper */
inline void glVertex2f(const GLfloat v1, const GLfloat v2)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glVertex2fGLfloat(" << v1 << "," << v2 << ");"
                  << std::endl;);
  ::glVertex2f(v1, v2);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex2fv wrapper */
inline void glVertex2fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "GLfloat(" << ");"
                  << std::endl;);
  ::glVertex2fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex2i wrapper */
inline void glVertex2i(GLint x, GLint y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2i("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glVertex2i(x, y);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex2iv wrapper */
inline void glVertex2iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2iv(" << ");"
                  << std::endl;);
  ::glVertex2iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex2s wrapper */
inline void glVertex2s(GLshort x, GLshort y)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2s("
                  << x << ", " << y << ");"
                  << std::endl;);
  ::glVertex2s(x, y);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex2sv wrapper */
inline void glVertex2sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex2sv(" << ");"
                  << std::endl;);
  ::glVertex2sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3d wrapper */
inline void glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3d("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glVertex3d(x, y, z);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3dv wrapper */
inline void glVertex3dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3dv(" << ");"
                  << std::endl;);
  ::glVertex3dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3f wrapper */
inline void glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3f("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glVertex3f(x, y, z);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! vertex wrapper */
inline void glVertex3fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS,
                  std::cout << "glVertex3fv(" << *((Vector3f *) v) << ");"
                  << std::endl;);
  ::glVertex3fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3i wrapper */
inline void glVertex3i(GLint x, GLint y, GLint z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3i("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glVertex3i(x, y, z);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3iv wrapper */
inline void glVertex3iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3iv(" << ");"
                  << std::endl;);
  ::glVertex3iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3s wrapper */
inline void glVertex3s(GLshort x, GLshort y, GLshort z)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3s("
                  << x << ", " << y << ", " << z << ");"
                  << std::endl;);
  ::glVertex3s(x, y, z);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex3sv wrapper */
inline void glVertex3sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex3sv(" << ");"
                  << std::endl;);
  ::glVertex3sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4d wrapper */
inline void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4d("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glVertex4d(x, y, z, w);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4dv wrapper */
inline void glVertex4dv(const GLdouble* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4dv(" << ");"
                  << std::endl;);
  ::glVertex4dv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4f wrapper */
inline void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4f("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glVertex4f(x, y, z, w);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4fv wrapper */
inline void glVertex4fv(const GLfloat* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4fv(" << ");"
                  << std::endl;);
  ::glVertex4fv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4i wrapper */
inline void glVertex4i(GLint x, GLint y, GLint z, GLint w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4i("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glVertex4i(x, y, z, w);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4iv wrapper */
inline void glVertex4iv(const GLint* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4iv(" << ");"
                  << std::endl;);
  ::glVertex4iv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4s wrapper */
inline void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4s("
                  << x << ", " << y << ", " << z << ", " << w << ");"
                  << std::endl;);
  ::glVertex4s(x, y, z, w);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertex4sv wrapper */
inline void glVertex4sv(const GLshort* v)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertex4sv(" << ");"
                  << std::endl;);
  ::glVertex4sv(v);
  // Cannot check for errors between glBegin() and glEnd()
}

/*! glVertexPointer wrapper */
inline void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                            const GLvoid* pointer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glVertexPointer("
                  << size << "," << Gl_wrapper::find(type) << ","
                  << stride << "," << "vertices" << ");"
                  << std::endl;);
  ::glVertexPointer(size, type, stride, pointer);
  SGAL_CHECK_GL();
}

/*! glViewport wrapper */
inline void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glViewport("
                  << x << ", " << y << ", " << width << ", " << height << ");"
                  << std::endl;);
  ::glViewport(x, y, width, height);
  SGAL_CHECK_GL();
}

#if defined(GL_ARB_vertex_buffer_object)
#ifdef GL_GLEXT_PROTOTYPES
inline void glBindBufferARB(GLenum target, GLuint buffer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glBindBufferARB("
                  << Gl_wrapper::find(target) << ", " << buffer << ");"
                  << std::endl;);
  ::glBindBufferARB(target, buffer);
  SGAL_CHECK_GL();
}

inline void glDeleteBuffersARB(GLsizei n, GLuint* buffers)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glDeleteBuffersARB("
                  << n << ", " << "buffers" << ");"
                  << std::endl;);
  ::glDeleteBuffersARB(n, buffers);
  SGAL_CHECK_GL();
}

inline void glGenBuffersARB(GLsizei n, GLuint* buffers)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGenBuffersARB("
                  << n << ", " << "buffers" << ");"
                  << std::endl;);
  ::glGenBuffersARB(n, buffers);
  SGAL_CHECK_GL();
}

inline GLboolean glIsBufferARB(GLuint buffer)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glIsBufferARB("
                  << buffer << ");"
                  << std::endl;);
  GLboolean res = ::glIsBufferARB(buffer);
  SGAL_CHECK_GL();
  return res;
}

inline void glBufferDataARB(GLenum target, GLsizei size, GLvoid* data,
                            GLenum usage)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glBufferDataARB("
                  << Gl_wrapper::find(target) << ", " << size << ", "
                  << "data" << ", "
                  << Gl_wrapper::find(usage) << ");"
             << std::endl;);
  ::glBufferDataARB(target, size, data, usage);
  SGAL_CHECK_GL();
}

inline void glBufferSubDataARB(GLenum target, GLint offset, GLsizei size,
                               GLvoid* data)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glBufferSubDataARB("
                  << Gl_wrapper::find(target) << ", " << offset << ", "
                  << size << ", " << "data" << ");"
                  << std::endl;);
  ::glBufferSubDataARB(target, offset, size, data);
  SGAL_CHECK_GL();
}

inline void glGetBufferSubDataARB(GLenum target, GLint offset, GLsizei size,
                                  GLvoid* data)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBufferSubDataARB("
                  << Gl_wrapper::find(target) << ", " << offset << ", "
                  << size << ", " << "data" << ");"
                  << std::endl;);
  ::glGetBufferSubDataARB(target, offset, size, data);
  SGAL_CHECK_GL();
}

inline GLvoid* glMapBufferARB(GLenum target, GLenum access)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glMapBufferARB("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(access) << ");"
                  << std::endl;);
  GLvoid* res = ::glMapBufferARB(target, access);
  SGAL_CHECK_GL();
  return res;
}

inline GLboolean glUnmapBufferARB(GLenum target)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glUnmapBufferARB("
                  << Gl_wrapper::find(target) << ");"
                  << std::endl;);
  GLboolean res = ::glUnmapBufferARB(target);
  SGAL_CHECK_GL();
  return res;
}

inline void glGetBufferParameterivARB(GLenum target, GLenum pname,
                                      GLint* params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBufferParameterivARB("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ", " << "params" << ");"
                  << std::endl;);
  ::glGetBufferParameterivARB(target, pname, params);
  SGAL_CHECK_GL();
}

inline void glGetBufferPointervARB(GLenum target, GLenum pname,
                                   GLvoid** params)
{
  SGAL_TRACE_CODE(Trace::GRAPHICS, std::cout << "glGetBufferPointervARB("
                  << Gl_wrapper::find(target) << ", "
                  << Gl_wrapper::find(pname) << ", " << "params" << ");"
                  << std::endl;);
  ::glGetBufferPointervARB(target, pname, params);
  SGAL_CHECK_GL();
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
