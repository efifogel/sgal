// Copyright (c) 2014 Israel.
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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IO_OPTION_PARSER_HPP
#define SGAL_IO_OPTION_PARSER_HPP

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

class SGAL_SGAL_DECL IO_option_parser {
public:
  /*! Constructor */
  IO_option_parser();

  /*! Destructor */
  virtual ~IO_option_parser();

  /*! Apply the options
   * \param variable_map
   */
  void apply(po::variables_map& variable_map);

  /*! Obtain the IO-option description.
   * \return the IO-option description.
   */
  const po::options_description& get_io_opts() const;

  // /*! Configure */
  // void configure(po::variables_map& variable_map, Configuration * conf);

protected:
  /*! The options */
  po::options_description m_io_opts;

private:
  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  IO_option_parser& operator=(const IO_option_parser&);
  IO_option_parser(const IO_option_parser&);

  // In C++11, VC2013, the following is supported:
  // IO_option_parser& operator=(const IO_option_parser&) = delete;
  // IO_option_parser(const IO_option_parser&) = delete;
};

//! \brief obtains the IO-option description.
inline const po::options_description& IO_option_parser::get_io_opts() const
{ return m_io_opts; }

SGAL_END_NAMESPACE

#endif
