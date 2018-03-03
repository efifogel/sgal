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
#include "SGAL/Math_defs.hpp"
#include "SGAL/Plane.hpp"
#include "SGAL/Line.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"

#include "SCGAL/Lines_through_segments_geo.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Lines_through_segments_geo::s_tag = "LinesThroughSegments";
Container_proto* Lines_through_segments_geo::s_prototype(NULL);

REGISTER_TO_FACTORY(Lines_through_segments_geo, "Lines_through_segments_geo");

//! \brief constructor.
Lines_through_segments_geo::Lines_through_segments_geo(Boolean proto) :
  Geometry(proto),
  m_owned_lts(false),
  m_lts(NULL),
  m_dirty(true),
  m_in_segments_dirty(true)
{}

//! \brief destructor.
Lines_through_segments_geo::~Lines_through_segments_geo()
{
  clear();
  if (m_owned_lts) {
    if (m_lts) {
      delete m_lts;
      m_lts = NULL;
    }
    m_owned_lts = false;
  }
}

//! \brief clears the internal representation and auxiliary data structures.
void Lines_through_segments_geo::clear()
{
  m_in_segments.clear();
  m_in_segments_dirty = true;
  m_dirty_bounding_sphere = true;
  m_out_lines.clear();
  m_dirty = true;
}

//! \brief initializes the container prototype.
void Lines_through_segments_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // segments
  Shared_container_handle_function segments_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Lines_through_segments_geo::segments_handle);
  s_prototype->add_field_info(new SF_shared_container(SEGMENTS, "segments",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      segments_func));
}

