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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

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

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Halftone : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    FILE_NAME,
    LAST
  };

  /*! Construct. */
  Halftone(Boolean proto = false);

  /*! Destruct. */
  virtual ~Halftone();

  /*! Construct the prototype. */
  static Halftone* prototype();

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

  /*! Set the attributes of this node.
   * \param[in] elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the halftone image.
   */
  Image* get_image() const;

  /*! Set the halftone image.
   */
  void set_image(Image* image);

  /*! Obtain the halftone format.
   */
  Image::Format get_format();

  /*! Set the halftone pixel-data.
   */
  void set_pattern(Ubyte* pattern);

  /*! Obtain the halftone pixel-data.
   */
  Ubyte* get_pattern();

  /*! Draw the halftone.
   */
  virtual void draw(Context* context);

  /*! Determine whether the object is dirty, and thus needs cleaning.
   */
  Boolean is_dirty() const { return m_dirty; }

  /*! Clean the object using the new decoded data.
   */
  virtual void clean();

  virtual Boolean attach_context(Context* context);
  virtual Boolean detach_context(Context* context = 0);

protected:
  //! true after a call to clean.
  Boolean m_dirty;

  //! The halftone pixels.
  Image* m_image;

  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! Clean the pattern.
   */
  void clean_pattern();

private:
  //! Halftone stipple pattern for backfacing elements.
  Ubyte* m_pattern;

  //! true after a call to clean.
  Boolean m_dirty_pattern;

  //! Halftone default stipple pattern for backfacing elements.
  static Ubyte s_def_pattern[];

  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Halftone* Halftone::prototype() { return new Halftone(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Halftone::create() { return new Halftone(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Halftone::get_tag() const { return s_tag; }

//! \brief obtains the halftone image.
inline Image* Halftone::get_image() const { return m_image; }

//! \brief obtain the halftone format. */
inline Image::Format Halftone::get_format() { return m_image->get_format(); }

SGAL_END_NAMESPACE

#endif
