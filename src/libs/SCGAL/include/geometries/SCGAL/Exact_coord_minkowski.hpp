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
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_EXACT_COORD_MINKOWSKI_HPP
#define SGAL_EXACT_COORD_MINKOWSKI_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SCGAL/Exact_kernel.hpp"

#include "SGAL/basic.hpp"
#include "SGAL/Coord_minkowski.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;

/*! Add two coord arrays */
class Exact_coord_minkowski : public Coord_minkowski
{
public:
  enum {
    FIRST = Coord_minkowski::LAST - 1,
    EXACT_EXECUTE,
    EXACT_COORD1,
    EXACT_COORD2,
    LAST
  };

  /*! Constructor */
  Exact_coord_minkowski(Boolean proto = SGAL_FALSE) : Coord_minkowski(proto) {}

  /*! Destructor */
  virtual ~Exact_coord_minkowski() {}

  /* Construct the prototype */
  static Exact_coord_minkowski * prototype()
  { return new Exact_coord_minkowski(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Exact_coord_minkowski(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */  
  virtual Container_proto * get_prototype();
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  /*! \brief transforms the input vertices */
  void execute(Field_info * field_info = NULL);
  
protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;
};

SGAL_END_NAMESPACE

#endif
