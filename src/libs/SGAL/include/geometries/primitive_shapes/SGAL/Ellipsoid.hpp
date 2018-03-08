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

/*! \file
 * A geometry container that represents an ellipsoid
 */

#ifndef SGAL_ELLIPSOID_HPP
#define SGAL_ELLIPSOID_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Ellipsoid : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    WIDTH,
    HEIGHT,
    DEPTH,
    STACKS,
    SLICES,
    LAST
  };

  /*! Constructor */
  Ellipsoid(Boolean proto = false);

  /*! Destructor */
  virtual ~Ellipsoid();

  /* Construct the prototype */
  static Ellipsoid* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* width_handle(const Field_info*) { return &m_width; }
  Float* height_handle(const Field_info*) { return &m_height; }
  Float* depth_handle(const Field_info*) { return &m_depth; }
  Uint* slices_handle(const Field_info*) { return &m_slices; }
  Uint* stacks_handle(const Field_info*) { return &m_stacks; }
  //@}

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the ellipsoid coordinate array and coordinate indices. */
  virtual void clean_coords();

  /*! Clean the coordinate indices.
   */
  virtual void clean_coord_indices();

  /*! Set the width of the ellipsoid. */
  void set_width(Float width);

  /*! Obtain the width of the ellipsoid. */
  Float get_width() const;

  /*! Set the height of the ellipsoid. */
  void set_height(Float height);

  /*! Obtain the height of the ellipsoid. */
  Float get_height() const;

  /*! Set the depth of the ellipsoid. */
  void set_depth(Float depth);

  /*! Obtain the depth of the ellipsoid. */
  Float get_depth() const;

  /*! Set the number of slices (horizontal) longitudes. */
  void set_slices(Uint slices);

  /*! Obtain the number of slices (horizontal) longitudes. */
  Uint get_slices() const;

  /*! Set the number of stacks (vertical) latitudes. */
  void set_stacks(Uint stacks);

  /*! Obtain the number of stacks (vertical) latitudes */
  Uint get_stacks() const;

  /*! Process change of structure. */
  void structure_changed(const Field_info* field_info);

protected:
  /*! The width of the ellipsoid. */
  Float m_width;

  /*! The height of the ellipsoid. */
  Float m_height;

  /*! The depth of the ellipsoid. */
  Float m_depth;

  /*! The number of vertical slices (horizontal) longitudes */
  Uint m_slices;

  /*! The number of horizontal stacks (vertical) latitudes */
  Uint m_stacks;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! default values for Ellipsoid.  */
  static const Float s_def_width;
  static const Float s_def_height;
  static const Float s_def_depth;
  static const Uint s_def_slices;
  static const Uint s_def_stacks;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype. */
inline Ellipsoid* Ellipsoid::prototype() { return new Ellipsoid(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Ellipsoid::create() { return new Ellipsoid(); }

/*! \brief obtains the width of the ellipsoid. */
inline Float Ellipsoid::get_width() const { return m_width; }

/*! \brief obtains the height of the ellipsoid. */
inline Float Ellipsoid::get_height() const { return m_height; }

/*! \brief obtains the depth of the ellipsoid. */
inline Float Ellipsoid::get_depth() const { return m_depth; }

/*! \brief sets the number of slices (horizontal) longitudes. */
inline void Ellipsoid::set_slices(Uint slices) { m_slices = slices; }

/*! \brief obtains the number of slices (horizontal) longitudes. */
inline Uint Ellipsoid::get_slices() const { return m_slices; }

/*! \brief sets the number of stacks (vertical) latitudes. */
inline void Ellipsoid::set_stacks(Uint stacks) { m_stacks = stacks; }

/*! \brief obtains the number of stacks (vertical) latitudes. */
inline Uint Ellipsoid::get_stacks() const { return m_stacks; }

/*! \brief obtainss the tag (type) of the container. */
inline const std::string& Ellipsoid::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
