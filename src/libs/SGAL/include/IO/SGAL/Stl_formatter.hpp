// Copyright (c) 2014 Israel.
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

#ifndef SGAL_STL_FORMATTER_HPP
#define SGAL_STL_FORMATTER_HPP

#include <iostream>
#include <string>
#include <stack>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Vector3f;
class Container;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Stl_formatter Stl_formatter.hpp
 * Stl_formater formats the scene, which is exported to an output stream,
 * in the STL format.
 */
class SGAL_SGAL_DECL Stl_formatter : public Text_formatter {
public:
  /*! Construct from file name.
   * \param[in] filename The file name.
   */
  Stl_formatter(const std::string& filename);

  /*! Construct an output formatter from an output stream.
   * \param[in] filename The file name.
   * \param[in] os the output stream.
   */
  Stl_formatter(const std::string& filename, std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param[in] filename The file name.
   * \param[in] is the input stream.
   */
  Stl_formatter(const std::string& filename, std::istream& is);

  /*! Destructor */
  virtual ~Stl_formatter();

  /// \name Export functions
  //@{

  /*! Export the headers of the scene graph.
   */
  virtual void begin();

  /*! Export the routing statements.
   */
  virtual void end();

  /*! Export a scene-graph container.
   */
  virtual void write(Shared_container container);

  /*! Export a triangular facet.
   * \param[in] p1 the point (in world coordinate system) of the first vertex.
   * \param[in] p2 the point (in world coordinate system) of the second vertex.
   * \param[in] p3 the point (in world coordinate system) of the third vertex.
   * \param[in] normal the facet normal.
   */
  virtual void facet(const Vector3f& local_p1, const Vector3f& local_p2,
                     const Vector3f& local_p3, const Vector3f& normal);

  //@}

  /*! Obtain the viewing matrix at the top of the stack.
   * \return the viewing matrix at the top of the stack.
   */
  const Matrix4f& top_matrix() const;

private:
  //! The stack of viewing matrices.
  std::stack<Matrix4f> m_matrices;

  /*! Export a vertex.
   * \param p the vertex point.
   */
  void vertex(const Vector3f& p);

  /*! Export a facet header.
   * \param normal (in) the facet normal.
   */
  void facet_begin(const Vector3f& normal);

  /*! Export a facet trailer.
   */
  void facet_end();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the viewing matrix at the top of the stack.
inline const Matrix4f& Stl_formatter::top_matrix() const
{ return m_matrices.top(); }

SGAL_END_NAMESPACE

#endif
