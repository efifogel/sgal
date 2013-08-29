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

class SGAL_SGAL_DECL Composed_shader : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    LANGUAGE,
    LAST
  };

  /*! Constructor */
  Composed_shader(Boolean proto = false);

  /*! Destructor */
  virtual ~Composed_shader();

  /*! Construct the prototype. */
  static Composed_shader* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  std::string* language_handle(Field_info*) { return &m_language; }
  //@}

  /*! Set the attributes of the shader extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Apply the shader. */
  virtual void draw(Context* context);

  /*! Clean the shader. */
  virtual void clean();

  // /*! Determines whether the shader is empty. */
  // virtual Boolean empty();

  // /*! Determine whether the object is dirty, and thus needs cleaning. */
  // virtual Boolean is_dirty() const;

  /*! Set the language. */
  void set_language(const std::string& language);

  /*! Obtain the language. */
  const std::string get_language() const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

  // Default values:

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The uniform resource locator (url) that refers to the (Internet)
   * resource that contains the shader language.
   */
  std::string m_language;

  /*! Determine whether the shader is dirty, and thus needs cleaning. */
  Boolean m_dirty;
};

/*! \brief constructs the prototype. */
inline Composed_shader* Composed_shader::prototype()
{ return new Composed_shader(true); }

/*! \brief clones. */
inline Container* Composed_shader::clone() { return new Composed_shader(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Composed_shader::get_tag() const { return s_tag; }

/*! \brief obtains the language. */
inline const std::string Composed_shader::get_language() const
{ return m_language; }

SGAL_END_NAMESPACE

#endif
