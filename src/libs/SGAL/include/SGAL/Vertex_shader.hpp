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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VERTEX_SHADER_HPP
#define SGAL_VERTEX_SHADER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Shader.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Vertex_shader : public Shader {
public:
  enum {
    FIRST = Shader::LAST - 1,
    LAST
  };

  /*! Constructor */
  Vertex_shader(Boolean proto = false);

  /*! Destructor */
  virtual ~Vertex_shader();

  /*! Construct the prototype. */
  static Vertex_shader* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of the shader extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Apply the shader. */
  virtual void draw(Context* ctx);

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  /*! Determines whether the shader is empty. */
  virtual Boolean empty();

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Vertex_shader* Vertex_shader::prototype()
{ return new Vertex_shader(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Vertex_shader::create() { return new Vertex_shader(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Vertex_shader::get_tag() const { return s_tag; }

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
