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
// $Source$
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_PIECE_HPP
#define SGAL_PIECE_HPP

/*! \file
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

class Formatter;

class Piece : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    UNIT_SIZE,
    LAST
  };

  /*! Constructor */
  Piece(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Piece();

  /* Construct the prototype */
  static Piece * prototype() { return new Piece(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Piece(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto * get_prototype(); 
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Write this container */
  virtual void write(Formatter * formatter);

  /*! Clean the representation */
  virtual void clean();

  /*! Set the width */
  void set_width(Uint width) { m_width = width; }

  /*! Obtain the width */
  Uint get_width() const { return m_width; }

  /*! Set the height */
  void set_height(Uint height) { m_height = height; }

  /*! Obtain the height */
  Uint get_height() const { return m_height; }
  
  /*! Set the depth */
  void set_depth(Uint depth) { m_depth = depth; }

  /*! Obtain the depth */
  Uint get_depth() const { return m_depth; }

  /*! Obtain the composition */
  const Array<Uint> & get_composition() const { return m_composition; }

  /*! Obtain the composition */
  Array<Uint> & get_composition() { return m_composition; }
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

  /*! The piece unit size */
  Uint m_unit_size;

  /*! The piece width */
  Uint m_width;

  /*! The piece height */
  Uint m_height;
  
  /*! The piece depth */
  Uint m_depth;

  /*! The piece composition array */
  Array<Uint> m_composition;
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! default values for Piece */
  static const Uint s_def_width;
  static const Uint s_def_height;
  static const Uint s_def_depth;
  static const Uint s_def_unit_size;
};

SGAL_END_NAMESPACE

#endif
