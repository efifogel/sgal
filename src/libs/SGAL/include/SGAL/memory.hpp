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

#ifndef SGAL_MEMORY_HPP
#define SGAL_MEMORY_HPP

#include <memory>

// SGAL_ALLOCATOR(t) defines the default allocator used by SGAL.
// SGAL_MEMORY(t) overloads the new and delete operators for a given class.

// When LEDA is there, the user could define these macros as
// leda_allocator< T >  and  LEDA_MEMORY(T) for example.

// For debugging with GCC, the following allocator can be useful :
// std::__allocator<T, std::__debug_alloc<std::__malloc_alloc_template<0> > >

#ifndef SGAL_ALLOCATOR
#  define SGAL_ALLOCATOR(T) std::allocator< T >
#endif

#ifndef SGAL_MEMORY
#  define SGAL_MEMORY(T)
#endif

#endif
