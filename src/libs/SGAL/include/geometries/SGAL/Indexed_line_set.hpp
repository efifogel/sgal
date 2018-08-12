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

#ifndef SGAL_INDEXED_LINE_SET_HPP
#define SGAL_INDEXED_LINE_SET_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Lines_set.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Color_array;
class Coord_array;
class Normal_array;
class Tex_coord_array;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Indexed_line_set : public Lines_set {
public:
  enum {
    FIRST = Lines_set::LAST - 1,
    COLOR_PER_VERTEX,
    NORMAL_PER_VERTEX,
    LINE_WIDTH,
    ELLIMINATE_HIDEN,
    LAST
  };

  // Drawing & representation:

#define SGAL_LI_FRAG_SOURCE_        0
#define SGAL_LI_FRAG_SOURCE         0x00000001
#define SGAL_LI_FRAG_INDEXED_       1
#define SGAL_LI_FRAG_INDEXED        0x00000002
#define SGAL_LI_FRAG_ATTACHMENT_    2
#define SGAL_LI_FRAG_ATTACHMENT     0x0000000c
#define SGAL_LI_TEXTURE_ENABLED_    4
#define SGAL_LI_TEXTURE_ENABLED     0x00000010
#define SGAL_LI_TEXTURE_INDEXED_    5
#define SGAL_LI_TEXTURE_INDEXED     0x00000020
#define SGAL_LI_PRIM_TYPE_          6
#define SGAL_LI_PRIM_TYPE           0x000000c0
#define SGAL_LI_VERTEX_ARRAY_       8
#define SGAL_LI_VERTEX_ARRAY        0x00000100
#define SGAL_NUM_LI_DRAWS           0x200

  /*! Constructor. */
  Indexed_line_set(Boolean proto = false);

  /*! Destructor. */
  virtual ~Indexed_line_set();

  /* Construct the prototype.
   */
  static Indexed_line_set* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the prototype. */
  virtual void delete_prototype();

  /*! Obtain the prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Float* line_width_handle(const Field_info*) { return &m_line_width; }
  //@}

  /*! Draw the geometry.
   * For efficiency reasons, differenrt methods were written to
   * draw geometries with different kinds of data (texture/normal/color).
   * \param action action.
   */
  virtual void draw(Draw_action* action);

  virtual void isect(Isect_action* action);

  /*! Clean the sphere bound of the lines. */
  virtual void clean_bounding_sphere();

  /*! Obtain the coordinate array.
   * \return the coordinate array.
   */
  virtual Shared_coord_array get_coord_array();

  /*! Clean the coordinate array.
   */
  virtual void clean_coords();

  /// \name Change Recators
  //@{
  /*! Respond to a change in the coordinate array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void coord_content_changed(const Field_info* field_info = nullptr);

  //! \todo add missing reactors.
  //@}

  /// \name Indices Change Reactors
  //@{
  /*! Respond to a change in the coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the normal indices.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void normal_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the color indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void color_indices_changed(const Field_info* field_info = nullptr);

  /*! Respond to a change in the texture-coordinate indices.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void tex_coord_indices_changed(const Field_info* field_info = nullptr);
  //@}

  /* Set the flag that indicates whether normals are bound per vertex
   * or per line.
   */
  void set_normal_per_vertex(Boolean normal_per_vertex);

  /* Obtain the flag that indicates whether normals are bound per vertex
   * or per line.
   */
  Boolean get_normal_per_vertex() const;

  /* Set the flag that indicates whether colors are bound per vertex or
   * per line.
   */
  void set_color_per_vertex(Boolean color_per_vertex);

  /*! Obtain the flag that indicates the colors are bound per vertex
   * or per line.
   */
  Boolean get_color_per_vertex() const;

  /*! Set the line width.
   * \param width (in) The line width.
   */
  void set_line_width(Float width);

  /*! Obtain the line width.
   * \param width (in) The line width.
   */
  Float get_line_width();

  /*! Draw the representation.
   * \param action action.
   */
  virtual void draw_lines(Draw_action* action);

protected:
  // Draw routines:
  void draw_invalid();

  // Lines (general):
  // Texture disabled:
  void draw_FANO_TENO_MOLI_VANO();

  void draw_FSNO_FINO_FAPV_TENO_MOLI_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOLI_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOLI_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOLI_VANO();

  void draw_FSNO_FINO_FAPL_TENO_MOLI_VANO();
  void draw_FSCO_FINO_FAPL_TENO_MOLI_VANO();
  void draw_FSNO_FIYE_FAPL_TENO_MOLI_VANO();
  void draw_FSCO_FIYE_FAPL_TENO_MOLI_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOLI_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOLI_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOLI_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOLI_VANO(); invalid

  // Texture enabled:
  void draw_FANO_TEYE_TINO_MOLI_VANO();

  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOLI_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOLI_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLI_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLI_VANO();

  void draw_FSNO_FINO_FAPL_TEYE_TINO_MOLI_VANO();
  void draw_FSCO_FINO_FAPL_TEYE_TINO_MOLI_VANO();
  void draw_FSNO_FIYE_FAPL_TEYE_TINO_MOLI_VANO();
  void draw_FSCO_FIYE_FAPL_TEYE_TINO_MOLI_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOLI_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOLI_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLI_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLI_VANO(); invalid

  void draw_FANO_TEYE_TIYE_MOLI_VANO();

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLI_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLI_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLI_VANO();

  void draw_FSNO_FINO_FAPL_TEYE_TIYE_MOLI_VANO();
  void draw_FSCO_FINO_FAPL_TEYE_TIYE_MOLI_VANO();
  void draw_FSNO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO();
  void draw_FSCO_FIYE_FAPL_TEYE_TIYE_MOLI_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLI_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLI_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLI_VANO(); invalid

