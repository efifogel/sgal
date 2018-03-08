// Copyright (c) 2014 Israel.
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

#ifndef SGAL_EXPORTER_HPP
#define SGAL_EXPORTER_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/File_format_3d.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Scene_graph;
class Field_info;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Exporter : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    TRIGGER,
    DIR_NAME,
    FILE_NAME,
    FILE_FORMAT,
    SEPARATE,
    LAST
  };

  /*! Constructor.
   * \param[in] proto determines whether to construct a prototype.
   */
  Exporter(Boolean proto = false);

  /*! Destructor. */
  virtual ~Exporter();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Exporter* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype.
   */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  std::string* dir_name_handle(const Field_info*) { return &m_dir_name; }
  std::string* file_name_handle(const Field_info*) { return &m_file_name; }
  File_format_3d::Id* file_format_handle(const Field_info*)
  { return &m_file_format; }
  Boolean* separate_handle(const Field_info*) { return &m_separate; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for the exporting, e.g., VRML.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Execute the engine---export the content of the scene graph. */
  virtual void execute(const Field_info* field_info);

  /*! Set the name of the dir where the snapshot is written to.
   */
  void set_dir_name(std::string dir_name);

  /*! Obtain the name of the dir where the snapshot is written to.
   */
  std::string get_dir_name() const;

  /*! Set the name of the file where the snapshot is written to.
   */
  void set_file_name(std::string& file_name);

  /*! Obtain the name of the file where the snapshot is written to.
   */
  const std::string& get_file_name() const;

  /*! Set the file format.
   */
  void set_file_format(File_format_3d::Id format);

  /*! Obtain the file format.
   */
  File_format_3d::Id get_file_format() const;

  /*! Indicates whether to save different geometries in separate files. */
  Boolean m_separate;

  /*! The sequence counter. */
  Uint m_count;

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! The directory to save the model at. */
  std::string m_dir_name;

  /*! The name of the file the image is written to. */
  std::string m_file_name;

  /*! The format of the file the image is written to. */
  File_format_3d::Id m_file_format;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The scene graph. */
  Scene_graph* m_scene_graph;

  /*! The trigger. */
  Boolean m_trigger;

  // Default values for fields
  const static std::string s_def_dir_name;
  const static std::string s_def_file_name;
  const static File_format_3d::Id s_def_file_format;
  const static Boolean s_def_separate;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Exporter* Exporter::prototype() { return new Exporter(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Exporter::create()
{ return new Exporter(); }

//! \brief sets the name of the dir where the snapshot is written to.
inline void Exporter::set_dir_name(std::string dir_name)
{ m_dir_name = dir_name; }

//! \brief obtains the name of the dir where the snapshot is written to.
inline std::string Exporter::get_dir_name() const { return m_dir_name; }

inline void Exporter::set_file_name(std::string& file_name)
{ m_file_name = file_name; }

//! \brief obtains the name of the file where the snapshot is written to.
inline const std::string& Exporter::get_file_name() const
{ return m_file_name; }

//! \brief sets the file format.
inline void Exporter::set_file_format(File_format_3d::Id format)
{ m_file_format = format; }

//! \brief obtains the file format.
inline File_format_3d::Id Exporter::get_file_format() const
{ return m_file_format; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Exporter::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
