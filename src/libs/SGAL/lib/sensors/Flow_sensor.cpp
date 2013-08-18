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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Flow_sensor.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

#define QUEUE_DEPTH 5

const std::string Flow_sensor::s_tag = "FlowSensor";
Container_proto* Flow_sensor::s_prototype(NULL);

REGISTER_TO_FACTORY(Flow_sensor, "Flow_sensor");

/*! Constructor */
Flow_sensor::Flow_sensor(Boolean proto) :
  Container(proto),
  m_isAnimationLoadingDone(false),
  m_isLoadingDone(false),
  m_isLevel0LoadingDone(false),
  m_isSnapshotDone(false),
  m_collaborated(false),
  m_num_polygons(0),
  m_accumNumPolygons(0),
  m_rate(0),
  m_geomMemory(0)
{
  for (Uint i = 0; i < QUEUE_DEPTH; i++) {
    m_timeQ.push(0);
    m_polygonCountQ.push(0);
  }
}

/*! Destructor */
Flow_sensor::~Flow_sensor() {}

/*! \brief */
void Flow_sensor::set_animation_loading_done(Boolean flag)
{
  m_isAnimationLoadingDone = flag;
  Field* field = get_field(ANIMATION_LOAD_DONE);
  if (field) field->Cascade();
}

/*! \brief */
void Flow_sensor::SetLevel0LoadingDone(Boolean flag)
{
  m_isLevel0LoadingDone = flag;
  Field* field = get_field(LEVEL0_LOAD_DONE);
  if (field) field->Cascade();
}

/*! \brief */
void Flow_sensor::SetLoadingDone(Boolean flag)
{
  m_isLoadingDone = flag;
  Field* field = get_field(LOAD_DONE);
  if (field) field->cascade();
}

/*! \brief */
void Flow_sensor::SetSnapshotDone(Boolean flag)
{
  m_isSnapshotDone = flag;
  Field* field = get_field(SNAPSHOT_DONE);
  if (field) field->cascade();
}

/*! \brief */
void Flow_sensor::AddNumPolygons(const Int n)
{
  m_num_polygons += n;
  if (n > 0) m_accumNumPolygons += n;
  Field* field = get_field(NUM_POLYGONS);
  if (field) field->cascade();
  field = get_field(ACCUM_NUM_POLYGONS);
  if (field) field->cascade();

  ULong time = m_timer.get_time();
  m_timeQ.pop();
  m_timeQ.push(time);
  ULong dt = time - m_timeQ.front();

  m_polygonCountQ.pop();
  m_polygonCountQ.push(m_accumNumPolygons);
  int dp = m_accumNumPolygons - m_polygonCountQ.front();

  if (dt != 0) {
    m_rate = 1000*dp/dt ;
    field = get_field(RATE);
    if (field) field->cascade();
  }
}

/*! \brief */
void Flow_sensor::AddGeometryMemory(const Int n)
{
  m_geomMemory += n;
  Field field = get_field(GEOM_MEMORY);
  if (field) field->cascade();
}

/*! \brief sets the attributes of this container. */
void Flow_sensor::set_attributes(Element * elem)
{
  Container::set_attributes(elem);

  // Override the DEF name( hard coded).
  //! \todo ? set_name( "enbFlowSensor");
}

#if 0
/*!
 */
Attribute_list Flow_sensor::get_attributes()
{
  Attribute_list attribs = Container::get_attributes();
  return attribs;
}

#endif

/*! initializes the node prototype */
void Flow_sensor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the object fields to the prototype
  s_prototype->
    add_field_info(new SF_bool(ANIMATION_LOAD_DONE, "isAnimationLoadingDone",
                               get_member_offset(&m_isAnimationLoadingDone)));

  s_prototype->
    add_field_info(new SF_bool(LEVEL0_LOAD_DONE, "isLevel0LoadingDone",
                               get_member_offset(&m_isLevel0LoadingDone)));

  // Add the object fields to the prototype
  s_prototype->
    add_field_info(new SF_bool(LOAD_DONE, "isLoadingDone",
                               get_member_offset(&m_isLoadingDone)));

  // Add the object fields to the prototype
  s_prototype->
    add_field_info(new SF_bool(SNAPSHOT_DONE, "isSnapshotDone",
                               get_member_offset(&m_isSnapshotDone)));

  // Add the object fields to the prototype
  s_prototype->
    add_field_info(new SF_bool(COLLABORATED, "collaborated",
                               get_member_offset(&m_collaborated)));

  // Add the object fields to the prototype
  s_prototype->
    add_field_info(new SF_int(NUM_POLYGONS, "numPolygons",
                              get_member_offset(&m_num_polygons)));

  // Add the object fields to the prototype
  s_prototype->
    add_field_info(new SF_int(ACCUM_NUM_POLYGONS, "accumNumPolygons",
                              get_member_offset(&m_accumNumPolygons)));

  // Add the object fields to the prototype
  s_prototype->add_field_info(new SF_float(RATE, "rate",
                                           get_member_offset(&m_rate)));

  // Add the object fields to the prototype
  s_prototype->add_field_info(new SF_int(GEOM_MEMORY, "geomMemory",
                                         get_member_offset(&m_geomMemory)));
}

/*! \brief */
void Flow_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
Container_proto* Flow_sensor::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
