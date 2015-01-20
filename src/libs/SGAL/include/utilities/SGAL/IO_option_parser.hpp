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

  typedef std::vector<std::string>      Input_path;
  typedef Input_path::const_iterator    Input_path_const_iterator;

  /*! Constructor. */
  IO_option_parser();

  /*! Destructor. */
  virtual ~IO_option_parser();

  /*! Obtain the variable map.
   * \return the variable map.
   */
  virtual const po::variables_map& get_variable_map() const = 0;

  /*! Apply the options.
   */
  void apply();

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
   * \return true if the operation is interactive; false otherwise.
   */
  Boolean is_interactive() const;

  /*! Set the output-file name.
   * \param filename (in) the output-file name.
   */
  void set_output_file(const std::string& filename);

  /*! Obtain the output-file name.
   * \return The output-file name.
   */
  const std::string& get_output_file() const;

  /*! Obtain the output-path.
   * \return The output-path.
   */
  const std::string& get_output_path() const;

  /*! Obtain the begin iterator of input paths.
   */
  Input_path_const_iterator dirs_begin();

  /*! Obtain the pass-the-end iterator of input paths.
   */
  Input_path_const_iterator dirs_end();

  /*! Apply a given function object to all input paths.
   */
  template <typename UnaryFunction>
  UnaryFunction for_each_dir(UnaryFunction func);

protected:
  /*! The options. */
  po::options_description m_io_opts;

  /*! The selected formats. */
  Formats m_formats;

  /*! Indicates whether to save the scene to an output file. */
  Boolean m_save;

  /*! Indicates whether the operation is interactive. */
  Boolean m_interactive;

  /*! Output file name. */
  std::string m_output_file;

  /*! Output path name. */
  std::string m_output_path;

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

//! \brief sets the output-file name.
inline void IO_option_parser::set_output_file(const std::string& filename)
{ m_output_file = filename; }

//! \brief obtains the output-file name.
inline const std::string& IO_option_parser::get_output_file() const
{ return m_output_file; }

//! \brief obtains the output path.
inline const std::string& IO_option_parser::get_output_path() const
{ return m_output_path; }

//! \brief obtains the begin iterator of input paths.
inline IO_option_parser::Input_path_const_iterator
IO_option_parser::dirs_begin()
{ return get_variable_map()["input-path"].as<Input_path>().begin(); }

//! \broef obtains the pass-the-end iterator of input paths.
inline IO_option_parser::Input_path_const_iterator
IO_option_parser::dirs_end()
{ return get_variable_map()["input-path"].as<Input_path>().end(); }

//! \brief applies a given function object to all input paths.
template <typename UnaryFunction>
inline UnaryFunction IO_option_parser::for_each_dir(UnaryFunction func)
{
  if (!get_variable_map().count("input-path")) return func;
  return std::for_each(dirs_begin(), dirs_end(), func);
}

SGAL_END_NAMESPACE

#endif
