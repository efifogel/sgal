// Copyright (c) 2004  Israel.
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
// $Id: Multisample.hpp 6147 2008-04-02 15:03:46Z efif $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel            <efifogel@gmail.com>

#ifndef SGAL_MULTISAMPLE_HPP
#define SGAL_MULTISAMPLE_HPP

/*! \file
 * Encapsulates the OpenGL Multisample application. Specifies the
 * parameters needed to apply multisample.
 */

#include <vector>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;

class SGAL_SGAL_DECL Multisample : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    NUMBER_OF_SAMPLES
  };
  
  /*! Constructor */
  Multisample(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Multisample() {}

  /*! Construct the prototype */
  static Multisample * prototype() { return new Multisample(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Multisample(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto * get_prototype();
  
  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Set the flag that indicates whether accumulation is enabled */
  void set_enabled(Boolean flag) { m_enabled = flag; }

  /*! is multisample enabled? */
  Boolean is_enabled() const { return m_enabled; }
  
  /*! Set the number of (multi)samples
   * \param samples number of (multi)samples
   */
  void set_number_of_samples(Uint samples) { m_number_of_samples = samples; }

  /*! Obtain the number of (multi)samples
   * \return the number of (multi)samples
   */
  Uint get_number_of_samples() const { return m_number_of_samples; }
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:  
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! Indicates whether accumulation is enabled */
  Boolean m_enabled;

  /*! The number of samples */
  Uint m_number_of_samples;
  
  // default values
  static Boolean s_def_enabled;
  static Uint s_def_number_of_samples;
};

SGAL_END_NAMESPACE

#endif
