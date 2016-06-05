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

#ifndef SGAL_NAVIGATION_INFO_HPP
#define SGAL_NAVIGATION_INFO_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Navigation_info_types.hpp"
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

  /*! Constructor. */
  Navigation_info(Boolean proto = false);

  /*! Destructor. */
  virtual ~Navigation_info();

  /*! Construct the prototype. */
  static Navigation_info* prototype();

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
  String_array* types_handle(const Field_info*) { return &m_types; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Set the navigation paradigm list.
   */
  void set_types(const String_array& justify);

  /*! Obtain the navigation paradigm list.
   */
  const String_array& get_types() const;

  /*! Set the type */
  void set_type(Navigation_info_type type);

  /*! Set the 'any' Boolean flag that indicates whether the user can
   * change navigation types.
   */
  void set_any(Boolean any);

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

  // Default values
  static const String_array s_def_types;

private:
  /*! The navigation type strings. */
  static const char* s_type_strings[];

  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether to allow the user to change navigation type. */
  bool m_any;

  /*! A list of navigation paradigms; one of "WALK", "EXAMINE", "FLY", or
   * "NONE".
   */
  String_array m_types;

  /*! The navigation type. */
  Navigation_info_type m_type;

  /*! Parse the type string-attribute. */
  void set_type(const std::string& type);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Navigation_info* Navigation_info::prototype()
{ return new Navigation_info(true); }

//! \brief clones.
inline Container* Navigation_info::clone() { return new Navigation_info(); }

//! \brief sets the navigation paradigm list.
inline void Navigation_info::set_types(const String_array& types)
{ m_types = types; }

//! \brief obtains the navigation paradigm list.
inline const String_array& Navigation_info::get_types() const
{ return m_types; }

//! \brief sets the type.
inline void Navigation_info::set_type(Navigation_info_type type)
{ m_type = type; }

/*! \brief sets the 'any' Boolean flag, which indicates whether the user can
 * change navigation types.
 */
inline void Navigation_info::set_any(Boolean any) { m_any = any; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Navigation_info::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
