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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COORD_ARRAY_HPP
#define SGAL_COORD_ARRAY_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

/*! \class Coord_array Coord_array.hpp
 * Coord_array maintains an abstract array of vertex coordinates.
 */
class SGAL_SGAL_DECL Coord_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    LAST
  };

  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Coord_array(Boolean proto = false);

  /*! Destruct. */
  virtual ~Coord_array();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this container.
   * \param[in] elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the array size.
   * \return the array size.
   */
  virtual Size size() const = 0;

  /*! Determine whether the array is empty. */
  Boolean empty() const;

  /*! Resize the array capacity. */
  virtual void resize(Size n) = 0;

  /*! Clear the array. */
  virtual void clear() = 0;

  /*! Obtain the number of coordinate dimensions.
   * \return the number of coordinate dimensions.
   */
  virtual Size num_coordinates() const = 0;

  /*! Obtain the data size.
    \return the data size.
   */
  virtual Size data_size() const = 0;

  /*! Obtain the data.
   * \return the data.
   */
  virtual const GLfloat* data() const = 0;

  /*! Obtain the datum at a given index.
   * \param[in] i the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const = 0;

  /*! Process change of points.
   * \param[in] field_info
   */
  void point_changed(const Field_info* field_info);

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;
};

//! \brief constructs.
inline Coord_array::Coord_array(Boolean proto) : Container(proto) {}

//! \brief destructs.
inline Coord_array::~Coord_array() {}

//! \brief determines whether the array is empty.
inline Boolean Coord_array::empty() const {return (size() == 0); }

SGAL_END_NAMESPACE

#endif
