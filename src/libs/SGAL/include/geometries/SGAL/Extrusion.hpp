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
  Extrusion(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Extrusion();

  /* Construct the prototype */
  static Extrusion * prototype() { return new Extrusion(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Extrusion(); }

  /*! Set the flag that Specifies whether the extruded shape is open at the
   * beginning
   */
  void set_begin_cap(Boolean begin_cap) { m_begin_cap = begin_cap; }

  /*! Obtain the flag that Specifies whether the extruded shape is open at the
   * beginning
   */
  Boolean get_begin_cap() const { return m_begin_cap; }

  /*! Set the flag that Specifies whether the extruded shape is open at the
   * end
   */
  void set_end_cap(Boolean end_cap) { m_end_cap = end_cap; }

  /*! Obtain the flag that Specifies whether the extruded shape is open at the
   * end
   */
  Boolean get_end_cap() const { return m_end_cap; }

  /*! Set the flag that Specifies whether the spine is a closed loop */
  void set_loop(Boolean loop) { m_loop = loop; }

  /*! Obtain the flag that Specifies whether the spine is a closed loop */
  Boolean get_loop() const { return m_loop; }

  /*! Set the 2-D cross section of the final shape defined in the XZ plane */
  void set_cross_section(SGAL::Array<Vector2f> & cross_section)
  { m_cross_section = cross_section; }

  /*! Obtain the 2-D cross section of the final shape defined in the XZ plane
   */
  const SGAL::Array<Vector2f> & get_cross_section() const
  { return m_cross_section; }
  
  /*! Set the orientation of the cross section */
  void set_orientation(SGAL::Array<Rotation> & orientation)
  { m_orientation = orientation; }

  /*! Obtain the orientation of the cross section */
  const SGAL::Array<Rotation> & get_orientation() const
  { return m_orientation; }

  /*! Set the scale of the cross section */
  void set_scale(SGAL::Array<Vector2f> & scale) { m_scale = scale; }

  /*! Obtain the scale of the cross section */
  const SGAL::Array<Vector2f> & get_scale() const {return m_scale; }

  /*! Set the path that the cross section travels to create the shape */
  void set_spine(SGAL::Array<Vector3f> & spine) { m_spine = spine; }
  
  /*! Obtains the path that the cross section travels to create the shape */
  const SGAL::Array<Vector3f> & get_spine() const { return m_spine; }

  /*! Obtains the path that the cross section travels to create the shape */
  SGAL::Array<Vector3f> & get_spine() { return m_spine; }
  
  /*! Set the cross section radius */
  void set_cross_section_radius(Float radius)
  { m_cross_section_radius = radius; }

  /*! Obtain the cross section radius */
  Float get_cross_section_radius() const { return m_cross_section_radius; }

  /*! Set the cross section slicess number */
  void set_cross_section_slices(Uint slices)
  { m_cross_section_slices = slices; }

  /*! Obtain the cross section slices number */
  Uint get_cross_section_slices() const { return m_cross_section_slices; }
  
  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype */
  virtual Container_proto * get_prototype();

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation */
  virtual void clean();
  
protected:
  /*! Specifies whether the extruded shape is open at the beginning */
  Boolean m_begin_cap;

  /*! Specifies whether the extruded shape is open at the end */
  Boolean m_end_cap;

  /*! Indicates whether the spine is a loop */
  Boolean m_loop;

  /*! A 2-D cross section of the final shape defined in the XZ plane */
  SGAL::Array<Vector2f> m_cross_section;

  /*! The orientation of the cross section */
  SGAL::Array<Rotation> m_orientation;

  /*! The scale of the cross section */
  SGAL::Array<Vector2f> m_scale;

  /*! Defines the path that the cross section travels to create the shape */
  SGAL::Array<Vector3f> m_spine;
  
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

  /*! The cross-section radius */
  Float m_cross_section_radius;

  /*! The number of slices of the cross-section */
  Uint m_cross_section_slices;
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto * s_prototype;

  /*! default values for Extrusion.  */
  static const Boolean s_def_begin_cap;
  static const Boolean s_def_end_cap;
  static const Boolean s_def_loop;
  static const Float s_def_cross_section_radius;
  static const Uint s_def_cross_section_slices;
};

SGAL_END_NAMESPACE

#endif
