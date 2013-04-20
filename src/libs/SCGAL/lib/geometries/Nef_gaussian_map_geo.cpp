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
// $Id: $
// $Revision: 7791 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#pragma warning ( disable : 4996 )
#endif

#include <CGAL/basic.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>

#include <vector>

#include "SCGAL/Nef_gaussian_map_geo.hpp"

#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
// #include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Nef_gaussian_map_geo::s_tag = "NefGaussianMap";
SGAL::Container_proto* Nef_gaussian_map_geo::s_prototype = 0;

REGISTER_TO_FACTORY(Nef_gaussian_map_geo, "Nef_gaussian_map_geo");

// Default values:
const Boolean Nef_gaussian_map_geo::s_def_draw_dual(false);
const Boolean Nef_gaussian_map_geo::s_def_draw_dual_sphere(true);
const Boolean Nef_gaussian_map_geo::s_def_draw_dual_opaque(false);
const Boolean Nef_gaussian_map_geo::s_def_draw_dual_haloed(true);
const Float Nef_gaussian_map_geo::s_def_dual_line_width = 1;
const Vector3f Nef_gaussian_map_geo::s_def_dual_line_color = Vector3f(1, 1, 1);
const Vector3f Nef_gaussian_map_geo::s_def_dual_sphere_color =
Vector3f(0.5f, 0.5f, 0.5f);
const Float Nef_gaussian_map_geo::s_def_delta_angle = 0.1f;

const Boolean Nef_gaussian_map_geo::s_def_draw_marked_vertex(false);
const Boolean Nef_gaussian_map_geo::s_def_draw_marked_edge(false);
const Boolean Nef_gaussian_map_geo::s_def_draw_marked_facet(false);
const Vector3f Nef_gaussian_map_geo::s_def_marked_vertex_color(0.5f, 0, 0);
const Vector3f Nef_gaussian_map_geo::s_def_marked_edge_color(0, 0.5f, 0);
const Vector3f Nef_gaussian_map_geo::s_def_marked_facet_color(0, 0, 0.5f);
const Float Nef_gaussian_map_geo::s_def_vertex_radius_scale(0.06f);
const Float Nef_gaussian_map_geo::s_def_edge_radius_scale(0.04f);

/*! Constructor. */
Nef_gaussian_map_geo::Nef_gaussian_map_geo(Boolean proto) :
  Mesh_set(proto),
  m_dirty_polyhedron(true),
  m_bb_is_pre_set(false),
  m_time(0),
  m_minkowski_sum(false),
  m_sphere(NULL),
  m_draw_dual(s_def_draw_dual), m_draw_primal(!s_def_draw_dual),
  m_draw_dual_opaque(s_def_draw_dual_opaque),
  m_draw_dual_haloed(s_def_draw_dual_haloed),
  m_dual_line_width(s_def_dual_line_width),
  m_dual_line_color(s_def_dual_line_color),
  m_dual_sphere_color(s_def_dual_sphere_color),
  m_delta_angle(s_def_delta_angle),
  m_draw_dual_sphere(s_def_draw_dual_sphere),
  m_draw_marked_vertex(s_def_draw_marked_vertex),
  m_draw_marked_edge(s_def_draw_marked_edge),
  m_draw_marked_facet(s_def_draw_marked_facet),
  m_marked_vertex_color(s_def_marked_vertex_color),
  m_marked_edge_color(s_def_marked_edge_color),
  m_marked_facet_color(s_def_marked_facet_color),
  m_vertex_radius_scale(s_def_vertex_radius_scale),
  m_edge_radius_scale(s_def_edge_radius_scale),
  m_marked_vertex_index(0),
  m_marked_edge_index(0),
  m_marked_facet_index(0)
{
  m_surface.set_nef_polyhedron(this);

  m_sphere = new Sphere();
  m_sphere->set_stacks(32);
  m_sphere->set_slices(32);
}

/*! Destructor. */
Nef_gaussian_map_geo::~Nef_gaussian_map_geo()
{
  if (m_sphere) {
    delete m_sphere;
    m_sphere = NULL;
  }
}

