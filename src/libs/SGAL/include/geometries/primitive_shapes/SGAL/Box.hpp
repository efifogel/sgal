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

/*! An implementation of the Box geometry.
 *                        
 *  A geometry of type box. The box is specified by its center and
 *  size.                            
 *              
 *  Inherits from Geometry.
 */

#ifndef SGAL_BOX_HPP
#define SGAL_BOX_HPP

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Container_proto;

class SGAL_CLASSDEF Box : public Geometry {
public:
  /*! Fields */
  enum {
    FIRST = Geometry::LAST - 1,
    SIZE,
    LAST
  };

  /*! Constructor */
  Box(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Box();

  /* Construct the prototype */
  static Box * prototype() { return new Box(SGAL_TRUE); }

  /*! clones a new instance */
  virtual Container * clone() { return new Box(); }

  /* sets the size of the box */
  void set_size(const Vector3f & size) { m_size = size; }

  /* gets the size of the box */
  void get_size(Vector3f & size) const { size = m_size; }

  /* gets the size of the box */
  Vector3f get_size() const { return m_size; }

  /*! */
  virtual void draw(Draw_action * action); 

  /*! */
  virtual void isect(Isect_action* action);

  /*! */
  virtual Boolean calculate_sphere_bound();

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto * get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Return true if the geometry has color (as opposed to material) */
  virtual Boolean has_color() const { return SGAL_FALSE; }

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

  /*! The size of the box */
  Vector3f m_size;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! Initialize the display list used to draw the box */
  void init();

  /*! Draw the box */
  void draw_box();

private:
  /*! the default size of the box */
  static const Vector3f m_def_size;
};

SGAL_END_NAMESPACE

#endif
