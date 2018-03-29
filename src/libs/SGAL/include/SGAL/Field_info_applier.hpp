// Copyright (c) 2018 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_INFO_APPLIER_HPP
#define SGAL_FIELD_INFO_APPLIER_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

template <typename T>
struct Field_info_applier {
  /*! Apply an unary operation of a single filed.
   * \param[in] handle the field handle.
   * \param[in] op the unary operation.
   */
  template <typename FieldApplier>
  void operator()(const T& handle, FieldApplier& op) const { op(handle); }
};

template <typename T>
struct Field_info_applier<std::vector<T> > {
  /*! Apply an unary operation on every element of a vector of fields.
   * \param[in] vect the multi field.
   * \param[in] op the unary operation.
   */
  template <typename FieldApplier>
  void operator()(const std::vector<T>& vect, FieldApplier& op) const
  {
    /*! Observe that the call:
     *   std::for_each(vect.begin(), vect.end(), op);
     * will not do the job, because it is interpreted as the a call using
     * the lambda expression:
     *   [](decltype(*std::begin(vect)) handle){ op(handle); });
     * which captures 'op' by copy.
     */
    std::for_each(vect.begin(), vect.end(),
                  [&op](decltype(*std::begin(vect)) handle) { op(handle); });
  }
};

SGAL_END_NAMESPACE

#endif
