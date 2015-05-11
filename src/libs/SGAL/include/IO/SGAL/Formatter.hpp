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

#ifndef SGAL_FORMATTER_HPP
#define SGAL_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Sphere_bound.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Field_info.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;

/*! A pure class that can be the base of specific formatter that exports a
 * scene graph to an output stream.
 */
class SGAL_SGAL_DECL Formatter {
public:
  typedef boost::shared_ptr<Container>          Shared_container;

  /*! Construct from file name.
   * \param[in] filename The file name.
   */
  Formatter(const std::string& filename);

  /*! Construct an output formatter.
   * \param[in] filename The file name.
   */
  Formatter(const std::string& filename, std::ostream& os);

  /*! Construct an input formatter.
   * \param[in] filename The file name.
   */
  Formatter(const std::string& filename, std::istream& is);

  /*! Destructor
   */
  virtual ~Formatter();

  /*! Set the output stream.
   */
  void set_out(std::ostream& os);

  /*! Set the input stream.
   */
  void set_in(std::istream& is);

  /*! Obtain the output stream. */
  inline std::ostream& out();

  /*! Obtain the input stream. */
  inline std::istream& in();

  /// \name Export functions
  //@{

  /*! Export the headers of the scene graph.
   */
  virtual void begin() = 0;

  /*! Export the routing statements.
   */
  virtual void end() = 0;

  /*! Export a scene-graph container.
   */
  virtual void write(Shared_container container);

  /*! Export the container beginning section */
  virtual void container_begin(const std::string& /* tag */) {}

  /*! Export the container ending section */
  virtual void container_end() {}

  /*! Export a triangular facet.
   * \param[in] p1 the point (in world coordinate system) of the first vertex.
   * \param[in] p2 the point (in world coordinate system) of the second vertex.
   * \param[in] p3 the point (in world coordinate system) of the third vertex.
   * \param[in] normal the facet normal.
   */
  virtual void facet(const Vector3f& /* p1 */, const Vector3f& /* p2 */,
                     const Vector3f& /* p3 */, const Vector3f& /* normal */) {}
  //@}

