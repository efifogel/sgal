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
 * Converts an image to a elevation_grid
 */

#ifndef SGAL_IMAGE_TO_ELEVATION_GRID_HPP
#define SGAL_IMAGE_TO_ELEVATION_GRID_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Field_info;
class Elevation_grid;

class SGAL_SGAL_DECL Image_to_elevation_grid : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    TRIGGER,
    IMAGE,
    ELEVATION_GRID,
    LAST
  };

  typedef boost::shared_ptr<Elevation_grid>     Shared_elevation_grid;
  typedef boost::shared_ptr<Image>              Shared_image;

  /*! Constructor. */
  Image_to_elevation_grid(Boolean proto = false);

  /*! Destructor. */
  virtual ~Image_to_elevation_grid();

  /*! Construct the prototype. */
  static Image_to_elevation_grid* prototype();

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
  Shared_elevation_grid* elevation_grid_handle(const Field_info*)
  { return &m_elevation_grid; }
  Shared_image* image_handle(const Field_info*) { return &m_image; }
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! The interpolation execution function.
   */
  virtual void execute(const Field_info* field_info);

  /*! Process change of elevation grid. */
  void elevation_grid_changed(const Field_info* field_info);

  /*! Obtain the texture image. */
  Shared_image get_image() const;

  /*! Set the texture image. */
  void set_image(Shared_image image);

  /*! Set the 2D array that represents the elevation_grid above a grid.
   * \param elevation_grid (in) the new elevation_grid field.
   */
  void set_elevation_grid(Shared_elevation_grid elevation_grid);

  /*! Obtain the 2D array that represents the elevation_grid above a grid.
   * \return the 2D array.
   */
  Shared_elevation_grid get_elevation_grid() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The texture pixels. */
  Shared_image m_image;

  /*! A 2D array of scalar values representing the elevation_grid above a grid.
   */
  Shared_elevation_grid m_elevation_grid;

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
inline Image_to_elevation_grid* Image_to_elevation_grid::prototype()
{ return new Image_to_elevation_grid(true); }

//! \brief clones.
inline Container* Image_to_elevation_grid::clone()
{ return new Image_to_elevation_grid(); }

//! \brief obtains the texture image.
inline Image_to_elevation_grid::Shared_image
Image_to_elevation_grid::get_image() const { return m_image; }

//! \brief sets the texture image.
inline  void Image_to_elevation_grid::set_image(Shared_image image)
{ m_image = image; }

//! \brief obtain the 2D array that represents the height above a grid.
inline Image_to_elevation_grid::Shared_elevation_grid
Image_to_elevation_grid::get_elevation_grid() const
{ return m_elevation_grid; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_to_elevation_grid::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
