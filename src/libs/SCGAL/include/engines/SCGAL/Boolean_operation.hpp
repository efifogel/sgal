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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_BOOLEAN_OPERATION_HPP
#define SCGAL_BOOLEAN_OPERATION_HPP

/*! \file
 * An engine that computes a Boolean operation.
 *
 * Inherits from Node
 */

#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/corefinement_operations.h>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Epec_polyhedron.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Container_proto;
class Element;
class Formatter;
class Indexed_face_set;

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
    OP_UNION = 0,
    OP_INTERSECTION,
    OP_P_MINUS_Q,
    OP_Q_MINUS_P
  };

  typedef CGAL::Polyhedron_corefinement<Epec_polyhedron, Epec_kernel> Bso;

  typedef boost::shared_ptr<Indexed_face_set>       Shared_indexed_face_set;

  /*! Construct. */
  Boolean_operation(Boolean proto = false);

  /*! Destruct. */
  virtual ~Boolean_operation();

  /* Construct the prototype. */
  static Boolean_operation* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

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
  Shared_indexed_face_set* operand1_handle(const Field_info*)
  { return &m_operand1; }
  Shared_indexed_face_set* operand2_handle(const Field_info*)
  { return &m_operand2; }
  std::vector<Shared_indexed_face_set>* result_handle(const Field_info*)
  { return &m_result; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! triggers the execution of the engine as a response to change in one
   * of the input fields.
   */
  void trigger_changed(const Field_info* field_info);

  /*! Execute the engine. */
  void execute();

  /*! Obtain the operation.
   */
  Operation get_operation() const;

  /*! Set the operation.
   */
  void set_operation(Operation operation);

  /*! Obtain the operation tag.
   */
  Bso::Boolean_operation_tag get_operation_tag() const;

  /*! Set the 1st operand.
   * \param[in] operand the operand.
   */
  void set_operand1(Shared_indexed_face_set operand);

  /*! Obtain the 1st operand.
   * \return the operand.
   */
  Shared_indexed_face_set get_operand1() const;

  /*! Set the 2nd operand.
   * \param[in] operand the operand.
   */
  void set_operand2(Shared_indexed_face_set operand);

  /*! Obtain the 2nd operand.
   * \return the operand.
   */
  Shared_indexed_face_set get_operand2() const;

  /*! Set the resulting geometry.
   * \param[in] result the resulting geomery.
   */
  void set_result(const std::vector<Shared_indexed_face_set>& result);

  /*! Obtain the resulting geometry.
   * \param[in] result the resulting geomery.
   */
  const std::vector<Shared_indexed_face_set>& get_result() const;

protected:
  //! 1st operand.
  Shared_indexed_face_set m_operand1;

  //! 2nd operand.
  Shared_indexed_face_set m_operand2;

  //! Result.
  std::vector<Shared_indexed_face_set> m_result;

  //! The operation.
  Operation m_operation;

  //! Obtain the tag (type) of the container.
  virtual const std::string& get_tag() const;

private:
  //! Trigger of the engine that makes the engine excute.
  Boolean m_trigger;

  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The prototype of this node.
  static Container_proto* s_prototype;

  //! The array of operation names.
  static const char* s_operation_names[];

  //! The array of operation tags.
  static const Bso::Boolean_operation_tag s_operation_tags[];
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Boolean_operation* Boolean_operation::prototype()
{ return new Boolean_operation(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Boolean_operation::create()
{ return new Boolean_operation(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Boolean_operation::get_tag() const { return s_tag; }

//! \brief obtains the operation.
inline Boolean_operation::Operation Boolean_operation::get_operation() const
{ return m_operation; }

//! \brief sets the operation.
inline void Boolean_operation::set_operation(Operation operation)
{ m_operation = operation; }

//! \brief obtains the 1st operand.
inline Boolean_operation::Shared_indexed_face_set
Boolean_operation::get_operand1() const
{ return m_operand1; }

//! \brief sets the 1st operand.
inline void Boolean_operation::set_operand1(Shared_indexed_face_set operand)
{ m_operand1 = operand; }

//! \brief obtains the 2nd operand.
inline Boolean_operation::Shared_indexed_face_set
Boolean_operation::get_operand2() const
{ return m_operand2; }

//! \brief sets the 2nd operand.
inline void Boolean_operation::set_operand2(Shared_indexed_face_set operand)
{ m_operand2 = operand; }

//! \brief sets the resulting geometry.
inline void Boolean_operation::
set_result(const std::vector<Shared_indexed_face_set>& result)
{ m_result = result; }

//! \brief obtains the resulting geometry.
inline const std::vector<Boolean_operation::Shared_indexed_face_set>&
Boolean_operation::get_result() const
{ return m_result; }

//! \brief obtains the operation tag.
inline Boolean_operation::Bso::Boolean_operation_tag
Boolean_operation::get_operation_tag() const
{ return s_operation_tags[m_operation]; }

SGAL_END_NAMESPACE

#endif
