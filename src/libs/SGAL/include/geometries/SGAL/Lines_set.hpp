// Copyright (c) 2018 Israel.
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

#ifndef SGAL_LINES_SET_HPP
#define SGAL_LINES_SET_HPP

#include <vector>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
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

class SGAL_SGAL_DECL Lines_set : public Geo_set {
public:
  enum {
    FIRST = Geo_set::LAST - 1,
    LAST
  };

  /*! Construct.
   */
  Lines_set(Boolean proto = false);

  /*! Destruct. */
  virtual ~Lines_set();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  /*! Draw the mesh conditionaly.
   */
  virtual void draw(Draw_action* action);

  /*! Draw the representation.
   */
  virtual void draw_lines(Draw_action* action) = 0;

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Obtain the bounding box.
   */
  virtual Bounding_box bounding_box();

  /// \name Field Getters and Setters
  //@{
  //@}

  /// \name Index-Arrays Getters
  //@{
  /*! Obtain the coordinate indices.
   * \return the coordinate indices.
   */
  virtual Int32_array& get_coord_indices();

  /*! Obtain the normal indices.
   * \return the normal indices.
   */
  virtual Int32_array& get_normal_indices();

  /*! Obtain the color indices.
   * \return the color indices.
   */
  virtual Int32_array& get_color_indices();

  /*! Obtain the texture-coordinate indices.
   * \return the texture-coordinate indices.
   */
  virtual Int32_array& get_tex_coord_indices();
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

  /// \name Lines Indices Change Reactors
  //@{
  /*! Respond to a change in the lines coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void lines_coord_indices_changed();

  /*! Respond to a change in the lines normal indices.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void lines_normal_indices_changed();

  /*! Respond to a change in the lines color indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void lines_color_indices_changed();

  /*! Respond to a change in the lines texture-coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void lines_tex_coord_indices_changed();
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

  /// \name Obtain const vectors of lines indices
  //@{
  /*! Obtain the lines coordinate indices.
   */
  const Lines_indices& get_lines_coord_indices();

  /*! Obtain the lines normal indices.
   */
  const Lines_indices& get_lines_normal_indices();

  /*! Obtain the lines color indices.
   */
  const Lines_indices& get_lines_color_indices();

  /*! Obtain the lines texture-coordinate indices.
   */
  const Lines_indices& get_lines_tex_coord_indices();
  //@}

  /// \name Lines Index Arrays
  //@{
  /*! Set the lines coordinate indices.
   */
  void set_lines_coord_indices(const Lines_indices& indices);

  /*! Set the lines normal indices.
   */
  void set_lines_normal_indices(const Lines_indices& indices);

  /*! Set the lines color indices.
   */
  void set_lines_color_indices(const Lines_indices& indices);

  /*! Set the lines texture-coordinate indices.
   */
  void set_lines_tex_coord_indices(const Lines_indices& indices);

  /*! Set the lines coordinate indices and dispose the input indices.
   */
  void set_lines_coord_indices(const Lines_indices&& indices);

  /*! Set the lines normal indices and dispose the input indices.
   */
  void set_lines_normal_indices(const Lines_indices&& indices);

  /*! Set the lines color indices and dispose the input indices.
   */
  void set_lines_color_indices(const Lines_indices&& indices);

  /*! Set the lines texture-coordinate indices and dispose the input indices.
   */
  void set_lines_tex_coord_indices(const Lines_indices&& indices);

  /*! Clean (validate) the lines coordinate indices.
   * In case of lines, remove the '-1' end-of-polyline indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_lines_coord_indices();

  /*! Clean (validate) the lines normal indices.
   * In case of lines, remove the '-1' end-of-polyline indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_lines_normal_indices();

  /*! Clean (validate) the lines color indices.
   * In case of lines, remove the '-1' end-of-polyline indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_lines_color_indices();

  /*! Clean (validate) the lines texture-coordinate indices.
   * In case of lines, remove the '-1' end-of-polyline indication
   * from the indices. This operation changes the structure of the
   * indices, and must be reflected in the drawing routines.
   */
  virtual void clean_lines_tex_coord_indices();

