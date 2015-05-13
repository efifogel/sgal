// Copyright (c) 2015 Israel.
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

#ifndef SGAL_IMAGE_WRITER_HPP
#define SGAL_IMAGE_WRITER_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/File_format_2d.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Field_info;
class Image;
class Formatter;

class SGAL_SGAL_DECL Image_writer : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    TRIGGER,
    IMAGE_TRIGGER,
    DIR_NAME,
    FILE_NAME,
    FILE_FORMAT,
    FLIP,
    IMAGE,
    LAST
  };

  typedef boost::shared_ptr<Image_base>         Shared_image_base;

  /*! Constructor. */
  Image_writer(Boolean proto = false);

  /*! Destructor. */
  virtual ~Image_writer();

  /*! Construct the prototype. */
  static Image_writer* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Boolean* Image_trigger_handle(const Field_info*) { return &m_image_trigger; }
  std::string* dir_name_handle(const Field_info*) { return &m_dir_name; }
  std::string* file_name_handle(const Field_info*) { return &m_file_name; }
  File_format_2d::Id* file_format_handle(const Field_info*)
  { return &m_file_format; }
  Boolean* flip_handle(const Field_info*) { return &m_flip; }
  Shared_image_base* image_base_handle(const Field_info*)
  { return &m_image; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Execute this engine.
   */
  void execute();

  /*! Trigger the execution of the engine as a response to change in one
   * of the input fields.
   */
  void trigger_changed(const Field_info* field_info);

  /*! Respond to a change in the image.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void image_changed(const Field_info* field_info);

  /*! Set the flag that indicates whether ca change to the image triggers the
   * engine.
   */
  void set_image_trigger(Boolean flag);

  /*! Determine whether ca change to the image triggers the engine.
   */
  Boolean is_image_trigger();

  /*! Set the name of the dir where the snapshot is written to.
   */
  void set_dir_name(std::string dir_name);

  /*! Obtain the name of the dir where the snapshot is written to.
   */
  std::string get_dir_name() const;

  /*! Set the name of the file where the snapshot is written to.
   */
  void set_file_name(const std::string& file_name);

  /*! Obtain the name of the file where the snapshot is written to.
   */
  const std::string& get_file_name() const;

  /*! Set the file format.
   */
  void set_file_format(File_format_2d::Id format);

  /*! Obtain the file format.
   */
  File_format_2d::Id get_file_format() const;

  /*! Set the flag that indicates whether the image should be reflected. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether the image should be reflected. */
  Boolean get_flip() const;

  /*! Set the image.
   */
  void set_image(Shared_image_base image);

  /*! Obtain the image.
   */
  Shared_image_base get_image() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The trigger of the engine, which makes the engine excute. */
  Boolean m_trigger;

  /*! Indicates whether a change to the image triggers the engine. */
  Boolean m_image_trigger;

  /*! The directory to save the image at. */
  std::string m_dir_name;

  /*! The name of the file the image is written to. */
  std::string m_file_name;

  /*! The format of the file the image is written to. */
  File_format_2d::Id m_file_format;

  /*! The quality of the (loss) image (applicable to jpeg). */
  Uint m_quality;

  /*! Indicates whether the rows are in reverse order (window system style) */
  Boolean m_flip;

  /*! The output image. */
  Shared_image_base m_image;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  // Default values:
  const static Boolean s_def_image_trigger;
  const static std::string s_def_dir_name;
  const static std::string s_def_file_name;
  const static File_format_2d::Id s_def_file_format;
  const static Uint s_def_quality;
  const static Boolean s_def_flip;
};

//! \brief constructs the prototype.
inline Image_writer* Image_writer::prototype()
{ return new Image_writer(true); }

//! \brief clones.
inline Container* Image_writer::clone()
{ return new Image_writer(); }

/*! \brief sets the flag that indicates whether ca change to the image triggers
 * the engine.
 */
inline void Image_writer::set_image_trigger(Boolean flag)
{ m_image_trigger = flag; }

//! \brief determines whether ca change to the image triggers the engine.
inline Boolean Image_writer::is_image_trigger()
{ return m_image_trigger; }

//! \brief sets the name of the dir where the snapshot is written to.
inline void Image_writer::set_dir_name(std::string dir_name)
{ m_dir_name = dir_name; }

//! \brief obtains the name of the dir where the snapshot is written to.
inline std::string Image_writer::get_dir_name() const { return m_dir_name; }

//! \brief sets the name of the file where the snapshot is written to.
inline void Image_writer::set_file_name(const std::string& file_name)
{ m_file_name = file_name; }

//! \brief obtains the name of the file where the snapshot is written to.
inline const std::string& Image_writer::get_file_name() const
{ return m_file_name; }

//! \brief sets the file format.
inline void Image_writer::set_file_format(File_format_2d::Id format)
{ m_file_format = format; }

//! \brief obtains the file format.
inline File_format_2d::Id Image_writer::get_file_format() const
{ return m_file_format; }

//! \brief sets the flag that indicates whether the image should be reflected.
inline void Image_writer::set_flip(Boolean flag) { m_flip = flag; }

/*! \brief obtains the flag that indicates whether the image should be
 * reflected.
 */
inline Boolean Image_writer::get_flip() const { return m_flip; }

//! \brief obtains the image.
inline Image_writer::Shared_image_base Image_writer::get_image() const
{ return m_image; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_writer::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