/*! \brief cleans the polyhedron data structure. */
void Nef_gaussian_map_geo::clean_polyhedron()
{
  // Construct the polyhedron:
  m_polyhedron.delegate(m_surface);
#if 0
  if (!m_polyhedron.normalized_border_is_valid()) {
    m_polyhedron.normalize_border();
  }
#else
  m_polyhedron.normalize_border();
#endif

  // Compute the plane equations:
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Plane_equation());

#if 0
  std::for_each(m_polyhedron.vertices_begin(), m_polyhedron.vertices_end(),
                Convert_inexact_point());
#endif

  m_dirty_polyhedron = false;
}

/*! \brief cleans the nef polyhedron data structure. */
void Nef_gaussian_map_geo::clean()
{
  clock_t start_time = clock();
  if (m_minkowski_sum) {
    Nef_gaussian_map_geo* ngm_geo1 = &*(*(m_ngm_nodes.begin()));
    Nef_gaussian_map_geo* ngm_geo2 = &*(*(++m_ngm_nodes.begin()));
    const Nef_gaussian_map& ngm1 = ngm_geo1->get_ngm();
    const Nef_gaussian_map& ngm2 = ngm_geo2->get_ngm();
    m_nef_gaussian_map.minkowski_sum(ngm1, ngm2);
    gausian_map_to_polyhedron_3<Polyhedron_traits,
      Polyhedron::HDS> Converter(m_nef_gaussian_map);

    m_polyhedron.clear();
    m_polyhedron.delegate(Converter); 
    std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                   m_polyhedron.planes_begin(), Plane_equation());
  } else {
    if (m_dirty_polyhedron) clean_polyhedron();
    m_nef_gaussian_map = Nef_gaussian_map(m_polyhedron);
  }

  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;

  Mesh_set::clean();
}

/*! \brief clears the internal representation. */
void Nef_gaussian_map_geo::clear()
{
  m_polyhedron.clear();
  m_dirty_polyhedron = true;
  m_nef_gaussian_map.clear();
  Mesh_set::clear();
}

/*! \brief */
void Nef_gaussian_map_geo::cull(SGAL::Cull_context& cull_context) {}

/*! \brief draws the intermediate polyhedron (for debugging purpose). */
void Nef_gaussian_map_geo::draw_polyhedron(Draw_action* action)
{
  if (m_dirty_polyhedron) clean_polyhedron();

  for (Polyhedron::Facet_const_iterator i = m_polyhedron.facets_begin();
       i != m_polyhedron.facets_end(); ++i)
  {
    Polyhedron::Halfedge_around_facet_const_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);

    const Vector_3& normal_3 = i->plane().orthogonal_vector();
    float x = static_cast<float>(CGAL::to_double(normal_3.x()));
    float y = static_cast<float>(CGAL::to_double(normal_3.y()));
    float z = static_cast<float>(CGAL::to_double(normal_3.z()));
    Vector3f normal(x, y, z);
    normal.normalize();
    glNormal3fv((float*)&normal);

    do {
      const Point_3& point = j->vertex()->point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      glVertex3f(x, y, z);
    } while (++j != i->facet_begin());

    glEnd();
  }
}

