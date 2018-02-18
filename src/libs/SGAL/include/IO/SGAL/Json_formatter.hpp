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
#include <boost/unordered_map.hpp>
#include <boost/io/ios_state.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

class Container;
class Group;
class Shape;
class Geometry;
class Appearance;
class Mesh_set;
class Matrix4f;
class Vector3f;
class Vector2f;
class Camera;
class Light;

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
  typedef boost::shared_ptr<Group>                  Shared_group;
  typedef boost::shared_ptr<Shape>                  Shared_shape;
  typedef boost::shared_ptr<Geometry>               Shared_geometry;
  typedef boost::shared_ptr<Appearance>             Shared_apperance;
  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;
  typedef boost::shared_ptr<Matrix4f>               Shared_matrix4f;
  typedef boost::shared_ptr<Light>                  Shared_light;

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

  /*! Pre-process the formatter. Traverse all input containers (An instance
   * is a container that has a name.) and assign a unique id to each geometry
   * and each material container and store these id in respective maps.
   * \param[in] containers.
   * \param[in] instances.
   */
  void pre_process(Camera* camera,
                   const std::list<Shared_container>& containers,
                   const std::map<std::string, Shared_container>& instances);

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

  /*! Export a 3D point.
   * \param[in] p the point.
   * \param compact[in] indicates whether to export in a compact manner
   *        (without spaces).
   */
  void vertex(const Vector3f& p, bool compact = false);

  /*! Export a 2D point.
   * \param[in] p the point.
   * \param compact[in] indicates whether to export in a compact manner
   *        (without spaces).
   */
  void vertex(const Vector2f& p, bool compact = false);

  /*! Export a triangular facet.
   * \param[in] i1
   * \param[in] i2
   * \param[in] i3
   */
  void facet(Uint type, const std::array<Index_type, 3>& tri, Uint i,
             bool compact = false);
  //@}

private:
  /*! A utility strcture used when dispatching the function that exports
   * a value.
   */
  template <typename T>
  struct identity { typedef T type; };

  /*! Export a string.
   * \param[in] str the string to export.
   */
  void out_string(const char* str);

  /*! Export a value.
   * \param[in] value the value to export.
   */
  template <typename Value>
  void out_value(Value value);

  /*! Export a generic value.
   * \param[in] value the value to export.
   */
  template <typename Value>
  void out_value(Value value, identity<Value>);

  /*! Export a (specific) sting value.
   */
  void out_value(const std::string& value, identity<std::string>);

  /*! Export a (specific) sting value.
   * \param[in] value the value to export.
   */
  void out_value(const char* value, identity<const char*>);

  /*! Export a (specific) unsigned int value.
   */
  void out_value(Uint value, identity<Uint>);

  /*! Export a (specific) Boolean value.
   * \param[in] value the value to export.
   */
  void out_value(Boolean value, identity<Boolean>);

  /*! Export a single value.
   * \param[in] value the value to export.
   */
  template <typename Value>
  void single_value(Value value, bool compact = false);

  /*! Convert a color to its hexadecimal representation.
   */
  Uint to_hex(const Vector3f& color);

  /*! Export a separator between the name of an attribute and its value.
   */
  void name_value_separator();

  //! Object manipulators.
  //@{

  /*! Begin an object.
   */
  void object_begin();

  /*! End an object.
   */
  void object_end();

  /*! Export a separator between consecutive objects.
   */
  void object_separator(bool compact = false);
  //@}

  //! Array manipulators.
  //@{

  //! Begin an array.
  void array_begin(bool compact = false);

  //! End an array.
  void array_end(bool compact = false);
  //@}

  /*! Export an object.
   * \param[in] op the main export operation.
   * \param[in] start_op the starting export operation.
   * \param[in] end_op the ending export operation.
   */
  template <typename UnaryOperation,
            typename StartOperation, typename EndOperation>
  inline void object(UnaryOperation op,
                     StartOperation start_op, EndOperation end_op);

  /*! Export a complex attribute.
   * \param[in] name the name of the attribute.
   * \param[in] op the main export operation.
   * \param[in] start_op the starting export operation.
   * \param[in] end_op the ending export operation.
   */
  template <typename UnaryOperation,
            typename StartOperation, typename EndOperation>
  inline void attribute_object(const std::string& name, UnaryOperation op,
                               StartOperation start_op, EndOperation end_op);

  template <typename UnaryOperation>
  inline void single_object(UnaryOperation op);

  /*! Export a simple attribute.
   * \param[in] name the attribute name.
   * \param[in] value the attribute value.
   */
  template <typename Value>
  void attribute(const std::string& name, Value value);

  /*! Export a single-object attribute.
   * \param[in] name the attribute name.
   * \param[in] op the operation that exposrts the attributes of the object.
   */
  template <typename UnaryOperation>
  inline void attribute_single(const std::string& name, UnaryOperation op);

  /*! Export an array-object attribute.
   * \param[in] name the attribute name.
   * \param[in] op the operation that exposrts the objects of the array.
   */
  template <typename UnaryOperation>
  inline void attribute_multiple(const std::string& name, UnaryOperation op,
                                 bool compact = false);

  /*! Export the main scene object.
   */
  void export_scene();

  /*! Export the attributes of a geometry object.
   * \param[in] geometry the geometry container.
   * \param[in] id the unique id of the geometry.
   */
  void export_geometry(Shared_geometry geometry, String& id);

  /*! Export the attributes of a material object.
   * \param[in] appearance the appearance container.
   * \param[in] id the unique id of the material.
   */
  void export_material(Shared_apperance appearance, String& id);

  void export_geometry_data(Shared_geometry geometry);

  /*! Export the camera.
   */
  void export_camera();

  /*! Export a group.
   */
  void export_group(Shared_group group);

  /*! Export a light source.
   */
  void export_light(Shared_light light);

  /*! Export a mesh.
   */
  void export_mesh(Shared_shape shape);

  /*! Export a matrix.
   * \param[in] the matrix.
   */
  void export_matrix(const Matrix4f& matrix);

  /*! Indicated whether the attribute is separated. */
  bool m_separated;

  Camera* m_camera;

  boost::unordered_map<Shared_geometry, String> m_lights;
  boost::unordered_map<Shared_geometry, String> m_geometries;
  boost::unordered_map<Shared_apperance, String> m_apperances;

  Matrix4f m_identity_matrix;
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
{ out_string(value.c_str()); }

