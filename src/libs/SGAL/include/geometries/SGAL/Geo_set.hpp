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

#ifndef SGAL_GEO_SET_HPP
#define SGAL_GEO_SET_HPP

/*! \file
 * A base class for mesh objects.
 *
 *  Inherits from Geometry.
 */

#include <sys/types.h>
#include <vector>
#include <boost/shared_ptr.hpp>

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Bounding_box.hpp"

SGAL_BEGIN_NAMESPACE

class Coord_array;
class Normal_array;
class Container_proto;
class Element;
class Formatter;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Geo_set describes geometry using a set of arrays that represent the
 * coordinates, normals, colors, and texture-coordinates of the vertices
 * of the geometry, and corresponding index arrays into the former resp.
 */
class SGAL_SGAL_DECL Geo_set : public Geometry {
public:
  /*! Fields */
  enum {
    FIRST = Geometry::LAST-1,
    NUM_PRIMITIVES,
    NORMAL_ATTACHMENT,
    COLOR_ATTACHMENT,
    COORD_ARRAY,
    NORMAL_ARRAY,
    COLOR_ARRAY,
    TEX_COORD_ARRAY,
    COORD_INDEX_ARRAY,
    NORMAL_INDEX_ARRAY,
    COLOR_INDEX_ARRAY,
    TEX_COORD_INDEX_ARRAY,
    LAST
  };

  enum Attachment {
    AT_NONE = 0x0,
    AT_PER_VERTEX = 0x1,
    AT_PER_PRIMITIVE = 0x2,
    AT_PER_MESH = 0x3
  };

  enum Primitive_type {
    PT_TRIANGLE_STRIP = 0x0,
    PT_TRIANGLE_FAN = 0x1,
    PT_TRIANGLES = 0x2,
    PT_QUAD_STRIP = 0x3,
    PT_QUADS = 0x4,
    PT_POLYGONS = 0x5,
    PT_LINES = 0x6,
    PT_LINE_STRIPS = 0x7,
    PT_LINE_LOOPS = 0x8,
    NUM_PRIMITIVE_TYPES = 9
  };

  enum Fragment_source {
    FS_NORMAL = 0x0,
    FS_COLOR = 0x1
  };

  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;
  typedef boost::shared_ptr<Normal_array>               Shared_normal_array;
  typedef boost::shared_ptr<Color_array>                Shared_color_array;
  typedef boost::shared_ptr<Tex_coord_array>            Shared_tex_coord_array;

  /*! Construct. */
  Geo_set(Boolean proto = false);

