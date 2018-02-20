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

/*! An implementation of the Box geometry.
 *
 *  A geometry of type box. The box is specified by its center and
 *  size.
 *
 *  Inherits from Geometry.
 */

#ifndef SGAL_BOX_HPP
#define SGAL_BOX_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Container_proto;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Box : public Geometry {
public:
  /*! Fields */
  enum {
    FIRST = Geometry::LAST - 1,
    SIZE,
    LAST
  };

  /*! Constructor */
  Box(Boolean proto = false);

  /*! Destructor */
  virtual ~Box();

  /* Construct the prototype. */
  static Box* prototype();

  /*! clones a new instance. */
  virtual Container* clone();

  /*! Initialize the box prototype. */
  virtual void init_prototype();

  /*! Delete the box prototype. */
  virtual void delete_prototype();

  /*! Obtain the box prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector3f* size_handle(const Field_info*) { return &m_size; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the box. */
  virtual void draw(Draw_action* action);

  /*! Draw the sphere in selection mode */
  virtual void isect(Isect_action* action);

  /*! Clean the bounding sphere of the box. */
  virtual void clean_bounding_sphere();

  /*! Determine whether the geometry has texture coordinates.
   */
  virtual Boolean has_tex_coord();

  /* Set the size of the box. */
  void set_size(const Vector3f& size);

  /* Obtain the size of the box. */
  void get_size(Vector3f& size) const;

  /* Obtain the size of the box. */
  Vector3f get_size() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The size of the box */
  Vector3f m_size;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Initialize the display list used to draw the box. */
  void init();

  /*! Draw the box. */
  void draw_box();

private:
  /*! the default size of the box. */
  static const Vector3f s_def_size;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Box* Box::prototype() { return new Box(true); }

//! \brief clones a new instance.
inline Container* Box::clone() { return new Box(); }

//! \brief sets the size of the box.
inline void Box::set_size(const Vector3f& size)
{
  m_size = size;
  m_dirty_bounding_sphere = true;
}

//! \brief gets the size of the box.
inline void Box::get_size(Vector3f& size) const { size = m_size; }

//! \brief gets the size of the box.
inline Vector3f Box::get_size() const { return m_size; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Box::get_tag() const { return s_tag; }

//! \brief determines whether the geometry has texture coordinate.
inline Boolean Box::has_tex_coord() { return do_generate_tex_coord(); }

SGAL_END_NAMESPACE

#endif
