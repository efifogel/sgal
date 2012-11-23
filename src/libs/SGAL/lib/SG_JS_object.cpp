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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/**
  SG_JSObject - implementation
*/

#include "SGAL/basic.hpp"
#include "SG_JSObject.h"
#include "SGVec_JSObject.h"
#include "SAIFieldSpecificServicesTemplate.h"

using namespace SGAL;

#define PROP_PREFIX 100


SG_JSObject::SG_JSObject(ESAIServices* SAI, 
               SAI_node_services* node,
               // if the node is referenced as a field of its father -
               // it's name is going to be the name it has as a field
               const String& nodeNameAsField):
  m_SAI(SAI),
  m_node(node),
  m_properties(NULL),
  m_name(nodeNameAsField)
{
  if (m_node==ENULL)
  {
    assert(false);
  }
  else
    // Set this as the SAINode client - the SAINode will destroy this
    // during application destruction
    m_node->SetClient(this);
}

/*! Destructor */
SG_JSObject::~SG_JSObject()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~SG_JSObject ...");
  if (m_properties) m_properties->destroy();
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

const std::string & SG_JSObject::get_name()
{
  return m_name;
}

SAI_node_services* SG_JSObject::get_sAINode() const
{
  return m_node;
}


SAI_fieldServices* SG_JSObject::get_field(int id)
{
  SAI_fieldServices* field = m_node->get_field(id);
  // if field with this id was not found - try to get the "set_"
  // prefix off it and check again
  if (field==ENULL)
    field = m_node->get_field(id-PROP_PREFIX);

  return field;
}


JSWClientObject::propType SG_JSObject::get_propType(int id)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
    return OTHER;

  Int type = field->get_type();

  if (type==SFBOOL)
    return BOOLEAN;
  else if (type==SFFLOAT || type==SFTIME)
    return DOUBLE;
  else if (type==SFINT32)
    return INTEGER;
  else if (type==SFVEC2F || type==SFVEC3F || 
       type==SFROTATION || type==SFVEC4F ||
       type==SFNODE)
    return OBJECT;
  else if (type==SFSTRING)
    return STRING;
  else
  {
    assert(false);
    return OTHER;
  }
}

void SG_JSObject::get_propValue(int id, bool& value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL>* boolField = 
    dynamic_cast<SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL>*>(field);
  if (boolField==ENULL)
  {
    assert(false);
    return;
  }
  value = boolField->get_value();
}


void SG_JSObject::SetPropValue(int id, bool value)
{  
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL>* boolField = 
    dynamic_cast<SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL>*>(field);
  if (boolField==ENULL)
  {
    assert(false);
    return;
  }
  boolField->set_value(value);
}


void SG_JSObject::get_propValue(int id, int& value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  SAI_fieldSpecificServicesTemplate<Int,SFINT32>* intField = 
    dynamic_cast<SAI_fieldSpecificServicesTemplate<Int,SFINT32>*>(field);
  if (intField==ENULL)
  {
    assert(false);
    return;
  }
  value = intField->get_value();
}


void SG_JSObject::SetPropValue(int id, int value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  SAI_fieldSpecificServicesTemplate<Int,SFINT32>* intField = 
    dynamic_cast<SAI_fieldSpecificServicesTemplate<Int,SFINT32>*>(field);
  if (intField==ENULL)
  {
    assert(false);
    return;
  }
  intField->set_value(value);
}


void SG_JSObject::get_propValue(int id, double& value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  Int type = field->get_type();
  if (type==SFFLOAT)
  {
    SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>* floatField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>*>(field);
    if (floatField==ENULL)
    {
      assert(false);
      return;
    }
    value = floatField->get_value();
  }
  else if (type==SFTIME)
  {
    SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME>* timeField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME>*>(field);
    if (timeField==ENULL)
    {
      assert(false);
      return;
    }
    value = timeField->get_value();
  }
  else
    assert(false);
}


void SG_JSObject::SetPropValue(int id, double value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  Int type = field->get_type();
  if (type==SFFLOAT)
  {
    SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>* floatField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>*>(field);
    if (floatField==ENULL)
    {
      assert(false);
      return;
    }
    floatField->set_value(value);
  }
  else if (type==SFTIME)
  {
    SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME>* timeField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME>*>(field);
    if (timeField==ENULL)
    {
      assert(false);
      return;
    }
    timeField->set_value(value);
  }
  else
    assert(false);
}


void SG_JSObject::get_propValue(int id, String& value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  SAI_fieldSpecificServicesTemplate<String,SFSTRING>* stringField = 
    dynamic_cast<SAI_fieldSpecificServicesTemplate<String,SFSTRING>*>(field);
  if (stringField==ENULL)
  {
    assert(false);
    return;
  }
  value = stringField->get_value();
}


void SG_JSObject::SetPropValue(int id, const String& value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  SAI_fieldSpecificServicesTemplate<String,SFSTRING>* stringField = 
    dynamic_cast<SAI_fieldSpecificServicesTemplate<String,SFSTRING>*>(field);
  if (stringField==ENULL)
  {
    assert(false);
    return;
  }
  stringField->set_value(value);
}


