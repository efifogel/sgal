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

#ifndef SGAL_COORD_MINKOWSKI_HPP
#define SGAL_COORD_MINKOWSKI_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;
class Container_proto;
class Coord_array;

class Coord_minkowski : public Container {
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

protected:
  /*! Determines whether the node is enabled */
  bool m_enabled;

  /*! Indicates that the transformation has been applied */
  bool m_changed;

  /*! Indicates that the operation should be executed */
  bool m_execute;

  /*! The input vertices */
  Coord_array * m_coord_array1;

  /*! The input vertices */
  Coord_array * m_coord_array2;
  
  /*! The output vertices */
  Coord_array * m_coord_array_changed;

  // default values
  static bool s_def_enabled;

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    EXECUTE,
    CHANGED,
    COORD1,
    COORD2,
    COORD_CHANGED,
    LAST
  };

  /*! Constructor */
  Coord_minkowski(Boolean proto = SGAL_FALSE);
  
  /* Construct the prototype */
  static Coord_minkowski * prototype() { return new Coord_minkowski(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Coord_minkowski(); }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto * get_prototype();

  /*! Transform the input vertices */
  void execute(Field_info * field_info = NULL);
  
  /*! Set the coordinate-set node */
  void set_coord_array1(Coord_array * coord) { m_coord_array1 = coord; }

  /*! Obtain the coordinate-set node */
  Coord_array * get_coord_array1() const { return m_coord_array1; }

  /*! Set the coordinate-set node */
  void set_coord_array2(Coord_array * coord) { m_coord_array2 = coord; }

  /*! Obtain the coordinate-set node */
  Coord_array * get_coord_array2() const { return m_coord_array2; }

  /*! Obtain the changed coordinate-set node */
  Coord_array * get_coord_array_changed() const
  { return m_coord_array_changed; }
};

SGAL_END_NAMESPACE

#endif
