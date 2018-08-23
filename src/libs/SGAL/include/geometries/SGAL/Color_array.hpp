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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COLOR_ARRAY_HPP
#define SGAL_COLOR_ARRAY_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Color_array Color_array.hpp
 * Coord_array maintains an abstract array of 3D vertex-colors.
 */
class SGAL_SGAL_DECL Color_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    COLOR,
    LAST
  };

  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Color_array(Boolean proto = false);

  /*! Destruct. */
  virtual ~Color_array();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype.
   */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the array size.
   * \return the array size.
   */
  virtual Size size() const = 0;

  /*! Determine whether the array is empty.
   */
  virtual Boolean empty() const;

  /*! Resize the array capacity.
   */
  virtual void resize(Size n) = 0;

  /*! Clear the array.
   */
  virtual void clear() = 0;

  /*! Obtain the number of coordinate dimensions.
   * \return the number of coordinate dimensions.
   */
  virtual Size num_coordinates() const = 0;

  /*! Obtain the data size.
   * \return the data size.
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

  /*! Process change of colors.
   * \param[in] field_info
   */
  void color_changed(const Field_info* field_info);

private:
  //! The node prototype.
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructor.
inline Color_array::Color_array(Boolean proto) : Container(proto) {}

//! \brief destructor.
inline Color_array::~Color_array() {}

//! \brief determines whether the array is empty.
inline Boolean Color_array::empty() const {return (size() == 0); }

SGAL_END_NAMESPACE

#endif
