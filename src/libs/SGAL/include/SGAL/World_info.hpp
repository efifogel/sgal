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

#ifndef SGAL_WORLD_INFO_HPP
#define SGAL_WORLD_INFO_HPP

#include <string>
#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

class World_info : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    INFO,
    TITLE
  };

  /*! Constructor */
  World_info(Boolean proto = false);

  /*! Destructor */
  virtual ~World_info() {};

  /*! Construct the prototype. */
  static World_info* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

protected:
  /*! obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Information. */
  std::list<std::string> m_info;

  /*! The scene title */
  std::string m_title;
};

/*! \brief constructs the prototype. */
inline World_info* World_info::prototype() { return new World_info(true); }

/*! \brief clones. */
inline Container* World_info::clone() { return new World_info(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& World_info::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif

