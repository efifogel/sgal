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

/*! \brief The material container represents a material
 *
 * A material has the following properties:
 *    ambient Intensity
 *    ambient color
 *    diffuse color
 *    specular color
 *    emissive color
 *    (ambient index)
 *    (diffuse index)
 *    (specular index)
 *    shininess
 *    transparency
 *
 * Inherits from Container.
 */

#ifndef SGAL_MATERIAL_HPP
#define SGAL_MATERIAL_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Container_proto;
class Element;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \brief A representation of a material */
class SGAL_SGAL_DECL Material : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    AMBIENT_INTENSITY,
    AMBIENT_COLOR,
    DIFFUSE_COLOR,
    SPECULAR_COLOR,
    EMISSIVE_COLOR,
    SHININESS,
    TRANSPARENCY,
    LAST
  };

  enum Face {
    FRONT,
    BACK,
    FRONT_AND_BACK
  };

  /*! Constructor */
  Material(bool proto = false);

  /*! Destructor */
  virtual ~Material();

  /*! Construct the prototype */
  static Material* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Set the ambient intensity */
  void set_ambient_intensity(float intensity);

  /*! Obtain the ambient intensity */
  float get_ambient_intensity();

  /*! Set the ambient color */
  void set_ambient_color(const Vector3f& color);

  /*! Set the ambient color */
  void set_ambient_color(float red, float green, float blue);

  /*! Obtain the ambient color */
  const Vector3f& get_ambient_color();

  /*! Set the diffuse color */
  void set_diffuse_color(const Vector3f& color);

  /*! Set the diffuse color */
  void set_diffuse_color(float red, float green, float blue);

  /*! Obtain the diffuse color */
  const Vector3f& get_diffuse_color() const;

  /*! Obtain the diffuse color */
  void get_diffuse_color(Vector3f& color) const;

  /*! Set the specular color */
  void set_specular_color(const Vector3f& color);

  /*! Set the specular color */
  void set_specular_color(float red, float green, float blue);

  /*! Obtain the specular color */
  void get_specular_color(Vector3f& color) const;

  /*! Obtain the specular color */
  const Vector3f& get_specular_color() const;

  /*! Set the emissive color */
  void set_emissive_color(const Vector3f& color);

  /*! Set the emissive color */
  void set_emissive_color(float red, float green, float blue);

  /*! Obtain the emissive color */
  const Vector3f& get_emissive_color() const;

  /*! Get the emissive color */
  void get_emissive_color(Vector3f& color) const;

  /*! Set the shininess */
  void set_shininess(float shininess);

  /*! Obtain the shininess */
  float get_shininess() const;

  /*! Set the transparency */
  void set_transparency(float transparency);

  /*! Obtain the transparency */
  float get_transparency() const;

  /*! The callback invoked when the material chages. */
  virtual void material_changed(const Field_info* field_info = nullptr);

  /*! Apply the material. */
  virtual void draw(Face which_face, Context* ctx);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  float* ambient_intensity_handle(const Field_info*)
  { return &m_ambient_intensity; }
  Vector3f* ambient_color_handle(const Field_info*) { return &m_ambient_color; }
  Vector3f* diffuse_color_handle(const Field_info*) { return &m_diffuse_color; }
  Vector3f* specular_color_handle(const Field_info*)
  { return &m_specular_color; }
  Vector3f* emissive_color_handle(const Field_info*)
  { return &m_emissive_color; }
  float* shininess_handle(const Field_info*) { return &m_shininess; }
  float* transparency_handle(const Field_info*) { return &m_transparency; }
  //@}

  /*! Set the attributes of this container */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  // static unsigned int m_dither_matrix[1025][32];

  /*! the node prototype */
  static Container_proto* s_prototype;

  /*! the ambient intensity */
  float m_ambient_intensity;

  /*! the ambient color */
  Vector3f m_ambient_color;

  /*! the diffuse color */
  Vector3f m_diffuse_color;

  /*! the specular color */
  Vector3f m_specular_color;

  /*! the emissive color */
  Vector3f m_emissive_color;

  /*! the shininess */
  float m_shininess;

  /*! the transparancy */
  float m_transparency;

  //! Indicates whether the ambient intensity should be cleaned.
  bool m_dirty_ambient_intensity;

  //! Indicates whether the ambient color should be cleaned.
  bool m_dirty_ambient_color;

  //! Default Values
  static const float s_def_ambient_intensity;
  static const Vector3f s_def_ambient_color;
  static const Vector3f s_def_diffuse_color;
  static const Vector3f s_def_specular_color;
  static const Vector3f s_def_emissive_color;
  static const float s_def_shininess;
  static const float s_def_transparency;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief construct the prototype.
inline Material* Material::prototype() { return new Material(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Material::create() { return new Material(); }

//! \brief sets the ambient color.
inline void Material::set_ambient_color(const Vector3f& color)
{ set_ambient_color(color[0], color[1], color[2]); }

//! \brief sets the diffuse color.
inline void Material::set_diffuse_color(const Vector3f& color)
{ set_diffuse_color(color[0], color[1], color[2]); }

//! \brief obtains the diffuse color.
inline const Vector3f& Material::get_diffuse_color() const
{ return m_diffuse_color; }

//! \brief obtains the diffuse color.
inline void Material::get_diffuse_color(Vector3f& color) const
{ color = m_diffuse_color; }

//! \brief sets the specular color.
inline void Material::set_specular_color(const Vector3f& color)
{ set_specular_color(color[0], color[1], color[2]); }

//! \brief obtains the specular color.
inline void Material::get_specular_color(Vector3f& color) const
{ color = m_specular_color; }

//! \brief obtains the specular color.
inline const Vector3f& Material::get_specular_color() const
{ return m_specular_color; }

//! \brief sets the emissive color.
inline void Material::set_emissive_color(const Vector3f& color)
{ set_emissive_color(color[0], color[1], color[2]); }

//! \brief obtains the emissive color.
inline const Vector3f& Material::get_emissive_color() const
{ return m_emissive_color; }

//! \brief gets the emissive color.
inline void Material::get_emissive_color(Vector3f& color) const
{ color = m_emissive_color; }

//! \brief obtains the shininess.
inline float Material::get_shininess() const
{ return m_shininess; }

//! \brief obtains the transparency.
inline float Material::get_transparency() const { return m_transparency;}

//! \brief obtains the tag (type) of the container.
inline const std::string& Material::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
