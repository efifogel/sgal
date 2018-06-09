// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_BASE_LOADER_HPP
#define SGAL_BASE_LOADER_HPP

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Loader_code.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;

//! A base parser used as a base class for parsers.
class SGAL_SGAL_DECL Base_loader {
public:
  /*! Construct.
   * \param[i] is the input stream.
   * \param[i] filename the input filename.
   */
  Base_loader(Scene_graph* m_scene_graph);

  /*! Parse.
   */
  virtual Loader_code operator()(std::istream& is, const String& filename);

  /*! Obtain the input stream.
   */
  std::istream& input_stream();

  /*! Obatin the file name.
   */
  const String& filename() const;

  /*! Obtain the current line number.
   */
  size_t line() const;

  /*! Set the current line number.
   */
  void set_line(size_t line);

  /*! Increment the line number.
   */
  void inc_line();

protected:
  //! The text input stream to parse.
  std::istream* m_is;

  //! The scene graph.
  Scene_graph* m_scene_graph;

  //! The file name
  const String* m_filename;

  //! The current line number
  size_t m_line;

public: //! \todo temporary
  //! The trace code.
  size_t m_trace_code;
};

//! \brief parses.
inline Loader_code Base_loader::operator()(std::istream& is,
                                           const String& filename)
{
  m_is = &is;
  m_filename = &filename;
  m_line = 0;
}

//! \brief constructs.
inline Base_loader::Base_loader(Scene_graph* sg) :
  m_scene_graph(sg),
  m_line(0),
  m_trace_code(static_cast<size_t>(Trace::INVALID))
{}

//! \brief obtains the input stream.
inline std::istream& Base_loader::input_stream() { return *m_is; }

//! \brief obatins the file name.
inline const String& Base_loader::filename() const { return *m_filename; }

//! \brief obtains the current line number.
inline size_t Base_loader::line() const { return m_line; }

//! \brief sets the current line number.
inline void Base_loader::set_line(size_t line) { m_line = line; }

//! \brief increments the line number.
inline void Base_loader::inc_line() { ++m_line; }

SGAL_END_NAMESPACE

#endif