  /*! Clear (invalidate) the lines indicess, i.e., coordinate indices,
   * normal indices, color indices, and texture-coordinate indicess.
   */
  void clear_lines_indices();

  /*! Clear (invalidate) the lines cordinate indices.
   */
  void clear_lines_coord_indices();

  /*! Clear (invalidate) the lines normal indices.
   */
  void clear_lines_normal_indices();

  /*! Clear (invalidate) the lines color indices.
   */
  void clear_lines_color_indices();

  /*! Clear (invalidate) the lines texture-cordinate indices.
   */
  void clear_lines_tex_coord_indices();

  /*! Determine whether the coordinate indices has been invalidated
   * (is dirty) and thus must be cleaned.
   */
  Boolean is_dirty_lines_coord_indices() const;

  /*! Determine whether the normal indices has been invalidated (is dirty)
   * thus must be cleaned.
   */
  Boolean is_dirty_lines_normal_indices() const;

  /*! Determine whether the color indices has been invalidated (is dirty)
   * and thus must be cleaned.
   */
  Boolean is_dirty_lines_color_indices() const;

  /*! Determine whether the texture-coordinate indices has been
   * invalidated (is dirty) and thus must be cleaned.
   */
  Boolean is_dirty_lines_tex_coord_indices() const;

  /*! Initialize the lines cordinate indices.
   */
  void init_lines_coord_indices();

  /*! Initialize the lines normal indices.
   */
  void init_lines_normal_indices();

  /*! Initialize the lines color indices.
   */
  void init_lines_color_indices();

  /*! Initialize the lines texture-cordinate indices.
   */
  void init_lines_tex_coord_indices();

  /*! Resize a lines indices structure with the same size as another.
   */
  void resize_lines_indices(Lines_indices& target, const Lines_indices& source);

  /*! Sequence lines indices visitor */
  class Sequence_lines_indices_visitor : public boost::static_visitor<> {
  public:
    void operator()(Line_indices& indices) const
    {
      Index_type k(0);
      for (auto& tri: indices) {
        tri[0] = k++;
        tri[1] = k++;
      }
    }

    void operator()(Polyline_indices& indices) const
    {
      Index_type k(0);
      for (auto& polyline: indices) {
        for (auto &index: polyline) index = k++;
      }
    }

    void operator()(Flat_indices& indices) const
    {
      Index_type k(0);
      for (auto& index: indices) index = k++;
    }
  };

  /*! Assign a lines indices structure with a sequence of indices starting at 0.
   */
  void sequence_lines_indices(Lines_indices& indices);

  /*! Set the value of an entry in a lines indices structure. */
  void set_index_lines_indices(Lines_indices& indices, size_t address,
                               Index_type value);

  /*! Get the value of an entry in a lines indices structure. */
  Index_type get_index_lines_indices(const Lines_indices& indices,
                                     size_t address);

  /*! Clear */
  void clear_lines_indices(Lines_indices& indices);

  /*! Equal */
  Boolean equal_lines_indices(const Lines_indices& target,
                              const Lines_indices& source);

  /*! Determine whether the given index vector is empty. */
  Boolean empty_lines_indices(const Lines_indices& indices);

  /*! Obtain the number of indices.
   */
  size_t size_lines_indices(const Lines_indices& indices);

  /*! */
  class Lines_indices_const_iterator {
  public:
    boost::variant<Line_indices::const_iterator,
                   Polyline_indices::const_iterator,
                   Flat_indices::const_iterator> m_it;

    std::array<Index_type, 3>::const_iterator m_tri_it;
    std::vector<Index_type>::const_iterator m_poly_it;

    Line_indices::const_iterator m_tri_end;
    Polyline_indices::const_iterator m_poly_end;

  public:
    /*! Construct default. */
    Lines_indices_const_iterator();

