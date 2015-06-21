// Copyright (c) 2015 Israel.
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

#ifndef SGAL_POLYHEDRON_ATTRIBUTES_HPP
#define SGAL_POLYHEDRON_ATTRIBUTES_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Bounding_box.hpp"
#include "SGAL/Bounding_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Polyhedron_attributes {
public:
  /*! Construct.
   */
  Polyhedron_attributes(Boolean valid,
                        size_t number_of_vertices, size_t number_of_facets,
                        Float volume, Float surface_area,
                        const Bounding_box& bounding_box,
                        const Bounding_sphere& bounding_sphere);

  /*! Set the flag that indicates whether polyhedron is valid.
   */
  void set_valid(Boolean flag);

  /*! Obtain the flag that indicates whether polyhedron is valid.
   */
  Boolean is_valid() const;

  /*! Set the number of vertices.
   */
  void set_number_of_vertices(size_t num);

  /*! Obtain the number of vertices.
   */
  size_t get_number_of_vertices() const;

  /*! Set the number of facets.
   */
  void set_number_of_facets(size_t num);

  /*! Obtain the number of facets.
   */
  size_t get_number_of_facets() const;

  /*! Set the volume.
   */
  void set_volume(Float volume);

  /*! Obtain the volume.
   */
  Float get_volume() const;

  /*! Set the surface area.
   */
  void set_surface_area(Float area);

  /*! Obtain the surface area.
   */
  Float get_surface_area() const;

  /*! Set the bounding box.
   */
  void set_bounding_box(const Bounding_box& bbox);

  /*! Obtain the bounding box.
   */
  const Bounding_box& get_bounding_box() const;

  /*! Set the bounding sphere.
   */
  void set_bounding_sphere(const Bounding_sphere& bsphere);

  /*! Obtain the bounding sphere.
   */
  const Bounding_sphere& get_bounding_sphere() const;

private:
  Boolean m_valid;
  size_t m_number_of_vertices;
  size_t m_number_of_facets;
  Float m_volume;
  Float m_surface_area;
  Bounding_box m_bounding_box;
  Bounding_sphere m_bounding_sphere;
};

//! \brief construct.
inline Polyhedron_attributes::
Polyhedron_attributes(Boolean valid,
                      size_t number_of_vertices,
                      size_t number_of_facets,
                      Float volume,
                      Float surface_area,
                      const Bounding_box& bounding_box,
                      const Bounding_sphere& bounding_sphere) :
  m_valid(valid),
  m_number_of_vertices(number_of_vertices),
  m_number_of_facets(number_of_facets),
  m_volume(volume),
  m_surface_area(surface_area),
  m_bounding_box(bounding_box),
  m_bounding_sphere(bounding_sphere)
{}

//! \brief sets the flag that indicates whether polyhedron is valid.
inline void Polyhedron_attributes::set_valid(Boolean flag)
{ m_valid = flag; }

//! \brief obtains the flag that indicates whether polyhedron is valid.
inline Boolean Polyhedron_attributes::is_valid() const
{ return m_valid; }

//! \brief sets the number of vertices.
inline void Polyhedron_attributes::set_number_of_vertices(size_t num)
{ m_number_of_vertices = num; }

//! \brief obtains the number of vertices.
inline size_t Polyhedron_attributes::get_number_of_vertices() const
{ return m_number_of_vertices; }

//! \brief set the number of facets.
inline void Polyhedron_attributes::set_number_of_facets(size_t num)
{ m_number_of_facets = num; }

//! \brief obtains the number of facets.
inline size_t Polyhedron_attributes::get_number_of_facets() const
{ return m_number_of_facets; }

//! \set the volume.
inline void Polyhedron_attributes::set_volume(Float volume)
{ m_volume = volume; }

//! \brief obtains the volume.
inline Float Polyhedron_attributes::get_volume() const
{ return m_volume; }

//! \brief sets the surface area.
inline void Polyhedron_attributes::set_surface_area(Float area)
{ m_surface_area = area; }

//! \brief obtains the surface area.
inline Float Polyhedron_attributes::get_surface_area() const
{ return m_surface_area; }

//! \brief sets the bounding box.
inline void Polyhedron_attributes::set_bounding_box(const Bounding_box& bbox)
{ m_bounding_box = bbox; }

//! \brief obtains the bounding box.
inline const Bounding_box& Polyhedron_attributes::get_bounding_box() const
{ return m_bounding_box; }

//! \brief sets the bounding sphere.
inline
void Polyhedron_attributes::set_bounding_sphere(const Bounding_sphere& bsphere)
{ m_bounding_sphere = bsphere; }

//! \brief obtains the bounding sphere.
inline const Bounding_sphere& Polyhedron_attributes::get_bounding_sphere() const
{ return m_bounding_sphere; }

SGAL_END_NAMESPACE

#endif