  /// \name Definitions
  //@{
  /*! Export a single Boolean field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_boolean(const std::string& /* name */,
                              Boolean /* value */,
                              Boolean /* default_value */) {}

  /*! Export a single Float field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_float(const std::string& /* name */,
                            Float /* value */, Float /* default_value */) {};

  /*! Export a single Uint field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_uint(const std::string& /* name */,
                           Uint /* value */, Uint /* default_value */) {}

  /*! Export a single Int field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_int32(const std::string& /* name */,
                            Int32 /* value */, Int32 /* default_value */) {}

  /*! Export a single Scene_time field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_scene_time(const std::string& /* name */,
                                 Scene_time /* value */,
                                 Scene_time /* default_value */) {}

  /*! Export a single Vector2f field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_vector2f(const std::string& /* name */,
                               const Vector2f& /* value */,
                               const Vector2f& /* default_value */) {}

  /*! Export a single Vector3f field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_vector3f(const std::string& /* name */,
                               const Vector3f& /* value */,
                               const Vector3f& /* default_value */) {}

  /*! Export a single Vector4f field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_vector4f(const std::string& /* name */,
                               const Vector4f& /* value */,
                               const Vector4f& /* default_value */) {}

  /*! Export a single Rotation field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_rotation(const std::string& /* name */,
                               const Rotation& /* value */,
                               const Rotation& /* default_value */) {}

  /*! Export a single Sphere_bound field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_sphere_bound(const std::string& /* name */,
                                   const Sphere_bound& /* value */,
                                   const Sphere_bound& /* default_value */) {}

  /*! Export a single string field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void single_string(const std::string& /* name */,
                             const std::string& /* value */,
                             const std::string& /* default_value */) {}

  /*! Export the single-container field beginning section.
   * \param[in] name The attribute name.
   */
  virtual void single_container_begin(const std::string& /* name */) {}

  /*! Export the single-container field ending section.
   */
  virtual void single_container_end() {}

  /*! Export a multi-Boolean field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_boolean(const std::string& /* name */,
                             const std::vector<Boolean>& /* value */,
                             const std::vector<Boolean>& /* default_value */) {}

  /*! Export a multi-Float field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_float(const std::string& /* name */,
                           const std::vector<Float>& /* value */,
                           const std::vector<Float>& /* default_value */) {}

  /*! Export a multi-Uint field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_uint(const std::string& /* name */,
                          const std::vector<Uint>& /* value */,
                          const std::vector<Uint>& /* default_value */) {}

  /*! Export a multi-Int field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_int32(const std::string& /* name */,
                           const std::vector<Int32>& /* value */,
                           const std::vector<Int32>& /* default_value */) {}

  /*! Export a multi-Scene_time field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_scene_time(const std::string& /* name */,
                                const std::vector<Scene_time>& /* value */,
                                const std::vector<Scene_time>& /* default_value */) {}

  /*! Export a multi-Vector2f field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_vector2f(const std::string& /* name */,
                              const std::vector<Vector2f>& /* value */,
                              const std::vector<Vector2f>& /* default_value */) {}

  /*! Export a multi-Vector3f field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_vector3f(const std::string& /* name */,
                              const std::vector<Vector3f>& /* value */,
                              const std::vector<Vector3f>& /* default_value */) {}

  /*! Export a multi-Vector4f field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_vector4f(const std::string& /* name */,
                              const std::vector<Vector4f>& /* value */,
                              const std::vector<Vector4f>& /* default_value */) {}

  /*! Export a multi-Rotation field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_rotation(const std::string& /* name */,
                              const std::vector<Rotation>& /* value */,
                              const std::vector<Rotation>& /* default_value */) {}

  /*! Export a multi-Sphere_bound field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_sphere_bound(const std::string& /* name */,
                                  const std::vector<Sphere_bound>& /* value */,
                                  const std::vector<Sphere_bound>& /* default_value */) {}

  /*! Export a multi-string field.
   * \param[in] name The attribute name.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   */
  virtual void multi_string(const std::string& /* name */,
                            const std::vector<std::string>& /* value */,
                            const std::vector<std::string>& /* default_value */) {}

  /*! Export the multi-container field beginning section.
   * \param[in] name The attribute name.
   */
  virtual void multi_container_begin(const std::string& /* name */) {}

  /*! Export the multi-container field ending section.
   */
  virtual void multi_container_end() {}
  //@}

  /// \name Declaration
  //@{
  /*! Export a declaration statement without default value.
   * \param[in] rule the field rule name.
   * \param[in] name The field name.
   * \param[in] value The field value.
   */
  virtual void declaration(const std::string& /* rule */,
                           const std::string& /* type */,
                           const std::string& /* name */) {}

  /*! Export a single Boolean field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_boolean(const std::string& /* rule */,
                              const std::string& /* type */,
                              const std::string& /* name */,
                              Boolean /* default_value */) {}

  /*! Export a single Float field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_float(const std::string& /* rule */,
                            const std::string& /* type */,
                            const std::string& /* name */,
                            Float /* value */) {};

  /*! Export a single Uint field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_uint(const std::string& /* rule */,
                           const std::string& /* type */,
                           const std::string& /* name */,
                           Uint /* value */) {}

  /*! Export a single Int field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_int32(const std::string& /* rule */,
                            const std::string& /* type */,
                            const std::string& /* name */,
                            Int32 /* value */) {}

  /*! Export a single Scene_time field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_scene_time(const std::string& /* rule */,
                                 const std::string& /* type */,
                                 const std::string& /* name */,
                                 Scene_time /* value */) {}

  /*! Export a single Vector2f field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_vector2f(const std::string& /* rule */,
                               const std::string& /* type */,
                               const std::string& /* name */,
                               const Vector2f& /* value */) {}

  /*! Export a single Vector3f field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_vector3f(const std::string& /* rule */,
                               const std::string& /* type */,
                               const std::string& /* name */,
                               const Vector3f& /* value */) {}

  /*! Export a single Vector4f field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_vector4f(const std::string& /* rule */,
                               const std::string& /* type */,
                               const std::string& /* name */,
                               const Vector4f& /* value */) {}

  /*! Export a single Rotation field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_rotation(const std::string& /* rule */,
                               const std::string& /* type */,
                               const std::string& /* name */,
                               const Rotation& /* value */) {}

  /*! Export a single Sphere_bound field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_sphere_bound(const std::string& /* rule */,
                                   const std::string& /* type */,
                                   const std::string& /* name */,
                                   const Sphere_bound& /* value */) {}

  /*! Export a single string field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void single_string(const std::string& /* rule */,
                             const std::string& /* type */,
                             const std::string& /* name */,
                             const std::string& /* value */) {}

  /*! Export a multi-Boolean field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_boolean(const std::string& /* rule */,
                             const std::string& /* type */,
                             const std::string& /* name */,
                             const std::vector<Boolean>& /* value */) {}

  /*! Export a multi-Float field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_float(const std::string& /* rule */,
                           const std::string& /* type */,
                           const std::string& /* name */,
                           const std::vector<Float>& /* value */) {}

  /*! Export a multi-Uint field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_uint(const std::string& /* rule */,
                          const std::string& /* type */,
                          const std::string& /* name */,
                          const std::vector<Uint>& /* value */) {}

  /*! Export a multi-Int field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_int32(const std::string& /* rule */,
                           const std::string& /* type */,
                           const std::string& /* name */,
                           const std::vector<Int32>& /* value */) {}

  /*! Export a multi-Scene_time field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_scene_time(const std::string& /* rule */,
                                const std::string& /* type */,
                                const std::string& /* name */,
                                const std::vector<Scene_time>& /* value */) {}

  /*! Export a multi-Vector2f field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_vector2f(const std::string& /* rule */,
                              const std::string& /* type */,
                              const std::string& /* name */,
                              const std::vector<Vector2f>& /* value */) {}

  /*! Export a multi-Vector3f field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_vector3f(const std::string& /* rule */,
                              const std::string& /* type */,
                              const std::string& /* name */,
                              const std::vector<Vector3f>& /* value */) {}

  /*! Export a multi-Vector4f field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_vector4f(const std::string& /* rule */,
                              const std::string& /* type */,
                              const std::string& /* name */,
                              const std::vector<Vector4f>& /* value */) {}

  /*! Export a multi-Rotation field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_rotation(const std::string& /* rule */,
                              const std::string& /* type */,
                              const std::string& /* name */,
                              const std::vector<Rotation>& /* value */) {}

  /*! Export a multi-Sphere_bound field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_sphere_bound(const std::string& /* rule */,
                                  const std::string& /* type */,
                                  const std::string& /* name */,
                                  const std::vector<Sphere_bound>& /* value */) {}

  /*! Export a multi-string field.
   * \param[in] name The field name.
   * \param[in] value The field value.
   * \param[in] default_value The field default value.
   */
  virtual void multi_string(const std::string& /* rule */,
                            const std::string& /* type */,
                            const std::string& /* name */,
                            const std::vector<std::string>& /* value */) {}
  //@}

