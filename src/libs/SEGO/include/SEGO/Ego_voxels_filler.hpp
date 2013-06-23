// Copyright (c) 2012 Israel.
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
// Author(s)     : Ophir Setter           <ophir.setter@gmail.com>

#ifndef SGAL_EGO_VOXELS_FILLER_HPP
#define SGAL_EGO_VOXELS_FILLER_HPP

#include "SEGO/Ego_voxels_filler_base.hpp"

#include "SEGO/basic.hpp"

#include <string>

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class SGAL_SEGO_DECL Ego_voxels_filler : public Ego_voxels_filler_base {
 public:

  /* Constructor */
  Ego_voxels_filler(bool = false);
  
  /* Construct the prototype */
  static Ego_voxels_filler* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  virtual void fill(Ego_voxels* voxels) const;

 private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

};

SGAL_END_NAMESPACE

#endif // SGAL_EGO_VOXELS_FILLER_HPP
