// Copyright (c) 2008 Israel.
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
// $Id: Assembly.cpp 14211 2012-11-26 16:46:31Z efif $
// $Revision: 14211 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )

#endif

#include <stdio.h> // Workaround for CGAL 4.1 using EOF.

#include <time.h>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <boost/config.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_matrix.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_spherical_gaussian_map_3.h>
#include <CGAL/IO/Arr_iostream.h>

//! \todo replace with direct computation of reflection.
#include <CGAL/convex_hull_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Switch.hpp"

#include "SCGAL/Assembly.hpp"
#include "SCGAL/Assembly_part.hpp"
#include "SCGAL/Spherical_gaussian_map.hpp"
#include "SCGAL/Exact_coord_array.hpp"
#include "SCGAL/Arrangement_on_sphere_marked_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_graph_geo.hpp"
#include "SCGAL/Arrangement_marked_overlay_traits.hpp"
#include "SCGAL/Arrangement_graph_overlay_traits.hpp"
#include "SCGAL/Sgm_color_overlay_traits.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Assembly::s_prototype = NULL;
const std::string Assembly::s_tag = "Assembly";

REGISTER_TO_FACTORY(Assembly, "Assembly");

/*! Constructor. */
Assembly::Assembly(Boolean proto) :
  Group(proto),
  m_dirty(true),
  m_trigger(false),
  m_number_of_parts(0),
  m_aos_graph(NULL),
  m_draw_alternate(false),
  m_inc_alternate(false),
  m_draw_aos_minkowski_sums(false),
  m_inc_minkowski_sums(false)
{
  if (proto) return;
  m_appearance.reset(new Appearance);
  m_material.reset(new Material);
  m_appearance->set_material(m_material);
  m_material->set_transparency(0.0001f);

  m_sphere_appearance.reset(new Appearance);
  m_sphere_material.reset(new Material);
  m_sphere_appearance->set_material(m_sphere_material);
  m_sphere_material->set_ambient_intensity(0.7f);
  m_sphere_material->set_specular_color(0.5f, 0.5f, 0.5f);
  m_sphere_material->set_diffuse_color(0.5f, 0.5f, 0.5f);
  m_sphere_material->set_transparency(0.5f);

  m_switch.reset(new Switch);

  m_sgm_geo_node.reset(new Switch);
  m_switch->add_child(m_sgm_geo_node);

  m_reflected_sgm_geo_node.reset(new Switch);
  m_switch->add_child(m_reflected_sgm_geo_node);

  m_ms_sgm_geo_node.reset(new Switch);
  m_switch->add_child(m_ms_sgm_geo_node);

  m_projection_aos_geo_node.reset(new Switch);
  m_switch->add_child(m_projection_aos_geo_node);

  m_part_projection_aos_geo_node.reset(new Switch);
  m_switch->add_child(m_part_projection_aos_geo_node);

  m_graph_node.reset(new Shape);
  m_switch->add_child(m_graph_node);

  m_switch->set_which_choice(0);

  m_sgm_geo_node->set_which_choice(0);
  m_reflected_sgm_geo_node->set_which_choice(0);
  m_ms_sgm_geo_node->set_which_choice(0);
  m_projection_aos_geo_node->set_which_choice(0);
  m_part_projection_aos_geo_node->set_which_choice(0);
}

/*! Destructor. */
Assembly::~Assembly()
{
  remove_child(m_switch);
  m_childs = m_childs_save;
  clear();
}

/*! \brief clears the representation. */
void Assembly::clear()
{
  m_parts.clear();                      // clear container of parts

  // Clear the geometry nodes:

  // Clear the reflection nodes + geometry children

  // Clear the Minkowski-sum Gaussian-map node + geometry children:
  Uint i;
  for (i = 0; i < m_ms_sgm_geo_node->children_size(); ++i) {
    Shared_node node = m_ms_sgm_geo_node->get_child(i);
    m_ms_sgm_geo_node->remove_child(node);
  }

  // Clear the Minkowski-sum projection Aos node + geometry children:
  for (i = 0; i < m_projection_aos_geo_node->children_size(); ++i) {
    Shared_node node = m_projection_aos_geo_node->get_child(i);
    m_projection_aos_geo_node->remove_child(node);
  }

  // Clear the per-part Minkowski-sum projection Aos node + geometry children:
  for (i = 0; i < m_part_projection_aos_geo_node->children_size(); ++i) {
    Shared_node node = m_part_projection_aos_geo_node->get_child(i);
    m_part_projection_aos_geo_node->remove_child(node);
  }

  // Clear map of Minkowski sum lists:
  // Clear map of projection lists:
  // Clear map of projections (union per part)

  // Clear the solutions:
  Solutions::const_iterator it;
  for (it = m_solutions.begin(); it != m_solutions.end(); ++it) delete *it;
  m_solutions.clear();
}

/*! \brief initializes the node prototype. */
void Assembly::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Assembly::solve);

  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          get_member_offset(&m_trigger),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Assembly::draw_alt_changed);
  s_prototype->
    add_field_info(new SF_bool(DRAW_ALT, "drawAlternate",
                               get_member_offset(&m_draw_alternate),
                               exec_func));

  exec_func =
    static_cast<Execution_function>(&Assembly::inc_alt_changed);
  s_prototype->
    add_field_info(new SF_bool(INC_ALT, "incAlternate",
                               get_member_offset(&m_inc_alternate),
                               exec_func));

  exec_func =
    static_cast<Execution_function>(&Assembly::draw_aos_minkowski_sums_changed);
  s_prototype->
    add_field_info(new SF_bool(DRAW_AOS_MINKOWSKI_SUMS, "drawAosMinkowskiSums",
                               get_member_offset(&m_draw_aos_minkowski_sums),
                               exec_func));

  exec_func =
    static_cast<Execution_function>(&Assembly::inc_minkowski_sums_changed);
  s_prototype->
    add_field_info(new SF_bool(INC_MINKOWSKI_SUMS, "incMinkowskiSums",
                               get_member_offset(&m_inc_minkowski_sums),
                               exec_func));
}

