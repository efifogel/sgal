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
// $Id: Arrangement_on_sphere_graph_geo.hpp 7789 2009-07-19 12:40:30Z efif $
// $Revision: 7789 $
//
// Author(s) : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#ifndef SCGAL_ARRANGEMENT_ON_SPHERE_GRAPH_GEO_HPP
#define SCGAL_ARRANGEMENT_ON_SPHERE_GRAPH_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_marked.hpp"
#include "SCGAL/Arrangement_on_sphere_graph.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_graph_overlay_traits.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */
class SGAL_SCGAL_DECL Arrangement_on_sphere_graph_geo :
  public Arrangement_on_sphere_base_geo
{
public:
  typedef boost::shared_ptr<Arrangement_on_sphere_graph_geo>
    Shared_arrangement_on_sphere_graph_geo;

  enum {
    FIRST = Arrangement_on_sphere_base_geo::LAST - 1,
    OVERLAY,
    LAST
  };

protected:
  typedef Arrangement_on_sphere_graph_geo           Self;
  typedef SGAL::Arrangement_on_sphere_graph         Aos_graph;
  typedef SGAL::Arrangement_on_sphere_marked        Aos_mark;

public:
  /*! Constructor. */
  Arrangement_on_sphere_graph_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Arrangement_on_sphere_graph_geo();

  /* Construct the prototype. */
  static Arrangement_on_sphere_graph_geo* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation. */
  virtual void clean();

  /*! Cull. */
  virtual void cull(Cull_context& /* cull_context */) {}

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();

  /*! Determine whether the representation empty. */
  virtual Boolean is_empty() const { return m_aos->is_empty(); }

  /*! Obrain the arrangement. */
  Aos_graph* get_aos();

  /*! Set the arrangement. */
  void set_aos(Aos_graph* aos);

  /*! Add a geometry container that represents an arrangement on a
   * sphere to the list of such geometry containers.
   */
  void add_aos_geo(Shared_arrangement_on_sphere_graph_geo aos_geo)
  { m_aoses.push_back(aos_geo); }

  /*! Obtain the overlay traits (const version). */
  const Arrangement_graph_overlay_traits<Aos_mark, Aos_graph> &
    get_overlay_traits() const
  { return m_overlay_traits; }

  /*! Obtain the overlay traits (non-const version). */
  Arrangement_graph_overlay_traits<Aos_mark, Aos_graph> & get_overlay_traits()
  { return m_overlay_traits; }

  /*! Print statistics. */
  void print_stat() { Arrangement_on_surface_geo::print_stat(this); }

  /*! Draw the arrangement vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action);

  /*! Draw the arrangement isolated vertices.
   * \param action
   */
  virtual void draw_aos_isolated_vertices(Draw_action* action);

  /*! Draw the arrangement edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action);

protected:
  typedef Vertices_renderer<Self>          Sphere_vertices_renderer;
  typedef Isolated_vertices_renderer<Self> Sphere_isolated_vertices_renderer;
  typedef Edges_renderer<Self>             Sphere_edges_renderer;

  typedef Colored_vertices_renderer<Sphere_vertices_renderer>
                                           Sphere_colored_vertices_renderer;
  typedef Colored_isolated_vertices_renderer<Sphere_isolated_vertices_renderer>
    Sphere_colored_isolated_vertices_renderer;
  typedef Colored_edges_renderer<Sphere_edges_renderer>
                                           Sphere_colored_edges_renderer;

  typedef Line_colored_edges_renderer<Sphere_colored_edges_renderer>
                                           Sphere_line_colored_edges_renderer;
  typedef Point_colored_vertices_renderer<Sphere_colored_vertices_renderer>
    Sphere_point_colored_vertices_renderer;
  typedef Ring_colored_vertices_renderer<Sphere_colored_vertices_renderer>
    Sphere_ring_colored_vertices_renderer;
  typedef Point_colored_isolated_vertices_renderer
    <Sphere_colored_isolated_vertices_renderer>
    Sphere_point_colored_isolated_vertices_renderer;
  typedef Ring_colored_isolated_vertices_renderer
    <Sphere_colored_isolated_vertices_renderer>
    Sphere_ring_colored_isolated_vertices_renderer;

  typedef Inflated_line_edges_renderer<Sphere_edges_renderer>
                                           Sphere_inflated_line_edges_renderer;
  typedef Inflated_strip_edges_renderer<Sphere_edges_renderer>
                                           Sphere_inflated_strip_edges_renderer;
  typedef Inflated_tube_edges_renderer<Sphere_edges_renderer>
                                           Sphere_inflated_tube_edges_renderer;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string & get_tag() const { return s_tag; }

  /*! Indicates whether the aos data structure is owned, i.e., explicitly
   * allocated. If the aos data structure is owned, it should be destructed
   * when this geometry node is destructed.
   */
  Boolean m_owned_aos;

  /*! The arrangement of great-circle arcs on a sphere. */
  Aos_graph* m_aos;

  typedef std::vector<Shared_arrangement_on_sphere_graph_geo> Aos_geo_vector;
  typedef Aos_geo_vector::iterator                            Aos_geo_iter;
  typedef Aos_geo_vector::difference_type                     Aos_geo_diff;

  /*! A container of geometry nodes that represent arrangements of
   * great-circle arcs on a sphere.
   */
  Aos_geo_vector m_aoses;

  /*! An overlay-traits class for computing the overlay. */
  Arrangement_graph_overlay_traits<Aos_mark, Aos_graph> m_overlay_traits;

  /*! Create the renderers. */
  void create_renderers();

  /*! Detsroy the renderers. */
  void destroy_renderers();

 private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The container prototype.
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Arrangement_on_sphere_graph_geo*
Arrangement_on_sphere_graph_geo::prototype()
{ return new Arrangement_on_sphere_graph_geo(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Arrangement_on_sphere_graph_geo::create()
{ return new Arrangement_on_sphere_graph_geo(); }

//! \brief draws the arrangement vertices.
inline void Arrangement_on_sphere_graph_geo::
draw_aos_vertices(Draw_action* action)
{ my_draw_aos_vertices(m_aos, action); }

//! \brief draws the arrangement isolated vertices.
inline void Arrangement_on_sphere_graph_geo::
draw_aos_isolated_vertices(Draw_action* action)
{ my_draw_aos_isolated_vertices(m_aos, action); }

//! \brief draws the arrangement edges.
inline void Arrangement_on_sphere_graph_geo::
draw_aos_edges(Draw_action* action)
{ my_draw_aos_edges(m_aos, action); }

SGAL_END_NAMESPACE

#endif
