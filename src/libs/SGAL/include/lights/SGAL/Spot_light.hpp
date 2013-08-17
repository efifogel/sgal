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

/*!
 * An implementation of a spot light.
 *
 * A spot light is a light that is positioned as specified, and has a
 * direction and  a cut off cone specified.
 *
 * Inherits from Light
 */

#ifndef SGAL_SPOT_LIGHT_HPP
#define SGAL_SPOT_LIGHT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;
class Element;

class SGAL_SGAL_DECL Spot_light : public Light {
public:
  enum {
    FIRST = Light::LAST - 1,
    DIRECTION,
    ATTENUATION,
    LOCATION,
    RADIUS,
    BEAMWIDTH,
    CUTOFFANGLE,
    LAST
  };

  /*! Constructor */
  Spot_light(Boolean proto = false);

  /*! Destructor */
  virtual ~Spot_light();

  /* Construct the prototype. */
  static Spot_light* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto*  get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element*  elem);

  virtual Action::Trav_directive draw(Draw_action*  draw_action);

  // virtual Attribute_list get_attributes();

  void set_direction(const Vector3f& direction);
  void get_direction(Vector3f& direction);
  void set_beam_width(const Float& beam_width);
  Float get_beam_width();
  void set_cutoff_angle(const Float& cutoff_angle);
  Float get_cutoff_angle();
  float get_exponent();
  void set_attenuation(const Vector3f& attenuation);
  void get_attenuation(Vector3f& attenuation);
  void set_location(const Vector3f& location);
  void get_location(Vector3f& location);
  void set_radius(const Float& radius);
  Float get_radius();

protected:
  /*! The attenuation factors (constant, linear, quadratic) */
  Vector3f m_attenuation;

  /*! The position of the light */
  Vector3f m_location;

  /*! The radius to which it shines (not used) */
  Float m_radius;

  // default values

  /*! The default attenuation factors (constant, linear, quadratic) */
  static Vector3f s_def_attenuation;

  /*! The default position of the light */
  static Vector3f s_def_location;

  /*! The default radius to which it shines (not used) */
  static Float s_def_radius;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The direction of the light */
  Vector3f m_direction;

  /*! The beam width (in degrees)*/
  Float m_beam_width;

  /*! The cut off angle (in degrees)*/
  Float m_cutoff_angle;

  // Default values

  /*! The direction of the light */
  static Vector3f s_def_direction;

  /*! The beam width */
  static Float s_def_beam_width;

  /*! The cut off angle */
  static Float s_def_cutoff_angle;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/* \brief constructs the prototype. */
inline Spot_light* Spot_light::prototype() { return new Spot_light(true); }

/*! \brief clones. */
inline Container* Spot_light::clone() { return new Spot_light(); }

SGAL_END_NAMESPACE

#endif
