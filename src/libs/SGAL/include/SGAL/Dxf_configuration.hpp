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
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;

class SGAL_SGAL_DECL Dxf_configuration : public Container {
  friend class Dxf_option_parser;

public:

  enum {
    FIRST = Container::LAST - 1,
    PALETTE_FILE_NAME,
    BACKGROUND_COLOR,
    MIN_BULGE,
    REFINMENT_ARCS_NUM,
    STORE_DATA,
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
  Vector4f* background_color_handle(const Field_info*)
  { return &m_background_color; }
  Float* min_bulge_handle(const Field_info*) { return &m_min_bulge; }
  Uint* refinement_arcs_num_handle(const Field_info*)
  { return &m_refinement_arcs_num; }
  Boolean* store_data_handle(const Field_info*) { return &m_store_data; }
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

  /*! Set the background color.
   */
  void set_background_color(const Vector4f& color);

  /*! Obatin the background color.
   */
  const Vector4f& get_background_color() const;

  /*! Set the min-bulge value.
   */
  void set_min_bulge(Float min_bulge);

  /*! Obatin the min-bulge value.
   */
  Float get_min_bulge() const;

  /*! Set the efinement arcs number.
   */
  void set_refinement_arcs_num(Uint num);

  /*! Obatin the efinement arcs number.
   */
  Uint get_refinement_arcs_num() const;

  /*! Set the flag that indicates whether to store the dxf data.
   */
  void set_store_data(Boolean flag);

  /*! Obatin the flag that indicates whether to store the dxf data.
   */
  Boolean get_store_data() const;

  /*! Set defualt values. */
  void reset(const String& def_palette_file_name = s_def_palette_file_name,
             const Vector4f& def_background_color = s_def_background_color,
             const Float def_min_bulge = s_def_min_bulge,
             const Uint def_refinement_arcs_num = s_def_refinement_arcs_num,
             const Boolean def_store_data = s_def_store_data);

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

  //! The default background color.
  Vector4f m_background_color;

  //! The minimum bulge value.
  Float m_min_bulge;

  //! The number of arcs used to represent a circle or an ellipsoid.
  Uint m_refinement_arcs_num;

  //! Indicates whether to store the dxf data for later use.
  Boolean m_store_data;

  // default values
  static const String s_def_palette_file_name;
  static const Vector4f s_def_background_color;
  static const Float s_def_min_bulge;
  static const Uint s_def_refinement_arcs_num;
  static const Boolean s_def_store_data;
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

//! \brief sets the background color.
inline void Dxf_configuration::set_background_color(const Vector4f& color)
{ m_background_color = color; }

//! \brief gets the background color.
inline const Vector4f& Dxf_configuration::get_background_color() const
{ return m_background_color; }

//! \brief sets the min-bulge value.
inline void Dxf_configuration::set_min_bulge(Float min_bulge)
{ m_min_bulge = min_bulge; }

//! \brief obtains the min-bulge value.
inline Float Dxf_configuration::get_min_bulge() const { return m_min_bulge; }

//! \brief sets the efinement arcs number.
inline void Dxf_configuration::set_refinement_arcs_num(Uint num)
{ m_refinement_arcs_num = num; }

//! \brief obtains the efinement arcs number.
inline Uint Dxf_configuration::get_refinement_arcs_num() const
{ return m_refinement_arcs_num; }

//! \brief sets the flag that indicates whether to store the dxf data.
inline void Dxf_configuration::set_store_data(Boolean flag)
{ m_store_data = flag; }

//! \brief obatins the flag that indicates whether to store the dxf data.
inline Boolean Dxf_configuration::get_store_data() const
{ return m_store_data; }

SGAL_END_NAMESPACE

#endif
