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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_NAVIGATION_INFO_HPP
#define SGAL_NAVIGATION_INFO_HPP

#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Navigation_type.hpp"
#include "SGAL/Navigation_sensor.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Scene_graph;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Navigation_info : public Navigation_sensor {
public:
  enum {
    FIRST = Navigation_sensor::LAST - 1,
    TYPES,
    LAST
  };

  /*! Construct.
   */
  Navigation_info(Boolean proto = false);

  /*! Destruct.
   */
  virtual ~Navigation_info();

  /*! Construct the prototype.
   */
  static Navigation_info* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype.
   */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  String_array* types_handle(const Field_info*) { return &m_types; }
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Set the navigation paradigm list.
   */
  void set_types(const String_array& types);

  /*! Obtain the navigation paradigm list.
   */
  const String_array& get_types() const;

  /*! Process change of types.
   * \param[in] field_info the field information record of the types.
   */
  void types_changed(const Field_info* field_info = nullptr);

  /*! Set the type.
   */
  void set_type(Navigation_type type);

  /*! Set the 'any' Boolean flag that indicates whether the user can
   * change navigation types.
   */
  void set_any(Boolean any);

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! Determine whether a navigation type is supported.
   */
  bool is_supported(Navigation_type type);

  // Default values
  static const Navigation_type s_def_type;
  static const String_array s_def_types;

private:
  /*! Add a type from a string.
   * \param[in] the input type as a string.
   */
  void add_type(const std::string& type);

  /*! A list of navigation paradigms; one of "WALK", "EXAMINE", "FLY", or
   * "NONE".
   */
  String_array m_types;

  //! The navigation type.
  Navigation_type m_type;

  //! A list of supported types.
  static const std::list<Navigation_type> s_supported_types;

  //! The tag that identifies this container type.
  static std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Navigation_info* Navigation_info::prototype()
{ return new Navigation_info(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Navigation_info::create() { return new Navigation_info(); }

//! \brief sets the type.
inline void Navigation_info::set_type(Navigation_type type) { m_type = type; }

//! \brief obtains the navigation paradigm list.
inline const String_array& Navigation_info::get_types() const
{ return m_types; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Navigation_info::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
