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
#include <boost/unordered_map.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Boundary_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Polyhedron.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

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
  //@}

  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Calculate multiple normals per vertex for all vertices.
   * If the angle between the geometric normals of two adjacent faces is less
   * than the crease angle, calculate the normals so that the facets are
   * smooth-shaded across the edge. Otherwise, calculate the normals so that
   * the facets are faceted.
   */
  void calculate_multiple_normals_per_vertex();

  /*! Clean the polyhedron. */
  virtual void clean_polyhedron();

  /*! Calculate the normals in case they are invalidated.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  virtual void clean_normals();

protected:
  /*! The actual polyhedron object. */
  Polyhedron m_polyhedron;

  /*! Indicates whether the polyhedron is dirty and thus should be cleaned. */
  Boolean m_dirty_polyhedron;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Determine whether the angle between two given vectors is smooth.
   */
  Boolean is_smooth(const Vector3f& normal1, const Vector3f& normal2) const;

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
     * \param edge (in) the given (half)edge.
     */
    template <typename Halfedge>
    void operator()(Halfedge& edge)
    {
      Kernel::Vector_3 normal1 = edge.facet()->plane();
      Kernel::Vector_3 normal2 = edge.opposite()->facet()->plane();
      Float angle = arccosf(normal1 * normal2);  // inner product
      if (angle > m_crease_angle) {
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

SGAL_END_NAMESPACE

#endif
