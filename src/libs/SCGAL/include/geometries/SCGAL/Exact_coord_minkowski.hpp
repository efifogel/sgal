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
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EXACT_COORD_MINKOWSKI_HPP
#define SCGAL_EXACT_COORD_MINKOWSKI_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_minkowski.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;

/*! Add two coord arrays. */
class SGAL_SCGAL_DECL Exact_coord_minkowski : public Coord_minkowski {
public:
  enum {
    FIRST = Coord_minkowski::LAST - 1,
    EXACT_EXECUTE,
    EXACT_COORD1,
    EXACT_COORD2,
    LAST
  };

  /*! Constructor. */
  Exact_coord_minkowski(Boolean proto = false) : Coord_minkowski(proto) {}

  /*! Destructor. */
  virtual ~Exact_coord_minkowski() {}

  /* Construct the prototype. */
  static Exact_coord_minkowski* prototype();

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
  Boolean* execute_handle(Field_info*) { return &m_execute; }
  // shared_container &m_coord_array1;
  // shared_container &m_coord_array2;
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Transform the input vertices and store the results in the output
   * vertices.
   * \param field_info the field information record.
   */
  void execute(Field_info* field_info = NULL);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
};

/* \brief constructs the prototype. */
inline Exact_coord_minkowski* Exact_coord_minkowski::prototype()
{ return new Exact_coord_minkowski(true); }

/*! \brief clones. */
inline Container* Exact_coord_minkowski::clone()
{ return new Exact_coord_minkowski(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Exact_coord_minkowski::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
