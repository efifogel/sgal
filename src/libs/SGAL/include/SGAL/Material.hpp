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
// $Revision: 12384 $
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
class Formatter;

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
  Material(Boolean proto = false);

  /*! Destructor */
  virtual ~Material();

  /*! Construct the prototype */
  static Material* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Set the ambient intensity */
  void set_ambient_intensity(Float intensity);

  /*! Obtain the ambient intensity */
  Float get_ambient_intensity() const;

  /*! Set the diffuse color */
  void set_diffuse_color(const Vector3f& color);

  /*! Set the diffuse color */
  void set_diffuse_color(Float v0, Float v1, Float v2);

  /*! Obtain the diffuse color */
  const Vector3f& get_diffuse_color() const;

  /*! Obtain the diffuse color */
  void get_diffuse_color(Vector3f& color) const;

  /*! Set the specular color */
  void set_specular_color(const Vector3f& color);

  /*! Set the specular color */
  void set_specular_color(Float v0, Float v1, Float v2);

  /*! Obtain the specular color */
  void get_specular_color(Vector3f& color) const;

  /*! Obtain the specular color */
  const Vector3f& get_specular_color() const;

  /*! Set the emissive color */
  void set_emissive_color(const Vector3f& color);

  /*! Set the emissive color */
  void set_emissive_color(Float v0, Float v1, Float v2);

  /*! Obtain the emissive color */
  const Vector3f& get_emissive_color() const;

  /*! Get the emissive color */
  void get_emissive_color(Vector3f& color) const;

  /*! Set the shininess */
  void set_shininess(Float shininess);

  /*! Obtain the shininess */
  Float get_shininess() const;

  /*! Set the transparency */
  void set_transparency(Float transparency);

  /*! Obtain the transparency */
  Float get_transparency() const;

  /*! The callback invoked when the material chages. */
  virtual void material_changed(Field_info* field_info = NULL);

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
  Float* ambient_intensity_handle(Field_info*) { return &m_ambient_intensity; }
  Vector3f* diffuse_color_handle(Field_info*) { return &m_diffuse_color; }
  Vector3f* specular_color_handle(Field_info*) { return &m_specular_color; }
  Vector3f* emissive_color_handle(Field_info*) { return &m_emissive_color; }
  Float* shininess_handle(Field_info*) { return &m_shininess; }
  Float* transparency_handle(Field_info*) { return &m_transparency; }
  //@}

  /*! Set the attributes of this container */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Write this container */
  virtual void write(Formatter* formatter);

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  // static unsigned int m_dither_matrix[1025][32];

  /*! the node prototype */
  static Container_proto* s_prototype;

  /*! Default Values */
  static Float m_def_ambient_intensity;
  static Vector3f m_def_diffuse_color;
  static Vector3f m_def_specular_color;
  static Vector3f m_def_emissive_color;
  static Float m_def_shininess;
  static Float m_def_transparency;

  /*! the ambient intensity */
  Float m_ambient_intensity;

  /*! the diffuse color */
  Vector3f m_diffuse_color;

  /*! the specular color */
  Vector3f m_specular_color;

  /*! the emissive color */
  Vector3f m_emissive_color;

  /*! the shininess */
  Float m_shininess;

  /*! the transparancy */
  Float m_transparency;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief construct the prototype. */
inline Material* Material::prototype() { return new Material(true); }

/*! \brief clones. */
inline Container* Material::clone() { return new Material(); }

/*! \brief obtains the ambient intensity. */
inline Float Material::get_ambient_intensity() const
{ return m_ambient_intensity; }

/*! \brief sets the diffuse color. */
inline void Material::set_diffuse_color(const Vector3f& color)
{ set_diffuse_color(color[0], color[1], color[2]); }

/*! \brief obtains the diffuse color. */
inline const Vector3f& Material::get_diffuse_color() const
{ return m_diffuse_color; }

/*! \brief obtains the diffuse color. */
inline void Material::get_diffuse_color(Vector3f& color) const
{ color = m_diffuse_color; }

/*! \brief sets the specular color. */
inline void Material::set_specular_color(const Vector3f& color)
{ set_specular_color(color[0], color[1], color[2]); }

/*! \brief obtains the specular color. */
inline void Material::get_specular_color(Vector3f& color) const
{ color = m_specular_color; }

/*! \brief obtains the specular color. */
inline const Vector3f& Material::get_specular_color() const
{ return m_specular_color; }

/*! \brief sets the emissive color */
inline void Material::set_emissive_color(const Vector3f& color)
{ set_emissive_color(color[0], color[1], color[2]); }

/*! \brief obtains the emissive color */
inline const Vector3f& Material::get_emissive_color() const
{ return m_emissive_color; }

/*! \brief gets the emissive color */
inline void Material::get_emissive_color(Vector3f& color) const
{ color = m_emissive_color; }

/*! \brief obtains the shininess */
inline Float Material::get_shininess() const
{ return m_shininess; }

/*! \brief obtains the transparency */
inline Float Material::get_transparency() const { return m_transparency;}

/*! \brief obtains the tag (type) of the container */
inline const std::string& Material::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
