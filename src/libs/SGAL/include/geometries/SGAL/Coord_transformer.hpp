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

#ifndef SGAL_COORD_TRANSFORMER_HPP
#define SGAL_COORD_TRANSFORMER_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Transform.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;
class Container_proto;
class Coord_array;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Coord_transformer : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    EXECUTE,
    CHANGED,
    COORD,
    COORD_CHANGED,
    TRANSLATION,
    ROTATION,
    TRANSLATED,
    ROTATED,
    LAST
  };

  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;

  /*! Constructor. */
  Coord_transformer(Boolean proto = false);

  /* Construct the prototype. */
  static Coord_transformer* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the prototype
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* enabled_handle(const Field_info*) { return &m_enabled; }
  Boolean* changed_handle(const Field_info*) { return &m_changed; }
  Boolean* translated_handle(const Field_info*) { return &m_translated; }
  Boolean* rotated_handle(const Field_info*) { return &m_rotated; }
  Vector3f* translation_handle(const Field_info*) { return &m_translation; }
  Rotation* rotation_handle(const Field_info*) { return &m_rotation; }
  Boolean* execute_handle(const Field_info*) { return &m_execute; }
  Shared_coord_array* coord_array_handle(const Field_info*)
  { return &m_coord_array; }
  Shared_coord_array* coord_array_changed_handle(const Field_info*)
  { return &m_coord_array_changed; }
  //@}

  /*! Apply the current transformation on the coordinates. */
  void apply();

  /*! Translate the input vertices. */
  void translate(const Field_info* field_info = NULL);

  /*! Rotate the input vertices. */
  void rotate(const Field_info* field_info = NULL);

  /*! Transform the input vertices. */
  void execute(const Field_info* field_info = NULL);

  /*! Set the rotation field. */
  void set_rotation(const Rotation& rotation);

  /*! Set the rotation field. */
  void set_rotation(float v0, float v1, float v2, float v3);

  /*! Set the translation field. */
  void set_translation(const Vector3f& translation);

  /*! Set the translation field. */
  void set_translation(float v0, float v1, float v2);

  /*! Set the reflection indication-flag. */
  void set_reflect(Boolean reflect);

  /*! Obtain the reflection indication-flag. */
  Boolean get_reflect() const;

  /*! Set the coordinate-set node. */
  void set_coord_array(Shared_coord_array coord);

  /*! Obtain the coordinate-set node. */
  Shared_coord_array get_coord_array() const;

  /*! Obtain the changed coordinate-set node. */
  Shared_coord_array get_coord_array_changed() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Determines whether the node is enabled */
  Boolean m_enabled;

  /*! Indicates whether reflection should be applied */
  Boolean m_reflect;

  /*! Indicates that the transformation has been applied */
  Boolean m_changed;

  /*! Indicates that the operation should be executed */
  Boolean m_execute;

  /*! Indicates that a translation has been applied */
  Boolean m_translated;

  /*! Indicates that a rotation has been applied */
  Boolean m_rotated;

  /*! The transformation */
  Transform m_transform;

  /*! The rotation vector */
  Rotation m_rotation;

  /*! The translation vector */
  Vector3f m_translation;

  /*! The input vertices */
  Shared_coord_array m_coord_array;

  /*! The output vertices */
  Shared_coord_array m_coord_array_changed;

  // default values
  static Boolean s_def_enabled;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Coord_transformer* Coord_transformer::prototype()
  { return new Coord_transformer(true); }

//! \brief clones.
inline Container* Coord_transformer::clone()
{ return new Coord_transformer(); }

//! \brief sets the reflection indication-flag.
inline void Coord_transformer::set_reflect(Boolean reflect)
{ m_reflect = reflect; }

//! \brief obtains the reflection indication-flag.
inline Boolean Coord_transformer::get_reflect() const { return m_reflect; }

//! \brief sets the coordinate-set node.
inline void Coord_transformer::set_coord_array(Shared_coord_array coord)
{ m_coord_array = coord; }

//! \brief obtains the coordinate-set node.
inline Coord_transformer::Shared_coord_array
Coord_transformer::get_coord_array() const
{ return m_coord_array; }

//! \brief obtains the changed coordinate-set node.
inline Coord_transformer::Shared_coord_array
Coord_transformer::get_coord_array_changed() const
{ return m_coord_array_changed; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Coord_transformer::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
