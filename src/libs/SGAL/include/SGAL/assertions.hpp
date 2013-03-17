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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ASSERTIONS_HPP
#define SGAL_ASSERTIONS_HPP

#include "SGAL/config.hpp"

SGAL_BEGIN_NAMESPACE

// types
// =====

enum Failure_behaviour { ABORT, EXIT, EXIT_WITH_SUCCESS, CONTINUE };

// function declarations
// =====================
// failure functions
// -----------------
void assertion_fail(const char*, const char*, int, const char* = 0);
void precondition_fail(const char*, const char*, int, const char* = 0);
void postcondition_fail(const char*, const char*, int, const char* = 0);

// warning function
// ----------------
void warning_fail(const char*, const char*, int, const char* = 0);


// macro definitions
// =================
// assertions
// ----------


#if defined(SGAL_NO_ASSERTIONS) || defined(NDEBUG)
#  define SGAL_assertion(EX) (static_cast<void>(0))
#  define SGAL_assertion_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_assertion_code(CODE)
#else
#  define SGAL_assertion(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__))
#  define SGAL_assertion_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_assertion_code(CODE) CODE
#endif

#if defined(SGAL_NO_ASSERTIONS) || !defined(SGAL_CHECK_EXACTNESS) \
  || defined(NDEBUG)
#  define SGAL_exactness_assertion(EX) (static_cast<void>(0))
#  define SGAL_exactness_assertion_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_exactness_assertion_code(CODE)
#else
#  define SGAL_exactness_assertion(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__))
#  define SGAL_exactness_assertion_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_exactness_assertion_code(CODE) CODE
#endif

#if defined(SGAL_NO_ASSERTIONS) || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_assertion(EX) (static_cast<void>(0))
#  define SGAL_expensive_assertion_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_assertion_code(CODE)
#else
#  define SGAL_expensive_assertion(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_assertion_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_assertion_code(CODE) CODE
#endif

#if defined(SGAL_NO_ASSERTIONS) \
  || !defined(SGAL_CHECK_EXACTNESS) \
  || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_exactness_assertion(EX) (static_cast<void>(0))
#  define SGAL_expensive_exactness_assertion_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_exactness_assertion_code(CODE)
#else
#  define SGAL_expensive_exactness_assertion(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_exactness_assertion_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::assertion_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_exactness_assertion_code(CODE) CODE
#endif


// preconditions
// -------------

#if defined(SGAL_NO_PRECONDITIONS) || defined(NDEBUG)
#  define SGAL_precondition(EX) (static_cast<void>(0))
#  define SGAL_precondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_precondition_code(CODE)
#else
#  define SGAL_precondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_precondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_precondition_code(CODE) CODE
#endif

#if defined(SGAL_NO_PRECONDITIONS) || !defined(SGAL_CHECK_EXACTNESS) \
  || defined(NDEBUG)
#  define SGAL_exactness_precondition(EX) (static_cast<void>(0))
#  define SGAL_exactness_precondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_exactness_precondition_code(CODE)
#else
#  define SGAL_exactness_precondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_exactness_precondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_exactness_precondition_code(CODE) CODE
#endif

#if defined(SGAL_NO_PRECONDITIONS) || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_precondition(EX) (static_cast<void>(0))
#  define SGAL_expensive_precondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_precondition_code(CODE)
#else
#  define SGAL_expensive_precondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_precondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_precondition_code(CODE) CODE
#endif

#if defined(SGAL_NO_PRECONDITIONS) \
  || !defined(SGAL_CHECK_EXACTNESS) \
  || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_exactness_precondition(EX) (static_cast<void>(0))
#  define SGAL_expensive_exactness_precondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_exactness_precondition_code(CODE)
#else
#  define SGAL_expensive_exactness_precondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_exactness_precondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::precondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_exactness_precondition_code(CODE) CODE
#endif


// postconditions
// --------------

#if defined(SGAL_NO_POSTCONDITIONS) || defined(NDEBUG)
#  define SGAL_postcondition(EX) (static_cast<void>(0))
#  define SGAL_postcondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_postcondition_code(CODE)
#else
#  define SGAL_postcondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_postcondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_postcondition_code(CODE) CODE
#endif

