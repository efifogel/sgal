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
// $Id: Assembly.hpp 13463 2012-06-04 15:22:36Z efif $
// $Revision: 13463 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ASSEMBLY_HPP
#define SGAL_ASSEMBLY_HPP

/*! \file
 * A class representing a puzzle.
 */

#include <string>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Switch.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Assembly_part.hpp"
#include "SCGAL/Arrangement_on_sphere_marked.hpp"
#include "SCGAL/Arrangement_on_sphere_graph.hpp"
#include "SCGAL/Spherical_gaussian_map_colored_geo.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Field_info;

class SGAL_SCGAL_DECL Assembly : public Group {
public:
  enum {
    FIRST = Group::LAST - 1,
    TRIGGER,
    DRAW_ALT,
    INC_ALT,
    INC_MINKOWSKI_SUMS,
    DRAW_AOS_MINKOWSKI_SUMS,
    LAST
  };

  /*! Constructor. */
  Assembly(Boolean proto = false);

  /*! Destructor. */
  virtual ~Assembly();

  /* Construct the prototype. */
  static Assembly* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Clear the representation. */
  virtual void clear();
  
  /*! Clean the representation. */
  virtual void clean();
    
protected:
  typedef std::list<Assembly_part*>                 Assembly_part_list;
  typedef Assembly_part_list::iterator              Assembly_part_iter;

  typedef Spherical_gaussian_map_colored_geo        Sgm_geo;
  typedef Spherical_gaussian_map_colored            Sgm;

  typedef boost::shared_ptr<Material>               Shared_material;
  typedef boost::shared_ptr<Appearance>             Shared_appearance;
  typedef boost::shared_ptr<Switch>                 Shared_switch;
  typedef boost::shared_ptr<Shape>                  Shared_shape;
  typedef boost::shared_ptr<Sgm_geo>                Shared_sgm_geo;
  
  typedef std::list<Sgm*>                           Sgm_list;
  typedef Sgm_list::iterator                        Sgm_iter;

  typedef std::list<Sgm_list>                       Sgm_list_list;
  typedef Sgm_list_list::iterator                   Sgm_list_iter;

  typedef std::list<Shared_sgm_geo>                 Sgm_geo_list;
  typedef Sgm_geo_list::iterator                    Sgm_geo_iter;
  typedef Sgm_geo_list::const_iterator              Sgm_geo_const_iter;

  typedef std::list<Sgm_geo_list>                   Sgm_geo_list_list;
  typedef Sgm_geo_list_list::iterator               Sgm_geo_list_iter;

  typedef std::list<Shared_appearance>              Appearance_list;
  typedef Appearance_list::iterator                 Appearance_iter;
  
  typedef std::pair<Uint, Uint>                     Key;
  struct Key_comparer {
    bool operator()(Key key1, Key key2) const
    {
      return ((key1.first < key2.first) ||
              ((key1.first == key2.first) && (key1.second < key2.second)));
    }
  };
  typedef std::map<Key, Sgm_list, Key_comparer>     Minkowski_sum_list_map;
  typedef Minkowski_sum_list_map::iterator          Minkowski_sum_list_iter;

  typedef Arrangement_on_sphere_graph               Aos_graph;
  typedef Arrangement_on_sphere_marked              Aos_mark;
  
  typedef std::list<Aos_mark*>                      Aos_list;
  typedef Aos_list::iterator                        Aos_iter;
  
  typedef std::map<Key, Aos_list, Key_comparer>     Projection_list_map;
  typedef Projection_list_map::iterator             Projection_list_iter;

  typedef std::map<Key, Aos_mark*, Key_comparer>    Projection_map;
  typedef Projection_map::iterator                  Projection_iter;
 
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Obtain all the SGM's that comprise this part. */
  template <typename OutputIterator>
  OutputIterator get_parts(Group* group, OutputIterator oi) const
  {
    Node_iterator it;
    for (it = group->children_begin(); it != group->children_end(); ++it) {
      Shared_node node = *it;
      oi = get_parts(&*node, oi);
    }
    return oi;
  }