    /*! Construct a begin iterator from lines iterators.
     */
    Lines_indices_const_iterator(Line_indices::const_iterator begin,
                                 Line_indices::const_iterator end);

    /*! Construct a begin iterator from polyline iterators.
     */
    Lines_indices_const_iterator(Polyline_indices::const_iterator begin,
                                 Polyline_indices::const_iterator end);

    /*! Construct a past-the-end iterator from a lines past-the-end iterator.
     */
    Lines_indices_const_iterator(Line_indices::const_iterator end);

    /*! Construct a past-the-end iterator from a polyline past-the-end iterator.
     */
    Lines_indices_const_iterator(Polyline_indices::const_iterator end);

    /*! Construct an iterator from a flat iterator. */
    Lines_indices_const_iterator(Flat_indices::const_iterator begin);

    /*! Obtain a reference. */
    const Index_type& operator*();

    /*! Post increment. */
    Lines_indices_const_iterator operator++();

    /*! Test equality with amother iterator. */
    Boolean operator==(Lines_indices_const_iterator other);

    /*! Test inequality amother iterator. */
    Boolean operator!=(Lines_indices_const_iterator other)
    { return ! operator==(other); }
  };

  /*! Reference visitor. */
  class Reference_visitor : public boost::static_visitor<const Index_type&> {
  private:
    Lines_indices_const_iterator& m_it;

  public:
    Reference_visitor(Lines_indices_const_iterator& it) : m_it(it) {}

    const Index_type& operator()(Line_indices::const_iterator& it) const
    { return *(m_it.m_tri_it); }

    const Index_type& operator()(Polyline_indices::const_iterator& it) const
    { return *(m_it.m_poly_it); }

    const Index_type& operator()(Flat_indices::const_iterator& it) const
    { return *it; }
  };

  /*! Post increment visitor. */
  class Incerement_visitor : public boost::static_visitor<> {
  private:
    Lines_indices_const_iterator& m_it;

  public:
    Incerement_visitor(Lines_indices_const_iterator& it) : m_it(it) {}

    void operator()(Line_indices::const_iterator& it)
    {
      ++(m_it.m_tri_it);
      if (m_it.m_tri_it == it->end()) {
        ++it;
        if (it != m_it.m_tri_end) m_it.m_tri_it = it->begin();
      }
    }

    void operator()(Polyline_indices::const_iterator& it)
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
    Lines_indices_const_iterator& m_it;
    Lines_indices_const_iterator& m_other;

  public:
    Compare_visitor(Lines_indices_const_iterator& it,
                    Lines_indices_const_iterator& other) :
      m_it(it), m_other(other) {}

    template <typename T, typename U>
    Boolean operator()(const T&, const U&) const { return false; }

    Boolean operator()(Line_indices::const_iterator& it,
                       Line_indices::const_iterator& other)
    {
      return (it == other) &&
        ((m_it.m_tri_it == m_other.m_tri_it) || (it == m_it.m_tri_end));
    }

    Boolean operator()(Polyline_indices::const_iterator& it,
                       Polyline_indices::const_iterator& other)
    {
      return (it == other) &&
        ((m_it.m_poly_it == m_other.m_poly_it) || (it == m_it.m_poly_end));
    }

    Boolean operator()(Flat_indices::const_iterator& it,
                       Flat_indices::const_iterator& other)
    { return it == other; }
  };

  /*! Begin lines indices visitor */
  class Begin_lines_indices_visitor :
    public boost::static_visitor<Lines_indices_const_iterator> {
  public:
    Lines_indices_const_iterator operator()(const Line_indices& indices)
      const
    { return Lines_indices_const_iterator(indices.begin(), indices.end()); }

   Lines_indices_const_iterator operator()(const Polyline_indices& indices) const
    { return Lines_indices_const_iterator(indices.begin(), indices.end()); }

   Lines_indices_const_iterator operator()(const Flat_indices& indices) const
    { return Lines_indices_const_iterator(indices.begin()); }
  };

