// Copyright (c) 2012 Israel.
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

#ifndef SGAL_EGO_BRICK_HPP
#define SGAL_EGO_BRICK_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector2u.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Ego_brick : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    SIZE,
    PITCH,
    HEIGHT,
    KNOB_RADIUS,
    KNOB_HEIGHT,
    TOLERANCE,
    KNOB_SLICES,
    LAST
  };

  /*! Constructor */
  Ego_brick(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Ego_brick();

  /* Construct the prototype */
  static Ego_brick* prototype() { return new Ego_brick(SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Ego_brick(); }

  /*! Set the 2D size of the brick */
  void set_size(Vector2u& size) { m_size = size; }

  /*! Obtain the 2D size of the brick */
  const Vector2u& get_size() const { return m_size; }
  
  /*! Set the horizontal pitch of the brick */
  void set_pitch(Float pitch) { m_pitch = pitch; }

  /*! Obtain the horizontal pitch of the brick */
  Float get_pitch() const { return m_pitch; }

  /*! Set the height of the brick */
  void set_height(Float height) { m_height = height; }

  /*! Obtain the height of the brick */
  Float get_height() const { return m_height; }
  
  /*! Set the knob radius */
  void set_knob_radius(Float radius) { m_knob_radius = radius; }

  /*! Obtain the knob radius */
  Float get_knob_radius() const { return m_knob_radius; }

  /*! Set the knob height of the brick */
  void set_knob_height(Float knob_height) { m_knob_height = knob_height; }

  /*! Obtain the knob height of the brick */
  Float get_knob_height() const { return m_knob_height; }

  /*! Set the tolerance */
  void set_tolerance(Float tolerance) { m_tolerance = tolerance; }

  /*! Obtain the tolerance */
  Float get_tolerance() const { return m_tolerance; }

  /*! Set the knob slicess number */
  void set_knob_slices(Uint slices) { m_knob_slices = slices; }

  /*! Obtain the knob slices number */
  Uint get_knob_slices() const { return m_knob_slices; }
  
  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element* elem);

  /*! Clean the representation */
  virtual void clean();
  
protected:
  /*! The 2D size of the brick */
  Vector2u m_size;

  /*! The horizontal pitch, or distance between knobs */
  Float m_pitch;

  /*! The brick height */
  Float m_height;

  /*! The knob radius */
  Float m_knob_radius;

  /*! The knob height */
  Float m_knob_height;

  /*! The horizontal tolerance. This is half the gap between bricks in the
   * horizontal plane. The horizontal tolerance prevents friction between
   * bricks during building.
   */
  Float m_tolerance;

  /*! The number of slices of a knob */
  Uint m_knob_slices;

  /*! A 2D cross section of the knob */
  Vector2f m_knob_cross_section;
  
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! default values for Ego_brick.  */
  static const Vector2u s_def_size;
  static const Float s_def_pitch;
  static const Float s_def_height;
  static const Float s_def_knob_radius;
  static const Float s_def_knob_height;
  static const Float s_def_tolerance;
  static const Uint s_def_knob_slices;
};

SGAL_END_NAMESPACE

#endif
