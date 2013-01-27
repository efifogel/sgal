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

#ifndef SGAL_SNAPSHOT_HPP
#define SGAL_SNAPSHOT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

class Local_stream;
class Container_proto;
class Element;
class Scene_graph;
class ofstream;

class SGAL_CLASSDEF Snapshot : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    TRIGGER,
    DIR_NAME,
    FILE_NAME,
    FILE_FORMAT,
    LAST
  };

  /*! Constructor */
  Snapshot(Boolean proto = false);

  /*! Destructor */
  virtual ~Snapshot();

  /*! Construct the prototype */
  static Snapshot* prototype() { return new Snapshot(true); }

  /*! Clone */
  virtual Container* clone() { return new Snapshot(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Trigger the snapshot */
  void trigger() { m_triggered = true; }

  // virtual Attribute_list get_attributes();

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
  
  void set_trigger(Field_info* field_info = NULL  );
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /* Set the name of the dir where the snapshot is written to */
  void set_dir_name(std::string dir_name) { m_dir_name = dir_name; }

  /* Obtain the name of the dir where the snapshot is written to */
  std::string get_dir_name() { return m_dir_name; }

  /* Set the name of the file where the snapshot is written to */
  void set_file_name(std::string& file_name) { m_file_name = file_name; }

  /* Obtain the name of the file where the snapshot is written to */
  const std::string& get_file_name() { return m_file_name; }

  /*! Set the file format */
  void set_file_format(File_format format) { m_file_format = format; }

  /*! Obtain the file format */
  File_format get_file_format() const { return m_file_format; }

  /*! Set the image */
  void set_image(Image* image) { m_image = image; }
  
  /*! Obtain the image */
  Image* get_image() const { return m_image; }
  
protected: 
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! A place holder for the image */
  Image* m_image;

  /*! The directory to save the image at */
  std::string m_dir_name;
  
  /*! The name of the file the image is written to */
  std::string m_file_name;

  /*! The format of the file the image is written to */
  File_format m_file_format;

  /*! The quality of the (loss) image (applicable to jpeg) */
  Int m_quality;

  /*! Indicates whether to generate the file name  as part of a sequence */
  Boolean m_sequence;

  /*! The sequence counter */
  Uint m_count;
  
protected:
  /*! File format names */
  static const char* s_file_format_names[];
  
private: 
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  Boolean m_triggered;

  Boolean m_use_front_buffer;

  Uint m_size;

  /*! Indicates whether the rows are in reverse order (window system style) */
  Boolean m_flip;
  
  // Default values for fields
  const static std::string s_def_dir_name;
  const static std::string s_def_file_name;
  const static File_format s_def_file_format;
  const static Int s_def_quality;

  /*! Allocate space for the image */
  Boolean allocate_space(Draw_action* action);
  
  /*! Take a snapshot of the window */
  void take_snapshot();

  /*! Open a file and writes the image into it */
  virtual void write_image();
};

SGAL_END_NAMESPACE

#endif
