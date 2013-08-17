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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TEX_COORD_ARRAY_HPP
#define SGAL_TEX_COORD_ARRAY_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

/*! This class maintains an array of vertex-texture-coordinates pairs */
class SGAL_SGAL_DECL Tex_coord_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    LAST
  };

  /*! Constructor */
  Tex_coord_array(Boolean proto = false) : Container(proto) {}

  /*! Destructor */
  virtual ~Tex_coord_array() {}

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Obtain the array size. */
  virtual Uint size() const = 0;

  /*! Resize the array capacity. */
  virtual void resize(Uint n) = 0;

  /*! Clear the array. */
  virtual void clear() = 0;

  /*! Obtain the GL data. */
  virtual GLfloat* get_gl_data() = 0;

private:  /*! The node prototype */
  static Container_proto* s_prototype;
};

SGAL_END_NAMESPACE

#endif