#if defined(SGAL_NO_POSTCONDITIONS) || !defined(SGAL_CHECK_EXACTNESS) \
  || defined(NDEBUG)
#  define SGAL_exactness_postcondition(EX) (static_cast<void>(0))
#  define SGAL_exactness_postcondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_exactness_postcondition_code(CODE)
#else
#  define SGAL_exactness_postcondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_exactness_postcondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_exactness_postcondition_code(CODE) CODE
#endif

#if defined(SGAL_NO_POSTCONDITIONS) || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_postcondition(EX) (static_cast<void>(0))
#  define SGAL_expensive_postcondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_postcondition_code(CODE)
#else
#  define SGAL_expensive_postcondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_postcondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_postcondition_code(CODE) CODE
#endif

#if defined(SGAL_NO_POSTCONDITIONS) \
  || !defined(SGAL_CHECK_EXACTNESS) \
  || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_exactness_postcondition(EX) (static_cast<void>(0))
#  define SGAL_expensive_exactness_postcondition_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_exactness_postcondition_code(CODE)
#else
#  define SGAL_expensive_exactness_postcondition(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_exactness_postcondition_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::postcondition_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_exactness_postcondition_code(CODE) CODE
#endif


// warnings
// --------

#if defined(SGAL_NO_WARNINGS) || defined(NDEBUG)
#  define SGAL_warning(EX) (static_cast<void>(0))
#  define SGAL_warning_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_warning_code(CODE)
#else
#  define SGAL_warning(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__))
#  define SGAL_warning_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_warning_code(CODE) CODE
#endif

#if defined(SGAL_NO_WARNINGS) || !defined(SGAL_CHECK_EXACTNESS) \
  || defined(NDEBUG)
#  define SGAL_exactness_warning(EX) (static_cast<void>(0))
#  define SGAL_exactness_warning_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_exactness_warning_code(CODE)
#else
#  define SGAL_exactness_warning(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__))
#  define SGAL_exactness_warning_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_exactness_warning_code(CODE) CODE
#endif

#if defined(SGAL_NO_WARNINGS) || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_warning(EX) (static_cast<void>(0))
#  define SGAL_expensive_warning_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_warning_code(CODE)
#else
#  define SGAL_expensive_warning(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_warning_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_warning_code(CODE) CODE
#endif

#if defined(SGAL_NO_WARNINGS) \
  || !defined(SGAL_CHECK_EXACTNESS) \
  || !defined(SGAL_CHECK_EXPENSIVE) \
  || defined(NDEBUG)
#  define SGAL_expensive_exactness_warning(EX) (static_cast<void>(0))
#  define SGAL_expensive_exactness_warning_msg(EX,MSG) (static_cast<void>(0))
#  define SGAL_expensive_exactness_warning_code(CODE)
#else
#  define SGAL_expensive_exactness_warning(EX) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__))
#  define SGAL_expensive_exactness_warning_msg(EX,MSG) \
  ((EX) ? (static_cast<void>(0)): \
   ::SGAL::warning_fail(# EX , __FILE__, __LINE__, MSG))
#  define SGAL_expensive_exactness_warning_code(CODE) CODE
#endif


// failure handler declarations
// ==========================
// failure handler
// ---------------
typedef void (*Failure_function)(const char*, const char*, const char*,
                                 int, const char*);

Failure_function set_error_handler(Failure_function handler);
Failure_function set_warning_handler(Failure_function handler);

// failure behaviour handler
// -------------------------
Failure_behaviour set_error_behaviour(Failure_behaviour eb);
Failure_behaviour set_warning_behaviour(Failure_behaviour eb);

// SGAL error
#define SGAL_error_msg(MSG) ::SGAL::assertion_fail( "", __FILE__, __LINE__, MSG )
#define SGAL_error()        ::SGAL::assertion_fail( "", __FILE__, __LINE__ )

SGAL_END_NAMESPACE

#endif