/*! \brief deletes the node prototype. */
void Assembly::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Assembly::get_prototype()
{
  if (!s_prototype) Assembly::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Assembly::set_attributes(Element* elem) { Group::set_attributes(elem); }

/*! \brief assigns each part a unique non-negative number. */
Uint Assembly::assign_id(Node* node, Uint id) const
{
  Assembly_part* part = dynamic_cast<Assembly_part*>(node);
  if (part) {
    part->set_id(id++);
    return id;
  }

  Switch* my_switch = dynamic_cast<Switch*>(node);
  if (my_switch) {
    Shared_node choice = my_switch->get_choice();
    if (!choice) return id;
    return assign_id(&*choice, id);
  }

  Group* group = dynamic_cast<Group*>(node);
  if (group) return assign_id(group, id);

  return id;
}

/*! \brief assigns each part a unique non-negative number. */
Uint Assembly::assign_id(Group* group, Uint id) const
{
  Node_const_iterator it;
  for (it = group->children_begin(); it != group->children_end(); ++it) {
    Shared_node node = *it;
    id = assign_id(&*node, id);
  }
  return id;
}

/*! \brief prints information to an output stream. */
void Assembly::print_info(std::ostream& out)
{
  out << "Number of parts: " << m_number_of_parts << std::endl;
  out << "Number of parts: " << m_parts.size() << std::endl;

  Assembly_part_iter ppit;
  for (ppit = m_parts.begin(); ppit != m_parts.end(); ++ppit) {
    Assembly_part* part = *ppit;
    Sgm_geo_list& sgm_geos = part->get_sgm_geos();
    out << "size: " << sgm_geos.size() << std::endl;
  }

  Minkowski_sum_list_iter msli;
  std::cout << "mk size: " << m_minkowski_sum_lists.size() << std::endl;
  for (msli = m_minkowski_sum_lists.begin();
       msli != m_minkowski_sum_lists.end();
       ++msli)
  {
    Key key = (*msli).first;
    Sgm_list& sgm_list = (*msli).second;
    std::cout << key.first << "," << key.second << " size: "
              << sgm_list.size() << std::endl;
  }

  std::cout << "Number of projection lists: " << m_projection_lists.size()
            << std::endl;
}

/*! \brief solves the puzzle. */
void Assembly::solve(Field_info* /* field_info */)
{
  if (m_dirty) clean();
}

/*! \brief constructs the lists of the reflected sgm geometry nodes. */
void Assembly::construct_reflected_sgms()
{
  clock_t start_time = clock();
  Sgm_geo_list_iter slit;
  Exact_kernel kernel;
  Exact_kernel::Construct_translated_point_3 tpoint =
    kernel.construct_translated_point_3_object();
  Exact_kernel::Construct_vector_3 cvec = kernel.construct_vector_3_object();

  Assembly_part_iter ppit;
  for (ppit = m_parts.begin(); ppit != m_parts.end(); ++ppit) {
    Assembly_part* part = *ppit;
    Sgm_geo_list& sgm_geos = part->get_sgm_geos();
    Sgm_geo_iter sit;
    m_parts_reflected_sgm_geos.push_back(Sgm_geo_list());
    Sgm_geo_list& part_reflected_sgm_geos = m_parts_reflected_sgm_geos.back();
    for (sit = sgm_geos.begin(); sit != sgm_geos.end(); ++sit) {
      Shared_sgm_geo sgm_geo = *sit;
      Shared_sgm_geo reflected_sgm_geo(new Sgm_geo);
      /*! \todo Replace the code that computes the reflection about the origin
       * from the input coordinate array, or from the input polyhedron with new
       * code that computes it directly from the Sgm.
       */
      typedef boost::shared_ptr<Exact_coord_array>    Shared_exact_coord_array;
      Sgm_geo::Shared_coord_array tmp = sgm_geo->get_coord_array();
      Shared_exact_coord_array coord_array =
        boost::dynamic_pointer_cast<Exact_coord_array>(tmp);
      if (coord_array) {
        Uint size  = coord_array->size();
        Shared_exact_coord_array
          inverse_coord_array(new Exact_coord_array(size));
        Exact_coord_array::Exact_point_const_iter its;
        Exact_coord_array::Exact_point_iter itt = inverse_coord_array->begin();
        for (its = coord_array->begin(); its != coord_array->end(); ++its) {
          const Exact_point_3& point = *its;
          Exact_vector_3 vec = cvec(point, CGAL::ORIGIN);
          *itt++ = tpoint(CGAL::ORIGIN, vec);
        }
        reflected_sgm_geo->set_coord_array(inverse_coord_array);
        reflected_sgm_geo->set_primitive_type(sgm_geo->get_primitive_type());
        reflected_sgm_geo->set_num_primitives(sgm_geo->get_num_primitives());
        const Array<Uint>& indices = sgm_geo->get_coord_indices();
        if (sgm_geo->are_coord_indices_flat())
          reflected_sgm_geo->set_reverse_flat_coord_indices(indices);
        else reflected_sgm_geo->set_reverse_coord_indices(indices);
      }
      else {
        Sgm_geo::Polyhedron* reflected_polyhedron = new Sgm_geo::Polyhedron;
        Spherical_gaussian_map_colored* sgm = sgm_geo->get_sgm();
        std::vector<Exact_point_3> inverse_points;
        inverse_points.resize(sgm->number_of_faces());
        unsigned int i = 0;
        Sgm_colored_face_const_iterator fit;
        for (fit = sgm->faces_begin(); fit != sgm->faces_end(); ++fit) {
          Exact_vector_3 vec = cvec(fit->point(), CGAL::ORIGIN);
          inverse_points[i++] = tpoint(CGAL::ORIGIN, vec);
        }
        CGAL::convex_hull_3(inverse_points.begin(), inverse_points.end(),
                            *reflected_polyhedron);
        inverse_points.clear();
        reflected_sgm_geo->set_polyhedron(reflected_polyhedron);
      }
      part_reflected_sgm_geos.push_back(reflected_sgm_geo);
    }
  }
  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Sub-part Gaussian map reflection: "
            << duration_time << " seconds."
            << std::endl;
}

/*! \brief computes the Minkowski sums. */
void Assembly::compute_minkowski_sums()
{
  clock_t start_time = clock();
  Uint i = 0;

  Assembly_part_iter ppit;
  for (ppit = m_parts.begin(); ppit != m_parts.end(); ++ppit, ++i) {
    Assembly_part* part = *ppit;
    Sgm_geo_list& sgm_geos1 = part->get_sgm_geos();
    Uint j = 0;
    Sgm_geo_list_iter slit2;
    for (slit2 = m_parts_reflected_sgm_geos.begin();
         slit2 != m_parts_reflected_sgm_geos.end();
         ++slit2, ++j)
    {
      if (i == j) continue;

      Sgm_geo_list& sgm_geos2 = *slit2;

      Key key = std::make_pair(i, j);
      m_minkowski_sum_lists[key] = Sgm_list();
      Sgm_list& sgms = m_minkowski_sum_lists[key];

      Sgm_geo_iter sit1;
      for (sit1 = sgm_geos1.begin(); sit1 != sgm_geos1.end(); ++sit1) {
        Shared_sgm_geo sgm_geo1 = *sit1;
        Sgm* sgm1 = sgm_geo1->get_sgm();

        Sgm_geo_iter sit2;
        for (sit2 = sgm_geos2.begin(); sit2 != sgm_geos2.end(); ++sit2) {
          Shared_sgm_geo sgm_geo2 = *sit2;
          Sgm* sgm2 = sgm_geo2->get_sgm();

          Sgm* sgm = new Sgm;
          Sgm_color_overlay_traits<Sgm> sgm_overlay;
          sgm->minkowski_sum(*sgm1, *sgm2, sgm_overlay);
          sgms.push_back(sgm);
        }
      }
    }
  }
  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Pairwise sub-part Minkowski sum construction: "
            << duration_time << " seconds."
            << std::endl;
}

/* \brief constructs the sgm geometry nodes. */
void Assembly::construct_sgms_nodes()
{
  clock_t start_time = clock();

  Assembly_part_iter ppit;
  for (ppit = m_parts.begin(); ppit != m_parts.end(); ++ppit) {
    Assembly_part* part = *ppit;
    Sgm_geo_list& sgm_geos = part->get_sgm_geos();
    Appearance_list& sgm_apps = part->get_sgm_apps();
    Sgm_geo_iter sgit;
    Appearance_iter apit = sgm_apps.begin();
    for (sgit = sgm_geos.begin(); sgit != sgm_geos.end(); ++sgit, ++apit) {
      SGAL_assertion(apit != sgm_apps.end());
      Shared_sgm_geo sgm_geo = *sgit;
      Shared_shape shape(new Shape());
      shape->set_appearance(*apit);
      shape->set_geometry(sgm_geo);
      m_sgm_geo_node->add_child(shape);
      // sgm_geo->set_aos_vertex_style(Arrangement_renderer::Vertex_shape::NONE);
      // sgm_geo->set_draw_aos(true);
    }
  }

  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Sub-part Gaussian map construction: "
            << duration_time << " seconds."
            << std::endl;
}

/* \brief constructs the reflected sgm geometry nodes. */
void Assembly::construct_reflected_sgms_nodes()
{
  Sgm_geo_list_iter rslit = m_parts_reflected_sgm_geos.begin();
  Assembly_part_iter ppit;
  for (ppit = m_parts.begin(); ppit != m_parts.end(); ++ppit, ++rslit) {
    Assembly_part* part = *ppit;
    Sgm_geo_list& sgm_geos = part->get_sgm_geos();
    Sgm_geo_list& reflected_sgm_geos = *rslit;

    Sgm_geo_iter rsgit = reflected_sgm_geos.begin();
    Sgm_geo_iter sgit;
    for (sgit = sgm_geos.begin(); sgit != sgm_geos.end(); ++sgit, ++rsgit) {
      Shared_sgm_geo sgm_geo = *sgit;
      Sgm* reflected_sgm = (*rsgit)->get_sgm();
      Shared_shape shape(new Shape);
      shape->set_appearance(m_appearance);
      m_reflected_sgm_geo_node->add_child(shape);
      Shared_sgm_geo reflected_sgm_geo(new Sgm_geo);
      shape->set_geometry(reflected_sgm_geo);
      reflected_sgm_geo->set_sgm(reflected_sgm);
      reflected_sgm_geo->set_aos_vertex_style
        (Arrangement_renderer::Vertex_shape::NONE);
      reflected_sgm_geo->set_draw_aos(true);

      const Vector3f& color = sgm_geo->get_aos_edge_color();
      reflected_sgm_geo->set_aos_edge_color(color);
      Sgm::Halfedge_iterator hei;
      for (hei = reflected_sgm->halfedges_begin();
           hei != reflected_sgm->halfedges_end(); ++hei)
        hei->set_color(color);
    }
  }
}

/*! \brief constructs the Minkowski-sum nodes. */
void Assembly::construct_minkowski_sum_nodes()
{
  Minkowski_sum_list_iter msli;
  for (msli = m_minkowski_sum_lists.begin();
       msli != m_minkowski_sum_lists.end();
       ++msli)
  {
    Sgm_list& sgm_list = (*msli).second;
    Sgm_iter sit;
    for (sit = sgm_list.begin(); sit != sgm_list.end(); ++sit) {
      Sgm* sgm = *sit;

      Shared_shape shape(new Shape());
      shape->set_appearance(m_appearance);
      m_ms_sgm_geo_node->add_child(shape);
      Shared_sgm_geo sgm_geo(new Sgm_geo);
      shape->set_geometry(sgm_geo);
      sgm_geo->set_minkowski_sum(true);
      sgm_geo->set_sgm(sgm);
      sgm_geo->update_facets();
      Vector3f color(0, 0, 0);
      sgm_geo->set_aos_vertex_color(color);
    }
  }
}

/*! \brief constructs the Minkowski-sum projection nodes */
void Assembly::construct_projection_nodes()
{
  Projection_list_iter pli;
  for (pli = m_projection_lists.begin(); pli != m_projection_lists.end(); ++pli)
  {
    Aos_list& aos_list = (*pli).second;
    Aos_iter ait;
    for (ait = aos_list.begin(); ait != aos_list.end(); ++ait) {
      Aos_mark* aos = *ait;

      Shared_shape shape(new Shape());
      shape->set_appearance(m_appearance);
      //shape->set_draw_depth(false);
      m_projection_aos_geo_node->add_child(shape);
      typedef boost::shared_ptr<Arrangement_on_sphere_marked_geo>
        Shared_arrangement_on_sphere_marked_geo;
      Shared_arrangement_on_sphere_marked_geo
        aos_geo(new Arrangement_on_sphere_marked_geo);
      shape->set_geometry(aos_geo);
      aos_geo->set_aos(aos);
      aos_geo->set_aos_vertex_style(Arrangement_renderer::Vertex_shape::DISC);
      aos_geo->set_aos_edge_style(Arrangement_renderer::Edge_shape::STRIP);
      Vector3f color(0, 0, 0);
      aos_geo->set_aos_vertex_color(color);
      aos_geo->set_aos_edge_color(color);

      aos_geo->set_aos_vertex_radius(0.03f);
    }
  }
}

/*! \brief computes the projection of a convex polyhedron represented by a
 * spherical Gaussian map.
 */
void Assembly::compute_projection(Sgm::Face_const_handle fit, Aos_mark* aos)
{
  /*! Traverse all the adjacent Gaussian-map faces, that is the adjacent
   * polytope vertices, and project them.
   */
  Sgm::Halfedge_const_iterator first;
  if (fit->number_of_outer_ccbs() == 1) {
    Sgm::Outer_ccb_const_iterator oit = fit->outer_ccbs_begin();
    first = *oit;
  } else {
    SGAL_assertion(fit->number_of_inner_ccbs() == 1);
    Sgm::Inner_ccb_const_iterator iit = fit->inner_ccbs_begin();
    first = *iit;
  }

  typedef std::list<Aos_mark::Halfedge_handle>          Halfedge_list;
  typedef Halfedge_list::iterator                       Halfedge_list_iter;

  Exact_kernel kernel;
  Exact_kernel::Construct_vector_3 cvec = kernel.construct_vector_3_object();

  CGAL::Arr_sgm_initializer<Aos_mark, Exact_kernel> initializer(*aos);
  Halfedge_list hes;

  // Advance the first to a proper place:
  if (first->source()->degree() == 2) first = first->next();

  const Exact_point_3& first_point = first->twin()->face()->point();
  Exact_vector_3 first_vec = cvec(CGAL::ORIGIN, first_point);

  Sgm::Halfedge_const_iterator curr = first->next();
  if (curr->source()->degree() == 2) curr = curr->next();

  const Exact_point_3& curr_point = curr->twin()->face()->point();
  Exact_vector_3 curr_vec = cvec(CGAL::ORIGIN, curr_point);

  initializer.insert(first_vec, curr_vec, std::back_inserter(hes));
  Halfedge_list_iter first_hit = hes.begin();
  Halfedge_list_iter last_hit = hes.end();
  --last_hit;
  Aos_mark::Vertex_handle first_vertex = (*first_hit)->source();
  Aos_mark::Vertex_handle curr_vertex = (*last_hit)->target();
  Exact_vector_3& prev_vec = curr_vec;
  Aos_mark::Vertex_handle prev_vertex = curr_vertex;

  curr = curr->next();
  if (curr->source()->degree() == 2) curr = curr->next();

  do {
    const Exact_point_3& curr_point = curr->twin()->face()->point();
    Exact_vector_3 curr_vec = cvec(CGAL::ORIGIN, curr_point);

    initializer.insert(prev_vec, prev_vertex, curr_vec,
                       std::back_inserter(hes));
    Halfedge_list_iter last_hit = hes.end();
    --last_hit;
    curr_vertex = (*last_hit)->target();
    prev_vec = curr_vec;
    prev_vertex = curr_vertex;

    curr = curr->next();
    if (curr->source()->degree() == 2) curr = curr->next();
  } while (curr != first);

  initializer.insert(prev_vec, prev_vertex, first_vec, first_vertex,
                     std::back_inserter(hes));
  last_hit = hes.end();
  --last_hit;
  (*last_hit)->twin()->face()->set_mark(true);
  hes.clear();
}

/*! \brief computes the projection of a convex polyhedron represented by a
 * spherical Gaussian map.
 */
void Assembly::compute_projection(Sgm::Halfedge_const_handle hit1,
                                  Aos_mark* aos)
{
  typedef std::list<Aos_mark::Halfedge_handle>          Halfedge_list;
  typedef Halfedge_list::iterator                       Halfedge_list_iter;

  Exact_kernel kernel;
  Exact_kernel::Construct_vector_3 cvec = kernel.construct_vector_3_object();

  Sgm::Halfedge_const_iterator hit2 = hit1->twin();

  Sgm::Face_const_handle fh1 = hit1->face();
  Sgm::Face_const_handle fh2 = hit2->face();

  const Exact_point_3& point0 = fh1->point();
  const Exact_point_3& point2 = fh2->point();

  const Exact_point_3& point1 = (hit1->source()->degree() > 2) ?
    hit1->prev()->twin()->face()->point() :
    hit1->prev()->prev()->twin()->face()->point();
  const Exact_point_3& point3 = (hit2->source()->degree() > 2) ?
    hit2->prev()->twin()->face()->point() :
    hit2->prev()->prev()->twin()->face()->point();

  CGAL::Arr_sgm_initializer<Aos_mark, Exact_kernel> initializer(*aos);
  Halfedge_list hes;

  Exact_vector_3 vec0 = cvec(CGAL::ORIGIN, point0);
  Exact_vector_3 vec1 = cvec(CGAL::ORIGIN, point1);
  Exact_vector_3 vec2 = cvec(CGAL::ORIGIN, point2);
  Exact_vector_3 vec3 = cvec(CGAL::ORIGIN, point3);

  initializer.insert(vec0, vec1, std::back_inserter(hes));
  Halfedge_list_iter first_hit = hes.begin();
  Halfedge_list_iter last_hit = hes.end();
  --last_hit;
  Aos_mark::Vertex_handle vertex0 = (*first_hit)->source();
  Aos_mark::Vertex_handle vertex1 = (*last_hit)->target();

  initializer.insert(vec1, vertex1, vec2, std::back_inserter(hes));
  last_hit = hes.end();
  --last_hit;
  Aos_mark::Vertex_handle vertex2 = (*last_hit)->target();

  initializer.insert(vec2, vertex2, vec3, std::back_inserter(hes));
  last_hit = hes.end();
  --last_hit;
  Aos_mark::Vertex_handle vertex3 = (*last_hit)->target();

  initializer.insert(vec3, vertex3, vec0, vertex0,
                     std::back_inserter(hes));

  last_hit = hes.end();
  --last_hit;
  (*last_hit)->twin()->face()->set_mark(true);
  hes.clear();
}

/*! \brief computes the projection of a convex polyhedron represented by a
 * spherical Gaussian map.
 */
void Assembly::compute_projection(Sgm::Vertex_const_handle vit, Aos_mark* aos)
{
  Exact_kernel kernel;
  Exact_kernel::Construct_vector_3 cvec = kernel.construct_vector_3_object();

  typedef std::list<Aos_mark::Halfedge_handle>          Halfedge_list;
  typedef Halfedge_list::iterator                       Halfedge_list_iter;

  CGAL::Arr_sgm_initializer<Aos_mark, Exact_kernel> initializer(*aos);
  Halfedge_list hes;

  Sgm::Halfedge_around_vertex_const_circulator hec = vit->incident_halfedges();
  Sgm::Halfedge_around_vertex_const_circulator first = hec++;
  const Exact_point_3& first_point = first->face()->point();
  const Exact_point_3& point = hec->face()->point();

  // Create first arc:
  Exact_vector_3 first_vec = cvec(CGAL::ORIGIN, first_point);
  Exact_vector_3 vec = cvec(CGAL::ORIGIN, point);

  initializer.insert(first_vec, vec, std::back_inserter(hes));
  Halfedge_list_iter first_hit = hes.begin();
  Halfedge_list_iter last_hit = hes.end();
  --last_hit;
  Aos_mark::Vertex_handle first_vertex = (*first_hit)->source();
  Aos_mark::Vertex_handle vertex = (*last_hit)->target();

  ++hec;
  do {
    const Exact_point_3& next_point = hec->face()->point();
    Exact_vector_3 next_vec = cvec(CGAL::ORIGIN, next_point);

    // Create arc:
    initializer.insert(vec, vertex, next_vec, std::back_inserter(hes));
    last_hit = hes.end();
    --last_hit;
    vertex = (*last_hit)->target();
    vec = next_vec;
    ++hec;
  } while (hec != first);

  // Create last arc:
  initializer.insert(vec, vertex, first_vec, first_vertex,
                     std::back_inserter(hes));

  last_hit = hes.end();
  --last_hit;

  (*last_hit)->face()->set_mark(true);
  hes.clear();
}

/*! \brief finds the next halfedge on the silhouette. */
Assembly::Sgm::Halfedge_const_handle
Assembly::find_next_silhouette_halfedge(Sgm::Halfedge_const_handle first_he)
{
  do {
    Sgm::Vertex_const_handle first_he_source =
      (first_he->source()->degree() > 2) ?
      first_he->source() : first_he->prev()->source();
    Sgm::Vertex_const_handle first_he_target =
      (first_he->target()->degree() > 2) ?
      first_he->target() : first_he->next()->target();

    Sgm::Halfedge_const_handle he = first_he;
    he = he->next();
    if (he->source()->degree() == 2) he = he->next();
    do {
      Sgm::Vertex_const_handle he_source = he->source();
      Sgm::Vertex_const_handle he_target =
        (he->target()->degree() > 2) ?
        he->target() : he->next()->target();

      CGAL::Oriented_side source_vertex_side = compute_side(he);
      CGAL::Oriented_side target_vertex_side = compute_side(he->twin());
      if ((source_vertex_side != CGAL::ON_POSITIVE_SIDE) &&
          (target_vertex_side != CGAL::ON_NEGATIVE_SIDE))
      {
        if ((source_vertex_side == CGAL::ON_ORIENTED_BOUNDARY) &&
            (first_he_target == he_source))
        {
          first_he = he->twin();
          break;
        }
        if ((target_vertex_side == CGAL::ON_ORIENTED_BOUNDARY) &&
            (first_he_source == he_target))
        {
          first_he = he->twin();
          break;
        }
        return he;
      }
      he = he->next();
      if (he->source()->degree() == 2) he = he->next();
    } while (he != first_he);
    if (he == first_he) return Sgm::Halfedge_const_handle();
  } while(true);
  return Sgm::Halfedge_const_handle();
}

/*! \brief computes the projection of a convex polyhedron represented by a
 * spherical Gaussian map.
 */
void Assembly::compute_general_projection(const Sgm* sgm, Aos_mark* aos)
{
  typedef std::list<Aos_mark::Halfedge_handle>          Halfedge_list;
  typedef Halfedge_list::iterator                       Halfedge_list_iter;

  Exact_kernel kernel;
  Exact_kernel::Construct_vector_3 cvec = kernel.construct_vector_3_object();

  CGAL::Arr_sgm_initializer<Aos_mark, Exact_kernel> initializer(*aos);
  Halfedge_list hes;

  /* Traverse all halfedges. Search for a halfedge, such that the origin lies
   * in the non-positive side of one and the non-negative side of the other:
   */
  Sgm::Halfedge_const_iterator heit;
  // Initialize to pacify the compiler
  CGAL::Oriented_side source_vertex_side = CGAL::ON_ORIENTED_BOUNDARY;
  CGAL::Oriented_side target_vertex_side = CGAL::ON_ORIENTED_BOUNDARY;
  for (heit = sgm->halfedges_begin(); heit != sgm->halfedges_end(); ++heit) {
    source_vertex_side = compute_side(heit);
    target_vertex_side = compute_side(heit->twin());
    if ((source_vertex_side != CGAL::ON_POSITIVE_SIDE) &&
        (target_vertex_side != CGAL::ON_NEGATIVE_SIDE))
      break;
  }
  SGAL_assertion(heit != sgm->halfedges_end());

  /* Advance to a silhouette halfedge, the face of which is associated with
   * a real silhouette vertex
   */
  Sgm::Halfedge_const_handle silhouette_he = heit;

  if ((source_vertex_side == CGAL::ON_ORIENTED_BOUNDARY) ||
      (target_vertex_side == CGAL::ON_ORIENTED_BOUNDARY)) {
    silhouette_he = find_next_silhouette_halfedge(heit->twin());
    SGAL_assertion(silhouette_he != Sgm::Halfedge_const_handle());
  }
  Sgm::Face_const_handle first_silhouette_face = silhouette_he->face();

  const Exact_point_3& first_point = first_silhouette_face->point();
  Exact_vector_3 first_vec = cvec(CGAL::ORIGIN, first_point);

  // Advance:
  silhouette_he = find_next_silhouette_halfedge(silhouette_he->twin());
  SGAL_assertion(silhouette_he != Sgm::Halfedge_const_handle());
  Sgm::Face_const_handle silhouette_face = silhouette_he->face();

  // Create arc:
  const Exact_point_3& point = silhouette_face->point();
  Exact_vector_3 vec = cvec(CGAL::ORIGIN, point);
  initializer.insert(first_vec, vec, std::back_inserter(hes));
  Halfedge_list_iter first_hit = hes.begin();
  Halfedge_list_iter last_hit = hes.end();
  --last_hit;
  Aos_mark::Vertex_handle first_vertex = (*first_hit)->source();
  Aos_mark::Vertex_handle vertex = (*last_hit)->target();

  // Advance:
  silhouette_he = find_next_silhouette_halfedge(silhouette_he->twin());
  SGAL_assertion(silhouette_he != Sgm::Halfedge_const_handle());
  silhouette_face = silhouette_he->face();

  /* Traverse the halfedges around the face associated with a primal vertex
   * on the silhouette, to find the next vertex in the loop:
   */
  while (silhouette_face != first_silhouette_face) {
    // Create arc:
    const Exact_point_3& next_point = silhouette_face->point();
    Exact_vector_3 next_vec = cvec(CGAL::ORIGIN, next_point);
    initializer.insert(vec, vertex, next_vec, std::back_inserter(hes));
    last_hit = hes.end();
    --last_hit;
    vertex = (*last_hit)->target();
    vec = next_vec;

    // Advance:
    silhouette_he = find_next_silhouette_halfedge(silhouette_he->twin());
    SGAL_assertion(silhouette_he != Sgm::Halfedge_const_handle());
    silhouette_face = silhouette_he->face();
  }

  // Create last arc:
  initializer.insert(vec, vertex, first_vec, first_vertex,
                     std::back_inserter(hes));
  last_hit = hes.end();
  --last_hit;

  (*last_hit)->face()->set_mark(true);
  hes.clear();
}

/*! \brief compute the oriented side of the origin with respect to the
 * underlying plane of a facet.
 */
CGAL::Oriented_side Assembly::compute_side(Sgm::Halfedge_const_handle heh)
{
  typedef Exact_kernel::Plane_3                         Exact_plane_3;

  Exact_kernel kernel;
  Exact_kernel::Oriented_side_3 os_op = kernel.oriented_side_3_object();
  Exact_kernel::Construct_plane_3 construct_plane =
    kernel.construct_plane_3_object();

  Sgm::Halfedge_const_handle twin_heh = heh->twin();
  const Exact_point_3& point1 = heh->face()->point();
  const Exact_point_3& point2 = twin_heh->face()->point();
  const Exact_point_3& point3 = (twin_heh->target()->degree() > 2) ?
    twin_heh->next()->twin()->face()->point() :
    twin_heh->next()->next()->twin()->face()->point();
  Exact_plane_3 plane = construct_plane(point1, point2, point3);
  return os_op(plane, CGAL::ORIGIN);
}

/*! \brief computes the projection of a convex polyhedron represented by a
 * spherical Gaussian map.
 */
void Assembly::compute_projection(const Sgm* sgm, Aos_mark* aos)
{
  /* - Search for a feature that contains the origin.
   *   traverse all faces, edges, and vertices of the Gaussian map. That is
   *   all vertices, edges, and facets of the polytope.
   * - If non of the features contains the origin, find an sgm face
   *   associated with a polyhedron vertex on the silhouette, and continue
   *   from there to find all the polyhedron vertices on the silhouette.
   */

  Exact_kernel kernel;
  Exact_kernel::Coplanar_3 coplanar = kernel.coplanar_3_object();

  // Traverse all Gaussian map vertices (primal facets):
  Sgm::Vertex_const_iterator vit;
  for (vit = sgm->vertices_begin(); vit != sgm->vertices_end(); ++vit) {
    if (vit->degree() < 3) continue;
    Sgm::Halfedge_around_vertex_const_circulator hedge1 =
      vit->incident_halfedges();
    Sgm::Halfedge_around_vertex_const_circulator hedge2 = hedge1;
    ++hedge2;
    Sgm::Halfedge_around_vertex_const_circulator hedge3 = hedge2;
    ++hedge3;

    const Exact_point_3& point1 = hedge1->face()->point();
    const Exact_point_3& point2 = hedge2->face()->point();
    const Exact_point_3& point3 = hedge3->face()->point();

    if (coplanar(point1, point2, point3, CGAL::ORIGIN)) {
      /* The origin is contained in the underlying plane of the primal
       * facet associated with vit. Traverse all adjacent primal facets:
       */
      Sgm::Halfedge_around_vertex_const_circulator hec =
        vit->incident_halfedges();
      Sgm::Halfedge_around_vertex_const_circulator first_hec = hec;

      do {
        CGAL::Oriented_side side = compute_side(hec);

        if (side == CGAL::ON_NEGATIVE_SIDE) {
          /* The origin is contained in the underlying plane of the primal
           * facet associated with vit, but it is separated from the polytope.
           * Resort to the general procedure. Provide vit as a hint:
           */
          compute_general_projection(sgm, aos);
          return;
        }
        if (side == CGAL::ON_ORIENTED_BOUNDARY) {
          /* The origin is contained in the underlying planes of the primal
           * facets associated with vit and the currently visited facet
           * respectively. Check the previous side and the next side:
           */
          CGAL::Oriented_side prev_side = compute_side(hec->twin()->prev());
          if (prev_side == CGAL::ON_ORIENTED_BOUNDARY) {
            /* The origin is contained in the underlying planes of 3 primal
             * facets. Thus, it coincides with a vertex of the polytope.
             * Dispatch to the dedicated procedure that computes the
             * projection in this case:
             */
            compute_projection(hec->twin()->face(), aos);
            return;
          }
          if (prev_side == CGAL::ON_NEGATIVE_SIDE) {
            compute_general_projection(sgm, aos);
            return;
          }
          CGAL::Oriented_side next_side = compute_side(hec->next()->twin());
          if (next_side == CGAL::ON_ORIENTED_BOUNDARY) {
            /* The origin is contained in the underlying planes of 3 primal
             * facets. Thus, it coincides with a vertex of the polytope.
             * Dispatch to the dedicated procedure that computes the
             * projection in this case:
             */
            compute_projection(hec->face(), aos);
            return;
          }
          if (next_side == CGAL::ON_NEGATIVE_SIDE) {
            compute_general_projection(sgm, aos);
            return;
          }
          /* The origin is contained in the underlying planes of 2 primal
           * facets. Thus, it lies inside the edge of the polytope.
           * Dispatch to the dedicated procedure that computes the
           * projection in this case:
           */
          compute_projection(hec, aos);
          return;
        }

        ++hec;
      } while (hec != first_hec);

      // The origin is contained inside the primal facet associated with vit
      compute_projection(vit, aos);
      return;
    }
  }
  compute_general_projection(sgm, aos);
}

/*! \brief computes the Minkowski-sum projections. */
void Assembly::compute_projections()
{
  clock_t start_time = clock();
  Uint i, j;
  for (i = 0; i < m_number_of_parts; ++i) {
    for (j = 0; j < m_number_of_parts; ++j) {
      if (i == j) continue;
      Key key = std::make_pair(i, j);
      m_projection_lists[key] = Aos_list();
      Aos_list& aoss = m_projection_lists[key];
      Sgm_list& sgms = m_minkowski_sum_lists[key];
      Sgm_iter sit;
      for (sit = sgms.begin(); sit != sgms.end(); ++sit) {
        Sgm* sgm = *sit;
        Aos_mark* aos = new Aos_mark;
        aoss.push_back(aos);
        compute_projection(sgm, aos);
      }
    }
  }
  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Pairwise sub-part Minkowski sum projection: "
            << duration_time << " seconds."
            << std::endl;
}

/*! \brief computes the union of the pairwise Minkowski-sum projections per
 * part.
 */
void Assembly::compute_part_projections()
{
  clock_t start_time = clock();
  Uint i, j;
  for (i = 0; i < m_number_of_parts; ++i) {
    for (j = 0; j < m_number_of_parts; ++j) {
      if (i == j) continue;
      Key key = std::make_pair(i, j);
      Aos_list& aoss = m_projection_lists[key];
      Aos_mark* aos = new Aos_mark;
      m_part_projections[key] = aos;
      // std::cout << "compute_part_projections ("
      //           << i << "," << j << ")" << std::endl;
      compute_part_projections(aoss, aos);
      remove_marked_edges(aos);
    }
  }
  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Pairwise Minkowski sum projection: "
            << duration_time << " seconds."
            << std::endl;
}

/*! \brief removes marked edges to lower the complexity of the arrangement. */
void Assembly::remove_marked_edges(Aos_mark* aos)
{
  // std::cout << *aos << std::endl;

  std::list<Aos_mark::Halfedge_handle> tmp;

  Aos_mark::Edge_iterator eit;
  for (eit = aos->edges_begin(); eit != aos->edges_end(); ++eit) {
    if (eit->mark()) {
      CGAL_assertion(eit->face()->mark() && eit->twin()->face()->mark());
      tmp.push_back(eit);
    }
  }

  std::list<Aos_mark::Halfedge_handle>::iterator it;
  for (it = tmp.begin(); it != tmp.end(); ++it) {
    Aos_mark::Halfedge_handle eh = *it;
    // std::cout << "removing ("
    //           << eh->source()->point() << ") => (" << eh->target()->point()
    //           << ")" << std::endl;
    aos->remove_edge(eh, eh->source()->mark(), eh->target()->mark());
    // std::cout << "removed" << std::endl;
    // std::cout << "valid: " << is_valid(*aos) << std::endl;
  }
  tmp.clear();

  // Merge mergable edges:
  Aos_mark::Vertex_iterator vit;
  for (vit = aos->vertices_begin(); vit != aos->vertices_end(); ++vit) {
    if (!vit->mark()) continue;
    if (vit->degree() == 0) {
      // A marked isolated vertex can be the result of overlaying an
      // a non-marked isolated vertex from one arrangement and a marked
      // face from another.
      aos->remove_isolated_vertex(vit);
      continue;
    }
    CGAL_assertion(vit->degree() == 2);
    Aos_mark::Halfedge_around_vertex_circulator eit = vit->incident_halfedges();
    const Aos_mark::Geometry_traits_2* traits = aos->geometry_traits();
    if (traits->are_mergeable_2_object()(eit->curve(), eit->next()->curve())) {
      Aos_mark::Geometry_traits_2::X_monotone_curve_2 c;
      traits->merge_2_object()(eit->curve(), eit->next()->curve(), c);
      aos->merge_edge(eit, eit->next(), c);
    }
  }
}

/*! \brief computes the union of the pairwise Minkowski-sum projections per
 * part.
 */
void Assembly::compute_part_projections(Aos_list& aoss, Aos_mark* res_aos)
{
  // SGAL_assertion(begin != end);
  // std::cout << "compute_part_projections: " << aoss.size() << std::endl;

  Arrangement_marked_overlay_traits<Aos_mark> overlay_traits;
  Aos_iter it = aoss.begin();
  Aos_mark* aos1 = *it++;
  if (it == aoss.end()) {
    // There is exactly one item in the range.
    // Simply copy the single arrangement from the range
    *res_aos = *aos1;
    return;
  }

  Aos_mark* aos2 = *it++;
  Aos_mark tmp_aos1;
  Aos_mark tmp_aos2;
  while (it != aoss.end()) {
    tmp_aos1.clear();
    // std::cout << "compute_part_projections: 1b " << std::endl;
    // std::cout << "Arr 1 "
    //   //<< CGAL::is_valid(*aos1)
    //           << std::endl << *aos1;
    // std::cout << "Arr 2 "
    //   //<< CGAL::is_valid(*aos2)
    //           << std::endl << *aos2;
    CGAL::overlay(*aos1, *aos2, tmp_aos1, overlay_traits);
    // std::cout << "compute_part_projections: 1a "
    //           << tmp_aos1.number_of_vertices() << ", "
    //           << tmp_aos1.number_of_halfedges() << ", "
    //           << tmp_aos1.number_of_faces() << ", "
    //           << std::endl;
    aos1 = &tmp_aos1;
    aos2 = *it++;

    if (it == aoss.end()) break;
    tmp_aos2.clear();
    // std::cout << "compute_part_projections: 2b " << std::endl;
    // std::cout << "Arr 1 "
    //   //<< CGAL::is_valid(*aos1)
    //           << std::endl << *aos1;
    // std::cout << "Arr 2 "
    //   //<< CGAL::is_valid(*aos2)
    //           << std::endl << *aos2;
    CGAL::overlay(*aos1, *aos2, tmp_aos2, overlay_traits);
    // std::cout << "compute_part_projections: 2a "
    //           << tmp_aos2.number_of_vertices() << ", "
    //           << tmp_aos2.number_of_halfedges() << ", "
    //           << tmp_aos2.number_of_faces() << ", "
    //           << std::endl;
    if (tmp_aos2.number_of_halfedges() > 64) remove_marked_edges(&tmp_aos2);
    aos1 = &tmp_aos2;
    aos2 = *it++;
  }
  // std::cout << "compute_part_projections: 3b " << std::endl;
//   {
//     std::cout << "Arr 1" << std::endl;
//     std::cout << *aos1;
//     std::cout << "Arr 2" << std::endl;
//     std::cout << *aos2;
//   }
  CGAL::overlay(*aos1, *aos2, *res_aos, overlay_traits);
  // std::cout << "compute_part_projections: 3a " << std::endl;
}

/*! \brief constructs the per-part Minkowski-sum projection nodes. */
void Assembly::construct_part_projection_nodes()
{
  // std::cout << "Assembly::construct_part_projection_nodes" << std::endl;
  Projection_iter pli;
  for (pli = m_part_projections.begin(); pli != m_part_projections.end();
       ++pli)
  {
    Aos_mark* aos = (*pli).second;
    Shared_shape shape(new Shape());
    shape->set_appearance(m_appearance);
    // shape->set_draw_depth(false);
    m_part_projection_aos_geo_node->add_child(shape);
    typedef boost::shared_ptr<Arrangement_on_sphere_marked_geo>
      Shared_arrangement_on_sphere_marked_geo;
    Shared_arrangement_on_sphere_marked_geo
      aos_geo(new Arrangement_on_sphere_marked_geo);
    shape->set_geometry(aos_geo);
    aos_geo->set_aos(aos);
    aos_geo->set_aos_vertex_style(Arrangement_renderer::Vertex_shape::DISC);
    aos_geo->set_aos_isolated_vertex_style(Arrangement_renderer::
                                           Vertex_shape::DISC);
    aos_geo->set_aos_edge_style(Arrangement_renderer::Edge_shape::STRIP);
    Vector3f color(0, 0, 0);
    aos_geo->set_aos_vertex_color(color);
    aos_geo->set_aos_isolated_vertex_color(color);
    aos_geo->set_aos_edge_color(color);
    aos_geo->set_aos_vertex_radius(0.06f);

    aos_geo->set_aos_marked_vertex_style(Arrangement_renderer::
                                         Vertex_shape::NONE);
    aos_geo->disable_aos_marked_edge();
    //aos_geo->set_aos_marked_vertex_style(Arrangement_renderer::DISC);
    //aos_geo->set_aos_marked_edge_shape(Arrangement_renderer::STRIP);
  }
  // std::cout << "Assembly::construct_part_projection_nodes end" << std::endl;
}

/* \brief computes the NDBG. */
void Assembly::compute_aos_graph()
{
  // std::cout << "compute_aos_graph" << std::endl;
  // static int cnt = 0;
  clock_t start_time = clock();
  typedef boost::adjacency_matrix<boost::directedS>     Graph;
  m_aos_graph = new Aos_graph;
  Aos_graph::Face_iterator fit = m_aos_graph->faces_begin();
  Graph* graph = new Graph(m_number_of_parts);
  fit->set_graph(graph);

  if (m_part_projections.empty()) return;

  // std::cout << "compute_aos_graph m_number_of_parts: "
  //           << m_number_of_parts << std::endl;
  Arrangement_graph_overlay_traits<Aos_mark, Aos_graph>
    overlay_traits(m_number_of_parts);

  Aos_graph tmp_aos;
  Projection_iter pli;
  for (pli = m_part_projections.begin();
       pli != m_part_projections.end(); ++pli)
  {
    tmp_aos.clear();
    tmp_aos = *m_aos_graph;
    m_aos_graph->clear();
    const Key& key = (*pli).first;
    overlay_traits.set_key(key);
    Aos_mark* aos = (*pli).second;
    // std::cout << "compute_aos_graph m_number_of_parts: befor overlay"
    //           << ", cnt: " << cnt++
    //           << std::endl;
    CGAL::overlay(tmp_aos, *aos, *m_aos_graph, overlay_traits);
    // std::cout << "compute_aos_graph m_number_of_parts: after overlay"
    //           << std::endl;
  }
  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Motion space construction: "
            << duration_time << " seconds."
            << std::endl;
}

/*! \brief construct the graph node. */
void Assembly::construct_graph_node()
{
  m_graph_node->set_appearance(m_appearance);
  typedef boost::shared_ptr<Arrangement_on_sphere_graph_geo>
    Shared_arrangement_on_sphere_graph_geo;
  Shared_arrangement_on_sphere_graph_geo
    aos_geo(new Arrangement_on_sphere_graph_geo);
  m_graph_node->set_geometry(aos_geo);
  aos_geo->set_aos(m_aos_graph);

  aos_geo->set_aos_vertex_style(Arrangement_renderer::Vertex_shape::DISC);
  aos_geo->set_aos_edge_style(Arrangement_renderer::Edge_shape::STRIP);
  Vector3f color(0, 0, 0);
  aos_geo->set_aos_vertex_color(color);
  aos_geo->set_aos_edge_color(color);
  aos_geo->set_aos_vertex_radius(.03f);
}

/*! \brief processes the NDBG. */
void Assembly::process_aos_graph()
{
  std::cout << "# vertices: " << m_aos_graph->number_of_vertices()
            << ", # edges: " << m_aos_graph->number_of_edges()
            << ", # faces: " << m_aos_graph->number_of_faces()
            << std::endl;

  typedef boost::adjacency_matrix<boost::directedS>     Graph;
  typedef Aos_graph::Vertex_const_handle                Vertex_const_handle;
  typedef Aos_graph::Halfedge_const_handle              Halfedge_const_handle;
  typedef Aos_graph::Face_const_handle                  Face_const_handle;

  typedef Aos_graph::Vertex_const_iterator              Vertex_const_iterator;
  typedef Aos_graph::Edge_const_iterator                Edge_const_iterator;
  typedef Aos_graph::Face_const_iterator                Face_const_iterator;

  Solution* solution = new Solution;

  clock_t start_time = clock();

  Face_const_iterator fit = m_aos_graph->faces_begin();
  for (; fit != m_aos_graph->faces_end(); ++fit) {
    const Graph* graph = fit->graph();
    // boost::print_graph(*graph);
    // std::vector<int> discover_time(boost::num_vertices(*graph));
    // std::vector<default_color_type> color(num_vertices(*graph));
    // std::vector<Vertex> root(num_vertices(G));
    Components& components = (solution->second).second;
    components.resize(m_number_of_parts);
#if defined(_MSC_VER)
    (solution->second).first =
      boost::strong_components
      (*graph,
       boost::make_iterator_property_map(components.begin(),
                                         boost::get(boost::vertex_index,
                                                    *graph),
                                         components[0]));
#else
    (solution->second).first = boost::strong_components(*graph, &components[0]);
#endif
    if ((solution->second).first > 1) {
      solution->first = fit;
      m_solutions.push_front(solution);
      solution = new Solution;
    }
  }

  Edge_const_iterator eit = m_aos_graph->edges_begin();
  for (; eit != m_aos_graph->edges_end(); ++eit) {
    const Graph* graph = eit->graph();
    // boost::print_graph(*graph);
    // std::vector<int> discover_time(boost::num_vertices(*graph));
    // std::vector<default_color_type> color(num_vertices(*graph));
    // std::vector<Vertex> root(num_vertices(G));

    Components& components = (solution->second).second;
    components.resize(m_number_of_parts);
#if defined(_MSC_VER)
    (solution->second).first =
      boost::strong_components
      (*graph,
       boost::make_iterator_property_map(components.begin(),
                                         boost::get(boost::vertex_index,
                                                    *graph),
                                         components[0]));
#else
    (solution->second).first = boost::strong_components(*graph, &components[0]);
#endif
    if ((solution->second).first > 1) {
      solution->first = eit;
      m_solutions.push_front(solution);
      solution = new Solution;
    }
  }

  Vertex_const_iterator vit = m_aos_graph->vertices_begin();
  for (; vit != m_aos_graph->vertices_end(); ++vit) {
    const Graph* graph = vit->graph();
    // boost::print_graph(*graph);
    // std::vector<int> discover_time(boost::num_vertices(*graph));
    // std::vector<default_color_type> color(num_vertices(*graph));
    // std::vector<Vertex> root(num_vertices(G));

    Components& components = (solution->second).second;
    components.resize(m_number_of_parts);

#if defined(_MSC_VER)
    (solution->second).first =
      boost::strong_components
      (*graph,
       boost::make_iterator_property_map(components.begin(),
                                         boost::get(boost::vertex_index,
                                                    *graph),
                                         components[0]));
#else
    (solution->second).first = boost::strong_components(*graph, &components[0]);
#endif
    if ((solution->second).first > 1) {
      solution->first = vit;
      m_solutions.push_front(solution);
      solution = new Solution;
    }
  }
  delete solution;

  clock_t end_time = clock();
  float duration_time =
    static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  std::cout << "Motion space processing: "
            << duration_time << " seconds."
            << std::endl;

  if (m_solutions.size() == 0) {
    std::cout << "The Assembly is interlocked!" << std::endl;
    return;
  }

  Solutions::const_iterator it;
  for (it = m_solutions.begin(); it != m_solutions.end(); ++it) {
    const Solution* solution = *it;
    Cell_const_handle ch = solution->first;
    Uint num_comp = (solution->second).first;
    const Components& components = (solution->second).second;
    const Face_const_handle* fh = boost::get<Face_const_handle>(&ch);
    if (fh) {
      std::cout << "Num components in face " << ": "
                << num_comp
                << std::endl;
      std::copy(components.begin(), components.end(),
                std::ostream_iterator<int>(std::cout, " "));
      std::cout << std::endl;
      continue;
    }

    const Halfedge_const_handle* eh = boost::get<Halfedge_const_handle>(&ch);
    if (eh) {
      std::cout << "Num components in edge " << (*eh)->curve() << ": "
                << num_comp
                << std::endl;
      std::copy(components.begin(), components.end(),
                std::ostream_iterator<int>(std::cout, " "));
      std::cout << std::endl;
      continue;
    }

    const Vertex_const_handle* vh = boost::get<Vertex_const_handle>(&ch);
    if (vh) {
      std::cout << "Num components in vertex " << (*vh)->point() << ": "
                << num_comp
                << std::endl;
      std::copy(components.begin(), components.end(),
                std::ostream_iterator<int>(std::cout, " "));
      std::cout << std::endl;
      continue;
    }
  }
}

/*! \brief cleans internal representation. */
void Assembly::clean()
{
  // Visit all parts and assign each a unique non-negative number:
  m_number_of_parts = assign_id(this, 0);

  // Obtain all parts:
  get_parts(this, std::back_inserter(m_parts));

  // Construct the sgm geometry nodes:
  construct_sgms_nodes();

  // Obtain the lists of the reflected sgm geometries:
  construct_reflected_sgms();

  // Construct the reflected sgm geometry nodes:
  construct_reflected_sgms_nodes();

  // Compute the Minkowski sums:
  compute_minkowski_sums();

  // Construct Minkowski-sum nodes:
  construct_minkowski_sum_nodes();

  // Compute the Minkowski-sum projections:
  compute_projections();

  // Construct the Minkowski-sum projection nodes:
  construct_projection_nodes();

  // Compute the union of the Minkowski-sum projections per part:
  compute_part_projections();

  // Construct the per-part Minkowski-sum projection nodes:
  construct_part_projection_nodes();

  // Compute the NDBG
  compute_aos_graph();

  // Construct the graph aos node:
  construct_graph_node();

  // Process the NDBG
  process_aos_graph();

  // print_info(std::cout);
  m_dirty = false;
}

/* \brief processes change of the flag that indicates whether to draw the
 * alternate geometry.
 */
void Assembly::draw_alt_changed(Field_info* field_info)
{
 if (!m_draw_alternate) {
    remove_child(m_switch);
    m_childs = m_childs_save;
    return;
  }

  solve(field_info);
  m_childs_save = m_childs;
  m_childs.clear();
  add_child(m_switch);
}

/* \brief processes change of the flag that indicates whether to increment the
 * alternate geometry
 */
void Assembly::inc_alt_changed(Field_info* field_info)
{
  Uint which_choice = m_switch->get_which_choice();
  if (++which_choice == m_switch->children_size()) which_choice = 0;
  m_switch->set_which_choice(which_choice);
}

/* \brief processes change of the flag that indicates whether  to draw the
 * Gausian maps of the Minkowski sums of the parts.
 */
void Assembly::draw_aos_minkowski_sums_changed(Field_info* field_info)
{
  solve(field_info);

  Uint i;
  for (i = 0; i < m_ms_sgm_geo_node->children_size(); ++i) {
    Shared_node node = m_ms_sgm_geo_node->get_child(i);
    Shared_shape shape = boost::static_pointer_cast<Shape>(node);
    typedef boost::shared_ptr<Geometry>         Shared_geometry;
    Shared_geometry geo = shape->get_geometry();
    typedef boost::shared_ptr<Spherical_gaussian_map_base_geo>
      Shared_spherical_gaussian_map_base_geo;
    Shared_spherical_gaussian_map_base_geo sgm_geo =
      boost::static_pointer_cast<Spherical_gaussian_map_base_geo>(geo);
    sgm_geo->set_draw_aos(m_draw_aos_minkowski_sums);
  }
}

/* \brief processes change of the flag that indicates whether  to increment
 * the Minkowski sums of the parts.
 */
void Assembly::inc_minkowski_sums_changed(Field_info* field_info)
{
  Uint which_choice = m_ms_sgm_geo_node->get_which_choice();
  if (++which_choice == m_ms_sgm_geo_node->children_size())
    which_choice = 0;
  m_ms_sgm_geo_node->set_which_choice(which_choice);

  which_choice = m_projection_aos_geo_node->get_which_choice();
  if (++which_choice == m_projection_aos_geo_node->children_size())
    which_choice = 0;
  m_projection_aos_geo_node->set_which_choice(which_choice);

  which_choice = m_sgm_geo_node->get_which_choice();
  if (++which_choice == m_sgm_geo_node->children_size())
    which_choice = 0;
  m_sgm_geo_node->set_which_choice(which_choice);

  which_choice = m_reflected_sgm_geo_node->get_which_choice();
  if (++which_choice == m_reflected_sgm_geo_node->children_size())
    which_choice = 0;
  m_reflected_sgm_geo_node->set_which_choice(which_choice);

  which_choice = m_part_projection_aos_geo_node->get_which_choice();
  if (++which_choice == m_part_projection_aos_geo_node->children_size())
    which_choice = 0;
  m_part_projection_aos_geo_node->set_which_choice(which_choice);
}

SGAL_END_NAMESPACE
