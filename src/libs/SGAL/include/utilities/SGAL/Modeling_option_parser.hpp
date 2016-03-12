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

#ifndef SGAL_MODELING_OPTION_PARSER_HPP
#define SGAL_MODELING_OPTION_PARSER_HPP

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512 )
#endif
#include <boost/program_options.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Array_types.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class Configuration;

class SGAL_SGAL_DECL Modeling_option_parser {
public:
  /*! Construct default.
   */
  Modeling_option_parser();

  /*! Destruct.
   */
  virtual ~Modeling_option_parser();

  /*! Obtain the variable map.
   * \return the variable map.
   */
  virtual const po::variables_map& get_variable_map() const = 0;

  /*! Apply the options
   */
  void apply();

  /*! Obtain the modeling-option description.
   * \return the modeling-option description.
   */
  inline const po::options_description& get_modeling_opts() const;

  /*! Configure. */
  void configure(Configuration* conf);

  /*! Determine whether to triangulate holes.
   * \return true if the triangulations of holes is desired, and false otherwise.
   */
  Boolean get_triangulate() const;

  /*! Determine whether to refine the triangulation of holes.
   * \return true if the refinement of the triangulations of holes is desired,
   *         and false otherwise.
   */
  Boolean get_refine() const;

  /*! Determine whether to fair (smooth) the triangulations of holes.
   * \return true if the fairing of the triangulations of holes is desired,
   *         and false otherwise.
   */
  Boolean get_fair() const;

  /*! Determine whether to split connected components.
   * \return true if splitting connected components is desired, and false
   *         otherwise.
   */
  Boolean get_split_ccs() const;

  /*! Determine whether to repair the orientation of facets of closed polyhedral
   * surfaces.
   * \return true if repairing is desired, and false otherwise.
   */
  Boolean get_repair_orientation() const;

  /*! Obtain the font paths.
   * \param variable_map (in)
   * \return the font paths.
   */
  const String_array& get_paths(const po::variables_map& variable_map) const;

protected:
  /*! The modeling options. */
  po::options_description m_modeling_opts;

private:
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

  /*! Indicates wither whether to split connected components. */
  Boolean m_split_ccs;

  /*! Indicates whether to repair the orientation of facets of closed polyhedral
   * surfaces.
   */
  Boolean m_repair_orientation;

  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  Modeling_option_parser& operator=(const Modeling_option_parser&);
  Modeling_option_parser(const Modeling_option_parser&);

  // In C++11, VC2013, the following is supported:
  // Modeling_option_parser& operator=(const Modeling_option_parser&) = delete;
  // Modeling_option_parser(const Modeling_option_parser&) = delete;
};

//! \brief obtains the window-option description.
inline const po::options_description&
Modeling_option_parser::get_modeling_opts() const { return m_modeling_opts; }

//! \brief determines whether to triangulate holes.
inline Boolean Modeling_option_parser::get_triangulate() const
{ return m_triangulate; }

//! \brief determines whether to refine the triangulation of holes.
inline Boolean Modeling_option_parser::get_refine() const { return m_refine; }

//! \brief determines whether to fair the triangulation of holes.
inline Boolean Modeling_option_parser::get_fair() const {return m_fair; }

//! \brief determines whether to split connected components.
inline Boolean Modeling_option_parser::get_split_ccs() const
{return m_split_ccs; }

/*! \brief determines whether to repair the orientation of facets of closed
 * polyhedral surfaces.
 */
inline Boolean Modeling_option_parser::get_repair_orientation() const
{return m_repair_orientation; }

SGAL_END_NAMESPACE

#endif
