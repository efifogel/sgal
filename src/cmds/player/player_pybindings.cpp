// Copyright (c) 2015 Israel.
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

#include <exception>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/exception_translator.hpp>

#include "SGAL/sgal.hpp"
#include "SGAL/Bounding_box.hpp"
#include "SGAL/Bounding_sphere.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Loader_errors.hpp"

#include "Player.hpp"

PyObject* exception_type = nullptr;

void translate_exception(std::exception const& e)
{ PyErr_SetString(PyExc_RuntimeError, e.what()); }

void translate_loader_error_exception(SGAL::Loader_error const& e)
{
  boost::python::object python_exception_instance(e);
  PyErr_SetObject(exception_type, python_exception_instance.ptr());
}

BOOST_PYTHON_MODULE(player)
{
  using namespace boost::python;

  register_exception_translator<std::exception>(&translate_exception);

  class_<SGAL::Loader_error> loader_error_class("Loader_error",
                                                init<const std::string&,
                                                     const std::string&>());
  exception_type = loader_error_class.ptr();
  register_exception_translator<SGAL::Loader_error>(&translate_loader_error_exception);
  loader_error_class
    .def("filename", &SGAL::Loader_error::filename,
         return_value_policy<copy_const_reference>())
    .def("what", &SGAL::Loader_error::what)
    ;

  class_<Player::Arguments>("Arguments")
    .def(vector_indexing_suite<Player::Arguments>() )
    ;

  class_<Player>("Player", init<Player::Arguments>())
    .def("__call__", static_cast<void(Player::*)()>(&Player::operator()))
    .def("__call__",
         static_cast<void(Player::*)(char*, int)>(&Player::operator()))
    .def("create", static_cast<void(Player::*)()>(&Player::create))
    .def("createFromData",
         static_cast<void(Player::*)(char*, int)>(&Player::create))
    .def("visualize", &Player::visualize)
    .def("destroy", &Player::destroy)
    .def("volume", &Player::volume)
    .def("surface_area", &Player::surface_area)
    .def("get_polyhedron_attributes_array",
         &Player::get_polyhedron_attributes_array,
         return_value_policy<reference_existing_object>())
    ;

  class_<SGAL::Polyhedron_attributes_array>("Polyhedron_attributes_array")
    .def("number_of_vertices",
         &SGAL::Polyhedron_attributes_array::number_of_vertices)
    .def("number_of_facets",
         &SGAL::Polyhedron_attributes_array::number_of_facets)
    .def("volume", &SGAL::Polyhedron_attributes_array::volume)
    .def("surface_area", &SGAL::Polyhedron_attributes_array::surface_area)
    .def("bounding_box", &SGAL::Polyhedron_attributes_array::bounding_box)
    .def("bounding_sphere", &SGAL::Polyhedron_attributes_array::bounding_sphere)
    .def("is_valid", &SGAL::Polyhedron_attributes_array::is_valid)
    ;

  class_<SGAL::Bounding_box>("Bounding_box")
    .def("xmin", &SGAL::Bounding_box::xmin)
    .def("ymin", &SGAL::Bounding_box::ymin)
    .def("zmin", &SGAL::Bounding_box::zmin)
    .def("xmax", &SGAL::Bounding_box::xmax)
    .def("ymax", &SGAL::Bounding_box::ymax)
    .def("zmax", &SGAL::Bounding_box::zmax)
    ;

  class_<SGAL::Bounding_sphere>("Bounding_sphere")
    .def("radius", &SGAL::Bounding_sphere::get_radius)
    .def("center", &SGAL::Bounding_sphere::get_center,
         return_value_policy<reference_existing_object>())
    ;

  class_<SGAL::Vector3f>("Vector3f")
    .def("x", &SGAL::Vector3f::x)
    .def("y", &SGAL::Vector3f::y)
    .def("z", &SGAL::Vector3f::z)
    ;
}
