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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <string>
#include <utility>
#include <list>
#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief adds a string attribute. */
void Element::add_attribute(Str_attr* attribute, bool back)
{
  if (back) m_str_attrs.push_back(attribute);
  else  m_str_attrs.push_front(attribute);
}

/*! \brief adds a container attribute. */
void Element::add_attribute(Cont_attr* attribute, bool back)
{
  if (back) m_cont_attrs.push_back(attribute);
  else  m_cont_attrs.push_front(attribute);
}

/*! \brief adds a container-list attribute. */
void Element::add_attribute(Multi_cont_attr* attribute, bool back)
{
  if (back) m_multi_cont_attrs.push_back(attribute);
  else  m_multi_cont_attrs.push_front(attribute);
}

/*! \brief adds a field attribute. */
void Element::add_attribute(Field_attr* attribute, bool back)
{
  if (back) m_field_attrs.push_back(attribute);
  else  m_field_attrs.push_front(attribute);
}

/*! \brief deletes the string-attribute pointed by a given iterator. */
void Element::mark_delete(Str_attr_iter ai)
{
  Str_attr* attr = *ai;
  delete attr->first;
  delete attr->second;
  delete attr;
  *ai = 0;
}

/*! \brief deletes the container-attribute pointed by a given iterator. */
void Element::mark_delete(Cont_attr_iter ai)
{
  Cont_attr* attr = *ai;
  delete attr->first;
  delete attr;
  *ai = 0;
}

/*! \brief deletes the multi-container-attribute pointed by a given iterator. */
void Element::mark_delete(Multi_cont_attr_iter ai)
{
  Multi_cont_attr* attr = *ai;
  delete attr->first;
  delete attr;
  *ai = 0;
}

/*! \brief deletes all attributes. */
void Element::delete_marked()
{
  m_str_attrs.erase(std::remove_if(m_str_attrs.begin(), m_str_attrs.end(),
                                   Str_eraser()), m_str_attrs.end());
  m_cont_attrs.erase(std::remove_if(m_cont_attrs.begin(), m_cont_attrs.end(),
                                    Cont_eraser()), m_cont_attrs.end());
  m_multi_cont_attrs.erase(std::remove_if(m_multi_cont_attrs.begin(),
                                          m_multi_cont_attrs.end(),
                                          Multi_cont_eraser()),
                           m_multi_cont_attrs.end());
  m_field_attrs.erase(std::remove_if(m_field_attrs.begin(), m_field_attrs.end(),
                                     Field_eraser()), m_field_attrs.end());
}

SGAL_END_NAMESPACE
