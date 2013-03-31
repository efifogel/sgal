// Copyright (c) 2012 Israel.
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
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

#include "SEGO/Ego_voxels_filler_base.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"

#include <boost/lexical_cast.hpp>

SGAL_BEGIN_NAMESPACE

Container_proto* Ego_voxels_filler_base::s_prototype;

/*! Constructor */
Ego_voxels_filler_base::Ego_voxels_filler_base(Boolean proto)
    : Node(proto) {}

Ego_voxels_filler_base::~Ego_voxels_filler_base() {}

/*! \brief initilalizes the container prototype. */
void Ego_voxels_filler_base::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());
  
  // Add the field-info records to the prototype:
  s_prototype->add_field_info(new SF_int(OFFSET, "offset",
                                         get_member_offset(&m_offset)));
}

/*! Delete the container prototype. */
void Ego_voxels_filler_base::delete_prototype() {
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Ego_voxels_filler_base::get_prototype() 
{
  if (!s_prototype) Ego_voxels_filler_base::init_prototype();
  return s_prototype;
}

/*! Set the attributes. */
void Ego_voxels_filler_base::set_attributes(Element* elem) {
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);

    if (name == "offset") {
      set_offset(boost::lexical_cast<Int>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! Set the offset. */
void Ego_voxels_filler_base::set_offset(Int offset) {
  m_offset = offset;
}

/*! Get the offset. */
Int Ego_voxels_filler_base::get_offset() const {
  return m_offset;
}

  
SGAL_END_NAMESPACE
