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

#ifndef SGAL_VRML_FORMATTER_HPP
#define SGAL_VRML_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"

SGAL_BEGIN_NAMESPACE

class Vector2f;
class Vector3f;
class Vector4f;
class Rotation;
class Bounding_sphere;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Vrml_formatter Vrml_formatter.hpp
 * Obj_formater formats the scene, which is exported to an output stream,
 * in the VRML format.
 */
class SGAL_SGAL_DECL Vrml_formatter : public Text_formatter {
public:
  /*! Construct from filename.
   * \param[in] filename The file name.
   */
  Vrml_formatter(const std::string& filename);

  /*! Construct an output formatter.
   * \param[in] filename The file name.
   * \param[in] os the output stream.
   */
  Vrml_formatter(const std::string& filename, std::ostream& os);

  /*! Construct an input formatter.
   * \param[in] filename The file name.
   * \param[in] is the input stream.
   */
  Vrml_formatter(const std::string& filename, std::istream& is);

  /*! Destructor */
  virtual ~Vrml_formatter();

  /// \name Export functions
  //@{

  /*! Export the headers of the scene graph.
   */
  virtual void begin();

  /*! Export the routing statements.
   */
  virtual void end();

  /*! Export a container.
   */
  virtual void write(Shared_container container);

  /*! Export the container header.
   */
  virtual void container_begin(const std::string& tag);

  /*! Export the container tailer.
   */
  virtual void container_end();
  //@}

  /// \name Declaration
  //@{
  /*! Export a declaration statement without default value.
   * \param rule[in] the field rule name.
   * \param name[in] The field name.
   * \param value[in] The field value.
   */
  virtual void declaration(const std::string& /* rule */,
                           const std::string& /* type */,
                           const std::string& /* name */);

