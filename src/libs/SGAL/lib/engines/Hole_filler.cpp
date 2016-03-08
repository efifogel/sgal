// Copyright (c) 2015 Israel.
// All rights reserved.
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Hole_filler.hpp"
#include "SGAL/Hole_filler_visitor.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Modeling.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Hole_filler::s_tag = "HoleFiller";
Container_proto* Hole_filler::s_prototype(nullptr);

// Default values:

REGISTER_TO_FACTORY(Hole_filler, "Hole_filler");

//! \brief constructs
Hole_filler::Hole_filler(Boolean proto) :
  Node(proto),
  m_trigger(false),
  m_refine(Modeling::s_def_refine),
  m_fair(Modeling::s_def_refine)
{}

//! \brief destruct.
Hole_filler::~Hole_filler() {}

/*! \brief triggers the execution of the engine as a response to change in one
 * of the input fields.
 */
void Hole_filler::trigger_changed(const Field_info* /* field_info */)
{ execute(); }

//! \brief initializes the container prototype.
void Hole_filler::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func =
    static_cast<Execution_function>(&Hole_filler::trigger_changed);

  auto trigger_func =
    static_cast<Boolean_handle_function>(&Hole_filler::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          Field_info::RULE_IN,
                                          trigger_func,
                                          exec_func));

  // surface
  auto surface_func = reinterpret_cast<Shared_container_handle_function>
    (&Hole_filler::surface_handle);
  s_prototype->add_field_info(new SF_shared_container(SURFACE, "surface",
                                                      Field_info::RULE_IN,
                                                      surface_func,
                                                      exec_func));
  // refine
  auto refine_func =
    static_cast<Boolean_handle_function>(&Hole_filler::refine_handle);
  s_prototype->add_field_info(new SF_bool(REFINE, "refine",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          refine_func, Modeling::s_def_refine,
                                          exec_func));

  // fair
  auto fair_func =
    static_cast<Boolean_handle_function>(&Hole_filler::fair_handle);
  s_prototype->add_field_info(new SF_bool(FAIR, "fair",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          fair_func, Modeling::s_def_fair,
                                          exec_func));

  // result
  auto result_func = reinterpret_cast<Shared_container_handle_function>
    (&Hole_filler::result_handle);
  s_prototype->add_field_info(new SF_shared_container(RESULT, "result",
                                                      Field_info::RULE_OUT,
                                                      result_func));
}

//! \brief deletes the container prototype.
void Hole_filler::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Hole_filler::get_prototype()
{
  if (!s_prototype) Hole_filler::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Hole_filler::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "refine") {
      set_refine(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fair") {
      set_fair(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  auto cai = elem->cont_attrs_begin();
  for (; cai != elem->cont_attrs_end(); ++cai) {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "surface") {
      auto mesh = boost::dynamic_pointer_cast<Mesh_set>(cont);
      set_surface(mesh);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "result") {
      auto result = boost::dynamic_pointer_cast<Mesh_set>(cont);
      set_result(result);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief executes the engine.
void Hole_filler::execute()
{
  if (!m_surface) return;

  if (!m_result) m_result = Shared_mesh_set(new Indexed_face_set);

  auto surface = boost::dynamic_pointer_cast<Indexed_face_set>(m_surface);
  SGAL_assertion(surface);

  auto result = boost::dynamic_pointer_cast<Indexed_face_set>(m_result);
  SGAL_assertion(result);

  if (result != surface) {
    result->set_coord_array(surface->get_coord_array());
    auto& indices = surface->get_facet_coord_indices();
    result->set_facet_coord_indices(indices);
    result->set_num_primitives(surface->get_num_primitives());
    result->set_primitive_type(surface->get_primitive_type());
  }

  // std::cout << surface->get_number_of_border_edges() << std::endl;
  if (!surface->is_closed()) {
    // Fill the holes.
    surface->set_polyhedron_type(Indexed_face_set::POLYHEDRON_EPIC);

    // We obtain the polyhedron by const reference, cast way constness, and
    // apply the visitor on it. Then, we need to invalidate all fields that
    // depend on the altered polyhedron. This is a bit risky, cause a new field
    // might be introduced.
    // A safer option, but less efficient, is to obtain the polyhedron by value,
    // apply the visotor, and then set the polyhedron back:
    // result->set_polyhedron(polyhedron);
    // The last statement willl invalidate all necessary fields.

    const auto& polyhedron = surface->get_polyhedron();
    Hole_filler_visitor visitor(m_refine, m_fair);
    boost::apply_visitor(visitor,
                         const_cast<Indexed_face_set::Polyhedron&>(polyhedron));
    result->clear_volume();
    result->clear_surface_area();
    result->clear_polyhedron_facet_normals();
    result->clear_normal_attributes();

    //! \todo fix the arrays instead of brutally invalidate them.
    result->clear_coord_array();
    result->clear_coord_indices();
    result->clear_facet_coord_indices();
  }

  // Cascade the result field:
  auto* field = get_field(RESULT);
  if (field) field->cascade();
}

//! \brief writes this container.
void Hole_filler::write(Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) Node::write(formatter);
}

SGAL_END_NAMESPACE
