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
// $Id: Lines_through_segments_geo.cpp 11155 2011-05-25 15:30:08Z efif $
// $Revision: 11155 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4503)
#endif

/*! \file
 * A geometry container that represents all lines that intersect tupples
 * of 4 segments, given a container of segments
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Plane.hpp"
#include "SGAL/Line.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"

#include "SCGAL/Lines_through_segments_geo.hpp"

SGAL_BEGIN_NAMESPACE

std::string Lines_through_segments_geo::s_tag = "LinesThroughSegments";
Container_proto* Lines_through_segments_geo::s_prototype = NULL;

REGISTER_TO_FACTORY(Lines_through_segments_geo, "Lines_through_segments_geo");

/*! Constructor */
Lines_through_segments_geo::Lines_through_segments_geo(Boolean proto) :
  Geometry(proto),
  m_own_lts(false),
  m_lts(NULL),
  m_segments(NULL),
  m_dirty(true),
  m_in_segments_dirty(true)
{
}

/*! Destructor */
Lines_through_segments_geo::~Lines_through_segments_geo()
{
  clear();
  if (m_lts && m_own_lts) delete m_lts;
}

/*! \brief clear the internal representation and auxiliary data structures
 */
void Lines_through_segments_geo::clear()
{
  m_in_segments.clear();
  m_in_segments_dirty = true;
  m_out_lines.clear();
  m_dirty = true;
}

/*! \brief initializes the container prototype */
void Lines_through_segments_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());
}