  /*! End lines indices visitor */
  class End_lines_indices_visitor :
    public boost::static_visitor<Lines_indices_const_iterator>
  {
  public:
    template <typename Indices>
    Lines_indices_const_iterator operator()(const Indices& indices) const
    { return Lines_indices_const_iterator(indices.end()); }
  };

  /*! Obtain a begin iterator of a lines indices structure. */
  Lines_indices_const_iterator begin_lines_indices(const Lines_indices& indices);

  /*! Obtain a past-the-end iterator of a lines indices structure. */
  Lines_indices_const_iterator end_lines_indices(const Lines_indices& indices);

  /*! Clean an indices from a lines indices structure. */
  void clean_indices(Int32_array& indices,
                     const Lines_indices& source);

  /*! Assign the lines coordinate indices with the reverse of given indices.
   * \param[in] indices the input indices to reverse.
   */
  void reverse_lines_coord_indices(const Lines_indices& indices);

  /*! Reverse the lines coordinate indices.
   */
  void reverse_lines_coord_indices();
  //@}

  /*! Assign the coordinate indices with the reverse of given indices.
   * \param[i] indices the indices to reverse.
   */
  void reverse_coord_indices(const Int32_array& indices);

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
  /*! Obtain the empty lines coordinate indices. */
  Lines_indices& get_empty_lines_coord_indices();

  /*! Obtain the empty lines normal indices. */
  Lines_indices& get_empty_lines_normal_indices();

  /*! Obtain the empty lines color indices. */
  Lines_indices& get_empty_lines_color_indices();

  /*! Obtain the empty lines texture-coordinate indices. */
  Lines_indices& get_empty_lines_tex_coord_indices();

  /*! Obtain the empty line coordinate indices. */
  Line_indices& get_empty_line_coord_indices();

  /*! Obtain the empty polyline coordinate indices. */
  Polyline_indices& get_empty_polyline_coord_indices();

  /*! Obtain the empty flat coordinate indices. */
  Flat_indices& get_empty_flat_coord_indices();

  /*! Obtain the empty line normal indices. */
  Line_indices& get_empty_line_normal_indices();

  /*! Obtain the empty polyline normal indices. */
  Polyline_indices& get_empty_polyline_normal_indices();

  /*! Obtain the empty flat normal indices. */
  Flat_indices& get_empty_flat_normal_indices();

  /*! Obtain the empty line color indices. */
  Line_indices& get_empty_line_color_indices();

  /*! Obtain the empty polyline color indices. */
  Polyline_indices& get_empty_polyline_color_indices();

  /*! Obtain the empty flat color indices. */
  Flat_indices& get_empty_flat_color_indices();

  /*! Obtain the empty line texture-coordinate indices. */
  Line_indices& get_empty_line_tex_coord_indices();

  /*! Obtain the empty polyline texture-coordinate indices. */
  Polyline_indices& get_empty_polyline_tex_coord_indices();

  /*! Obtain the empty flat texture-coordinate indices. */
  Flat_indices& get_empty_flat_tex_coord_indices();
  //@}

  /// \name Obtain const index vectors
  //@{
  /*! Obtain the lines coordinate indices. */
  const Line_indices& line_coord_indices() const;

  /*! Obtain the polyline coordinate indices. */
  const Polyline_indices& polyline_coord_indices() const;

  /*! Obtain the lines normal indices. */
  const Line_indices& line_normal_indices() const;

  /*! Obtain the polyline normal indices. */
  const Polyline_indices& polyline_normal_indices() const;

  /*! Obtain the flat normal indices. */
  const Flat_indices& flat_normal_indices() const;

  /*! Obtain the lines color indices. */
  const Line_indices& line_color_indices() const;

  /*! Obtain the polyline color indices. */
  const Polyline_indices& polyline_color_indices() const;

  /*! Obtain the flat color indices. */
  const Flat_indices& flat_color_indices() const;

  /*! Obtain the lines texture-coordinate indices. */
  const Line_indices& line_tex_coord_indices() const;

