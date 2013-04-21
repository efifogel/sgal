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
 * A geometry container that represents a quadric surface
 */

#ifndef SGAL_QUADRIC_GEO_HPP
#define SGAL_QUADRIC_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>
#include <vector>

#if QdX_USE_AcX
#include <QdX/Quadric_3.h>
#else
#include <QdX/P_quadric_3.h>
#endif
#include <NiX/Arithmetic_traits.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Scene_graph;
class Element;
class Cull_context;

class SGAL_CLASSDEF Quadric_geo : public Geometry {
public:
  typedef NiX::Arithmetic_traits AT;
#if QdX_USE_AcX
typedef QdX::Quadric_3<AT> Quadric_3;
#else
typedef QdX::P_quadric_3<AT> Quadric_3;
#endif
  
  enum {
    FIRST = Geometry::LAST - 1,
    COEFFICIENTS,
    LAST
  };

  /*! Constructor */
  Quadric_geo(Boolean proto = false);

  /*! Destructor */
  virtual ~Quadric_geo();

  /* Construct the prototype. */
  static Quadric_geo* prototype() { return new Quadric_geo(true); }

  /*! Clone. */
  virtual Container* clone() { return new Quadric_geo(); }

  /*! Set the coefficients of the quadric. */
  void set_coefficients(std::vector<AT::Integer>& coeff)
  { m_coefficients = coeff; }

  /*! Obtain the coefficients of the quadric. */
  const std::vector<AT::Integer>& get_coefficients() const
  { return m_coefficients; }

  /*! \brief initializes the container prototype. */
  virtual void init_prototype();

  /*! \brief deletes the container prototype. */
  virtual void delete_prototype(); 

  /*! \brief obtains the container prototype. */
  virtual Container_proto* get_prototype();

  /*! \brief sets the ellpsoid attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! */
  virtual Boolean clean_sphere_bound();

  /*! Draw the geometry
   * \param action
   */
  virtual void draw(Draw_action* action);

  /*! Clean the representation and prepares it for drawing.
   */
  void clean();
  
  /*! Clear the internal representation and auxiliary data structures.
   */
  void clear();

  /*! Determine whether the representation hasn't been updated.
   */
  Boolean is_dirty() const { return m_dirty; }
  
  /*! Is the representation empty ?
   * \return a Boolean that indicates whether the representation empty.
   */
  Boolean is_empty() const;

  /*! Obtain the quadric object.
   * \return the quadric object
   */
  const Quadric_3& get_quadric() const { return m_quadric; }
  
protected:
  /*! The coefficients of the quadric */
  std::vector<AT::Integer> m_coefficients;

  /*! obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:  
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether the mesh must be cleaned. */
  Boolean m_dirty;

  /*! The quadric object. */
  Quadric_3 m_quadric;

  /*! Draws the representation.
   * \param action
   */
  void draw_geometry(Draw_action* action);
};

SGAL_END_NAMESPACE

#endif