/*! \brief draws the dual edges. */
void Nef_gaussian_map_geo::draw_dual_edges()
{
  glColor3fv((float*)&m_dual_line_color);
  Nef_gaussian_map::SHalfedge_const_iterator sei;
  Uint i = 0;
  CGAL_forall_sedges(sei, m_nef_gaussian_map) {
    Nef_gaussian_map::Sphere_point src = sei->source()->point();
    float x_src = static_cast<float>(CGAL::to_double(src.x()));
    float y_src = static_cast<float>(CGAL::to_double(src.y()));
    float z_src = static_cast<float>(CGAL::to_double(src.z()));

    Nef_gaussian_map::Sphere_point tgt = sei->twin()->source()->point();
    float x_trg = static_cast<float>(CGAL::to_double(tgt.x()));
    float y_trg = static_cast<float>(CGAL::to_double(tgt.y()));
    float z_trg = static_cast<float>(CGAL::to_double(tgt.z()));

    Vector3f p_src(x_src, y_src, z_src);
    Vector3f p_trg(x_trg, y_trg, z_trg);
    p_src.normalize();
    p_trg.normalize();
    if (m_draw_marked_edge && (m_marked_edge_index == i)) {
      glColor3fv((float*)&m_marked_edge_color);
      draw_dual_marked_edge(p_src, p_trg);
      glColor3fv((float*)&m_dual_line_color);
    } else {
      Rotation rot;
      rot.make(p_src, p_trg);
      float angle = rot.get_angle();
    
      glBegin(GL_LINE_STRIP);
      glVertex3fv((float*)&p_src);
      for (float tmp_angle = m_delta_angle; tmp_angle < angle;
           tmp_angle += m_delta_angle)
      {
        rot.set_angle(tmp_angle);
        Vector3f tmp;
        rot.rotate(p_src, tmp);
        glVertex3fv((float*)&tmp);
      }
      glVertex3fv((float*)&p_trg);
      glEnd();
    }
    ++i;
  }
  glColor3f(1, 1, 1);
}

/*! \brief draws the dual marked facet, that is, the vertex. */
void Nef_gaussian_map_geo::draw_dual_marked_facet()
{
  glColor3fv((float*)&m_marked_facet_color);
  float radius = m_vertex_radius_scale;
  Nef_gaussian_map::SVertex_const_iterator svi;
  Uint i = 0;
  CGAL_forall_svertices(svi, m_nef_gaussian_map) {
    if (m_marked_facet_index == i) {
      float x = static_cast<float>(CGAL::to_double(svi->point().x()));
      float y = static_cast<float>(CGAL::to_double(svi->point().y()));
      float z = static_cast<float>(CGAL::to_double(svi->point().z()));
      Vector3f point(x, y, z);
      Rotation rot;
      Vector3f src(0, 0, 1);
      rot.make(src, point);
      float angle = 0;
      float delta = SGAL_TWO_PI / 16;
      glBegin(GL_POLYGON);
      Vector3f vec;
      vec[2] = 1;
      for (unsigned int j = 0; j < 16; ++j) {
        vec[0] = radius * cosf(angle);
        vec[1] = radius * sinf(angle);
        angle += delta;
        Vector3f tmp;
        rot.rotate(vec, tmp);
        glVertex3fv((float*)&tmp);
      }
      glEnd();
    }
    ++i;
  }
  glColor3f(1, 1, 1);
}

/*! \brief draws the dual marked edge. */
void Nef_gaussian_map_geo::draw_dual_marked_edge(Vector3f& src, Vector3f& trg)
{
  float radius = m_edge_radius_scale;
  Rotation rot;
  rot.make(src, trg);
  float angle = rot.get_angle();
  float tmp_angle;
  Vector3f ext, v0, v1, my_src(src);

  glBegin(GL_QUAD_STRIP);
  for (tmp_angle = m_delta_angle; tmp_angle < angle; tmp_angle += m_delta_angle)
  {
    rot.set_angle(tmp_angle);
    Vector3f my_trg;
    rot.rotate(src, my_trg);
    Vector3f dir;
    dir.sub(my_trg, my_src);
    dir.normalize();
    Vector3f perp;
    perp.cross(dir, my_src);
    ext.scale(radius, perp);
    v0.sub(my_src, ext);
    v1.add(my_src, ext);
    glVertex3fv((float*)&v0);
    glVertex3fv((float*)&v1);
    my_src = my_trg;
  }

  v0.sub(trg, ext);
  v1.add(trg, ext);  
  glVertex3fv((float*)&v0);
  glVertex3fv((float*)&v1);
  glEnd();
}

