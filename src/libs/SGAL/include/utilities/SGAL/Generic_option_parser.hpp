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

#ifndef SGAL_GENERIC_OPTION_PARSER_HPP
#define SGAL_GENERIC_OPTION_PARSER_HPP

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512 )
#endif
#include <boost/program_options.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class SGAL_SGAL_DECL Generic_option_parser {
public:
  enum Generic_option_id { AUTHOR, HELP, LICENSE, VERSION };
  struct Generic_option_exception {
    Generic_option_exception(Generic_option_id option) : m_option(option) {}
    Generic_option_id m_option;
  };

  /*! Constructor. */
  Generic_option_parser();

  /*! Destructor. */
  virtual ~Generic_option_parser();

  /*! Obtain the variable map.
   * \return the variable map.
   */
  virtual const po::variables_map& get_variable_map() const = 0;

  /*! Apply the options.
   */
  void apply();

  /*! Obtain the generic-option description. */
  const po::options_description& get_generic_opts() const;

protected:
  /*! The generic options. */
  po::options_description m_generic_opts;

private:
  virtual const po::options_description& get_visible_opts() const = 0;

  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  Generic_option_parser& operator=(const Generic_option_parser&);
  Generic_option_parser(const Generic_option_parser&);

  // In C++11, VC2013, the following is supported:
  // Generic_option_parser& operator=(const Generic_option_parser&) = delete;
  // Generic_option_parser(const Generic_option_parser&) = delete;
};

//! \brief obtains the generic-option description.
inline const po::options_description&
Generic_option_parser::get_generic_opts() const
{ return m_generic_opts; }

SGAL_END_NAMESPACE

#endif
