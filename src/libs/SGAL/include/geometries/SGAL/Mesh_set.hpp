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

#ifndef SGAL_MESH_SET_HPP
#define SGAL_MESH_SET_HPP

#include <vector>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"
#include "SGAL/Geo_set.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Field_info;
class Formatter;
class Stl_formatter;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Mesh_set : public Geo_set {
public:
  enum {
    FIRST = Geo_set::LAST - 1,
    CCW,
    SOLID,
    CONVEX,
    CREASE_ANGLE,
    LAST
  };

  typedef boost::variant<Triangle_indices, Quad_indices, Polygon_indices,
                         Flat_indices>                      Facet_indices;

  /*! Constructor.
   */
  Mesh_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Mesh_set();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* is_ccw_handle(const Field_info*) { return &m_is_ccw; }
  Boolean* is_solid_handle(const Field_info*) { return &m_is_solid; }
  Boolean* is_convex_handle(const Field_info*) { return &m_is_convex; }
  Float* crease_angle_handle(const Field_info*) { return &m_crease_angle; }
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  /*! Draw the mesh conditionaly.
   */
  virtual void draw(Draw_action* action);

  /*! Draw the mesh (unconditionaly).
   */
  virtual void draw_mesh(Draw_action* action);

  /*! Draw the representation. */
  virtual void draw_geometry(Draw_action* action) = 0;

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Obtain the bounding box.
   */
  virtual Bounding_box bounding_box();

  /// \name Field Getters and Setters
  //@{
  /*! Set the counter-clockwise flag.
   */
  void set_ccw(Boolean ccw);

  /*! Determine whether the surface polygons are counter-clockwise.
   */
  Boolean is_ccw() const;

  /*! Set the flag that indicates whether the mesh represents is solid
   * (watertight) polytope. In case the mesh represents a solid polytope,
   * the back faces can be culled out during rendering, and the front faces
   * can be rendered single sided.
   */
  void set_solid(Boolean solid);

  /*! Determines whether the mesh represents a solid (watertight) polytope. */
  Boolean is_solid() const;

  /*! Set the flag that indicates whether the mesh represents a convex
   * polytope.
   */
  void set_convex(Boolean convex);

  /*! Determines whether the mesh represents a convex polytope. */
  Boolean is_convex() const;

  /*! Set the crease angle. */
  void set_crease_angle(Float crease_angle);

  /*! Obtain the creas_angle. */
  Float get_crease_angle() const;
  //@}

  /// \name Index-Arrays Getters
  //@{
  /*! Obtain the coordinate indices.
   * \return the coordinate indices.
   */
  virtual std::vector<Int32>& get_coord_indices();

  /*! Obtain the normal indices.
   * \return the normal indices.
   */
  virtual std::vector<Int32>& get_normal_indices();

  /*! Obtain the color indices.
   * \return the color indices.
   */
  virtual std::vector<Int32>& get_color_indices();

  /*! Obtain the texture-coordinate indices.
   * \return the texture-coordinate indices.
   */
  virtual std::vector<Int32>& get_tex_coord_indices();
  //@}

  /// \name Indices Change Reactors
  //@{
  /*! Respond to a change in the coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the normal indices.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void normal_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the color indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void color_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the texture-coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void tex_coord_indices_changed(const Field_info* field_info = nullptr);
  //@}

  /// \name Facet Indices Change Reactors
  //@{
  /*! Respond to a change in the facet coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_coord_indices_changed();

  /*! Respond to a change in the facet normal indices.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void facet_normal_indices_changed();

  /*! Respond to a change in the facet color indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_color_indices_changed();

  /*! Respond to a change in the facet texture-coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_tex_coord_indices_changed();
  //@}

  /// \name Cleaners
  //@{
  /*! Clean (validate) the coordinate indices. */
  void clean_coord_indices();

  /*! Clean (validate) the normal indices. */
  void clean_normal_indices();

  /*! Clean (validate) the color indices. */
  void clean_color_indices();

  /*! Clean (validate) the texture-coordinate indices. */
  void clean_tex_coord_indices();

  /*! Clean the bounding sphere of the mesh set. */
  virtual void clean_bounding_sphere();
  //@}

  /// \name Clearers
  //@{
  /*! Clear (invalidate) the coordinate indices. */
  void clear_coord_indices();

