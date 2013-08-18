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
  Incrementor(Boolean proto = false);

  /*! Destructor */
  virtual ~Incrementor() {}

  /*! Construct the prototype */
  static Incrementor* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Int* min_value_handle(Field_info*) { return &m_min_value; }
  Int* max_value_handle(Field_info*) { return &m_max_value; }
  Boolean* trigger_handle(Field_info*) { return &m_trigger; }
  Int* value_handle(Field_info*) { return &m_value; }
  //@}

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Increment the counter. */
  virtual void execute(Field_info* field_info);

  /*! Obtain the minimum value. */
  Int get_min_value() const;

  /*! Set the minimum value. */
  void set_min_value(Int min_value);

  /*! Obtain the maximum value. */
  Int get_max_value() const;

  /*! Set the maximum value. */
  void set_max_value(Int max_value);

  /*! Obtain the range. */
  std::pair<Int, Int> get_range() const;

  /*! Obtain the size. */
  Uint get_size() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The minimum value */
  Int m_min_value;

  /*! The maximum value */
  Int m_max_value;

  /*! Trigger the engine to excute. */
  Boolean m_trigger;

  /*! The counter value */
  Int m_value;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  // Default values:
  static const Int s_def_min_value;
  static const Int s_def_max_value;
};

/*! \brief constructs the prototype */
inline Incrementor* Incrementor::prototype() { return new Incrementor(true); }

/*! \brief clones */
inline Container* Incrementor::clone() { return new Incrementor(); }

/*! \brief Obtain the minimum value. */
inline Int Incrementor::get_min_value() const { return m_min_value; }

/*! \brief Set the minimum value. */
inline void Incrementor::set_min_value(Int min_value)
{ m_min_value = min_value; }

/*! \brief Obtain the maximum value. */
inline Int Incrementor::get_max_value() const { return m_max_value; }

/*! \brief Set the maximum value. */
inline void Incrementor::set_max_value(Int max_value)
{ m_max_value = max_value; }

/*! \brief Obtain the range. */
inline std::pair<Int, Int> Incrementor::get_range() const
{ return std::pair<Int, Int>(m_min_value, m_max_value); }

/*! \brief Obtain the size. */
inline Uint Incrementor::get_size() const
{ return m_max_value - m_min_value + 1; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Incrementor::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif

