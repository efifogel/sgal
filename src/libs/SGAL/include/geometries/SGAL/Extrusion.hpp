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

/*! \file
 * A geometry container that represents an extrusion
 */

#ifndef SGAL_EXTRUSION_HPP
#define SGAL_EXTRUSION_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Extrusion : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    BEGIN_CAP,
    END_CAP,
    LOOP,
    CROSS_SECTION,
    ORIENTATION,
    SCALE,
    SPINE,
    LAST
  };

  /*! Constructor */
  Extrusion(Boolean proto = false);

  /*! Destructor */
  virtual ~Extrusion();

  /* Construct the prototype. */
  static Extrusion* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Set the flag that specifies whether the extruded shape is open at the
   * beginning.
   */
  void set_begin_cap(Boolean begin_cap);

  /*! Obtain the flag that specifies whether the extruded shape is open at the
   * beginning.
   */
  Boolean get_begin_cap() const;

  /*! Set the flag that specifies whether the extruded shape is open at the
   * end.
   */
  void set_end_cap(Boolean end_cap);

  /*! Obtain the flag that specifies whether the extruded shape is open at the
   * end.
   */
  Boolean get_end_cap() const;

  /*! Set the flag that specifies whether the spine is a closed loop. */
  void set_loop(Boolean loop);

  /*! Obtain the flag that specifies whether the spine is a closed loop. */
  Boolean get_loop() const;

  /*! Set the 2-D cross section of the final shape defined in the XZ plane. */
  void set_cross_section(SGAL::Array<Vector2f>& cross_section);

  /*! Obtain the 2-D cross section of the final shape defined in the XZ plane.
   */
  const SGAL::Array<Vector2f>& get_cross_section() const;
  
  /*! Set the orientation of the cross section. */
  void set_orientation(SGAL::Array<Rotation>& orientation);

  /*! Obtain the orientation of the cross section. */
  const SGAL::Array<Rotation>& get_orientation() const;

  /*! Set the scale of the cross section. */
  void set_scale(SGAL::Array<Vector2f>& scale);

  /*! Obtain the scale of the cross section. */
  const SGAL::Array<Vector2f>& get_scale() const;

  /*! Set the path that the cross section travels to create the shape. */
  void set_spine(SGAL::Array<Vector3f>& spine);
  
  /*! Obtains the path that the cross section travels to create the shape. */
  const SGAL::Array<Vector3f>& get_spine() const;

  /*! Obtains the path that the cross section travels to create the shape. */
  SGAL::Array<Vector3f>& get_spine();
  
  /*! Set the cross section radius. */
  void set_cross_section_radius(Float radius);

  /*! Obtain the cross section radius. */
  Float get_cross_section_radius() const;

  /*! Set the cross section slicess number. */
  void set_cross_section_slices(Uint slices);

  /*! Obtain the cross section slices number. */
  Uint get_cross_section_slices() const;
  
  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation. */
  virtual void clean();
  
protected:
  /*! Specifies whether the extruded shape is open at the beginning. */
  Boolean m_begin_cap;

  /*! Specifies whether the extruded shape is open at the end. */
  Boolean m_end_cap;

  /*! Indicates whether the spine is a loop. */
  Boolean m_loop;

  /*! A 2-D cross section of the final shape defined in the XZ plane. */
  SGAL::Array<Vector2f> m_cross_section;

  /*! The orientation of the cross section. */
  SGAL::Array<Rotation> m_orientation;

  /*! The scale of the cross section. */
  SGAL::Array<Vector2f> m_scale;

  /*! Defines the path that the cross section travels to create the shape. */
  SGAL::Array<Vector3f> m_spine;
  
  /*! obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The cross-section radius. */
  Float m_cross_section_radius;

  /*! The number of slices of the cross-section. */
  Uint m_cross_section_slices;
  
private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! default values for Extrusion.  */
  static const Boolean s_def_begin_cap;
  static const Boolean s_def_end_cap;
  static const Boolean s_def_loop;
  static const Float s_def_cross_section_radius;
  static const Uint s_def_cross_section_slices;
};

/* \brief constructs the prototype. */
inline Extrusion* Extrusion::prototype() { return new Extrusion(true); }

/*! \brief clones. */
inline Container* Extrusion::clone() { return new Extrusion(); }

/*! \brief sets the flag that specifies whether the extruded shape is open at
 * the beginning.
 */
inline void Extrusion::set_begin_cap(Boolean begin_cap)
{ m_begin_cap = begin_cap; }

/*! \brief obtains the flag that specifies whether the extruded shape is
 * open at the beginning.
 */
inline Boolean Extrusion::get_begin_cap() const { return m_begin_cap; }

/*! \brief sets the flag that specifies whether the extruded shape is open
 * at the end.
 */
inline void Extrusion::set_end_cap(Boolean end_cap) { m_end_cap = end_cap; }

/*! \brief obtains the flag that specifies whether the extruded shape is 
 * open at the end.
 */
inline Boolean Extrusion::get_end_cap() const { return m_end_cap; }

/*! \brief sets the flag that specifies whether the spine is a closed loop. */
inline void Extrusion::set_loop(Boolean loop) { m_loop = loop; }

/*! \brief obtains the flag that specifies whether the spine is a closed
 * loop.
 */
inline Boolean Extrusion::get_loop() const { return m_loop; }

/*! \brief sets the 2-D cross section of the final shape defined in the XZ
 * plane.
 */
inline void Extrusion::set_cross_section(SGAL::Array<Vector2f>& cross_section)
{
  m_cross_section = cross_section;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the 2-D cross section of the final shape defined in the
 * XZ plane.
 */
inline const SGAL::Array<Vector2f>& Extrusion::get_cross_section() const
{ return m_cross_section; }
  
/*! \brief sets the orientation of the cross section. */
inline void Extrusion::set_orientation(SGAL::Array<Rotation>& orientation)
{
  m_orientation = orientation;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the orientation of the cross section. */
inline const SGAL::Array<Rotation>& Extrusion::get_orientation() const
{ return m_orientation; }

/*! \brief sets the scale of the cross section. */
inline void Extrusion::set_scale(SGAL::Array<Vector2f>& scale)
{
  m_scale = scale;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the scale of the cross section. */
inline const SGAL::Array<Vector2f>& Extrusion::get_scale() const
{ return m_scale; }

/*! \brief sets the path that the cross section travels to create the shape. */
inline void Extrusion::set_spine(SGAL::Array<Vector3f>& spine)
{
  m_spine = spine;
  m_dirty_sphere_bound = true;
}
  
/*! \brief obtains the path that the cross section travels to create the
 * shape.
 */
inline const SGAL::Array<Vector3f>& Extrusion::get_spine() const
{ return m_spine; }

/*! \brief obtains the path that the cross section travels to create the
 * shape.
 */
inline SGAL::Array<Vector3f>& Extrusion::get_spine() { return m_spine; }
  
/*! \brief sets the cross section radius. */
inline void Extrusion::set_cross_section_radius(Float radius)
{
  m_cross_section_radius = radius;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the cross section radius. */
inline Float Extrusion::get_cross_section_radius() const
{ return m_cross_section_radius; }

/*! \brief sets the cross section slicess number. */
inline void Extrusion::set_cross_section_slices(Uint slices)
{ m_cross_section_slices = slices; }

/*! \brief obtains the cross section slices number. */
inline Uint Extrusion::get_cross_section_slices() const
{ return m_cross_section_slices; }
  
/*! \brief obtains the tag (type) of the container. */
inline const std::string& Extrusion::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