  /*! Clear (invalidate) the normal indices. */
  void clear_normal_indices();

  /*! Clear (invalidate) the color indices. */
  void clear_color_indices();

  /*! Clear (invalidate) the texture-coordinate indices. */
  void clear_tex_coord_indices();
  //@}

  /// \name Index-Array Predicates
  //@{
  /*! Determine whether the coordinate indices has been invalidated
   * (is dirty).
   * \return a Boolean flag that indicates whether the coordinate indices
   *         is dirty.
   */
  Boolean is_dirty_coord_indices() const;

  /*! Determine whether the normal indices has been invalidated
   * (is dirty).
   * \return a Boolean flag that indicates whether the normal indices is
   *         dirty.
   */
  Boolean is_dirty_normal_indices() const;

  /*! Determine whether the color indices has been invalidated
   * (is dirty).
   * \return a Boolean flag that indicates whether the color indices is
   *         dirty.
   */
  Boolean is_dirty_color_indices() const;

  /*! Determine whether the texture-coordinate indices has been
   * invalidated (is dirty).
   * \return a Boolean flag that indicates whether the texture-coordinate index
   *         array is dirty.
   */
  Boolean is_dirty_tex_coord_indices() const;
  //@}

  /// \name Obtain const vectors of facet indices
  //@{
  /*! Obtain the facet coordinate indices.
   */
  const Facet_indices& get_facet_coord_indices();

  /*! Obtain the facet normal indices.
   */
  const Facet_indices& get_facet_normal_indices();

  /*! Obtain the facet color indices.
   */
  const Facet_indices& get_facet_color_indices();

  /*! Obtain the facet texture-coordinate indices.
   */
  const Facet_indices& get_facet_tex_coord_indices();
  //@}

  /// \name Facet Index Arrays
  //@{
  /*! Set the facet coordinate indices.
   */
  void set_facet_coord_indices(const Facet_indices& indices);

  /*! Set the facet normal indices.
   */
  void set_facet_normal_indices(const Facet_indices& indices);

  /*! Set the facet color indices.
   */
  void set_facet_color_indices(const Facet_indices& indices);

  /*! Set the facet texture-coordinate indices.
   */
  void set_facet_tex_coord_indices(const Facet_indices& indices);

   /*! Clean (validate) the facet coordinate indices.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_facet_coord_indices();

  /*! Clean (validate) the facet normal indices.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_facet_normal_indices();

  /*! Clean (validate) the facet color indices.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_facet_color_indices();

  /*! Clean (validate) the facet texture-coordinate indices.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_facet_tex_coord_indices();

  /*! Clear (invalidate) the facet indicess, i.e., coordinate indices,
   * normal indices, color indices, and texture-coordinate indicess.
   */
  void clear_facet_indices();

  /*! Clear (invalidate) the facet cordinate indices.
   */
  void clear_facet_coord_indices();

  /*! Clear (invalidate) the facet normal indices.
   */
  void clear_facet_normal_indices();

  /*! Clear (invalidate) the facet color indices.
   */
  void clear_facet_color_indices();

  /*! Clear (invalidate) the facet texture-cordinate indices.
   */
  void clear_facet_tex_coord_indices();

  /*! Determine whether the coordinate indices has been invalidated
   * (is dirty) and thus must be cleaned.
   */
  Boolean is_dirty_facet_coord_indices() const;

  /*! Determine whether the normal indices has been invalidated (is dirty)
   * thus must be cleaned.
   */
  Boolean is_dirty_facet_normal_indices() const;

  /*! Determine whether the color indices has been invalidated (is dirty)
   * and thus must be cleaned.
   */
  Boolean is_dirty_facet_color_indices() const;

  /*! Determine whether the texture-coordinate indices has been
   * invalidated (is dirty) and thus must be cleaned.
   */
  Boolean is_dirty_facet_tex_coord_indices() const;

  /*! Initialize the facet cordinate indices.
   */
  void init_facet_coord_indices();

  /*! Initialize the facet normal indices.
   */
  void init_facet_normal_indices();

  /*! Initialize the facet color indices.
   */
  void init_facet_color_indices();

  /*! Initialize the facet texture-cordinate indices.
   */
  void init_facet_tex_coord_indices();

  /*! Resize a facet indices structure with the same size as another.
   */
  void resize_facet_indices(Facet_indices& target, const Facet_indices& source);

