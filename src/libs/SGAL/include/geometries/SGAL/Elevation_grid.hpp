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
class Coord_array_1d;
class Formatter;
class Field_info;
class Formatter;

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
    HEIGHT_MAP,
    X_DIMENSION,
    X_SPACING,
    Z_DIMENSION,
    Z_SPACING,
    IS_CLOSED,
    BASE_HEIGHT,
    LAST
  };

  typedef boost::shared_ptr<Coord_array_1d>             Shared_coord_array_1d;

  /*! Constructor.
   * \para[in]m proto determines whether to construct a prototype.
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
  Shared_coord_array_1d* height_map_handle(const Field_info*)
  { return &m_height_map; }
  Uint* x_dimension_handle(const Field_info*) { return &m_x_dimension; }
  Float* x_spacing_handle(const Field_info*) { return &m_x_spacing; }
  Uint* z_dimension_handle(const Field_info*) { return &m_z_dimension; }
  Float* z_spacing_handle(const Field_info*) { return &m_z_spacing; }
  Boolean* is_closed_handle(const Field_info*) { return &m_is_closed; }
  Float* base_height_handle(const Field_info*) { return &m_base_height; }
  //@}

  /*! Set the attributes of the object.
   * \param[in] elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  /*! Clean the coordinates of the representation.
   */
  virtual void clean_coords();

  /*! Clean the facet coordinate indices.
   */
  virtual void clean_facet_coord_indices();

  /*! Calculate the default 2D texture-mapping oordinates.
   */
  virtual void clean_tex_coord_array_2d();

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Process change of field.
   */
  virtual void field_changed(const Field_info* field_info);

  /*! Process change of height.
   */
  void height_changed(const Field_info* field_info);

  /*! Process change of height map.
   */
  void height_map_changed(const Field_info* field_info);

  /*! Process change of structure.
   */
  void structure_changed(const Field_info* field_info);

  /*! Set the 2D array that represents the height above a grid.
   * \param[in] height the new height field.
   */
  void set_height(const Float_array& height);

  /*! Obtain the (const) 2D array that represents the height above a grid.
   * \return the 2D array.
   */
  Float_array& get_height();

  /*! Set the height map.
   * \param[in] height the new height map.
   */
  void set_height_map(Shared_coord_array_1d height_map);

  /*! Obtain the height map.
   * \return the height map.
   */
  Shared_coord_array_1d get_height_map();

  /*! Set the number of grid points along the x-dimension.
   * \param[in] x_dimension the number of grid points along the x-dimension.
   */
  void set_x_dimension(Uint x_dimension);

  /*! Obtain the number of grid points along the x-dimension.
   * \return the number of grid points along the x-dimension.
   */
  Uint get_x_dimension() const;

  /*! Set the distance between two successive grid points along the
   * x-dimension.
   * \param[in] x_spacing the distance.
   */
  void set_x_spacing(Float x_spacing);

  /*! Obtain the distance between two successive grid points along the
   * x-dimension.
   * \return the distance.
   */
  Float get_x_spacing() const;

  /*! Set the number of grid points along the z-dimension.
   * \param[in] z_dimension the number of grid points along the z-dimension.
   */
  void set_z_dimension(Uint z_dimension);

  /*! Obtain the number of grid points along the z-dimension.
   * \return the number of grid points along the z-dimension.
   */
  Uint get_z_dimension() const;

  /*! Set the distance between two successive grid points along the
   * z-dimension.
   * \param[in] z_spacing the distance.
   */
  void set_z_spacing(Float z_spacing);

  /*! Obtain the distance between two successive grid points along the
   * z-dimension.
   * \return the distance.
   */
  Float get_z_spacing() const;

  /*! Turn on the flag that indicates whether the shape should be closed. */
  void set_closed();

  /*! Turn off the flag that indicates whether the shape should be closed. */
  void set_open();

  /*! Set the flag that indicates whether the shape should be closed. */
  void set_closed(Boolean flag);

  /*! Determine whether the shape should be closed. */
  Boolean is_closed() const;

  /*! Set the height of the base in case the surface is closed.
   * \param[in] base_height the base height.
   */
  void set_base_height(Float base_height);

  /*! Obtain the height of the base in case the surface is closed.
   * \return the base height.
   */
  Float get_base_height() const;

protected:
  /*! A 2D array of scalar values representing the height above a grid. */
  Float_array m_height;

  /*! A 2D array of scalar values representing the height above a grid. */
  Shared_coord_array_1d m_height_map;

  /*! The number of grid points along the x-dimension. */
  Uint m_x_dimension;

  /*! The distance between two successive grid points along the x-dimension. */
  Float m_x_spacing;

  /*! The number of grid points along the z-dimension. */
  Uint m_z_dimension;

  /*! The distance between two successive grid points along the z-dimension. */
  Float m_z_spacing;

  /*! Indicates whether the 3D object is closed. */
  Boolean m_is_closed;

  /*! The base height in case the surface is closed. */
  Float m_base_height;

  /*! Indicates whether the 2D array representing the height above the grid is
   * dirty.
   */
  Boolean m_dirty_height;

  /*! Indicates whether the height map is dirty. */
  Boolean m_dirty_height_map;

  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

private:
  /*! Clean the 2D array representing the height above the grid.
   */
  void clean_height();

  /*! Clean the height map.
   */
  void clean_height_map();

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  // default values
  static const Uint s_def_x_dimension;
  static const Float s_def_x_spacing;
  static const Uint s_def_z_dimension;
  static const Float s_def_z_spacing;
  static const Boolean s_def_is_closed;
  static const Float s_def_base_height;
};

//! \brief constructs the prototype.
inline Elevation_grid* Elevation_grid::prototype()
{ return new Elevation_grid(true); }

//! \brief clones.
inline Container* Elevation_grid::clone() { return new Elevation_grid(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Elevation_grid::get_tag() const { return s_tag; }

//! \brief obtains the number of grid points along the x-dimension.
inline Uint Elevation_grid::get_x_dimension() const { return m_x_dimension; }

/*! \brief obtain the distance between two successive grid points along the
 * the x-dimension.
 */
inline Float Elevation_grid::get_x_spacing() const { return m_x_spacing; }

//! \brief obtains the number of grid points along the z-dimension.
inline Uint Elevation_grid::get_z_dimension() const { return m_z_dimension; }

/*! \brief obtains the distance between two successive grid points along the
 * the z-dimension
 */
inline Float Elevation_grid::get_z_spacing() const { return m_z_spacing; }

//! \brief determines whether the shape should be closed.
inline Boolean Elevation_grid::is_closed() const { return m_is_closed; }

//! \brief obtains the height of the base.
inline Float Elevation_grid::get_base_height() const { return m_base_height; }

SGAL_END_NAMESPACE

#endif
