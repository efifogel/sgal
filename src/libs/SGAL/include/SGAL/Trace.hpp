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

#ifndef SGAL_TRACE_HPP
#define SGAL_TRACE_HPP

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#if (defined( _MSC_VER) && (_MSC_VER <= 1200))
#pragma warning ( disable : 4800 )

#endif

#ifdef TRACE
#undef TRACE
#endif

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Trace {
public:
  enum Code {
    GRAPHICS = 0,
    VRML_PARSING,
    WINDOW_MANAGER,
    EVENTS,
    SCRIPT,
    INDEXED_FACE_SET,
    INDEXED_LINE_SET,
    POLYHEDRON,
    CUBICAL_GAUSSIAN_MAP,
    DESTRUCTOR,
    WRITING,
    NUM_CODES
  };

  /*! Obtain the trace singleton.
   * \return the trace singleton.
   */
  static Trace* get_instance();

  /*! Enable the trace for the given code.
   * \param code (in) the given code.
   */
  void enable(Code code);

  /*! Enable the trace for the given code-signature.
   * \param my_signature (in) the given code-signature.
   */
  void enable(Uint my_signature);

  /*! Disable the trace for the given code.
   * \param code (in) the given code.
   */
  void disable(Code code);

  /*! Disable the trace for the given code-signature.
   * \param my_signature (in) the given code-signature.
   */
  void disable(Uint my_signature);

  /*! Determine whether the trace for the given code is enabled.
   * \param code (in) the given code.
   * \return a flag that indicates whether the trace is enabled.
   */
  Boolean is_enabled(Code code) const;

private:
  /*! Constructor.
   */
  Trace();

  /*! Obtain the signature of the given trace-code.
   */
  Uint signature(Code code) const;

  /*! The signature. */
  Uint m_signature;

  /*! The singleton. */
  static Trace* s_instance;
};

/*! \brief */
inline Boolean TRACE(Trace::Code code)
{ return Trace::get_instance()->is_enabled(code); }

//! \brief enables the trace for the given code.
inline void Trace::enable(Code code) { enable(signature(code)); }

//! \brief enables the trace for the given code-signature.
inline void Trace::enable(Uint my_signature) { m_signature |= my_signature; }

//! \brief disables the trace for the given code.
inline void Trace::disable(Code code) { disable(signature(code)); }

//! \brief disables the trace for the given code-signature.
inline void Trace::disable(Uint my_signature) { m_signature &= !my_signature; }

//! \brief determines whether the trace for the given code is enabled.
inline Boolean Trace::is_enabled(Code code) const
{ return ((m_signature & signature(code)) != 0x0); }

//! \brief constructor.
inline Trace::Trace() : m_signature(0x0) {}

//! \brief obtains the signature of the given trace-code.
inline Uint Trace::signature(Code code) const { return 0x1 << code; }

#if defined(NDEBUG) && !defined(SGAL_TRACE)
#define SGAL_TRACE_MSG(key, msg)
#define SGAL_TRACE_CODE(key, code)
#else
#define SGAL_TRACE_MSG(key, msg)     if (SGAL::TRACE(key)) std::cout << msg
#define SGAL_TRACE_CODE(key, code)   if (SGAL::TRACE(key)) { code } else {}
#endif

SGAL_END_NAMESPACE

#endif
