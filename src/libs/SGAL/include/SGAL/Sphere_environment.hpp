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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node that represents a reflection spherical map.
 */

#ifndef SGAL_SPHERE_ENVIRONMENT_HPP
#define SGAL_SPHERE_ENVIRONMENT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Execution_coordinator;
class Container_proto;
class Element;

class SGAL_CLASSDEF Sphere_environment : public Texture {
public:
  enum {
    FIRST = Texture::LAST - 1,
    ALPHA,
    LAST
  };

  /*! Constructor */
  Sphere_environment(Boolean proto = false);

  /*! Destructor */
  virtual ~Sphere_environment();

  /*! Construct the prototype */
  static Sphere_environment* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();

  /*! Set the alpha value */
  void set_alpha(float alpha);

  /*! Obtain the alpha value */
  float get_alpha() const;

  /*! Set the quality */
  void set_quality(bool flag);

  /*! Obtain the quality */
  bool get_quality() const;

  /*! Set the texture */
  void set_texture(Texture* texture);

  /*! Obtain the texture */
  Texture* get_texture();

  // // this method is used when an appearance is created by the environment
  // // object it should not be called in any other case (FIX)
  // Execution_coordinator* get_execution_coordinator() const
  // { return m_execution_coordinator; }

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! the node prototype */
  static Container_proto* s_prototype;

  /*! the texture object */
  Texture* m_texture;

  /*! the alpha value to use in blending in the second pass */
  float m_alpha;

  /*! if true, double pass is used to implement reflection */
  bool m_quality;

  /*! Default values */
  static const Float m_def_alpha;
  static const Boolean m_def_quality;

  /*! Copy constructor */
  Sphere_environment(const Sphere_environment& se) : Texture(se) {} 
};

/*! \brief constructs the prototype. */
inline Sphere_environment* Sphere_environment::prototype()
{ return new Sphere_environment(true); }

/*! \brief clones. */
inline Container* Sphere_environment::clone()
{ return new Sphere_environment(); }

/*! \brief sets the alpha value. */
inline void Sphere_environment::set_alpha(float alpha) { m_alpha = alpha; }

/*! \brief obtains the alpha value. */
inline float Sphere_environment::get_alpha() const { return m_alpha; }

/*! \brief sets the quality. */
inline void Sphere_environment::set_quality(bool flag) { m_quality = flag; }

/*! \brief obtains the quality. */
inline bool Sphere_environment::get_quality() const { return m_quality; }

/*! \brief sets the texture. */
inline void Sphere_environment::set_texture(Texture* texture)
{ m_texture = texture; }

/*! \brief obtain the texture. */
inline Texture* Sphere_environment::get_texture() { return m_texture; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Sphere_environment::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
