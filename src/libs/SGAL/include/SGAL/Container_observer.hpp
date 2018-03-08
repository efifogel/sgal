// Copyright (c) 2009 Israel.
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

/*! \file
 * A pure observer that is part of the implementation of a simple observer
 * design pattern. A container, serves as the subject, maintains a list of
 * its dependents, called observers, and notifies them automatically of any
 * state changes, by calling a method of the observer.
 */

#ifndef CONTAINER_OBSERVER_HPP
#define CONTAINER_OBSERVER_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;

class SGAL_SGAL_DECL Container_observer {
public:
  /*! Destructor */
  virtual ~Container_observer() {}

  /*! Process change of field */
  virtual void field_changed(const Field_info* /* field_info */) {}
};

SGAL_END_NAMESPACE

#endif
