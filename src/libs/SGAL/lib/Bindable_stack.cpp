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

#include "SGAL/Bindable_stack.hpp"
#include "SGAL/Bindable_node.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Configuration.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief binds the top-stack bindable element.
void Bindable_stack::bind_top()
{
  auto* bindable = top();
  if (bindable) bindable->bind();
}

//! \brief erases the bindable.
void Bindable_stack::erase(const Bindable_node* bindable)
{
  auto ni = m_bindable_stack.begin();
  for (; ni != m_bindable_stack.end(); ++ni) if ((*ni) == bindable) break;
  if (ni != m_bindable_stack.end()) m_bindable_stack.erase(ni);
}

SGAL_END_NAMESPACE