  /*! Destruct. */
  virtual ~Geo_set();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name Field Handlers
  //@{
  Shared_coord_array* coord_array_handle(const Field_info*)
  { return &m_coord_array; }
  Shared_normal_array* normal_array_handle(const Field_info*)
  { return &m_normal_array; }
  Shared_color_array* color_array_handle(const Field_info*)
  { return &m_color_array; }
  Shared_tex_coord_array* tex_coord_array_handle(const Field_info*)
  { return &m_tex_coord_array; }
  Int32_array* coord_indices_handle(const Field_info*)
  { return &m_coord_indices; }
  Int32_array* normal_indices_handle(const Field_info*)
  { return &m_normal_indices; }
  Int32_array* color_indices_handle(const Field_info*)
  { return &m_color_indices; }
  Int32_array* tex_coord_indices_handle(const Field_info*)
  { return &m_tex_coord_indices; }
  Attachment* color_attachment_handle(const Field_info*)
  { return &m_color_attachment; }
  Attachment* normal_attachment_handle(const Field_info*)
  { return &m_normal_attachment; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param[in] elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  //! \name Array Setters and Getters
  // A setter calls a virtual function, so it doesn't need to be virtual as well.
  //@{
  /*! Set the coordinate array.
   * \param[in] coord_array a pointer to a coordinate array
   */
  void set_coord_array(Shared_coord_array coord_array);

  /*! Set the normal array.
   * \param[in] normal_array the normal array.
   */
  void set_normal_array(Shared_normal_array normal_array);

  /*! Set the color field.
   * \param[in] color_array a pointer to a color array
   */
  void set_color_array(Shared_color_array color_array);

  /*! Set the texture-coordinate array.
   * \param[in] tex_coord_array the texture coordinate array.
   */
  void set_tex_coord_array(Shared_tex_coord_array tex_coord_array);

  /*! Obtain the coordinate array.
   * \return the coordinate array.
   */
  virtual Shared_coord_array get_coord_array();

  /*! Obtain the normal array.
   * \return the normal array.
   */
  virtual Shared_normal_array get_normal_array();

  /*! Obtain the color array.
   * \return the color array.
   */
  virtual Shared_color_array get_color_array();

  /*! Obtain the texture-coordinate array.
   * \return the texture-coordinate array.
   */
  virtual Shared_tex_coord_array get_tex_coord_array();
  //@}

  /// \name Index-Array Setters and Getters
  // A setter calls a virtual function, so it doesn't need to be virtual as well.
  //@{
  /*! Set the coordinate-index array.
   * \param[in] indices
   */
  void set_coord_indices(const Int32_array& indices);

  /*! Set the normal-index array.
   * \param[in] indices
   */
  void set_normal_indices(const Int32_array& indices);

  /*! Set the color-index array.
   * \param[in] indices
   */
  void set_color_indices(const Int32_array& indices);

  /*! Set the texture_coordinate-index array.
   * \param[in] indices
   */
  void set_tex_coord_indices(const Int32_array& indices);

  /*! Obtain the coordinate-index array.
   * \return the coordinate-index array.
   */
  virtual Int32_array& get_coord_indices();

  /*! Obtain the normal-index array.
   * \return the normal-index array.
   */
  virtual Int32_array& get_normal_indices();

  /*! Obtain the color-index array.
   * \return the color-index array.
   */
  virtual Int32_array& get_color_indices();

  /*! Obtain the texture-coordinate-index array.
   * \return the texture-coordinate-index array.
   */
  virtual Int32_array& get_tex_coord_indices();
  //@}

  /// \name Index-Array Vector Getters
  //@{
  /*! Obtain the (const) coordinate-index vector.
   * \return the coordinate-index array.
   */
  const Int32* get_coord_indices_vector() const;

  /*! Obtain the (non-const) coordinate-index vector.
   * \return the coordinate-index array.
   */
  Int32* get_coord_indices_vector();

  /*! Obtain the (const) color-index vector.
   * \return the color-index array.
   */
  const Int32* get_color_indices_vector() const;

  /*! Obtain the (non-const) color-index vector.
   * \return the color-index array.
   */
  Int32* get_color_indices_vector();

  /*! Obtain the (const) normal-index vector.
   * \return the normal-index array.
   */
  const Int32* get_normal_indices_vector() const;

  /*! Obtain the (non-const) normal-index vector.
   * \return the normal-index array.
   */
  Int32* get_normal_indices_vector();

  /*! Obtain the (const) texture-coordinate-index vector.
   * \return the texture-coordinate-index array.
   */
  const Int32* get_tex_coord_indices_vector() const;

  /*! Obtain the (non-const) texture-coordinate-index vector.
   * \return the texture-coordinate-index array.
   */
  Int32* get_tex_coord_indices_vector();

  /*! Obtain the ith coord index.
   * \return the ith coord index.
   */
  Int32 get_coord_index(Uint i) const;

  /*! Obtain the ith normal index.
   * \return the ith normal index.
   */
  Int32 get_normal_index(Uint i) const;

  /*! Obtain the ith color index.
   * \return the ith color index.
   */
  Int32 get_color_index(Uint i) const;

  /*! Obtain the ith tex-coord index.
   * \return the ith tex-coord index.
   */
  Int32 get_tex_coord_index(Uint i) const;
  //@}

  /// \name Change Recators
  //@{
  /*! Respond to a change in the coordinate array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_content_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the normal array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void normal_content_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the color array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void color_content_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the texture coordinate array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void tex_coord_content_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the coordinate-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the normal-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void normal_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the color-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void color_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the texture-coordinate index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void tex_coord_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the sphere bound.
   *  (i) Indicate the change in the current node (through a call to the
   *      corresponding member of the base class).
   * (ii) Indicate the change in the parent nodes (through a call to
   *      field_changed(), which in turn applies the observers).
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void bounding_sphere_changed(const Field_info* field_info = nullptr);
  //@}

  /*! Calculate the sphere bound.
   * The implementation assumes that m_coord_array points at an object of
   * type Coord_array_3d. If this is not the case, this virtual member
   * function must be overriden!
   */
  virtual void clean_bounding_sphere();

  /*! Determine whether the geometry has color.
   */
  virtual Boolean has_color();

  /*! Determine whether the geometry has texture coordinates.
   */
  virtual Boolean has_tex_coord();

  /*! Obtain the bounding box.
   */
  virtual Bounding_box bounding_box();

  /*! Determine whether the coordinate indices container is empty.
   */
  Boolean is_coord_indices_empty() const;

  /*! Obtain the ith 3D coordinate.
   */
  virtual const Vector3f& get_coord_3d(Uint i) const;

  /*! Obtain a pointer to an element directly. */
  template <typename Array_T>
  const GLfloat* get(Array_T array, Uint i) const;

  /*! Obtain a pointer to an element through the coord indices. */
  template <typename Array_T>
  const GLfloat* get_by_coord_index(Array_T array, Uint i) const;

  /*! Obtain apointer to an element through the normal indices. */
  template <typename Array_T>
  const GLfloat* get_by_normal_index(Array_T array, Uint i) const;

  /*! Obtain a pointer to an element through the color indices. */
  template <typename Array_T>
  const GLfloat* get_by_color_index(Array_T array, Uint i) const;

  /*! Obtain a pointer to an element through the tex. coord indices. */
  template <typename Array_T>
  const GLfloat* get_by_tex_coord_index(Array_T array, Uint i) const;

  /*! Set the representation mode. */
  void set_primitive_type(Primitive_type type);

  /*! Obtain the representation mode. */
  virtual Primitive_type get_primitive_type();

  /*! Set the number of primitives. */
  void set_num_primitives(Size num);

  /*! Obtain the number of primitives. */
  virtual Size get_num_primitives();

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action) = 0;

