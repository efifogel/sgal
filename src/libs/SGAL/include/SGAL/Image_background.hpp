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

#ifndef SGAL_IMAGE_BACKGROUND_HPP
#define SGAL_IMAGE_BACKGROUND_HPP

/*! \file
 * A node in the scene graph representing a background that is an image.
 */

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Background.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Appearance;
class Element;

class SGAL_SGAL_DECL Image_background : public Background {
public:
  enum {
    FIRST = Background::LAST - 1,
    LAST
  };

  typedef boost::shared_ptr<Appearance>                 Shared_appearance;
  
  /*! Constructor */
  Image_background(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_background();

  /*! Construct the prototype */
  static Image_background* prototype() { return new Image_background(true); }

  /*! Clone */
  virtual Container* clone() { return new Image_background(); }

  /*! Set the attributes of this container */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();;

  virtual Container_proto* get_prototype(); 

  virtual void draw(Draw_action* draw_action);

  Shared_appearance get_appearance() { return m_appearance; }

  void set_appearance(Shared_appearance appearance);

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }
  
private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! An apperance used for the background */
  Shared_appearance m_appearance;

  /*! Indicates whether the appearance is a default */
  Boolean m_is_default_appearance;
};

SGAL_END_NAMESPACE

#endif
