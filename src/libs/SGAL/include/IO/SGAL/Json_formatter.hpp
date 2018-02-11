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
#include <list>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/unordered_map.hpp>
#include <boost/io/ios_state.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Container;
class Shape;
class Geometry;
class Appearance;
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
  typedef boost::shared_ptr<Geometry>               Shared_geometry;
  typedef boost::shared_ptr<Appearance>             Shared_apperance;
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
   * \param[in] i1
   * \param[in] i2
   * \param[in] i3
   */
  void facet(size_t i1, size_t i2, size_t i3);

  void pre_process(const std::list<Shared_container>& containers,
                   const std::map<std::string, Shared_container>& instances);
  //@}

private:
  template<typename T>
  struct identity { typedef T type; };

  //! Export a string.
  void out_string(const std::string& name);

  //! Export a value.
  template <typename Value>
  void out_value(Value value);

  //! Export a generic value.
  template <typename Value>
  void out_value(Value value, identity<Value>);

  //! Export a (specific) sting value.
  void out_value(const std::string& value, identity<std::string>);

  //! Export a (specific) unsigned int.
  void out_value(Uint value, identity<Uint>);

  /*! Export a vertex.
   * \param p the vertex point.
   */
  void vertex(const Vector3f& p, bool compact = false);

  void name_value_separator();

  template <typename Value>
  void attribute(const std::string& name, Value value);

  //! Object manipulators.
  //@{

  //! Begin an object.
  void object_begin();

  //! End an object.
  void object_end();

  //! Print a separator between objects.
  void object_separator(bool compact = false);
  //@}

  //! Array manipulators.
  //@{

  //! Begin an array.
  void array_begin(bool compact = false);

  //! End an array.
  void array_end(bool compact = false);
  //@}

  template <typename UnaryOperation,
            typename StartOperation, typename EndOperation>
  inline void object_body(UnaryOperation op,
                          StartOperation start_op, EndOperation end_op);

  template <typename UnaryOperation,
            typename StartOperation, typename EndOperation>
  inline void object(const std::string& name, UnaryOperation op,
                     StartOperation start_op, EndOperation end_op);

  template <typename UnaryOperation>
  inline void single_object_body(UnaryOperation op);

  template <typename UnaryOperation>
  inline void single_object(const std::string& name, UnaryOperation op);

  template <typename UnaryOperation>
  inline void multiple_object(const std::string& name, UnaryOperation op,
                              bool compact = false);

  void export_geometry(Shared_geometry geometry, String& id);

  void export_material(Shared_apperance appearance, String& id);

  void export_geometry_data(Shared_geometry geometry);

  void export_object();

  Uint to_hex(const Vector3f& color);

  boost::unordered_map<Shared_geometry, String> m_geometries;
  boost::unordered_map<Shared_apperance, String> m_apperances;

  boost::uuids::uuid m_uuid;

  //! The stack of viewing matrices.
  std::stack<Shared_matrix4f> m_matrices;

  /*! The sequence of shapes and their world transformations. */
  std::vector<World_shape> m_shapes;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief exports a generic value.
template <typename Value>
inline void Json_formatter::out_value(Value value)
{ out_value(value, Json_formatter::identity<Value>()); }

//! \brief exports a generic value.
template <typename Value>
inline void Json_formatter::out_value(Value value, identity<Value>)
{ out() << value; }

//! brief exports a (specific) sting value.
inline void Json_formatter::out_value(const std::string& value,
                                      identity<std::string>)
{ out_string(value); }

//! Export a (specific) unsigned int.
inline void Json_formatter::out_value(Uint value, identity<Uint>)
{
  boost::io::ios_flags_saver ifs(out());
  out() << std::hex << "0x" << value;
}

template <typename Value>
inline void Json_formatter::attribute(const std::string& name, Value value)
{
  indent();
  out_string(name);
  name_value_separator();
  out_value(value);
}

//! exports the body of an object.
template <typename UnaryOperation,
          typename StartOperation, typename EndOperation>
inline void Json_formatter::object_body(UnaryOperation op,
                                        StartOperation start_op,
                                        EndOperation end_op)
{
  start_op();
  op();
  end_op();
}

template <typename UnaryOperation,
          typename StartOperation, typename EndOperation>
inline void Json_formatter::object(const std::string& name, UnaryOperation op,
                                   StartOperation start_op, EndOperation end_op)
{
  indent();
  out_string(name);
  name_value_separator();
  object_body(op, start_op, end_op);
}

template <typename UnaryOperation>
inline void Json_formatter::single_object_body(UnaryOperation op)
{
  auto start_op = std::bind(&Json_formatter::object_begin, this);
  auto end_op = std::bind(&Json_formatter::object_end, this);
  object_body(op, start_op, end_op);
}

template <typename UnaryOperation>
inline void Json_formatter::single_object(const std::string& name,
                                          UnaryOperation op)
{
  auto start_op = std::bind(&Json_formatter::object_begin, this);
  auto end_op = std::bind(&Json_formatter::object_end, this);
  object(name, op, start_op, end_op);
}

template <typename UnaryOperation>
inline void Json_formatter::multiple_object(const std::string& name,
                                            UnaryOperation op, bool compact)
{
  auto start_op = std::bind(&Json_formatter::array_begin, this, compact);
  auto end_op = std::bind(&Json_formatter::array_end, this, compact);
  object(name, op, start_op, end_op);
}

SGAL_END_NAMESPACE

#endif