/*! \brief draws the dual marked vertex, that is a face. */
void Nef_gaussian_map_geo::draw_dual_marked_vertex()
{
  Uint i = 0;

  Nef_gaussian_map::SFace_const_iterator f;
  CGAL_forall_sfaces(f, m_nef_gaussian_map) {
    if (m_marked_vertex_index == i) {
      Nef_gaussian_map::SFace_cycle_const_iterator fc = 
        f->sface_cycles_begin();
      Nef_gaussian_map::SHalfedge_const_handle se(fc);

      // Find the center:
      Vector3f center;
      Uint j = 0;
      Nef_gaussian_map::SHalfedge_around_sface_const_circulator
        hc(se), hend(hc);      do {
        const Nef_gaussian_map::Sphere_point& p = hc->source()->point();
        float x = static_cast<float>(CGAL::to_double(p.x()));
        float y = static_cast<float>(CGAL::to_double(p.y()));
        float z = static_cast<float>(CGAL::to_double(p.z()));
        Vector3f vec(x, y, z);
        center.add(vec);
        ++hc;
        ++j;
      } while(hc != hend);
      if (j == 0) break;
      center.scale(1.0 / static_cast<float>(j));
      center.normalize();

      // Draw the fan:
      // glFrontFace(GL_CCW);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3fv((float*)&center);
      hc = se;
      do {
        const Nef_gaussian_map::Sphere_point& p = hc->source()->point();
        float x = static_cast<float>(CGAL::to_double(p.x()));
        float y = static_cast<float>(CGAL::to_double(p.y()));
        float z = static_cast<float>(CGAL::to_double(p.z()));
        Vector3f vec(x, y, z);
        vec.normalize();
        glVertex3fv((float*)&vec);
        ++hc;
      } while(hc != hend);
      const Nef_gaussian_map::Sphere_point& p = hc->source()->point();
      float x = static_cast<float>(CGAL::to_double(p.x()));
      float y = static_cast<float>(CGAL::to_double(p.y()));
      float z = static_cast<float>(CGAL::to_double(p.z()));
      Vector3f vec(x, y, z);
      vec.normalize();
      glVertex3fv((float*)&vec);
      glEnd();
    }
    ++i;
  }
}

/*! \brief draws the Gaussian map. */
void Nef_gaussian_map_geo::draw_dual(Draw_action* action)
{
  if (m_draw_dual_opaque) {
    draw_dual_opaque(action);
    return;
  }

  Context* context = action->get_context();

  context->draw_depth_enable(false);

  if (m_draw_dual_sphere) {
    context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
    glColor4f(m_dual_sphere_color[0], m_dual_sphere_color[1],
              m_dual_sphere_color[2], 0.5f);
    context->draw_transp_enable(true);
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
    context->draw_cull_face(SGAL::Gfx::FRONT_CULL);
    m_sphere->draw(action);
    context->draw_light_model_sides(Gfx::ONE_SIDE);
    context->draw_cull_face(SGAL::Gfx::BACK_CULL);
    m_sphere->draw(action);
    context->draw_transp_enable(false);
    context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
  }
  
  context->draw_light_enable(false);

  if (!m_draw_dual_haloed) {
    context->draw_line_width(m_dual_line_width);
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
    context->draw_cull_face(SGAL::Gfx::NO_CULL);
    draw_dual_edges();
    context->draw_cull_face(SGAL::Gfx::BACK_CULL);
    context->draw_light_model_sides(SGAL::Gfx::ONE_SIDE);
    context->draw_depth_enable(true);
  } else {
    context->draw_depth_enable(true);
    context->draw_line_width(m_dual_line_width * 8);
    context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
    glDepthRange (0.1, 1.0);
    float save_radius = m_edge_radius_scale;
    m_edge_radius_scale *= 1.5;
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
    context->draw_cull_face(SGAL::Gfx::NO_CULL);
    draw_dual_edges();
    context->draw_cull_face(SGAL::Gfx::BACK_CULL);
    context->draw_light_model_sides(SGAL::Gfx::ONE_SIDE);
    m_edge_radius_scale = save_radius;

    context->draw_line_width(m_dual_line_width);
    glDepthRange(0, 1.0);
    context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));    
    context->draw_depth_func(Gfx::LEQUAL_DFUNC);
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
    context->draw_cull_face(SGAL::Gfx::NO_CULL);
    draw_dual_edges();
    context->draw_cull_face(SGAL::Gfx::BACK_CULL);
    context->draw_light_model_sides(SGAL::Gfx::ONE_SIDE);
    context->draw_depth_func(Gfx::LESS_DFUNC);
  }

  // Draw the dual vertices
  if (m_draw_marked_facet) {
    glDepthRange (-0.2, 0.8);
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
    context->draw_cull_face(SGAL::Gfx::NO_CULL);
    draw_dual_marked_facet();
    context->draw_cull_face(SGAL::Gfx::BACK_CULL);
    context->draw_light_model_sides(SGAL::Gfx::ONE_SIDE);
    glDepthRange(0, 1.0);
  }

  // Draw the dual facets
  if (m_draw_marked_vertex) {
    glColor3fv((float*)&m_marked_vertex_color);
    draw_dual_marked_vertex();
    glColor3f(1, 1, 1);
  }
    
  context->draw_line_width(1.0f);
  context->draw_light_enable(true);
}

