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
// $Revision: 9185 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_INDEXED_LINE_SET_HPP
#define SGAL_INDEXED_LINE_SET_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"

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

class SGAL_SGAL_DECL Indexed_line_set : public Geo_set {
public:
  enum {
    FIRST = Geo_set::LAST - 1,
    COLOR_PER_VERTEX,
    NORMAL_PER_VERTEX,
    LINE_WIDTH,
    ELLIMINATE_HIDEN,
    LAST
  };

  /*! Constructor */
  Indexed_line_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Indexed_line_set();

  /* Construct the prototype. */
  static Indexed_line_set* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the prototype. */
  virtual void delete_prototype();

  /*! Obtain the prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* line_width_handle(Field_info*) { return &m_line_width; }
  //@}

  /*! Draw the geometry.
   * For efficiency reasons, differenrt methods were written to
   * draw geometries with different kinds of data (texture/normal/color).
   * \param action action.
   */
  virtual void draw(Draw_action* action);

  virtual void isect(Isect_action* action);

  /*! Calculate the sphere bound of the mesh. Returns true if the BS has
   * changed since lst time this was called.
   */
  virtual Boolean clean_sphere_bound();

  /* Set the flag that indicates whether normals are bound per vertex
   * or per line.
   */
  void set_normal_per_vertex(Boolean normal_per_vertex);

  Boolean get_normal_per_vertex() const;

  /* Set the flag that indicates whether colors are bound per vertex or
   * per line.
   */
  void set_color_per_vertex(Boolean color_per_vertex);

  Boolean get_color_per_vertex() const;

protected:
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
  Int m_display_list_id;

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

/* \brief constructs the prototype. */
inline Indexed_line_set* Indexed_line_set::prototype()
{ return new Indexed_line_set(true); }

/*! \brief clones. */
inline Container* Indexed_line_set::clone()
{ return new Indexed_line_set(); }

/*! \brief */
inline Boolean Indexed_line_set::get_normal_per_vertex() const
{ return m_normal_per_vertex; }

/*! \brief */
inline Boolean Indexed_line_set::get_color_per_vertex() const
{ return m_color_per_vertex; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Indexed_line_set::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
