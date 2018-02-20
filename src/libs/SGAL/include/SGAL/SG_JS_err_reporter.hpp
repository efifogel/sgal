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
 * This class handles error reporting for the java script interpreter
 */

#ifndef SGAL_SG_JS_ERR_REPORTER_HPP
#define SGAL_SG_JS_ERR_REPORTER_HPP

#include "SGAL/basic.hpp"
#include "EnbTypes.h"
#include "JSWEngineInt.h"

SGAL_BEGIN_NAMESPACE

class SG_JSErrReporter: public JSWErrorReporter {
public:
  virtual void Report(const String & objectName, const String & currFuncName,
                      int lineNo, const String & message)
  {
    if (lineNo > 0)
      debug("JS Error - Object: %s, Function: %s, Line No: %d, Message: %s", 
            objectName, currFuncName, lineNo, message);
    else
      debug("JS Error - Object: %s, Function: %s, Message: %s", 
            objectName, currFuncName, message);
  }

  virtual void Trace(const String & text)
  {
    debug("%s", text);
  }
};

SGAL_END_NAMESPACE

#endif
