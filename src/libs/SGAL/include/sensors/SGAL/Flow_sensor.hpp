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
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FLOW_SENSOR_HPP
#define SGAL_FLOW_SENSOR_HPP

/*! \file
 * This sensor is used to inform several loading and configuration 
 * parameters. A web based application can register on the fields of 
 * this sensor to be informed on these attributes.
 * The currently supported attributes are:
 * load done - returns true when the loading of the model (including all
 *             geometry and image data) is done. Currently supports a single
 *             scene data.
 * collaboration - returns true when the client is joined into a collaboration 
 *                 session and the collaboration is active.
 * number of polygons - the total number of polygons currently being rendered 
 * (and in memory)
 * absolute number of polygons - the total number of polygons that have been 
 * produced since the beginning. This number always increases.
 */

#include <queue>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
//! \todo #include "Time_check.h"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

typedef std::queue<int>         Int_queue;
typedef std::queue<Ulong>       Ulong_queue;

class SGAL_SGAL_DECL Flow_sensor : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    ANIMATION_LOAD_DONE,
    LEVEL0_LOAD_DONE,
    LOAD_DONE,
    SNAPSHOT_DONE,
    COLLABORATED,
    NUM_POLYGONS,
    ACCUM_NUM_POLYGONS,
    RATE,
    GEOM_MEMORY,
    LAST
  };

  /*! Constructor */
  Flow_sensor(Boolean proto = false);

  /*! Destructor */
  virtual ~Flow_sensor();

  /* Construct the prototype */
  static Flow_sensor* prototype() { return new Flow_sensor(true); }

  /*! Clone */
  virtual Container* clone() { return new Flow_sensor(); }

  void set_loading_done(Boolean flag = true);
  void set_level0_loading_done(Boolean flag = true);
  void set_snapshot_done(Boolean flag = true);
  void set_animation_loading_done(Boolean flag = true);
  void add_num_polygons(const Int n);
  void add_geometry_memory(const Int n);

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! true after all interpolators have been loaded */
  Boolean m_is_animation_loading_done;

  /*! true when the loading of the model's level 0 is completed */
  Boolean m_is_level0_loading_done;

  /*! true when the loading of the model is completed */
  Boolean m_is_loading_done;

  /*! true when a snapshot of the model is completed */
  Boolean m_is_snapshot_done;

  /*! true indicates the collaboration is currently active */
  Boolean m_collaborated;

  /*! total number of polygons */
  Int m_num_polygons;

  /*! total number of polygons produced */
  Int m_accum_num_polygons;

  /*! total memory allocated for geometry */
  Int m_geomMemory;

  Int_queue m_polygon_count_q;
  Ulong_queue m_time_Q;
  //! \todo Time_check m_timer;
  Float m_rate;
};

SGAL_END_NAMESPACE

#endif
