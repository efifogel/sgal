// Copyright (c) 2015 Israel.
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

/*! \file
 *
 * Converts an image to a height map.
 */

#ifndef SGAL_IMAGE_TO_HEIGHT_MAP_HPP
#define SGAL_IMAGE_TO_HEIGHT_MAP_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Field_info;
class Coord_array_1d;
class Formatter;

class SGAL_SGAL_DECL Image_to_height_map : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    TRIGGER,
    IMAGE,
    HEIGHT_MAP,
    X_DIMENSION,
    Z_DIMENSION,
    LAST
  };

  typedef boost::shared_ptr<Coord_array_1d>     Shared_coord_array_1d;
  typedef boost::shared_ptr<Image>              Shared_image;

  /*! Constructor. */
  Image_to_height_map(Boolean proto = false);

  /*! Destructor. */
  virtual ~Image_to_height_map();

  /*! Construct the prototype. */
  static Image_to_height_map* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Shared_image* image_handle(const Field_info*) { return &m_image; }
  Shared_coord_array_1d* height_map_handle(const Field_info*)
  { return &m_height_map; }
  Uint* x_dimension_handle(const Field_info*) { return &m_x_dimension; }
  Uint* z_dimension_handle(const Field_info*) { return &m_z_dimension; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! The interpolation execution function.
   */
  virtual void execute(const Field_info* field_info);

  /*! Obtain the texture image. */
  Shared_image get_image() const;

  /*! Set the texture image. */
  void set_image(Shared_image image);

  /*! Set the 2D array that represents the height map.
   * \param[in] height_map the new height_map field.
   */
  void set_height_map(Shared_coord_array_1d height_map);

  /*! Obtain the 2D array that represents the height map.
   * \return the 2D array.
   */
  Shared_coord_array_1d get_height_map() const;

  /*! Set the number of grid points along the x-dimension.
   * \param[in] x_dimension the number of grid points along the x-dimension.
   */
  void set_x_dimension(Uint x_dimension);

  /*! Set the number of grid points along the z-dimension.
   * \param[in] z_dimension the number of grid points along the z-dimension.
   */
  void set_z_dimension(Uint z_dimension);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The texture pixels. */
  Shared_image m_image;

  /*! A 2D array of scalar values representing the height map.
   */
  Shared_coord_array_1d m_height_map;

  /*! The number of grid points along the x-dimension. */
  Uint m_x_dimension;

  /*! The number of grid points along the z-dimension. */
  Uint m_z_dimension;

  /*! Trigger of the engine that makes the engine excute. */
  Boolean m_trigger;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  // Default values:
};

//! \brief constructs the prototype.
inline Image_to_height_map* Image_to_height_map::prototype()
{ return new Image_to_height_map(true); }

//! \brief clones.
inline Container* Image_to_height_map::clone()
{ return new Image_to_height_map(); }

//! \brief obtains the texture image.
inline Image_to_height_map::Shared_image
Image_to_height_map::get_image() const { return m_image; }

//! \brief sets the texture image.
inline  void Image_to_height_map::set_image(Shared_image image)
{ m_image = image; }

//! \brief obtain the 2D array that represents the height map.
inline Image_to_height_map::Shared_coord_array_1d
Image_to_height_map::get_height_map() const { return m_height_map; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_to_height_map::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
