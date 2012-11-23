// Copyright (c) 2007 INRIA Sophia-Antipolis (France) and
// Max-Planck-Institute Saarbruecken (Germany).
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
// $Revision: 4973 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>
//
// Large content of this file was copied from CGAL/exceptions.h, version 3.3

#ifndef SGAL_ERRORS_HPP
#define SGAL_ERRORS_HPP

/*! \file Exceptions for pre- and postconditions, assertions, warnings, and
 * general errors
 */

#include <string>
#include <boost/lexical_cast.hpp> 

#include "SGAL/errors.hpp"

SGAL_BEGIN_NAMESPACE

/*! \defgroup General Failures
 * \brief <tt>\#include <SGAL/basic.h></tt> for pre- and postconditions,
 * assertions, warnings, and error handler.
 *
 * \c SGAL/assertions.h provides macros for pre- and postconditions,
 * assertions, warnings, and errors that are active by default in debug
 * mode. There are variants for expensive checks that are inactive by
 * default.
 *
 * A failed precondition, postcondition, or assertion is a fatal error
 * by default. If one of them is detected, the program execution is aborted.
 * In case of a warning the program continues to run by default. Both defaul
 * behaviours can be changed with the respective \c error_behavior or
 * \c warning_behavior, or with the corresponding \c error_handler or
 * \c warning_handler functions.
 * 
 * These checks can be disabled with the corresponding preprocessor symbols
 * or with the \c NDEBUG. When disabled, none of these error handling
 * take place, and the program execution continues. Naturally, a
 * disastrous condition that is not handled may lead to disastrous results.
 *
 * These checks are not for external error handling. For example, testing
 * proper user input or file formats must be done differently, and must
 * response with a proper response to the user. Furthermore, it is
 * recommended to keep these checks active even in the production versions
 * of the program, as they should not cost too much runtime overhead, and
 * they are active only in debug mode.
 */
//@{

// Failure types and exceptions
// ============================

//! Exception base class for all failure types of assertions etc.
class Failure_exception : public error {
  /*! The name of the library */
  std::string m_lib;

  /*! The expression */
  std::string m_expr; // can be empty

  /*! The file name */
  std::string m_file;

  /*! The line number */
  int m_line;

  /*! The message to the user that runs the program */
  std::string m_msg;  // can be empty

public:
  /*! Constructor
   * Initialize local members and the <tt>std::logic_error</tt> with a
   * suitable message.
   */
  Failure_exception(std::string lib, std::string expr, std::string file,
                    int line, std::string msg,
                    std::string kind = "Unknown kind") :
    error(lib + std::string(" ERROR: ") + kind + std::string("!")
          + ((expr.empty()) ? (std::string("")) :
             (std::string("\nExpr: ")+expr))
          + std::string( "\nFile: ") + file
          + std::string( "\nLine: ") + boost::lexical_cast<std::string>(line)
          + ((msg.empty()) ? (std::string("")) :
             (std::string("\nExplanation: ") + msg))),
    m_lib(lib),
    m_expr(expr),
    m_file(file),
    m_line(line),
    m_msg(msg)
  {}

  /*! Destructor */
  ~Failure_exception() throw() {}

  /*! Obtain the name of the library that issued this message */
  std::string library() const { return m_lib; }

  /*! Obtain the expression that failed in assertion, precondition, or
   * postcondition
   */
  std::string expression() const { return m_expr; }

  /*! Obtain the source code filename where the failure was detected */
  std::string filename() const { return m_file; }
    
  /*! Obtain the line number in source code file where the failure was
   * detected
   */
  int line_number() const { return m_line; }

  /*! Obtain the optional message explaining the kind of failure */
  std::string message() const { return m_msg; }
};

//! Exception thrown for \c SGAL_error.
class Error_exception : public Failure_exception {
public:
  Error_exception(std::string lib, std::string msg, std::string file,
                  int line) :
    Failure_exception(lib, "", file, line, msg, "failure") {}
};

//! Exception thrown for \c SGAL_precond.
class Precondition_exception : public Failure_exception {
public:
  Precondition_exception(std::string lib, std::string expr, std::string file,
                         int line, std::string msg) :
    Failure_exception(lib, expr, file, line, msg, "precondition violation") {}
};

//! Exception thrown for \c SGAL_postcond.
class Postcondition_exception : public Failure_exception {
public:
  Postcondition_exception(std::string lib, std::string expr, std::string file,
                          int line, std::string msg) :
    Failure_exception(lib, expr, file, line, msg, "postcondition violation") {}
};

//! Exception thrown for \c SGAL_assert.
class Assertion_exception : public Failure_exception {
public:
  Assertion_exception(std::string lib, std::string expr, std::string file,
                      int line, std::string msg) :
    Failure_exception(lib, expr, file, line, msg, "assertion violation") {}
};

//! Exception thrown for \c SGAL_test.
class Test_exception : public Failure_exception {
public:
    Test_exception(std::string lib, std::string expr, std::string file,
                   int line, std::string msg) :
      Failure_exception(lib, expr, file, line, msg,
                        "test in test-suite violation") {}
};

//! Exception thrown for \c SGAL_warning.
class Warning_exception : public Failure_exception {
public:
  Warning_exception(std::string lib, std::string expr, std::string file,
                    int line, std::string msg) :
    Failure_exception(lib, expr, file, line, msg,
                      "warning condition failed") {}
};

SGAL_END_NAMESPACE

#endif
