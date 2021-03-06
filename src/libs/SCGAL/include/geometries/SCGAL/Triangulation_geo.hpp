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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_TRIANGULATION_GEO_HPP
#define SCGAL_TRIANGULATION_GEO_HPP

/*! \file
 * A node in the scene graph that maintains a polyhedron.
 * The the point of each vertex and the plane of each facet is in exact
 * representation. Each facet is extended with the normal of the facet-plane
 * in Vector3f (approximate) representation. The later is used for rendering.
 */

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#if defined(CGAL_USE_LEDA)
#include <CGAL/leda_rational.h>
#endif
#include <CGAL/enum.h>
#include <CGAL/Triangulation_data_structure_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Triangulation_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Min_sphere.hpp"
#include "SCGAL/Exact_number_type.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Cull_context;
class Isect_action;
class Draw_action;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Triangulation_geo : public SGAL::Geometry {
public:
  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;
  typedef boost::shared_ptr<Color_array>                Shared_color_array;

  typedef CGAL::Tag_true                                Tag_true;
  typedef CGAL::Tag_false                               Tag_false;

  typedef Exact_number_type                             Number_type;
  typedef CGAL::Cartesian<Number_type>                  Kernel;
  typedef Kernel::Construct_vertex_3                    Construct_vertex_3;

  // Trainagulation stuff:
  class Vertex_data {
  public:
    void set_coord(const Vector3f& coord) { m_coord = coord; }
    void set_coord(float x, float y, float z) { m_coord.set(x, y, z); }
    const Vector3f& get_coord() const { return m_coord; }

    void set_color(const Vector3f& color) { m_color = color; }
    void set_color(float r, float g, float b) { m_color.set(r, g, b); }
    const Vector3f& get_color() const { return m_color; }

  private:
    /*! The coordinate of the point in drawable format. */
    Vector3f m_coord;

    /*! The color of the point. */
    Vector3f m_color;
  };
  typedef CGAL::Triangulation_vertex_base_with_info_3<Vertex_data, Kernel> Vb;
  typedef CGAL::Triangulation_data_structure_3<Vb>      Tds;

  typedef CGAL::Triangulation_3<Kernel, Tds>            Triangulation_3;
  typedef Triangulation_3::Point                        Point;
  typedef Triangulation_3::Segment                      Segment;
  typedef Triangulation_3::Triangle                     Triangle;
  typedef Triangulation_3::Finite_edges_iterator        Finite_edges_iterator;
  typedef Triangulation_3::Finite_vertices_iterator
    Finite_vertices_iterator;
  typedef Triangulation_3::Cell_handle                  Cell_handle;
  typedef Triangulation_3::Vertex_handle                Vertex_handle;
public:
  enum {
    FIRST = Geometry::LAST - 1,
    COORD,
    COLOR,
    LINE_WIDTH,
    DRAW_HALOED,
    LAST
  };

  /*! Constructor. */
  Triangulation_geo(Boolean proto = false);

  /*! Destructor. */
  ~Triangulation_geo();

  /*! Construct the prototype. */
  static Triangulation_geo* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  // coord
  Shared_coord_array* coord_array_handle(const Field_info*)
  { return &m_coord_array; }
  Float* line_width_handle(const Field_info*) { return &m_line_width; }
  Boolean* draw_haloed_handle(const Field_info*) { return &m_draw_haloed; }

  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Draw the triangulation. */
  virtual void draw(Draw_action* action);

  virtual void cull(Cull_context& cull_context);

  virtual void isect(Isect_action* action);

  /*! Clean the bounding sphere of the triangulation. */
  virtual void clean_bounding_sphere();

 /*! Print statistics */
  void print_stat();

  /*! Determine whether the geometry has color. */
  virtual Boolean has_color();

private:
  /*! Extracts the approximate point from a polyhedron vertex. */
  struct Convert_approximate_sphere {
    Inexact_sphere_3
    operator()(const Triangulation_3::Vertex::Point& point) const
    { return to_inexact_sphere(point); }
  };

  struct Convert_exact_point {
    Point operator()(const Vector3f& point) const
    {
      Number_type x(point[0]);
      Number_type y(point[1]);
      Number_type z(point[2]);
      return Point(x, y, z);
    }
  };

  struct Point_cmp {
    bool operator()(const Point* p1, const Point* p2) const
    {
      Kernel::Less_xyz_3 cmp = kernel.less_xyz_3_object();
      return cmp(*p1, *p2);
    }
    Kernel kernel;
  };

  //! The tag that identifies this container type.
  static std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! Indicates whether the representation is dirty.
  Boolean m_dirty;

  //! A temporary place holder for the coordinate set.
  Shared_coord_array m_coord_array;

  //! A temporary place holder for the color set.
  Shared_color_array m_color_array;

  //! The width of the dual lines.
  float m_line_width;

  //! Indicates whether to draw haloed lines.
  bool m_draw_haloed;

  //! Default value.
  static const float s_def_line_width;
  static const bool s_def_draw_haloed;

  //! The triangulation representation.
  Triangulation_3 m_triangulation;

  //! Indicates that the bbox is set externally.
  bool m_bb_is_pre_set;

  //! The time is took to compute the minkowski sum in seconds.
  float m_time;

protected:
  /*! obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Clean the polyhedron data structure. */
  void clean();

  /*! Clear the internal representation. */
  void clear();

  /*! Determine whether the representation be cleaned. */
  Boolean is_dirty() const { return m_dirty; }

  /*! Mark the object as not initialized. */
  void coord_changed(const Field_info* field_info);

  /*! Set the coordinate set. */
  void set_coord_array(Shared_coord_array coord_array);

  /*! Obtain the coordinate set. */
  Shared_coord_array get_coord_array() const;

  /*! Set the coordinate set. */
  void set_color_array(Shared_color_array color_array);

  /*! Obtain the color set. */
  Shared_color_array get_color_array() const;

  /*! Draw the geometry. */
  void draw_geometry(Draw_action* action);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Triangulation_geo* Triangulation_geo::prototype()
{ return new Triangulation_geo(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Triangulation_geo::create()
{ return new Triangulation_geo(); }

//! \brief sets the coordinate set.
inline void Triangulation_geo::set_coord_array(Shared_coord_array coord_array)
{ m_coord_array = coord_array; }

//! \brief obtain the coordinate set.
inline Triangulation_geo::Shared_coord_array
Triangulation_geo::get_coord_array() const
{ return m_coord_array; }

//! \brief sets the color set.
inline void Triangulation_geo::set_color_array(Shared_color_array color_array)
{ m_color_array = color_array; }

//! \brief obtains the color set.
inline Triangulation_geo::Shared_color_array
Triangulation_geo::get_color_array() const
{ return m_color_array; }

//! \brief determines whether the geometry has color.
inline Boolean Triangulation_geo::has_color()
{
  return do_generate_color() && m_color_array && (0 != m_color_array->size());
}

SGAL_END_NAMESPACE

#endif
