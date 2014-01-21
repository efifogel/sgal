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
// $Source$
// $Revision: 12347 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VRML_FORMATTER_HPP
#define SGAL_VRML_FORMATTER_HPP

/*! \file
 */

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

/*! Writes a scene graph to an output stream in the VRML format */
class SGAL_SGAL_DECL Vrml_formatter : public Formatter {
public:
  /*! Constructor */
  Vrml_formatter() : m_indent(0), m_indented(false) {}

  /*! Construct an output formatter. */
  Vrml_formatter(std::ostream& os) :
    Formatter(os), m_indent(0), m_indented(false) {}

  /*! Construct an input formatter. */
  Vrml_formatter(std::istream& is) :
    Formatter(is), m_indent(0), m_indented(false) {}

  /*! Destructor */
  virtual ~Vrml_formatter() {}

  /// \name Write functions
  //@{

  /*! Write the headers of the scene graph */
  virtual void begin();

  /*! Write the routing statements */
  virtual void end();

  /*! Write a scene-graph node */
  virtual void write(Container* container);

  /*! Write the container header */
  virtual void container_begin(const std::string& tag);

  /*! Write the container tailer */
  virtual void container_end();

  /*! Write a single Boolean field */
  virtual void single_boolean(const std::string& name,
                              Boolean value, Boolean default_value);

  /*! Write a single Int field */
  virtual void single_int(const std::string& name,
                          Int value, Int default_value)
  { single_field(name, value, default_value); }

  /*! Write a single Vector3d field */
  virtual void single_vector3f(const std::string& name,
                               const Vector3f& value,
                               const Vector3f& default_value)
  { single_field(name, value, default_value); }

  /*! Write the header of a single-container field */
  virtual void single_container_begin(const std::string& name);

  /*! Write the tailer of a single-container field */
  virtual void single_container_end();

  /*! Write a multi Uint field */
  virtual void multi_uint(const std::string& name, const Array<Uint>& value);

  /*! Write the header of a multi-container field */
  virtual void multi_container_begin(const std::string& name);

  /*! Write the tailer of a multi-container field */
  virtual void multi_container_end();

  //@}

private:
  /*! Write a single field of type T */
  template <typename T>
  void single_field(const std::string& name, const T& value,
                    const T& default_value)
  {
    if (value == default_value) return;
    new_line();
    indent();
    out() << name << " " << value;
    new_line();
  }

  /*! Obtain the indentation level */
  Uint get_indent() const { return m_indent; }

  /*! Write an indentation */
  void indent();

  /*! Begin a new line */
  void new_line();

  /*! Pop the indentation level */
  void pop_indent() { --m_indent; }

  /*! Pushes the indentation level */
  void push_indent() { ++m_indent; }

  /*! The indentation level */
  Uint m_indent;

  /*! Indicates whether indentation has been applied */
  Boolean m_indented;

  /*! Space holder for the output stream mode */
  // std::IO::Mode m_old_out_mode;

  /*! Space holder for the input stream mode */
  // IO::Mode m_old_in_mode;
};

SGAL_END_NAMESPACE

#endif
