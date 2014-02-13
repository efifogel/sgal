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
// $Id: $
// $Revision: 12348 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FORMATTER_HPP
#define SGAL_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Sphere_bound.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

/*! A pure class that can be the base of specific formatter that exports a
 * scene graph to an output stream.
 */
class SGAL_SGAL_DECL Formatter {
public:
  /*! Constructor
   */
  Formatter();

  /*! Construct an output formatter.
   */
  Formatter(std::ostream& os);

  /*! Construct an input formatter.
   */
  Formatter(std::istream& is);

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

  /// \name Write functions
  //@{

  /*! Write the headers of the scene graph.
   */
  virtual void begin() = 0;

  /*! Write the routing statements.
   */
  virtual void end() = 0;

  /*! Write a scene-graph container.
   */
  virtual void write(Container* container);

  /*! Write the container beginning section */
  virtual void container_begin(const std::string& /* tag */) {}

  /*! Write the container ending section */
  virtual void container_end() {}

  /*! Write a single Boolean field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_boolean(const std::string& /* name */,
                              Boolean /* value */,
                              Boolean /* default_value */) {}

  /*! Write a single Float field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_float(const std::string& /* name */,
                            Float /* value */, Float /* default_value */) {};

  /*! Write a single Uint field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_uint(const std::string& /* name */,
                           Uint /* value */, Uint /* default_value */) {}

  /*! Write a single Int field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_int(const std::string& /* name */,
                          Int /* value */, Int /* default_value */) {}

  /*! Write a single Scene_time field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_scene_time(const std::string& /* name */,
                                 Scene_time /* value */,
                                 Scene_time /* default_value */) {}

  /*! Write a single Vector2f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_vector2f(const std::string& /* name */,
                               const Vector2f& /* value */,
                               const Vector2f& /* default_value */) {}

  /*! Write a single Vector3f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_vector3f(const std::string& /* name */,
                               const Vector3f& /* value */,
                               const Vector3f& /* default_value */) {}

  /*! Write a single Vector4f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_vector4f(const std::string& /* name */,
                               const Vector4f& /* value */,
                               const Vector4f& /* default_value */) {}

  /*! Write a single Rotation field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_rotation(const std::string& /* name */,
                               const Rotation& /* value */,
                               const Rotation& /* default_value */) {}

  /*! Write a single Sphere_bound field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_sphere_bound(const std::string& /* name */,
                                   const Sphere_bound& /* value */,
                                   const Sphere_bound& /* default_value */) {}

  /*! Write a single string field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_string(const std::string& /* name */,
                             const std::string& /* value */,
                             const std::string& /* default_value */) {}

  /*! Write the single-container field beginning section.
   * \param name The attribute name.
   */
  virtual void single_container_begin(const std::string& /* name */) {}

  /*! Write the single-container field ending section.
   */
  virtual void single_container_end() {}

  /*! Write a multi-Boolean field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_boolean(const std::string& /* name */,
                             const std::vector<Boolean>& /* value */,
                             const std::vector<Boolean>& /* default_value */) {}

  /*! Write a multi-Float field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_float(const std::string& /* name */,
                           const std::vector<Float>& /* value */,
                           const std::vector<Float>& /* default_value */) {}

  /*! Write a multi-Uint field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_uint(const std::string& /* name */,
                          const std::vector<Uint>& /* value */,
                          const std::vector<Uint>& /* default_value */) {}

  /*! Write a multi-Int field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_int(const std::string& /* name */,
                         const std::vector<Int>& /* value */,
                         const std::vector<Int>& /* default_value */) {}

  /*! Write a multi-Scene_time field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_scene_time(const std::string& /* name */,
                                const std::vector<Scene_time>& /* value */,
                                const std::vector<Scene_time>& /* default_value */) {}

  /*! Write a multi-Vector2f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_vector2f(const std::string& /* name */,
                              const std::vector<Vector2f>& /* value */,
                              const std::vector<Vector2f>& /* default_value */) {}

  /*! Write a multi-Vector3f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_vector3f(const std::string& /* name */,
                              const std::vector<Vector3f>& /* value */,
                              const std::vector<Vector3f>& /* default_value */) {}

  /*! Write a multi-Vector4f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_vector4f(const std::string& /* name */,
                              const std::vector<Vector4f>& /* value */,
                              const std::vector<Vector4f>& /* default_value */) {}

  /*! Write a multi-Rotation field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_rotation(const std::string& /* name */,
                              const std::vector<Rotation>& /* value */,
                              const std::vector<Rotation>& /* default_value */) {}

  /*! Write a multi-Sphere_bound field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_sphere_bound(const std::string& /* name */,
                                  const std::vector<Sphere_bound>& /* value */,
                                  const std::vector<Sphere_bound>& /* default_value */) {}

  /*! Write a multi-string field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_string(const std::string& /* name */,
                            const std::vector<std::string>& /* value */,
                            const std::vector<std::string>& /* default_value */) {}

  /*! Write the multi-container field beginning section.
   * \param name The attribute name.
   */
  virtual void multi_container_begin(const std::string& /* name */) {}

  /*! Write the multi-container field ending section.
   */
  virtual void multi_container_end() {}

  /*! Export a triangular facet.
   * \param p1 (in) the point (in world coordinate system) of the first vertex.
   * \param p2 (in) the point (in world coordinate system) of the second vertex.
   * \param p3 (in) the point (in world coordinate system) of the third vertex.
   * \param normal (in) the facet normal.
   */
  virtual void facet(const Vector3f& /* p1 */, const Vector3f& /* p2 */,
                     const Vector3f& /* p3 */, const Vector3f& /* normal */) {}

  //@}

protected:
  /*! The output stream */
  std::ostream* m_out;

  /*! The input stream */
  std::istream* m_in;
};

//! \brief constructs
inline Formatter::Formatter() : m_out(NULL), m_in(NULL) {}

//! \brief Construct an output formatter.
inline Formatter::Formatter(std::ostream& os) : m_out(&os), m_in(NULL) {}

//! \brief Construct an input formatter.
inline Formatter::Formatter(std::istream& is) : m_out(NULL), m_in(&is) {}

//! \brief Destructor
inline Formatter::~Formatter() {}

//! \brief Set the output stream.
inline void Formatter::set_out(std::ostream& os) { m_out = &os; }

//! \brief Set the input stream.
inline void Formatter::set_in(std::istream& is) { m_in = &is; }

//! \brief Obtain the output stream. */
inline std::ostream& Formatter::out()
{
  SGAL_assertion(m_out != NULL);
  return *m_out;
}

//! \brief Obtain the input stream. */
inline std::istream& Formatter::in()
{
  SGAL_assertion(m_in != NULL);
  return *m_in;
}

//! \brief exports a scene-graph node.
inline void Formatter::write(Container* container) { container->write(this); }

SGAL_END_NAMESPACE

#endif
