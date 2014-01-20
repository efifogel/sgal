// Copyright (c) 2010 Israel.
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
// $Id: Lines_through_segments_geo.hpp 14223 2012-11-29 22:33:55Z efif $
// $Revision: 14223 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents all lines that intersect tupples
 * of 4 segments, given a container of segments
 */

#ifndef SCGAL_LINES_THROUGH_SEGMENTS_HPP
#define SCGAL_LINES_THROUGH_SEGMENTS_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <list>
#include <string>
#include <vector>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#include <CGAL/Lines_through_segments_3.h>
#include <CGAL/Lines_through_segments_traits_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Cull_context;
class Indexed_line_set;
class Field_info;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! A geometry container that represents all lines that intersect tupples
 * of 4 segments, given a container of segments
 */
class SGAL_SCGAL_DECL Lines_through_segments_geo : public Geometry {
public:
  typedef boost::shared_ptr<Indexed_line_set>           Shared_indexed_line_set;

  typedef CGAL::CORE_algebraic_number_traits            Nt_traits;
  typedef Nt_traits::Algebraic                          Algebraic;
  typedef Nt_traits::Rational                           Rational;

  typedef CGAL::Cartesian<Algebraic>                    Alg_kernel;
  typedef CGAL::Cartesian<Rational>                     Rat_kernel;
  typedef Rat_kernel::Line_3                            Rat_line_3;
  typedef Rat_kernel::Segment_3                         Rat_segment_3;
  typedef Rat_kernel::Point_3                           Rat_point_3;
  typedef Rat_kernel::FT                                Rat_number_type;

  typedef CGAL::Lines_through_segments_traits_3<Alg_kernel, Rat_kernel>
    Lines_through_segments_traits_3;

  typedef boost::true_type With_segments;
  typedef CGAL::Lines_through_segments_3<Lines_through_segments_traits_3,
                                         With_segments>
    Lines_through_segments_3;
  typedef Lines_through_segments_3::Transversal_with_segments
    Transversal_with_segments;

  enum {
    FIRST = Geometry::LAST - 1,
    SEGMENTS,
    LAST
  };

  /*! Constructor */
  Lines_through_segments_geo(Boolean proto = false);

  /*! Destructor */
  virtual ~Lines_through_segments_geo();

  /* Construct the prototype */
  static Lines_through_segments_geo* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Shared_indexed_line_set* segments_handle(const Field_info*)
  { return &m_segments; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation */
  virtual void clean();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! */
  virtual Boolean clean_sphere_bound();

  /*! Draw the geometry */
  virtual void draw(Draw_action* action);

  /*! Clear the internal representation and auxiliary data structures
   */
  virtual void clear();

  /*! Determine whether the representation hasn't been updated
   */
  virtual Boolean is_dirty() const { return m_dirty; }

  /*! Is the representation empty ?
   */
  virtual Boolean is_empty();

  /*! Process change of content */
  virtual void field_changed(Field_info* field_info);

  /*! Set the segments.
   * \param segments the segments.
   */
  void set_segments(Shared_indexed_line_set segments);

  /*! Obtain the segments.
   * \return the segments.
   */
  const Shared_indexed_line_set get_segments() const { return m_segments; }

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Indicates whether the lts data structure is owned, i.e., explicitly
   * allocated. If the lts data structure is owned, it should be destructed
   * when this geometry node is destructed.
   */
  Boolean m_owned_lts;

  /* The internal Lines Through Segments data structutre. */
  Lines_through_segments_3* m_lts;

  /*! The segments */
  Shared_indexed_line_set m_segments;

  /*! Indicates whether the data structure must be cleaned. */
  Boolean m_dirty;

  /*! Draw a single line. */
  template <typename Line_type>
  void draw_line(Draw_action* action,
                 Line_type& line,
                 const Matrix4f& view_mat, const Matrix4f& view_mat_inv);

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! The exact segments. */
  std::vector<Rat_segment_3> m_in_segments;

  /*! Indicates whether the rational segments must be cleaned. */
  bool m_in_segments_dirty;

  /*! The output list of lines. */
  std::list<Transversal_with_segments> m_out_lines;

  Alg_kernel m_alg_kernel;
  Rat_kernel m_rat_kernel;

  /*! Default values */
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/* \brief constructs the prototype. */
inline Lines_through_segments_geo* Lines_through_segments_geo::prototype()
{ return new Lines_through_segments_geo(true); }

/*! \brief clones. */
inline Container* Lines_through_segments_geo::clone()
{ return new Lines_through_segments_geo(); }

SGAL_END_NAMESPACE

#endif
