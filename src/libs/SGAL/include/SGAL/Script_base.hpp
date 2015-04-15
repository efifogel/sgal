// Copyright (c) 2014 Israel.
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

#ifndef SGAL_SCRIPT_BASE_HPP
#define SGAL_SCRIPT_BASE_HPP

#include <list>
#include <string>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Agent.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_info_template.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Tick_event;
class Field_info;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL std::allocator<char>;
template class SGAL_SGAL_DECL std::basic_string<char>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Script_base Script_base.hpp
 */
class SGAL_SGAL_DECL Script_base : public Node, public Agent {
public:
  enum {
    FIRST = Node::LAST-1,
    URL,
    DIRECT_OUTPUT,
    MUST_EVALUATE,
    LAST
  };

  enum Protocol {
    PROTOCOL_INVALID = 0,
    PROTOCOL_CUSTOM_ECMASCRIPT, // "javascript: ..."
    PROTOCOL_ECMASCRIPT,        // "http://bar.com/foo.js"
    PROTOCOL_JAVA_BYTECODE      // "http://bar.com/foo.class"
  };

  typedef boost::shared_ptr<Container>                  Shared_container;

  typedef boost::variant<Boolean,
                         Float,
                         Scene_time,
                         Int32,
                         Vector2f,
                         Vector3f,
                         Rotation,
                         std::string,
                         Shared_container,
                         Boolean_array,
                         Float_array,
                         Scene_time_array,
                         Int32_array,
                         Vector2f_array,
                         Vector3f_array,
                         Rotation_array,
                         String_array,
                         Shared_container_array> Variant_field;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Script_base(Boolean proto = false);

  /*! Destructor. */
  virtual ~Script_base();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  std::string* url_handle(const Field_info*) { return &m_url; }
  Boolean* direct_output_handle(const Field_info*) { return &m_direct_output; }
  Boolean* must_evaluate_handle(const Field_info*) { return &m_must_evaluate; }

  /*! Obtain a handle to a dynamically generated field.
   * \param field_info (in) the field information record.
   * \return a pointer to the field.
   */
  template <typename T>
  T* field_handle(Uint id)
  {
    std::list<Variant_field>::iterator it = m_fields.begin();
    std::advance(it, id - LAST);
    T& field = boost::get<T>(*it);
    return &field;
  }

  /*! Obtain a handle to a dynamically generated field.
   * \param id (in) the field information record.
   * \return a pointer to the field.
   */
  template <typename T>
  T* field_handle(const Field_info* field_info)
  {
    Uint id = field_info->get_id();
    return field_handle<T>(id);
  }
  //@}

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Dummy. */
  virtual Action::Trav_directive Draw(Draw_action* /* draw_action */)
  { return Action::TRAV_CONT; }

  /*! Handle tick events. */
  virtual void handle(Tick_event* event);

  /*! Print out the name of this agent (for debugging purposes). */
  virtual void identify();

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Determine whether it is possible to send events directly to any node to
   * which this script node has access, and may dynamically establish or break
   * routes.
   */
  bool is_direct_output() const;

  /*! Set the m_direct_output Boolean flag. */
  void set_direct_output();

  /*! Set the m_must_evaluate Boolean flag. */
  void set_must_evaluate();

  /*! Execute the script function according to the event. */
  virtual void execute(const Field_info* field_info) = 0;

protected:
  /*! The script. */
  std::string m_url;

  /*! Specifies how to evaluate the script.
   * If TRUE, the script may also send events directly to any node to which it
   * has access, and may dynamically establish or break routes. If
   * m_direct_output is FALSE (the default), the script may only affect the rest
   * of the world via events sent through its eventOuts.
   */
  Boolean m_direct_output;

  /*! If the mustEvaluate field is FALSE, the browser may delay sending input
   * events to the script until its outputs are needed by the browser. If the
   * mustEvaluate field is TRUE, the browser shall send input events to the
   * script as soon as possible, regardless of whether the outputs are
   * needed. The mustEvaluate field shall be set to TRUE only if the Script node
   * has effects that are not known to the browser (such as sending information
   * across the network). Otherwise, poor performance may result.
   */
  Boolean m_must_evaluate;

  /*! Timestamp */
  Scene_time m_time;

  /*! Additional fields. */
  std::list<Variant_field> m_fields;

  /*! Add a field to the prototype---a generic implementation.
   * \param id (in)
   * \param name (in)
   * \param rule (in)
   * \param initial_value (in)
   * \param exec_func (in)
   * \param prototype (in)
   */
  template <Field_info::Field_type type, typename ValueType_>
  void add_fi(Uint id, const std::string& name, Field_info::Field_rule rule,
              const ValueType_& initial_value,
              Execution_function exec_func, Container_proto* prototype)
  {
    typedef ValueType_                                  Value_type;
    typedef typename Handle_function<Value_type>::type  Handle_function;
    typedef Field_info_template<Value_type, type>       Field_info_type;

    auto field_func =
      static_cast<Handle_function>(&Script_base::field_handle<Value_type>);
    auto field_info =
      new Field_info_type(id, name, rule, field_func, initial_value, exec_func);
    prototype->add_field_info(field_info);
  }

 private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The script protocol. */
  Protocol m_protocol;

  /*! Default Values */
  static const Boolean s_def_direct_output;
  static const Boolean s_def_must_evaluate;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief sets the URL.
inline void Script_base::set_url(const std::string& url) { m_url = url; }

//! \brief obtains the URL.
inline const std::string Script_base::get_url() const { return m_url; }

//! \brief determines whether it is possible to send events directly.
inline bool Script_base::is_direct_output() const { return m_direct_output; }

//! \brief sets the m_direct_output Boolean flag.
inline void Script_base::set_direct_output() { m_direct_output = true; }

//! \brief sets the m_must_evaluate Boolean flag.
inline void Script_base::set_must_evaluate() { m_must_evaluate = true; }

SGAL_END_NAMESPACE

#endif
