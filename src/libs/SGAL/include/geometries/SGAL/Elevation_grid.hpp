// Copyright (c) 2014 Israel.
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

#ifndef SGAL_ELEVATION_GRID_HPP
#define SGAL_ELEVATION_GRID_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

/*! class Elevation_grid Elevation_grid.hpp
 * The ElevationGrid node specifies a uniform rectangular grid of varying
 * height in the XZ plane of the local coordinate system. The geometry is
 * described by a scalar array of height values that specify the height of
 * a rectangular surface above each point of the grid.
 */
class SGAL_SGAL_DECL Elevation_grid : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    HEIGHT,
    X_DIMENSION,
    X_SPACING,
    Z_DIMENSION,
    Z_SPACING,
    LAST
  };

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Elevation_grid(Boolean proto = false);

  /*! Destructor */
  virtual ~Elevation_grid();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Elevation_grid* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Float_array* height_handle(const Field_info*) { return &m_height; }
  Uint* x_dimension_handle(const Field_info*) { return &m_x_dimension; }
  Float* x_spacing_handle(const Field_info*) { return &m_x_spacing; }
  Uint* z_dimension_handle(const Field_info*) { return &m_z_dimension; }
  Float* z_spacing_handle(const Field_info*) { return &m_z_spacing; }
  //@}

  /*! Set the attributes of the object.
   * \param elem (in) contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  /*! Clean the representation.
   */
  virtual void clean();

  /*! Calculate the default 2D texture-mapping oordinates.
   */
  virtual void clean_tex_coords_2d();

  /*! Process change of structure.
   */
  void structure_changed(const Field_info* field_info);

  /*! Set the 2D array that represents the height above a grid.
   * \param height (in) the new height field.
   */
  void set_height(Float_array& height);

  /*! Obtain the 2D array that represents the height above a grid.
   * \return the 2D array.
   */
  const Float_array& get_height() const;

  /*! Set the number of grid points along the x-dimension.
   * \param x_dimension (in) the number of grid points along the x-dimension.
   */
  void set_x_dimension(Uint x_dimension);

  /*! Obtain the number of grid points along the x-dimension.
   * \return the number of grid points along the x-dimension.
   */
  Uint get_x_dimension() const;

  /*! Set the distance between two successive grid points along the
   * x-dimension.
   * \param x_spacing (in) the distance.
   */
  void set_x_spacing(Float x_spacing);

  /*! Obtain the distance between two successive grid points along the
   * x-dimension.
   * \return the distance.
   */
  Float get_x_spacing() const;

  /*! Set the number of grid points along the z-dimension.
   * \param z_dimension (in) the number of grid points along the z-dimension.
   */
  void set_z_dimension(Uint z_dimension);

  /*! Obtain the number of grid points along the z-dimension.
   * \return the number of grid points along the z-dimension.
   */
  Uint get_z_dimension() const;

  /*! Set the distance between two successive grid points along the
   * z-dimension.
   * \param z_spacing (in) the distance.
   */
  void set_z_spacing(Float z_spacing);

  /*! Obtain the distance between two successive grid points along the
   * z-dimension.
   * \return the distance.
   */
  Float get_z_spacing() const;

protected:
  /*! A 2D array of scalar values representing the height above a grid. */
  Float_array m_height;

  /*! The number of grid points along the x-dimension. */
  Uint m_x_dimension;

  /*! The distance between two successive grid points along the x-dimension. */
  Float m_x_spacing;

  /*! The number of grid points along the z-dimension. */
  Uint m_z_dimension;

  /*! The distance between two successive grid points along the z-dimension. */
  Float m_z_spacing;

  /*! Generate the coordinate indices.
   */
  void generate_coord_indices();

  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  // default values
  static const Uint s_def_x_dimension;
  static const Float s_def_x_spacing;
  static const Uint s_def_z_dimension;
  static const Float s_def_z_spacing;
};

//! \brief constructs the prototype.
inline Elevation_grid* Elevation_grid::prototype()
{ return new Elevation_grid(true); }

//! \brief clones.
inline Container* Elevation_grid::clone() { return new Elevation_grid(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Elevation_grid::get_tag() const { return s_tag; }

//! \brief obtain the 2D array that represents the height above a grid.
inline const Float_array& Elevation_grid::get_height() const
{ return m_height; }

//! \brief obtains the number of grid points along the x-dimension.
inline Uint Elevation_grid::get_x_dimension() const { return m_x_dimension; }

//! \brief obtain the distance between two successive grid points along the
inline Float Elevation_grid::get_x_spacing() const { return m_x_spacing; }

//! \brief obtains the number of grid points along the z-dimension.
inline Uint Elevation_grid::get_z_dimension() const { return m_z_dimension; }

//! \brief obtains the distance between two successive grid points along the
inline Float Elevation_grid::get_z_spacing() const { return m_z_spacing; }

SGAL_END_NAMESPACE

#endif
