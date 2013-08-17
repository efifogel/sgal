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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>; Boris Kozorovitzky <zbzzn@hotmail.com>

#ifndef SGAL_INDEXED_GEODESIC_SET_HPP
#define SGAL_INDEXED_GEODESIC_SET_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Color_array;
class Coord_array;
class Normal_array;
class Tex_coord_array;

class SGAL_SGAL_DECL Indexed_geodesic_set : public Geo_set {
public:
  enum {
    FIRST = Geo_set::LAST - 1,
    COLOR,
    COORD,
    TEX_COORD,
    COORD_INDEX,
    TEX_COORD_INDEX,
    NORMAL_INDEX,
    COLOR_PER_VERTEX,
    LINE_WIDTH,
    ELLIMINATE_HIDEN,
    LAST
  };

  /*! Constructor. */
  Indexed_geodesic_set(Boolean proto = false);

  /*! Destructor. */
  virtual ~Indexed_geodesic_set();

  /* Construct the prototype. */
  static Indexed_geodesic_set* prototype();

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
  //@}

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action);

  virtual void isect(Isect_action* action);

  /*! Calculate the sphere bound of the mesh. Returns true if the BS has
   * changed since lst time this was called.
   */
  virtual bool clean_sphere_bound();

  virtual void set_color_array(Shared_color_array color_array);

  /*! Set the coordinate set. Pass the pointer to the geometry object
   * used by the decoder as well.
   * \param coord_array (in) a pointer to a coord set
   */
  virtual void set_coord_array(Shared_coord_array coord_array);

  /*! Set the normal set. Pass the pointer to the geometry object
   * used by the decoder as well.
   * \param coord_array (in) a pointer to a coord set
   */
  virtual void set_normal_array(Shared_normal_array normal_array);

  /*! Set the texture coordinate set. Pass the pointer to the geometry
   * object used by the decoder as well.
   * \param tex_coord_array (in) a pointer to a coord set
   */
  virtual void set_tex_coord_array(Shared_tex_coord_array tex_coord_array);

  /* Set the flag that indicates whether normals are bound per vertex or
   * per face.
   * \param normal_per_vertex true if normals are bound per vertex
   */
  void set_normal_per_vertex(Boolean normal_per_vertex);

  Boolean get_normal_per_vertex();

  /* Set the flag that indicates whether colors are bound per vertex or
   * per face.
   * \param color_per_vertex true if normals are bound per vertex
   */
  void set_color_per_vertex(Boolean color_per_vertex);

  Boolean get_color_per_vertex();

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
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Default value */
  static const Boolean m_def_color_per_vertex;
  static const Boolean m_def_normal_per_vertex;
  static const Float s_def_line_width;
};

/* \brief constructs the prototype. */
inline Indexed_geodesic_set* Indexed_geodesic_set::prototype()
{ return new Indexed_geodesic_set(true); }

/*! \brief clones. */
inline Container* Indexed_geodesic_set::clone()
{ return new Indexed_geodesic_set(); }

/*! \brief obtains. */
inline Boolean Indexed_geodesic_set::get_normal_per_vertex()
{ return m_normal_per_vertex; }

/*! \brief obtains. */
inline Boolean Indexed_geodesic_set::get_color_per_vertex()
{ return m_color_per_vertex; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Indexed_geodesic_set::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