protected:
  /*! The file name. */
  const std::string& m_filename;

  /*! The output stream */
  std::ostream* m_out;

  /*! The input stream */
  std::istream* m_in;
};

//! \brief constructs from file name.
inline Formatter::Formatter(const std::string& filename) :
  m_filename(filename),
  m_out(nullptr),
  m_in(nullptr)
{}

//! \brief Construct an output formatter.
inline Formatter::Formatter(const std::string& filename, std::ostream& os) :
  m_filename(filename),
  m_out(&os),
  m_in(nullptr)
{}

//! \brief Construct an input formatter.
inline Formatter::Formatter(const std::string& filename, std::istream& is) :
  m_filename(filename),
  m_out(nullptr),
  m_in(&is)
{}

//! \brief Destructor
inline Formatter::~Formatter() {}

//! \brief Set the output stream.
inline void Formatter::set_out(std::ostream& os) { m_out = &os; }

//! \brief Set the input stream.
inline void Formatter::set_in(std::istream& is) { m_in = &is; }

//! \brief Obtain the output stream. */
inline std::ostream& Formatter::out()
{
  SGAL_assertion(m_out != nullptr);
  return *m_out;
}

//! \brief Obtain the input stream. */
inline std::istream& Formatter::in()
{
  SGAL_assertion(m_in != nullptr);
  return *m_in;
}

//! \brief exports a scene-graph node.
inline void Formatter::write(Shared_container container)
{ container->write(this); }

SGAL_END_NAMESPACE

#endif