void SG_JSObject::get_propValue(int id, JSWClientObjectPnt& value)
{
  SAI_fieldServices* field = get_field(id);
  if (field==ENULL)
  {
    assert(false);
    return;
  }
  Int type = field->get_type();

  // Handle SFNode field
  if (type==SFNODE)
  {
    SAI_fieldSpecificServicesTemplate<SAI_node_servicesPnt,SFNODE>* nodeField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<SAI_node_servicesPnt,SFNODE>*>(field);
    if (nodeField==ENULL)
    {
      assert(false);
      return;
    }
    SAI_node_services* nodeValue = nodeField->get_value();
    if (nodeValue==ENULL)
    {
      assert(false);
      return;
    }

    // If the SAI node has a client - it served a SG_JSObject before -
    // use this client
    if (nodeValue->get_client()==ENULL)
      // if the SAI node has no client - allocate a new object and return it
      value = new SG_JSObject(m_SAI,nodeValue,nodeField->get_name());
    else
    {
      // Cast the client to SG_JSObject
      SG_JSObject* obj = 
        dynamic_cast<SG_JSObject*>(nodeValue->get_client());
      if (obj==ENULL)
      {
        assert(false);
        return;
      }
      value = obj;
    }
  }
  else if (type==SFVEC2F || type==SFVEC3F || type==SFROTATION || type==SFVEC4F)
  {
    // If the SAI field has a client - it served a ESGVec_JSObject before -
    // use this client
    if (field->get_client()==ENULL)
      // if the SAI node has no client - allocate a new object and return it
      value = new ESGVec_JSObject(m_SAI,m_node,field);
    else
    {
      // Cast the client to SG_JSObject
      ESGVec_JSObject* obj = 
        dynamic_cast<ESGVec_JSObject*>(field->get_client());
      if (obj==ENULL)
      {
        assert(false);
        return;
      }
      value = obj;
    }
  }
}

void SG_JSObject::SetPropValue(int id, JSWClientObjectPnt value)
{
  double dval;
  SAI_fieldServices* genField = get_field(id);
  if (genField==ENULL)
  {
    assert(false);
    return;
  }
  Int type = genField->get_type();

  // Handle SFNode field
  if (type==SFNODE)
  {
    SAI_fieldSpecificServicesTemplate<SAI_node_servicesPnt,SFNODE>* nodeField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<SAI_node_servicesPnt,SFNODE>*>(genField);
    if (nodeField==ENULL)
    {
      assert(false);
      return;
    }
    SG_JSObject* obj = dynamic_cast<SG_JSObject*>(value);
    if (obj==ENULL)
    {
      assert(false);
      return;
    }
    nodeField->set_value(obj->get_sAINode());
  }
  if (type==SFVEC2F)
  {
    SAI_fieldSpecificServicesTemplate<Vector2f,SFVEC2F>* field = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Vector2f,SFVEC2F>*>(genField);
    if (field==ENULL)
    {
      assert(false);
      return;
    }
    value->get_propValue(0,dval);
    (field->get_value())[0] = dval;
    value->get_propValue(1,dval);
    (field->get_value())[1] = dval;
  }
  else if (type==SFVEC3F)
  {
    SAI_fieldSpecificServicesTemplate<Vector3f,SFVEC3F>* field = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Vector3f,SFVEC3F>*>(genField);
    if (field==ENULL)
    {
      assert(false);
      return;
    }
    value->get_propValue(0,dval);
    (field->get_value())[0] = dval;
    value->get_propValue(1,dval);
    (field->get_value())[1] = dval;
    value->get_propValue(2,dval);
    (field->get_value())[2] = dval;
  }
  else if (type==SFVEC4F)
  {
    SAI_fieldSpecificServicesTemplate<Vector4f,SFVEC4F>* field = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Vector4f,SFVEC4F>*>(genField);
    if (field==ENULL)
    {
      assert(false);
      return;
    }
    value->get_propValue(0,dval);
    (field->get_value())[0] = dval;
    value->get_propValue(1,dval);
    (field->get_value())[1] = dval;
    value->get_propValue(2,dval);
    (field->get_value())[2] = dval;
    value->get_propValue(3,dval);
    (field->get_value())[3] = dval;
  }
  else if (type==SFROTATION)
  {
    SAI_fieldSpecificServicesTemplate<Rotation,SFROTATION>* field = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Rotation,SFROTATION>*>(genField);
    if (field==ENULL)
    {
      assert(false);
      return;
    }
    value->get_propValue(0,dval);
    (field->get_value())[0] = dval;
    value->get_propValue(1,dval);
    (field->get_value())[1] = dval;
    value->get_propValue(2,dval);
    (field->get_value())[2] = dval;
    value->get_propValue(3,dval);
    (field->get_value())[3] = dval;
  }
}


JSWPropContainerInt* SG_JSObject::get_propContainer()
{
  // if m_properties was created before - return it
  if (m_properties!=NULL)
    return m_properties;

  // Create m_properties
  m_properties = CreatePropContainer();

  FieldIDsList IDsList;
  m_node->get_fieldsList(IDsList);

  // Loop over the IDs and add each one to m_properties
    FieldIDsList::iterator iter = IDsList.begin();
    while( iter != IDsList.end())
    {
    m_properties->AddProp((*iter).second,(*iter).first);
    // Add the same property with the "set_" prefix - sometimes
    // in VRML an event in property is refered this way
    m_properties->AddProp("set_"+(*iter).second,
                PROP_PREFIX+(*iter).first);
    iter++;
  }

  return m_properties;
}


