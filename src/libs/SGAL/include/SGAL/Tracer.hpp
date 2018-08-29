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

#ifndef SGAL_TRACER_HPP
#define SGAL_TRACER_HPP

#include <iostream>
#include <bitset>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Option_mapper.hpp"

#if (defined( _MSC_VER) && (_MSC_VER <= 1200))
#pragma warning ( disable : 4800 )

#endif

#ifdef TRACE
#undef TRACE
#endif

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Tracer : public Option_mapper {
public:
  //! Pre-defined codes:
  enum Code {
    GRAPHICS = START_CODE,
    VRML_PARSING,
    WINDOW_MANAGER,
    EVENTS,
    SCRIPT,
    INDEXED_FACE_SET,
    INDEXED_LINE_SET,
    POLYHEDRON,
    CUBICAL_GAUSSIAN_MAP,
    DESTRUCTOR,
    SNAPSHOT,
    EXPORT,
    FONT,
    PROTO,
    BOUNDING_SPHERE,
    NUM_CODES,
    MAX_NUM_CODES = 32          // must be larger than NUM_CODES
  };

  //! The trace signature type.
  typedef std::bitset<MAX_NUM_CODES>                    Signature;

  /*! Obtain the trace singleton.
   * \return the trace singleton.
   */
  static Tracer* get_instance();

  /*! Enable the trace for the given code.
   * \param[in] code the given code.
   */
  void enable(size_t code);

  /*! Enable the trace for the given code-signature.
   * \param[in] my_signature the given code-signature.
   */
  void enable(Signature my_signature);

  /*! Disable the trace for the given code.
   * \param[in] code the given code.
   */
  void disable(size_t code);

  /*! Disable the trace for the given code-signature.
   * \param[in] my_signature the given code-signature.
   */
  void disable(Signature my_signature);

  /*! Determine whether the trace for the given code is enabled.
   * \param[in] code the given code.
   * \return a flag that indicates whether the trace is enabled.
   */
  Boolean is_enabled(size_t code) const;

private:
  /*! Construct.
   */
  Tracer();

  /*! Obtain the signature of the given trace-code.
   */
  Signature signature(size_t code) const;

  //! The signature.
  Signature m_signature;

  //! The singleton.
  static Tracer* s_instance;
};

//! \brief
inline Boolean TRACE(size_t code)
{ return Tracer::get_instance()->is_enabled(code); }

//! \brief enables the trace for the given code.
inline void Tracer::enable(size_t code) { enable(signature(code)); }

//! \brief enables the trace for the given code-signature.
inline void Tracer::enable(Signature my_signature)
{ m_signature |= my_signature; }

//! \brief disables the trace for the given code.
inline void Tracer::disable(size_t code) { disable(signature(code)); }

//! \brief disables the trace for the given code-signature.
inline void Tracer::disable(Signature my_signature)
{ m_signature &= ~my_signature; }

//! \brief determines whether the trace for the given code is enabled.
inline Boolean Tracer::is_enabled(size_t code) const
{ return ((m_signature & signature(code)) != 0x0); }

//! \brief obtains the signature of the given trace-code.
inline Tracer::Signature Tracer::signature(size_t code)
  const { return 0x1 << code; }

#if defined(NDEBUG) && !defined(SGAL_TRACE)
#define SGAL_TRACE_MSG(key, msg)
#define SGAL_TRACE_CODE(key, code)
#else
#define SGAL_TRACE_MSG(key, msg)     if (SGAL::TRACE(key)) std::cout << msg
#define SGAL_TRACE_CODE(key, code)   if (SGAL::TRACE(key)) { code } else {}
#endif

SGAL_END_NAMESPACE

#endif