  /*! Sequence facet indices visitor */
  class Sequence_facet_indices_visitor : public boost::static_visitor<> {
  public:
    void operator()(Triangle_indices& indices) const
    {
      Index_type k(0);
      for (auto& tri: indices) {
        tri[0] = k++;
        tri[1] = k++;
        tri[2] = k++;
      }
    }

    void operator()(Quad_indices& indices) const
    {
      Index_type k(0);
      for (auto& quad: indices) {
        quad[0] = k++;
        quad[1] = k++;
        quad[2] = k++;
        quad[3] = k++;
      }
    }

    void operator()(Polygon_indices& indices) const
    {
      Index_type k(0);
      for (auto& polygon: indices) {
        for (auto &index: polygon) index = k++;
      }
    }

    void operator()(Flat_indices& indices) const
    {
      Index_type k(0);
      for (auto& index: indices) index = k++;
    }
  };

  /*! Assign a facet indices structure with a sequence of indices starting at 0.
   */
  void sequence_facet_indices(Facet_indices& indices);

  /*! Set the value of an entry in a facet indices structure. */
  void set_index_facet_indices(Facet_indices& indices, size_t address,
                               Index_type value);

  /*! Get the value of an entry in a facet indices structure. */
  Index_type get_index_facet_indices(const Facet_indices& indices,
                                     size_t address);

  /*! Clear */
  void clear_facet_indices(Facet_indices& indices);

  /*! Equal */
  Boolean equal_facet_indices(const Facet_indices& target,
                              const Facet_indices& source);

  /*! Determine whether the given index vector is empty. */
  Boolean empty_facet_indices(const Facet_indices& indices);

  /*! Obtain the number of indices.
   */
  size_t size_facet_indices(const Facet_indices& indices);

  /*! */
  class Facet_indices_const_iterator {
  public:
    boost::variant<Triangle_indices::const_iterator,
                   Quad_indices::const_iterator,
                   Polygon_indices::const_iterator,
                   Flat_indices::const_iterator> m_it;

    std::array<Index_type, 3>::const_iterator m_tri_it;
    std::array<Index_type, 4>::const_iterator m_quad_it;
    std::vector<Index_type>::const_iterator m_poly_it;

    Triangle_indices::const_iterator m_tri_end;
    Quad_indices::const_iterator m_quad_end;
    Polygon_indices::const_iterator m_poly_end;

  public:
    /*! Construct default. */
    Facet_indices_const_iterator();

    /*! Construct a begin iterator from triangle iterators.
     */
    Facet_indices_const_iterator(Triangle_indices::const_iterator begin,
                                 Triangle_indices::const_iterator end);

    /*! Construct a begin iterator from quad iterators.
     */
    Facet_indices_const_iterator(Quad_indices::const_iterator begin,
                                 Quad_indices::const_iterator end);

    /*! Construct a begin iterator from polygon iterators.
     */
    Facet_indices_const_iterator(Polygon_indices::const_iterator begin,
                                 Polygon_indices::const_iterator end);

    /*! Construct a past-the-end iterator from a triangle past-the-end iterator.
     */
    Facet_indices_const_iterator(Triangle_indices::const_iterator end);

    /*! Construct a past-the-end iterator from a quad past-the-end iterator.
     */
    Facet_indices_const_iterator(Quad_indices::const_iterator end);

    /*! Construct a past-the-end iterator from a polygon past-the-end iterator.
     */
    Facet_indices_const_iterator(Polygon_indices::const_iterator end);

    /*! Construct an iterator from a flat iterator. */
    Facet_indices_const_iterator(Flat_indices::const_iterator begin);

    /*! Obtain a reference. */
    const Index_type& operator*();

    /*! Post increment. */
    Facet_indices_const_iterator operator++();

    /*! Test equality with amother iterator. */
    Boolean operator==(Facet_indices_const_iterator other);

    /*! Test inequality amother iterator. */
    Boolean operator!=(Facet_indices_const_iterator other)
    { return ! operator==(other); }
  };

  /*! Reference visitor. */
  class Reference_visitor : public boost::static_visitor<const Index_type&> {
  private:
    Facet_indices_const_iterator& m_it;

  public:
    Reference_visitor(Facet_indices_const_iterator& it) : m_it(it) {}

