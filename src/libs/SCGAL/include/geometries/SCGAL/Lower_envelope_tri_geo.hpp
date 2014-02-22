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
// $Id: Lower_envelope_tri_geo.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_LOWER_ENVELOPE_TRI_GEO_HPP
#define SCGAL_LOWER_ENVELOPE_TRI_GEO_HPP

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure, where the surfaces are triangles.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>
#include <CGAL/envelope_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Coord_array.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Lower_envelope_geo.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Min_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Isect_action;
class Draw_action;
class Isect_action;
class Appearance;

/*! A lower-envelope data structure. */
class SGAL_SCGAL_DECL Lower_envelope_tri_geo : public Lower_envelope_geo {
public:
  typedef boost::shared_ptr<Appearance>                   Shared_appearance;

  typedef CGAL::Env_triangle_traits_3<Exact_kernel>
    Env_triangle_traits_base_3;
  typedef Env_triangle_traits_base_3::Surface_3           Base_triangle_3;

  typedef CGAL::Env_surface_data_traits_3<Env_triangle_traits_base_3,
                                          Shared_appearance>
    Env_triangle_traits_3;
  typedef Env_triangle_traits_3::Surface_3                Triangle_3;
  typedef CGAL::Envelope_diagram_2<Env_triangle_traits_3> Envelope_diagram_2;

public:
  enum {
    FIRST = Lower_envelope_geo::LAST - 1,
    LAST
  };

  /*! Constructor. */
  Lower_envelope_tri_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Lower_envelope_tri_geo();

  /*! Construct the prototype. */
  static Lower_envelope_tri_geo* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the prototype of this container. */
  virtual void delete_prototype();

  /*! Obtain the prototype of this container. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Transform the coordinates of the envelope into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  virtual void transform_coords(Approximate_sphere_vector& spheres);

  /*! Draw the envelope faces. */
  virtual void draw_envelope_faces(Draw_action* action);

  /*! Draw the envelope edges. */
  virtual void draw_envelope_edges(Draw_action* action);

  /*! Draw the envelope vertices. */
  virtual void draw_envelope_vertices(Draw_action* action);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Clean the representation. */
  virtual void clean();

  /*! Clear the internal representation. */
  virtual void clear();

  /*! Determine whether the envelope is empty. */
  virtual Boolean is_empty() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether the envelope data structure is owned, i.e., explicitly
   * allocated. If the envelope data structure is owned, it should be
   * destructed when this geometry node is destructed.
   */
  Boolean m_owned_envelope;

  /*! The lower envelope data structure. */
  Envelope_diagram_2* m_envelope;

  /*! Obtain all triangles.
   * \param oi the output iterator that collects all triangles.
   * \return the output iterator at the end.
   */
  template <typename OutputIterator>
  OutputIterator get_triangles(OutputIterator oi);
};

/*! \brief constructs the prototype. */
inline Lower_envelope_tri_geo* Lower_envelope_tri_geo::prototype()
{ return new Lower_envelope_tri_geo(true); }

/*! \brief clones. */
inline Container* Lower_envelope_tri_geo::clone()
{ return new Lower_envelope_tri_geo(); }

/*! \brief transforms the coordinates of the envelope into spheres. */
inline void Lower_envelope_tri_geo::
transform_coords(Approximate_sphere_vector& spheres)
{ transform_coords_impl(m_envelope, spheres); }

/*! \brief determines whether the envelope is empty. */
inline Boolean Lower_envelope_tri_geo::is_empty() const
{ return m_envelope->is_empty(); }

/*! \brief obtains all triangles. */
template <typename OutputIterator>
OutputIterator Lower_envelope_tri_geo::get_triangles(OutputIterator oi)
{
  for (Node_iter it = m_surfaces.begin(); it != m_surfaces.end(); ++it) {
    Shared_node node = *it;
    boost::shared_ptr<Shape> shape = boost::dynamic_pointer_cast<Shape>(node);
    SGAL_assertion(shape);
    boost::shared_ptr<Geometry> geo = shape->get_geometry();
    boost::shared_ptr<Indexed_face_set> ifs =
      boost::dynamic_pointer_cast<Indexed_face_set>(geo);
    SGAL_assertion(ifs);
    SGAL_assertion(ifs->get_primitive_type() == Geo_set::PT_TRIANGLES);
    boost::shared_ptr<Coord_array> coord_array = ifs->get_coord_array();
    const std::vector<Uint>& coord_indices = ifs->get_coord_indices();

    Uint j = 0;
    for (Uint i = 0; i < ifs->get_num_primitives(); ++i) {
      const Vector3f& v1 = (*coord_array)[coord_indices[j++]];
      const Vector3f& v2 = (*coord_array)[coord_indices[j++]];
      const Vector3f& v3 = (*coord_array)[coord_indices[j++]];

      Exact_point_3 p1(v1[0], v1[1], v1[2]);
      Exact_point_3 p2(v2[0], v2[1], v2[2]);
      Exact_point_3 p3(v3[0], v3[1], v3[2]);
      Base_triangle_3 t(p1, p2, p3);
      *oi++ = Triangle_3(t, shape->get_appearance());
    }
  }
  return oi;
}

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Lower_envelope_tri_geo::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
