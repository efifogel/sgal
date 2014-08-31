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
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Take a snapshot view and save to a file.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#ifndef SGAL_SNAPSHOT_HPP
#define SGAL_SNAPSHOT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

class Local_stream;
class Container_proto;
class Element;
class Scene_graph;
class ofstream;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Snapshot Snapshot.hpp
 * Snapshot is a node in the scene graph that specifies the parameters of the
 * snapshot operation, which is, recording the content of a graphics window
 * in an image file.
 */
class SGAL_SGAL_DECL Snapshot : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    TRIGGER,
    DIR_NAME,
    FILE_NAME,
    FILE_FORMAT,
    IMAGE,
    LAST
  };

  /*! File formats */
  enum File_format {
    FF_illegal = 0,
    FF_jpeg,
    FF_png,
    FF_gif,
    FF_tiff,
    FF_bmp,
    FF_ppm,
    FF_pgm,
    FF_pbm,
    FF_num
  };

  typedef boost::shared_ptr<Image>              Shared_image;

  /*! Constructor
   */
  Snapshot(Boolean proto = false);

  /*! Destructor */
  virtual ~Snapshot();

  /*! Construct the prototype.
   */
  static Snapshot* prototype();

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
  std::string* dir_name_handle(const Field_info*) { return &m_dir_name; }
  std::string* file_name_handle(const Field_info*) { return &m_file_name; }
  File_format* file_format_handle(const Field_info*) { return &m_file_format; }
  Shared_image* image_handle(const Field_info*) { return &m_image; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Write this container.
   * \param formatter The formatter to use for the writing, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Trigger the snapshot.
   */
  void trigger();

  // virtual Attribute_list get_attributes();

  /*! Set the triger field.
   */
  void set_trigger(Field_info* field_info = NULL);

  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Set the name of the dir where the snapshot is written to.
   */
  void set_dir_name(std::string dir_name);

  /*! Obtain the name of the dir where the snapshot is written to.
   */
  std::string get_dir_name();

  /*! Set the name of the file where the snapshot is written to.
   */
  void set_file_name(std::string& file_name);

  /*! Obtain the name of the file where the snapshot is written to.
   */
  const std::string& get_file_name();

  /*! Set the file format.
   */
  void set_file_format(File_format format);

  /*! Obtain the file format.
   */
  File_format get_file_format() const;

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

  /*! A place holder for the image. */
  Shared_image m_image;

  /*! The directory to save the image at. */
  std::string m_dir_name;

  /*! The name of the file the image is written to. */
  std::string m_file_name;

  /*! The format of the file the image is written to. */
  File_format m_file_format;

  /*! The quality of the (loss) image (applicable to jpeg). */
  Int m_quality;

  /*! Indicates whether to generate the file name  as part of a sequence. */
  Boolean m_sequence;

  /*! The sequence counter. */
  Uint m_count;

protected:
  /*! File format names. */
  static const char* s_file_format_names[];

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  Boolean m_trigger;

  Boolean m_use_front_buffer;

  Uint m_size;

  /*! Indicates whether the rows are in reverse order (window system style) */
  Boolean m_flip;

  // Default values for fields
  const static std::string s_def_dir_name;
  const static std::string s_def_file_name;
  const static File_format s_def_file_format;
  const static Int s_def_quality;
  const static Boolean s_def_flip;

  /*! Allocate space for the image.
   */
  Boolean allocate_space(Draw_action* action);

  /*! Take a snapshot of the window.
   */
  void take_snapshot();

  /*! Open a file and writes the image into it .
   */
  virtual void write_image();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Snapshot* Snapshot::prototype() { return new Snapshot(true); }

//! \brief clones.
inline Container* Snapshot::clone() { return new Snapshot(); }

//! \brief triggers the snapshot.
inline void Snapshot::trigger() { m_trigger = true; }

//! \brief sets the name of the dir where the snapshot is written to.
inline void Snapshot::set_dir_name(std::string dir_name)
{ m_dir_name = dir_name; }

//! \brief obtains the name of the dir where the snapshot is written to.
inline std::string Snapshot::get_dir_name() { return m_dir_name; }

//! \brief sets the name of the file where the snapshot is written to.
inline void Snapshot::set_file_name(std::string& file_name)
{ m_file_name = file_name; }

//! \brief obtains the name of the file where the snapshot is written to.
inline const std::string& Snapshot::get_file_name() { return m_file_name; }

//! \brief sets the file format.
inline void Snapshot::set_file_format(File_format format)
{ m_file_format = format; }

//! \brief obtains the file format.
inline Snapshot::File_format Snapshot::get_file_format() const
{ return m_file_format; }

//! \brief sets the image.
inline void Snapshot::set_image(Shared_image image) { m_image = image; }

//! \brief obtains the image.
inline Snapshot::Shared_image Snapshot::get_image() const { return m_image; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Snapshot::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
