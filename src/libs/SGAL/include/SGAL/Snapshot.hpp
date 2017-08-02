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

/*! This file and its content, namely the class Snapshot, are obsolete.
 * They are retained for backward compatibility.
 * Use Snapshotter instead.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#ifndef SGAL_SNAPSHOT_HPP
#define SGAL_SNAPSHOT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Snapshotter.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/File_format_2d.hpp"

SGAL_BEGIN_NAMESPACE

class Local_stream;
class Container_proto;
class Element;
class Scene_graph;
class ofstream;
class Formatter;
class Image_writer;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Snapshot Snapshot.hpp
 * Snapshot is a node in the scene graph that takes a snapshot of the 3D scene
 * and exports it to an image file.
 */
class SGAL_SGAL_DECL Snapshot : public Snapshotter {
public:
  enum {
    FIRST = Snapshotter::LAST - 1,
    DIR_NAME,
    FILE_NAME,
    FILE_FORMAT,
    SEQUENCE,
    IMAGE_WRITER,
    LAST
  };

  typedef boost::shared_ptr<Image_writer>               Shared_image_writer;

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
  std::string* dir_name_handle(const Field_info*) { return &m_dir_name; }
  std::string* file_name_handle(const Field_info*) { return &m_file_name; }
  File_format_2d::Id* file_format_handle(const Field_info*)
  { return &m_file_format; }
  Shared_image_writer* image_writer_handle(const Field_info*)
  { return &m_image_writer; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Execute the engine.
   */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Respond to a change to a field.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  void field_changed(const Field_info* field_info);

  /*! Set the name of the dir where the snapshot is written to.
   */
  void set_dir_name(std::string dir_name);

  /*! Obtain the name of the dir where the snapshot is written to.
   */
  std::string get_dir_name();

  /*! Set the name of the file where the snapshot is written to.
   */
  void set_file_name(const std::string& file_name);

  /*! Obtain the name of the file where the snapshot is written to.
   */
  const std::string& get_file_name();

  /*! Set the file format.
   */
  void set_file_format(File_format_2d::Id format);

  /*! Obtain the file format.
   */
  File_format_2d::Id get_file_format();

  /*! Set the flag that indicates whether to generate the file name as part of
   * a sequence.
   */
  void set_sequence(Boolean flag);

  /*! Set the quality.
   */
  void set_quality(Uint quality);

  /*! Get the quality.
   */
  Uint get_quality() const;

  /*! Set the image writer.
   */
  void set_image_writer(Shared_image_writer image_writer);

  /*! Obtain the image.
   */
  Shared_image_writer get_image_writer();

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! Clean the attributes.
   */
  void clean_attributes();

  /*! Clean the image writer.
   */
  void clean_image_writer();

  /*! The image writer. */
  Shared_image_writer m_image_writer;

  /*! The directory to save the image at. */
  std::string m_dir_name;

  /*! The name of the file the image is written to. */
  std::string m_file_name;

  /*! The format of the file the image is written to. */
  File_format_2d::Id m_file_format;

  /*! The quality of the (loss) image (applicable to jpeg). */
  Uint m_quality;

  /*! Indicates whether to generate the file name  as part of a sequence. */
  Boolean m_sequence;

  /*! The sequence counter. */
  Uint m_count;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Indicates whether the image writer is dirty and thus must be cleaned. */
  Boolean m_dirty_image_writer;

  /*! Indicates whether the file name is dirty and thus must be cleaned. */
  Boolean m_dirty_file_name;

  /*! Indicates whether the directory name is dirty and thus must be cleaned. */
  Boolean m_dirty_dir_name;

  /*! Indicates whether the file format is dirty and thus must be cleaned. */
  Boolean m_dirty_file_format;

  // Default values for fields
  static const Boolean s_def_sequence;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Snapshot* Snapshot::prototype() { return new Snapshot(true); }

//! \brief clones.
inline Container* Snapshot::clone() { return new Snapshot(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Snapshot::get_tag() const { return s_tag; }

//! \brief gets the quality.
inline Uint Snapshot::get_quality() const { return m_quality; }

/*! \brief sets the flag that indicates whether to generate the file name as
 * part of a sequence.
 */
inline void Snapshot::set_sequence(Boolean flag) { m_sequence = flag; }

SGAL_END_NAMESPACE

#endif
