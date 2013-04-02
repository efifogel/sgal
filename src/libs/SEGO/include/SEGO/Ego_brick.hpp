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
 * A geometry container that represents an Ego brick.
 */

#ifndef SGAL_EGO_BRICK_HPP
#define SGAL_EGO_BRICK_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;
class Coord_array;
class Normal_array;
class Tex_coord_array;
class Field_info;

class SGAL_CLASSDEF Ego_brick : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    NUMBER_OF_KNOBS1,
    NUMBER_OF_KNOBS2,
    PITCH,
    HEIGHT,
    KNOB_RADIUS,
    KNOB_HEIGHT,
    TOLERANCE,
    KNOB_SLICES,
    LAST
  };

  /*! Constructor */
  Ego_brick(Boolean proto = false);

  /*! Destructor */
  virtual ~Ego_brick();

  /* Construct the prototype */
  static Ego_brick* prototype() { return new Ego_brick(true); }

  /*! Clone. */
  virtual Container* clone() { return new Ego_brick(); }

  /*! Set the number_of_knobs along the 1st dimension. */
  void set_number_of_knobs1(Uint number) { m_number_of_knobs1 = number; }

  /*! Obtain the number_of_knobs along the 1st dimension. */
  Uint get_number_of_knobs1() const { return m_number_of_knobs1; }
  
  /*! Set the number_of_knobs along the 2nd dimension. */
  void set_number_of_knobs2(Uint number) { m_number_of_knobs2 = number; }

  /*! Obtain the number_of_knobs along the 2nd dimension. */
  Uint get_number_of_knobs2() const { return m_number_of_knobs2; }
  
  /*! Set the horizontal pitch of the brick. */
  void set_pitch(Float pitch) { m_pitch = pitch; }

  /*! Obtain the horizontal pitch of the brick. */
  Float get_pitch() const { return m_pitch; }

  /*! Set the height of the brick. */
  void set_height(Float height) { m_height = height; }

  /*! Obtain the height of the brick. */
  Float get_height() const { return m_height; }
  
  /*! Set the knob radius. */
  void set_knob_radius(Float radius) { m_knob_radius = radius; }

  /*! Obtain the knob radius. */
  Float get_knob_radius() const { return m_knob_radius; }

  /*! Set the knob height of the brick. */
  void set_knob_height(Float knob_height) { m_knob_height = knob_height; }

  /*! Obtain the knob height of the brick. */
  Float get_knob_height() const { return m_knob_height; }

  /*! Set the tolerance. */
  void set_tolerance(Float tolerance) { m_tolerance = tolerance; }

  /*! Obtain the tolerance. */
  Float get_tolerance() const { return m_tolerance; }

  /*! Set the knob slicess number. */
  void set_knob_slices(Uint slices) { m_knob_slices = slices; }

  /*! Obtain the knob slices number. */
  Uint get_knob_slices() const { return m_knob_slices; }

  /*! Set the flag that indicates whether the knos are visible. */
  void set_knobs_visible(Boolean flag) { m_knobs_visible = flag; }
  
  /* Obtain the flag that indicates whether the knos are visible. */
  Boolean are_knobs_visible() const { return m_knobs_visible; }
    
  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  /*! Clean the representation. */
  virtual void clean();

  /*! Set the center of the Ego brick. */
  void set_center(Vector3f& center);

  /*! Obtain the center of the Ego brick. */
  Vector3f& get_center();
  
  /*! Set the coordinates. */
  void set_coord_array(Shared_coord_array coord_array);

  /*! Set the normals. */
  void set_normal_array(Shared_normal_array normal_array);
  
  /*! Set the texture coordinates. */
  void set_tex_coord_array(Shared_tex_coord_array tex_coord_array);

  /*! Set the texture indices. */
  void set_coord_indices(const Array<Uint>& indices);
  
  /*! Obtain the coordinate array. */
  Shared_coord_array get_coord_array();

  /*! Obtain the normal array. */
  Shared_normal_array get_normal_array();

  /*! Obtain the texture-coordinate array. */
  Shared_tex_coord_array get_tex_coord_array();

  /*! Obtain the coord-index array. */
  Array<Uint>& get_coord_indices();

protected:
  friend class Ego;
  
  /*! The number of knobs along the 1st dimension. */
  Uint m_number_of_knobs1;

  /*! The number of knobs along the 2nd dimension. */
  Uint m_number_of_knobs2;

  /*! The horizontal pitch, or distance between knobs. */
  Float m_pitch;

  /*! The brick height. */
  Float m_height;

  /*! The knob radius. */
  Float m_knob_radius;

  /*! The knob height. */
  Float m_knob_height;

  /*! The horizontal tolerance. This is half the gap between bricks in the
   * horizontal plane. The horizontal tolerance prevents friction between
   * bricks during building.
   */
  Float m_tolerance;

  /*! The number of slices of a knob. */
  Uint m_knob_slices;

  /*! Are the knobs visible. */
  Boolean m_knobs_visible;

  /*! Indicates whether the center is dirty and thus needs cleaning. */
  Boolean m_dirty_center;

  /*! Indicates whether the coordinate array is dirty and thus needs cleaning.
   */
  Boolean m_dirty_coords;
  
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Generate the knob_cross_section. */
  void clean_knob_cross_section();

  /*! Clean the coordinates. */
  void clean_coords();

  /*! Clean the normals. */
  void clean_normals();
  
  /*! Clean the texture coordinates. */
  void clean_tex_coords();

  /*! Clean the texture indices. */
  void clean_indices();

  /*! Clean the center of the Ego brick. */
  void clean_center();
  
  /*! Process change of coordinates. */
  void coord_changed(Field_info* /* field_info */);

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! A 2D cross section of the knob */
  std::vector<Vector2f> m_knob_cross_section;

  /*! The center of the Ego structure. The generated texture coordinate of a
   * vertex v is the displacement of v relative to m_center (v - m_center).
   */
  Vector3f m_center;

  /*! default values for Ego_brick.  */
  static const Uint s_def_number_of_knobs1;
  static const Uint s_def_number_of_knobs2;
  static const Float s_def_pitch;
  static const Float s_def_height;
  static const Float s_def_knob_radius;
  static const Float s_def_knob_height;
  static const Float s_def_tolerance;
  static const Uint s_def_knob_slices;
  static const Boolean s_def_knobs_visible;
};

SGAL_END_NAMESPACE

#endif