/*! \brief deletes the container prototype */
void Lines_through_segments_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto* Lines_through_segments_geo::get_prototype()
{
  if (!s_prototype) Lines_through_segments_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this node */
void Lines_through_segments_geo::set_attributes(Element* elem)
{
  typedef Element::Cont_attr_iter         Cont_attr_iter;
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container * cont = elem->get_value(cai);
    if (name == "segments") {
      Indexed_line_set * segments = dynamic_cast<Indexed_line_set*>(cont);
      set_segments(segments);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief determines whether the representation empty */
Boolean Lines_through_segments_geo::is_empty() { return true; }

/*! \brief clean the representation */
void Lines_through_segments_geo::clean()
{
  std::cout << "Lines_through_segments_geo::clean()" << std::endl;
  if (!m_lts) {
    m_lts = new Lines_through_segments_3(m_alg_kernel, m_rat_kernel);
    m_own_lts = true;
  }
  
  if (m_in_segments_dirty) {
    if (!m_segments) return;
//     Exact_coord_array* coord_array =
//       dynamic_cast<Exact_coord_array *>(m_segments->get_coord_array());
//     if (!coord_array) return;
    Coord_array* coord_array = m_segments->get_coord_array();
    if (!coord_array) return;
    const SGAL::Array<Uint>& coord_indices = m_segments->get_coord_indices();

    for (unsigned int i = 0; i < coord_indices.size(); i += 2) {
      const Vector3f& ep1 = (*coord_array)[i];
      const Vector3f& ep2 = (*coord_array)[i+1];
      Rat_point_3 p1(ep1[0], ep1[1], ep1[2]);
      Rat_point_3 p2(ep2[0], ep2[1], ep2[2]);
      m_in_segments.push_back(Rat_segment_3(p1, p2));
    }
    m_in_segments_dirty = false;
    std::cout << "# input segments: " << m_in_segments.size() << std::endl;

    copy(m_in_segments.begin(), m_in_segments.end(),
         std::ostream_iterator<Rat_segment_3>(std::cout, "\n"));
    
  }
  (*m_lts)(m_in_segments.begin(), m_in_segments.end(),
           std::back_inserter(m_out_lines), false, true);
  m_dirty = false;
  std::cout << "# output lines: " <<  m_out_lines.size() << std::endl;

  std::list<Transversal_with_segments>::const_iterator it;
  for (it = m_out_lines.begin(); it != m_out_lines.end(); ++it) {

    typedef Lines_through_segments_3::Transversal         Transversal;
      
    typedef Lines_through_segments_3::Line_3              Line_3;
    typedef Lines_through_segments_3::Mapped_2            Mapped_2;
    typedef Lines_through_segments_3::Through_3           Through_3;

    typedef Lines_through_segments_3::Mapped_transversal  Mapped_transversal;
    typedef Lines_through_segments_3::Through_transversal Through_transversal;
      
    typedef Lines_through_segments_3::Mapped_x_monotone_curve_2
      Mapped_x_monotone_curve_2;
    typedef Lines_through_segments_3::Mapped_general_polygon_2
      Mapped_general_polygon_2;
    typedef Lines_through_segments_3::Mapped_point_2      Mapped_point_2;
    typedef Lines_through_segments_3::Mapped_rat_point_2  Mapped_rat_point_2;
 
    typedef Lines_through_segments_3::Through_point_3     Through_point_3;
    typedef Lines_through_segments_3::Through_point_3_segment_3
      Through_point_3_segment_3;
    typedef Lines_through_segments_3::Through_segment_3   Through_segment_3;

    Transversal transversal = it->first;
    
    Line_3* line_obj;
    Mapped_2* mapped_obj;
    Through_3* through_obj;

    if ((line_obj = boost::get<Line_3>(&transversal))) {
      std::cout << "line = " << *line_obj << std::endl;
    }
    else if ((mapped_obj = boost::get<Mapped_2>(&transversal))) {
      Mapped_transversal mapped_transversal = mapped_obj->mapped_transversal();

      Mapped_x_monotone_curve_2* curve_obj;
      Mapped_general_polygon_2* polygon_obj;
      Mapped_point_2* point_obj;
      Mapped_rat_point_2* rat_point_obj;

      if ((rat_point_obj =
                boost::get<Mapped_rat_point_2>(&mapped_transversal)))
      {
        Line_3 line = mapped_obj->rational_line();
        std::cout << "line = " << line << std::endl;
      } else {
        if ((curve_obj =
             boost::get<Mapped_x_monotone_curve_2>(&mapped_transversal)))
        {
          std::cout << "Mapped_x_monotone_curve_2 = " << *curve_obj << ", ";
        }
        else if ((polygon_obj =
                  boost::get<Mapped_general_polygon_2>(&mapped_transversal)))
        {
          std::cout << "Mapped_general_polygon_2 = " << *polygon_obj << ", ";
        }
        else if ((point_obj = boost::get<Mapped_point_2>(&mapped_transversal))) {
          std::cout << "Mapped_point_2 = " << *point_obj << ", ";
        }

        Mapped_2::Mapped_line_3 line = mapped_obj->line();
        std::cout << "Line = " << line << std::endl;
      }
    }
    else if ((through_obj = boost::get<Through_3>(&transversal))) {
      Through_transversal through_transversal =
        through_obj->through_transversal();

      Through_point_3* point_obj;
      Through_point_3_segment_3* arc_obj;
      Through_segment_3* seg_obj;

      if ((arc_obj =
           boost::get<Through_point_3_segment_3>(&through_transversal)))
      {
        std::cout << "Through_point_3_segment_3 = (" << arc_obj->first << ","
                  << arc_obj->second << ")" << std::endl;
      }
      else if ((seg_obj = boost::get<Through_segment_3>(&through_transversal)))
      {
        std::cout << "Through_segment_3 = " << *seg_obj << std::endl;
      }
      else if ((point_obj = boost::get<Through_point_3>(&through_transversal)))
      {
        std::cout << "Through_point_3 = " << *point_obj << std::endl;
      }
    }
  }
}

/*! \brief draws the geometry */
void Lines_through_segments_geo::draw(Draw_action* action)
{
  if (m_dirty) clean();
  if (m_dirty) return;

  // Draw the output
  Context* context = action->get_context();
  Camera* camera = context->get_active_camera();

  Matrix4f view_mat;
  camera->get_view_mat(view_mat);
  Matrix4f view_mat_inv;
  view_mat_inv.invert_affine(view_mat);

  Frustum& frustum = camera->get_base_frust();
  float left;
  float right;
  float bottom;
  float top;
  float near_clip;
  float far_clip;
  frustum.get_corners(left, right, bottom, top, near_clip, far_clip);
  std::cout <<  "left: " << left << ", right: " << right
            << ", bottom: " << bottom << ", top: " << top
            << ", near: " << near_clip << ", far: " << far_clip
            << std::endl;
  Plane* planes = frustum.get_facets();
  
  std::list<Transversal_with_segments>::const_iterator it;
  for (it = m_out_lines.begin(); it != m_out_lines.end(); ++it) {

    typedef Lines_through_segments_3::Transversal         Transversal;
      
    typedef Lines_through_segments_3::Line_3              Line_3;
    typedef Lines_through_segments_3::Mapped_2            Mapped_2;
    typedef Lines_through_segments_3::Through_3           Through_3;

    typedef Lines_through_segments_3::Mapped_transversal  Mapped_transversal;
    typedef Lines_through_segments_3::Through_transversal Through_transversal;
      
    typedef Lines_through_segments_3::Mapped_x_monotone_curve_2
      Mapped_x_monotone_curve_2;
    typedef Lines_through_segments_3::Mapped_general_polygon_2
      Mapped_general_polygon_2;
    typedef Lines_through_segments_3::Mapped_point_2      Mapped_point_2;

    typedef Lines_through_segments_3::Through_point_3     Through_point_3;
    typedef Lines_through_segments_3::Through_point_3_segment_3
      Through_point_3_segment_3;
    typedef Lines_through_segments_3::Through_segment_3   Through_segment_3;

    Transversal transversal = it->first;
    
    Line_3* line_obj;
    Mapped_2* mapped_obj;
    Through_3* through_obj;

    if ((line_obj = boost::get<Line_3>(&transversal))) {
      Rat_point_3 ep1 = line_obj->point(0);
      Rat_point_3 ep2 = line_obj->point(1);
      float x = CGAL::to_double(ep1.x());
      float y = CGAL::to_double(ep1.y());
      float z = CGAL::to_double(ep1.z());
      Vector3f p1(x, y, z);
      std::cout << p1 << std::endl;

      x = CGAL::to_double(ep2.x());
      y = CGAL::to_double(ep2.y());
      z = CGAL::to_double(ep2.z());
      Vector3f p2(x, y, z);
      std::cout << p2 << std::endl;

      Vector3f xp1;
      xp1.xform_pt(p1, view_mat);
      Vector3f xp2;
      xp2.xform_pt(p2, view_mat);
      
      Line line(xp1, xp2);
      Vector3f p;
      if (planes[Frustum::LEFT_PLANE].intersect(line, p)) {
        std::cout << "(left) intersection point: " << p << std::endl;
      }
      if (planes[Frustum::RIGHT_PLANE].intersect(line, p)) {
        std::cout << "(right) intersection point: " << p << std::endl;
      }
      if (planes[Frustum::NEAR_PLANE].intersect(line, p)) {
        std::cout << "(near) intersection point: " << p << std::endl;
      }
      if (planes[Frustum::FAR_PLANE].intersect(line, p)) {
        std::cout << "(far) intersection point: " << p << std::endl;
      }
      if (planes[Frustum::BOTTOM_PLANE].intersect(line, p)) {
        std::cout << "(bottom) intersection point: " << p << std::endl;
      }
      if (planes[Frustum::TOP_PLANE].intersect(line, p)) {
        std::cout << "(top) intersection point: " << p << std::endl;
      }
    }
#if 0
    else if ((mapped_obj = boost::get<Mapped_2>(&transversal))) {
      Mapped_transversal mapped_transversal = mapped_obj->mapped_transversal();
      Mapped_2::Mapped_line_3 line = mapped_obj->line();

      Mapped_x_monotone_curve_2* curve_obj;
      Mapped_general_polygon_2* polygon_obj;
      Mapped_point_2* point_obj;

      if ((curve_obj =
           boost::get<Mapped_x_monotone_curve_2>(&mapped_transversal)))
      {
        std::cout << "Mapped_x_monotone_curve_2 = " << *curve_obj << ", ";
      }
      else if ((polygon_obj =
                boost::get<Mapped_general_polygon_2>(&mapped_transversal)))
      {
        std::cout << "Mapped_general_polygon_2 = " << *polygon_obj << ", ";
      }
      else if ((point_obj = boost::get<Mapped_point_2>(&mapped_transversal))) {
        std::cout << "Mapped_point_2 = " << *point_obj << ", ";
      }
      std::cout << "Line = " << line << std::endl;
    }
    else if ((through_obj = boost::get<Through_3>(&transversal))) {
      Through_transversal through_transversal =
        through_obj->through_transversal();

      Through_point_3* point_obj;
      Through_point_3_segment_3* arc_obj;
      Through_segment_3* seg_obj;

      if ((arc_obj =
           boost::get<Through_point_3_segment_3>(&through_transversal)))
      {
        std::cout << "Through_point_3_segment_3 = (" << arc_obj->first << ","
                  << arc_obj->second << ")" << std::endl;
      }
      else if ((seg_obj = boost::get<Through_segment_3>(&through_transversal)))
      {
        std::cout << "Through_segment_3 = " << *seg_obj << std::endl;
      }
      else if ((point_obj = boost::get<Through_point_3>(&through_transversal)))
      {
        std::cout << "Through_point_3 = " << *point_obj << std::endl;
      }
    }
#endif
  }
}

/*! \brief */
void Lines_through_segments_geo::cull(Cull_context& cull_context)
{
}

/*! \brief */
void Lines_through_segments_geo::isect(Isect_action* action)
{
  //! \todo isect with a sphere
}

/*! \brief */
Boolean Lines_through_segments_geo::calculate_sphere_bound()
{
  return true;
}

SGAL_END_NAMESPACE
