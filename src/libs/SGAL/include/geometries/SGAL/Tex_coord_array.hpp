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
  Tex_coord_array(Boolean proto = false);

  /*! Destructor */
  virtual ~Tex_coord_array();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the container prototype.
   * \return the container prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  //@}

  /*! Sets the attributes of this container.
   * \param elem (in) contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the array size.
   * \return the array size.
   */
  virtual Uint size() const = 0;

  /*! Resize the array capacity. */
  virtual void resize(Uint n) = 0;

  /*! Clear the array. */
  virtual void clear() = 0;

  /*! Obtain the number of texture coordinate dimensions.
   * \return the number of texture coordinate dimensions.
   */
  virtual Uint num_coordinates() const = 0;

  /*! Obtain the data size.
   * \return the data size.
   */
  virtual Uint data_size() const = 0;

  /*! Obtain the data.
   * \return the data.
   */
  virtual const GLfloat* data() const = 0;

  /*! Obtain the datum at a given index.
   * \param i (in) the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const = 0;

private:  /*! The node prototype */
  static Container_proto* s_prototype;
};

//! \brief constructor.
inline Tex_coord_array::Tex_coord_array(Boolean proto) : Container(proto) {}

//! \brief destructor.
inline Tex_coord_array::~Tex_coord_array() {}

SGAL_END_NAMESPACE

#endif
