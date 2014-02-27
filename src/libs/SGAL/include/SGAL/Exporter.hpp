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
#include "SGAL/File_format.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Scene_graph;
class Field_info;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Exporter : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    TRIGGER,
    FILE_NAME,
    FILE_FORMAT,
    LAST
  };

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Exporter(Boolean proto = false);

  /*! Destructor. */
  virtual ~Exporter();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Exporter* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

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
  std::string* file_name_handle(const Field_info*) { return &m_file_name; }
  File_format::Id* file_format_handle(const Field_info*)
  { return &m_file_format; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Execute the engine---write the content of the scene graph to a file. */
  virtual void execute(Field_info* field_info);

  /*! Set the name of the file where the snapshot is written to.
   */
  void set_file_name(std::string& file_name);

  /*! Obtain the name of the file where the snapshot is written to.
   */
  const std::string& get_file_name();

  /*! Set the file format.
   */
  void set_file_format(File_format::Id format);

  /*! Obtain the file format.
   */
  File_format::Id get_file_format() const;

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! The name of the file the image is written to. */
  std::string m_file_name;

  /*! The format of the file the image is written to. */
  File_format::Id m_file_format;

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
  const static std::string s_def_file_name;
  const static File_format::Id s_def_file_format;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Exporter* Exporter::prototype() { return new Exporter(true); }

//! \brief clones.
inline Container* Exporter::clone() { return new Exporter(); }

inline void Exporter::set_file_name(std::string& file_name)
{ m_file_name = file_name; }

//! \brief obtains the name of the file where the snapshot is written to.
inline const std::string& Exporter::get_file_name() { return m_file_name; }

//! \brief sets the file format.
inline void Exporter::set_file_format(File_format::Id format)
{ m_file_format = format; }

//! \brief obtains the file format.
inline File_format::Id Exporter::get_file_format() const
{ return m_file_format; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Exporter::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
