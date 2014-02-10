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
// $Revision: 12347 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VRML_FORMATTER_HPP
#define SGAL_VRML_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"

SGAL_BEGIN_NAMESPACE

class Vector2f;
class Vector3f;
class Vector4f;
class Rotation;
class Sphere_bound;

/*! Exports a scene graph to an output stream in the VRML format */
class SGAL_SGAL_DECL Vrml_formatter : public Text_formatter {
public:
  /*! Constructor */
  Vrml_formatter();

  /*! Construct an output formatter from an output stream.
   * \param os the output stream.
   */
  Vrml_formatter(std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param is the input stream.
   */
  Vrml_formatter(std::istream& is);

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

  /*! Export the container header.
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
  /*! Export a single field of type T.
   */
  template <typename T>
  void single_field(const std::string& name, const T& value,
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
  void multi_field(const std::string& name, const Array<T>& value,
                   const Array<T>& default_value)
  {
    const T* vec = value.get_vector();
    const T* default_vec = default_value.get_vector();
    if (value.size() == 0) return;
    if ((value.size() == default_value.size()) &&
        std::equal(&vec[0], &vec[value.size()], &default_vec[value.size()]))
      return;

    new_line();
    indent();
    out() << name << " [";
    new_line();
    push_indent();
    indent();
    std::copy(vec, &vec[value.size()], std::ostream_iterator<T>(out(), " "));
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

//! \brief writes a single Float field.
inline void Vrml_formatter::single_float(const std::string& name,
                                         Float value, Float default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Uint field.
inline void Vrml_formatter::single_uint(const std::string& name,
                                        Uint value, Uint default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Int field.
inline void Vrml_formatter::single_int(const std::string& name,
                                       Int value, Int default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Scene_time field.
inline void Vrml_formatter::single_scene_time(const std::string& name,
                                              Scene_time value,
                                              Scene_time default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Vector2f field.
inline void Vrml_formatter::single_vector2f(const std::string& name,
                                            const Vector2f& value,
                                            const Vector2f& default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Vector3f field.
inline void Vrml_formatter::single_vector3f(const std::string& name,
                                            const Vector3f& value,
                                            const Vector3f& default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Vector4f field.
inline void Vrml_formatter::single_vector4f(const std::string& name,
                                            const Vector4f& value,
                                            const Vector4f& default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Rotation field.
inline void Vrml_formatter::single_rotation(const std::string& name,
                                            const Rotation& value,
                                            const Rotation& default_value)
{ single_field(name, value, default_value); }

//! \brief writes a single Sphere_bound field.
inline void
Vrml_formatter::single_sphere_bound(const std::string& name,
                                    const Sphere_bound& value,
                                    const Sphere_bound& default_value)
{ single_field(name, value, default_value); }

// Multi-field
//! \brief writes a single Float field.
inline void Vrml_formatter::multi_float(const std::string& name,
                                        const Array<Float>& value,
                                        const Array<Float>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Uint field.
inline void Vrml_formatter::multi_uint(const std::string& name,
                                       const Array<Uint>& value,
                                       const Array<Uint>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Int field.
inline void Vrml_formatter::multi_int(const std::string& name,
                                      const Array<Int>& value,
                                      const Array<Int>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Scene_time field.
inline void
Vrml_formatter::multi_scene_time(const std::string& name,
                                 const Array<Scene_time>& value,
                                 const Array<Scene_time>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Vector2f field.
inline void Vrml_formatter::multi_vector2f(const std::string& name,
                                           const Array<Vector2f>& value,
                                           const Array<Vector2f>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Vector3f field.
inline void Vrml_formatter::multi_vector3f(const std::string& name,
                                           const Array<Vector3f>& value,
                                           const Array<Vector3f>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Vector4f field.
inline void Vrml_formatter::multi_vector4f(const std::string& name,
                                           const Array<Vector4f>& value,
                                           const Array<Vector4f>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Rotation field.
inline void Vrml_formatter::multi_rotation(const std::string& name,
                                           const Array<Rotation>& value,
                                           const Array<Rotation>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single Sphere_bound field.
inline void
Vrml_formatter::multi_sphere_bound(const std::string& name,
                                   const Array<Sphere_bound>& value,
                                   const Array<Sphere_bound>& default_value)
{ multi_field(name, value, default_value); }

//! \brief writes a single string field.
inline void
Vrml_formatter::multi_string(const std::string& name,
                             const Array<std::string>& value,
                             const Array<std::string>& default_value)
{ multi_field(name, value, default_value); }

SGAL_END_NAMESPACE

#endif
