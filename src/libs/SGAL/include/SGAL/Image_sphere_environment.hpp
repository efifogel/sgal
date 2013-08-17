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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_SPHERE_ENVIRONMENT_HPP
#define SGAL_IMAGE_SPHERE_ENVIRONMENT_HPP

#include <string>

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Image_sphere_environment : public Sphere_environment {
public:
  typedef Image::Path_list                          Path_list;

  /*! Constructor */
  Image_sphere_environment(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_sphere_environment();

  /*! Construct the prototype. */
  static Image_sphere_environment* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this image textute extracted from the VRML or X3D
   * file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene
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

  /*! Set the directory-search structure. */
  void set_dirs(const Path_list& dirs);

  /*! Obtain the directory-search structure. */
  const Path_list& get_dirs() const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The image. */
  Image m_images[2];
};

/*! \brief constructs the prototype. */
inline Image_sphere_environment* Image_sphere_environment::prototype()
{ return new Image_sphere_environment(true); }

/*! \brief clones. */
inline Container* Image_sphere_environment::clone()
{ return new Image_sphere_environment(); }

/*! \brief adds the container to a given scene */
inline void Image_sphere_environment::add_to_scene(Scene_graph* sg)
{
  m_images[0].set_dirs(sg->get_data_dirs());
  m_images[1].set_dirs(sg->get_data_dirs());
}

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Image_sphere_environment::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
