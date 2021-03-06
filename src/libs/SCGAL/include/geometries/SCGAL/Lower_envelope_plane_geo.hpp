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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_LOWER_ENVELOPE_PLANE_GEO_HPP
#define SCGAL_LOWER_ENVELOPE_PLANE_GEO_HPP

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure, where the surfaces are planes.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Env_plane_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>
#include <CGAL/envelope_3.h>
#include <CGAL/Arr_observer.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Epec_plane_array.hpp"
#include "SGAL/Epec_coord_array_2d.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Lower_envelope_geo.hpp"
#include "SCGAL/Min_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Isect_action;
class Draw_action;
class Isect_action;
class Color_array;

/*! A lower-envelope data structure. */
class SGAL_SCGAL_DECL Lower_envelope_plane_geo : public Lower_envelope_geo {
public:
  typedef CGAL::Env_plane_traits_3<Epec_kernel>
    Env_plane_traits_base_3;
  typedef Env_plane_traits_base_3::Surface_3           Base_plane_3;

  typedef CGAL::Env_surface_data_traits_3<Env_plane_traits_base_3, Uint>
    Env_plane_traits_3;
  typedef Env_plane_traits_3::Surface_3                Plane_3;
  typedef CGAL::Envelope_diagram_2<Env_plane_traits_3> Envelope_diagram_2;

public:
  enum {
    FIRST = Lower_envelope_geo::LAST - 1,
    LAST
  };

  typedef boost::shared_ptr<Color_array>           Shared_color_array;
  typedef boost::shared_ptr<Epec_coord_array_2d>   Shared_exact_coord_array_2d;
  typedef boost::shared_ptr<Epec_plane_array>      Shared_exact_plane_array;

  /*! Constructor. */
  Lower_envelope_plane_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Lower_envelope_plane_geo();

  /*! Construct the prototype. */
  static Lower_envelope_plane_geo* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the prototype of this container. */
  virtual void delete_prototype();

  /*! Obtain the prototype of this container. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Transform the coordinates of the envelope into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  virtual void transform_coords(Inexact_sphere_vector& spheres)
  { transform_coords_impl(m_envelope, spheres); }

  /*! Draw the envelope faces */
  virtual void draw_envelope_faces(Draw_action* action);

  /*! Draw the envelope edges */
  virtual void draw_envelope_edges(Draw_action* action);

  /*! Draw the envelope vertices */
  virtual void draw_envelope_vertices(Draw_action* action);

protected:
  class My_observer :
    public CGAL::Arr_observer<Envelope_diagram_2::Arrangement>
  {
  public:
    My_observer(Envelope_diagram_2::Arrangement& arr) :
      CGAL::Arr_observer<Envelope_diagram_2::Arrangement>(arr)
    {}

    /*! Notification after a face was split.
     * \param f A handle to the face we have just split.
     * \param new_f A handle to the new face that has been created.
     * \param is_hole Whether the new face forms a hole inside f.
     */
    virtual void after_split_face(Envelope_diagram_2::Face_handle f,
                                  Envelope_diagram_2::Face_handle new_f,
                                  bool /* is_hole */)
    { new_f->set_data(f->get_data()); }
  };

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Clean the representation. */
  virtual void clean();

  /*! Clear the internal representation. */
  virtual void clear();

  /*! Return true if the envelope is empty. */
  virtual Boolean is_empty() const { return m_envelope->is_empty(); }

  /*! Draw the geometry of the lower envelope. */
  virtual void draw(Draw_action* action);

  /*! Draw the patches of the plane that induce the envelope. */
  void draw_patches(Draw_action* action);

  /*! Set the plane array. */
  void set_plane_array(Shared_exact_plane_array array);

  /*! Obtain the plane array. */
  Shared_exact_plane_array get_plane_array() const;

  /*! Set the color array. */
  void set_color_array(Shared_color_array array);

  /*! Obtain the color array. */
  Shared_color_array get_color_array() const;

  /*! Set the bounding polygon. */
  void set_bounding_polygon(Shared_exact_coord_array_2d array);

  /*! Obtain the bounding polygon. */
  Shared_exact_coord_array_2d get_bounding_polygon() const;

private:
  //! The tag that identifies this container type
  static const std::string s_tag;

  //! The node prototype
  static Container_proto* s_prototype;

  /*! Indicates whether the envelope data structure is owned, i.e., explicitly
   * allocated. If the envelope data structure is owned, it should be
   * destructed when this geometry node is destructed.
   */
  Boolean m_owned_envelope;

  //! The lower envelope data structure
  Envelope_diagram_2* m_envelope;

  //! An array of direction ccordinates
  Shared_exact_plane_array m_plane_array;

  //! The color array
  Shared_color_array m_color_array;

  //! The bounding polygon of the lower envelope
  Shared_exact_coord_array_2d m_bounding_polygon;

  //! Indicates whther the surface patches should be drawn.
  bool m_draw_patches;

  //! Default value.
  static bool s_def_draw_patches;

  /*! Obtain all planes.
   * \param oi the output iterator that collects all planes.
   * \return the output iterator at the end.
   */
  template <typename OutputIterator>
  OutputIterator get_planes(OutputIterator oi)
  {
    if (!m_plane_array || (m_plane_array->size() == 0)) return oi;
    Uint i = 0;
    for (auto it = m_plane_array->begin(); it != m_plane_array->end(); ++it)
      *oi++ = Plane_3(*it, i++);
    return oi;
  }
};

//! \brief constructs the prototype.
inline Lower_envelope_plane_geo* Lower_envelope_plane_geo::prototype()
{ return new Lower_envelope_plane_geo(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Lower_envelope_plane_geo::create()
{ return new Lower_envelope_plane_geo(); }

//! \brief sets the plane array.
inline void Lower_envelope_plane_geo::
set_plane_array(Shared_exact_plane_array array)
{ m_plane_array = array; }

//! \brief obtains the plane array.
inline Lower_envelope_plane_geo::Shared_exact_plane_array
Lower_envelope_plane_geo::get_plane_array() const
{ return m_plane_array; }

//! \brief sets the color array.
inline void Lower_envelope_plane_geo::set_color_array(Shared_color_array array)
{ m_color_array = array; }

//! \brief obtains the color array.
inline Lower_envelope_plane_geo::Shared_color_array
Lower_envelope_plane_geo::get_color_array() const
{ return m_color_array; }

//! \brief sets the bounding polygon.
inline void Lower_envelope_plane_geo::
set_bounding_polygon(Shared_exact_coord_array_2d array)
{ m_bounding_polygon = array; }

//! \brief obtains the bounding polygon.
inline Lower_envelope_plane_geo::Shared_exact_coord_array_2d
Lower_envelope_plane_geo::get_bounding_polygon() const
{ return m_bounding_polygon; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Lower_envelope_plane_geo::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
