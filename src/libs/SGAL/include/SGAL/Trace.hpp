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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TRACE_HPP
#define SGAL_TRACE_HPP

#include <iostream>
#include <map>
#include <bitset>

#include <boost/icl/interval_set.hpp>

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

  //! Pre-defined codes:
  enum Code {
    INVALID = -1,
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
    SNAPSHOT,
    EXPORT,
    FONT,
    PROTO,
    DXF_PARSING,
    NUM_CODES,
    MAX_NUM_CODES = 32          // must be larger than NUM_CODES
  };

  //! The trace signature type.
  typedef std::bitset<MAX_NUM_CODES>                    Signature;

  /*! Obtain the trace singleton.
   * \return the trace singleton.
   */
  static Trace* get_instance();

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

  /*! Obtain the number of trace options.
   */
  size_t trace_opts_size() const;

  /*! Find the trace code of an option.
   */
  size_t find_trace_code(const String& opt) const;

  /*! Add a trace option.
   * \param[in] the new option.
   * \return the code allocated for the external option.
   */
  size_t add_trace_opt(const String& opt);

  /*! Remove a trace option.
   * \param[in] the option to remove.
   */
  void remove_trace_opt(const String& opt);

private:
  typedef boost::icl::interval_set<size_t>      set_t;
  typedef set_t::interval_type                  ival;

  /*! Construct.
   */
  Trace();

  /*! Obtain the signature of the given trace-code.
   */
  Signature signature(size_t code) const;

  //! The signature.
  Signature m_signature;

  //! trace options.
  static std::map<String, size_t> s_trace_opts;

  //! Free codes arranged in a set of intervals.
  set_t m_free_codes;

  //! The singleton.
  static Trace* s_instance;
};

//! \brief
inline Boolean TRACE(size_t code)
{ return Trace::get_instance()->is_enabled(code); }

//! \brief enables the trace for the given code.
inline void Trace::enable(size_t code) { enable(signature(code)); }

//! \brief enables the trace for the given code-signature.
inline void Trace::enable(Signature my_signature)
{ m_signature |= my_signature; }

//! \brief disables the trace for the given code.
inline void Trace::disable(size_t code) { disable(signature(code)); }

//! \brief disables the trace for the given code-signature.
inline void Trace::disable(Signature my_signature)
{ m_signature &= ~my_signature; }

//! \brief determines whether the trace for the given code is enabled.
inline Boolean Trace::is_enabled(size_t code) const
{ return ((m_signature & signature(code)) != 0x0); }

//! \brief obtains the signature of the given trace-code.
inline Trace::Signature Trace::signature(size_t code)
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
