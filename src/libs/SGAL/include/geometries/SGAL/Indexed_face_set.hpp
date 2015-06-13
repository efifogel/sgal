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

#ifndef SGAL_INDEXED_FACE_SET_HPP
#define SGAL_INDEXED_FACE_SET_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Boundary_set.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Polyhedron.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Draw_action;
class Isect_action;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Indexed_face_set Indexed_face_set.hpp
 * Boundary_set is a geometry node that represnts a mesh made of facets
 * with some additional attributes, efficient OpenGL rendering capabilities,
 * the generation of normals based on the angle neighboring facets, and a CGAL
 * polyhedral surface. The generation of normals utiles the polyhedral surface.
 */
class SGAL_SGAL_DECL Indexed_face_set : public Boundary_set {
public:
  enum {
    FIRST = Boundary_set::LAST - 1,
    VOLUME,
    SURFACE_AREA,
    LAST
  };

  typedef Inexact_kernel                               Kernel;
  typedef Inexact_polyhedron                           Polyhedron;

  /*! Constructor */
  Indexed_face_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Indexed_face_set();

  /* Construct the prototype. */
  static Indexed_face_set* prototype();

  /*! Clone. */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Float* volume_handle(const Field_info*) { return &m_volume; }
  Float* surface_area_handle(const Field_info*) { return &m_surface_area; }
  //@}

  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the polygons.
   */
  virtual void draw(Draw_action* action);

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
   virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Draw the polygons for selection.
   */
  virtual void isect(Isect_action* action);

  /*! Clean the sphere bound.
   */
  virtual void clean_sphere_bound();

  /*! Clean the coordinate array and coordinate indices.
   */
  virtual void clean_coords();

  /*! Obtain the coordinate array.
   * \return the coordinate array.
   */
  virtual Shared_coord_array get_coord_array();

  /*! Clear the coordinate array.
   */
  void clear_coord_array();

  /*! Determine whether the coordinate array has been invalidated, and thus
   * must be cleaned.
   */
  Boolean is_dirty_coord_array() const;

  /*! Clean the normal array and the normal indices.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  virtual void clean_normals();

  /*! Clean the polyhedron.
   */
  virtual void clean_polyhedron();

  /*! Clear the polyhedron.
   */
  void clear_polyhedron();

  /*! Determine whether the polyhedron has been invalidated, and thus been
   * must be cleaned.
   */
  Boolean is_dirty_polyhedron() const;

  /*! Set the polyhedron data-structure.
   * \param[in] polyhedron the polyhedron data-structure.
   */
  void set_polyhedron(Polyhedron& polyhedron);

  /*! Obtain the polyhedron data-structure.
   * \param[in] with_planes indicates whether to clean the planes (as well
   *            as the polyhedron itself.
   * \return the polyhedron data-structure.
   */
  const Polyhedron& get_polyhedron(Boolean with_planes = false);

  /*! Determine whether the polyhedron representation is empty.
   */
  bool is_polyhedron_empty() const;

  /// \name Change Recators
  //@{
  /*! Respond to a change in the coordinate array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void coord_content_changed(const Field_info* field_info);
  //@}

  /*! Calculate multiple normals per vertex for all vertices.
   * If the angle between the geometric normals of two adjacent faces is less
   * than the crease angle, calculate the normals so that the facets are
   * smooth-shaded across the edge. Otherwise, calculate the normals so that
   * the facets are faceted.
   */
  void calculate_multiple_normals_per_vertex();

  /*! Compute the volume of the polyhedron.
   */
  Float volume();

  /*! Compute the surface area of the polyhedron.
   */
  Float surface_area();

protected:

  /*! The volume of the polyhedron. */
  Float m_volume;

  /*! The surface area of the polyhedron. */
  Float m_surface_area;

  /*! Indicates whether the volume is dirty and thus must be cleaned. */
  Boolean m_dirty_volume;

  /*! Indicates whether the surface area is dirty and thus must be cleaned. */
  Boolean m_dirty_surface_area;

  /*! Indicates whether the coordinate array has beeen invalidated. */
  Boolean m_dirty_coord_array;

  /*! The actual polyhedron object. */
  Polyhedron m_polyhedron;

  /*! Indicates whether the polyhedron has been invalidated, and thus must be
   * cleaned.
   */
  Boolean m_dirty_polyhedron;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Determine whether the angle between two given vectors is smooth.
   */
  Boolean is_smooth(const Vector3f& normal1, const Vector3f& normal2) const;

  /*! Clean (compute) the volume.
   */
  void clean_volume();

  /*! Clean (compute) the surface area.
   */
  void clean_surface_area();

private:
  /*! A functor that calculates the normal of a given facet. */
  struct Facet_normal_calculator {
    template <typename Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      // Facet::Plane_3 is the normal vector type. We assume the
      // CGAL Kernel here and use its global functions.
      Kernel::Vector_3 normal =
        CGAL::cross_product(h->next()->vertex()->point() -
                            h->vertex()->point(),
                            h->next()->next()->vertex()->point() -
                            h->next()->vertex()->point());
      return normal / CGAL::sqrt(normal.squared_length());
    }
  };

  /*! A functor that calculates the normal of a given (half)edge. */
  struct Edge_normal_calculator {
    /*! The crease angle. */
    Float m_crease_angle;

    /*! Indicates whether all edges are creased. */
    Boolean m_creased;

    /*! Indicates whether all edges are smooth. */
    Boolean m_smooth;

    /*! Constructor. */
    Edge_normal_calculator(Float crease_angle) :
      m_crease_angle(crease_angle),
      m_creased(true),
      m_smooth(true)
    {}

    /*! Calculate the normal of a given (half)edge.
     * \param[in] edge the given (half)edge.
     */
    template <typename Halfedge>
    void operator()(Halfedge& edge)
    {
      if (edge.is_border_edge()) return;
      Kernel::Vector_3 normal1 = edge.facet()->plane();
      Kernel::Vector_3 normal2 = edge.opposite()->facet()->plane();
      Float angle = arccosf(normal1 * normal2);  // inner product
      if (abs(angle) > m_crease_angle) {
        edge.m_creased = true;
        edge.opposite()->m_creased = true;
        m_smooth = false;
        return;
      }
      edge.m_creased = false;
      edge.opposite()->m_creased = false;
      m_creased = false;
    }
  };

  /*! Clean the coordinate indices.
   */
  void clean_coord_indices();

  /*! The builder. */
  Polyhedron_geo_builder<Polyhedron::HalfedgeDS> m_surface;

  /*! Indicates whether all edges are creased. */
  Boolean m_creased;

  /*! Indicates whether all edges are smooth. */
  Boolean m_smooth;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Indexed_face_set* Indexed_face_set::prototype()
{ return new Indexed_face_set(true); }

//! \brief clones.
inline Container* Indexed_face_set::clone() { return new Indexed_face_set(); }

/*! \brief determines whether the coordinate array has been invalidated,
 * and thus must be cleaned.
 */
inline Boolean Indexed_face_set::is_dirty_coord_array() const
{ return m_dirty_coord_array; }

/*! \brief determine whether the polyhedron has been invalidated, and thus
 * must be cleaned.
 */
inline Boolean Indexed_face_set::is_dirty_polyhedron() const
{ return m_dirty_polyhedron; }

//! \brief determines whether the polyhedron representation is empty.
inline bool Indexed_face_set::is_polyhedron_empty() const
{ return m_polyhedron.empty(); }

SGAL_END_NAMESPACE

#endif
