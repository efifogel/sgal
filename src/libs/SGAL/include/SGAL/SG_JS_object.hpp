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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Implements the java script client object for the scene graph objects.
 *
 * Uses the SAI to communicate with the scene graph.
 */

#ifndef SGAL_SG_JS_HPP
#define SGAL_SG_JS_HPP

#include "SGAL/basic.hpp"
#include "SAIClient.h"
#include "JSWClientObject.h"

SGAL_BEGIN_NAMESPACE

class ESG_JSObject: public ESAIClient, public JSWClientObject {
public:
  ESG_JSObject(ESAIServices* SAI, SAI_node_services* node,
               // if the node is referenced as a field of its father -
               // it's name is going to be the name it has as a field
               const String & nodeNameAsField);

  virtual ~ESG_JSObject();

  virtual const String & get_name();
  virtual propType get_propType(int id);

  virtual void get_propValue(int id, bool & value);
  virtual void SetPropValue(int id, bool value);

  virtual void get_propValue(int id, int & value);
  virtual void SetPropValue(int id, int value);

  virtual void get_propValue(int id, double & value);
  virtual void SetPropValue(int id, double value);

  virtual void get_propValue(int id, String & value);
  virtual void SetPropValue(int id, const String & value);

  virtual void get_propValue(int id, JSWClientObjectPnt & value);
  virtual void SetPropValue(int id, JSWClientObjectPnt value);

  virtual JSWPropContainerInt * get_propContainer();

  SAI_node_services * get_sAINode() const;

private:
  SAI_fieldServices * get_field(int id);

  ESAIServices* m_SAI;
  SAI_node_services* m_node;

  JSWPropContainerInt * m_properties;
  String m_name;
};

SGAL_END_NAMESPACE

#endif