//! \brief exports a (specific) sting value.
inline void Json_formatter::out_value(const char* str, identity<const char*>)
{ out_string(str); }

//! Export a (specific) unsigned int.
inline void Json_formatter::out_value(Uint value, identity<Uint>)
{
  // Normally, we would print the value in the hexadecimal format, where the
  // number is preceeded with "0x", but json does not support this.
  // boost::io::ios_flags_saver ifs(out());
  // out() << std::hex << "0x" << value;
  out() << value;
}

//! \brief exports a (specific) Boolean value.
inline void Json_formatter::out_value(Boolean value, identity<Boolean>)
{ out() << ((value) ? "true" : "false"); }

//! \brief exports a single value.
template <typename Value>
inline void Json_formatter::single_value(Value value, bool compact)
{
  object_separator(compact);
  indent();
  out_value(value);
  m_separated = false;
}

//! \brief exports a simple attribute.
template <typename Value>
inline void Json_formatter::attribute(const std::string& name, Value value)
{
  object_separator();
  indent();
  out_string(name.c_str());
  name_value_separator();
  out_value(value);
  m_separated = false;
}

//! \brief exports an object.
template <typename UnaryOperation,
          typename StartOperation, typename EndOperation>
inline void Json_formatter::object(UnaryOperation op,
                                   StartOperation start_op, EndOperation end_op)
{
  start_op();
  op();
  end_op();
}

//! \brief exports a complex attribute.
template <typename UnaryOperation,
          typename StartOperation, typename EndOperation>
inline void Json_formatter::attribute_object(const std::string& name,
                                             UnaryOperation op,
                                             StartOperation start_op,
                                             EndOperation end_op)
{
  object_separator();
  indent();
  out_string(name.c_str());
  name_value_separator();
  object(op, start_op, end_op);
}

template <typename UnaryOperation>
inline void Json_formatter::single_object(UnaryOperation op)
{
  object_separator();
  auto start_op = std::bind(&Json_formatter::object_begin, this);
  auto end_op = std::bind(&Json_formatter::object_end, this);
  object(op, start_op, end_op);
}

//! \brief exports a single-object attribute.
template <typename UnaryOperation>
inline void Json_formatter::attribute_single(const std::string& name,
                                             UnaryOperation op)
{
  auto start_op = std::bind(&Json_formatter::object_begin, this);
  auto end_op = std::bind(&Json_formatter::object_end, this);
  attribute_object(name, op, start_op, end_op);
}

//! \brief exports an array-object attribute.
template <typename UnaryOperation>
inline void Json_formatter::attribute_multiple(const std::string& name,
                                               UnaryOperation op, bool compact)
{
  auto start_op = std::bind(&Json_formatter::array_begin, this, compact);
  auto end_op = std::bind(&Json_formatter::array_end, this, compact);
  attribute_object(name, op, start_op, end_op);
}

SGAL_END_NAMESPACE

#endif
