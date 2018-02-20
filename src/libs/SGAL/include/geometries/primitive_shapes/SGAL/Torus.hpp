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
 * A geometry container that represents a torus
 */

#ifndef SGAL_TORUS_HPP
#define SGAL_TORUS_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/Extrusion.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Torus : public Extrusion {
public:
  enum {
    FIRST = Extrusion::LAST - 1,
    SPINE_RADIUS,
    CROSS_SECTION_RADIUS,
    STACKS,
    SLICES,
    LAST
  };

  /*! Constructor */
  Torus(Boolean proto = false);

  /*! Destructor */
  virtual ~Torus();

  /* Construct the prototype. */
  static Torus* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* spine_radius_handle(const Field_info*) { return &m_spine_radius; }
  Float* cross_section_radius_handle(const Field_info*)
  { return &m_cross_section_radius; }
  Uint* stacks_handle(const Field_info*) { return &m_stacks; }
  Uint* slices_handle(const Field_info*) { return &m_slices; }
  //@}

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the torus coordinate array. */
  virtual void clean_coords();

  /*! Set the spine radius of the ellipsoid. */
  void set_spine_radius(Float spine_radius);

  /*! Obtain the spine radius of the ellipsoid. */
  Float get_spine_radius() const;

  /*! Set the number of slices (horizontal) longitudes. */
  void set_slices(Uint slices);

  /*! Obtain the number of slices (horizontal) longitudes. */
  Uint get_slices() const;

  /*! Set the number of stacks (vertical) latitudes. */
  void set_stacks(Uint stacks);

  /*! Obtain the number of stacks (vertical) latitudes. */
  Uint get_stacks() const;

protected:
  /*! The spine_height of the ellipsoid. */
  Float m_spine_radius;

  /*! The number of vertical slices (horizontal) longitudes. */
  Uint m_slices;

  /*! The number of horizontal stacks (vertical) latitudes. */
  Uint m_stacks;

  /*! obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! Default values for Torus. */
  static const Float s_def_cross_section_radius; // Override Extrusion def.
  static const Float s_def_spine_radius;
  static const Uint s_def_slices;
  static const Uint s_def_stacks;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/* \brief constructs the prototype. */
inline Torus* Torus::prototype() { return new Torus(true); }

/*! \brief clones. */
inline Container* Torus::clone() { return new Torus(); }

/*! \brief obtains the spine radius of the ellipsoid. */
inline Float Torus::get_spine_radius() const { return m_spine_radius; }

/*! \brief obtains the number of slices (horizontal) longitudes. */
inline Uint Torus::get_slices() const { return m_slices; }

/*! \brief obtains the number of stacks (vertical) latitudes. */
inline Uint Torus::get_stacks() const { return m_stacks; }

  /*! obtains the tag (type) of the container. */
inline const std::string& Torus::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
