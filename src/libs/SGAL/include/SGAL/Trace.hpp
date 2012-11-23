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
// $Source: $
// $Revision: 12554 $
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

class Trace {
public:
  enum Code {
    GRAPHICS = 0,
    VRML_PARSING,
    WINDOW_MANAGER,
    EVENTS,
    INDEXED_FACE_SET,
    POLYHEDRON,
    GAUSSIAN_MAP,
    DESTRUCTOR,
    NUM_CODES
  };

  /*! Obtain the trace singleton */
  static Trace * get_instance();  

  /*! Enable the trace for the given code */
  void enable(Code code) { enable(signature(code)); }

  /*! Enable the trace for the given code-signature */
  void enable(Uint my_signature);

  /*! Disable the trace for the given code */
  void disable(Code code) { disable(signature(code)); }

  /*! Disable the trace for the given code-signature */
  void disable(Uint my_signature) { m_signature &= !my_signature; }

  /*! Is the trace for the given code enabled? */
  Boolean is_enabled(Code code)
  { return ((m_signature & signature(code)) != 0x0); }
  
private:
  /*! Constructor */
  Trace() : m_signature(0x0) {}

  /*! The singleton */
  static Trace * s_instance;

  /*! return the signature of the given trace-code */
  Uint signature(Code code) { return 0x1 << code; }
  
  /*! The signature */
  Uint m_signature;
};

inline Boolean TRACE(Trace::Code code)
{
  return Trace::get_instance()->is_enabled(code);
}

#if defined(NDEBUG)
#define TRACE_MSG(key, msg)
#define TRACE_CODE(key, code)
#else
#define TRACE_MSG(key, msg)     if (SGAL::TRACE(key)) std::cout << msg
#define TRACE_CODE(key, code)   if (SGAL::TRACE(key)) { code } else {}
#endif

SGAL_END_NAMESPACE

#endif