  /*! Process change of coordinate field.
   * Generally process the change of coordinate field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void coord_changed(const Field_info* field_info);

  /*! Process change of normal field.
   * Generally process the change of normal field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void normal_changed(const Field_info* field_info);

  /*! Process change of color field.
   * Generally process the change of color field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void color_changed(const Field_info* field_info);

  /*! Process change of texture coordinate field.
   * Generally process the change of texture coordinate field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void tex_coord_changed(const Field_info* field_info);

  /*! Process change of field.
   */
  virtual void field_changed(const Field_info* field_info);

  /*! Assign the coord indices with the reverse of given indices.
   * \param[in] indices the indices to reverse.
   */
  void reverse_coord_indices(const Int32_array& indices);

  /*! Resolve the conflict between normal and colors.
   */
  Fragment_source resolve_fragment_source() const;

  /*! Set the normal attachment method.
   */
  void set_normal_attachment(Attachment attachment);

  /*! Obtain the normal attachment method.
   */
  Attachment get_normal_attachment() const;

  /*! Set the color attachment method.
   */
  void set_color_attachment(Attachment attachment);

  /*! Obtain the color attachment method.
   */
  Attachment get_color_attachment() const;

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   * We overide this virtual function in order to export the color and normal
   * attachments properly.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

protected:
  /*! Obtain the attachment name of an attachment given its code.
   * \param[in] attachment the attachment code.
   */
  static const char* attachment_name(Attachment attachment);

