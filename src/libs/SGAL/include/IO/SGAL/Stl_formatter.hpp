// Copyright (c) 2014 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_STL_FORMATTER_HPP
#define SGAL_STL_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>
#include <stack>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Vector2f;
class Vector3f;
class Vector4f;
class Rotation;
class Sphere_bound;
class Container;

/*! Writes a scene graph to an output stream in the VRML format */
class SGAL_SGAL_DECL Stl_formatter : public Text_formatter {
public:
  /*! Constructor */
  Stl_formatter();

  /*! Construct an output formatter from an output stream.
   * \param os the output stream.
   */
  Stl_formatter(std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param is the input stream.
   */
  Stl_formatter(std::istream& is);

  /*! Destructor */
  virtual ~Stl_formatter();

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
  virtual void write(Container* container);

  /*! Export the container header.
   * \param tag (in) the container tag.
   */
  virtual void container_begin(const std::string& tag);

  /*! Export the container tailer.
   */
  virtual void container_end();

  /*! Export a single Boolean field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_boolean(const std::string& name,
                              Boolean value, Boolean default_value);

  /*! Export a single Float field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_float(const std::string& name,
                            Float value, Float default_value);

  /*! Export a single Uint field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_uint(const std::string& name,
                           Uint value, Uint default_value);

  /*! Export a single Int field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_int(const std::string& name,
                          Int value, Int default_value);

  /*! Export a single Scene_time field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_scene_time(const std::string& name,
                                 Scene_time value,
                                 Scene_time default_value);

  /*! Export a single Vector2f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_vector2f(const std::string& name,
                               const Vector2f& value,
                               const Vector2f& default_value);

  /*! Export a single Vector3f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_vector3f(const std::string& name,
                               const Vector3f& value,
                               const Vector3f& default_value);

  /*! Export a single Vector4f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_vector4f(const std::string& name,
                               const Vector4f& value,
                               const Vector4f& default_value);

  /*! Export a single Rotation field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_rotation(const std::string& name,
                               const Rotation& value,
                               const Rotation& default_value);

  /*! Export a single sphere-bound field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_sphere_bound(const std::string& name,
                                   const Sphere_bound& value,
                                   const Sphere_bound& default_value);

  /*! Export a single string field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void single_string(const std::string& name,
                             const std::string& value,
                             const std::string& default_value);

  /*! Export the header of a single-container field.
   */
  virtual void single_container_begin(const std::string& name);

  /*! Export the tailer of a single-container field.
   */
  virtual void single_container_end();

  /*! Export a multi-Boolean field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_boolean(const std::string& name,
                             const Array<Boolean>& value,
                             const Array<Boolean>& default_value);

  /*! Export a multi-Float field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_float(const std::string& name,
                           const Array<Float>& value,
                           const Array<Float>& default_value);

  /*! Export a multi-Uint field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_uint(const std::string& name,
                          const Array<Uint>& value,
                          const Array<Uint>& default_value);

  /*! Export a multi-Int field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_int(const std::string& name,
                         const Array<Int>& value,
                         const Array<Int>& default_value);

  /*! Export a multi-Scene_time field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_scene_time(const std::string& name,
                                const Array<Scene_time>& value,
                                const Array<Scene_time>& default_value);

  /*! Export a multi-Vector2f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_vector2f(const std::string& name,
                              const Array<Vector2f>& value,
                              const Array<Vector2f>& default_value);

  /*! Export a multi-Vector3f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_vector3f(const std::string& name,
                              const Array<Vector3f>& value,
                              const Array<Vector3f>& default_value);

  /*! Export a multi-Vector4f field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_vector4f(const std::string& name,
                              const Array<Vector4f>& value,
                              const Array<Vector4f>& default_value);

  /*! Export a multi-Rotation field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_rotation(const std::string& name,
                              const Array<Rotation>& value,
                              const Array<Rotation>& default_value);

  /*! Export a multi-Sphere_bound field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_sphere_bound(const std::string& name,
                                  const Array<Sphere_bound>& value,
                                  const Array<Sphere_bound>& default_value);

  /*! Export a multi-string field.
   * \param name The attribute name.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  virtual void multi_string(const std::string& name,
                            const Array<std::string>& value,
                            const Array<std::string>& default_value);

  /*! Export the header of a multi-container field.
   */
  virtual void multi_container_begin(const std::string& name);

  /*! Export the tailer of a multi-container field */
  virtual void multi_container_end();

  //@}

private:
  //! The stack of viewing matrices.
  std::stack<Matrix4f> m_matrices;

  /*! Compute the normal of a plane given by three points.
   * \param p1 (in) the first point.
   * \param p2 (in) the second point.
   * \param p3 (in) the third point.
   * \param normal (out) the output normal.
   */
  void compute_normal(const Vector3f& p1, const Vector3f& p2,
                      const Vector3f& p3, Vector3f& normal);

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

  /*! Export a facet.
   * \param p1 (in) the point (in world coordinate system) of the first vertex.
   * \param p2 (in) the point (in world coordinate system) of the second vertex.
   * \param p3 (in) the point (in world coordinate system) of the third vertex.
   * \param normal (in) the facet normal.
   */
  void facet(const Vector3f& local_p1, const Vector3f& local_p2,
             const Vector3f& local_p3, const Vector3f& normal);

  /*! Export a triangular facet.
   * \param p1 (in) the point, in local coordinate system, of the first vertex.
   * \param p2 (in) the point, in local coordinate system, of the second vertex.
   * \param p3 (in) the point, in local coordinate system, of the third vertex.
   * \param matrix (in) the transform viewing matrix.
   */
  void facet(const Vector3f& local_p1, const Vector3f& local_p2,
             const Vector3f& local_p3, const Matrix4f& matrix);

  /*! Export a quadrilateral facet.
   * \param p1 (in) the point of the first vertex.
   * \param p2 (in) the point of the second vertex.
   * \param p3 (in) the point of the third vertex.
   * \param p4 (in) the point of the forth vertex.
   */
  void facet(const Vector3f& local_p1, const Vector3f& local_p2,
             const Vector3f& local_p3, const Vector3f& local_p4,
             const Matrix4f& matrix);
};

//! \brief writes a single Float field.
inline void Stl_formatter::single_float(const std::string& name,
                                        Float value, Float default_value)
{}

//! \brief writes a single Uint field.
inline void Stl_formatter::single_uint(const std::string& name,
                                       Uint value, Uint default_value)
{}

//! \brief writes a single Int field.
inline void Stl_formatter::single_int(const std::string& name,
                                      Int value, Int default_value)
{}

//! \brief writes a single Scene_time field.
inline void Stl_formatter::single_scene_time(const std::string& name,
                                             Scene_time value,
                                             Scene_time default_value)
{}

//! \brief writes a single Vector2f field.
inline void Stl_formatter::single_vector2f(const std::string& name,
                                           const Vector2f& value,
                                           const Vector2f& default_value)
{}

//! \brief writes a single Vector3f field.
inline void Stl_formatter::single_vector3f(const std::string& name,
                                           const Vector3f& value,
                                           const Vector3f& default_value)
{}

//! \brief writes a single Vector4f field.
inline void Stl_formatter::single_vector4f(const std::string& name,
                                           const Vector4f& value,
                                           const Vector4f& default_value)
{}

//! \brief writes a single Rotation field.
inline void Stl_formatter::single_rotation(const std::string& name,
                                           const Rotation& value,
                                           const Rotation& default_value)
{}

//! \brief writes a single Sphere_bound field.
inline void
Stl_formatter::single_sphere_bound(const std::string& name,
                                   const Sphere_bound& value,
                                   const Sphere_bound& default_value)
{}

//! \brief writes a single string field.
inline void Stl_formatter::single_string(const std::string& name,
                                         const std::string& value,
                                         const std::string& default_value)
{}

// Multi-field
//! \brief writes a single Float field.
inline void Stl_formatter::multi_float(const std::string& name,
                                       const Array<Float>& value,
                                       const Array<Float>& default_value)
{}

//! \brief writes a single Uint field.
inline void Stl_formatter::multi_uint(const std::string& name,
                                      const Array<Uint>& value,
                                      const Array<Uint>& default_value)
{}

//! \brief writes a single Int field.
inline void Stl_formatter::multi_int(const std::string& name,
                                     const Array<Int>& value,
                                     const Array<Int>& default_value)
{}

//! \brief writes a single Scene_time field.
inline void
Stl_formatter::multi_scene_time(const std::string& name,
                                const Array<Scene_time>& value,
                                const Array<Scene_time>& default_value)
{}

//! \brief writes a single Vector2f field.
inline void Stl_formatter::multi_vector2f(const std::string& name,
                                          const Array<Vector2f>& value,
                                          const Array<Vector2f>& default_value)
{}

//! \brief writes a single Vector3f field.
inline void Stl_formatter::multi_vector3f(const std::string& name,
                                          const Array<Vector3f>& value,
                                          const Array<Vector3f>& default_value)
{}

//! \brief writes a single Vector4f field.
inline void Stl_formatter::multi_vector4f(const std::string& name,
                                          const Array<Vector4f>& value,
                                          const Array<Vector4f>& default_value)
{}

//! \brief writes a single Rotation field.
inline void Stl_formatter::multi_rotation(const std::string& name,
                                          const Array<Rotation>& value,
                                          const Array<Rotation>& default_value)
{}

//! \brief writes a single Sphere_bound field.
inline void
Stl_formatter::multi_sphere_bound(const std::string& name,
                                  const Array<Sphere_bound>& value,
                                  const Array<Sphere_bound>& default_value)
{}

//! \brief writes a single string field.
inline void
Stl_formatter::multi_string(const std::string& name,
                            const Array<std::string>& value,
                            const Array<std::string>& default_value)
{}

SGAL_END_NAMESPACE

#endif
