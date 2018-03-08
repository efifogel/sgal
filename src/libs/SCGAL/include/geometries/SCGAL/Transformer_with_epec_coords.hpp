// Copyright (c) 2014 Israel.
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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_TRANSFORMER_WITH_EXACT_COORDS_HPP
#define SCGAL_TRANSFORMER_WITH_EXACT_COORDS_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Coord_transformer.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class SGAL_SCGAL_DECL Transformer_with_epec_coords : public Coord_transformer {
public:
  enum {
    FIRST = Coord_transformer::LAST - 1,
    LAST
  };

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Transformer_with_epec_coords(Boolean proto = false);

  /*! Destructor. */
  virtual ~Transformer_with_epec_coords();

  /* Construct the prototype. */
  static Transformer_with_epec_coords* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node.
   * \param elem (in) lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  /*! Apply the transformation. */
  virtual void apply();

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;
};

//! \brief constructor.
inline Transformer_with_epec_coords::
Transformer_with_epec_coords(Boolean proto) : Coord_transformer(proto) {}

//! \brief destructor.
inline Transformer_with_epec_coords::~Transformer_with_epec_coords() {}

//! \brief constructs the prototype.
inline Transformer_with_epec_coords* Transformer_with_epec_coords::prototype()
{ return new Transformer_with_epec_coords(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Transformer_with_epec_coords::create()
{ return new Transformer_with_epec_coords(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Transformer_with_epec_coords::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
