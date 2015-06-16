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
#include "SGAL/File_format_2d.hpp"
#include "SGAL/File_format_3d.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL IO_option_parser {
public:
  typedef std::vector<File_format_2d::Id>   Formats_2d;
  typedef Formats_2d::const_iterator        Format_2d_const_iter;

  typedef std::vector<File_format_3d::Id>   Formats_3d;
  typedef Formats_3d::const_iterator        Format_3d_const_iter;

  typedef std::vector<std::string>          Input_path;
  typedef Input_path::const_iterator        Input_path_const_iterator;

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

  /*! Obtain the begin iterator of the selected 2d format container.
   * \return the begin iterator of the selected 2d format container.
   */
  Format_2d_const_iter formats_2d_begin() const;

  /*! Obtain the past-the-end iterator of the selected 2d format container.
   * \return the past-the-end iterator of the selected 2d format container.
   */
  Format_2d_const_iter formats_2d_end() const;

  /*! Obtain the size of the selected 2d format container.
   * \return the size of the selected 2d format container.
   */
  size_t formats_2d_size() const;

  /*! Determine whether to take a snapshot of the scene and save it in a file.
   * \return true if a snapshot of the the scene should be taken;
   *         false otherwise.
   */
  Boolean do_snapshot() const;

  /*! Obtain the begin iterator of the selected 3d format container.
   * \return the begin iterator of the selected 3d format container.
   */
  Format_3d_const_iter formats_3d_begin() const;

  /*! Obtain the past-the-end iterator of the selected 3d format container.
   * \return the past-the-end iterator of the selected 3d format container.
   */
  Format_3d_const_iter formats_3d_end() const;

  /*! Obtain the size of the selected 3d format container.
   * \return the size of the selected 3d format container.
   */
  size_t formats_3d_size() const;

  /*! Determine whether to export the scene.
   * \return true if the scene should be saved in output files; false otherwise.
   */
  Boolean do_export() const;

  /*! Determine whether the operation is interactive.
   * \return true if the operation is interactive; false otherwise.
   */
  Boolean is_interactive() const;

  /*! Determine whether the interactive option has not been specified.
   * \return true if the interactive option was left defaulted.
   */
  Boolean is_interactive_defaulted() const;

  /*! Obtain the output-file name.
   * \return The output-file name.
   */
  const std::string& get_output_file() const;

  /*! Determine whether the output-file option is empty.
   * \return true if the output-file option is empty.
   */
  Boolean is_output_file_empty() const;

  /*! Obtain the output-path name.
   * \return The output-path name.
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

  /*! The selected 2D formats. */
  Formats_2d m_formats_2d;

  /*! The selected 3D formats. */
  Formats_3d m_formats_3d;

  /*! Indicates whether a snapshot of the scene should be taken. */
  Boolean m_snapshot;

  /*! Indicates whether to save the scene to an output file. */
  Boolean m_export;

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

//! \brief obtains the begin iterator of the selected 2d format container.
inline
IO_option_parser::Format_2d_const_iter IO_option_parser::formats_2d_begin() const
{ return m_formats_2d.begin(); }

//! \brief obtains the past-the-end iterator of the selected 2d format container.
inline
IO_option_parser::Format_2d_const_iter IO_option_parser::formats_2d_end() const
{ return m_formats_2d.end(); }

//! \brief obtains the size of the selected 2d format container.
inline size_t IO_option_parser::formats_2d_size() const
{ return m_formats_2d.size(); }

//! \brief determines whether a snapshot of the scene should be taken.
inline Boolean IO_option_parser::do_snapshot() const { return m_snapshot; }

//! \brief obtains the begin iterator of the selected 3d format container.
inline
IO_option_parser::Format_3d_const_iter IO_option_parser::formats_3d_begin() const
{ return m_formats_3d.begin(); }

//! \brief obtains the past-the-end iterator of the selected 3d format container.
inline
IO_option_parser::Format_3d_const_iter IO_option_parser::formats_3d_end() const
{ return m_formats_3d.end(); }

//! \brief obtains the size of the selected 3d format container.
inline size_t IO_option_parser::formats_3d_size() const
{ return m_formats_3d.size(); }

//! \brief determines whether to save to output files.
inline Boolean IO_option_parser::do_export() const { return m_export; }

//! \brief determines whether the operation is interactive.
inline Boolean IO_option_parser::is_interactive() const
{ return m_interactive; }

//! \brief determines whether the interactive option has not been specified.
inline Boolean IO_option_parser::is_interactive_defaulted() const
{ return get_variable_map()["interactive"].defaulted(); }

//! \brief obtains the output-file name.
inline const std::string& IO_option_parser::get_output_file() const
{ return m_output_file; }

//! \brief determines whether the output-file option is empty.
inline Boolean IO_option_parser::is_output_file_empty() const
{ return get_variable_map()["output-file"].empty(); }

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

/*! Extractors of 3D (graphics) and 2D (image) file format types from the
 * corresponding Id types.
 */
template <typename T> struct File_format;
template <> struct File_format<File_format_2d::Id>
{ typedef File_format_2d type; };
template <> struct File_format<File_format_3d::Id>
{ typedef File_format_3d type; };

//! Import 3D (graphics) and 2D (image) file formats
template <typename InputStream, typename Id>
InputStream& import(InputStream& in, Id& format)
{
  typedef typename File_format<Id>::type                My_file_format;
  std::string token;
  in >> token;
  for (size_t i = 0; i < My_file_format::NUM_IDS; ++i) {
    if (! My_file_format::compare_name(i, token)) continue;
    format = static_cast<Id>(i);
    return in;
  }
  throw po::validation_error(po::validation_error::invalid_option_value);
  return in;
}

//! Import 2D (image) file formats
template <typename InputStream>
InputStream& operator>>(InputStream& in, File_format_2d::Id& format)
{ return import(in, format); }

//! Import 3D (graphics) file formats
template <typename InputStream>
InputStream& operator>>(InputStream& in, File_format_3d::Id& format)
{ return import(in, format); }

SGAL_END_NAMESPACE

#endif
