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
 * A geometry container that represents a cylindrical patch. Such a patch
 * is defined by the radius and height of the underlying cylinder and by
 * an angle of the horizontal cross section.
 */

#ifndef SGAL_CYLINDRICAL_PATCH_HPP
#define SGAL_CYLINDRICAL_PATCH_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Cylindrical_patch : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    RADIUS,
    HEIGHT,
    SIDE,
    BOTTOM, 
    TOP,
    SLICES,
    ALPHA,
    BETA,
    LAST
  };

  /*! Constructor */
  Cylindrical_patch(Boolean proto = false);

  /*! Destructor */
  virtual ~Cylindrical_patch();

  /* Construct the prototype */
  static Cylindrical_patch* prototype()
  { return new Cylindrical_patch(true); }

  /*! Clone */
  virtual Container* clone() { return new Cylindrical_patch(); }

  /*! Set the radius of the underlying cylinder
   * \param radius the radius of the underlying cylinder
   */
  void set_radius(Float radius)
  {
    m_radius = radius;
    m_dirty_sphere_bound = true;
  }

  /*! Obtain the radius of the underlying cylinder
   * \return the radius of the underlying cylinder
   */
  Float get_radius() const { return m_radius; }

  /*! Set the height of the underlying cylinder
   * \param height the height of the underlying cylinder
   */  
  void set_height(Float height)
  {
    m_height = height;
    m_dirty_sphere_bound = true;
  }

  /*! Obtain the height of the underlying cylinder
   * \return the height of the underlying cylinder
   */  
  Float get_height() const { return m_height; }
  
  /*! Set the angle between the vector (1,0,0) and the vector to the left
   * endpoint of the arc of a horizontal cross section of the patch
   * \param alpha the angle 
   */
  void set_alpha(Float alpha) { m_alpha = alpha; }

  /*! Obtain the angle between the vector (1,0,0) and the vector to the left
   * endpoint of the arc of a horizontal cross section of the patch
   * \return the angle
   */
  Float get_alpha() const { return m_alpha; }

  /*! Set the angle between the vector (1,0,0) and the vector to the right
   * endpoint of the arc of a horizontal cross section of the patch
   * \param beta the angle
   */
  void set_beta(Float beta) { m_beta = beta; }

  /*! Obtain the angle between the vector (1,0,0) and the vector to the right
   * endpoint of the arc of a horizontal cross section of the patch
   * \return the angle
   */
  Float get_beta() const { return m_beta; }
  
  /*! Set the number of slices of a horizontal cross section of the patch
   * \param slices the number of slices of a horizontal cross section
   */
  void set_slices(Uint slices) { m_slices = slices; }

  /*! Obtain the number of slices of a horizontal cross section of the patch
   * \return the number of slices of a horizontal cross section
   */
  Uint get_slices() const { return m_slices; }

  /*! Draw the cylindrical patch
   * \param action the draw action
   */
  virtual void draw(Draw_action* action); 
  
  /*! Draw the cylindrical patch in selection mode 
   * \param action the draw action
   */
  virtual void isect(Isect_action* action);

  /*! Calculare the sphere bound of the cylindrical patch */
  virtual Boolean clean_sphere_bound();
    
  /*! Initializes the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype
   * \return the container prototype
   */
  virtual Container_proto* get_prototype();

  /*! Sets the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Determine whether the geometry has a texture coordinate array. */
  virtual Boolean has_tex_coord() const;

  /*! Is dirty? */
  /* Boolean is_dirty() const { return m_dirty; } */

protected:
  /*! Indicates whether the patch has been cleaned */
  /* Boolean m_dirty; */
  
  /*! The radius of the underlying cylinder */
  Float m_radius;

  /*! The height of the underlying cylinder */
  Float m_height;

  /*! The angle between the vector (1,0,0) and the vector to the left
   * endpoint of the arc of a horizontal cross section of the patch
   */
  Float m_alpha;

  /*! The angle between the vector (1,0,0) and the vector to the right
   * endpoint of the arc of a horizontal cross section of the patch
   */
  Float m_beta;
  
  /*! The number if vertical slices used in rendering */
  Uint m_slices;

  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Default values */
  static const Float s_def_radius;
  static const Float s_def_height;
  static const Float s_def_alpha;
  static const Float s_def_beta;
  static const Uint s_def_slices;

  /*! Draw a single quadrilateral
   * \param cos_left the cosine of the angle between the vector (1,0,0) and
   * the vector to the bottm left corner
   * \param sin_left the sine of the angle between the vector (1,0,0) and
   * the vector to the bottm left corner
   * \param cos_right the cosine of the angle between the vector (1,0,0) and
   * the vector to the bottm right corner
   * \param sin_right the sine of the angle between the vector (1,0,0) and
   * the vector to the bottm right corner
   */
  void draw_quad(Float cos_left, Float sin_left,
                 Float cos_right, Float sin_right);
};

/*! \brief determines whether the geometry has a texture coordinate array. */
inline Boolean Cylindrical_patch::has_tex_coord() const { return true; }

SGAL_END_NAMESPACE

#endif
