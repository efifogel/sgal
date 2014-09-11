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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CONTAINER_FACTORY_HPP
#define SGAL_CONTAINER_FACTORY_HPP

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Container;

#if !defined (SGAL_LIB)
#define REGISTER_TO_FACTORY(class_name, name)                         \
struct Reg_##class_name {                                             \
  Reg_##class_name() {                                                \
    Container_factory* factory = Container_factory::get_instance();   \
    factory->doregister(class_name::prototype());                     \
  }                                                                   \
  virtual ~Reg_##class_name() {}                                      \
} instance_##class_name;

#define REGISTER_OBJECT(className)
#else

#define REGISTER_TO_FACTORY(class_name, name)

#define REGISTER_OBJECT(class_name) \
  doregister(class_name::prototype());

#endif

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Produces containers. */
class SGAL_SGAL_DECL Container_factory {
public:
  typedef boost::shared_ptr<Container>            Shared_container;

  /*! Obtain the factory singletone. */
  static Container_factory* get_instance();

  /*! Register the given container type. */
  void doregister(Container* container);

  /*! Create a clone of a container,
   * \param type the type of the container to be created.
   */
  Shared_container create(const std::string& type);

private:
  /*! The container-factory singletone. */
  static Container_factory* m_instance;

  typedef std::map<std::string, Container*>     Cont_map;
  typedef Cont_map::iterator                    Cont_iter;

  /*! The tag to container mapping. */
  Cont_map m_map;

  /*! Th parameter-less constructor. */
  Container_factory() { initialize(); }

  /*! Register all containers in the container factory. */
  void initialize();
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
