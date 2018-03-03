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

#ifndef SGAL_PROTO_HPP
#define SGAL_PROTO_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class Proto : public Container {
public:
  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Proto(Boolean proto = false);

  /*! Construct the prototype container.
   * \return the prototype container.
   */
  static Proto* prototype();

  /*! Destruct. */
  virtual ~Proto();

  /*! Clone the container from the prototype container.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Container protoype handling
  //@{
  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   * \return the container prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the tag (type) of the container.
   */
  virtual const String& get_tag() const;

  /*! Set the tag (type) of the container.
   */
  void set_tag(const String& tag);

protected:
  /*! Set the container prototype.
   * \param[in] the container prototype.
   */
  void set_prototype(Container_proto* prototype);

private:
  /*! The tag that identifies this container type. */
  std::string m_tag;

  //! The container prototype.
  Container_proto* m_prototype;
};

//! \brief constructs the prototype container.
inline Proto* Proto::prototype() { return new Proto(true); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Proto::get_tag() const { return m_tag; }

//! \brief sets the tag (type) of the container.
inline void Proto::set_tag(const String& tag) { m_tag = tag; }

//! \brief sets the container prototype.
inline void Proto::set_prototype(Container_proto* prototype)
{ m_prototype = prototype; }

SGAL_END_NAMESPACE

#endif
