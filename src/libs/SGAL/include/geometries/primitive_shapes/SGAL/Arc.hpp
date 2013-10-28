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
// $Id: $
// $Revision: 7263 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * An implementation of spherecal arc
 *
 * A geometry of type arc. The arc is specified by its radius assumed to
 * be centered at (0, 0, 0), and 4 angles. 2 Angles specify the lower left
 * corner, and 2 angles specify the arc dimensions.
 *
 * Inherits from Geometry.
 */

#ifndef SGAL_ARC_HPP
#define SGAL_ARC_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;
class Element;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Arc : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    RADIUS,
    ALPHA,
    BETA,
    GAMMA,
    DELTA,
    SOLID,
    STACKS,
    SLICES,
    LAST
  };

  /*! Constructor */
  Arc(Boolean proto = false);

  /*! Destructor */
  virtual ~Arc();

  /* Construct the prototype */
  static Arc* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype
   * \return the node prototype
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* radius_handle(Field_info*) { return &m_radius; }
  Uint* stacks_handle(Field_info*) { return &m_stacks; }
  Uint* slices_handle(Field_info*) { return &m_slices; }
  Float* alpha_handle(Field_info*) { return &m_alpha; }
  Float* beta_handle(Field_info*) { return &m_beta; }
  Float* gamma_handle(Field_info*) { return &m_gamma; }
  Float* delta_handle(Field_info*) { return &m_delta; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the arc
   * \param action the draw action
   */
  virtual void draw(Draw_action* action);

  /*! Draw the arc in selection mode
   * \param action the draw action
   */
  virtual void isect(Isect_action* action);

  /*! Calculare the sphere bound of the arc */
  virtual Boolean clean_sphere_bound();

  // virtual void FieldChanged(short fieldId);

  /*! Set the arc radius
   * \param radius the new radius
   */
  void set_radius(Float radius);

  /*! Obtain the arc radius
   * \return the arc radius
   */
  Float get_radius() const;

  /*! Set the number of stacks
   * \param stacks the new number of stacks
   */
  void set_stacks(Uint stacks);

  /*! Obtain the number of stacks
   * \return the number of stacks
   */
  Uint get_stacks() const;

  /*! Set the number of slices
   * \param  the new number of slices
   */
  void set_slices(Uint slices);

  /*! Obtain the number of slices
   * \return the number of slices
   */
  Uint get_slices() const;

  /*! Set the lower left corner (X and Z coordinates.
   * The lower left corner is the intersection of the sphere and a vector. The
   * vector is defined by 2 angles.
   * \param the angle between the vector and the Z axis in the XZ plane.
   */
  void set_alpha(Float alpha);

  /*! Obtain */
  Float get_alpha() const;

  /*! Set */
  void set_beta(Float beta);

  /*! Obtain */
  Float get_beta() const;

  /*! Set */
  void set_gamma(Float gamma);

  /*! Obtain */
  Float get_gamma() const;

  /*! Set */
  void set_delta(Float delta);

  /*! Obtain */
  Float get_delta() const;

  /*! Set */
  void set_solid(Boolean solid);

  /*! Obtain */
  Boolean is_solid() const;

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const;

  /*! Halftone stipple pattern for backfacing elements */
  static Ubyte s_halftone[];

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The radius of the arc sphere */
  Float m_radius;

  /*! The number of stacks of the arc */
  Uint m_stacks;

  /*! The number of slices of the arc */
  Uint m_slices;

  Float m_alpha;

  Float m_beta;

  Float m_gamma;

  Float m_delta;

  /*! Indicates wether the object is a solid (water-proof) polytope.
   * When rendering a solid polytope, back-facing facets cann be culled out,
   * and front-facing facets can be rendered one-sided.
   */
  Boolean m_is_solid;

private:
  /*! default values of arc */
  static const Float s_def_radius;
  static const Uint s_def_stacks;
  static const Uint s_def_slices;
  static const Float s_def_alpha;
  static const Float s_def_beta;
  static const Float s_def_gamma;
  static const Float s_def_delta;
  static const Boolean s_def_is_solid;
};

/* \brief constructs the prototype. */
inline Arc* Arc::prototype() { return new Arc(true); }

/*! \brief clones. */
inline Container* Arc::clone() { return new Arc(); }

/*! \brief sets the arc radius.*/
inline void Arc::set_radius(Float radius)
{
  m_radius = radius;
  m_dirty_sphere_bound = true;
}

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief obtains the arc radius. */
inline Float Arc::get_radius() const { return m_radius; }

/*! \brief sets the number of stacks. */
inline void Arc::set_stacks(Uint stacks) { m_stacks = stacks; }

/*! \brief obtains the number of stacks. */
inline Uint Arc::get_stacks() const { return m_stacks; }

/*! \brief sets the number of slices. */
inline void Arc::set_slices(Uint slices) { m_slices = slices; }

/*! \brief obtains the number of slices */
inline Uint Arc::get_slices() const { return m_slices; }

/*! \brief sets the lower left corner. */
inline void Arc::set_alpha(Float alpha) { m_alpha = alpha; }

/*! \brief obtains */
inline Float Arc::get_alpha() const { return m_alpha; }

/*! \brief sets */
inline void Arc::set_beta(Float beta) { m_beta = beta; }

/*! \brief obtains */
inline Float Arc::get_beta() const { return m_beta; }

/*! \brief sets */
inline void Arc::set_gamma(Float gamma) { m_gamma = gamma; }

/*! \brief obtains */
inline Float Arc::get_gamma() const { return m_gamma; }

/*! \brief sets */
inline void Arc::set_delta(Float delta) { m_delta = delta; }

/*! \brief obtains */
inline Float Arc::get_delta() const { return m_delta; }

/*! \brief sets */
inline void Arc::set_solid(Boolean solid) { m_is_solid = solid; }

/*! \brief obtain */
inline Boolean Arc::is_solid() const { return m_is_solid; }

/*! \brief obtains the tag (type) of the container */
inline const std::string& Arc::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
