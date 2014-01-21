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

#include <vector>
#include <string>

#ifndef SGAL_IO_OPTION_PARSER_HPP
#define SGAL_IO_OPTION_PARSER_HPP

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512)
#endif
#include <boost/program_options.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/File_format.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL IO_option_parser {
public:
  typedef std::vector<File_format::Id>  Formats;
  typedef Formats::const_iterator       Format_const_iter;

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

  /*! Obtain the begin iterator of the selected format container.
   * \return the begin iterator of the selected format container.
   */
  Format_const_iter formats_begin() const;

  /*! Obtain the past-the-end iterator of the selected format container.
   * \return the past-the-end iterator of the selected format container.
   */
  Format_const_iter formats_end() const;

  /*! Obtain the size of the selected format container.
   * \return the size of the selected format container.
   */
  Uint formats_size() const;

  /*! Determine whether to save the scene to output files.
   * \return true if the scene should be saved in output files; false otherwise.
   */
  Boolean do_save() const;

  /*! Determine whether the operation is interactive.
   * \param variable_map
   * \return true if the operation is interactive; false otherwise.
   */
  Boolean is_interactive() const;

  /*! Obtain the output-file name.
   * \return The output-file name.
   */
  const std::string& get_output_file() const;

protected:
  /*! The options */
  po::options_description m_io_opts;

  /*! The selected formats. */
  Formats m_formats;

  /*! Indicates whether to save the scene to an output file. */
  Boolean m_save;

  /*! Indicates whether the operation is interactive. */
  Boolean m_interactive;

  /*! Output file name. */
  std::string m_output_file;

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

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the IO-option description.
inline const po::options_description& IO_option_parser::get_io_opts() const
{ return m_io_opts; }

//! \brief obtains the begin iterator of the selected format container.
inline
IO_option_parser::Format_const_iter IO_option_parser::formats_begin() const
{ return m_formats.begin(); }

//! \brief obtains the past-the-end iterator of the selected format container.
inline
IO_option_parser::Format_const_iter IO_option_parser::formats_end() const
{ return m_formats.end(); }

//! \brief obtains the size of the selected format container.
inline Uint IO_option_parser::formats_size() const { return m_formats.size(); }

//! \brief determines whether to save to output files.
inline Boolean IO_option_parser::do_save() const { return m_save; }

//! \brief determines whether the operation is interactive.
inline Boolean IO_option_parser::is_interactive() const
{ return m_interactive; }

//! \brief obtains the output-file name.
inline const std::string& IO_option_parser::get_output_file() const
{ return m_output_file; }

SGAL_END_NAMESPACE

#endif
