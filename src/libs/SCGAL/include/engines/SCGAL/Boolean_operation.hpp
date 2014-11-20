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

#ifndef SCGAL_BOOLEAN_OPERATION_HPP
#define SCGAL_BOOLEAN_OPERATION_HPP

/*! \file
 * An engine that computes a Boolean operation.
 *
 * Inherits from Node
 */

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Math_defs.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Container_proto;
class Element;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Boolean_operation : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    OPERAND1,
    OPERAND2,
    OPERATION,
    RESULT,
    TRIGGER,
    LAST
  };

  enum Operation {
    OP_NOP,
    OP_INTERSECTION
  };

  typedef boost::shared_ptr<Exact_polyhedron_geo>   Shared_exact_polyhedron_geo;
  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;

  /*! Constructor */
  Boolean_operation(Boolean proto = false);

  /*! Destructor */
  virtual ~Boolean_operation();

  /* Construct the prototype */
  static Boolean_operation* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Operation* operation_handle(const Field_info*) { return &m_operation; }
  Shared_mesh_set* operand1_handle(const Field_info*) { return &m_operand1; }
  Shared_mesh_set* operand2_handle(const Field_info*) { return &m_operand2; }
  Shared_exact_polyhedron_geo* result_handle(const Field_info*)
  { return &m_result; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Write this container.
   * \param formatter (in) the formatter to use; e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! . */
  void trigger_changed(const Field_info* field_info);

  /*! Execute the engine. */
  void execute();

  /*! Obtain the operation. */
  Operation get_operation() const;

  /*! Set the operation. */
  void set_operation(Operation operation);

  /*! Set the 1st operand.
   * \param operand the operand.
   */
  void set_operand1(Shared_mesh_set operand);

  /*! Obtain the 1st operand.
   * \result the operand.
   */
  Shared_mesh_set get_operand1() const;

  /*! Set the 2nd operand.
   * \param operand (in) the operand.
   */
  void set_operand2(Shared_mesh_set operand);

  /*! Obtain the 2nd operand.
   * \result the (in) operand.
   */
  Shared_mesh_set get_operand2() const;

  /*! Set the resulting geometry.
   * \param result (in) the resulting geomery.
   */
  void set_result(Shared_exact_polyhedron_geo result);

  /*! Obtain the resulting geometry.
   * \result (in) the resulting geomery.
   */
  Shared_exact_polyhedron_geo get_result() const;

protected:
  /*! 1st operand. */
  Shared_mesh_set m_operand1;

  /*! 2nd operand. */
  Shared_mesh_set m_operand2;

  /*! Result. */
  Shared_exact_polyhedron_geo m_result;

  /*! The operation. */
  Operation m_operation;

  /*! Obtain an exact polyhedron geometry from an operand geometry container.
   */
  Shared_exact_polyhedron_geo get_geometry(Shared_mesh_set operand) const;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! Trigger of the engine that makes the engine excute. */
  Boolean m_trigger;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The prototype of this node. */
  static Container_proto* s_prototype;

  /*! The array of operation names. */
  static const char* s_operation_names[];
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Boolean_operation* Boolean_operation::prototype()
{ return new Boolean_operation(true); }

//! \brief clones.
inline Container* Boolean_operation::clone() { return new Boolean_operation(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Boolean_operation::get_tag() const { return s_tag; }

//! \brief obtains the operation.
inline Boolean_operation::Operation Boolean_operation::get_operation() const
{ return m_operation; }

//! \brief sets the operation.
inline void Boolean_operation::set_operation(Operation operation)
{ m_operation = operation; }

//! \brief obtains the 1st operand.
inline Boolean_operation::Shared_mesh_set Boolean_operation::get_operand1()
  const
{ return m_operand1; }

//! \brief sets the 1st operand.
inline void Boolean_operation::set_operand1(Shared_mesh_set operand)
{ m_operand1 = operand; }

//! \brief obtains the 2nd operand.
inline Boolean_operation::Shared_mesh_set Boolean_operation::get_operand2()
  const
{ return m_operand2; }

//! \brief sets the 2nd operand.
inline void Boolean_operation::set_operand2(Shared_mesh_set operand)
{ m_operand2 = operand; }

//! \brief sets the resulting geometry.
inline void Boolean_operation::set_result(Shared_exact_polyhedron_geo result)
{ m_result = result; }

//! \brief obtains the resulting geometry.
inline Boolean_operation::Shared_exact_polyhedron_geo
Boolean_operation::get_result() const
{ return m_result; }

SGAL_END_NAMESPACE

#endif
