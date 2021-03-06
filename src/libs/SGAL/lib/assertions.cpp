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

#include <cstdlib>
#include <iostream>
#include <cassert>

#include "SGAL/config.hpp"
#include "SGAL/assertions.hpp"
#include "SGAL/exceptions.hpp"

SGAL_BEGIN_NAMESPACE

// not_implemented function
// ------------------------
void not_implemented()
{
  assert(false);
}

// static behaviour variables
// --------------------------

static Failure_behaviour _error_behaviour   = THROW_EXCEPTION;
static Failure_behaviour _warning_behaviour = CONTINUE;

// standard error handlers
// -----------------------
static void _standard_error_handler(const char* what, const char* expr,
                                    const char* file, int line,
                                    const char* msg)
{
#if defined(__GNUG__) && !defined(__llvm__)
  // After g++ 3.4, std::terminate defaults to printing to std::cerr itself.
  if (_error_behaviour == THROW_EXCEPTION) return;
#endif
  std::cerr << "SGAL Error: " << what << " violation!" << std::endl
            << "Expr: " << expr << std::endl
            << "File: " << file << std::endl
            << "Line: " << line << std::endl;
  if (msg != nullptr) std::cerr << "Explanation: " << msg << std::endl;
}

// standard warning handler
// ------------------------
static void _standard_warning_handler(const char*, const char* expr,
                                      const char* file, int line,
                                      const char* msg)
{
#if defined(__GNUG__) && !defined(__llvm__)
    // After g++ 3.4, std::terminate defaults to printing to std::cerr itself.
    if (_warning_behaviour == THROW_EXCEPTION) return;
#endif
  std::cerr << "SGAL Warning: check violation!" << std::endl
            << "Expr: " << expr << std::endl
            << "File: " << file << std::endl
            << "Line: " << line << std::endl;
  if (msg != 0)
    std::cerr << "Explanation: " << msg << std::endl;
}

// default handler settings
// ------------------------
static Failure_function _error_handler = _standard_error_handler;

static Failure_function _warning_handler = _standard_warning_handler;

// failure functions
// -----------------
void assertion_fail(const char* expr, const char* file, int line,
                    const char* msg)
{
  (*_error_handler)("assertion", expr, file, line, msg);
  switch (_error_behaviour) {
   case ABORT: abort();
   case EXIT: exit(1);  // EXIT_FAILURE
   case EXIT_WITH_SUCCESS: exit(0);  // EXIT_SUCCESS
   case CONTINUE:
   case THROW_EXCEPTION:
   default: throw Assertion_exception("SGAL", expr, file, line, msg ? msg : "");
  }
}

void precondition_fail(const char* expr, const char* file, int line,
                       const char* msg)
{
  (*_error_handler)("precondition", expr, file, line, msg);
  switch (_error_behaviour) {
   case ABORT: abort();
   case EXIT: exit(1);  // EXIT_FAILURE
   case EXIT_WITH_SUCCESS: exit(0);  // EXIT_SUCCESS
   case CONTINUE: ;
   case THROW_EXCEPTION:
   default:
    throw Precondition_exception("SGAL", expr, file, line, msg ? msg : "");
  }
}

void postcondition_fail(const char* expr, const char* file, int line,
                        const char* msg)
{
  (*_error_handler)("postcondition", expr, file, line, msg);
  switch (_error_behaviour) {
   case ABORT: abort();
   case EXIT: exit(1);  // EXIT_FAILURE
   case EXIT_WITH_SUCCESS: exit(0);  // EXIT_SUCCESS
   case CONTINUE:
   case THROW_EXCEPTION:
   default:
    throw Postcondition_exception("SGAL", expr, file, line, msg ? msg : "");
  }
}

// warning function
// ----------------
void warning_fail(const char* expr, const char* file, int line,
                  const char* msg)
{
  (*_warning_handler)("warning", expr, file, line, msg);
  switch (_warning_behaviour) {
   case ABORT: abort();
   case EXIT: exit(1);  // EXIT_FAILURE
   case EXIT_WITH_SUCCESS: exit(0);  // EXIT_SUCCESS
   case THROW_EXCEPTION:
    throw Warning_exception("SGAL", expr, file, line, msg ? msg : "");
   case CONTINUE: ;
  }
}

// error handler set functions
// ---------------------------
  Failure_function set_error_handler(Failure_function handler)
{
  Failure_function result = _error_handler;
  _error_handler = handler;
  return(result);
}

Failure_function set_warning_handler(Failure_function handler)
{
  Failure_function result = _warning_handler;
  _warning_handler = handler;
  return(result);
}

Failure_behaviour set_error_behaviour(Failure_behaviour eb)
{
  Failure_behaviour result = _error_behaviour;
  _error_behaviour = eb;
  return result;
}

Failure_behaviour set_warning_behaviour(Failure_behaviour eb)
{
  Failure_behaviour result = _warning_behaviour;
  _warning_behaviour = eb;
  return result;
}

SGAL_END_NAMESPACE
