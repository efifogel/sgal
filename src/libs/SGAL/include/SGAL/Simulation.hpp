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

/*!
 * Simulation
 */

#ifndef SGAL_SIMULATION_HPP
#define SGAL_SIMULATION_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Scene_graph;

class Simulation : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    START,
    LAST
  };

  /*! Constructor */
  Simulation(Boolean proto = false);

  /*! Destructor */
  virtual ~Simulation() {};

  /*! Construct the prototype. */
  static Simulation* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* start_handle(const Field_info*) { return &m_start; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Start simulation. */
  void start();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! A field to trigger the start of the simulation. */
  Boolean m_start;
};

/*! \brief constructs the prototype. */
inline Simulation* Simulation::prototype() { return new Simulation(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Simulation::create() { return new Simulation(); }

/*! \brief obtains the tag (type) of the container */
inline const std::string& Simulation::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
