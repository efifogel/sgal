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
// $Revision: 4979 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_BENCH_OPTION_PARSER_HPP
#define SGAL_BENCH_OPTION_PARSER_HPP

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 )
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
  /*! Constructor */
  Bench_option_parser();

  /*! Destructor */
  virtual ~Bench_option_parser();

  /*! Apply the options
   * \param variable_map
   */
  void apply(po::variables_map& variable_map);

  /*! Obtain the bench-option description */
  const po::options_description& get_bench_opts() const
  { return m_bench_opts; }

  /*! Display the log-table header? */
  Boolean is_print_header() const { return m_print_header; }

  /*! Obtain the numbr of charactes in the name field */
  Uint get_name_length() const { return m_name_length; }

  /*! Obtain the number of seconds allotted to perform the bench */
  Uint get_seconds()  const { return m_seconds; }

  /*! Obtain the number of samples to perform the bench */
  Uint get_samples()  const { return m_samples; }

  /*! Obtain the number of iterations */
  Uint get_iterations()  const { return m_iterations; }

protected:
  /*! The bench options */
  po::options_description m_bench_opts;

private:
  /*! Indicates whether to display the log-table header */
  Boolean m_print_header;

  /*! The numbr of charactes in the name field */
  Uint m_name_length;

  /*! Number of seconds allotted to perform the bench */
  Uint m_seconds;

  /*! Number of samples to perform the bench */
  Uint m_samples;

  /*! Number of iterations */
  Uint m_iterations;
};

SGAL_END_NAMESPACE

#endif
