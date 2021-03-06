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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node that represents a reflection spherical map.
 */

#ifndef SGAL_SPHERE_ENVIRONMENT_HPP
#define SGAL_SPHERE_ENVIRONMENT_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Execution_coordinator;
class Container_proto;
class Element;
class Image;

#if (defined _MSC_VER)
template struct SGAL_SGAL_DECL std::pair<boost::shared_ptr<Image>, Boolean>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Sphere_environment : public Texture {
public:
  enum {
    FIRST = Texture::LAST - 1,
    IMAGE,
    ALPHA,
    LAST
  };

  enum Images {
    IMAGE0 = 0,
    IMAGE1,
    NUM_IMAGES
  };

  typedef boost::shared_ptr<Image>              Shared_image;

  /*! Constructor */
  Sphere_environment(Boolean proto = false);

  /*! Destructor */
  virtual ~Sphere_environment();

  /*! Construct the prototype */
  static Sphere_environment* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* alpha_handle(const Field_info*) { return &m_alpha; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Clean the object in case it is dirty. */
  virtual void clean();

  /*! Determines whether the texture is empty. */
  virtual Boolean empty();

  /*! Obtain the texture number of components. */
  virtual Uint get_component_count() const;

  /*! Set the alpha value */
  void set_alpha(float alpha);

  /*! Obtain the alpha value */
  float get_alpha() const;

  /*! Set the quality */
  void set_quality(bool flag);

  /*! Obtain the quality */
  bool get_quality() const;

protected:
  /*! The texture images and a flag that indicates that the image is owned. */
  std::pair<Shared_image, Boolean> m_images[NUM_IMAGES];

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! the node prototype */
  static Container_proto* s_prototype;

  /*! the alpha value to use in blending in the second pass */
  float m_alpha;

  /*! if true, double pass is used to implement reflection */
  bool m_quality;

  /*! Default values */
  static const Float m_def_alpha;
  static const Boolean m_def_quality;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Sphere_environment* Sphere_environment::prototype()
{ return new Sphere_environment(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Sphere_environment::create()
{ return new Sphere_environment(); }

//! \brief sets the alpha value.
inline void Sphere_environment::set_alpha(float alpha) { m_alpha = alpha; }

//! \brief obtains the alpha value.
inline float Sphere_environment::get_alpha() const { return m_alpha; }

//! \brief sets the quality.
inline void Sphere_environment::set_quality(bool flag) { m_quality = flag; }

//! \brief obtains the quality.
inline bool Sphere_environment::get_quality() const { return m_quality; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Sphere_environment::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