    const Index_type& operator()(Triangle_indices::const_iterator& it) const
    { return *(m_it.m_tri_it); }

    const Index_type& operator()(Quad_indices::const_iterator& it) const
    { return *(m_it.m_quad_it); }

    const Index_type& operator()(Polygon_indices::const_iterator& it) const
    { return *(m_it.m_poly_it); }

    const Index_type& operator()(Flat_indices::const_iterator& it) const
    { return *it; }
  };

  /*! Post increment visitor. */
  class Incerement_visitor : public boost::static_visitor<> {
  private:
    Facet_indices_const_iterator& m_it;

  public:
    Incerement_visitor(Facet_indices_const_iterator& it) : m_it(it) {}

    void operator()(Triangle_indices::const_iterator& it)
    {
      ++(m_it.m_tri_it);
      if (m_it.m_tri_it == it->end()) {
        ++it;
        if (it != m_it.m_tri_end) m_it.m_tri_it = it->begin();
      }
    }

    void operator()(Quad_indices::const_iterator& it)
    {
      ++(m_it.m_quad_it);
      if (m_it.m_quad_it == it->end()) {
        ++it;
        if (it != m_it.m_quad_end) m_it.m_quad_it = it->begin();
      }
    }

    void operator()(Polygon_indices::const_iterator& it)
    {
      ++(m_it.m_poly_it);
      if (m_it.m_poly_it == it->end()) {
        ++it;
        if (it != m_it.m_poly_end) m_it.m_poly_it = it->begin();
      }
    }

    void operator()(Flat_indices::const_iterator& it) { ++it; }
  };

  /*! Compare visitor. */
  class Compare_visitor : public boost::static_visitor<Boolean> {
  private:
    Facet_indices_const_iterator& m_it;
    Facet_indices_const_iterator& m_other;

  public:
    Compare_visitor(Facet_indices_const_iterator& it,
                    Facet_indices_const_iterator& other) :
      m_it(it), m_other(other) {}

    template <typename T, typename U>
    Boolean operator()(const T&, const U&) const { return false; }

    Boolean operator()(Triangle_indices::const_iterator& it,
                       Triangle_indices::const_iterator& other)
    {
      return (it == other) &&
        ((m_it.m_tri_it == m_other.m_tri_it) || (it == m_it.m_tri_end));
    }

    Boolean operator()(Quad_indices::const_iterator& it,
                       Quad_indices::const_iterator& other)
    {
      return (it == other) &&
        ((m_it.m_quad_it == m_other.m_quad_it) || (it == m_it.m_quad_end));
    }

    Boolean operator()(Polygon_indices::const_iterator& it,
                       Polygon_indices::const_iterator& other)
    {
      return (it == other) &&
        ((m_it.m_poly_it == m_other.m_poly_it) || (it == m_it.m_poly_end));
    }

    Boolean operator()(Flat_indices::const_iterator& it,
                       Flat_indices::const_iterator& other)
    { return it == other; }
  };

  /*! Begin facet indices visitor */
  class Begin_facet_indices_visitor :
    public boost::static_visitor<Facet_indices_const_iterator> {
  public:
    Facet_indices_const_iterator operator()(const Triangle_indices& indices)
      const
    { return Facet_indices_const_iterator(indices.begin(), indices.end()); }

   Facet_indices_const_iterator operator()(const Quad_indices& indices) const
    { return Facet_indices_const_iterator(indices.begin(), indices.end()); }

   Facet_indices_const_iterator operator()(const Polygon_indices& indices) const
    { return Facet_indices_const_iterator(indices.begin(), indices.end()); }

   Facet_indices_const_iterator operator()(const Flat_indices& indices) const
    { return Facet_indices_const_iterator(indices.begin()); }
  };

  /*! End facet indices visitor */
  class End_facet_indices_visitor :
    public boost::static_visitor<Facet_indices_const_iterator>
  {
  public:
    template <typename Indices>
    Facet_indices_const_iterator operator()(const Indices& indices) const
    { return Facet_indices_const_iterator(indices.end()); }
  };

  /*! Obtain a begin iterator of a facet indices structure. */
  Facet_indices_const_iterator begin_facet_indices(const Facet_indices& indices);

  /*! Obtain a past-the-end iterator of a facet indices structure. */
  Facet_indices_const_iterator end_facet_indices(const Facet_indices& indices);

