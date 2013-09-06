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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_PIECE_HPP
#define SGAL_PIECE_HPP

/*! \file
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

class Formatter;

class SGAL_SGAL_DECL Piece : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    UNIT_SIZE,
    LAST
  };

  /*! Constructor */
  Piece(Boolean proto = false);

  /*! Destructor */
  virtual ~Piece();

  /* Construct the prototype. */
  static Piece* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Uint* unit_size_handle(Field_info*) { return &m_unit_size; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Write this container. */
  virtual void write(Formatter* formatter);

  /*! Clean the representation. */
  virtual void clean();

  /*! Clean the coordinate index array. */
  virtual void clean_coord_indices();

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
  const Array<Uint>& get_composition() const;

  /*! Obtain the composition. */
  Array<Uint>& get_composition();

  /*! Process change of structure. */
  void structure_changed(Field_info* field_info);

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
  Array<Uint> m_composition;

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

/* \brief constructs the prototype. */
inline Piece* Piece::prototype() { return new Piece(true); }

/*! \brief clones. */
inline Container* Piece::clone() { return new Piece(); }

/*! \brief sets the width. */
inline void Piece::set_width(Uint width) { m_width = width; }

/*! \brief obtains the width. */
inline Uint Piece::get_width() const { return m_width; }

/*! \brief sets the height. */
inline void Piece::set_height(Uint height) { m_height = height; }

/*! \brief obtains the height. */
inline Uint Piece::get_height() const { return m_height; }

/*! \brief sets the depth. */
inline void Piece::set_depth(Uint depth) { m_depth = depth; }

/*! \brief obtains the depth. */
inline Uint Piece::get_depth() const { return m_depth; }

/*! \brief obtains the composition. */
inline const Array<Uint>& Piece::get_composition() const
{ return m_composition; }

/*! \brief obtains the composition. */
inline Array<Uint>& Piece::get_composition() { return m_composition; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Piece::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