  /*! Export a single Boolean field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_boolean(const std::string& rule, const std::string&_type,
                              const std::string& name,
                              Boolean default_value);

  /*! Export a single Float field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_float(const std::string& rule, const std::string&_type,
                            const std::string& name,
                            Float value);

  /*! Export a single Uint field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_uint(const std::string& rule, const std::string&_type,
                           const std::string& name,
                           Uint value);

  /*! Export a single Int field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_int32(const std::string& rule, const std::string&_type,
                            const std::string& name,
                            Int32 value);

  /*! Export a single Scene_time field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_scene_time(const std::string& rule,
                                 const std::string&_type,
                                 const std::string& name,
                                 Scene_time value);

  /*! Export a single Vector2f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_vector2f(const std::string& rule, const std::string&_type,
                               const std::string& name,
                               const Vector2f& value);

  /*! Export a single Vector3f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_vector3f(const std::string& rule, const std::string&_type,
                               const std::string& name,
                               const Vector3f& value);

  /*! Export a single Vector4f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_vector4f(const std::string& rule, const std::string&_type,
                               const std::string& name,
                               const Vector4f& value);

  /*! Export a single Rotation field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_rotation(const std::string& rule, const std::string&_type,
                               const std::string& name,
                               const Rotation& value);

  /*! Export a single Bounding_sphere field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_sphere_bound(const std::string& rule,
                                   const std::string&_type,
                                   const std::string& name,
                                   const Bounding_sphere& value);

  /*! Export a single string field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_string(const std::string& rule, const std::string&_type,
                             const std::string& name,
                             const std::string& value);

  /*! Export a multi-Boolean field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_boolean(const std::string& rule, const std::string&_type,
                             const std::string& name,
                             const std::vector<Boolean>& value);

  /*! Export a multi-Float field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_float(const std::string& rule, const std::string&_type,
                           const std::string& name,
                           const std::vector<Float>& value);

  /*! Export a multi-Uint field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_uint(const std::string& rule, const std::string&_type,
                          const std::string& name,
                          const std::vector<Uint>& value);

  /*! Export a multi-Int field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_int32(const std::string& rule, const std::string&_type,
                           const std::string& name,
                           const std::vector<Int32>& value);

  /*! Export a multi-Scene_time field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_scene_time(const std::string& rule,
                                const std::string&_type,
                                const std::string& name,
                                const std::vector<Scene_time>& value);

  /*! Export a multi-Vector2f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_vector2f(const std::string& rule, const std::string&_type,
                              const std::string& name,
                              const std::vector<Vector2f>& value);

  /*! Export a multi-Vector3f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_vector3f(const std::string& rule, const std::string&_type,
                              const std::string& name,
                              const std::vector<Vector3f>& value);

  /*! Export a multi-Vector4f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_vector4f(const std::string& rule, const std::string&_type,
                              const std::string& name,
                              const std::vector<Vector4f>& value);

  /*! Export a multi-Rotation field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_rotation(const std::string& rule, const std::string&_type,
                              const std::string& name,
                              const std::vector<Rotation>& value);

  /*! Export a multi-Bounding_sphere field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_sphere_bound(const std::string& rule,
                                  const std::string&_type,
                                  const std::string& name,
                                  const std::vector<Bounding_sphere>& value);

  /*! Export a multi-string field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_string(const std::string& rule, const std::string&_type,
                            const std::string& name,
                            const std::vector<std::string>& value);
  //@}

  /// \name Definition
  //@{
  /*! Export a single Boolean field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_boolean(const std::string& name,
                              Boolean value, Boolean default_value);

  /*! Export a single Float field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_float(const std::string& name,
                            Float value, Float default_value);

  /*! Export a single Uint field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_uint(const std::string& name,
                           Uint value, Uint default_value);

  /*! Export a single Int field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_int32(const std::string& name,
                            Int32 value, Int32 default_value);

  /*! Export a single Scene_time field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_scene_time(const std::string& name,
                                 Scene_time value,
                                 Scene_time default_value);

  /*! Export a single Vector2f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_vector2f(const std::string& name,
                               const Vector2f& value,
                               const Vector2f& default_value);

  /*! Export a single Vector3f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_vector3f(const std::string& name,
                               const Vector3f& value,
                               const Vector3f& default_value);

  /*! Export a single Vector4f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_vector4f(const std::string& name,
                               const Vector4f& value,
                               const Vector4f& default_value);

  /*! Export a single Rotation field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_rotation(const std::string& name,
                               const Rotation& value,
                               const Rotation& default_value);

  /*! Export a single sphere-bound field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void single_sphere_bound(const std::string& name,
                                   const Bounding_sphere& value,
                                   const Bounding_sphere& default_value);

  /*! Export a single string field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
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
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_boolean(const std::string& name,
                             const std::vector<Boolean>& value,
                             const std::vector<Boolean>& default_value);

  /*! Export a multi-Float field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_float(const std::string& name,
                           const std::vector<Float>& value,
                           const std::vector<Float>& default_value);

  /*! Export a multi-Uint field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_uint(const std::string& name,
                          const std::vector<Uint>& value,
                          const std::vector<Uint>& default_value);

  /*! Export a multi-Int field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_int32(const std::string& name,
                           const std::vector<Int32>& value,
                           const std::vector<Int32>& default_value);

  /*! Export a multi-Scene_time field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_scene_time(const std::string& name,
                                const std::vector<Scene_time>& value,
                                const std::vector<Scene_time>& default_value);

  /*! Export a multi-Vector2f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_vector2f(const std::string& name,
                              const std::vector<Vector2f>& value,
                              const std::vector<Vector2f>& default_value);

  /*! Export a multi-Vector3f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_vector3f(const std::string& name,
                              const std::vector<Vector3f>& value,
                              const std::vector<Vector3f>& default_value);

  /*! Export a multi-Vector4f field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_vector4f(const std::string& name,
                              const std::vector<Vector4f>& value,
                              const std::vector<Vector4f>& default_value);

  /*! Export a multi-Rotation field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_rotation(const std::string& name,
                              const std::vector<Rotation>& value,
                              const std::vector<Rotation>& default_value);

  /*! Export a multi-Bounding_sphere field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_sphere_bound(const std::string& name,
                                  const std::vector<Bounding_sphere>& value,
                                  const std::vector<Bounding_sphere>& default_value);

  /*! Export a multi-string field.
   * \param name The field name.
   * \param value The field value.
   * \param default_value The field default value.
   */
  virtual void multi_string(const std::string& name,
                            const std::vector<std::string>& value,
                            const std::vector<std::string>& default_value);

  /*! Export the header of a multi-container field.
   */
  virtual void multi_container_begin(const std::string& name);

  /*! Export the tailer of a multi-container field */
  virtual void multi_container_end();
  //@}

