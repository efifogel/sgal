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
// $Revision: 1311 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VERSION_INFO_HPP
#define SGAL_VERSION_INFO_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_CLASSDEF Version_info {
private:
  Ulong m_major;
  Ulong m_middle;
  Ulong m_minor;

  void copy(const Version_info & Version_info);

public:
  Version_info();
  Version_info(Ulong major, Ulong middle, Ulong minor);

  Version_info& operator=(const Version_info&);
  friend bool operator>(const Version_info & vinfo1,
                        const Version_info & vinfo2);
  friend bool operator<(const Version_info & vinfo1,
                        const Version_info & vinfo2);
  friend bool operator==(const Version_info & vinfo1,
                         const Version_info & vinfo2);
  friend bool operator!=(const Version_info & vinfo1,
                         const Version_info & vinfo2);
  friend bool operator>=(const Version_info & vinfo1,
                         const Version_info & vinfo2);
  friend bool operator<=(const Version_info & vinfo1,
                         const Version_info & vinfo2);
};

SGAL_END_NAMESPACE

#endif
