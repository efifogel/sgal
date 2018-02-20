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

#ifndef SGAL_BENCH_OPTION_PARSER_HPP
#define SGAL_BENCH_OPTION_PARSER_HPP

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512 )
#endif
#include <boost/program_options.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class SGAL_SGAL_DECL Bench_option_parser {
public:
  /*! Constructor. */
  Bench_option_parser();

  /*! Destructor. */
  virtual ~Bench_option_parser();

  /*! Obtain the variable map.
   * \return the variable map.
   */
  virtual const po::variables_map& get_variable_map() const = 0;

  /*! Apply the options.
   */
  void apply();

  /*! Obtain the bench-option description. */
  const po::options_description& get_bench_opts() const;

  /*! Display the log-table header? */
  Boolean is_print_header() const;

  /*! Obtain the numbr of charactes in the name field. */
  Uint get_name_length() const;

  /*! Obtain the number of seconds allotted to perform the bench. */
  Uint get_seconds()  const;

  /*! Obtain the number of samples to perform the bench. */
  Uint get_samples()  const;

  /*! Obtain the number of iterations. */
  Uint get_iterations()  const;

protected:
  /*! The bench options */
  po::options_description m_bench_opts;

private:
  /*! Indicates whether to display the log-table header. */
  Boolean m_print_header;

  /*! The numbr of charactes in the name field. */
  Uint m_name_length;

  /*! Number of seconds allotted to perform the bench. */
  Uint m_seconds;

  /*! Number of samples to perform the bench. */
  Uint m_samples;

  /*! Number of iterations. */
  Uint m_iterations;

  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  Bench_option_parser& operator=(const Bench_option_parser&);
  Bench_option_parser(const Bench_option_parser&);

  // In C++11, VC2013, the following is supported:
  // Bench_option_parser& operator=(const Bench_option_parser&) = delete;
  // Bench_option_parser(const Bench_option_parser&) = delete;
};

//! \brief obtains the bench-option description.
inline const po::options_description&
Bench_option_parser::get_bench_opts() const { return m_bench_opts; }

//! \brief determines whether to display the log-table header.
inline Boolean Bench_option_parser::is_print_header() const
{ return m_print_header; }

//! \brief obtains the numbr of charactes in the name field.
inline Uint Bench_option_parser::get_name_length() const
{ return m_name_length; }

//! \brief obtains the number of seconds allotted to perform the bench.
inline Uint Bench_option_parser::get_seconds()  const { return m_seconds; }

//! \brief obtains the number of samples to perform the bench.
inline Uint Bench_option_parser::get_samples()  const { return m_samples; }

//! \brief obtains the number of iterations.
inline Uint Bench_option_parser::get_iterations()  const
{ return m_iterations; }

SGAL_END_NAMESPACE

#endif