  /*! The name so fthe containers that have been exported already. */
  std::set<std::string> m_names;

private:
  /*! Declare a single field of type T with default value.
   */
  template <typename T>
  void single_declaration(const std::string& rule, const std::string& type,
                          const std::string& name, const T& default_value)
  {
    new_line();
    indent();
    out() << rule << " " << type << " " << name << " " << default_value;
    new_line();
  }

  /*! Declare a multi field of type T.
   */
  template <typename T>
  void multi_declaration(const std::string& rule, const std::string& type,
                          const std::string& name, const T& value)
  {
    SGAL_error_msg("Not implemented yet!");
  }

  /*! Export a single field of type T.
   */
  template <typename T>
  void single_definition(const std::string& name, const T& value,
                    const T& default_value)
  {
    if (value == default_value) return;
    new_line();
    indent();
    out() << name << " " << value;
    new_line();
  }

  /*! Export a multi field of Array of type T.
   */
  template <typename T>
  void multi_definition(const std::string& name, const std::vector<T>& value,
                   const std::vector<T>& default_value)
  {
    if (value.size() == 0) return;
    if ((value.size() == default_value.size()) &&
        std::equal(value.begin(), value.end(), default_value.begin()))
      return;

    new_line();
    indent();
    out() << name << " [";
    new_line();
    push_indent();
    indent();
    std::copy(value.begin(), value.end(), std::ostream_iterator<T>(out(), " "));
    new_line();
    pop_indent();
    indent();
    out() << "]";
    new_line();
  }

  /*! Space holder for the output stream mode */
  // std::IO::Mode m_old_out_mode;

