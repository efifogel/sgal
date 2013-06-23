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

#ifndef SGAL_EGO_VOXELS_FILLER_BASE_HPP
#define SGAL_EGO_VOXELS_FILLER_BASE_HPP

#include "SGAL/config.hpp"
#include "SGAL/Node.hpp"

#include "SEGO/config.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class SGAL_SEGO_DECL Ego_filler {
public:
  virtual void fill(Ego_voxels*) const = 0;
};

class SGAL_SEGO_DECL Ego_voxels_filler_base : public Node {
 public:
  
  enum {
    FIRST = Node::LAST - 1,
    OFFSET,
    LAST
  };

  /*! Constructor */
  Ego_voxels_filler_base(Boolean proto = false);
  
  virtual ~Ego_voxels_filler_base();

  /*! Initialize the container prototype. */
  virtual void init_prototype();
  
  /*! Delete the container prototype. */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the attributes. */
  virtual void set_attributes(Element* elem);

  /*! Fill. */
  virtual void fill(Ego_voxels*) const = 0;

  /*! Set the offset. */
  void set_offset(Int offset);

  /*! Get the offset. */
  Int get_offset() const;

 protected:
  Int m_offset;

 private:
  /*! The container prototype. */
  static Container_proto* s_prototype;
  
  /*! Default values */
  static const Int s_def_offset;
};

SGAL_END_NAMESPACE

#endif // SGAL_EGO_VOXELS_FILLER_BASE_HPP