  /*! Clean an indices from a facet indices structure. */
  void clean_indices(std::vector<Int32>& indices,
                     const Facet_indices& source);

  /*! Assign the facet coordinate indices with the reverse of given indices.
   * \param[in] indices the input indices to reverse.
   */
  void reverse_facet_coord_indices(const Facet_indices& indices);

  /*! Reverse the facet coordinate indices.
   */
  void reverse_facet_coord_indices();
  //@}

  /*! Assign the coordinate indices with the reverse of given indices.
   * \param[i] indices the indices to reverse.
   */
  void reverse_coord_indices(const std::vector<Int32>& indices);

  /*! Set the coordinate indices from a range of elements that counts
   * the number of vertices per primitive.
   * \param begin The begin iterator of the range of numbers.
   * \param end The past0the-end iterator of the range of numbers.
   */
  template <typename InputIterator>
  void set_coord_indices_from_range(InputIterator begin, InputIterator end);

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates();

  /// \name Obtain empty index vectors
  //@{
  /*! Obtain the empty facet coordinate indices. */
  Facet_indices& get_empty_facet_coord_indices();

  /*! Obtain the empty facet normal indices. */
  Facet_indices& get_empty_facet_normal_indices();

  /*! Obtain the empty facet color indices. */
  Facet_indices& get_empty_facet_color_indices();

  /*! Obtain the empty facet texture-coordinate indices. */
  Facet_indices& get_empty_facet_tex_coord_indices();

  /*! Obtain the empty triangle coordinate indices. */
  Triangle_indices& get_empty_triangle_coord_indices();

  /*! Obtain the empty quad coordinate indices. */
  Quad_indices& get_empty_quad_coord_indices();

  /*! Obtain the empty polygon coordinate indices. */
  Polygon_indices& get_empty_polygon_coord_indices();

  /*! Obtain the empty flat coordinate indices. */
  Flat_indices& get_empty_flat_coord_indices();

  /*! Obtain the empty triangle normal indices. */
  Triangle_indices& get_empty_triangle_normal_indices();

  /*! Obtain the empty quad normal indices. */
  Quad_indices& get_empty_quad_normal_indices();

  /*! Obtain the empty polygon normal indices. */
  Polygon_indices& get_empty_polygon_normal_indices();

  /*! Obtain the empty flat normal indices. */
  Flat_indices& get_empty_flat_normal_indices();

  /*! Obtain the empty triangle color indices. */
  Triangle_indices& get_empty_triangle_color_indices();

  /*! Obtain the empty quad color indices. */
  Quad_indices& get_empty_quad_color_indices();

  /*! Obtain the empty polygon color indices. */
  Polygon_indices& get_empty_polygon_color_indices();

  /*! Obtain the empty flat color indices. */
  Flat_indices& get_empty_flat_color_indices();

  /*! Obtain the empty triangle texture-coordinate indices. */
  Triangle_indices& get_empty_triangle_tex_coord_indices();

  /*! Obtain the empty quad texture-coordinate indices. */
  Quad_indices& get_empty_quad_tex_coord_indices();

  /*! Obtain the empty polygon texture-coordinate indices. */
  Polygon_indices& get_empty_polygon_tex_coord_indices();

  /*! Obtain the empty flat texture-coordinate indices. */
  Flat_indices& get_empty_flat_tex_coord_indices();
  //@}

  /// \name Obtain const index vectors
  //@{
  /*! Obtain the triangle coordinate indices. */
  const Triangle_indices& triangle_coord_indices() const;

  /*! Obtain the quad coordinate indices. */
  const Quad_indices& quad_coord_indices() const;

  /*! Obtain the polygon coordinate indices. */
  const Polygon_indices& polygon_coord_indices() const;

  /*! Obtain the triangle normal indices. */
  const Triangle_indices& triangle_normal_indices() const;

  /*! Obtain the quad normal indices. */
  const Quad_indices& quad_normal_indices() const;

  /*! Obtain the polygon normal indices. */
  const Polygon_indices& polygon_normal_indices() const;

  /*! Obtain the flat normal indices. */
  const Flat_indices& flat_normal_indices() const;

  /*! Obtain the triangle color indices. */
  const Triangle_indices& triangle_color_indices() const;

