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

#ifndef SGAL_PIECE_HPP
#define SGAL_PIECE_HPP

/*! \file
 */

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

class Formatter;

#if defined(_MSC_VER)
template class SGAL_SGAL_DECL std::allocator<Uint>;
template class SGAL_SGAL_DECL std::vector<Uint>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Piece : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    UNIT_SIZE,
    WIDTH,
    HEIGHT,
    DEPTH,
    LAST
  };

  /*! Constructor */
  Piece(Boolean proto = false);

  /*! Destructor */
  virtual ~Piece();

  /* Construct the prototype. */
  static Piece* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Uint* unit_size_handle(const Field_info*) { return &m_unit_size; }
  Uint* width_handle(const Field_info*) { return &m_width; }
  Uint* height_handle(const Field_info*) { return &m_height; }
  Uint* depth_handle(const Field_info*) { return &m_depth; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the coordinate array. */
  virtual void clean_coords();

  /*! Clean the (flat) coordinate index array. */
  virtual void clean_facet_coord_indices();

  /*! Set the width. */
  void set_width(Uint width);

  /*! Obtain the width. */
  Uint get_width() const;

  /*! Set the height. */
  void set_height(Uint height);

  /*! Obtain the height. */
  Uint get_height() const;

  /*! Set the depth. */
  void set_depth(Uint depth);

  /*! Obtain the depth. */
  Uint get_depth() const;

  /*! Obtain the composition. */
  const std::vector<Uint>& get_composition() const;

  /*! Obtain the composition. */
  std::vector<Uint>& get_composition();

  /*! Process change of structure. */
  void structure_changed(const Field_info* field_info);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The piece unit size. */
  Uint m_unit_size;

  /*! The piece width. */
  Uint m_width;

  /*! The piece height. */
  Uint m_height;

  /*! The piece depth. */
  Uint m_depth;

  /*! The piece composition array. */
  std::vector<Uint> m_composition;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! default values for Piece. */
  static const Uint s_def_width;
  static const Uint s_def_height;
  static const Uint s_def_depth;
  static const Uint s_def_unit_size;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Piece* Piece::prototype() { return new Piece(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Piece::create() { return new Piece(); }

//! \brief obtains the width.
inline Uint Piece::get_width() const { return m_width; }

//! \brief obtains the height.
inline Uint Piece::get_height() const { return m_height; }

//! \brief obtains the depth.
inline Uint Piece::get_depth() const { return m_depth; }

//! \brief obtains the composition.
inline const std::vector<Uint>& Piece::get_composition() const
{ return m_composition; }

//! \brief obtains the composition.
inline std::vector<Uint>& Piece::get_composition() { return m_composition; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Piece::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
