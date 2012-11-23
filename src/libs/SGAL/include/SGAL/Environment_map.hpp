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

#ifndef SGAL_ENVIRONMENT_MAP_HPP
#define SGAL_ENVIRONMENT_MAP_HPP

/*! \file
 * A node that represents a reflection map. At firts we only implement 
 * sperical reflection map.
 *
 * Inherits from Container
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Texture;
class Execution_coordinator;
class Container_proto;
class Element;

class SGAL_CLASSDEF Environment_map : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    TEXTURE,
    ALPHA,
    LAST
  };

  /*! Constructor */
  Environment_map(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Environment_map();

  /*! Construct the prototype */
  static Environment_map * prototype() { return new Environment_map(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Environment_map(); }

  void set_alpha(float alpha);
  float get_alpha() const;
  void set_quality(bool flag) { m_quality = flag; }
  bool is_quality() const { return m_quality; }
  void set_texture(Texture * texture);
  Texture * get_texture() { return m_texture; }

  // this method is used when an appearance is created by the environment
  // object it should not be called in any other case (FIX)
  Execution_coordinator * get_execution_coordinator() const
  {
    return m_execution_coordinator;
  }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto * get_prototype();

  virtual Boolean attach_context(Context * context ); 
  virtual Boolean detach_context(Context * context = 0); 

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! the node prototype */
  static Container_proto * s_prototype;

  /*! the texture object */
  Texture * m_texture;

  /*! the alpha value to use in blending in the second pass */
  float m_alpha;

  /*! if true, double pass is used to implement reflection */
  bool m_quality;

  /*! Default values */
  static float m_def_alpha;
  static bool m_def_quality;

  /*! Copy constructor */
  Environment_map(const Environment_map &) {}
};

SGAL_END_NAMESPACE

#endif