//! \brief deletes the container prototype.
void Lines_through_segments_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Lines_through_segments_geo::get_prototype()
{
  if (!s_prototype) Lines_through_segments_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this node.
void Lines_through_segments_geo::set_attributes(Element* elem)
{
  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const std::string & name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "segments") {
      Shared_indexed_line_set segments =
        boost::dynamic_pointer_cast<Indexed_line_set>(cont);
      set_segments(segments);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief determines whether the representation empty.
Boolean Lines_through_segments_geo::is_empty() { return true; }

//! \brief clean the representation.
void Lines_through_segments_geo::clean()
{
  if (!m_lts) {
    m_lts = new Lines_through_segments_3(m_alg_kernel, m_rat_kernel);
    SGAL_assertion(m_lts);
    m_owned_lts = true;
  }

  if (m_in_segments_dirty) {
    if (!m_segments) return;
//     Epec_coord_array_3d* coord_array =
//       dynamic_cast<Epec_coord_array *>(m_segments->get_coord_array());
//     if (!coord_array) return;
    const auto coord_array =
      boost::dynamic_pointer_cast<Coord_array_3d>(m_segments->get_coord_array());
    if (!coord_array) return;
    auto& coord_indices = m_segments->get_coord_indices();

    Uint j = 0;
    for (Uint i = 0; i < m_segments->get_num_primitives(); ++i) {
      const auto& ep1 = (*coord_array)[coord_indices[j++]];
      const auto& ep2 = (*coord_array)[coord_indices[j++]];
      // Ensure that every polyline consists of a single segment
      SGAL_assertion(coord_indices[j] == static_cast<Uint>(-1));
      ++j;

      Rat_point_3 p1(ep1[0], ep1[1], ep1[2]);
      Rat_point_3 p2(ep2[0], ep2[1], ep2[2]);
      m_in_segments.push_back(Rat_segment_3(p1, p2));
    }
    m_in_segments_dirty = false;

    // std::cout << "# input segments: " << m_in_segments.size() << std::endl;
    // copy(m_in_segments.begin(), m_in_segments.end(),
    //      std::ostream_iterator<Rat_segment_3>(std::cout, "\n"));
  }
  (*m_lts)(m_in_segments.begin(), m_in_segments.end(),
           std::back_inserter(m_out_lines));
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

//! \brief draws a single line.
template <typename Line_type>
void Lines_through_segments_geo::draw_line(Draw_action* action,
                                           Line_type& line_obj,
                                           const Matrix4f& view_mat,
                                           const Matrix4f& view_mat_inv)
{
  Context* context = action->get_context();
  Camera* camera = context->get_active_camera();

  Frustum& frustum = camera->get_frustum();
  // float left;
  // float right;
  // float bottom;
  // float top;
  // float near_clip;
  // float far_clip;
  // frustum.get_corners(left, right, bottom, top, near_clip, far_clip);
  // std::cout <<  "left: " << left << ", right: " << right
  //           << ", bottom: " << bottom << ", top: " << top
  //           << ", near: " << near_clip << ", far: " << far_clip
  //           << std::endl;
  Plane* planes = frustum.get_facets();
  const Plane& left_plane = planes[Frustum::LEFT_PLANE];
  const Plane& right_plane = planes[Frustum::RIGHT_PLANE];
  const Plane& near_plane = planes[Frustum::NEAR_PLANE];
  const Plane& far_plane = planes[Frustum::FAR_PLANE];
  const Plane& bottom_plane = planes[Frustum::BOTTOM_PLANE];
  const Plane& top_plane = planes[Frustum::TOP_PLANE];

  // std::cout << "near:m_normal: " << near_plane.get_normal() << std::endl;
  // std::cout << "near:m_offset: " << near_plane.get_offset() << std::endl;
  // std::cout << "far:m_normal: " << far_plane.get_normal() << std::endl;
  // std::cout << "far:m_offset: " << far_plane.get_offset() << std::endl;

  //Rat_point_3 ep1 = line_obj.point(0);
  //Rat_point_3 ep2 = line_obj.point(1);
  float x = CGAL::to_double(line_obj.point(0).x());
  float y = CGAL::to_double(line_obj.point(0).y());
  float z = CGAL::to_double(line_obj.point(0).z());
  Vector3f p1(x, y, z);
  // std::cout << p1 << std::endl;

  x = CGAL::to_double(line_obj.point(1).x());
  y = CGAL::to_double(line_obj.point(1).y());
  z = CGAL::to_double(line_obj.point(1).z());
  Vector3f p2(x, y, z);
  // std::cout << p2 << std::endl;

  Vector3f xp1;
  xp1.xform_pt(p1, view_mat);
  //std::cout << xp1 << std::endl;

  Vector3f xp2;
  xp2.xform_pt(p2, view_mat);
  //std::cout << xp2 << std::endl;

  Line line(xp1, xp2);
  std::list<Vector3f> points;
  Vector3f p;
  if (left_plane.intersect(line, p)) {
    //std::cout << "Intersection with left: " << p << std::endl;
    if ((near_plane.contains(p) >= 0) &&
        (far_plane.contains(p) >= 0) &&
        (bottom_plane.contains(p) >= 0) &&
        (top_plane.contains(p) >= 0))
      points.push_back(p);
  }
  if (right_plane.intersect(line, p)) {
    //std::cout << "Intersection with right: " << p << std::endl;
    if ((near_plane.contains(p) >= 0) &&
        (far_plane.contains(p) >= 0) &&
        (bottom_plane.contains(p) >= 0) &&
        (top_plane.contains(p) >= 0))
      points.push_back(p);
  }
  if ((points.size() < 2) && (bottom_plane.intersect(line, p))) {
    //std::cout << "Intersection with bottom: " << p << std::endl;
    if ((left_plane.contains(p) >= 0) &&
        (right_plane.contains(p) >= 0) &&
        (near_plane.contains(p) >= 0) &&
        (far_plane.contains(p) >= 0))
      points.push_back(p);
  }
  if ((points.size() < 2) && (top_plane.intersect(line, p))) {
    //std::cout << "Intersection with top: " << p << std::endl;
    if ((left_plane.contains(p) >= 0) &&
        (right_plane.contains(p) >= 0) &&
        (near_plane.contains(p) >= 0) &&
        (far_plane.contains(p) >= 0))
      points.push_back(p);
  }
  if ((points.size() < 2) && (near_plane.intersect(line, p))) {
    //std::cout << "Intersection with near: " << p << std::endl;
    if ((left_plane.contains(p) >= 0) &&
        (right_plane.contains(p) >= 0) &&
        (bottom_plane.contains(p) >= 0) &&
        (top_plane.contains(p) >= 0))
      points.push_back(p);
  }
  if ((points.size() < 2) && (far_plane.intersect(line, p))) {
    //std::cout << "Intersection with far: " << p << std::endl;
    if ((left_plane.contains(p) >= 0) &&
        (right_plane.contains(p) >= 0) &&
        (bottom_plane.contains(p) >= 0) &&
        (top_plane.contains(p) >= 0))
      points.push_back(p);
  }

  //std::cout << "# points: " << points.size() << std::endl;
  SGAL_assertion(points.size() >= 2);

  std::list<Vector3f>::const_iterator cit = points.begin();
  xp1.xform_pt(*cit++, view_mat_inv);
  xp2.xform_pt(*cit, view_mat_inv);
  points.clear();

  // Vector3f center;
  // center.add(xp1, xp2);
  // center.scale(0.5f);
  // Sphere sphere;
  // sphere.set_center(center);
  // sphere.draw(action);

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3fv((float*)&xp1);
  // std::cout << xp1 << std::endl;
  glVertex3fv((float*)&xp2);
  // std::cout << xp2 << std::endl;
  glEnd();
}

//! \brief draws the geometry.
void Lines_through_segments_geo::draw(Draw_action* action)
{
  // Draw the input:

  if (m_dirty) clean();
  if (m_dirty) return;

  // Draw the output:
  Context* context = action->get_context();
  Camera* camera = context->get_active_camera();

  const Matrix4f& camera_mat = camera->get_view_mat();
  boost::shared_ptr<Matrix4f> world_mat = action->get_current_wtm();
  Matrix4f view_mat;
  view_mat.mult(camera_mat, *world_mat);

  Matrix4f view_mat_inv;
  view_mat_inv.invert_affine(view_mat);

  std::list<Transversal_with_segments>::const_iterator it;

  // Set graphics state for line rendering
  context->draw_light_enable(false);
  context->draw_line_width(2);
  // glDepthRange(0.05f, 1);

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
      draw_line(action, *line_obj, view_mat, view_mat_inv);
    }
    else if ((mapped_obj = boost::get<Mapped_2>(&transversal))) {
      Mapped_transversal mapped_transversal = mapped_obj->mapped_transversal();
      Mapped_2::Mapped_line_3 line = mapped_obj->line();
      draw_line(action, line, view_mat, view_mat_inv);

      Mapped_x_monotone_curve_2* curve_obj;
      Mapped_general_polygon_2* polygon_obj;
      Mapped_point_2* point_obj;

      if ((curve_obj =
           boost::get<Mapped_x_monotone_curve_2>(&mapped_transversal)))
      {
        // std::cout << "Mapped_x_monotone_curve_2 = " << *curve_obj << ", ";
      }
      else if ((polygon_obj =
                boost::get<Mapped_general_polygon_2>(&mapped_transversal)))
      {
        // std::cout << "Mapped_general_polygon_2 = " << *polygon_obj << ", ";
      }
      else if ((point_obj = boost::get<Mapped_point_2>(&mapped_transversal))) {
        // std::cout << "Mapped_point_2 = " << *point_obj << ", ";
      }
      // std::cout << "Line = " << line << std::endl;
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
        // std::cout << "Through_point_3_segment_3 = (" << arc_obj->first << ","
        //           << arc_obj->second << ")" << std::endl;
      }
      else if ((seg_obj = boost::get<Through_segment_3>(&through_transversal)))
      {
        // std::cout << "Through_segment_3 = " << *seg_obj << std::endl;
      }
      else if ((point_obj = boost::get<Through_point_3>(&through_transversal)))
      {
        // std::cout << "Through_point_3 = " << *point_obj << std::endl;
      }
    }
  }

  // Restore graphics state to default values:
  context->draw_line_width(1.0f);
  context->draw_light_enable(true);
  // glDepthRange(0, 1);
}

//! \brief
void Lines_through_segments_geo::cull(Cull_context& cull_context) {}

//! \brief
void Lines_through_segments_geo::isect(Isect_action* action)
{
  //! \todo isect with a sphere
}

//! \brief cleans the bounding sphere of the geometry container.
void Lines_through_segments_geo::clean_bounding_sphere()
{
  if (is_dirty()) clean();

  // TODO: calculate the sphere bound

  m_dirty_bounding_sphere = false;
}

//! \biref sets the segments.
void Lines_through_segments_geo::set_segments(Shared_indexed_line_set segments)
{
  clear();
  m_segments = segments;
  std::cout << "this: " << this << std::endl;
  Observer observer(this, get_field_info(SEGMENTS));
  segments->register_observer(observer);
}

//! \biref processes change of points.
void Lines_through_segments_geo::field_changed(const Field_info* field_info)
{
  Container::field_changed(field_info);
  clear();
}

SGAL_END_NAMESPACE
