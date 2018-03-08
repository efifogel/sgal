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

#ifndef SGAL_COLOR_BACKGROUND_HPP
#define SGAL_COLOR_BACKGROUND_HPP

/*! \file
 * A fixed color background.
 *
 * Inherits from Background
 */

#include "SGAL/basic.hpp"
#include "SGAL/Background.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Draw_action;

class Color_background : public Background {
public:
  enum {
    FIRST = Background::LAST - 1,
    COLOR,
    LAST
  };

  /*! Constructor */
  Color_background(Boolean proto = false);

  /*! Destructor */
  virtual ~Color_background();

  /*! Construct the prototype */
  static Color_background* prototype() { return new Color_background(true); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! \breif drwas the background */
  virtual void draw(Draw_action* draw_action);

  /*! Set the attributes of this container */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();;

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector4f* color_handle(const Field_info*) { return &m_color; }
  //@}

  /*! Set the color of the background */
  void set_color(const Vector4f& color) { m_color = color; };

  /*! Obtain the color of the background */
  const Vector4f& get_color() const { return m_color; }

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The background color */
  Vector4f m_color;

  /*! Default background color. */
  static const Vector4f m_def_color;
};

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Color_background::create()
{ return new Color_background(); }

SGAL_END_NAMESPACE

#endif
