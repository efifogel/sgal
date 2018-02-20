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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4146)
#pragma warning( disable : 4244)
#pragma warning( disable : 4390)
#pragma warning( disable : 4503)
#pragma warning( disable : 4800)
#pragma warning( disable : 4996)
#endif

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a quadric
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include <QdX/gfx_utils.h>
#if QdX_USE_AcX
#include <QdX/Quadric_3.h>
#else
#include <QdX/P_quadric_3.h>
#include <QdX/Cut_of_surfaces_to_segments.h>
#endif
#include <NiX/Arithmetic_traits.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Context.hpp"

#include "SCGAL/Quadric_geo.hpp"
#include "SCGAL/Arrangement_on_quadric_geo.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Arrangement_on_quadric_geo::s_tag = "ArrangementOnQuadric";
Container_proto* Arrangement_on_quadric_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Arrangement_on_quadric_geo, "Arrangement_on_quadric_geo");

//! \brief constructs.
Arrangement_on_quadric_geo::
Arrangement_on_quadric_geo(Boolean proto) : Arrangement_on_surface_geo(proto) {}

//! \brief destructs.
Arrangement_on_quadric_geo::~Arrangement_on_quadric_geo() { clear(); }

//! \brief initializes the container prototype.
void Arrangement_on_quadric_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_surface_geo::get_prototype());
}

//! \brief deletes the container prototype.
void Arrangement_on_quadric_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Arrangement_on_quadric_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_quadric_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the container attributes.
 void Arrangement_on_quadric_geo::set_attributes(Element* elem)
{
  Arrangement_on_surface_geo::set_attributes(elem);

#if 0
  typedef Element::Str_attr_iter        Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
  }
#endif

  typedef Element::Cont_attr_iter       Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Element::Shared_container cont = elem->get_value(cai);
    if (name == "baseQuadric") {
      Shared_quadric_geo quadric =
        boost::dynamic_pointer_cast<Quadric_geo>(cont);
      set_base_quadric(quadric);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); mcai++)
  {
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "quadrics") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Container* cont = *ci;
        Quadric_geo* quadric = dynamic_cast<Quadric_geo*>(cont);
        if (quadric) insert_quadric(quadric);
        else {
          std::cerr << "Invalid " << s_tag << " quadric nodes!"
                    << std::endl;
        }
      }
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief cleans the representation.
void Arrangement_on_quadric_geo::clean()
{
  typedef NiX::Arithmetic_traits AT;
#if QdX_USE_AcX
  typedef QdX::Quadric_3<AT> Quadric_3;
#else
  typedef QdX::P_quadric_3<AT> Quadric_3;
#endif

  if (m_base_quadric->is_dirty()) m_base_quadric->clean();
  const Quadric_3& base_quadric = m_base_quadric->get_quadric();

  // creating the spatial segments
  std::vector<X_monotone_curve> segments_3;
  std::vector<Point> points_3;

  // Iterate over the quadric nodes:
  for (auto it = m_quadric_nodes.begin(); it != m_quadric_nodes.end(); ++it) {
    std::vector< X_monotone_curve::Projected_segment_2 > lower_2, upper_2;
    Quadric_geo* qg = *it;
    if (qg->is_dirty()) qg->clean();
#if !QdX_USE_AcX
    QdX::segments_of_surface_cut(base_quadric, qg->get_quadric(),
                                 std::back_inserter(lower_2),
                                 std::back_inserter(upper_2));
    QdX::lift_segments<X_monotone_curve>(lower_2.begin(), lower_2.end(),
                                         base_quadric, 0,
                                         std::back_inserter(segments_3));

    QdX::lift_segments<X_monotone_curve>(upper_2.begin(), upper_2.end(),
                                         base_quadric, 1,
                                         std::back_inserter(segments_3));
#else
    Quadric_pair_3 pair(base_quadric, qg->get_quadric());

    // lift segments
    pair.spatial_intersections<X_monotone_curve>(base_quadric,
                                                 std::back_inserter(segments_3),
                                                 std::back_inserter(points_3));
    // TODO degenerate segments
#if 0
    for (int i = 0; i < static_cast< int >(points_3.size()); i++) {
      segments_3.push_back(Segment_3(points_3[i]));
    }
#endif
#endif
  }

  // Initialize:
  Geom_traits geo_traits(base_quadric);
  m_aoq = Arrangement_on_quadric(&geo_traits);

  // Insert:
  CGAL::insert_empty(m_aoq,
                     segments_3.begin(), segments_3.end(),
                     points_3.begin(), points_3.end());

  m_dirty = false;
}

//! \brief
void Arrangement_on_quadric_geo::cull(Cull_context& cull_context)
{
  //! \todo
}

//! \brief
void Arrangement_on_quadric_geo::isect(Isect_action* action)
{
  //! \todo
}

//! \brief
Boolean Arrangement_on_quadric_geo::clean_bounding_sphere()
{
  Boolean changed = false;
  if (m_base_quadric)
    m_bounding_sphere = *m_base_quadric->get_bounding_sphere(changed);
  return changed;
}

//! \brief clears the internal representation and auxiliary data structures.
void Arrangement_on_quadric_geo::clear()
{ m_aoq.clear(); }

//! \brief determines whether the representation is empty.
Boolean Arrangement_on_quadric_geo::is_empty() const
{ return m_aoq.is_empty(); }

//! \brief draws the embedding surface.
void Arrangement_on_quadric_geo::draw_surface(Draw_action* action)
{ m_base_quadric->draw(action); }

//! \brief draws the arrangement vertices.
void Arrangement_on_quadric_geo::draw_aos_vertices(Draw_action* action)
{
  Aoq_vertex_const_iterator vi;
  for (vi = m_aoq.vertices_begin(); vi != m_aoq.vertices_end(); ++vi) {
    const Point& point = vi->point();
    if (point.is_at_infinity()) continue;
    QdX::Gfx_point_3 dpt = point.to_double();
    float x = static_cast<float>(dpt.x());
    float y = static_cast<float>(dpt.y());
    float z = static_cast<float>(dpt.z());
    Vector3f center(x, y, z);
    center.normalize();
    draw_aos_vertex(action, center);
  }
}

//! \brief draws the arrangement edges.
void Arrangement_on_quadric_geo::draw_aos_edges(Draw_action* action)
{
#if 0
  Vector3f src;
  Vector3f trg;
  Aos_edge_const_iterator hei;
  for (hei = m_aoq.edges_begin(); hei != m_aoq.edges_end(); ++hei) {
    const X_monotone_curve& curve = hei->curve();
    const Point& source = curve.source();
    const Point& target = curve.target();

    src[0] = static_cast<float>(CGAL::to_double(source.dx()));
    src[1] = static_cast<float>(CGAL::to_double(source.dy()));
    src[2] = static_cast<float>(CGAL::to_double(source.dz()));

    trg[0] = static_cast<float>(CGAL::to_double(target.dx()));
    trg[1] = static_cast<float>(CGAL::to_double(target.dy()));
    trg[2] = static_cast<float>(CGAL::to_double(target.dz()));

    src.normalize();
    trg.normalize();
    draw_aos_edge(action, src, trg);
  }
#endif
}

//! \brief sets the base quadric.
void Arrangement_on_quadric_geo::set_base_quadric(Shared_quadric_geo quadric)
{
  m_base_quadric = quadric;
  m_bounding_sphere_dirty = true;
}

SGAL_END_NAMESPACE
