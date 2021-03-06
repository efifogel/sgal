// Copyright (c) 2004, 2005, 2006 Israel.
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

#ifndef SCGAL_SPHERICAL_GAUSSIAN_MAP_GEO_HPP
#define SCGAL_SPHERICAL_GAUSSIAN_MAP_GEO_HPP

/*! \file
 */

#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Spherical_gaussian_map.hpp"
#include "SCGAL/Spherical_gaussian_map_base_geo.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_renderer.hpp"
#include "SCGAL/Arrangement_renderers.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Container_proto;
class Draw_action;
class Scene_graph;
class Cylinder;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Spherical_gaussian_map_geo :
  public Spherical_gaussian_map_base_geo {
public:
  typedef boost::shared_ptr<Spherical_gaussian_map_geo>
    Shared_spherical_gaussian_map_geo;

  typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Spherical_gaussian_map,
                                                Epec_kernel>
                                                        Polyhedron;

  /*! Fields */
  enum {
    FIRST = Spherical_gaussian_map_base_geo::LAST - 1,
    GEOMETRIES,
    LAST
  };

private:
  typedef Spherical_gaussian_map                         Sgm;

protected:
  typedef Spherical_gaussian_map_geo                     Self;

  // List of pointers to Spherical_gaussian_map_geo objects. */
  typedef std::vector<Shared_spherical_gaussian_map_geo> Sgm_node_array;
  typedef Sgm_node_array::iterator                       Sgm_node_iter;

  /*! The minkowski sum operands. */
  Sgm_node_array m_sgm_nodes;

  /*! Obtain the tag (type) of the container .*/
  virtual const std::string& get_tag() const;

  /*! Create the renderers. */
  void create_renderers();

  /*! Destroy the renderers. */
  void destroy_renderers();

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default values. */

  /*! Indicates whether the sgm data structure is owned, i.e., explicitly
   * allocated. If the sgm data structure is owned, it should be destructed
   * when this geometry node is destructed.
   */
  Boolean m_owned_sgm;

  //! The spherical Gaussian map representation.
  Sgm* m_sgm;

  //! Indicates whether to compute the minkowski sum.
  Boolean m_minkowski_sum;

  //! For benchmarking.
  float m_time;

  //! The vertices renderer.
  Arrangement_renderer::Renderer* m_vertices_renderer;

  //! The vertices renderer.
  Arrangement_renderer::Renderer* m_colored_vertices_renderer;

  //! The edges renderer.
  Arrangement_renderer::Renderer* m_edges_renderer;

  //! The colored edges renderer.
  Arrangement_renderer::Renderer* m_colored_edges_renderer;

  //! The inflated line edges renderer.
  Arrangement_renderer::Renderer* m_inflated_line_edges_renderer;

  //! The inflated strip edges renderer.
  Arrangement_renderer::Renderer* m_inflated_strip_edges_renderer;

  //! The inflated tube edges renderer.
  Arrangement_renderer::Renderer* m_inflated_tube_edges_renderer;

  /*! Transform the coordinates of the SGM into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  virtual void transform_coords(Inexact_sphere_vector& spheres)
  { transform_primal_coords(m_sgm, spheres); }

  /*! Draw the primal representation of the polyhedron. */
  virtual void draw_primal(Draw_action* action);

  /*! Draw the arrangement on sphere opaque.
   * \param action
   */
  virtual void draw_aos_opaque(Draw_action* action);

  virtual void isect_primary();

public:
  /*! Constructor */
  Spherical_gaussian_map_geo(Boolean proto = false);

  /*! Copy constructor */
  Spherical_gaussian_map_geo(const Spherical_gaussian_map_geo& gm);

  /*! Destructor. */
  virtual ~Spherical_gaussian_map_geo();

  /*! Construct the prototype. */
  static Spherical_gaussian_map_geo* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the prototype. */
  virtual void delete_prototype();

  /*! Obtain the prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Sgm_node_array* sgm_nodes_handle(const Field_info*) { return &m_sgm_nodes; }
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Clean the representation. */
  virtual void clean_sgm();

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear_sgm();

  /*! Determine whether the representation is empty. */
  virtual Boolean is_sgm_empty() const;

  /*! Draw the arrangement on sphere vertices.
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action);

  /*! Draw the arrangement on sphere edges.
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action);

  /*! Clean the renderer. */
  virtual void clean_renderer();

  /*! Print statistics. */
  virtual void print_stat();

  /*! Set the source gausian maps of the minkowski sum. */
  void insert_sgm(Shared_spherical_gaussian_map_geo sgm);

  /*! Obrain the Gaussian map. */
  Sgm* get_sgm();

  /*! Set the Gaussian map. */
  void set_sgm(Sgm* sgm);

  /*! Set the flag that indicates whether to compute the minkowski sum. */
  void set_minkowski_sum(Boolean flag);

  /*! Compute the equations of, or the normals to, the planes of the aos
   * facets and store them at the vertices of the planar map.
   */
  void update_facets();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Spherical_gaussian_map_geo* Spherical_gaussian_map_geo::prototype()
{ return new Spherical_gaussian_map_geo(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Spherical_gaussian_map_geo::create()
{ return new Spherical_gaussian_map_geo(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Spherical_gaussian_map_geo::get_tag() const
{ return s_tag; }

//! \brief determines whether the representation is empty.
inline Boolean Spherical_gaussian_map_geo::is_sgm_empty() const
{ return m_sgm->is_empty(); }

//! \brief sets the flag that indicates whether to compute the minkowski sum.
inline void Spherical_gaussian_map_geo::set_minkowski_sum(Boolean flag)
{ m_minkowski_sum = flag; }

/*! \brief computes the equations of, or the normals to, the planes of the aos
 * facets and store them at the vertices of the planar map.
 */
inline void Spherical_gaussian_map_geo::update_facets()
{ Spherical_gaussian_map_base_geo::update_facets(m_sgm); }

SGAL_END_NAMESPACE

#endif