/*! \brief draws the dual representation opaque */
void Nef_gaussian_map_geo::draw_dual_opaque(Draw_action* action)
{
  Context* context = action->get_context();

  if (m_draw_dual_sphere) {
    // \todo why is it necessary? should be the default.
    context->draw_cull_face(SGAL::Gfx::BACK_CULL);  
    context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
    glColor4f(m_dual_sphere_color[0], m_dual_sphere_color[1],
              m_dual_sphere_color[2], 0.5f);
    // \todo Move to context
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2., 2.);
    m_sphere->draw(action);
    glDisable(GL_POLYGON_OFFSET_FILL);
    context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
  }
  
  context->draw_light_enable(false);
  glColor3fv((float*)&m_dual_line_color);
  context->draw_line_width(m_dual_line_width);

  draw_dual_edges();

  context->draw_line_width(1.0f);
  context->draw_light_enable(true);
  glColor3f(1, 1, 1);
}

/*! \brief draws the polyhedron directly from the gaussian map representation.
 */
void Nef_gaussian_map_geo::draw_primal(Draw_action* action)
{
  // draw_polyhedron(action);

  //  TRACE_MSG(SGAL::Trace::POLYHEDRON, "Polyhedron_geo::draw_geometry ... ");
  for (Facet_iterator i = m_polyhedron.facets_begin();
       i != m_polyhedron.facets_end(); ++i)
  {
    Halfedge_facet_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);
    Vector_3 normal = i->plane().orthogonal_vector();
    Vector3f n(static_cast<float>(CGAL::to_double(normal.x())), 
               static_cast<float>(CGAL::to_double(normal.y())), 
               static_cast<float>(CGAL::to_double(normal.z())));
    n.normalize();
    glNormal3fv((float*)&n);
    do {
      Point_3& point = j->vertex()->point();
      glVertex3f(static_cast<float>(CGAL::to_double(point.x())), 
                 static_cast<float>(CGAL::to_double(point.y())), 
                 static_cast<float>(CGAL::to_double(point.z())));
    } while (++j != i->facet_begin());
    glEnd();
  }

  /*
  for (Nef_halffacet_iterator f = m_nef_polyhedron.halffacets_begin();
       f != m_nef_polyhedron.halffacets_end(); ++f)
  {
    if (f->incident_volume() != m_nef_polyhedron.volumes_begin()) continue;

    // \todo do not ignore the holes!
    Nef_polyhedron_3::SHalfedge_const_handle se = f->facet_cycles_begin();
    Nef_polyhedron_3::SHalfedge_around_facet_const_circulator j = se;
    Nef_polyhedron_3::SHalfedge_around_facet_const_circulator j_begin = j;
    // Ensure facets are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);

    glBegin(GL_POLYGON);
    
    Vector_3 normal = f->plane().orthogonal_vector();
    float x = CGAL::to_double(normal.x());
    float y = CGAL::to_double(normal.y());
    float z = CGAL::to_double(normal.z());
    Vector3f normal3f(x,y,z);
    normal3f.normalize();
    glNormal3fv((float*)&normal3f);

    do {
      const Point_3& point = j->source()->source()->point();
      float x = CGAL::to_double(point.x());
      float y = CGAL::to_double(point.y());
      float z = CGAL::to_double(point.z());
      glVertex3f(x, y, z);
    } while (++j != j_begin);
    glEnd();
  }
  */
}

