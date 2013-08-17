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

#ifndef SGAL_ENVIRONMENT_BACKGROUND_HPP
#define SGAL_ENVIRONMENT_BACKGROUND_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Background.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Image_texture.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;
class Container_proto;
class Appearance;

/*! Virtual base class for backgrounds */
class Environment_background : public Background {
public:
  enum {
    FIRST = Background::LAST - 1,
    LAST
  };

  /*! Constructor */
  Environment_background(Boolean proto = false);

  /*! Destructor */
  virtual ~Environment_background();

  /*! Construct the prototype */
  static Environment_background * prototype()
  { return new Environment_background(true); }

  /*! Clone */
  virtual Container * clone() { return new Environment_background(); }

  /*! Set the attributes of this container */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();;

  /*! Obtain the node prototype */
  virtual Container_proto * get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Draw the background */
  virtual void draw(Draw_action * draw_action);

  /*! Set the sky color */
  void set_sky_color(const Vector3f & sky_color) { m_sky_color = sky_color; }

  /*! Obtain the sky color */
  const Vector3f & get_sky_color() const { return m_sky_color; }

  /*! Set the appearance of the object  */
  void set_back_appearance(Appearance * app);

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The background color */
  Vector3f m_sky_color;

  /*! Default background color. */
  static const Vector3f m_def_sky_color;

  /*! An apperance container used to hold the back texture */
  Appearance * m_back_appearance;

  /*! The back texture */
  Image_texture * m_back_texture;
};

SGAL_END_NAMESPACE

#endif
