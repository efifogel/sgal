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

#ifndef SGAL_EXTRUSION_HPP
#define SGAL_EXTRUSION_HPP

#if defined(_MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Extrusion Extrusion.hpp
 * Extrusion is a geometry container that specifies geometric shapes based on
 * a two dimensional cross section extruded along a three dimensional spine.
 * The cross section can be scaled and rotated at each spine point to produce
 * a wide variety of shapes.
 * If a cross section is not directly specified a radius must be specified
 * instead, and a disk shaped of the given radius is used as the cross section.
 */

class SGAL_SGAL_DECL Extrusion : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    BEGIN_CAP,
    END_CAP,
    CROSS_SECTION,
    ORIENTATION,
    SCALE,
    SPINE,
    LOOP,
    CROSS_SECTION_CLOSED,
    CROSS_SECTION_RADIUS,
    CROSS_SECTION_SLICES,
    LAST
  };

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Extrusion(Boolean proto = false);

  /*! Destructor */
  virtual ~Extrusion();

  /* Construct the prototype. */
  static Extrusion* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Set the flag that specifies whether the extruded shape is open at the
   * beginning.
   */
  void set_begin_cap(Boolean begin_cap);

  /*! Obtain the flag that specifies whether the extruded shape is open at the
   * beginning.
   */
  Boolean get_begin_cap() const;

  /*! Set the flag that specifies whether the extruded shape is open at the
   * end.
   */
  void set_end_cap(Boolean end_cap);

  /*! Obtain the flag that specifies whether the extruded shape is open at the
   * end.
   */
  Boolean get_end_cap() const;

  /*! Set the flag that indicates whether the spine is a closed loop.
   */
  void set_loop(Boolean loop);

  /*! Obtain the flag that indicates whether the spine is a closed loop.
   */
  Boolean is_loop() const;

  /*! Set the 2-D cross section of the final shape defined in the XZ plane. */
  void set_cross_section(std::vector<Vector2f>& cross_section);

  /*! Obtain the 2-D cross section of the final shape defined in the XZ plane.
   */
  const std::vector<Vector2f>& get_cross_section() const;

  /*! Set the orientation of the cross section. */
  void set_orientation(std::vector<Rotation>& orientation);

  /*! Obtain the orientation of the cross section. */
  const std::vector<Rotation>& get_orientation() const;

  /*! Set the scale of the cross section. */
  void set_scale(std::vector<Vector2f>& scale);

  /*! Obtain the scale of the cross section. */
  const std::vector<Vector2f>& get_scale() const;

  /*! Set the path that the cross section travels to create the shape. */
  void set_spine(std::vector<Vector3f>& spine);

  /*! Obtains the path that the cross section travels to create the shape. */
  const std::vector<Vector3f>& get_spine() const;

  /*! Obtains the path that the cross section travels to create the shape. */
  std::vector<Vector3f>& get_spine();

  /*! Set the flag that indicates whether the cross section is a closed loop.
   */
  void set_cross_section_closed(Boolean cross_section_closed);

  /*! Determine whether the cross section is a closed loop.
   */
  Boolean is_cross_section_closed() const;

  /*! Set the cross section radius. */
  void set_cross_section_radius(Float radius);

  /*! Obtain the cross section radius. */
  Float get_cross_section_radius() const;

  /*! Set the cross section slicess number. */
  void set_cross_section_slices(Uint slices);

  /*! Obtain the cross section slices number. */
  Uint get_cross_section_slices() const;

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
  // beginCap
  Boolean* begin_cap_handle(const Field_info*) { return &m_begin_cap; }
  Boolean* end_cap_handle(const Field_info*) { return &m_end_cap; }
  Boolean* loop_handle(const Field_info*) { return &m_loop; }
  std::vector<Vector2f>* cross_section_handle(const Field_info*)
  { return &m_cross_section; }
  std::vector<Rotation>* orientation_handle(const Field_info*)
  { return &m_orientation; }
  std::vector<Vector2f>* scale_handle(const Field_info*) { return &m_scale; }
  std::vector<Vector3f>* spine_handle(const Field_info*) { return &m_spine; }

  Boolean* cross_section_closed_handle(const Field_info*)
  { return &m_cross_section_closed; }
  Float* cross_section_radius_handle(const Field_info*)
  { return &m_cross_section_radius; }
  Uint* cross_section_slices_handle(const Field_info*)
  { return &m_cross_section_slices; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation.
   */
  virtual void clean();

  /*! Calculate the default 2D texture-mapping oordinates.
   */
  virtual void clean_tex_coords_2d();

  /*! Process change of structure.
   */
  void structure_changed(const Field_info* field_info);

protected:
  /*! Specifies whether the extruded shape is open at the beginning. */
  Boolean m_begin_cap;

  /*! Specifies whether the extruded shape is open at the end. */
  Boolean m_end_cap;

  /*! Indicates whether the spine is a loop. */
  Boolean m_loop;

  /*! A 2-D cross section of the final shape defined in the XZ plane. */
  std::vector<Vector2f> m_cross_section;

  /*! The orientation of the cross section. */
  std::vector<Rotation> m_orientation;

  /*! The scale of the cross section. */
  std::vector<Vector2f> m_scale;

  /*! Defines the path that the cross section travels to create the shape. */
  std::vector<Vector3f> m_spine;

  /*! obtains the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! Indicates whether the cross section is closed. */
  Boolean m_cross_section_closed;

  /*! The cross-section radius. */
  Float m_cross_section_radius;

  /*! The number of slices of the cross-section. */
  Uint m_cross_section_slices;

  /*! Generate the cross section (in case it is not provided).
   */
  void generate_cross_section();

  /*! Generate the coordinate indices.
   */
  void generate_coord_indices();

  /*! Generate the texture coordinate indices.
   */
  void generate_tex_coord_indices();

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! default values for Extrusion.  */
  static const Boolean s_def_begin_cap;
  static const Boolean s_def_end_cap;
  static const Boolean s_def_loop;
  static const Boolean s_def_cross_section_closed;
  static const Float s_def_cross_section_radius;
  static const Uint s_def_cross_section_slices;

  /*! Add triangle indices given four points that form a quad.
   * \param k (in) the index into the coordinate indices array.
   * \param indices (out) the array of indices.
   * \param ll (in) lower left corner.
   * \param lr (in) lower right corner.
   * \param ur (in) upper right corner.
   * \param ul (in) upper left corner.
   * \return the size of the coordinate indices array after the addition.
   */
  size_t add_triangle_indices(size_t k, std::vector<Uint>& indices,
                              Uint ll, Uint lr, Uint ur, Uint ul);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Extrusion* Extrusion::prototype() { return new Extrusion(true); }

//! \brief clones.
inline Container* Extrusion::clone() { return new Extrusion(); }

/*! \brief obtains the flag that specifies whether the extruded shape is
 * open at the beginning.
 */
inline Boolean Extrusion::get_begin_cap() const { return m_begin_cap; }

/*! \brief obtains the flag that specifies whether the extruded shape is
 * open at the end.
 */
inline Boolean Extrusion::get_end_cap() const { return m_end_cap; }

/*! \brief obtains the flag that specifies whether the spine is a closed
 * loop.
 */
inline Boolean Extrusion::is_loop() const { return m_loop; }

/*! \brief obtains the 2-D cross section of the final shape defined in the
 * XZ plane.
 */
inline const std::vector<Vector2f>& Extrusion::get_cross_section() const
{ return m_cross_section; }

//! \brief obtains the orientation of the cross section.
inline const std::vector<Rotation>& Extrusion::get_orientation() const
{ return m_orientation; }

//! \brief obtains the scale of the cross section.
inline const std::vector<Vector2f>& Extrusion::get_scale() const
{ return m_scale; }

/*! \brief obtains the path that the cross section travels to create the
 * shape.
 */
inline const std::vector<Vector3f>& Extrusion::get_spine() const
{ return m_spine; }

/*! \brief obtains the path that the cross section travels to create the
 * shape.
 */
inline std::vector<Vector3f>& Extrusion::get_spine() { return m_spine; }

//! \brief determine whether the cross section is a closed loop.
inline Boolean Extrusion::is_cross_section_closed() const
{ return m_cross_section_closed; }

//! \brief obtains the cross section radius.
inline Float Extrusion::get_cross_section_radius() const
{ return m_cross_section_radius; }

//! \brief obtains the cross section slices number.
inline Uint Extrusion::get_cross_section_slices() const
{ return m_cross_section_slices; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Extrusion::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