  /*! Obtain the quad color indices. */
  const Quad_indices& quad_color_indices() const;

  /*! Obtain the polygon color indices. */
  const Polygon_indices& polygon_color_indices() const;

  /*! Obtain the flat color indices. */
  const Flat_indices& flat_color_indices() const;

  /*! Obtain the triangle texture-coordinate indices. */
  const Triangle_indices& triangle_tex_coord_indices() const;

  /*! Obtain the quad texture-coordinate indices. */
  const Quad_indices& quad_tex_coord_indices() const;

  /*! Obtain the polygon texture-coordinate indices. */
  const Polygon_indices& polygon_tex_coord_indices() const;
  //@}

protected:
  /*! Indicates the direction of the vertices in a face. The default is
   * counter clock wise.
   */
  Boolean m_is_ccw;

  /*! Indicates wether the object is a solid. If it is not a solid we need
   * to draw both sides of the polygons.
   */
  Boolean m_is_solid;

  /*! Indicates whether all faces are convex. */
  Boolean m_is_convex;

  /*! The crease angle. */
  Float m_crease_angle;

  /*! The polygon offset factor. */
  Float m_polygon_offset_factor;

  /*! An array of indices into the coordinate array. */
  Facet_indices m_facet_coord_indices;

  /*! An array of indices into the texture-coordinate array. */
  Facet_indices m_facet_normal_indices;

  /*! An array of indices into the color array. */
  Facet_indices m_facet_color_indices;

  /*! An array of indices into the normal array. */
  Facet_indices m_facet_tex_coord_indices;

  /*! Indicates whether the facet coordinate indices should be cleaned. */
  Boolean m_dirty_facet_coord_indices;

  /*! Indicates whether the facet normal indices should be cleaned. */
  Boolean m_dirty_facet_normal_indices;

  /*! Indicates whether the facet color indices should be cleaned. */
  Boolean m_dirty_facet_color_indices;

  /*! Indicates whether the facet texture-coordinate indices should be
   * cleaned.
   */
  Boolean m_dirty_facet_tex_coord_indices;

  /*! Indicates whether the coordinate indices should be cleaned. */
  Boolean m_dirty_coord_indices;

  /*! Indicates whether the normal indices should be cleaned. */
  Boolean m_dirty_normal_indices;

  /*! Indicates whether the color indices should be cleaned. */
  Boolean m_dirty_color_indices;

  /*! Indicates whether the texture-coordinate indices should be cleaned. */
  Boolean m_dirty_tex_coord_indices;

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates(std::vector<Int32>& indices);

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates(Facet_indices& indices);

  /*! Export a triangular facet.
   * \param[in] p1 the point of the first vertex.
   * \param[in] p2 the point of the second vertex.
   * \param[in] p3 the point of the third vertex.
   * \param[in] stl_formater the formatter.
   */
  void write_facet(const Vector3f& p1, const Vector3f& p2,
                   const Vector3f& p3, Formatter* formatter);