  /*! Obtain all the SGM's that comprise this part. */
  template <typename OutputIterator>
  OutputIterator get_parts(Node* node, OutputIterator oi) const
  {
    Assembly_part* part = dynamic_cast<Assembly_part*>(node);
    if (part) {
      oi++ = part;
      return oi;
    }
    
    Switch* my_switch = dynamic_cast<Switch*>(node);
    if (my_switch) {
      Shared_node choice = my_switch->get_choice();
      if (!choice) return oi;
      return get_parts(&*choice, oi);
    }
    
    Group* group = dynamic_cast<Group*>(node);
    if (group) return get_parts(group, oi);

    return oi;
  }

  /*! Solve the puzzle. */
  void solve(Field_info* field_info);

  /* Process change of the flag that indicates whether to draw the alternate
   * geometry.
   */
  void draw_alt_changed(Field_info* field_info);

  /* Process change of the flag that indicates whether to increment the
   * alternate geometry.
   */
  void inc_alt_changed(Field_info* field_info);
  
  /* Process change of the flag that indicates whether  to draw the Gausian
   * maps of the Minkowski sums of the parts.
   */
  void draw_aos_minkowski_sums_changed(Field_info* field_info);

  /* Process change of the flag that indicates whether  to increment
   * the Minkowski sums of the parts.
   */
  void inc_minkowski_sums_changed(Field_info* field_info);

  /*! Find the next halfedge on the silhouette starting from a halfedge on the
   * silhouette.
   * \param silhouette_he the starting point.
   */
  Sgm::Halfedge_const_handle
  find_next_silhouette_halfedge(Sgm::Halfedge_const_handle silhouette_he);
  
private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Is the internal representation dirty and requires cleaning. */
  Boolean m_dirty;
  
  /*! When trigerred, the puzzle solves itself. */
  Boolean m_trigger;
  
  /*! The number of parts in the puzzle. */
  Uint m_number_of_parts;

  /*! A container of puzzle parts. */
  Assembly_part_list m_parts;
  
  /*! A container of containers of (reflected) Gaussian map geometries. */
  Sgm_geo_list_list m_parts_reflected_sgm_geos;
  
  /* The pairwise Minkowski sums map. */
  Minkowski_sum_list_map m_minkowski_sum_lists;

  /*! The pairwise Minkowski-sum projections. */
  Projection_list_map m_projection_lists;

  /*! The union of the pairwise Minkowski-sum projections per part. */
  Projection_map m_part_projections;

  /*! The final arrangement. */
  Aos_graph* m_aos_graph;
  
  /*! Space holder. */
  Shared_appearance m_appearance;

  /*! Space holder. */
  Shared_material m_material;
  
  /*! Space holder. */
  Shared_appearance m_sphere_appearance;
  
  /*! Space holder. */
  Shared_material m_sphere_material;

  /*! A node that holds the Sgm shapes. */
  Shared_switch m_sgm_geo_node;
  
  /*! A node that holds the reflected Sgm shapes. */
  Shared_switch m_reflected_sgm_geo_node;

  /*! A node that holds the Minkowski-sum Sgm shapes. */
  Shared_switch m_ms_sgm_geo_node;

  /*! A node that holds the Minkowski-sum projection Aos shapes. */
  Shared_switch m_projection_aos_geo_node;

  /*! A node that holds the per-part Minkowski-sum projection Aos shapes. */
  Shared_switch m_part_projection_aos_geo_node;

  /*! A shape node that holds the graph Aos. */
  Shared_shape m_graph_node;
  
  /*! A switch node. */
  Shared_switch m_switch;
  
  /*! Indicates whether to draw the alternate geometry. */
  Boolean m_draw_alternate;

  /*! Indicates whether to increment the alternate geometry. */
  Boolean m_inc_alternate;
  
  /*! Space holder. */
  Node_list m_childs_save;
  
  /*! Indicates whether to draw the Gausian maps of the Minkowski sums of the
   * parts.
   */
  Boolean m_draw_aos_minkowski_sums;

  /*! Indicates whether to increment the switch counter of the Minkowski
   * sums of the parts.
   */
  Boolean m_inc_minkowski_sums;
  
  /*! Assign each part a unique non-negative number.
   */
  Uint assign_id(Node* node, Uint id) const;

  /*! Assign each part a unique non-negative number.
   */
  Uint assign_id(Group* group, Uint id) const;

  /*! Obtain the lists of the reflected sgm geometries. */
  void construct_reflected_sgms();
  
  /*! Compute the Minkowski sums. */
  void compute_minkowski_sums();

  /*! Compute the Minkowski-sum projections. */
  void compute_projections();

