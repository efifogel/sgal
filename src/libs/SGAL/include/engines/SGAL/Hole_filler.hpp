// Copyright (c) 2015 Israel.
// All rights reserved.
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! An implementation of the Repairer engine.
 * Inherits from Node.
 */

#ifndef SGAL_HOLE_FILLER_HPP
#define SGAL_HOLE_FILLER_HPP

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/variant.hpp>

#include <CGAL/basic.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Inexact_polyhedron.hpp"
#include "SGAL/Epic_polyhedron.hpp"
#include "SGAL/Epec_polyhedron.hpp"

SGAL_BEGIN_NAMESPACE

namespace PMP = CGAL::Polygon_mesh_processing;

class Field_info;
class Container_proto;
class Mesh_set;

/*! \class Hole_filler Hole_filler.hpp
 * Hole_filler is an engine node in the scene graph that fills holes if exists.
 * Holes are filled, possibly refined, and possibly faired one after the other;
 * the process stops when there are no border edges left.
 */
class SGAL_SGAL_DECL Hole_filler : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    SURFACE,
    REFINE,
    FAIR,
    RESULT,
    TRIGGER,
    LAST
  };

  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;

  /*! Construct.
   */
  Hole_filler(Boolean proto = false);

  /*! Destruct.
   */
  virtual ~Hole_filler();

  /* Construct the prototype */
  static Hole_filler* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Shared_mesh_set* surface_handle(const Field_info*) { return &m_surface; }
  Boolean* refine_handle(const Field_info*) { return &m_refine; }
  Boolean* fair_handle(const Field_info*) { return &m_fair; }
  Shared_mesh_set* result_handle(const Field_info*) { return &m_result; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Export this container.
   * \param[in] formatter the formatter to use for exporting; e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! triggers the execution of the engine as a response to change in one
   * of the input fields.
   */
  void trigger_changed(const Field_info* field_info);

  /*! Execute the engine. */
  void execute();

  /*! Set the input surface, which is a mesh that might be open (may have holes).
   * \param surface the input surface.
   */
  void set_surface(Shared_mesh_set surface);

  /*! Obtain the input surface, which is a mesh that might be open (may have
   * holes).
   * \return the input surface.
   */
  Shared_mesh_set get_surface() const;

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
   * \return true if the fairing of the triangulations of holes is desired,
   *         and false otherwise.
   */
  Boolean get_fair() const;

  /*! Set the result, which is a closed mesh (a surface without holes).
   * \param[in] result the resulting closed mesh.
   */
  void set_result(Shared_mesh_set result);

  /*! Obtain the result, which is a closed mesh (a surface without holes).
   * \param[in] result the resulting closed mesh.
   */
  Shared_mesh_set get_result() const;

  /*! The input polygonal mesh, which might be open. */
  Shared_mesh_set m_surface;

  /*! Indicates whether to refine the triangulation of a hole by applying
   * local averaging rules.
   */
  Boolean m_refine;

  /*! Indicates whether to smooth the triangulation of a hole to obtain
   * as-smooth-as-possible shape deformation.
   */
  Boolean m_fair;

  //! The resulting polygonal closed mesh.
  Shared_mesh_set m_result;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  //! Trigger of the engine that makes the engine excute.
  Boolean m_trigger;

  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The prototype of this node.
  static Container_proto* s_prototype;

  //! Default values.
};

//! \brief constructs the prototype.
inline Hole_filler* Hole_filler::prototype() { return new Hole_filler(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Hole_filler::create() { return new Hole_filler(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Hole_filler::get_tag() const { return s_tag; }

//! \brief obtains the input surface, which is a mesh that might be open.
inline Hole_filler::Shared_mesh_set Hole_filler::get_surface() const
{ return m_surface; }

//! \brief sets the input surface, which is a mesh that might be open.
inline void Hole_filler::set_surface(Shared_mesh_set surface)
{ m_surface = surface; }

//! \brief sets the result, which is a closed mesh.
inline void Hole_filler::
set_result(Shared_mesh_set result) { m_result = result; }

//! \brief obtains the result, which is a closed mesh.
inline Hole_filler::Shared_mesh_set Hole_filler::get_result() const
{ return m_result; }

/*! \brief sets the flag that indicates whether to refine the triangulations of
 * holes.
 */
inline void Hole_filler::set_refine(Boolean flag) { m_refine = flag; }

/*! \brief obtains the flag that indicates whether to refine the triangulations
 * of holes.
 */
inline Boolean Hole_filler::get_refine() const { return m_refine; }

/*! \brief sets the flag that indicates whether to fair (smooth) the
 * triangulations of holes.
 */
inline void Hole_filler::set_fair(Boolean flag) { m_fair = flag; }

/*! \brief obtains the flag that indicates whether to fair (smooth) the
 * triangulations of holes.
 */
inline Boolean Hole_filler::get_fair() const { return m_fair; }

SGAL_END_NAMESPACE

#endif