  /*! Export a quadrilateral facet.
   * \param[in] p1 the point of the first vertex.
   * \param[in] p2 the point of the second vertex.
   * \param[in] p3 the point of the third vertex.
   * \param[in] p4 the point of the forth vertex.
   * \param[in] stl_formater the formatter.
   */
  void write_facet(const Vector3f& p1, const Vector3f& p2,
                   const Vector3f& p3, const Vector3f& p4,
                   Formatter* formatter);

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default value. */
  static const Boolean s_def_is_ccw;
  static const Boolean s_def_is_solid;
  static const Boolean s_def_is_convex;
  static const Float s_def_crease_angle;
  static const Float s_def_polygon_offset_factor;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief sets the counter-clockwise flag. */
inline void Mesh_set::set_ccw(Boolean ccw) { m_is_ccw = ccw; }

//! \brief returns true if the surface polygons are counter-clockwise.
inline Boolean Mesh_set::is_ccw() const { return m_is_ccw; }

//! \brief sets the solid flag.
inline void Mesh_set::set_solid(Boolean solid) { m_is_solid = solid; }

//! \brief returns true if the polyhedron is solid, and false otherwise.
inline Boolean Mesh_set::is_solid() const { return m_is_solid; }

//! \brief sets the convex flag.
inline void Mesh_set::set_convex(Boolean convex) { m_is_convex = convex; }

//! \brief returns true if all faces of the polyhedron are convex.
inline Boolean Mesh_set::is_convex() const { return m_is_convex; }

//! \brief sets the crease angle of the polyhedron.
inline void Mesh_set::set_crease_angle(Float crease_angle)
{ m_crease_angle = crease_angle; }

//! \brief obtains the creas_angle.
inline Float Mesh_set::get_crease_angle() const { return m_crease_angle; }

//! \brief determines whether the facet coordinate indices is dirty.
inline Boolean Mesh_set::is_dirty_facet_coord_indices() const
{ return m_dirty_facet_coord_indices; }

//! \brief determines whether the facet normal indices is dirty.
inline Boolean Mesh_set::is_dirty_facet_normal_indices() const
{ return m_dirty_facet_normal_indices; }

//! \brief determines whether the facet color indices is dirty.
inline Boolean Mesh_set::is_dirty_facet_color_indices() const
{ return m_dirty_facet_color_indices; }

//! \brief determines whether the facet texture-coordinate indices is dirty.
inline Boolean Mesh_set::is_dirty_facet_tex_coord_indices() const
{ return m_dirty_facet_tex_coord_indices; }

/*! \brief set the coordinate indices from a range of elements that counts
 * the number of vertices per primitive.
 */
template <typename InputIterator>
void Mesh_set::set_coord_indices_from_range(InputIterator begin,
                                            InputIterator end)
{
  Boolean is_tris(true);
  Boolean is_quads(true);
  Uint size = 0;
  Uint num = 0;
  for (auto it = begin; it != end; ++it) {
    ++num;
    size += *it;
    if (*it == 3) is_quads = false;
    else if (*it == 4) is_tris = false;
    else {
      is_quads = false;
      is_tris = false;
    }
  }
  SGAL_assertion(is_tris && is_quads);

  if (is_tris) {
    set_primitive_type(Geo_set::PT_TRIANGLES);
    auto& tris = get_empty_triangle_coord_indices();
    tris.resize(num);
    Index_type j(0);
    for (size_t i = 0; i < num; ++i) {
      tris[i][0] = j++;
      tris[i][1] = j++;
      tris[i][2] = j++;
    }
  }
  else if (is_quads) {
    set_primitive_type(Geo_set::PT_TRIANGLES);
    auto& quads = get_empty_quad_coord_indices();
    quads.resize(num);
    Index_type j(0);
    for (size_t i = 0; i < num; ++i) {
      quads[i][0] = j++;
      quads[i][1] = j++;
      quads[i][2] = j++;
      quads[i][3] = j++;
    }
  }
  else {
    set_primitive_type(Geo_set::PT_POLYGONS);
    auto& polys = get_empty_polygon_coord_indices();
    polys.resize(num);
    Index_type k(0);
    auto it = begin;
    for (size_t i = 0; i < num; ++i, ++it) {
      for (size_t j = 0; j < *it; ++j) polys[i][j] = k++;
    }
  }
  set_num_primitives(num);
  m_dirty_facet_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief determines whether the coordinate indices is dirty.
inline Boolean Mesh_set::is_dirty_coord_indices() const
{ return m_dirty_coord_indices; }

//! \brief determines whether the normal indices is dirty.
inline Boolean Mesh_set::is_dirty_normal_indices() const
{ return m_dirty_normal_indices; }

//! \brief determines whether the color indices is dirty.
inline Boolean Mesh_set::is_dirty_color_indices() const
{ return m_dirty_color_indices; }

//! \brief determines whether the texture-coordinate indices is dirty.
inline Boolean Mesh_set::is_dirty_tex_coord_indices() const
{ return m_dirty_tex_coord_indices; }

//! \brief obtains the triangle coordinate indices.
inline const Triangle_indices& Mesh_set::triangle_coord_indices() const
{ return boost::get<Triangle_indices>(m_facet_coord_indices); }

//! \brief obtains the quad coordinate indices.
inline const Quad_indices& Mesh_set::quad_coord_indices() const
{ return boost::get<Quad_indices>(m_facet_coord_indices); }

//! \brief obtains the polygon coordinate indices.
inline const Polygon_indices& Mesh_set::polygon_coord_indices() const
{ return boost::get<Polygon_indices>(m_facet_coord_indices); }

//! \brief obtains the triangle normal indices.
inline const Triangle_indices& Mesh_set::triangle_normal_indices() const
{ return boost::get<Triangle_indices>(m_facet_normal_indices); }

//! \brief obtains the quad normal indices.
inline const Quad_indices& Mesh_set::quad_normal_indices() const
{ return boost::get<Quad_indices>(m_facet_normal_indices); }

//! \brief obtains the polygon normal indices.
inline const Polygon_indices& Mesh_set::polygon_normal_indices() const
{ return boost::get<Polygon_indices>(m_facet_normal_indices); }

//! \brief Obtain the flat normal indices.
inline const Flat_indices& Mesh_set::flat_normal_indices() const
{ return boost::get<Flat_indices>(m_facet_normal_indices); }

//! \brief obtains the triangle color indices.
inline const Triangle_indices& Mesh_set::triangle_color_indices() const
{ return boost::get<Triangle_indices>(m_facet_color_indices); }

//! \brief obtains the quad normal indices.
inline const Quad_indices& Mesh_set::quad_color_indices() const
{ return boost::get<Quad_indices>(m_facet_color_indices); }

//! \brief obtains the polygon normal indices.
inline const Polygon_indices& Mesh_set::polygon_color_indices() const
{ return boost::get<Polygon_indices>(m_facet_color_indices); }

//! \brief obtains the flat color indices.
inline const Flat_indices& Mesh_set::flat_color_indices() const
{ return boost::get<Flat_indices>(m_facet_color_indices); }

//! \brief obtains the triangle texture-coordinate indices.
inline const Triangle_indices& Mesh_set::triangle_tex_coord_indices() const
{ return boost::get<Triangle_indices>(m_facet_tex_coord_indices); }

//! \brief obtains the quad texture-coordinate indices.
inline const Quad_indices& Mesh_set::quad_tex_coord_indices() const
{ return boost::get<Quad_indices>(m_facet_tex_coord_indices); }

//! \brief obtains the polygon texture-coordinate indices.
inline const Polygon_indices& Mesh_set::polygon_tex_coord_indices() const
{ return boost::get<Polygon_indices>(m_facet_tex_coord_indices); }

//! \brief constructs default.
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator() : m_it() {}

//! \brief constructs a begin iterator from triangle iterators.
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Triangle_indices::const_iterator it,
                             Triangle_indices::const_iterator end) :
  m_it(it), m_tri_it(it->begin()), m_tri_end(end) {}

//! \brief constructs a begin iterator from quad iterators.
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Quad_indices::const_iterator it,
                             Quad_indices::const_iterator end) :
  m_it(it), m_quad_it(it->begin()), m_quad_end(end) {}

