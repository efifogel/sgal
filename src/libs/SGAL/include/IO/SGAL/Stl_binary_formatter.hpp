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

#ifndef SGAL_BINARY_FORMATTER_HPP
#define SGAL_BINARY_FORMATTER_HPP

#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Shape;
class Mesh_set;
class Matrix4f;

/*! A pure class that can be the base of specific formatter that exports a
 * scene graph to an output stream.
 */
class SGAL_SGAL_DECL Stl_binary_formatter : public Formatter {
public:
  typedef boost::shared_ptr<Container>              Shared_container;
  typedef boost::shared_ptr<Shape>                  Shared_shape;
  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;
  typedef boost::shared_ptr<Matrix4f>               Shared_matrix4f;
  typedef std::pair<Shared_shape, Shared_matrix4f>  World_shape;

  /*! Construct from file name.
   * \param[in] filename The file name.
   */
  Stl_binary_formatter(const std::string& filename);

  /*! Construct an output formatter.
   * \param[in] filename The file name.
   */
  Stl_binary_formatter(const std::string& filename, std::ostream& os);

  /*! Construct an input formatter.
   * \param[in] filename The file name.
   */
  Stl_binary_formatter(const std::string& filename, std::istream& is);

  /*! Destructor
   */
  virtual ~Stl_binary_formatter();

  /// \name Export functions
  //@{

  /*! Export the headers of the scene graph.
   */
  virtual void begin();

  /*! Export the routing statements.
   */
  virtual void end();

  /*! Export a scene-graph container.
   */
  virtual void write(Shared_container container);

  //@}

private:
  /*! Write a string.
   */
  void write(const char* str, size_t size);

  /*! Write a 32-bit integer.
   */
  void write(Int32 i);

  /*! Write a Float.
   */
  void write(Float f);

  /*! Write a vector3f.
   */
  void write(const Vector3f& v);

  /*! Transform a shape.
   */
  void clean(Shared_shape shape);

  /*! Write a shape.
   */
  void write(Shared_shape shape, Shared_matrix4f matrix);

  /*! Write a triangular facet.
   */
  void write_facet(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3,
                   Ushort spacer);


  /*! Write a quadrilateral facet.
   */
  void write_facet(const Vector3f& p1, const Vector3f& p2,
                   const Vector3f& p3, const Vector3f& p4, Ushort spacer);

  /*! Obtain the total number of triangles.
   */
  size_t number_of_triangles();

  /*! The stack of viewing matrices. */
  std::stack<Shared_matrix4f> m_matrices;

  /*! The sequence of shapes and their world transformations. */
  std::vector<World_shape> m_shapes;

  /*! Count triangles indices visitor */
  class Count_triangles_visitor : public boost::static_visitor<size_t> {
  public:
    typedef uint32_t                                    Index_type;
    typedef std::vector<std::array<Index_type, 3> >     Triangle_indices;
    typedef std::vector<std::array<Index_type, 4> >     Quad_indices;
    typedef std::vector<std::vector<Index_type> >       Polygon_indices;
    typedef std::vector<Index_type>                     Flat_indices;

    /*! Obtain the total number of triangles.
     */
    size_t operator()(const Triangle_indices& indices);

    /*! Obtain the total number of triangles.
     */
    size_t operator()(const Quad_indices& indices);

    /*! Obtain the total number of triangles.
     */
    size_t operator()(const Polygon_indices& indices);

    /*! Obtain the total number of triangles.
     */
    size_t operator()(const Flat_indices& indices);
  };

  /*! Export facet indices visitor */
  class Export_facet_visitor : public boost::static_visitor<> {
  public:
    typedef uint32_t                                    Index_type;
    typedef std::vector<std::array<Index_type, 3> >     Triangle_indices;
    typedef std::vector<std::array<Index_type, 4> >     Quad_indices;
    typedef std::vector<std::vector<Index_type> >       Polygon_indices;
    typedef std::vector<Index_type>                     Flat_indices;

    typedef boost::shared_ptr<Mesh_set>                 Shared_mesh_set;

    const Shared_mesh_set& m_mesh;
    const std::vector<Vector3f>& m_world_coords;
    Stl_binary_formatter& m_formater;

    Export_facet_visitor(Shared_mesh_set mesh,
                         const std::vector<Vector3f>& world_coords,
                         Stl_binary_formatter& formater) :
      m_mesh(mesh),
      m_world_coords(world_coords),
      m_formater(formater)
    {}

    void operator()(const Triangle_indices& indices);

    void operator()(const Quad_indices& indices);

    void operator()(const Polygon_indices& indices);

    void operator()(const Flat_indices& /* indices */)
    { SGAL_error(); }
  };
};

//! \brief Destructor
inline Stl_binary_formatter::~Stl_binary_formatter() {}

SGAL_END_NAMESPACE

#endif
