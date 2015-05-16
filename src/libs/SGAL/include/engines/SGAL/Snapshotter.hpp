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
 * Take a snapshot view and save to a file.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#ifndef SGAL_SNAPSHOTTER_HPP
#define SGAL_SNAPSHOTTER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/File_format_2d.hpp"

SGAL_BEGIN_NAMESPACE

class Local_stream;
class Container_proto;
class Element;
class Scene_graph;
class ofstream;
class Image;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Snapshotter Snapshotter.hpp
 * Snapshotter is a node in the scene graph that specifies the parameters of the
 * snapshot operation, which is, recording the content of a graphics window
 * in an image file.
 */
class SGAL_SGAL_DECL Snapshotter : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    TRIGGER,
    MODE,
    FLIP,
    IMAGE,
    LAST
  };

  enum Mode {
    MODE_FRONT_LEFT = 0,
    MODE_FRONT_RIGHT,
    MODE_BACK_LEFT,
    MODE_BACK_RIGHT,
    MODE_FRONT,
    MODE_BACK,
    MODE_LEFT,
    MODE_RIGHT,
    MODE_COLOR_ATTACHMENT
  };

  typedef boost::shared_ptr<Image>              Shared_image;

  /*! Constructor
   */
  Snapshotter(Boolean proto = false);

  /*! Destructor */
  virtual ~Snapshotter();

  /*! Construct the prototype.
   */
  static Snapshotter* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype.
   */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Mode* mode_handle(const Field_info*) { return &m_mode; }
  Boolean* flip_handle(const Field_info*) { return &m_flip; }
  Shared_image* image_handle(const Field_info*) { return &m_image; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Set the size of the image.
   */
  void set_size(Draw_action* action);

  /*! If triggered takes a snapshot and writes it to an image file.
   */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Take a snapshot and write it to an image file.
   */
  void execute();

  /*! Trigger the snapshot.
   */
  void trigger();

  /*! Set the color buffer.
   */
  void set_mode(Mode mode);

  /*! Get the color buffer.
   */
  Mode get_mode() const;

  /*! Set the flag that indicates whether the image should be reflected. */
  void set_flip(Boolean flag);

  /*! Determine whether the image should be reflected. */
  Boolean is_flip() const;

  /*! Set the image.
   */
  void set_image(Shared_image image);

  /*! Obtain the image.
   */
  Shared_image get_image() const;

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! The trigger of the engine, which makes the engine excute. */
  Boolean m_trigger;

  /*! The color buffer, whcih is the source for subsequent gl OpenGL calls,
   * such as glReadPixels()
   */
  Mode m_mode;

  /*! Indicates whether the rows are in reverse order (window system style) */
  Boolean m_flip;

  /*! A place holder for the image. */
  Shared_image m_image;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The color buffer names. */
  static const Char* s_mode_names[];

  // Default values
  //@{
  static const Boolean s_def_flip;
  static const Mode s_def_mode;
  //@}

  /*! Take a snapshot of the window.
   */
  void take_snapshot();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Snapshotter* Snapshotter::prototype() { return new Snapshotter(true); }

//! \brief clones.
inline Container* Snapshotter::clone() { return new Snapshotter(); }

//! \brief triggers the snapshot.
inline void Snapshotter::trigger() { m_trigger = true; }

//! Set the color buffer.
inline void Snapshotter::set_mode(Mode mode) { m_mode = mode; }

//! gets the color buffer.
inline Snapshotter::Mode Snapshotter::get_mode() const { return m_mode; }

//! \brief sets the flag that indicates whether the image should be reflected.
inline void Snapshotter::set_flip(Boolean flag) { m_flip = flag; }

//! \brief determines whether the image should be reflected.
inline Boolean Snapshotter::is_flip() const { return m_flip; }

//! \brief sets the image.
inline void Snapshotter::set_image(Shared_image image) { m_image = image; }

//! \brief obtains the image.
inline Snapshotter::Shared_image Snapshotter::get_image() const
{ return m_image; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Snapshotter::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
