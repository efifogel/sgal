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
// $Source: $
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SINGLETON_DESTROYER_HPP
#define SGAL_SINGLETON_DESTROYER_HPP

#include "Types.h"

template<class Singleton> class Singleton_destroyer {
public:
  Singleton_destroyer(Singleton * singleton = NULL)
  {
    m_singleton = singleton;
  }
  ~Singleton_destroyer() { DELETE_OBJECT(m_singleton); }

  void set_singleton(Singleton * singleton) { m_singleton = singleton; }
  Singleton * get_singleton() { return m_singleton; }

private:
  Singleton_destroyer(const Singleton_destroyer<Singleton>&);
  void operator=(const Singleton_destroyer<Singleton>&);
		
  Singleton * m_singleton;
};

#endif
