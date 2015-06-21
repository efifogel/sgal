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

#ifndef SGAL_TEXT_HPP
#define SGAL_TEXT_HPP

/*! \file
 * A node in the scene graph that renders text.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Font_style;
class Container_proto;
class Element;
class Cull_context;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Text : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    STRING,
    LAST
  };

  typedef boost::shared_ptr<Font_style>         Shared_font_style;

  /*! Constructor */
  Text(Boolean proto = false);

  /*! Destructor */
  virtual ~Text();

  /*! Construct the prototype. */
  static Text* prototype();

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
  std::string* string_handle(const Field_info*) { return &m_string; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  virtual void cull(Cull_context& /* cull_context */) {}

  virtual void draw(Draw_action* action);

  virtual void isect(Isect_action* /* isect_action */) {}

  /*! Clean the sphere bound */
  virtual void clean_bounding_sphere();

  /*! Set the font style */
  void set_font_style(Shared_font_style font_style);

  /*! Obtain the font style */
  Shared_font_style get_font_style() const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The text to render */
  std::string m_string;

  /*! The font style */
  Shared_font_style m_font_style;

  /*! Indicates whether a default fontStyle was constructed */
  Shared_font_style m_default_font_style;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief construct the prototype.
inline Text* Text::prototype() { return new Text(true); }

//! \brief clones.
inline Container* Text::clone() { return new Text(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Text::get_tag() const { return s_tag; }

//! \brief obtains the font style.
inline Text::Shared_font_style Text::get_font_style() const
{ return m_font_style; }

SGAL_END_NAMESPACE

#endif