/*! \brief draws the nef polyhedron. */
void Nef_gaussian_map_geo::draw_geometry(SGAL::Draw_action* action)
{
  if (m_draw_dual) draw_dual(action);
  else draw_primal(action);
}

/*! \brief */
void Nef_gaussian_map_geo::isect(SGAL::Isect_action* action)
{
  if (is_dirty()) clean();
  if (is_empty()) return;

  for (Facet_iterator i = m_polyhedron.facets_begin();
       i != m_polyhedron.facets_end(); ++i)
  {
    Halfedge_facet_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);
    do {
      Point_3& point = j->vertex()->point();
      glVertex3f(static_cast<float>(CGAL::to_double(point.x())), 
                 static_cast<float>(CGAL::to_double(point.y())), 
                 static_cast<float>(CGAL::to_double(point.z())));
    } while (++j != i->facet_begin());
    glEnd();
  }
}

/*! \brief calculates the bounding sphere. */
Boolean Nef_gaussian_map_geo::clean_sphere_bound()
{
  if (!m_dirty_sphere_bound) return false;
  if (is_dirty()) clean();
  if (m_bb_is_pre_set) return true;
  
  Inexact_sphere_vector spheres;
  if (!m_polyhedron.empty()) {
    spheres.resize(m_polyhedron.size_of_vertices());
    Convert_inexact_sphere convert;
    std::transform(m_polyhedron.vertices_begin(),
                   m_polyhedron.vertices_end(),
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

/*! \brief sets the attributes of this object. */
void Nef_gaussian_map_geo::set_attributes(Element* elem)
{
  SGAL::Mesh_set::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "drawDual") {
      m_draw_dual = compare_to_true(value);
      m_draw_primal = !m_draw_dual;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualSphere") {
      m_draw_dual_sphere = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "deltaAngle") {
      m_delta_angle = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualLineWidth") {
      sscanf(value.c_str(), "%f", &m_dual_line_width);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualSphereColor") {
      Vector3f col(value);
      set_dual_sphere_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualLineColor") {
      Vector3f col(value);
      set_dual_line_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualOpaque") {
      m_draw_dual_opaque = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualHaloed") {
      m_draw_dual_haloed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawMarkedVertex") {
      m_draw_marked_vertex = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawMarkedEdge") {
      m_draw_marked_edge = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawMarkedFacet") {
      m_draw_marked_facet = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedVertexColor") {
      Vector3f col(value);
      set_marked_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedEdgeColor") {
      Vector3f col(value);
      set_marked_edge_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedFaceColor") {
      Vector3f col(value);
      set_marked_facet_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedVertexIndex") {
      set_marked_vertex_index(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedEdgeIndex") {
      set_marked_edge_index(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedFacetIndex") {
      set_marked_facet_index(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;
  
  // Sets the multi-container attributes of this node:
  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "geometries") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Element::Shared_container cont = *ci;
        Shared_nef_gaussian_map_geo ngm =
          boost::dynamic_pointer_cast<Nef_gaussian_map_geo>(cont);
        insert_ngm(ngm);
      }
      m_minkowski_sum = true;
      elem->mark_delete(mcai);
    }
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief sets the attributes of this node. */
void Nef_gaussian_map_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new SGAL::Container_proto(SGAL::Mesh_set::get_prototype());

  //! Container execution function
  typedef void (SGAL::Container::* Execution_function)(SGAL::Field_info*);
  Execution_function exec_func;

  // Add the object fields to the prototype
  exec_func =
    static_cast<Execution_function>(&Nef_gaussian_map_geo::draw_changed);
  s_prototype->add_field_info(new SF_bool(DRAW_DUAL, "drawDual",
                                          get_member_offset(&m_draw_dual),
                                          exec_func));
 
  s_prototype->
    add_field_info(new SF_bool(DRAW_DUAL_SPHERE, "drawDualSphere",
                               get_member_offset(&m_draw_dual_sphere)));

  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_PRIMAL, "trueDrawPrimal",
                                          get_member_offset(&m_draw_primal)));

 s_prototype->add_field_info(new SF_bool(TRUE_DRAW_DUAL, "trueDrawDual",
                                         get_member_offset(&m_draw_dual)));

  s_prototype->
    add_field_info(new SF_bool(DRAW_DUAL_OPAQUE, "drawDualOpaque",
                               get_member_offset(&m_draw_dual_opaque)));
  
  s_prototype->
    add_field_info(new SF_bool(DRAW_DUAL_HALOED, "drawDualHaloed",
                               get_member_offset(&m_draw_dual_haloed)));

  exec_func =
    static_cast<Execution_function>(&Nef_gaussian_map_geo::
                                    increase_vertex_index);
  s_prototype->
    add_field_info(new SF_bool(INCREASE_VERTEX_INDEX, "increaseVertexIndex",
                               get_member_offset(&m_increase_vertex_index),
                               exec_func));
  exec_func =
    static_cast<Execution_function>(&Nef_gaussian_map_geo::
                                    increase_edge_index);
  s_prototype->
    add_field_info(new SF_bool(INCREASE_EDGE_INDEX, "increaseEdgeIndex",
                               get_member_offset(&m_increase_edge_index),
                               exec_func));
  exec_func =
    static_cast<Execution_function>(&Nef_gaussian_map_geo::
                                    increase_facet_index);
  s_prototype->
    add_field_info(new SF_bool(INCREASE_FACET_INDEX, "increaseFacetIndex",
                               get_member_offset(&m_increase_facet_index),
                               exec_func));

  MF_container* field = new MF_container(GEOMETRIES, "geometries",
                                         get_member_offset(&m_ngm_nodes));
  s_prototype->add_field_info(field);
}

/*! \brief */
void Nef_gaussian_map_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = 0;
}

