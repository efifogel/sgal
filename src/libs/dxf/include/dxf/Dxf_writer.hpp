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

#ifndef DXF_WRITER_HPP
#define DXF_WRITER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Base_writer.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Dxf_writer : public SGAL::Base_writer {
public:
  /*! Write the scene to a file.
   * \param[in] scene_graph the scene graph that represents the scene.
   * \param[in] os the output stream.
   * \param[in] filename the output filename.
   */
  virtual void operator()(SGAL::Scene_graph* scene_graph,
                          std::ostream& os, const std::string& filename,
                          bool is_binary = false);
};

DXF_END_NAMESPACE

#endif
