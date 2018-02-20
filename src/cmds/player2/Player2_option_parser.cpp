// Copyright (c) 2004 Israel.
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

#include "Player2_option_parser.hpp"

namespace po = boost::program_options;

/*! Constructor */
Player2_option_parser::Player2_option_parser()
{
#if 0
  // For some reason the code below doesn't work. This is a workaround
  po::option_description * vp_scale =
    new po::option_description("viewport-scale",
                               po::value<SGAL::Float>(&m_viewport_scale)->
                                 default_value(1.0f),
                               "scale viewport");
  m_player_opts.add(boost::shared_ptr<po::option_description>(vp_scale));

  po::option_description * vp_margin =
    new po::option_description("viewport-margin",
                               po::value<SGAL::Int>(&m_viewport_margin)->
                                 default_value(0),
                               "mrgin viewport");
  m_player_opts.add(boost::shared_ptr<po::option_description>(vp_margin));
#else
  m_player2_opts.add_options()
    ("viewport-scale",
     po::value<SGAL::Float>(&m_viewport_scale)->default_value(1.0f),
     "scale viewport")
    ("viewport-margin",
     po::value<SGAL::Int>(&m_viewport_margin)->default_value(0),
     "mrgin viewport")
    ;
  m_player_opts.add(m_player2_opts);
#endif
}