/*! \brief */
SGAL::Container_proto* Nef_gaussian_map_geo::get_prototype() 
{
  if (!s_prototype) Nef_gaussian_map_geo::init_prototype();
  return s_prototype;
}

/*! \brief prints statistics. */
void Nef_gaussian_map_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty()) clean();
  std::cout << "Construction took " << m_time << " seconds to compute"
            << std::endl;
}

/*! \brief raises the flag that indicates that the sphere bound changed. */
void Nef_gaussian_map_geo::draw_changed(Field_info* /* field_info */)
{
  m_draw_primal = !m_draw_dual;
  m_dirty_sphere_bound = true;

  if (m_draw_dual) {
    Field* field = get_field(TRUE_DRAW_DUAL);
    if (field) field->cascade();
  } else {
    Field* field = get_field(TRUE_DRAW_PRIMAL);
    if (field) field->cascade();
  }
}

/*! \brief increase the vertex index. */
void Nef_gaussian_map_geo::increase_vertex_index(Field_info* field_info)
{
  m_marked_vertex_index++;
  if (m_marked_vertex_index == m_nef_gaussian_map.number_of_sfaces())
    m_marked_vertex_index = 0;
}

/*! \brief increase the face index. */
void Nef_gaussian_map_geo::increase_edge_index(Field_info* field_info)
{
  m_marked_edge_index++;
  if (m_marked_edge_index == m_nef_gaussian_map.number_of_sedges())
    m_marked_edge_index = 0;
}

/*! \brief increases the face index. */
void Nef_gaussian_map_geo::increase_facet_index(Field_info* field_info)
{
  m_marked_facet_index++;
  if (m_marked_facet_index == m_nef_gaussian_map.number_of_svertices())
    m_marked_facet_index = 0;
}

/*! \brief obtains he Nef_gaussian_map. */
Nef_gaussian_map_geo::Nef_gaussian_map& Nef_gaussian_map_geo::get_ngm()
{
  if (m_dirty) clean();
  return m_nef_gaussian_map;
}

SGAL_END_NAMESPACE