  /*! Compute the projection of a convex polyhedron represented by a
   * spherical Gaussian map.
   * \param sgm the spherical Gaussian map that represents the input polyhedron.
   * \param aos the output projection represented as an arrangement on the
   * sphere.
   */
  void compute_projection(const Sgm* sgm, Aos_mark* aos);

  /*! Compute the projection of a convex polyhedron represented by a
   * spherical Gaussian map.
   * \param fit
   * \param aos the output projection represented as an arrangement on the
   * sphere.
   */
  void compute_projection(Sgm::Face_const_handle fit, Aos_mark* aos);

  /*! Compute the projection of a convex polyhedron represented by a
   * spherical Gaussian map.
   * \param hit
   * \param aos the output projection represented as an arrangement on the
   * sphere.
   */
  void compute_projection(Sgm::Halfedge_const_handle hit, Aos_mark* aos);

  /*! Compute the projection of a convex polyhedron represented by a
   * spherical Gaussian map.
   * \param vit 
   * \param aos the output projection represented as an arrangement on the
   * sphere
   */
  void compute_projection(Sgm::Vertex_const_handle vit, Aos_mark* aos);

  /*! Compute the projection of a convex polyhedron represented by a
   * spherical Gaussian map.
   * \param sgm the spherical Gaussian map that represents the input polyhedron.
   * \param aos the output projection represented as an arrangement on the
   * sphere.
   */
  void compute_general_projection(const Sgm* sgm, Aos_mark* aos);

  /*! Compute the union of the pairwise Minkowski-sum projections per
   * part.
   */
  void compute_part_projections();
  
  /*! Compute the union of the pairwise Minkowski-sum projections per
   * part.
   * \param aoss the list of projections for the part.
   * \param aos the union of the projections aoss.
   */
  void compute_part_projections(Aos_list& aoss, Aos_mark* aos);
  
  /*! Remove marked edges to lower the complexity of the arrangement
   * \param aos the arrangement.
   */
  void remove_marked_edges(Aos_mark* aos);
  
  /*! Construct the per-part Minkowski-sum projection nodes. */
  void construct_part_projection_nodes();
  
  /* Compute the NDBG. */
  void compute_aos_graph();

  /*! Construct the graph node. */
  void construct_graph_node();

  /*! Process the NDBG. */
  void process_aos_graph();
  
  /* Construct the sgm geometry nodes. */
  void construct_sgms_nodes();

  /* Construct the reflected sgm geometry nodes. */
  void construct_reflected_sgms_nodes();
  
  /*! Construct the Minkowski-sum nodes. */
  void construct_minkowski_sum_nodes();

  /*! Construct the Minkowski-sum projection nodes. */
  void construct_projection_nodes();

  /*! Compute the oriented side of the origin with respect to the
   * underlying plane of a facet.
   */
  CGAL::Oriented_side compute_side(Sgm::Halfedge_const_handle heh);
  
  /*! Print information to an output stream. */
  void print_info(std::ostream& out);
};

// template <typename PartIterator>
// compute_minkowski_sums(PartIterator begin, PartIterator end)
// {
//   typedef typename PartIterator::value_type    Part;
//   typedef typename Part::ConvexPartIterator   ConvexPartIterator;
//   typedef typename PartIterator::value_type    ConvexPart;
  
//   PartIterator pit1;
//   for (pit1 = begin; pit1 != end; ++pit1) {
//     Part part1 = *pit1;
//     ConvexPartIterator cpit1;
//     for (cpit1 = part1.convex_parts_begin();
//          cpit1 != part1.convex_parts_end();
//          ++cpit1)
//     {
//       ConvexPart convex_part1 = *cpit1;

//       PartIterator pit2 = pit;
//       for (++pit2; pit2 != end; ++pit2) {
//         Part part2 = *pit2;
//         ConvexPartIterator cpit2;
//         for (cpit2 = part2.convex_parts_begin();
//              cpit2 != part2.convex_parts_end();
//              ++cpit2)
//         {
//           ConvexPart convex_part2 = *cpit2;
//           minkowski_sum(convex_part1.get_sgm(), convex_part2.get_sgm());
//         }
//       }
//     }
//   }
// }

/*! \brief constructs the prototype. */
inline Assembly* Assembly::prototype() { return new Assembly(true); }

/*! \brief clones. */
inline  Container* Assembly::clone() { return new Assembly(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Assembly::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
