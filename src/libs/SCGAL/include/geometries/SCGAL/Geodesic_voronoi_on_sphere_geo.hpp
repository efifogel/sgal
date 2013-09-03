// Copyright (c) 2007 Israel.
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
// $Id: Geodesic_voronoi_on_sphere_geo.hpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#ifndef SCGAL_GEODESIC_VORONOI_ON_SPHERE_GEO_HPP
#define SCGAL_GEODESIC_VORONOI_ON_SPHERE_GEO_HPP

#include <CGAL/basic.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/envelope_voronoi_2.h>
#include <CGAL/Envelope_voronoi_traits_2/Spherical_voronoi_diagram_traits_2.h>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Coord_array;
class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;
class Sphere;

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */
class SGAL_SCGAL_DECL Geodesic_voronoi_on_sphere_geo :
  public Arrangement_on_sphere_base_geo
{
public:
  enum {
    FIRST = Arrangement_on_sphere_base_geo::LAST - 1,
    SITE_INDEX_ARRAY,
    DRAW_SITES,
    SITE_COLOR,
    LAST
  };

  // Kernel, Number type, etc.
  typedef Exact_number_type                        Number_type;
  typedef Exact_kernel                             Kernel;
  typedef Kernel::Point_3                          Point_3;

public:
  /*! Constructor. */
  Geodesic_voronoi_on_sphere_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Geodesic_voronoi_on_sphere_geo();

  /*! Clone. */
  virtual Container* clone() = 0;

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* draw_sites_handle(Field_info*) { return &m_draw_sites; }
  //@}

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! Obtain the point index array */
  const std::vector<Uint>& get_site_indices() const;

  /*! Obtain the i-th point index */
  Uint get_site_index(Uint i) const;

  /*! Set the site color */
  void set_site_color(Vector3f& color) { m_site_color = color; }

protected:
  /*! An array of indices into the coordinate array for points. */
  std::vector<Uint> m_site_indices;

  /*! Indicates whether to draw the sites. */
  Boolean m_draw_sites;

  /*! The site color. */
  Vector3f m_site_color;

private:
  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! Default values. */
  static const Boolean s_def_draw_sites;
  static const Vector3f s_def_site_color;

  /*! Draw the marked vertex. */
  void draw_marked_vertex();

  /*! Draw the marked edge. */
  void draw_marked_edge();

  /*! Draw the marked face. */
  void draw_marked_face();

  /*! Increase the vertex index.
   * \param field_info
   */
  void increase_vertex_index(Field_info* field_info = NULL);

  /*! Increase the edge index.
   * \param field_info
   */
  void increase_edge_index(Field_info* field_info = NULL);

  /*! Increase the face index.
   * \param field_info
   */
  void increase_face_index(Field_info* field_info = NULL);
};

/*! \brief obtains the site index array. */
inline const std::vector<Uint>&
Geodesic_voronoi_on_sphere_geo::get_site_indices() const
{ return m_site_indices; }

/*! \brief obtains the i-th site index. */
inline Uint
Geodesic_voronoi_on_sphere_geo::get_site_index(Uint i) const
{ return m_site_indices[i]; }

SGAL_END_NAMESPACE

#endif
