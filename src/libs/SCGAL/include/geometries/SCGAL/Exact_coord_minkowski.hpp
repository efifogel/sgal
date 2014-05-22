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

#ifndef SCGAL_EXACT_COORD_MINKOWSKI_HPP
#define SCGAL_EXACT_COORD_MINKOWSKI_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;
class Exact_coord_array;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Add two coord arrays. */
class SGAL_SCGAL_DECL Exact_coord_minkowski : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    EXECUTE,
    COORD1,
    COORD2,
    COORD_CHANGED,
    LAST
  };

  typedef boost::shared_ptr<Exact_coord_array>  Shared_exact_coord_array;

  /*! Constructor. */
  Exact_coord_minkowski(Boolean proto = false);

  /*! Destructor. */
  virtual ~Exact_coord_minkowski();

  /* Construct the prototype. */
  static Exact_coord_minkowski* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* execute_handle(const Field_info*) { return &m_execute; }
  Shared_exact_coord_array* coord_array1_handle (const Field_info*)
  { return &m_coord_array1; }
  Shared_exact_coord_array* coord_array2_handle (const Field_info*)
  { return &m_coord_array2; }
  Shared_exact_coord_array* coord_array_changed_handle(const Field_info*)
  { return &m_coord_array_changed; }
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Transform the input vertices and store the results in the output
   * vertices.
   * \param field_info the field information record.
   */
  void execute(const Field_info* field_info = nullptr);

  /*! Set the coordinate-set node. */
  void set_coord_array1(Shared_exact_coord_array coord);

  /*! Obtain the coordinate-set node. */
  Shared_exact_coord_array get_coord_array1() const;

  /*! Set the coordinate-set node. */
  void set_coord_array2(Shared_exact_coord_array coord);

  /*! Obtain the coordinate-set node. */
  Shared_exact_coord_array get_coord_array2() const;

  /*! Obtain the changed coordinate-set node. */
  Shared_exact_coord_array get_coord_array_changed() const;

protected:
  /*! Determines whether the node is enabled. */
  Boolean m_enabled;

  /*! Indicates that the operation should be executed. */
  Boolean m_execute;

  /*! The input vertices. */
  Shared_exact_coord_array m_coord_array1;

  /*! The input vertices. */
  Shared_exact_coord_array m_coord_array2;

  /*! The output vertices. */
  Shared_exact_coord_array m_coord_array_changed;

  // default values.
  static const Boolean s_def_enabled;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief destructor.
inline Exact_coord_minkowski::~Exact_coord_minkowski() {}

//! \brief constructs the prototype.
inline Exact_coord_minkowski* Exact_coord_minkowski::prototype()
{ return new Exact_coord_minkowski(true); }

//! \brief clones.
inline Container* Exact_coord_minkowski::clone()
{ return new Exact_coord_minkowski(); }

//! \brief sets the coordinate-set node.
inline void Exact_coord_minkowski::
set_coord_array1(Shared_exact_coord_array coord)
{ m_coord_array1 = coord; }

//! \brief obtains the coordinate-set node.
inline Exact_coord_minkowski::Shared_exact_coord_array
Exact_coord_minkowski::get_coord_array1() const
{ return m_coord_array1; }

//! \brief sets the coordinate-set node.
inline void Exact_coord_minkowski::
set_coord_array2(Shared_exact_coord_array coord)
{ m_coord_array2 = coord; }

//! \brief obtains the coordinate-set node.
inline Exact_coord_minkowski::Shared_exact_coord_array
Exact_coord_minkowski::get_coord_array2() const
{ return m_coord_array2; }

//! \brief obtains the changed coordinate-set node.
inline Exact_coord_minkowski::Shared_exact_coord_array
Exact_coord_minkowski::get_coord_array_changed() const
{ return m_coord_array_changed; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Exact_coord_minkowski::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
