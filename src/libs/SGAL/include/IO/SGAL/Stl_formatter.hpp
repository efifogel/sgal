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
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_STL_FORMATTER_HPP
#define SGAL_STL_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <string>
#include <stack>

#include "SGAL/basic.hpp"
#include "SGAL/Text_formatter.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Vector3f;
class Container;

/*! Writes a scene graph to an output stream in the VRML format */
class SGAL_SGAL_DECL Stl_formatter : public Text_formatter {
public:
  /*! Constructor */
  Stl_formatter();

  /*! Construct an output formatter from an output stream.
   * \param os the output stream.
   */
  Stl_formatter(std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param is the input stream.
   */
  Stl_formatter(std::istream& is);

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
  virtual void write(Container* container);

  /*! Export a triangular facet.
   * \param p1 (in) the point (in world coordinate system) of the first vertex.
   * \param p2 (in) the point (in world coordinate system) of the second vertex.
   * \param p3 (in) the point (in world coordinate system) of the third vertex.
   * \param normal (in) the facet normal.
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

//! \brief obtains the viewing matrix at the top of the stack.
inline const Matrix4f& Stl_formatter::top_matrix() const
{ return m_matrices.top(); }

SGAL_END_NAMESPACE

#endif