  // Line strips:
  // Texture disabled:
  void draw_FANO_TENO_MOLS_VANO();

  void draw_FSNO_FINO_FAPV_TENO_MOLS_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOLS_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOLS_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOLS_VANO();

  void draw_FSNO_FINO_FAPL_TENO_MOLS_VANO();
  void draw_FSCO_FINO_FAPL_TENO_MOLS_VANO();
  void draw_FSNO_FIYE_FAPL_TENO_MOLS_VANO();
  void draw_FSCO_FIYE_FAPL_TENO_MOLS_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOLS_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOLS_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOLS_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOLS_VANO(); invalid

  // Texture enabled:
  void draw_FANO_TEYE_TINO_MOLS_VANO();

  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOLS_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOLS_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLS_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLS_VANO();

  void draw_FSNO_FINO_FAPL_TEYE_TINO_MOLS_VANO();
  void draw_FSCO_FINO_FAPL_TEYE_TINO_MOLS_VANO();
  void draw_FSNO_FIYE_FAPL_TEYE_TINO_MOLS_VANO();
  void draw_FSCO_FIYE_FAPL_TEYE_TINO_MOLS_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOLS_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOLS_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLS_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLS_VANO(); invalid

  void draw_FANO_TEYE_TIYE_MOLS_VANO();

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLS_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLS_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLS_VANO();

  void draw_FSNO_FINO_FAPL_TEYE_TIYE_MOLS_VANO();
  void draw_FSCO_FINO_FAPL_TEYE_TIYE_MOLS_VANO();
  void draw_FSNO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO();
  void draw_FSCO_FIYE_FAPL_TEYE_TIYE_MOLS_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLS_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLS_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLS_VANO(); invalid

  // Line loops:
  // Texture disabled:
  void draw_FANO_TENO_MOLL_VANO();

  void draw_FSNO_FINO_FAPV_TENO_MOLL_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOLL_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOLL_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOLL_VANO();

  void draw_FSNO_FINO_FAPL_TENO_MOLL_VANO();
  void draw_FSCO_FINO_FAPL_TENO_MOLL_VANO();
  void draw_FSNO_FIYE_FAPL_TENO_MOLL_VANO();
  void draw_FSCO_FIYE_FAPL_TENO_MOLL_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOLL_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOLL_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOLL_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOLL_VANO(); invalid

  // Texture enabled:
  void draw_FANO_TEYE_TINO_MOLL_VANO();

  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOLL_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOLL_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOLL_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOLL_VANO();

  void draw_FSNO_FINO_FAPL_TEYE_TINO_MOLL_VANO();
  void draw_FSCO_FINO_FAPL_TEYE_TINO_MOLL_VANO();
  void draw_FSNO_FIYE_FAPL_TEYE_TINO_MOLL_VANO();
  void draw_FSCO_FIYE_FAPL_TEYE_TINO_MOLL_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOLL_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOLL_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOLL_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOLL_VANO(); invalid

  void draw_FANO_TEYE_TIYE_MOLL_VANO();

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOLL_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOLL_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOLL_VANO();

  void draw_FSNO_FINO_FAPL_TEYE_TIYE_MOLL_VANO();
  void draw_FSCO_FINO_FAPL_TEYE_TIYE_MOLL_VANO();
  void draw_FSNO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO();
  void draw_FSCO_FIYE_FAPL_TEYE_TIYE_MOLL_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOLL_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOLL_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOLL_VANO(); invalid

  // Vertex Buffer:
  void draw_FAPV_VAYE();
  void draw_FAPL_VAYE();
  void draw_FAPM_VAYE();

  /*! Indicates whether a single color is specified per vertex. */
  Boolean m_color_per_vertex;

  /*! Indicates whether a single normal is specified per vertex. */
  Boolean m_normal_per_vertex;

  /*! The width of the dual lines. */
  Float m_line_width;

  /*! Indicates whether to elliminate hiden lines using depth test. */
  Boolean m_elliminate_hiden;

  /*! Indicates whether the mesh is textured. */
  Boolean m_has_texture;

  /* Indicates that the bbox is set externally. */
  Boolean m_bb_is_pre_set;

  /*! */
  Boolean m_use_display_list;

  /*! */
  Int32 m_display_list_id;

  static void (Indexed_line_set::*m_draws[SGAL_NUM_LI_DRAWS])();
  static Boolean m_draws_initialized;

  //! Indicates whether the coordinate array has beeen invalidated.
  Boolean m_dirty_coord_array;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default value. */
  static const Boolean m_def_color_per_vertex;
  static const Boolean m_def_normal_per_vertex;
  static const Float s_def_line_width;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Indexed_line_set* Indexed_line_set::prototype()
{ return new Indexed_line_set(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Indexed_line_set::create()
{ return new Indexed_line_set(); }

/*! \brief obtains the flag that indicates the normals are bound per vertex
 * or per line.
 */
inline Boolean Indexed_line_set::get_normal_per_vertex() const
{ return m_normal_per_vertex; }

/*! \brief obtains the flag that indicates the colors are bound per vertex
 * or per line.
 */
inline Boolean Indexed_line_set::get_color_per_vertex() const
{ return m_color_per_vertex; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Indexed_line_set::get_tag() const { return s_tag; }

//! \brief sets the line width.
inline void Indexed_line_set::set_line_width(Float width)
{ m_line_width = width; }

//! \brief obtains the line width.
inline Float Indexed_line_set::get_line_width()
{ return m_line_width; }

SGAL_END_NAMESPACE

#endif
