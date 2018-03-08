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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

/*! \file
/*  class : Interpolator_int
/*
/*  Purpose :
/*    This interface is used by the convertor
/*    To encode enb format data for VRML interpolator
/*    classes
/**/

#include "SGAL/basic.hpp"
#include "Interpolator_data.h"
#include "ExecutionCoordinator.h"

SGAL_BEGIN_NAMESPACE

class Interpolator_int {
public:
  Interpolator_int() {}
  virtual ~Interpolator_int() {}

  virtual Interpolator_data *get_interpolator_data() = 0;
  virtual void set_executeCoordinator(Execution_coordinator *pEc) = 0;

protected:
  virtual float get_valueRange(float *values, int number)
  {
    if (number <= 0) {
      return (float)1e-3;
    }
    float min = values[0];
    float max = values[0];
    float *pVal = values;
    for ( int i = 0; i < number; i++, pVal++) {
      if (*pVal < min) min = *pVal;
      if (*pVal > max) max = *pVal;
    }
    float range = (max - min);
    float absMax = max;
    if (absMax < (float)0.0) {
      absMax = -absMax;
    }
    if (range < (absMax * 1e-1))
    {
      range = absMax * 1e-1;
    }
    return range;
  }
};

SGAL_END_NAMESPACE
