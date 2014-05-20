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

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <CGAL/Cartesian.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <map>

#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"

#include "SCGAL/Triangulation_geo.hpp"

SGAL_BEGIN_NAMESPACE

std::string Triangulation_geo::s_tag = "Triangulation";
SGAL::Container_proto* Triangulation_geo::s_prototype(nullptr);

const Float Triangulation_geo::s_def_line_width(1.0f);
const Boolean Triangulation_geo::s_def_draw_haloed(true);

REGISTER_TO_FACTORY(Triangulation_geo, "Triangulation_geo");

//! \brief constructor.
Triangulation_geo::Triangulation_geo(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_line_width(s_def_line_width),
  m_draw_haloed(s_def_draw_haloed),
  m_bb_is_pre_set(false),
  m_time(0)
{ set_generated_tex_coord(false); }

//! \brief destructor.
Triangulation_geo::~Triangulation_geo() {}

//! \brief cleans the polyhedron data structure.
void Triangulation_geo::clean()
{
  clock_t start_time = clock();

  typedef std::vector<Point>            Point_vector;
  typedef Point_vector::const_iterator  Point_iter;

  Point_vector points;
  points.resize(m_coord_array->size());
  Convert_exact_point convert;
  boost::shared_ptr<Coord_array_3d> coord_array =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coord_array);
  std::transform(coord_array->begin(), coord_array->end(),
                 points.begin(), convert);
  m_triangulation.insert(points.begin(), points.end());

  // An inverse index for the points
  typedef std::map<const Point*, Uint, Point_cmp>       Index_map;
  typedef Index_map::const_iterator                     Index_iter;

  Index_map point_index;
  if (are_generated_color()) {
    SGAL_assertion_msg(m_coord_array->size() <= m_color_array->size(),
                       "color size must at least as large as point size!");
    Uint index = 0;
    for (Point_iter ci = points.begin(); ci != points.end(); ++ci) {
      point_index[&*ci] = index++;
    }
  }

  // Update the extended vertex with drawable coordinates and color:
  for (Finite_vertices_iterator fvi = m_triangulation.finite_vertices_begin();
       fvi != m_triangulation.finite_vertices_end(); ++fvi)
  {
    const Point& p = fvi->point();
    Vertex_data& vd = fvi->info();
    float x = static_cast<float>(CGAL::to_double(p.x()));
    float y = static_cast<float>(CGAL::to_double(p.y()));
    float z = static_cast<float>(CGAL::to_double(p.z()));
    vd.set_coord(x, y, z);

    if (are_generated_color()) {
      Index_iter ii = point_index.find(&p);
      SGAL_assertion(ii != point_index.end());
      Uint i = (*ii).second;
      const Vector3f& color = (*m_color_array)[i];
      vd.set_color(color);
    }
  }

  points.clear();
  m_dirty = false;

  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
}

//! \brief clears the internal representation.
void Triangulation_geo::clear()
{
  m_triangulation.clear();
  m_dirty = true;
}

//! \brief
void Triangulation_geo::cull(SGAL::Cull_context& /* cull_context */) {}

//! \brief
void Triangulation_geo::draw(SGAL::Draw_action* action)
{
  if (is_dirty()) clean();

  Context* context = action->get_context();

  if (m_draw_haloed) {
    context->draw_line_width(m_line_width * 8);
    context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
    context->draw_depth_enable(true);
    glDepthRange (0.1, 1.0);
    draw_geometry(action);

    context->draw_line_width(m_line_width);
    glDepthRange (0, 1.0);
    context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));
    context->draw_depth_func(Gfx::LEQUAL_DFUNC);
    draw_geometry(action);
    context->draw_depth_func(Gfx::LESS_DFUNC);
  } else {
    context->draw_line_width(m_line_width);
    draw_geometry(action);
  }
  context->draw_line_width(1.0f);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//! \brief
