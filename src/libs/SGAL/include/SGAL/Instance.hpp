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

/*! \todo nore needed?
 * Inctance is a reference object in the scene graph.
 * In the X3D it has the tag "USE" and it is used to reference
 * a node in the scene graph instead of duplicating it.
 */

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Instance : public XML_entity {
public:

  EInstance();
  virtual ~EInstance();

  void SetInstanceName(const String &name);
  String get_instanceName() const;

  static EInstance * prototype() { return new EInstance(); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  virtual Attribute_list get_attributes();

private:
  /** the name of the object we refere to */
  String m_instanceName;
};

SGAL_END_NAMESPACE