//! \brief constructs a begin iterator from polygon iterators.
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Polygon_indices::const_iterator it,
                             Polygon_indices::const_iterator end) :
  m_it(it), m_poly_it(it->begin()), m_poly_end(end) {}

/*! \brief constructs a past-the-end iterator from a triangle past-the-end
 * iterator.
 */
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Triangle_indices::const_iterator end) :
  m_it(end), m_tri_end(end) {}

//! \brief constructs a past-the-end iterator from a quad past-the-end iterator.
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Quad_indices::const_iterator end) :
  m_it(end), m_quad_end(end) {}

/*! \brief constructs a past-the-end iterator from a polygon past-the-end
 * iterator.
 */
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Polygon_indices::const_iterator end) :
  m_it(end), m_poly_end(end) {}

//! \brief construct an iterator from a flat iterator.
inline Mesh_set::Facet_indices_const_iterator::
Facet_indices_const_iterator(Flat_indices::const_iterator it) : m_it(it) {}

inline const Index_type& Mesh_set::Facet_indices_const_iterator::operator*()
{
  Reference_visitor visitor(*this);
  return boost::apply_visitor(visitor, m_it);
}

inline Mesh_set::Facet_indices_const_iterator
Mesh_set::Facet_indices_const_iterator::operator++()
{
  Incerement_visitor visitor(*this);
  boost::apply_visitor(visitor, m_it);
  return *this;
}

inline Boolean Mesh_set::Facet_indices_const_iterator::
operator==(Facet_indices_const_iterator other)
{
  Compare_visitor visitor(*this, other);
  return boost::apply_visitor(visitor, m_it, other.m_it);
}

SGAL_END_NAMESPACE

#endif
