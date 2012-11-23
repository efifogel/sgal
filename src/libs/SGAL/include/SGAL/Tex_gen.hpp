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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *
 */

#ifndef SGAL_TEX_GEN_HPP
#define SGAL_TEX_GEN_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Element;

class Tex_gen : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    MODE_S,
    MODE_T,
    MODE_R,
    MODE_Q,
    PLANE_S,
    PLANE_T,
    PLANE_R,
    PLANE_Q,
    LAST
  };

  /*! Constructor */
  Tex_gen(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Tex_gen();

  /*! Construct the prototype */
  static Tex_gen* prototype() { return new Tex_gen(SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Tex_gen(); }

  enum Mode {
    OFF,
    EYE_LINEAR,
    EYE_LINEAR_IDENT,
    OBJECT_LINEAR,
    SPHERE_MAP
  };

  void set_modes(Mode modes);
  Mode get_modes();

  void set_modet(Mode modet);
  Mode get_modeT();

  void set_moder(Mode moder);
  Mode get_modeR();

  void set_modeq(Mode modeq);
  Mode get_modeQ();

  void set_planes(const Vector4f& planes);
  void get_planes(Vector4f& planes);
  void set_planes(Float v0, Float v1, Float v2, Float v3);
  void get_planes(Float* v0, Float* v1, Float* v2, Float* v3);
  
  void set_planet(const Vector4f& planet);
  void get_planet(Vector4f& planet);
  void set_planet(Float v0, Float v1, Float v2, Float v3);
  void get_planet(Float* v0, Float* v1, Float* v2, Float* v3);

  void set_planer(const Vector4f& planer);
  void get_planer(Vector4f& planer);
  void set_planer(Float v0, Float v1, Float v2, Float v3);
  void get_planer(Float* v0, Float* v1, Float* v2, Float* v3);

  void set_planeq(const Vector4f& planeq);
  void get_planeq(Vector4f& planeq);
  void set_planeq(Float v0, Float v1, Float v2, Float v3);
  void get_planeq(Float* v0, Float* v1, Float* v2, Float* v3);

  bool is_changed() { return false; }

  virtual void draw(Context * ctx);

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();
  
protected: 
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  Int m_dirty_id;
  Int m_dirty_count;

  friend class Context;

  // Fields
  Int m_modes;
  Int m_modet;
  Int m_moder;
  Int m_modeq;
  Vector4f m_planes;
  Vector4f m_planet;
  Vector4f m_planer;
  Vector4f m_planeq;

private:
  // Efi: Why is it (preventing copy construction) here?
  Tex_gen(const Tex_gen& tex_gen) :
    Container(tex_gen)
  {}
  
  void init();
};

/*!
 */
inline void Tex_gen::set_planes(const Vector4f& planes)
{
  set_planes(planes.get(0), planes.get(1), planes.get(2), planes.get(3));
}

/*!
 */
inline void Tex_gen::get_planes(Vector4f& planes)
{
  get_planes(&planes[0], &planes[1], &planes[2], &planes[3]);
}

/*!
 */
inline void Tex_gen::set_planet(const Vector4f& planet)
{
  set_planet(planet.get(0), planet.get(1), planet.get(2), planet.get(3));
}

/*!
 */
inline void Tex_gen::get_planet(Vector4f& planet)
{
  get_planet(&planet[0], &planet[1], &planet[2], &planet[3]);
}

/*!
 */
inline void Tex_gen::set_planer(const Vector4f& planer)
{
  set_planer(planer.get(0), planer.get(1), planer.get(2), planer.get(3));
}

/*!
 */
inline void Tex_gen::get_planer(Vector4f& planer)
{
  get_planer(&planer[0], &planer[1], &planer[2], &planer[3]);
}

/*!
 */
inline void Tex_gen::set_planeq(const Vector4f& planeq)
{
  set_planeq(planeq.get(0), planeq.get(1), planeq.get(2), planeq.get(3));
}

/*!
 */
inline void Tex_gen::get_planeq(Vector4f& planeq)
{
  get_planeq(&planeq[0], &planeq[1], &planeq[2], &planeq[3]);
}

SGAL_END_NAMESPACE

#endif
