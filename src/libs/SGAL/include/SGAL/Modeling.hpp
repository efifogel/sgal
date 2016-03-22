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
    MAKE_CONSISTENT,
    TRIANGULATE_HOLES,
    REFINE,
    FAIR,
    SPLIT_CCS,
    REMOVE_DEGENERACIES,
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
  Boolean* make_consistent_handle(const Field_info*)
  { return &m_make_consistent; }
  Boolean* triangulate_holes_handle(const Field_info*)
  { return &m_triangulate_holes; }
  Boolean* refine_handle(const Field_info*) { return &m_refine; }
  Boolean* fair_handle(const Field_info*) { return &m_fair; }
  Boolean* split_ccs_handle(const Field_info*) { return &m_split_ccs; }
  Boolean* remove_degeneracies_handle(const Field_info*)
  { return &m_remove_degeneracies; }
  Boolean* repair_orientation_handle(const Field_info*)
  { return &m_repair_orientation; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Set the flag that indicates whether to repair consistency.
   * \param[in] flag indicates whether to enable consistency repairing.
   */
  void set_make_consistent(Boolean flag);

  /*! Obtain the flag that indicates whether to repair consistency.
   * \return indicates whether the consistency repairing desired.
   */
  Boolean get_make_consistent() const;

  /*! Set the flag that indicates whether to triangulate holes.
   * \param[in] flag indicates whether to triangulate holes.
   */
  void set_triangulate_holes(Boolean flag);

  /*! Obtain the flag that indicates whether to triangulate holes.
   * \return indicates whether the triangulations of holes is desired.
   */
  Boolean get_triangulate_holes() const;

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

  /*! Set the flag that indicates Determine to remove shapes the geometry of
   * which is degenerate; that is, the volume of the geometry has zero volume.
   * \param[in] flag indicates whether the removing is desired.
   */
  void set_remove_degeneracies(Boolean flag);

  /*! Determine whether to remove shapes the geometry of which is degenerate;
   * that is, the volume of the geometry has zero volume.
   * \return true if removing is desired, and false otherwise.
   */
  Boolean get_remove_degeneracies() const;

  /*! Set the flag that indicates whether to repair the orientation of facets
   * of closed polyhedral surfaces.
   * \param[in] flag indicates whether the repairing is desired
   */
  void set_repair_orientation(Boolean flag);

  /*! Obtain the flag that indicates whether to repair the orientation of facets
   * of closed polyhedral surfaces.
   * \return true if the repairing is desired, and false otherwise.
   */
  Boolean get_repair_orientation() const;

  /*! Set defualt values. */
  void reset(Boolean def_make_consistent = s_def_make_consistent,
             Boolean def_triangulate_holes = s_def_triangulate_holes,
             Boolean def_refine = s_def_refine,
             Boolean def_fair = s_def_fair,
             Boolean def_split_ccs = s_def_split_ccs,
             Boolean def_remove_degeneracies = s_def_remove_degeneracies,
             Boolean def_repair_orientation = s_def_repair_orientation);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether to repair consistency of facet-orientation. */
  Boolean m_make_consistent;

  /*! Indicates whether to triangulate a hole thereby filling it. */
  Boolean m_triangulate_holes;

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

  /*! Indicates whether to remove shapes the geometry of which is degenerate;
   * that is, the geometry has zero volume.
   */
  Boolean m_remove_degeneracies;

  /*! Indicates whether to repair the orientation of facets of closed polyhedral
   * surface.
   */
  Boolean m_repair_orientation;

  // default values
  static const Boolean s_def_make_consistent;
  static const Boolean s_def_triangulate_holes;
  static const Boolean s_def_refine;
  static const Boolean s_def_fair;
  static const Boolean s_def_split_ccs;
  static const Boolean s_def_remove_degeneracies;
  static const Boolean s_def_repair_orientation;
};

//! \brief constructs the prototype.
inline Modeling* Modeling::prototype() { return new Modeling(true); }

//! \brief clones.
inline Container* Modeling::clone() { return new Modeling(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Modeling::get_tag() const { return s_tag; }

//! \brief sets the flag that indicates whether to repair consistency.
inline void Modeling::set_make_consistent(Boolean flag)
{ m_make_consistent = flag; }

//! \brief obtains the flag that indicates whether to repair consistency.
inline Boolean Modeling::get_make_consistent() const
{ return m_make_consistent; }

//! \brief sets the flag that indicates whether to triangulate holes.
inline void Modeling::set_triangulate_holes(Boolean flag)
{ m_triangulate_holes = flag; }

//! \brief obtains the flag that indicates whether to triangulate holes.
inline Boolean Modeling::get_triangulate_holes() const
{ return m_triangulate_holes; }

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

/*! \brief sets the flag that indicates Determine to remove shapes the geometry
 * of which is degenerate; that is, the volume of the geometry has zero volume.
 */
inline void Modeling::set_remove_degeneracies(Boolean flag)
{ m_remove_degeneracies = flag; }

/*! \brief determines whether to remove shapes the geometry of which is
 * degenerate; that is, the volume of the geometry has zero volume.
 */
inline Boolean Modeling::get_remove_degeneracies() const
{ return m_remove_degeneracies; }

SGAL_END_NAMESPACE

#endif
