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
// $Revision: 12348 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FORMATTER_HPP
#define SGAL_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Array.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

/*! Writes a scene graph to an output stream in the VRML format */
class SGAL_SGAL_DECL Formatter {
public:
  /*! Constructor */
  Formatter() : m_out(NULL), m_in(NULL) {}

  /*! Construct an output formatter. */
  Formatter(std::ostream& os) : m_out(&os), m_in(NULL) {}

  /*! Construct an input formatter. */
  Formatter(std::istream& is) : m_out(NULL), m_in(&is) {}

  /*! Destructor */
  virtual ~Formatter() {}
  
  /*! Set the output stream */
  void set_out(std::ostream& os) { m_out = &os; }

  /*! Set the input stream */
  void set_in(std::istream& is) { m_in = &is; }

  /*! Get the output stream. */
  inline std::ostream& out()
  {
    SGAL_assertion(m_out != NULL);
    return *m_out;
  }

  /*! Get the input stream. */
  inline std::istream& in()
  {
    SGAL_assertion(m_in != NULL);
    return *m_in;
  }

  /// \name Write functions
  //@{

  /*! Write the headers of the scene graph */
  virtual void begin() = 0;

  /*! Write the routing statements */
  virtual void end() = 0;

  /*! Write a scene-graph container */  
  virtual void write(Container* container) = 0;

  /*! Write the container beginning section */
  virtual void container_begin(const std::string& tag) = 0;

  /*! Write the container ending section */
  virtual void container_end() = 0;

  /*! Write a single Boolean field */
  virtual void single_boolean(const std::string& name,
                              Boolean value, Boolean default_value) = 0;

  /*! Write a single Int field */
  virtual void single_int(const std::string& name,
                          Int value, Int default_value) = 0;
  
  /*! Write a single Vector3d field */
  virtual void single_vector3f(const std::string& name,
                               const Vector3f& value,
                               const Vector3f& default_value) = 0;
  
  /*! Write the single-container field beginning section */
  virtual void single_container_begin(const std::string& name) = 0;

  /*! Write the single-container field ending section */
  virtual void single_container_end() = 0;

  /*! Write a multi Uint field */
  virtual void multi_uint(const std::string& name,
                          const Array<Uint>& value) = 0;

  /*! Write the multi-container field beginning section */
  virtual void multi_container_begin(const std::string& name) = 0;

  /*! Write the multi-container field ending section */
  virtual void multi_container_end() = 0;
  
  //@}

protected:
  /*! The output stream */
  std::ostream* m_out;

  /*! The input stream */
  std::istream* m_in;
};

SGAL_END_NAMESPACE

#endif
