// Copyright (c) 2018 Israel.
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

#ifndef SGAL_JSON_FORMATTER_HPP
#define SGAL_JSON_FORMATTER_HPP

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Container;
class Shape;
class Mesh_set;
class Matrix4f;
class Vector3f;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Json_formatter Json_formatter.hpp
 * Stl_formater formats the scene, which is exported to an output stream,
 * in the STL format.
 */
class SGAL_SGAL_DECL Json_formatter : public Text_formatter {
public:
  typedef boost::shared_ptr<Container>              Shared_container;
  typedef boost::shared_ptr<Shape>                  Shared_shape;
  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;
  typedef boost::shared_ptr<Matrix4f>               Shared_matrix4f;
  typedef std::pair<Shared_shape, Shared_matrix4f>  World_shape;

  /*! Construct from file name.
   * \param[in] filename The file name.
   */
  Json_formatter(const std::string& filename);

  /*! Construct an output formatter from an output stream.
   * \param[in] filename The file name.
   * \param[in] os the output stream.
   */
  Json_formatter(const std::string& filename, std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param[in] filename The file name.
   * \param[in] is the input stream.
   */
  Json_formatter(const std::string& filename, std::istream& is);

  /*! Destructor */
  virtual ~Json_formatter();

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

  /*! Export a triangular facet.
   * \param[in] p1 the point (in world coordinate system) of the first vertex.
   * \param[in] p2 the point (in world coordinate system) of the second vertex.
   * \param[in] p3 the point (in world coordinate system) of the third vertex.
   * \param[in] normal the facet normal.
   */
  virtual void facet(const Vector3f& local_p1, const Vector3f& local_p2,
                     const Vector3f& local_p3, const Vector3f& normal);

  //@}

private:
  /*! Export a vertex.
   * \param p the vertex point.
   */
  void vertex(const Vector3f& p);

  /*! Export a facet header.
   * \param normal (in) the facet normal.
   */
  void facet_begin(const Vector3f& normal);

  /*! Export a facet trailer.
   */
  void facet_end();

  void out_string(const std::string& name);

  void out_value(const std::string& value);

  void name_value_separator();

  template <typename Value>
  void attribute(const std::string& name, const Value& value);

  //! Object manipulators.
  //@{

  //! Begin an object.
  void object_begin();

  //! End an object.
  void object_end();

  //! Print a separator between objects.
  void object_separator();
  //@}

  //! Array manipulators.
  //@{

  //! Begin an array.
  void array_begin();

  //! End an array.
  void array_end();
  //@}

  void geometries();

  void materials();

  template <typename UnaryOperation, typename StartOperation>
    void object(const std::string& name, StartOperation start_op,
                UnaryOperation op);

  //! The stack of viewing matrices.
  std::stack<Shared_matrix4f> m_matrices;

  /*! The sequence of shapes and their world transformations. */
  std::vector<World_shape> m_shapes;

  class Object_exporter {
  public:
    void operator()() {}
  };
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

template <typename UnaryOperation, typename StartOperation>
inline void Json_formatter::object(const std::string& name,
                                   StartOperation start_op, UnaryOperation op)
{
  indent();
  out_string(name);
  name_value_separator();
  // object_begin();
  start_op();
  op();
  new_line();
  object_end();
}

SGAL_END_NAMESPACE

#endif
