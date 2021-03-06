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
 * A base class representing a light.
 *
 * A material has the following properties:
 *    is on (boolean)
 *    intensity (0-1)
 *    ambient intensity (0-1)
 *    color - assumed for ambient, diffuse and specular
 *
 *  Inherits from Node.
 */

#ifndef SGAL_LIGHT_HPP
#define SGAL_LIGHT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Scene_graph;

class SGAL_SGAL_DECL Light : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    ON,
    INTENSITY,
    AMBIENT_INTENSITY,
    COLOR,
    LAST
  };

  /*! Construct. */
  Light(Boolean proto = false);

  /*! Destruct. */
  virtual ~Light();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* is_on_handle(const Field_info*) { return &m_is_on; }
  Vector3f* color_handle(const Field_info*) { return &m_color; }
  Float* intensity_handle(const Field_info*) { return &m_intensity; }
  Float* ambient_intensity_handle(const Field_info*)
  { return &m_ambient_intensity; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  void set_on(Boolean is_on);

  Boolean get_on();

  void set_intensity(Float intensity);

  Float get_intensity();

  void set_ambient_intensity(Float ambient_intensity);

  Float get_ambient_intensity();

  void set_color(const Vector3f& color);

  /*! Obtain the (const) light color.
   */
  const Vector3f& get_color() const;

  /*! Obtain the (non-const) light color.
   */
  Vector3f& get_color();

  // void pop(const Matrix4f& mat);

  virtual void cull(Cull_context& cull_context);

  virtual Action::Trav_directive draw(Draw_action* draw_action);

  void ref() { m_ref++; }
  void unref() { --m_ref; }

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

protected:
  //! a uniqe id for a light (0-7).
  Int32 m_id;

  //! indicates whether the light is on or off.
  Boolean m_is_on;

  //! The intensity of the light (0-1).
  Float m_intensity;

  //! The ambient intensity of the light (0-1).
  Float m_ambient_intensity;

  //! The color of the light (applied to ambient, diffuse and specular).
  Vector3f m_color;

private:
  // Defaults values

  //! Indicates whether the light is on or off.
  static const Boolean m_def_is_on;

  //! The intensity of the light (0-1).
  static const Float m_def_intensity;

  //! The ambient intensity of the light (0-1).
  static const Float m_def_ambient_intensity;

  //! The color of the light (applied to ambient, diffuse and specular).
  static const Vector3f m_def_color;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! Reference count.
  Uint m_ref;

  // void init(void);
  // int m_dirty_id, m_dirty_count;
};

//! \brief sets the light color.
inline void Light::set_color(const Vector3f& color) { m_color = color; }

//! \brief obtains the (const) light color.
inline const Vector3f& Light::get_color() const { return m_color; }

//! \brief obtains the (non-const) light color.
inline Vector3f& Light::get_color() { return m_color; }

SGAL_END_NAMESPACE

#endif
