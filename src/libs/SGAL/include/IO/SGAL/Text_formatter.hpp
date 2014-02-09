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

#ifndef SGAL_TEXT_FORMATTER_HPP
#define SGAL_TEXT_FORMATTER_HPP

/*! \file
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

/*! A pure class that can be the base of specific formatter that exports a
 * scene graph to an output stream.
 */
class SGAL_SGAL_DECL Text_formatter : public Formatter {
public:
  /*! Constructor
   */
  Text_formatter();

  /*! Construct an output formatter from an output stream.
   * \param os the output stream.
   */
  Text_formatter(std::ostream& os);

  /*! Construct an input formatter from an input stream.
   * \param is the input stream.
   */
  Text_formatter(std::istream& is);

  /*! Destructor */
  virtual ~Text_formatter();

protected:
  /*! Obtain the indentation level.
   */
  Uint get_indent() const;

  /*! Write an indentation.
   */
  void indent();

  /*! Begin a new line.
   */
  void new_line();

  /*! Pop the indentation level.
   */
  void pop_indent();

  /*! Push the indentation level.
   */
  void push_indent();

private:
  /*! The indentation level */
  Uint m_indent;

  /*! Indicates whether indentation has been applied */
  Boolean m_indented;
};

//! \brief obtains the indentation level.
inline Uint Text_formatter::get_indent() const { return m_indent; }

//! \brief pops the indentation level.
inline void Text_formatter::pop_indent() { --m_indent; }

//! \brief pushes the indentation level.
inline void Text_formatter::push_indent() { ++m_indent; }

SGAL_END_NAMESPACE

#endif
