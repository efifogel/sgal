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

//! \brief adds a string attribute.
void Element::add_attribute(Str_attr& attribute, bool back)
{
  if (back) m_str_attrs.push_back(attribute);
  else m_str_attrs.push_front(attribute);
}

//! \brief adds a string-list attribute.
void Element::add_attribute(Multi_str_attr& attribute, bool back)
{
  if (back) m_multi_str_attrs.push_back(attribute);
  else m_multi_str_attrs.push_front(attribute);
}

//! \brief adds a container attribute.
void Element::add_attribute(Cont_attr& attribute, bool back)
{
  if (back) m_cont_attrs.push_back(attribute);
  else m_cont_attrs.push_front(attribute);
}

//! \brief adds a container-list attribute.
void Element::add_attribute(Multi_cont_attr& attribute, bool back)
{
  if (back) m_multi_cont_attrs.push_back(attribute);
  else m_multi_cont_attrs.push_front(attribute);
}

//! \brief adds a field attribute.
void Element::add_attribute(Field_attr& attribute, bool back)
{
  if (back) m_field_attrs.push_back(attribute);
  else m_field_attrs.push_front(attribute);
}

//! \brief adds a multi-string field attribute.
void Element::add_attribute(Field_multi_str_attr& attribute, bool back)
{
  if (back) m_field_multi_str_attrs.push_back(attribute);
  else m_field_multi_str_attrs.push_front(attribute);
}

//! \brief deletes the string-attribute pointed by a given iterator.
void Element::mark_delete(Str_attr_iter ai)
{
  ai->first.reset();
  ai->second.reset();
}

//! \brief deletes the attribute pointed by a given iterator.
void Element::mark_delete(Multi_str_attr_iter ai) { ai->first.reset(); }

//! \brief deletes the container-attribute pointed by a given iterator.
void Element::mark_delete(Cont_attr_iter ai) { ai->first.reset(); }

//! \brief deletes the multi-container-attribute pointed by a given iterator.
void Element::mark_delete(Multi_cont_attr_iter ai) { ai->first.reset(); }

//! \brief deletes the Field-attribute pointed by a given iterator.
void Element::mark_delete(Field_attr_iter ai)
{
  ai->first.reset();
  auto value = std::get<2>(ai->second);
  ai->second = std::make_tuple(static_cast<Field_info::Field_rule>(0),
                               static_cast<Field_info::Field_type>(0),
                               Shared_string());
}

//! \brief deletes the attribute pointed by a given iterator.
void Element::mark_delete(Field_multi_str_attr_iter ai)
{
  ai->first.reset();
  auto* value = std::get<2>(ai->second);
  if (value) value->clear();
  ai->second = std::make_tuple(static_cast<Field_info::Field_rule>(0),
                               static_cast<Field_info::Field_type>(0),
                               nullptr);
}

//! \brief deletes all attributes.
void Element::delete_marked()
{
  m_str_attrs.erase(std::remove_if(m_str_attrs.begin(), m_str_attrs.end(),
                                   [](Str_attr attr)
                                   { return attr.first == nullptr; }),
                    m_str_attrs.end());
  m_multi_str_attrs.erase(std::remove_if(m_multi_str_attrs.begin(),
                                         m_multi_str_attrs.end(),
                                         [](Multi_str_attr attr)
                                         { return attr.first == nullptr; }),
                           m_multi_str_attrs.end());
  m_cont_attrs.erase(std::remove_if(m_cont_attrs.begin(), m_cont_attrs.end(),
                                    [](Cont_attr attr)
                                   { return attr.first == nullptr; }),
                     m_cont_attrs.end());
  m_multi_cont_attrs.erase(std::remove_if(m_multi_cont_attrs.begin(),
                                          m_multi_cont_attrs.end(),
                                          [](Multi_cont_attr attr)
                                          { return attr.first == nullptr; }),
                           m_multi_cont_attrs.end());
  m_field_attrs.erase(std::remove_if(m_field_attrs.begin(), m_field_attrs.end(),
                                     [](Field_attr attr)
                                     { return attr.first == nullptr; }),
                      m_field_attrs.end());
  m_field_multi_str_attrs.erase(std::remove_if(m_field_multi_str_attrs.begin(),
                                               m_field_multi_str_attrs.end(),
                                     [](Field_multi_str_attr attr)
                                     { return attr.first == nullptr; }),
                      m_field_multi_str_attrs.end());
}

//! \brief transfers attributes from a given element into this element.
void Element::splice(Element& element)
{
  m_str_attrs.splice(m_str_attrs.end(), element.get_str_attributes());
  m_multi_str_attrs.splice(m_multi_str_attrs.end(),
                           element.get_multi_str_attributes());
  m_cont_attrs.splice(m_cont_attrs.end(), element.get_cont_attributes());
  m_multi_cont_attrs.splice(m_multi_cont_attrs.end(),
                            element.get_multi_cont_attributes());
  m_field_attrs.splice(m_field_attrs.end(), element.get_field_attributes());
  m_field_multi_str_attrs.splice(m_field_multi_str_attrs.end(),
                                 element.get_field_multi_str_attributes());
}

SGAL_END_NAMESPACE
