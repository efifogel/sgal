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

#ifndef SGAL_FIELD_INFO_TRANSFORMER_HPP
#define SGAL_FIELD_INFO_TRANSFORMER_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

template <typename T>
struct Field_info_transformer {
  template <typename FieldTransformer>
  void operator()(const T& source, T& target, FieldTransformer& op) const
  { target = op(source); }
};

template <typename T>
struct Field_info_transformer<std::vector<T> > {
  template <typename FieldTransformer>
  void operator()(const std::vector<T>& source, std::vector<T>& target,
                  FieldTransformer& op) const
  {
    target.resize(source.size());
    /*! Observe that the call:
     *   std::transform(source.begin(), source.end(), target.begin(), op);
     * will not do the job, because it is interpreted as the a call using
     * the lambda expression:
     *   [](decltype(*std::begin(source)) cont){ return op(cont); });
     * which captures 'op' by copy.
     */
    std::transform(source.begin(), source.end(), target.begin(),
                   [&op](decltype(*std::begin(source)) cont)
                   { return op(cont); });
  }
};

SGAL_END_NAMESPACE

#endif
