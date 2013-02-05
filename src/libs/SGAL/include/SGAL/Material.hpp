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

/*! \brief A representation of a material */
class Material : public Container {
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

  /*! Constructor */
  Material(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Material();

  /*! Construct the prototype */
  static Material * prototype() { return new Material(SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Material(); }

  enum Face {
    FRONT,
    BACK,
    FRONT_AND_BACK
  };

  /*! Set the ambient intensity */
  void set_ambient_intensity(Float intensity)
  { m_ambient_intensity = intensity; }

  /*! Obtain the ambient intensity */
  Float get_ambient_intensity() const { return m_ambient_intensity; }

  /*! Set the diffuse color */
  void set_diffuse_color(const Vector3f& color) { m_diffuse_color = color; }

  /*! Set the diffuse color */
  void set_diffuse_color(float v0, float v1, float v2);

  /*! Obtain the diffuse color */
  const Vector3f& get_diffuse_color() const { return m_diffuse_color; }
  
  /*! Obtain the diffuse color */
  void get_diffuse_color(Vector3f& color) const { color = m_diffuse_color; }

  /*! Set the specular color */
  void set_specular_color(const Vector3f& color) { m_specular_color = color; }

  /*! Set the specular color */
  void set_specular_color(float v0, float v1, float v2);

  /*! Obtain the specular color */
  void get_specular_color(Vector3f& color) const { color = m_specular_color; }

  /*! Obtain the specular color */
  const Vector3f& get_specular_color() const { return m_specular_color; }
  
  /*! Set the emissive color */
  void set_emissive_color(const Vector3f& color) { m_emissive_color = color; }

  /*! Set the emissive color */
  void set_emissive_color(float v0, float v1, float v2);

  /*! Obtain the emissive color */
  const Vector3f& get_emissive_color() const { return m_emissive_color; }
  
  /*! Get the emissive color */
  void get_emissive_color(Vector3f& color) const { color = m_emissive_color; }

  /*! Set the shininess */
  void set_shininess(Float shininess);

  /*! Obtain the shininess */
  Float get_shininess() const { return m_shininess; }

  /*! Set the transparency */
  void set_transparency(Float transparency) { m_transparency = transparency; }

  /*! Obtain the transparency */
  Float get_transparency() const { return m_transparency;}

  bool is_changed();
  virtual void material_changed(Field_info* field_info = NULL);

  virtual void draw(Face which_face, Context* ctx);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this container */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Write this container */
  virtual void write(Formatter* formatter);

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

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

  /*! indicates that the material attribute has changed */
  bool m_changed;
};

SGAL_END_NAMESPACE

#endif
