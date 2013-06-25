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
// $Id: $
// $Revision: 1311 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ELEMENT_HPP
#define SGAL_ELEMENT_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Element {
public:
  typedef boost::shared_ptr<Container>              Shared_container;

  typedef std::pair<std::string*, std::string*>     Str_attr;
  typedef std::list<Str_attr*>                      Str_attr_list;
  typedef Str_attr_list::iterator                   Str_attr_iter;
  
  typedef std::pair<std::string*, Shared_container> Cont_attr;
  typedef std::list<Cont_attr*>                     Cont_attr_list;
  typedef Cont_attr_list::iterator                  Cont_attr_iter;
  
  typedef std::list<Shared_container>               Cont_list;
  typedef Cont_list::iterator                       Cont_iter;
  typedef std::pair<std::string*, Cont_list*>       Multi_cont_attr;
  typedef std::list<Multi_cont_attr*>               Multi_cont_attr_list;
  typedef Multi_cont_attr_list::iterator            Multi_cont_attr_iter;

private:
  /*! */
  struct Str_eraser {
    bool operator()(const Str_attr* attr) { return attr == 0; }
  };

  /*! */
  struct Cont_eraser {
    bool operator()(const Cont_attr* attr) { return attr == 0; }
  };

  /*! */
  struct Multi_cont_eraser {
    bool operator()(const Multi_cont_attr* attr) { return attr == 0; }
  };
  
public:  
  /*! */
  void add_attribute(Str_attr* attribute, bool back = true);

  /*! */
  void add_attribute(Cont_attr* attribute, bool back = true);

  /*! */
  void add_attribute(Multi_cont_attr* attribute, bool back = true);
    
  /*! */
  Str_attr_list& get_str_attributes() { return m_str_attrs; }

  /*! */
  Cont_attr_list& get_cont_attributes() { return m_cont_attrs; }

  /*! */
  Multi_cont_attr_list& get_multi_cont_attributes()
  {
    return m_multi_cont_attrs;
  }

  /*! */
  Str_attr_iter str_attrs_begin() { return m_str_attrs.begin(); }

  /*! */
  Str_attr_iter str_attrs_end() { return m_str_attrs.end(); }

  /*! */
  void mark_delete(Str_attr_iter& ai);

  /*! */
  const std::string& get_name(Str_attr_iter ai) { return *((*ai)->first); }

  /*! */
  const std::string& get_value(Str_attr_iter ai) { return *((*ai)->second); }

  /*! */
  Cont_attr_iter cont_attrs_begin() { return m_cont_attrs.begin(); }

  /*! */
  Cont_attr_iter cont_attrs_end() { return m_cont_attrs.end(); }

  /*! */
  void mark_delete(Cont_attr_iter& ai);

  /*! */
  const std::string& get_name(Cont_attr_iter ai) { return *((*ai)->first); }

  /*! */
  Shared_container get_value(Cont_attr_iter ai) { return (*ai)->second; }

  /*! */
  Multi_cont_attr_iter multi_cont_attrs_begin()
  {
    return m_multi_cont_attrs.begin();
  }

  /*! */
  Multi_cont_attr_iter multi_cont_attrs_end()
  {
    return m_multi_cont_attrs.end();
  }

  /*! */
  void mark_delete(Multi_cont_attr_iter& ai);

  /*! */
  const std::string& get_name(Multi_cont_attr_iter ai)
  {
    return *((*ai)->first);
  }

  /*! */
  Cont_list& get_value(Multi_cont_attr_iter ai) { return *((*ai)->second); }
  
  /*! */
  void delete_marked();
  
protected:
  /*! */
  Str_attr_list m_str_attrs;

  /*! */
  Cont_attr_list m_cont_attrs;

  /*! */
  Multi_cont_attr_list m_multi_cont_attrs;
};

SGAL_END_NAMESPACE

#endif