  /*! Space holder for the input stream mode */
  // IO::Mode m_old_in_mode;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief writes a single Boolean field.
inline void Vrml_formatter::single_boolean(const std::string& rule,
                                           const std::string& type,
                                           const std::string& name,
                                           Boolean value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Float field.
inline void Vrml_formatter::single_float(const std::string& rule,
                                         const std::string& type,
                                         const std::string& name,
                                         Float value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Uint field.
inline void Vrml_formatter::single_uint(const std::string& rule,
                                        const std::string& type,
                                        const std::string& name,
                                        Uint value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Int field.
inline void Vrml_formatter::single_int32(const std::string& rule,
                                         const std::string& type,
                                         const std::string& name,
                                         Int32 value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Scene_time field.
inline void Vrml_formatter::single_scene_time(const std::string& rule,
                                              const std::string& type,
                                              const std::string& name,
                                              Scene_time value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Vector2f field.
inline void Vrml_formatter::single_vector2f(const std::string& rule,
                                            const std::string& type,
                                            const std::string& name,
                                            const Vector2f& value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Vector3f field.
inline void Vrml_formatter::single_vector3f(const std::string& rule,
                                            const std::string& type,
                                            const std::string& name,
                                            const Vector3f& value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Vector4f field.
inline void Vrml_formatter::single_vector4f(const std::string& rule,
                                            const std::string& type,
                                            const std::string& name,
                                            const Vector4f& value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Rotation field.
inline void Vrml_formatter::single_rotation(const std::string& rule,
                                            const std::string& type,
                                            const std::string& name,
                                            const Rotation& value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single Bounding_sphere field.
inline void Vrml_formatter::single_sphere_bound(const std::string& rule,
                                                const std::string& type,
                                                const std::string& name,
                                                const Bounding_sphere& value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a single string field.
inline void Vrml_formatter::single_string(const std::string& rule,
                                          const std::string& type,
                                          const std::string& name,
                                          const std::string& value)
{ single_declaration(rule, type, name, value); }

//! \brief exports a multi-Boolean field.
inline void Vrml_formatter::multi_boolean(const std::string& rule,
                                          const std::string& type,
                                          const std::string& name,
                                          const std::vector<Boolean>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Float field.
inline void Vrml_formatter::multi_float(const std::string& rule,
                                        const std::string& type,
                                        const std::string& name,
                                        const std::vector<Float>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Uint field.
inline void Vrml_formatter::multi_uint(const std::string& rule,
                                       const std::string& type,
                                       const std::string& name,
                                       const std::vector<Uint>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Int field.
inline void Vrml_formatter::multi_int32(const std::string& rule,
                                        const std::string& type,
                                        const std::string& name,
                                        const std::vector<Int32>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Scene_time field.
inline void Vrml_formatter::multi_scene_time(const std::string& rule,
                                             const std::string& type,
                                             const std::string& name,
                                             const std::vector<Scene_time>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Vector2f field.
inline void Vrml_formatter::multi_vector2f(const std::string& rule,
                                           const std::string& type,
                                           const std::string& name,
                                           const std::vector<Vector2f>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Vector3f field.
inline void Vrml_formatter::multi_vector3f(const std::string& rule,
                                           const std::string& type,
                                           const std::string& name,
                                           const std::vector<Vector3f>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Vector4f field.
inline void Vrml_formatter::multi_vector4f(const std::string& rule,
                                           const std::string& type,
                                           const std::string& name,
                                           const std::vector<Vector4f>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Rotation field.
inline void Vrml_formatter::multi_rotation(const std::string& rule,
                                           const std::string& type,
                                           const std::string& name,
                                           const std::vector<Rotation>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-Bounding_sphere field.
inline void Vrml_formatter::multi_sphere_bound(const std::string& rule,
                                               const std::string& type,
                                               const std::string& name,
                                               const std::vector<Bounding_sphere>& value)
{ multi_declaration(rule, type, name, value); }

//! \brief exports a multi-string field.
inline void Vrml_formatter::multi_string(const std::string& rule,
                                         const std::string& type,
                                         const std::string& name,
                                         const std::vector<std::string>& value)
{ multi_declaration(rule, type, name, value); }

// Definitions
//! \brief exports a single Float field.
inline void Vrml_formatter::single_float(const std::string& name,
                                         Float value, Float default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Uint field.
inline void Vrml_formatter::single_uint(const std::string& name,
                                        Uint value, Uint default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Int field.
inline void Vrml_formatter::single_int32(const std::string& name,
                                         Int32 value, Int32 default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Scene_time field.
inline void Vrml_formatter::single_scene_time(const std::string& name,
                                              Scene_time value,
                                              Scene_time default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Vector2f field.
inline void Vrml_formatter::single_vector2f(const std::string& name,
                                            const Vector2f& value,
                                            const Vector2f& default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Vector3f field.
inline void Vrml_formatter::single_vector3f(const std::string& name,
                                            const Vector3f& value,
                                            const Vector3f& default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Vector4f field.
inline void Vrml_formatter::single_vector4f(const std::string& name,
                                            const Vector4f& value,
                                            const Vector4f& default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Rotation field.
inline void Vrml_formatter::single_rotation(const std::string& name,
                                            const Rotation& value,
                                            const Rotation& default_value)
{ single_definition(name, value, default_value); }

//! \brief exports a single Bounding_sphere field.
inline void
Vrml_formatter::single_sphere_bound(const std::string& name,
                                    const Bounding_sphere& value,
                                    const Bounding_sphere& default_value)
{ single_definition(name, value, default_value); }

// Multi-field
//! \brief exports a single Float field.
inline void Vrml_formatter::multi_float(const std::string& name,
                                        const std::vector<Float>& value,
                                        const std::vector<Float>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Uint field.
inline void Vrml_formatter::multi_uint(const std::string& name,
                                       const std::vector<Uint>& value,
                                       const std::vector<Uint>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Int field.
inline void Vrml_formatter::multi_int32(const std::string& name,
                                        const std::vector<Int32>& value,
                                        const std::vector<Int32>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Scene_time field.
inline void
Vrml_formatter::multi_scene_time(const std::string& name,
                                 const std::vector<Scene_time>& value,
                                 const std::vector<Scene_time>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Vector2f field.
inline void Vrml_formatter::multi_vector2f(const std::string& name,
                                           const std::vector<Vector2f>& value,
                                           const std::vector<Vector2f>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Vector3f field.
inline void Vrml_formatter::multi_vector3f(const std::string& name,
                                           const std::vector<Vector3f>& value,
                                           const std::vector<Vector3f>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Vector4f field.
inline void Vrml_formatter::multi_vector4f(const std::string& name,
                                           const std::vector<Vector4f>& value,
                                           const std::vector<Vector4f>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Rotation field.
inline void Vrml_formatter::multi_rotation(const std::string& name,
                                           const std::vector<Rotation>& value,
                                           const std::vector<Rotation>& default_value)
{ multi_definition(name, value, default_value); }

//! \brief exports a single Bounding_sphere field.
inline void
Vrml_formatter::multi_sphere_bound(const std::string& name,
                                   const std::vector<Bounding_sphere>& value,
                                   const std::vector<Bounding_sphere>& default_value)
{ multi_definition(name, value, default_value); }

SGAL_END_NAMESPACE

#endif