  /*! Obtain the polyline texture-coordinate indices. */
  const Polyline_indices& polyline_tex_coord_indices() const;
  //@}

protected:
  /*! An array of indices into the coordinate array. */
  Lines_indices m_lines_coord_indices;

  /*! An array of indices into the texture-coordinate array. */
  Lines_indices m_lines_normal_indices;

  /*! An array of indices into the color array. */
  Lines_indices m_lines_color_indices;

  /*! An array of indices into the normal array. */
  Lines_indices m_lines_tex_coord_indices;

  /*! Indicates whether the lines coordinate indices should be cleaned. */
  Boolean m_dirty_lines_coord_indices;

  /*! Indicates whether the lines normal indices should be cleaned. */
  Boolean m_dirty_lines_normal_indices;

  /*! Indicates whether the lines color indices should be cleaned. */
  Boolean m_dirty_lines_color_indices;

  /*! Indicates whether the lines texture-coordinate indices should be
   * cleaned.
   */
  Boolean m_dirty_lines_tex_coord_indices;

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
  void collapse_identical_coordinates(Int32_array& indices);

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates(Lines_indices& indices);

private:
  //! The node prototype.
  static Container_proto* s_prototype;

  //! Default value.
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief determines whether the lines coordinate indices is dirty.
inline Boolean Lines_set::is_dirty_lines_coord_indices() const
{ return m_dirty_lines_coord_indices; }

//! \brief determines whether the lines normal indices is dirty.
inline Boolean Lines_set::is_dirty_lines_normal_indices() const
{ return m_dirty_lines_normal_indices; }

//! \brief determines whether the lines color indices is dirty.
inline Boolean Lines_set::is_dirty_lines_color_indices() const
{ return m_dirty_lines_color_indices; }

//! \brief determines whether the lines texture-coordinate indices is dirty.
inline Boolean Lines_set::is_dirty_lines_tex_coord_indices() const
{ return m_dirty_lines_tex_coord_indices; }

/*! \brief set the coordinate indices from a range of elements that counts
 * the number of vertices per primitive.
 */
template <typename InputIterator>
void Lines_set::set_coord_indices_from_range(InputIterator begin,
                                             InputIterator end)
{
  Boolean is_lines(true);
  size_t num(0);
  for (auto it = begin; it != end; ++it) {
    ++num;
    if (*it != 2) is_lines = false;
  }

  if (is_lines) {
    set_primitive_type(Geo_set::PT_LINES);
    auto& lines = get_empty_line_coord_indices();
    lines.resize(num);
    Index_type j(0);
    for (size_t i = 0; i < num; ++i) {
      lines[i][0] = j++;
      lines[i][1] = j++;
    }
  }
  else {
    set_primitive_type(Geo_set::PT_LINE_STRIPS);
    auto& polylines = get_empty_polyline_coord_indices();
    polylines.resize(num);
    Index_type k(0);
    auto it = begin;
    for (size_t i = 0; i < num; ++i, ++it) {
      polylines[i].resize(*it);
      for (size_t j = 0; j < *it; ++j) polylines[i][j] = k++;
    }
  }
  set_num_primitives(num);
  m_dirty_lines_coord_indices = false;
  m_dirty_coord_indices = true;
}

//! \brief determines whether the coordinate indices is dirty.
inline Boolean Lines_set::is_dirty_coord_indices() const
{ return m_dirty_coord_indices; }

//! \brief determines whether the normal indices is dirty.
inline Boolean Lines_set::is_dirty_normal_indices() const
{ return m_dirty_normal_indices; }

//! \brief determines whether the color indices is dirty.
inline Boolean Lines_set::is_dirty_color_indices() const
{ return m_dirty_color_indices; }

//! \brief determines whether the texture-coordinate indices is dirty.
inline Boolean Lines_set::is_dirty_tex_coord_indices() const
{ return m_dirty_tex_coord_indices; }

//! \brief obtains the lines coordinate indices.
inline const Line_indices& Lines_set::line_coord_indices() const
{ return boost::get<Line_indices>(m_lines_coord_indices); }

//! \brief obtains the polyline coordinate indices.
inline const Polyline_indices& Lines_set::polyline_coord_indices() const
{ return boost::get<Polyline_indices>(m_lines_coord_indices); }

//! \brief obtains the lines normal indices.
inline const Line_indices& Lines_set::line_normal_indices() const
{ return boost::get<Line_indices>(m_lines_normal_indices); }

//! \brief obtains the polyline normal indices.
inline const Polyline_indices& Lines_set::polyline_normal_indices() const
{ return boost::get<Polyline_indices>(m_lines_normal_indices); }

//! \brief Obtain the flat normal indices.
inline const Flat_indices& Lines_set::flat_normal_indices() const
{ return boost::get<Flat_indices>(m_lines_normal_indices); }

//! \brief obtains the lines color indices.
inline const Line_indices& Lines_set::line_color_indices() const
{ return boost::get<Line_indices>(m_lines_color_indices); }

//! \brief obtains the polyline normal indices.
inline const Polyline_indices& Lines_set::polyline_color_indices() const
{ return boost::get<Polyline_indices>(m_lines_color_indices); }

//! \brief obtains the flat color indices.
inline const Flat_indices& Lines_set::flat_color_indices() const
{ return boost::get<Flat_indices>(m_lines_color_indices); }

//! \brief obtains the lines texture-coordinate indices.
inline const Line_indices& Lines_set::line_tex_coord_indices() const
{ return boost::get<Line_indices>(m_lines_tex_coord_indices); }

//! \brief obtains the polyline texture-coordinate indices.
inline const Polyline_indices& Lines_set::polyline_tex_coord_indices() const
{ return boost::get<Polyline_indices>(m_lines_tex_coord_indices); }

//! \brief constructs default.
inline Lines_set::Lines_indices_const_iterator::
Lines_indices_const_iterator() : m_it() {}

//! \brief constructs a begin iterator from lines iterators.
inline Lines_set::Lines_indices_const_iterator::
Lines_indices_const_iterator(Line_indices::const_iterator it,
                             Line_indices::const_iterator end) :
  m_it(it), m_tri_it(it->begin()), m_tri_end(end) {}

//! \brief constructs a begin iterator from polyline iterators.
inline Lines_set::Lines_indices_const_iterator::
Lines_indices_const_iterator(Polyline_indices::const_iterator it,
                             Polyline_indices::const_iterator end) :
  m_it(it), m_poly_it(it->begin()), m_poly_end(end) {}

/*! \brief constructs a past-the-end iterator from a lines past-the-end
 * iterator.
 */
inline Lines_set::Lines_indices_const_iterator::
Lines_indices_const_iterator(Line_indices::const_iterator end) :
  m_it(end), m_tri_end(end) {}

/*! \brief constructs a past-the-end iterator from a polyline past-the-end
 * iterator.
 */
inline Lines_set::Lines_indices_const_iterator::
Lines_indices_const_iterator(Polyline_indices::const_iterator end) :
  m_it(end), m_poly_end(end) {}

//! \brief construct an iterator from a flat iterator.
inline Lines_set::Lines_indices_const_iterator::
Lines_indices_const_iterator(Flat_indices::const_iterator it) : m_it(it) {}

inline const Index_type& Lines_set::Lines_indices_const_iterator::operator*()
{
  Reference_visitor visitor(*this);
  return boost::apply_visitor(visitor, m_it);
}

inline Lines_set::Lines_indices_const_iterator
Lines_set::Lines_indices_const_iterator::operator++()
{
  Incerement_visitor visitor(*this);
  boost::apply_visitor(visitor, m_it);
  return *this;
}

inline Boolean Lines_set::Lines_indices_const_iterator::
operator==(Lines_indices_const_iterator other)
{
  Compare_visitor visitor(*this, other);
  return boost::apply_visitor(visitor, m_it, other.m_it);
}

SGAL_END_NAMESPACE

#endif
