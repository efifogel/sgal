// Copyright (c) 2011 Israel.
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
// $Revision: 10982 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_HALFTONE_HPP
#define SGAL_HALFTONE_HPP

/*! \file
 * A class that implements a halftone.
 *
 * Inherits from Container
 */

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Appearance;
class Element;
class Container_proto;

class SGAL_CLASSDEF Halftone : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    FILE_NAME,
    LAST
  };

  /*! Constructor */
  Halftone(Boolean proto = false);

  /*! Destructor */
  virtual ~Halftone();

  /*! Construct the prototype */
  static Halftone* prototype() { return new Halftone(true); }

  /*! Clone */
  virtual Container* clone() { return new Halftone(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype(); 
  
  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Obtain the halftone image */
  Image* get_image() const { return m_image; }

  /*! Set the halftone image */
  void set_image(Image* image) { m_image = image; }
  
  /*! Obtain the halftone format */
  Image::Format get_format() { return m_image->get_format(); }

  /*! Obtain the halftone pixel-data */
  Ubyte* get_pattern();
  
  /*! Draw the halftone */
  virtual void draw(Context* context);

  /*! Determine whether the object is dirty, and thus needs cleaning */
  Boolean is_dirty() const { return m_dirty; }

  /*! Clean the object using the new decoded data */
  virtual void clean();

  virtual Boolean attach_context(Context * context);
  virtual Boolean detach_context(Context * context = 0);

protected:
  /*! true after a call to clean. */
  Boolean m_dirty;

  /*! The halftone pixels */
  Image* m_image;
  
  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! Halftone stipple pattern for backfacing elements */
  Ubyte* m_pattern;
    
  /*! true after a call to clean. */
  Boolean m_dirty_pattern;

  /*! Halftone default stipple pattern for backfacing elements */
  static Ubyte s_def_pattern[];

  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

SGAL_END_NAMESPACE

#endif
