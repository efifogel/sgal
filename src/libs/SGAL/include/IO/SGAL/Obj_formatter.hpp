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

#ifndef SGAL_OBJ_FORMATTER_HPP
#define SGAL_OBJ_FORMATTER_HPP

/*! \file
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <set>
#include <boost/shared_ptr.hpp>

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

/*! \class Obj_formatter Obj_formatter.hpp
 * Obj_formater formats the scene, which is exported to an output stream,
 * in the OBJ format.
 */
class SGAL_SGAL_DECL Obj_formatter : public Text_formatter {
public:
  /*! Construct default. */
  Obj_formatter(const std::string& filename);

  /*! Construct an output formatter from an output stream.
   * \param[in] os the output stream.
   */
  Obj_formatter(const std::string& filename, std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param is the input stream.
   */
  Obj_formatter(const std::string& filename, std::istream& is);

  /*! Destructor */
  virtual ~Obj_formatter();

  /// \name Export functions
  //@{

  /*! Export the headers of the scene graph.
   */
  virtual void begin();

  /*! Export the routing statements.
   */
  virtual void end();

  /*! Export a scene-graph container.
   * \param[in] container The container to export.
   */
  virtual void write(Shared_container container);

  /*! Export a vertex.
   * \param[in] vertex vertex (in world coordinate system).
   */
  virtual void vertex(const Vector3f& vertex);

  /*! Export a triangular facet.
   * \param[in] i0 the index of the first vertex.
   * \param[in] i1 the index of the second vertex.
   * \param[in] i2 the index of the third vertex.
   */
  virtual void triangle(Uint i0, Uint i1, Uint i2);

  /*! Export a triangular facet.
   * \param[in] i0 the index of the first vertex.
   * \param[in] i1 the index of the second vertex.
   * \param[in] i2 the index of the third vertex.
   * \param[in] i3 the index of the fourth vertex.
   */
  virtual void quad(Uint i0, Uint i1, Uint i2, Uint i3);
  //@}

  /*! Obtain the viewing matrix at the top of the stack.
   * \return the viewing matrix at the top of the stack.
   */
  const Matrix4f& top_matrix() const;

  /*! Add a given size to the last recorded index.
   * \param[in] size the number to add to the last recorded index.
   */
  void add_index(Uint size);

  /*! Obtain the output material stream.
   * \return the output material stream.
   */
  inline std::ostream& material_out();

private:
  //! The stack of viewing matrices.
  std::stack<Matrix4f> m_matrices;

  /*! The output material stream. */
  std::ofstream m_material_out;

  //! The last coordinate index.
  Uint m_index;

  //! The number of appearances.
  size_t m_num_appearances;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the viewing matrix at the top of the stack.
inline const Matrix4f& Obj_formatter::top_matrix() const
{ return m_matrices.top(); }

//! \brief adds a given size to the last recorded index.
inline void Obj_formatter::add_index(Uint size) { m_index += size; }

//! \brief Obtain the output stream. */
inline std::ostream& Obj_formatter::material_out()
{ return m_material_out; }

SGAL_END_NAMESPACE

#endif
