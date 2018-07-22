// Copyright (c) 2016 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DXF_CONFIGURATION_HPP
#define SGAL_DXF_CONFIGURATION_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;

class SGAL_SGAL_DECL Dxf_configuration : public Container {
  friend class Dxf_option_parser;

public:

  enum {
    FIRST = Container::LAST - 1,
    PALETTE_FILE_NAME,
    LAST
  };

  /*! Construct. */
  Dxf_configuration(Boolean proto = false);

  /*! Destruct. */
  virtual ~Dxf_configuration() {}

  /*! Construct the prototype. */
  static Dxf_configuration* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  String* palette_file_name_handle(const Field_info*)
  { return &m_palette_file_name; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Set the palette filename.
   * \param[in] filename the palette filename.
   */
  void set_palette_file_name(const String& filename);

  /*! Obtain the palette filename.
   * \return the palette filename.
   */
  const String& get_palette_file_name() const;

  /*! Set defualt values. */
  void reset(const String& def_palette_file_name = s_def_palette_file_name);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! The palette file-name.
  String m_palette_file_name;

  // default values
  static const String s_def_palette_file_name;
};

//! \brief constructs the prototype.
inline Dxf_configuration* Dxf_configuration::prototype()
{ return new Dxf_configuration(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Dxf_configuration::create()
{ return new Dxf_configuration(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Dxf_configuration::get_tag() const { return s_tag; }

//! \brief sets the palette file-name.
inline void Dxf_configuration::set_palette_file_name(const String& filename)
{ m_palette_file_name = filename; }

//! \brief obtains the palette file-name.
inline const String& Dxf_configuration::get_palette_file_name() const
{ return m_palette_file_name; }

SGAL_END_NAMESPACE

#endif
