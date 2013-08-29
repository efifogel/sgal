// Copyright (c) 2013 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Base class for shaders. The shader container represents a shader---a program
 * designed to run on some stage of a graphics processor.
 */

#ifndef SGAL_SHADER_HPP
#define SGAL_SHADER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;

class SGAL_SGAL_DECL Shader : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    URL,
    LAST
  };

  /*! Constructor */
  Shader(Boolean proto = false);

  /*! Destructor */
  virtual ~Shader();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  std::string* url_handle(Field_info*) { return &m_url; }
  //@}

  /*! Set the attributes of the shader extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Apply the shader. */
  virtual void draw(Context* context);

  /*! Determine whether the object is dirty, and thus needs cleaning. */
  Boolean is_dirty() const;

  /*! Clean the object. */
  virtual void clean();

  /*! Determines whether the shader is empty. */
  virtual Boolean empty() = 0;

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string& get_url() const;

protected:
  /*! The uniform resource locator (url) that refers to the (Internet)
   * resource that contains the shader code.
   */
  std::string m_url;

  /*! Determine whether the shader is dirty, and thus needs cleaning. */
  Boolean m_dirty;

  // Default values:

private:
  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief obtains the URL. */
inline const std::string& Shader::get_url() const { return m_url; }

SGAL_END_NAMESPACE

#endif
