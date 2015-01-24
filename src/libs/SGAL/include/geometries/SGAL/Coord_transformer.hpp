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

#include <string>

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;
class Container_proto;
class Coord_array;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Coord_transformer Coord_transformer.hpp
 * \todo move to engines.
 */
class SGAL_SGAL_DECL Coord_transformer : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    EXECUTE,
    COORD,
    COORD_CHANGED,
    TRANSLATION,
    ROTATION,
    SCALE,
    CHANGED,
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
   * \param elem (in) lists of attribute names and values.
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
  Vector3f* translation_handle(const Field_info*) { return &m_translation; }
  Rotation* rotation_handle(const Field_info*) { return &m_rotation; }
  Vector3f* scale_handle(const Field_info*) { return &m_scale; }
  Boolean* execute_handle(const Field_info*) { return &m_execute; }
  Shared_coord_array* coord_array_handle(const Field_info*)
  { return &m_coord_array; }
  Shared_coord_array* coord_array_changed_handle(const Field_info*)
  { return &m_coord_array_changed; }
  Boolean* changed_handle(const Field_info*) { return &m_changed; }
  //@}

  /*! Translate the input vertices. */
  void translate(const Field_info* field_info = nullptr);

  /*! Rotate the input vertices. */
  void rotate(const Field_info* field_info = nullptr);

  /*! Scale the input vertices. */
  void scale(const Field_info* field_info = nullptr);

  /*! Transform the input vertices. */
  void execute(const Field_info* field_info = nullptr);

  /*! Set the rotation field. */
  void set_rotation(const Rotation& rotation);

  /*! Set the rotation field. */
  void set_rotation(float v0, float v1, float v2, float v3);

  /*! Set the translation field. */
  void set_translation(const Vector3f& translation);

  /*! Set the translation field. */
  void set_translation(float v0, float v1, float v2);

  /*! Set the scale field. */
  void set_scale(const Vector3f& scale);

  /*! Set the scale field. */
  void set_scale(float v0, float v1, float v2);

  /*! Set the reflection indication-flag. */
  void set_reflect(Boolean reflect);

  /*! Determine whether to reflect the coordinates. */
  Boolean do_reflect() const;

  /*! Set the coordinate-set node. */
  void set_coord_array(Shared_coord_array coord);

  /*! Obtain the coordinate-set node. */
  Shared_coord_array get_coord_array() const;

  /*! Obtain the changed coordinate-set node. */
  Shared_coord_array get_coord_array_changed() const;

protected:
  /*! The input vertices */
  Shared_coord_array m_coord_array;

  /*! The output vertices */
  Shared_coord_array m_coord_array_changed;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Apply the transformation. */
  virtual void apply();

  /*! Reflect the coordinates.
   * \param begin (in) points at the begin iterator of the input range of
   *        coordinates
   * \param end (in) points at the pass-the-end iterator of the input range of
   *        coordinates
   * \param out (out) points at the begin iterator of the transformed range of
   *        coordinates
   */
  template <typename Input_iterator, typename Output_iterator>
  void reflect(Input_iterator begin, Input_iterator end,
               Output_iterator out)
  { for (auto it = begin; it != end; ++it) out++->negate(*it); }

  /*! Transform the coordinates.
   */
  template <typename Input_iterator, typename Output_iterator>
  void transform(Input_iterator begin, Input_iterator end,
                 Output_iterator out)
  {
    const Matrix4f& mat = m_transform.get_matrix();
    for (auto it = begin; it != end; ++it) out++->xform_pt(*it, mat);
  }

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Determines whether the node is enabled. */
  Boolean m_enabled;

  /*! Indicates whether reflection should be applied. */
  Boolean m_reflect;

  /*! Indicates that the operation should be executed. */
  Boolean m_execute;

  /*! The transformation. */
  Transform m_transform;

  /*! The rotation vector. */
  Rotation m_rotation;

  /*! The translation vector. */
  Vector3f m_translation;

  /*! The scale vector. */
  Vector3f m_scale;

  /*! Indicates whether output has changed.
   * This is temporary; it should be eliminated once script nodes fully
   * support (shared) containers as input.
   */
  Boolean m_changed;

  // default values
  static const Boolean s_def_enabled;
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

//! \brief determines whether to reflect the coordinates.
inline Boolean Coord_transformer::do_reflect() const { return m_reflect; }

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
