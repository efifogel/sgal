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

/*! \file
 *
 * Extract the border of a polygonal surface
 */

#ifndef SGAL_SURFACE_BORDER_HPP
#define SGAL_SURFACE_BORDER_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Indexed_line_set.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Container_proto;
class Mesh_set;

/*! \class Surface_border Surface_border.hpp
 * Surface_border is an engine node in the scene graph that computes the
 * borders of a surface polyhedron.
 */
class SGAL_SGAL_DECL Surface_border : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    SURFACE,
    BORDER,
    TRIGGER,
    LAST
  };

  typedef boost::shared_ptr<Indexed_line_set>       Shared_indexed_line_set;
  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;
  typedef boost::shared_ptr<Exact_polyhedron_geo>   Shared_exact_polyhedron_geo;

  /*! Construct.
   */
  Surface_border(Boolean proto = false);

  /*! Destruct.
   */
  virtual ~Surface_border();

  /* Construct the prototype */
  static Surface_border* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Shared_mesh_set* surface_handle(const Field_info*) { return &m_surface; }
  Shared_indexed_line_set* border_handle(const Field_info*) { return &m_border; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Export this container.
   * \param[in] formatter the formatter to use for exporting; e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! triggers the execution of the engine as a response to change in one
   * of the input fields.
   */
  void trigger_changed(const Field_info* field_info);

  /*! Execute the engine. */
  void execute();

  /*! Set the surface.
   * \param operand the surface.
   */
  void set_surface(Shared_mesh_set operand);

  /*! Obtain the surface.
   * \border the surface.
   */
  Shared_mesh_set get_surface() const;

  /*! Set the resulting polylines.
   * \param[in] border the resulting polylines.
   */
  void set_border(Shared_indexed_line_set border);

  /*! Obtain the resulting polyline.
   * \param[in] border the resulting polyline.
   */
  Shared_indexed_line_set get_border() const;

protected:
  /*! Surface border visitor. */
  class Construct_border_visitor : public boost::static_visitor<>
  {
  private:
    /*! The resulting polylines. */
    Shared_indexed_line_set m_border;

  public:
    Construct_border_visitor(Shared_indexed_line_set border) :
      m_border(border)
    {}

    template <typename Polyhedron_>
    void operator()(const Polyhedron_& polyhedron)
    {
      typedef Polyhedron_               Polyhedron;
      typedef std::list<typename Polyhedron::Halfedge_const_handle>   Hole;

      if (polyhedron.empty()) return;

      auto begin = polyhedron.border_edges_begin();
      auto end = polyhedron.edges_end();
      init_border_edges(begin, end);
      if (polyhedron.is_closed()) return;
      std::list<Hole> holes;
      construct_holes(begin, end, holes);
      // std::cout << "# : " << holes.size() << std::endl;
      construct_polylines(holes);
      for (auto& hole: holes) hole.clear();
      holes.clear();
    }
  private:
    /*! Initialize the border edges.
     */
    template <typename InputIterator>
    void init_border_edges(InputIterator begin, InputIterator end) const
    { for (auto it = begin; it != end; ++it) it->opposite()->set_flag(false); }

    /*! Construct the holes.
     */
    template <typename InputIterator, typename Holes>
    void construct_holes(InputIterator begin, InputIterator end, Holes& holes)
      const
    {
      auto it = begin;
      while (true) {
        auto first = it->opposite();
        holes.emplace_back();
        auto& border_edges = holes.back();
        auto circ = first;
        do {
          border_edges.push_back(circ);
          circ->set_flag(true);
          circ = circ->next();
        } while(circ != first);

        while (it->opposite()->is_flag()) {
          ++it;
          if (it == end) return;
        }
      }
    }

    /*! Construct the polylines.
     */
    template <typename Holes>
    void construct_polylines(const Holes& holes)
    {
      size_t size(0);
      for (auto& hole: holes) size += hole.size();
      m_border->set_num_primitives(holes.size());
      m_border->set_primitive_type(Geo_set::PT_LINE_LOOPS);
      auto& indices = m_border->get_coord_indices();
      indices.resize(size + holes.size());
      if (!m_border->get_coord_array()) {
        Geo_set::Shared_coord_array coord_array(new Coord_array_3d(size));
        m_border->set_coord_array(coord_array);
      }
      auto coord_array = m_border->get_coord_array();
      auto coords = boost::static_pointer_cast<Coord_array_3d>(coord_array);
      size_t i(0);
      size_t j(0);
      for (auto& hole: holes) {
        for (auto hh: hole) {
          const auto& point = hh->vertex()->point();
          (*coords)[j] = to_vector3f(point);
          indices[i++] = j++;
        }
        indices[i++] = -1;
      }
    }
  };

  /*! The input polygonal mesh. */
  Shared_mesh_set m_surface;

  /*! The resulting polylines. */
  Shared_indexed_line_set m_border;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Initialize the border edges.
   */
  template <typename InputIterator>
  void init_border_edges(InputIterator begin, InputIterator end)
  { for (auto it = begin; it != end; ++it) it->opposite()->set_flag(false); }

  /*! Construct the holes.
   */
  template <typename InputIterator, typename Holes>
  void construct_holes(InputIterator begin, InputIterator end, Holes& holes)
  {
    auto it = begin;
    while (true) {
      auto first = it->opposite();
      holes.emplace_back();
      auto& border_edges = holes.back();
      auto circ = first;
      do {
        border_edges.push_back(circ);
        circ->set_flag(true);
        circ = circ->next();
      } while(circ != first);

      while (it->opposite()->is_flag()) {
        ++it;
        if (it == end) return;
      }
    }
  }

  /*! Construct the polylines.
   */
  template <typename Holes>
  void construct_polylines(const Holes& holes)
  {
    size_t size(0);
    for (auto& hole: holes) size += hole.size();
    m_border->set_num_primitives(holes.size());
    m_border->set_primitive_type(Geo_set::PT_LINE_LOOPS);
    auto& indices = m_border->get_coord_indices();
    indices.resize(size + holes.size());
    if (!m_border->get_coord_array()) {
      Geo_set::Shared_coord_array coord_array(new Coord_array_3d(size));
      m_border->set_coord_array(coord_array);
    }
    auto coord_array = m_border->get_coord_array();
    auto coords = boost::static_pointer_cast<Coord_array_3d>(coord_array);
    size_t i(0);
    size_t j(0);
    for (auto& hole: holes) {
      for (auto hh: hole) {
        const auto& point = hh->vertex()->point();
        (*coords)[j] = to_vector3f(point);
        indices[i++] = j++;
      }
      indices[i++] = -1;
    }
  }

  /*! Construt the border.
   */
  template <typename Polyhedron>
  void construct_border(const Polyhedron& polyhedron)
  {
    typedef std::list<typename Polyhedron::Halfedge_const_handle>   Hole;

    if (polyhedron.empty()) return;
    init_border_edges(polyhedron.border_edges_begin(), polyhedron.edges_end());
    if (polyhedron.is_closed()) return;
    std::list<Hole> holes;
    construct_holes(polyhedron.border_edges_begin(), polyhedron.edges_end(),
                    holes);
    // std::cout << "# : " << holes.size() << std::endl;
    construct_polylines(holes);
    for (auto& hole: holes) hole.clear();
    holes.clear();
  }

private:
  /*! Trigger of the engine that makes the engine excute. */
  Boolean m_trigger;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The prototype of this node. */
  static Container_proto* s_prototype;

  /*! Default value. */
};

//! \brief constructs the prototype.
inline Surface_border* Surface_border::prototype()
{ return new Surface_border(true); }

//! \brief clones.
inline Container* Surface_border::clone() { return new Surface_border(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Surface_border::get_tag() const { return s_tag; }

//! \brief obtains the input surface.
inline Surface_border::Shared_mesh_set Surface_border::get_surface() const
{ return m_surface; }

//! \brief sets the input surface.
inline void Surface_border::set_surface(Shared_mesh_set surface)
{ m_surface = surface; }

//! \brief sets the resulting geometry.
inline void Surface_border::
set_border(Shared_indexed_line_set border) { m_border = border; }

//! \brief obtains the resulting border.
inline Surface_border::Shared_indexed_line_set Surface_border::get_border() const
{ return m_border; }

SGAL_END_NAMESPACE

#endif
