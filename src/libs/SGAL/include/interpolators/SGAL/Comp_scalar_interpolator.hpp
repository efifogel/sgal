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
 * Implements scalar interpolator with asynchronic loading of compressed data
 */

#ifndef SGAL_COMP_SCALAR_INTERPOLATOR_HPP
#define SGAL_COMP_SCALAR_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "Scalar_interpolator.h"
#include "Interpolator_int.h"
#include "Load_int.h"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Comp_scalar_interpolator :
  public Scalar_interpolator,
  public Interpolator_int,      // Encoding interface methods
  public Load_int               // Async. loading capability
{
public:
  /*! Constructor */
  Comp_scalar_interpolator(Boolean interpolate_flag = true,
                           Boolean proto = false);

  /*! Destructor */
  virtual ~Comp_scalar_interpolator();

  /* Construct the prototype */
  static Comp_scalar_interpolator* prototype()
  { return new Comp_scalar_interpolator(false, true); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  virtual void delete_prototype() {}

  // Overiding base interpolator functions
  virtual Attribute_list get_attributes();

  // Load_int methods
  virtual void AllocateMemory() {}  // Not used
  virtual Item_primitive* get_data();
  virtual void convert() {} // Not used
  virtual void update(Boolean is_last = true);
  virtual void delete_data() {}

  // Interpolator_int methods
  virtual Interpolator_data* get_interpolator_data();
  virtual void set_execute_coordinator(Execution_coordinator* pEc)
  {
    m_pExecutionCoordinator = pEc;
  }

protected:
  virtual float get_key_bin() { return m_default_key_bin; }
  virtual float get_value_bin_factor() { return m_value_bin_factor; }

  Interpolator_data* m_pInterpolator_data;
  Execution_coordinator* m_pExecutionCoordinator;

private:
  static const float m_default_key_bin;
  static const float m_value_bin_factor;
};

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Comp_scalar_interpolator::create()
{ return new Comp_scalar_interpolator(); }

SGAL_END_NAMESPACE

#endif
