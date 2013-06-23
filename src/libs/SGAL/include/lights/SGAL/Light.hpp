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
// $Source$
// $Revision: 6147 $
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

  /*! Constructor */
  Light(Boolean proto = false);

  /*! Destructor */
  virtual ~Light();

  /*! Clone */
  virtual Container* clone() { SGAL_assertion(0); return 0; }

  // ---  Get/Set Methods  ---
  void set_on(Boolean is_on);
  Boolean get_on();

  void set_intensity(Float intensity);
  Float get_intensity();

  void set_ambient_intensity(Float ambient_intensity);
  Float get_ambient_intensity();

  void set_color(const Vector3f& color);
  void get_color(Vector3f& color);

  // void pop(const Matrix4f& mat);

  virtual void cull(Cull_context& cull_context);
  virtual Action::Trav_directive draw(Draw_action* draw_action);
  
  void ref() { m_ref++; }
  void unref() { --m_ref; }
    
  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();
  
  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

protected:
  /*! a uniqu id for a light (0-7) */
  Int m_id;

  /*! indicates whether the light is on or off */
  Boolean m_is_on;

  /*! The intensity of the light (0-1) */
  Float m_intensity;

  /*! The ambient intensity of the light (0-1) */
  Float m_ambient_intensity;

  /*! The color of the light (applied to ambient, diffuse and specular) */
  Vector3f m_color;

private:
  // Defaults values

  /*! Indicates whether the light is on or off */
  static const Boolean m_def_is_on;

  /*! The intensity of the light (0-1) */
  static const Float m_def_intensity;

  /*! The ambient intensity of the light (0-1) */
  static const Float m_def_ambient_intensity;

  /*! The color of the light (applied to ambient, diffuse and specular) */
  static const Vector3f m_def_color;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Reference count */
  Int m_ref;

  // void init(void);
  // int m_dirty_id, m_dirty_count;
};

SGAL_END_NAMESPACE

#endif