void Triangulation_geo::draw_geometry(SGAL::Draw_action* action)
{
  SGAL::Context* context = action->get_context();
  if (are_generated_color()) context->draw_light_enable(false);
  glBegin(GL_LINES);
  for (Finite_edges_iterator fei = m_triangulation.finite_edges_begin();
       fei != m_triangulation.finite_edges_end(); ++fei)
  {
    // Kernel kernel;
    // Construct_vertex_3 construct_vertex = kernel.construct_vertex_3_object();

    const Cell_handle& ch = fei->first;
    int i1 = fei->second;
    int i2 = fei->third;
    const Vertex_handle& v1 = ch->vertex(i1);
    const Vertex_handle& v2 = ch->vertex(i2);

    const Vertex_data& vd1 = v1->info();

    if (are_generated_color()) {
      const Vector3f& c1 = vd1.get_color();
      glColor3fv((float*)&c1);
    }

    const Vector3f& p1 = vd1.get_coord();
    glVertex3fv((float*)&p1);

    const Vertex_data& vd2 = v2->info();

    if (are_generated_color()) {
      const Vector3f& c2 = vd2.get_color();
      glColor3fv((float*)&c2);
    }

    const Vector3f& p2 = vd2.get_coord();
    glVertex3fv((float*)&p2);
  }
  glEnd();

  if (are_generated_color()) {
    glColor3f(1.0f, 1.0f, 1.0f);
    context->draw_light_enable(true);
  }
}

//! \brief
void Triangulation_geo::isect(SGAL::Isect_action* /* action */) {}

//! \brief
Boolean Triangulation_geo::clean_sphere_bound()
{
  if (is_dirty()) clean();
  if (m_bb_is_pre_set) return true;

  Approximate_sphere_vector spheres;
  if (m_triangulation.number_of_vertices() > 0) {
    spheres.resize(m_triangulation.number_of_vertices());
    Convert_approximate_sphere convert;
    std::transform(m_triangulation.points_begin(),
                   m_triangulation.points_end(),
                   spheres.begin(), convert);
  }
  if (!spheres.empty()) {
    Min_sphere min_sphere(spheres.begin(), spheres.end());
    Vector3f center_vec;
    std::copy(min_sphere.center_cartesian_begin(),
              min_sphere.center_cartesian_end(),
              &center_vec[0]);
    m_sphere_bound.set_center(center_vec);
    m_sphere_bound.set_radius(min_sphere.radius());
  }
  m_dirty_sphere_bound = false;
  return true;
}

//! \brief sets the attributes of the object.
void Triangulation_geo::set_attributes(SGAL::Element* elem)
{
  SGAL::Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);

    if (name == "lineWidth") {
      sscanf(value.c_str(), "%f", &m_line_width);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawHaloed") {
      m_draw_haloed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Element::Shared_container cont = elem->get_value(cai);
    if (name == "coord") {
      Shared_coord_array coord_array =
        boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "color") {
      Shared_color_array color_array =
        boost::dynamic_pointer_cast<Color_array>(cont);
      set_color_array(color_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief updates the internal representation in a cascade chain.
void Triangulation_geo::coord_changed(const Field_info* /* field_info */)
{
  SGAL_TRACE_CODE(SGAL::Trace::POLYHEDRON,
                  for (Uint i = 0; i < m_coord_array->size(); ++i)
                    std::cout << (*m_coord_array)[i] << std::endl;);
  clear();
}

//! \brief sets the attributes of this node.
void Triangulation_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // coord
  Execution_function exec_func =
    static_cast<Execution_function>(&Triangulation_geo::coord_changed);
  Shared_container_handle_function coord_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Triangulation_geo::coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD,
                                                      "coord",
                                                      RULE_EXPOSED_FIELD,
                                                      coord_array_func,
                                                      exec_func));

  // lineWidth
  Float_handle_function line_width_func =
    static_cast<Float_handle_function>(&Triangulation_geo::line_width_handle);
  s_prototype->add_field_info(new SF_float(LINE_WIDTH,
                                           "lineWidth",
                                           RULE_EXPOSED_FIELD,
                                           line_width_func));

  // drawHaloed
  Boolean_handle_function draw_haloed_func =
    static_cast<Boolean_handle_function>
    (&Triangulation_geo::draw_haloed_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_HALOED,
                                          "drawHaloed",
                                          RULE_EXPOSED_FIELD,
                                          draw_haloed_func));

}

//! \brief deletes the prototype.
void Triangulation_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
SGAL::Container_proto* Triangulation_geo::get_prototype()
{
  if (!s_prototype) Triangulation_geo::init_prototype();
  return s_prototype;
}

//! \brief prints statistics.
void Triangulation_geo::print_stat()
{
  if (is_dirty()) clean();

  std::cout << "no. tetrahedrons: " << "blah"
            << std::endl;

  std::cout << "Construction took " << m_time << " seconds to compute"
            << std::endl;
}

SGAL_END_NAMESPACE
