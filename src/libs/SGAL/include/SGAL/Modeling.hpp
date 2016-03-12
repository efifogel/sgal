// Copyright (c) 2016 Israel.
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

#ifndef SGAL_MODELING_HPP
#define SGAL_MODELING_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;

class SGAL_SGAL_DECL Modeling : public Container {
  friend class Indexed_face_set;
  friend class Hole_filler;
public:
  enum {
    FIRST = Container::LAST - 1,
    TRIANGULATE,
    REFINE,
    FAIR,
    SPLIT_CCS,
    REPAIR_ORIENTATION
  };

  /*! Construct. */
  Modeling(Boolean proto = false);

  /*! Destruct. */
  virtual ~Modeling() {}

  /*! Construct the prototype. */
  static Modeling* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* triangulate_handle(const Field_info*) { return &m_triangulate; }
  Boolean* refine_handle(const Field_info*) { return &m_refine; }
  Boolean* fair_handle(const Field_info*) { return &m_fair; }
  Boolean* split_ccs_handle(const Field_info*) { return &m_split_ccs; }
  Boolean* repair_orientation_handle(const Field_info*)
  { return &m_repair_orientation; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Set the flag that indicates whether to triangulate holes.
   * \param[in] flag indicates whether to triangulate holes.
   */
  void set_triangulate(Boolean flag);

  /*! Obtain the flag that indicates whether to triangulate holes.
   * \return indicates whether the triangulations of holes is desired.
   */
  Boolean get_triangulate() const;

  /*! Set the flag that indicates whether to refine the triangulations of holes.
   * \param[in] flag indicates whether the refinement of the triangulations of
   *                 holes is desired.
   */
  void set_refine(Boolean flag);

  /*! Obtain the flag that indicates whether to refine the triangulations of
   * holes.
   * \return true if the refinement of the triangulations of holes is desired,
   *         and false otherwise.
   */
  Boolean get_refine() const;

  /*! Set the flag that indicates whether to fair (smooth) the triangulations
   * of holes.
   * \param[in] flag indicates whether the fairing of the triangulations of
   *                 holes is desired,
   */
  void set_fair(Boolean flag);

  /*! Obtain the flag that indicates whether to fair (smooth) the
   * triangulations of holes.
   * \return true if the splitting of connected components is desired,
   *         and false otherwise.
   */
  Boolean get_fair() const;

  /*! Set the flag that indicates whether to split connected components.
   * \param[in] flag indicates whether the the splitting of connected
   *            components is desired
   */
  void set_split_ccs(Boolean flag);

  /*! Obtain the flag that indicates whether to split connected components.
   * \return true if splitting connected components is desired, and false
   *         otherwise.
   */
  Boolean get_split_ccs() const;

  /*! Set the flag that indicates whether to repair the orientation of facets
   * of closed polyhedral surfaces.
   * \param[in] flag indicates whether the the repairing is desired
   */
  void set_repair_orientation(Boolean flag);

  /*! Obtain the flag that indicates whether to repair the orientation of facets
   * of closed polyhedral surfaces.
   * \return true if the repairing is desired, and false otherwise.
   */
  Boolean get_repair_orientation() const;

  /*! Set defualt values. */
  void reset(Boolean def_triangulate = s_def_triangulate,
             Boolean def_refine = s_def_refine,
             Boolean def_fair = s_def_fair,
             Boolean def_split_ccs = s_def_split_ccs,
             Boolean def_repair_orientation = s_def_repair_orientation);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether to triangulate a hole thereby filling it. */
  Boolean m_triangulate;

  /*! Indicates whether to refine the triangulation of a hole by applying
   * local averaging rules.
   */
  Boolean m_refine;

  /*! Indicates whether to smooth the triangulation of a hole to obtain
   * as-smooth-as-possible shape deformation.
   */
  Boolean m_fair;

  /*! Indicates whether to split connected components.
   */
  Boolean m_split_ccs;

  /*! Indicates whether to repair the orientation of facets of closed polyhedral
   * surface.
   */
  Boolean m_repair_orientation;

  // default values
  static const Boolean s_def_triangulate;
  static const Boolean s_def_refine;
  static const Boolean s_def_fair;
  static const Boolean s_def_split_ccs;
  static const Boolean s_def_repair_orientation;
};

//! \brief constructs the prototype.
inline Modeling* Modeling::prototype() { return new Modeling(true); }

//! \brief clones.
inline Container* Modeling::clone() { return new Modeling(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Modeling::get_tag() const { return s_tag; }

//! \brief sets the flag that indicates whether to triangulate holes.
inline void Modeling::set_triangulate(Boolean flag) { m_triangulate = flag; }

//! \brief obtains the flag that indicates whether to triangulate holes.
inline Boolean Modeling::get_triangulate() const { return m_triangulate; }

/*! \brief sets the flag that indicates whether to refine the triangulations of
 * holes.
 */
inline void Modeling::set_refine(Boolean flag) { m_refine = flag; }

/*! \brief obtains the flag that indicates whether to refine the triangulations
 * of holes.
 */
inline Boolean Modeling::get_refine() const { return m_refine; }

/*! \brief sets the flag that indicates whether to fair (smooth) the
 * triangulations of holes.
 */
inline void Modeling::set_fair(Boolean flag) { m_fair = flag; }

/*! \brief obtains the flag that indicates whether to fair (smooth) the
 * triangulations of holes.
 */
inline Boolean Modeling::get_fair() const { return m_fair; }

//! Set the flag that indicates whether to split connected components.
inline void Modeling::set_split_ccs(Boolean flag) { m_split_ccs = flag; }

//! Obtain the flag that indicates whether to split connected components.
inline Boolean Modeling::get_split_ccs() const { return m_split_ccs; }

//! Set the flag that indicates whether to repair the orientation of facets.
inline void Modeling::set_repair_orientation(Boolean flag)
{ m_repair_orientation = flag; }

//! Obtain the flag that indicates whether to repair the orientation of facets.
inline Boolean Modeling::get_repair_orientation() const
{ return m_repair_orientation; }

SGAL_END_NAMESPACE

#endif
