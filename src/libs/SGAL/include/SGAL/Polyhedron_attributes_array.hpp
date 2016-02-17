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

#ifndef SGAL_POLYHEDRON_ATTRIBUTES_ARRAY_HPP
#define SGAL_POLYHEDRON_ATTRIBUTES_ARRAY_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Polyhedron_attributes.hpp"
#include "SGAL/Bounding_box.hpp"
#include "SGAL/Bounding_sphere.hpp"

SGAL_BEGIN_NAMESPACE

/*! \class Polyhedron_attributes_array Polyhedron_attributes_array.hpp
 */
class SGAL_SGAL_DECL Polyhedron_attributes_array {
public:
  /*! Construct default
   */
  Polyhedron_attributes_array();

  /*! Destruct default
   */
  ~Polyhedron_attributes_array();

  /*! Obtain the (mutable) array of polyhedron attributes.
   */
  std::vector<Polyhedron_attributes>& get_array();

  /*! Obtain the (const) array of polyhedron attributes.
   */
  const std::vector<Polyhedron_attributes>& get_array() const;

  /*! Obtain the number of vertices of all polyhedrons.
   */
  Size number_of_vertices() const;

  /*! Obtain the number of edges of all polyhedrons.
   */
  Size number_of_edges() const;

  /*! Obtain the number of facets of all polyhedrons.
   */
  Size number_of_facets() const;

  /*! Obtain the number of vertices of all polyhedrons.
   */
  Float volume() const;

  /*! Obtain the number of vertices of all polyhedrons.
   */
  Float surface_area() const;

  /*! Obtain the number of connected_components of all polyhedrons.
   */
  Size number_of_connected_components() const;

  /*! Obtain the number of vertices of all polyhedrons.
   */
  const Bounding_box bounding_box() const;

  /*! Obtain the number of vertices of all polyhedrons.
   */
  const Bounding_sphere bounding_sphere() const;

  /*! Determine wheather all polyhedra are valid.
   * \return true if all polyhedra valid and false otherwise.
   */
  Boolean is_valid() const;

private:
  /*! The attributes of the polyhedrons. */
  std::vector<Polyhedron_attributes> m_array;
};

SGAL_END_NAMESPACE

#endif
