// Copyright (c) 2007 Israel.
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
// $Id: Arrangement_on_surface_geo_overlay.hpp 6681 2008-07-27 12:01:54Z efif $
// $Revision: 6681 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A pure geometry container that represents an arrangement embeded on a
 * parametric surface
 */

#ifndef SGAL_ARRANGEMENT_ON_SURFACE_GEO_OVERLAY_HPP
#define SGAL_ARRANGEMENT_ON_SURFACE_GEO_OVERLAY_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Overlay the arrangements in the given range and store the result in
 * the given arrangement.
 */
template <typename Input_iterator, typename Difference, typename Aos_geo>
void overlay_all(Input_iterator begin, Input_iterator end,
                 Difference diff, Aos_geo * res_geo)
{
  typedef typename Aos_geo::Arrangement_on_surface       Aos;
  
  if (begin == end) return;

  Input_iterator it;
  for (it = begin; it != end; ++it) {
    Aos_geo * aos_geo = (*it);
    if (aos_geo->is_dirty()) aos_geo->clean();
  }

  Aos * res_aos = res_geo->get_aos();
  if (res_geo->get_aos()->is_empty() && (1 == diff)) {
    // There is exactly one item in the range.
    // The arrangement to store the result in is empty.
    // Simply copy the single arrangement from the range
    *res_aos = *((*begin)->get_aos());
    return;
  }
  
  Aos copy_aos;
  Aos * aos1;
  if (res_aos->is_empty()) {
    Aos_geo * aos_geo = *begin++;
    aos1 = aos_geo->get_aos();
  } else {
    copy_aos = *res_aos;
    res_aos->clear();
    aos1 = &copy_aos;
  }

  Aos_geo * aos_geo2 = *begin++;
  Aos * aos2 = aos_geo2->get_aos();
  Aos tmp_aos1;
  Aos tmp_aos2;
  while (begin != end) {
    tmp_aos1.clear();
    CGAL::overlay(*aos1, *aos2, tmp_aos1, res_geo->get_overlay_traits());
    aos1 = &tmp_aos1;
    aos_geo2 = *begin++;
    aos2 = aos_geo2->get_aos();

    if (begin == end) break;
    tmp_aos2.clear();
    CGAL::overlay(*aos1, *aos2, tmp_aos2, res_geo->get_overlay_traits());
    aos1 = &tmp_aos2;
    aos_geo2 = *begin++;
    aos2 = aos_geo2->get_aos();
  }
  CGAL::overlay(*aos1, *aos2, *res_aos, res_geo->get_overlay_traits());
}

SGAL_END_NAMESPACE

#endif
