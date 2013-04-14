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
// $Id: Lower_envelope_sphere_geo.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOWER_ENVELOPE_SPHERE_GEO_HPP
#define SGAL_LOWER_ENVELOPE_SPHERE_GEO_HPP

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure, where the surfaces are spheres.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Env_sphere_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>
#include <CGAL/envelope_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Sphere.hpp"

#include "SCGAL/Lower_envelope_geo.hpp"
#include "SCGAL/Lower_envelope_sphere.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Min_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Isect_action;
class Draw_action;
class Isect_action;
class Appearance;

/*! A lower-envelope data structure. */
class Lower_envelope_sphere_geo : public Lower_envelope_geo {
public:
  typedef boost::shared_ptr<Appearance>                   Shared_appearance;

  typedef CGAL::CORE_algebraic_number_traits              Nt_traits;
  typedef Nt_traits::Rational                             Rational;
  typedef Nt_traits::Algebraic                            Algebraic;
  typedef CGAL::Cartesian<Rational>                       Rat_kernel;
  typedef Rat_kernel::Point_3                             Rat_point_3;
  typedef CGAL::Cartesian<Algebraic>                      Alg_kernel;

  /* Convert an exact point into Vector3f. */
  typedef CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>
                                                          Conic_traits_2;

  typedef CGAL::Env_sphere_traits_3<Conic_traits_2>
    Env_sphere_traits_base_3;
  typedef Env_sphere_traits_base_3::Surface_3             Sphere_base_3;

  typedef CGAL::Env_surface_data_traits_3<Env_sphere_traits_base_3,
                                          Shared_appearance>
    Env_sphere_traits_3;
  typedef Env_sphere_traits_3::Surface_3                  Sphere_3;
  // Adding the specific type because I removed template template parameter.
  // \todo Consult Efi.
  typedef CGAL::Envelope_diagram_2<Env_sphere_traits_3,
                                   Rendered_Envelope_diagram_dcel< 
                                     Env_sphere_traits_3, 
                                     Env_sphere_traits_3::Xy_monotone_surface_3
                                     >
                                   >
                                                          Envelope_diagram_2;

public:
  enum {
    FIRST = Lower_envelope_geo::LAST - 1,
    LAST
  };

  /*! Constructor. */
  Lower_envelope_sphere_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Lower_envelope_sphere_geo();

  /*! Construct the prototype. */
  static Lower_envelope_sphere_geo* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the prototype of this container. */
  virtual void delete_prototype();

  /*! Obtain the prototype of this container. */
  virtual Container_proto* get_prototype();

  struct Convert_approximate_sphere {
    Approximate_sphere_3
    operator()(const Envelope_diagram_2::Vertex& vertex) const
    {
      const Conic_traits_2::Point_2 point = vertex.point();
      Vector2f vec(static_cast<float>(CGAL::to_double(point.x())),
                   static_cast<float>(CGAL::to_double(point.y())));
      Approximate_point_3 approximate_point(vec[0], vec[1], 0);
      return Approximate_sphere_3(approximate_point, 0);
    }
  };

  /*! Transform the coordinates of the envelope into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  virtual void transform_coords(Approximate_sphere_vector& spheres)
  {

    // transform_coords_impl(m_envelope, spheres);
    if (!m_envelope || is_empty()) return;
  
    spheres.resize(m_envelope->number_of_vertices());
    Convert_approximate_sphere convert;
    std::transform(m_envelope->vertices_begin(), m_envelope->vertices_end(),
                   spheres.begin(), convert);
  }

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

  /*! */
  Uint m_resolution;

  // Default values:
  static Uint s_def_resolution;

  /*! Obtain all spheres.
   * \param oi the output iterator that collects all spheres.
   * \return the output iterator at the end.
   */
  template <typename OutputIterator>
  OutputIterator get_spheres(OutputIterator oi);
};

/*! \brief constructs the prototype. */
inline Lower_envelope_sphere_geo* Lower_envelope_sphere_geo::prototype()
{ return new Lower_envelope_sphere_geo(true); }

/*! \brief clones. */
inline Container* Lower_envelope_sphere_geo::clone()
{ return new Lower_envelope_sphere_geo(); }

/*! \brief determines whether the envelope is empty. */
inline Boolean Lower_envelope_sphere_geo::is_empty() const
{ return m_envelope->is_empty(); }

/*! \brief obtains all spheres. */
template <typename OutputIterator>
OutputIterator Lower_envelope_sphere_geo::get_spheres(OutputIterator oi)
{
  for (Node_iter it = m_surfaces.begin(); it != m_surfaces.end(); ++it) {
    Shared_node node = *it;
    boost::shared_ptr<Shape> shape = boost::dynamic_pointer_cast<Shape>(node);
    SGAL_assertion(shape);
    boost::shared_ptr<Geometry> geo = shape->get_geometry();
    boost::shared_ptr<Sphere> sphere =
      boost::dynamic_pointer_cast<Sphere>(geo);
    SGAL_assertion(sphere);

    const Vector3f& center = sphere->get_center();
    Rat_point_3 rat_center(center[0], center[1], center[2]);
    float radius = sphere->get_radius();
    Sphere_base_3 rat_sphere(rat_center, Rational(radius* radius));
    *oi++ = Sphere_3(rat_sphere, shape->get_appearance());
  }
  return oi;
}

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Lower_envelope_sphere_geo::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
