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

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Bench_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Bench_option_parser::Bench_option_parser() :
  m_bench_opts("SGAL bench options"),
  m_print_header(true),
  m_name_length(32),
  m_seconds(1),
  m_samples(10),
  m_iterations(1)
{
  // Generic options:
  m_bench_opts.add_options()
    ("header,p", po::value<bool>(&m_print_header)->default_value(true),
     "print header")
    ("name-length,n",
     po::value<Uint>(&m_name_length)->default_value(32),
     "name-field length")
    ("samples,s", po::value<Uint>(&m_samples)->default_value(10),
     "number of samples")
    ("seconds,t", po::value<Uint>(&m_seconds)->default_value(1),
     "number of seconds")
    ("iterations,i", po::value<Uint>(&m_iterations)->default_value(1),
     "number of iterations")
    ;
}

//! \brief destructor.
Bench_option_parser::~Bench_option_parser() {}

//! \brief applies the options.
void Bench_option_parser::apply() {}

SGAL_END_NAMESPACE
