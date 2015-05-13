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
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Texture.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Scene_graph;
class Image_reader;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Cube_environment : public Texture {
public:
  enum {
    FIRST = Texture::LAST - 1,
    RIGHT,
    LEFT,
    BACK,
    FRONT,
    TOP,
    BOTTOM,
    LAST
  };

  enum Images {
    RIGHT_IMAGE = 0,
    LEFT_IMAGE,
    BACK_IMAGE,
    FRONT_IMAGE,
    TOP_IMAGE,
    BOTTOM_IMAGE,
    NUM_IMAGES
  };

  typedef boost::shared_ptr<Image_reader>       Shared_image;

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

  /// \name field handlers
  //@{
  //@}

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  /*! Determines whether the texture is empty. */
  virtual Boolean empty();

  /*! Obtain the texture number of components. */
  virtual Uint get_component_count() const;

  /*! Set the left image.
   * \param image (in) the image.
   */
  void set_left_image(Shared_image image);

  /*! Obtain the (non const) left image. */
  Shared_image get_left_image();

  /*! Obtain the (const) left image. */
  const Shared_image get_left_image() const;

  /*! Set the right image.
   * \param image (in) the image.
   */
  void set_right_image(Shared_image image);

  /*! Obtain the (non const) right image. */
  Shared_image get_right_image();

  /*! Obtain the (const) right image. */
  const Shared_image get_right_image() const;

  /*! Set the bottom image.
   * \param image (in) the image.
   */
  void set_front_image(Shared_image image);

  /*! Obtain the (non const) front image. */
  Shared_image get_front_image();

  /*! Obtain the (const) front image. */
  const Shared_image get_front_image() const;

  /*! Set the top image.
   * \param image (in) the image.
   */
  void set_back_image(Shared_image image);

  /*! Obtain the (non const) back image. */
  Shared_image get_back_image();

  /*! Obtain the (const) back image. */
  const Shared_image get_back_image() const;

  /*! Set the back image.
   * \param image (in) the image.
   */
  void set_bottom_image(Shared_image image);

  /*! Obtain the (non const) bottom image. */
  Shared_image get_bottom_image();

  /*! Obtain the (const) bottom image. */
  const Shared_image get_bottom_image() const;

  /*! Set the front image.
   * \param image (in) the image.
   */
  void set_top_image(Shared_image image);

  /*! Obtain the (non const) top image. */
  Shared_image get_top_image();

  /*! Obtain the (const) top image. */
  const Shared_image get_top_image() const;

protected:
  /*! The texture images and a flag that indicates that the image is owned. */
  Shared_image m_images[NUM_IMAGES];

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! the node prototype */
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype. */
inline Cube_environment* Cube_environment::prototype()
{ return new Cube_environment(true); }

/*! \brief clones. */
inline Container* Cube_environment::clone() { return new Cube_environment(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Cube_environment::get_tag() const { return s_tag; }

/*! \brief obtains the (non const) left image. */
inline Cube_environment::Shared_image Cube_environment::get_left_image()
{ return m_images[LEFT_IMAGE]; }

/*! \brief obtains the (const) left image. */
inline const Cube_environment::Shared_image Cube_environment::get_left_image()
  const
{ return m_images[LEFT_IMAGE]; }

/*! \brief obtains the (non const) right image. */
inline Cube_environment::Shared_image Cube_environment::get_right_image()
{ return m_images[RIGHT_IMAGE]; }

/*! \brief obtains the (const) right image. */
inline const Cube_environment::Shared_image Cube_environment::get_right_image()
  const
{ return m_images[RIGHT_IMAGE]; }

/*! \brief obtains the (non const) front image. */
inline Cube_environment::Shared_image Cube_environment::get_front_image()
{ return m_images[FRONT_IMAGE]; }

/*! \brief obtains the (const) front image. */
inline const Cube_environment::Shared_image Cube_environment::get_front_image()
  const
{ return m_images[FRONT_IMAGE]; }

/*! \brief obtains the (non const) back image. */
inline Cube_environment::Shared_image Cube_environment::get_back_image()
{ return m_images[BACK_IMAGE]; }

/*! \brief obtains the (const) back image. */
inline const Cube_environment::Shared_image Cube_environment::get_back_image()
  const
{ return m_images[BACK_IMAGE]; }

/*! \brief obtains the (non const) bottom image. */
inline Cube_environment::Shared_image Cube_environment::get_bottom_image()
{ return m_images[BOTTOM_IMAGE]; }

/*! \brief obtains the (const) bottom image. */
inline const Cube_environment::Shared_image Cube_environment::get_bottom_image()
  const
{ return m_images[BOTTOM_IMAGE]; }

/*! \brief obtains the (non const) top image. */
inline Cube_environment::Shared_image Cube_environment::get_top_image()
{ return m_images[TOP_IMAGE]; }

/*! \brief obtains the (const) top image. */
inline const Cube_environment::Shared_image Cube_environment::get_top_image()
  const
{ return m_images[TOP_IMAGE]; }

SGAL_END_NAMESPACE

#endif
