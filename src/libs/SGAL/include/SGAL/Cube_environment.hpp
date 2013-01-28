// Copyright (c) 2013 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node that represents a reflection cubical map.
 */

#ifndef SGAL_CUBE_ENVIRONMENT_HPP
#define SGAL_CUBE_ENVIRONMENT_HPP

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Texture.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Scene_graph;
class Image;

class SGAL_CLASSDEF Cube_environment : public Texture {
public:
  enum {
    FIRST = Texture::LAST - 1,
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    BOTTOM,
    TOP,
    LAST
  };

  enum Images {
    LEFT_IMAGE = 0,
    RIGHT_IMAGE,
    FRONT_IMAGE,
    BACK_IMAGE,
    BOTTOM_IMAGE,
    TOP_IMAGE,
    NUM_IMAGES
  };
  
  /*! Constructor */
  Cube_environment(Boolean proto = false);

  /*! Destructor */
  virtual ~Cube_environment();

  /*! Construct the prototype. */
  static Cube_environment* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of the cubical environment map. */
  virtual void set_attributes(Element* elem);
  
  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  /*! Determines whether the texture is empty. */
  virtual Boolean empty();

  /*! Obtain the texture number of components. */
  virtual Uint get_component_count() const;

  /*! Set the left image.
   * \param image (in) the image.
   * \param owned (in) indicates whether the image is owned. If it is owned,
   *        a default image is constructed and destructed by the construct.
   */
  void set_left_image(Image* image, Boolean owned = false);

  /*! Obtain the (non const) left image. */
  Image* get_left_image();
  
  /*! Obtain the (const) left image. */
  const Image* get_left_image() const;

  /*! Set the right image.
   * \param image (in) the image.
   * \param owned (in) indicates whether the image is owned. If it is owned, 
   *        a default image is constructed and destructed by the construct.
   */
  void set_right_image(Image* image, Boolean owned = false);

  /*! Obtain the (non const) right image. */
  Image* get_right_image();

  /*! Obtain the (const) right image. */
  const Image* get_right_image() const;

  /*! Set the bottom image.
   * \param image (in) the image.
   * \param owned (in) indicates whether the image is owned. If it is owned, 
   *        a default image is constructed and destructed by the construct.
   */
  void set_front_image(Image* image, Boolean owned = false);

  /*! Obtain the (non const) front image. */
  Image* get_front_image();

  /*! Obtain the (const) front image. */
  const Image* get_front_image() const;

  /*! Set the top image.
   * \param image (in) the image.
   * \param owned (in) indicates whether the image is owned. If is it owned, 
   *        a default image is constructed and destructed by the construct.
   */
  void set_back_image(Image* image, Boolean owned = false);

  /*! Obtain the (non const) back image. */
  Image* get_back_image();

  /*! Obtain the (const) back image. */
  const Image* get_back_image() const;
  
  /*! Set the back image.
   * \param image (in) the image.
   * \param owned (in) indicates whether the image is owned. If it is owned, 
   *        a default image is constructed and destructed by the construct.
   */
  void set_bottom_image(Image* image, Boolean owned = false);

  /*! Obtain the (non const) bottom image. */
  Image* get_bottom_image();

  /*! Obtain the (const) bottom image. */
  const Image* get_bottom_image() const;
  
  /*! Set the front image.
   * \param image (in) the image.
   * \param owned (in) indicates whether the image is owned. If it is owned, 
   *        a default image is constructed and destructed by the construct.
   */
  void set_top_image(Image* image, Boolean owned = false);

  /*! Obtain the (non const) top image. */
  Image* get_top_image();

  /*! Obtain the (const) top image. */
  const Image* get_top_image() const;
  
protected:
  /*! The texture images and a flag that indicates that the image is owned. */
  std::pair<Image*,Boolean> m_images[NUM_IMAGES];

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! the node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Cube_environment* Cube_environment::prototype()
{ return new Cube_environment(true); }

/*! \brief clones. */
inline Container* Cube_environment::clone() { return new Cube_environment(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Cube_environment::get_tag() const { return s_tag; }

/*! \brief obtains the (non const) left image. */
inline Image* Cube_environment::get_left_image()
{ return m_images[LEFT_IMAGE].first; }
  
/*! \brief obtains the (const) left image. */
inline const Image* Cube_environment::get_left_image() const
{ return m_images[LEFT_IMAGE].first; }
  
/*! \brief obtains the (non const) right image. */
inline Image* Cube_environment::get_right_image()
{ return m_images[RIGHT_IMAGE].first; }

/*! \brief obtains the (const) right image. */
inline const Image* Cube_environment::get_right_image() const
{ return m_images[RIGHT_IMAGE].first; }

/*! \brief obtains the (non const) front image. */
inline Image* Cube_environment::get_front_image()
{ return m_images[FRONT_IMAGE].first; }

/*! \brief obtains the (const) front image. */
inline const Image* Cube_environment::get_front_image() const
{ return m_images[FRONT_IMAGE].first; }

/*! \brief obtains the (non const) back image. */
inline Image* Cube_environment::get_back_image()
{ return m_images[BACK_IMAGE].first; }

/*! \brief obtains the (const) back image. */
inline const Image* Cube_environment::get_back_image() const
{ return m_images[BACK_IMAGE].first; }

/*! \brief obtains the (non const) bottom image. */
inline Image* Cube_environment::get_bottom_image()
{ return m_images[BOTTOM_IMAGE].first; }

/*! \brief obtains the (const) bottom image. */
inline const Image* Cube_environment::get_bottom_image() const
{ return m_images[BOTTOM_IMAGE].first; }

/*! \brief obtains the (non const) top image. */
inline Image* Cube_environment::get_top_image()
{ return m_images[TOP_IMAGE].first; }

/*! \brief obtains the (const) top image. */
inline const Image* Cube_environment::get_top_image() const
{ return m_images[TOP_IMAGE].first; }

SGAL_END_NAMESPACE

#endif
