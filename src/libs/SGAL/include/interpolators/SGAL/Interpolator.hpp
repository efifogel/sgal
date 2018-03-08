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
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Interpolator : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    FRACTION,
    KEY,
    LAST
  };

  /*! Construct. */
  Interpolator(Boolean interpolate_flag = true, Boolean proto = false);

  /*! Destruct. */
  virtual ~Interpolator();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* fraction_handle(const Field_info*) { return &m_fraction; }
  //@}

  // Functions that handles the creation of an instance in the scene graph
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  //! The interpolation execution function.
  virtual void execute(const Field_info* field_info) = 0;

  //! Obtain the range keys.
  const Float_array& get_keys() const { return m_keys; }

  //! Obtain the range keys.
  Float_array& get_keys() { return m_keys; }

protected:
  //! The interpolator range keys.
  Float_array m_keys;

  //! The fraction of the interpolator range.
  Float m_fraction;

  //! Indicates whether to interpolate between keys.
  Boolean m_interpolate_flag;

private:
  //! The node prototype.
  static Container_proto* s_prototype;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
