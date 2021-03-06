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
// $Id: $
// $Revision: 5322 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_DEGENERATE_PLANE_3_HPP
#define SCGAL_DEGENERATE_PLANE_3_HPP

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief a plane that goes through the origin extended with a few methods */
template <typename Kernel_>
class SGAL_SCGAL_DECL Degenerate_plane_3 : public Kernel_::Plane_3 {
public:
  typedef Kernel_                       Kernel;
  typedef typename Kernel::Plane_3      Base;
  typedef typename Kernel::Point_2      Point_2;
  typedef typename Kernel::Point_3      Point_3;
  typedef typename Kernel::RT           RT;
  typedef typename Kernel::FT           FT;
  
  /*! Constructor */
  Degenerate_plane_3(const Point_3& p, const Point_3& q, const Point_3& r) :
    Base(p, q, r)
  {}

  /*!  returns the i-th coefficient of the plane */
  RT operator[](Uint i) const
  {
    CGAL_assertion(i < 3);
    return (i == 0) ? a() : ((i == 1) ? b() : c());
  }
  
  /*! Return a 3d point p_3 coincident to the plane, such that the image point
   * of the projection of p_3 under an affine transformation, which maps the
   * plane onto the a given axis-parallel plane is a given 2d point.
   * \param p_2 the image point
   * \param i the index of the axis-parallel plane. 0, 1, or 2 indicate the
   * yz-, zx-, and xy-plane respectively
   */
  Point_3 to_3d(const Point_2 & p_2, Uint i) const
  {
    CGAL_assertion(i < 3);

    // We know that the plane goes throgh the origin
    RT coef_a = a();
    RT coef_b = b();
    RT coef_c = c();

#if 0
    std::cout << "(a, b, c): " << coef_a << "," << coef_b << "," << coef_c
              << std::endl;
#endif
    
    if (i == 0) {
      CGAL_assertion(coef_a != 0);
      FT y = p_2.x();
      FT z = p_2.y();
      FT x = -(coef_b * y + coef_c * z) / coef_a;
      Point_3 p_3(x, y, z);
      return p_3;
    }

    if (i == 1) {
      CGAL_assertion(coef_b != 0);
      FT z = p_2.x();
      FT x = p_2.y();
      FT y = -(coef_a * x + coef_c * z) / coef_b;
      Point_3 p_3(x, y, z);
      return p_3;
    }

    // if (i == 2) return Base::to_3d(p_2);
    CGAL_assertion(coef_c != 0);
    FT x = p_2.x();
    FT y = p_2.y();
    FT z = -(coef_a * x + coef_b * y) / coef_c;
    Point_3 p_3(x, y, z);
    return p_3;
  }
};

/*!
 */
template <typename Kernel>
inline std::ostream& operator<<(std::ostream& os,
                                 const Degenerate_plane_3<Kernel> & plane)
{
  os << plane[0] << ", " << plane[1] << ", " << plane[2];
  return os;
}  

SGAL_END_NAMESPACE

#endif