  //! The number of primitives in this Geo_set.
  Size m_num_primitives;

  /*! Indicates how normals are bound.
   * By default, normals are bound per primitive (e.g., polygon, triangle,
   * and auad). Onserve that this is different than the specification of VRML,
   * where normalPerVertex is TRUE by default. This is rectified in
   * Indexed_face_set, where colorPerVertex, normalPerVertex, and creaseAngle
   * are introduced.
   */
  Attachment m_normal_attachment;

  /*! Indicates how normals are bound.
   * By default, colors are bound per primitive (e.g., polygon, triangle,
   * and auad). Onserve that this is different than the specification of VRML,
   * where colorPerVertex is TRUE by default. This is rectified in
   * Indexed_face_set, where colorPerVertex, normalPerVertex, and creaseAngle
   * are introduced.
   */
  Attachment m_color_attachment;

  //! An array of vertex ccordinates.
  Shared_coord_array m_coord_array;

  //! An array of normals.
  Shared_normal_array m_normal_array;

  //! An array of vertex colors.
  Shared_color_array m_color_array;

  //! An array of vertex texture-ccordinates.
  Shared_tex_coord_array m_tex_coord_array;

  //! An array of indices into the vertex-coordinate array.
  Int32_array m_coord_indices;

  //! An array of indices into the vertex-texture-coordinate array.
  Int32_array m_normal_indices;

  //! An array of indices into the vertex-color array.
  Int32_array m_color_indices;

  //! An array of indices into the vertex-normal array.
  Int32_array m_tex_coord_indices;

  //! Indicates the geometry mode (triangles, quads, or general polygons).
  Primitive_type m_primitive_type;

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The name of the representation modes. */
  static const Char* s_primitive_type_names[];

  /*! The name of the attachments. */
  static const Char* s_attachment_names[];

  // Default values
  static const Primitive_type s_def_primitive_type;
  static const Attachment s_def_normal_attachment;
  static const Attachment s_def_color_attachment;

