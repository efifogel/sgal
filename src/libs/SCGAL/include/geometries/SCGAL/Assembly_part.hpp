// Copyright (c) 2008 Israel.
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
// $Id: Assembly_part.hpp 13487 2012-06-05 23:31:41Z efif $
// $Revision: 13487 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_ASSEMBLY_PART_HPP
#define SCGAL_ASSEMBLY_PART_HPP

/*! \file
 * A class representing a puzzle.
 */

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Switch.hpp"
#include "SGAL/Shape.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Spherical_gaussian_map_colored_geo.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Appearance;

class SGAL_SCGAL_DECL Assembly_part : public Group {
  typedef Spherical_gaussian_map_colored_geo        Sgm_geo;
  typedef Spherical_gaussian_map_colored            Sgm;

  typedef boost::shared_ptr<Appearance>             Shared_appearance;
  typedef boost::shared_ptr<Sgm_geo>                Shared_sgm_geo;
  
  typedef std::list<Shared_sgm_geo>                 Sgm_geo_list;
  typedef std::list<Boolean>                        Boolean_list;
  typedef std::list<Shared_appearance>              Appearance_list; 
  
  typedef Sgm_geo_list::iterator                    Sgm_geo_iter;

 public:
  enum {
    FIRST = Group::LAST - 1,
    LAST
  };

  /*! Constructor. */
  Assembly_part(Boolean proto = false);

  /*! Destructor. */
  virtual ~Assembly_part();

  /* Construct the prototype. */
  static Assembly_part* prototype() { return new Assembly_part(true); }

  /*! Clone. */
  virtual Container* clone() { return new Assembly_part(); }

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Clear the representation. */
  virtual void clear();

  /*! Clean the representation. */
  virtual void clean();

  /*! Set the part id. */
  void set_id(Uint id) { m_id = id; }
  
  /*! Print information to an output stream. */
  void print_info(std::ostream& out);

  /*! Obtain the container of the Sgm geometries that comprise this part. */
  Sgm_geo_list& get_sgm_geos();

  /*! Obtain the container of the Sgm appearances that comprise this part */
  Appearance_list& get_sgm_apps();
  
protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Construct all the SGM's that comprise this part. */
  void clean_sgm_geos(Node* node);

  /*! Construct all the SGM's that comprise this part. */
  void clean_sgm_geos(Group* group);
 
private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Is the internal representation dirty and requires cleaning. */
  Boolean m_dirty;

  /*! A non-negative number unique among the parts of a given puzzle. */
  Uint m_id;

  /*! The list of Sgm geometries. */
  Sgm_geo_list m_sgm_geos;

  /*! The list of Sgm appearances. */
  Appearance_list m_sgm_apps;

  /*! A list of flags that indicates whether the corresponding appearance is
   * owned, i.e., allocated explicitly. If it is owned it should be explicitly
   * deallocated.
   */
  Boolean_list m_owned_sgm_aps;
  
};

SGAL_END_NAMESPACE

#endif
