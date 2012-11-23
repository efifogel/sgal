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

/*! \file
 * A geometry container that represents an ellipsoid
 */

#ifndef SGAL_ELLIPSOID_HPP
#define SGAL_ELLIPSOID_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Ellipsoid : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    WIDTH,
    HEIGHT,
    DEPTH,
    STACKS, 
    SLICES,
    LAST
  };

  /*! Constructor */
  Ellipsoid(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Ellipsoid();

  /* Construct the prototype */
  static Ellipsoid * prototype() { return new Ellipsoid(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Ellipsoid(); }

  /*! Set the width of the ellipsoid */
  void set_width(Float width) { m_width = width; }

  /*! Obtain the width of the ellipsoid */
  Float get_width() const { return m_width; }

  /*! Set the height of the ellipsoid */
  void set_height(Float height) { m_height = height; }

  /*! Obtain the height of the ellipsoid */
  Float get_height() const { return m_height; }

  /*! Set the depth of the ellipsoid */
  void set_depth(Float depth) { m_depth = depth; }

  /*! Obtain the depth of the ellipsoid */
  Float get_depth() const { return m_depth; }

  /*! Set the number of slices (horizontal) longitudes */
  void set_slices(Uint slices) { m_slices = slices; }

  /*! Obtain the number of slices (horizontal) longitudes */
  Uint get_slices() const { return m_slices; }

  /*! Set the number of stacks (vertical) latitudes */
  void set_stacks(Uint stacks) { m_stacks = stacks; }

  /*! Obtain the number of stacks (vertical) latitudes */
  Uint get_stacks() const { return m_stacks; }

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype */
  virtual Container_proto * get_prototype();

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation */
  virtual void clean();
  
protected:
  /*! The width of the ellipsoid */
  Float m_width;

  /*! The height of the ellipsoid */
  Float m_height;

  /*! The depth of the ellipsoid */
  Float m_depth;
  
  /*! The number of vertical slices (horizontal) longitudes */
  Uint m_slices;

  /*! The number of horizontal stacks (vertical) latitudes */
  Uint m_stacks;

  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto * s_prototype;

  /*! default values for Ellipsoid.  */
  static const Float s_def_width;
  static const Float s_def_height;
  static const Float s_def_depth;
  static const Uint s_def_slices;
  static const Uint s_def_stacks;
};

SGAL_END_NAMESPACE

#endif
