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

/*!
 * Te texture_transform class enables simple geometric transformations, scale,
 * rotation, and translation, on texture coordinates
 */

#ifndef SGAL_TEXTURE_TRANSFORM_HPP
#define SGAL_TEXTURE_TRANSFORM_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Context;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Texture_transform Texture_transform.hpp
 */
class SGAL_SGAL_DECL Texture_transform : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    CENTER,
    ROTATION,
    SCALE,
    TRANSLATION,
    LAST
  };

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Texture_transform(Boolean proto = false);

  /*! Destructor. */
  virtual ~Texture_transform();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Texture_transform* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Vector2f* center_handle(const Field_info*) { return &m_center; }
  Float* rotation_handle(const Field_info*) { return &m_rotation; }
  Vector2f* scale_handle(const Field_info*) { return &m_scale; }
  Vector2f* translation_handle(const Field_info*) { return &m_translation; }
  //@}

  /*! Set the attributes of the transform.
   * \param elem (in) contains lists of attribute name and value pairs.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Apply the material. */
  virtual void draw(Context* context);

  /*! Set the center point of the rotation.
   */
  void set_center(const Vector2f& center);

  /*! Obtain the center point of the rotation.
   */
  const Vector2f& get_center();

  /*! Set the rotation angle.
   */
  void set_rotation(Float center);

  /*! Obtain the rotation angle.
   */
  Float get_rotation();

  /*! Set the a non-uniform scale about an arbitrary center point.
   */
  void set_scale(const Vector2f& scale);

  /*! Obtain the non-uniform scale about an arbitrary center point.
   */
  const Vector2f& get_scale();

  /*! Set the translation.
   */
  void set_translation(const Vector2f& translation);

  /*! Obtain the translation.
   */
  const Vector2f& get_translation();

  /*! Process change of parts.
   * \param field_info (in) the field information record of the particular
   *                   part that got changed.
   */
  void parts_changed(const Field_info* field_info = nullptr);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  Vector2f m_center;
  Float m_rotation;
  Vector2f m_scale;
  Vector2f m_translation;

  /*! The matrix representing the transform. */
  Matrix4f m_matrix;

  /*! Indicates whether the matrix is not up-to-date. */
  Boolean m_dirty_matrix;

  /*! Indicates whether the individual parts are dirty and thus must be
   * cleaned. The parts become dirty if the matrix that represents the
   * transform is set, for example.
   */
  Boolean m_dirty_parts;

  // Default values
  static const Vector2f s_def_center;
  static const Float s_def_rotation;
  static const Vector2f s_def_scale;
  static const Vector2f s_def_translation;

  /*! Calculate the matrix out of the individual parts. */
  void clean_matrix();

  /*! Extract the individual Transformations (e.g., scale) from the matrix. */
  void clean_parts();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Texture_transform* Texture_transform::prototype()
{ return new Texture_transform(true); }

//! \brief clones.
inline Container* Texture_transform::clone()
{ return new Texture_transform(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Texture_transform::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
