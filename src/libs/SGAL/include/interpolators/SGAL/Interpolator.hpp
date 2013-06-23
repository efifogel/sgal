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
// $Id: Interpolator.hpp 6147 2008-04-02 15:03:46Z efif $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Pure interpolator engine
 *
 * The pure coordinate interpolator engine serves as the base class for
 * all interpolators. It has the following fields:
 * - fraction (field)
 * - key - array of floating-point numbers
 *
 * When the fraction field is set the interpolating function (execute)
 * is evaluated, resulting in a change of value with the same timestamp
 * as the timestamp of fraction change. The intepolating value is
 * calculated according to the fraction, keys, and values.
 */

#ifndef SGAL_INTERPOLATOR_HPP
#define SGAL_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

class SGAL_SGAL_DECL Interpolator : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    FRACTION,
    KEY,
    LAST
  };

  /*! Constructor */
  Interpolator(Boolean interpolate_flag = true, Boolean proto = false);

  /*! Destructor */
  virtual ~Interpolator();

  /*! Clone */
  virtual Container* clone() { SGAL_assertion(0); return 0; }

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();
  
  // Functions that handles the creation of an instance in the scene graph
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! The interpolation execution function */
  virtual void execute(Field_info* field_info) = 0;

  /*! Obtain the range keys */
  const Float_array& get_keys() const { return m_keys; }
  
  /*! Obtain the range keys */
  Float_array& get_keys() { return m_keys; }
  
protected:
  /*! The interpolator range keys */
  Float_array m_keys;

  /*! The fraction of the interpolator range */
  Float m_fraction;

  /*! Indicates whether to interpolate between keys */
  Boolean m_interpolate_flag;

private:
  /*! The node prototype */
  static Container_proto* s_prototype;
};

SGAL_END_NAMESPACE

#endif
