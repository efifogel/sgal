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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COORD_MINKOWSKI_HPP
#define SGAL_COORD_MINKOWSKI_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;
class Container_proto;
class Coord_array;

class SGAL_SGAL_DECL Coord_minkowski : public Container {
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

  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;

  /*! Constructor. */
  Coord_minkowski(Boolean proto = false);
  
  /* Construct the prototype. */
  static Coord_minkowski* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this node.
   * \param elem lists of attribute names and values extracted from the input
   * file.
   */
  virtual void set_attributes(Element* elem);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Transform the input vertices. */
  void execute(Field_info* field_info = NULL);
  
  /*! Set the coordinate-set node. */
  void set_coord_array1(Shared_coord_array coord);

  /*! Obtain the coordinate-set node. */
  Shared_coord_array get_coord_array1() const;

  /*! Set the coordinate-set node. */
  void set_coord_array2(Shared_coord_array coord);

  /*! Obtain the coordinate-set node. */
  Shared_coord_array get_coord_array2() const;

  /*! Obtain the changed coordinate-set node. */
  Shared_coord_array get_coord_array_changed() const;

protected:
  /*! Determines whether the node is enabled. */
  Boolean m_enabled;

  /*! Indicates that the transformation has been applied. */
  Boolean m_changed;

  /*! Indicates that the operation should be executed. */
  Boolean m_execute;

  /*! The input vertices. */
  Shared_coord_array m_coord_array1;

  /*! The input vertices. */
  Shared_coord_array m_coord_array2;
  
  /*! The output vertices. */
  Shared_coord_array m_coord_array_changed;

  // default values.
  static Boolean s_def_enabled;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
};

/* \brief constructs the prototype. */
inline Coord_minkowski* Coord_minkowski::prototype()
{ return new Coord_minkowski(true); }

/*! \brief clones. */
inline Container* Coord_minkowski::clone() { return new Coord_minkowski(); }

/*! \brief sets the coordinate-set node. */
inline void Coord_minkowski::set_coord_array1(Shared_coord_array coord)
{ m_coord_array1 = coord; }

/*! \brief obtains the coordinate-set node. */
inline Coord_minkowski::Shared_coord_array Coord_minkowski::get_coord_array1()
  const
{ return m_coord_array1; }

/*! \brief sets the coordinate-set node. */
inline void Coord_minkowski::set_coord_array2(Shared_coord_array coord)
{ m_coord_array2 = coord; }

/*! \brief obtains the coordinate-set node. */
inline Coord_minkowski::Shared_coord_array Coord_minkowski::get_coord_array2()
  const
{ return m_coord_array2; }

/*! \brief obtains the changed coordinate-set node. */
inline Coord_minkowski::Shared_coord_array
Coord_minkowski::get_coord_array_changed() const
{ return m_coord_array_changed; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Coord_minkowski::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
