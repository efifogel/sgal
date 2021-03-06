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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IO_OPTION_PARSER_HPP
#define SGAL_IO_OPTION_PARSER_HPP

#include <vector>
#include <string>

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
#include "SGAL/Image_format.hpp"
#include "SGAL/Geometry_format.hpp"

namespace po = boost::program_options;

SGAL_BEGIN_NAMESPACE

class Configuration;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL IO_option_parser {
public:
  typedef std::vector<Uint>                     File_formats;
  typedef File_formats::const_iterator          Format_const_iter;

  typedef std::vector<std::string>              Input_path;
  typedef Input_path::const_iterator            Input_path_const_iterator;

  /*! Construct. */
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

  /*! Configure.
   */
  void configure(Configuration* conf);

  /*! Obtain the begin iterator of the selected image format container.
   * \return the begin iterator of the selected image format container.
   */
  Format_const_iter image_formats_begin() const;

  /*! Obtain the past-the-end iterator of the selected image format container.
   * \return the past-the-end iterator of the selected image format container.
   */
  Format_const_iter image_formats_end() const;

  /*! Obtain the size of the selected image format container.
   * \return the size of the selected image format container.
   */
  size_t image_formats_size() const;

  /*! Determine whether to take a snapshot of the scene and save it in a file.
   * \return true if a snapshot of the the scene should be taken;
   *         false otherwise.
   */
  Boolean do_snapshot() const;

  /*! Obtain the begin iterator of the selected geometry format container.
   * \return the begin iterator of the selected geometry format container.
   */
  Format_const_iter geometry_formats_begin() const;

  /*! Obtain the past-the-end iterator of the selected geometry format container.
   * \return the past-the-end iterator of the selected geometry format container.
   */
  Format_const_iter geometry_formats_end() const;

  /*! Obtain the size of the selected geometry format container.
   * \return the size of the selected geometry format container.
   */
  size_t geometry_formats_size() const;

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

  /*! Determine whether the output file format is binary.
   * \return true if the output file format is binary.
   */
  Boolean is_binary() const;

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

  /*! Obtain the root of the scene to export.
   * \return the root of the scene to export:
   *         0---The root of the scene, which implies that the exported scene
   *             will include the entire scene including the camera and the
   *             light sources.
   *         1---The  navigation root.
   *         2---The children of the navigation root.
   */
  Uint get_export_scene_root() const;


  /*! Determine whether to export non visible geometries.
   * \return true if non-visible geometries should be exported.
   */
  Boolean get_export_non_visible() const;

protected:
  //! The options.
  po::options_description m_io_opts;

  //! The selected image formats.
  File_formats m_image_formats;

  //! The selected geometry formats.
  File_formats m_geometry_formats;

  //! Indicates whether a snapshot of the scene should be taken.
  Boolean m_snapshot;

  //! Indicates whether to save the scene to an output file.
  Boolean m_export;

  //! Indicates whether the operation is interactive.
  Boolean m_interactive;

  //! Indicates whether the output file format is binary.
  Boolean m_binary;

  //! Output file name.
  std::string m_output_file;

  //! Output path name.
  std::string m_output_path;

  /*! The root of the scene to export:
   * 0---The root of the scene, which implies that the exported scene will
   *     include the entire scene including the camera and the light sources.
   * 1---The  navigation root.
   * 2---The children of the navigation root.
   */
  Uint m_export_scene_root;

  //! Indicates whether to export non-visible geometries.
  Boolean m_export_non_visible;

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

//! \brief obtains the begin iterator of the selected image format container.
inline
IO_option_parser::Format_const_iter
IO_option_parser::image_formats_begin() const
{ return m_image_formats.begin(); }

/*! \brief obtains the past-the-end iterator of the selected image format
 * container.
 */
inline
IO_option_parser::Format_const_iter IO_option_parser::image_formats_end() const
{ return m_image_formats.end(); }

//! \brief obtains the size of the selected 2d format container.
inline size_t IO_option_parser::image_formats_size() const
{ return m_image_formats.size(); }

//! \brief determines whether a snapshot of the scene should be taken.
inline Boolean IO_option_parser::do_snapshot() const { return m_snapshot; }

//! \brief obtains the begin iterator of the selected geometry format container.
inline IO_option_parser::Format_const_iter
IO_option_parser::geometry_formats_begin() const
{ return m_geometry_formats.begin(); }

/*! \brief obtains the past-the-end iterator of the selected geometry format
 * container.
 */
inline IO_option_parser::Format_const_iter
IO_option_parser::geometry_formats_end() const
{ return m_geometry_formats.end(); }

//! \brief obtains the size of the selected geometry format container.
inline size_t IO_option_parser::geometry_formats_size() const
{ return m_geometry_formats.size(); }

//! \brief determines whether to save to output files.
inline Boolean IO_option_parser::do_export() const { return m_export; }

//! \brief determines whether the operation is interactive.
inline Boolean IO_option_parser::is_interactive() const
{ return m_interactive; }

//! \brief determines whether to save to output files.
inline Boolean IO_option_parser::is_binary() const { return m_binary; }

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

template <> struct File_format<Image_format::Code>
{ typedef Image_format type; };

template <> struct File_format<Geometry_format::Code>
{ typedef Geometry_format type; };

//! Import geometry and image file formats
template <typename InputStream, typename Code>
InputStream& import(InputStream& in, Code& format)
{
  typedef typename File_format<Code>::type                My_format;
  auto* file_format = My_format::get_instance();
  std::string name;
  in >> name;
  format = file_format->find_code(name);
  if (format != My_format::INVALID) return in;

  throw po::validation_error(po::validation_error::invalid_option_value);
  return in;
}

//! Import image file formats
template <typename InputStream>
InputStream& operator>>(InputStream& in, Image_format::Code& format)
{ return import(in, format); }

//! Import geometry file formats
template <typename InputStream>
InputStream& operator>>(InputStream& in, Geometry_format::Code& format)
{ return import(in, format); }

//! \brief obtains the root of the scene to export.
inline Uint IO_option_parser::get_export_scene_root() const
{ return m_export_scene_root; }

SGAL_END_NAMESPACE

#endif