  /*! Intialize. */
  void init();
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the attachment name of an attachment given its code.
inline const char* Geo_set::attachment_name(Attachment attachment)
{ return s_attachment_names[attachment]; }

//! \brief determines whether colors are generated by the geometry.
inline Boolean Geo_set::has_color()
{ return (m_color_array && (m_color_array->size() != 0)); }

//! \brief determines whether the geometry has texture coordinates.
inline Boolean Geo_set::has_tex_coord()
{ return (m_tex_coord_array && (m_tex_coord_array->size() != 0)); }

//! \brief obtains the coordinate-index array.
inline Int32_array& Geo_set::get_coord_indices()
{ return m_coord_indices; }

//! \brief determines whether the coordinate indices container is empty.
inline Boolean Geo_set::is_coord_indices_empty() const
{ return m_coord_indices.empty(); }

//! \brief obtains the normal-index array.
inline Int32_array& Geo_set::get_normal_indices()
{ return m_normal_indices; }

//! \brief obtains the color-index array.
inline Int32_array& Geo_set::get_color_indices()
{ return m_color_indices; }

//! \brief obtains the texture-coordinate-index array.
inline Int32_array& Geo_set::get_tex_coord_indices()
{ return m_tex_coord_indices; }

//! \brief obtains the (const) coordinate-index vector.
inline const Int32* Geo_set::get_coord_indices_vector() const
{ return &(*(m_coord_indices.begin())); }

//! \brief obtains the (non-const) coordinate-index vector.
inline Int32* Geo_set::get_coord_indices_vector()
{ return &(*(m_coord_indices.begin())); }

//! \brief obtains the (const) color-index vector.
inline const Int32* Geo_set::get_color_indices_vector() const
{ return &(*(m_color_indices.begin())); }

//! \brief obtains the (non-const) color-index vector.
inline Int32* Geo_set::get_color_indices_vector()
{ return &(*(m_color_indices.begin())); }

//! \brief obtains the (const) normal-index vector.
inline const Int32* Geo_set::get_normal_indices_vector() const
{ return &(*(m_normal_indices.begin())); }

//! \brief obtains the (non-const) normal-index vector.
inline Int32* Geo_set::get_normal_indices_vector()
  { return &(*(m_normal_indices.begin())); }

//! \brief obtains the (const) texture-coordinate-index vector.
inline const Int32* Geo_set::get_tex_coord_indices_vector() const
{ return &(*(m_tex_coord_indices.begin())); }

//! \brief obtains the (non-const) texture-coordinate-index vector.
inline Int32* Geo_set::get_tex_coord_indices_vector()
{ return &(*(m_tex_coord_indices.begin())); }

//! \brief Obtain the i-th coord index.
inline Int32 Geo_set::get_coord_index(Uint i) const
{ return m_coord_indices[i]; }

//! \brief obtains the ith normal index.
inline Int32 Geo_set::get_normal_index(Uint i) const
{ return m_normal_indices[i]; }

//! \brief obtains the ith color index.
inline Int32 Geo_set::get_color_index(Uint i) const
{ return m_color_indices[i]; }

//! \brief obtains the ith tex-coord index.
inline Int32 Geo_set::get_tex_coord_index(Uint i) const
{ return m_tex_coord_indices[i]; }

/*! \brief resolvess the conflict between normal and colors. If the color
 * array is present, we use colors as source (disable lighting, etc.).
 */
inline Geo_set::Fragment_source Geo_set::resolve_fragment_source() const
{ return (m_color_array && m_color_array->size()) ? FS_COLOR : FS_NORMAL; }

//! \brief obtains the coordinate array.
inline Geo_set::Shared_coord_array Geo_set::get_coord_array()
{ return m_coord_array; }

//! \brief obtains the normal array.
inline Geo_set::Shared_normal_array Geo_set::get_normal_array()
{ return m_normal_array; }

//! \brief obtains the texture-coordinate array.
inline Geo_set::Shared_tex_coord_array Geo_set::get_tex_coord_array()
{ return m_tex_coord_array; }

//! \brief obtains the color array.
inline Geo_set::Shared_color_array Geo_set::get_color_array()
{ return m_color_array; }

//! \brief obtains a pointer to an element directly.
template <typename Array_T>
inline const GLfloat* Geo_set::get(Array_T array, Uint i) const
{ return array->datum(i); }

//! \brief obtains a pointer to an element through the coord indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_coord_index(Array_T array, Uint i) const
{ return array->datum(m_coord_indices[i]); }

//! \brief obtains a pointer to an element through the normal indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_normal_index(Array_T array, Uint i) const
{ return array->datum(m_normal_indices[i]); }

//! \brief obtains a pointer to an element through the color indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_color_index(Array_T array, Uint i) const
{ return array->datum(m_color_indices[i]); }

//! \brief obtains a pointer to an element through the tex. coord indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_tex_coord_index(Array_T array, Uint i)
  const
{ return array->datum(m_tex_coord_indices[i]); }

//! \brief sets the representation mode. */
inline void Geo_set::set_primitive_type(Primitive_type type)
{ m_primitive_type = type; }

//! \brief sets the number of primitives.
inline void Geo_set::set_num_primitives(Size num) { m_num_primitives = num; }

//! \brief obtains the normal attachment method.
inline Geo_set::Attachment Geo_set::get_normal_attachment() const
{ return m_normal_attachment; }

//! \brief obtains the color attachment method.
inline Geo_set::Attachment Geo_set::get_color_attachment() const
{ return m_color_attachment; }

SGAL_END_NAMESPACE

#endif
