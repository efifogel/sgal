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
// $Id: Incrementor.hpp 6147 2008-04-02 15:03:46Z efif $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 *
 * Increments a counter within a range
 */

#ifndef SGAL_INCREMENTOR_HPP
#define SGAL_INCREMENTOR_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Field_info;

class Incrementor : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    MIN_VALUE,
    MAX_VALUE,
    TRIGGER,
    VALUE,
    LAST
  };

  /*! Constructor */
  Incrementor(Boolean proto = SGAL_FALSE) :
    Node(proto),
    m_min_value(s_def_min_value),
    m_max_value(s_def_max_value),
    m_trigger(SGAL_FALSE),
    m_value(s_def_min_value)
  {}

  /*! Destructor */
  virtual ~Incrementor() {}

  /*! Construct the prototype */
  static Incrementor * prototype() { return new Incrementor(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Incrementor(); }

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto * get_prototype();
  
  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! The interpolation execution function */
  virtual void execute(Field_info * field_info);

  /*! Obtain the minimum value */
  Int get_min_value() const { return m_min_value; }

  /*! Set the minimum value */
  void set_min_value(Int min_value) { m_min_value = min_value; }

  /*! Obtain the maximum value */
  Int get_max_value() const { return m_max_value; }

  /*! Set the maximum value */
  void set_max_value(Int max_value) { m_max_value = max_value; }

  /*! Obtain the range */
  std::pair<Int,Int> get_range() const
  { return std::pair<Int,Int>(m_min_value, m_max_value); }

  /*! Obtain the size */
  Uint get_size() const { return m_max_value - m_min_value + 1; }
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

  /*! The minimum value */
  Int m_min_value;

  /*! The maximum value */
  Int m_max_value;
  
  /*! Trigger of the engine that makes the engine excute */
  Boolean m_trigger;

  /*! The counter value */
  Int m_value;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  // Default values:
  static const Int s_def_min_value;
  static const Int s_def_max_value;
};

SGAL_END_NAMESPACE

#endif

