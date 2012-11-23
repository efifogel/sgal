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
// $Source: $
// $Revision: 4971 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ISO_CUBOID_BBOX_3_HPP
#define SGAL_ISO_CUBOID_BBOX_3_HPP

// CGAL_BEGIN_NAMESPACE

/*! \brief a parallel axis cuboid bounding-box */
template <class Kernel_>
class Iso_cuboid_bbox_3 {
private:
  typedef Kernel_                               Kernel;
  typedef typename Kernel::FT                   FT;
  typedef typename Kernel::RT                   RT;
  typedef typename Kernel::Point_3              Point_3;
  typedef typename Kernel::Aff_transformation_3 Aff_transformation_3;
  
  /*! the min and max points of the bounding box */
  FT m_xmin, m_ymin, m_zmin, m_xmax, m_ymax, m_zmax;

public:
  /*! Constructor */
  Iso_cuboid_bbox_3() {}

  /*! Constructor */
  Iso_cuboid_bbox_3(Iso_cuboid_bbox_3 & r)
  {
    m_xmin = r.m_xmin;
    m_ymin = r.m_ymin;
    m_zmin = r.m_zmin;
    m_xmax = r.m_xmax;
    m_ymax = r.m_ymax;
    m_zmax = r.m_zmax;
  }

  /*! Constructor */
  Iso_cuboid_bbox_3(const Point_3 & p)
  {
    m_xmin = m_xmax = p.x();
    m_ymin = m_ymax = p.y();
    m_zmin = m_zmax = p.z();
  }

  /*! Constructor */
  Iso_cuboid_bbox_3(const Point_3 & p, const Point_3 & q)
  {
    set(p.x(), p.y(), p.z(), q.x(), q.y(), q.z());
  }

  /*! Constructor */
  Iso_cuboid_bbox_3(const Point_3 & left,   const Point_3 & right,
                    const Point_3 & bottom, const Point_3 & top,
                    const Point_3 & far_,   const Point_3 & near)
  {
    m_xmin = left.x();
    m_xmax = right.x();
    m_ymin = bottom.y();
    m_ymax = top.y();
    m_zmin = near.z();
    m_zmax = far_.z();
  }

  /*! Constructor */
  Iso_cuboid_bbox_3(const RT & min_hx, const RT & min_hy, const RT & min_hz,
                    const RT & max_hx, const RT & max_hy, const RT & max_hz, 
                    const RT & hw)
  {
    m_xmin = xmin / hw;
    m_ymin = ymin / hw;
    m_zmin = zmin / hw;
    m_xmax = xmax / hw;
    m_ymax = ymax / hw;
    m_zmax = zmax / hw;
  }

  /*! Constructor */
  Iso_cuboid_bbox_3(const RT & xmin, const RT & ymin, const RT & zmin,
                    const RT & xmax, const RT & ymax, const RT & zmax)
  {
    m_xmin = xmin;
    m_ymin = ymin;
    m_zmin = zmin;
    m_xmax = xmax;
    m_ymax = ymax;
    m_zmax = zmax;
  }

  /*! returns the smallest vertex of c (= vertex(0)). */
  Point_3 min() const
  {
    return Point_3(m_xmin, m_ymin, m_zmin);
  }

  /*! returns the largest vertex of c (= vertex(7)). */
  Point_3 max() const
  {
    return Point_3(m_xmax, m_ymax, m_zmax);
  }
  
  /*! updates the bounding box */
  Iso_cuboid_bbox_3 & Iso_cuboid_bbox_3::operator+=(const Point_3 & p)
  {
    if (p.x() < m_xmin) m_xmin = p.x();
    else if (p.x() > m_xmax) m_xmax = p.x();
    if (p.y() < m_ymin) m_ymin = p.y();
    else if (p.y() > m_ymax) m_ymax = p.y();
    if (p.z() < m_zmin) m_zmin = p.z();
    else if (p.z() > m_zmax) m_zmax = p.z();
    return *this;
  }

  /* obtains the extreme valuse */
  FT xmin() const { return m_xmin; }
  FT ymin() const { return m_ymin; }
  FT zmin() const { return m_zmin; }
  FT xmax() const { return m_xmax; }
  FT ymax() const { return m_xmax; }
  FT zmax() const { return m_xmax; }

  /*! \brief obtains the extreme valuse */
  FT operator[](unsigned int i) const
  {
    switch(i) {
     case 0: return m_xmin;
     case 1: return m_ymin;
     case 2: return m_zmin;
     case 3: return m_xmax;
     case 4: return m_ymax;
     case 5: return m_zmax;
    }
    return 0;
  }
  
  /*! transforms the iso cuboid */
  void transform(const Aff_transformation_3 & t)
  {
    Point_3 pmin(m_xmin, m_ymin, m_zmin);
    pmin = t(pmin);
    m_xmin = pmin.x();
    m_ymin = pmin.y();
    m_zmin = pmin.z();
    Point_3 pmax(m_xmax, m_ymax, m_zmax);
    pmax = t(pmax);
    m_xmax = pmax.x();
    m_ymax = pmax.y();
    m_zmax = pmax.z();
  }

  /*! returns the midpoint of the diagonal between the min and max cirners */
  Point_3 midpoint()
  {
    typedef typename Kernel::Construct_midpoint_3       Construct_midpoint;
    Kernel kernel;
    Construct_midpoint construct_midpoint =
      kernel.construct_midpoint_3_object();    
    return construct_midpoint(Point_3(m_xmin, m_ymin, m_zmin),
                              Point_3(m_xmax, m_ymax, m_zmax));
  }
  
private:
  /*! sets the bounding box */
  void set(const Point_3 & p, const Point_3 & q)
  {
    m_xmin = std::min(p.x(), q.x());
    m_ymin = std::min(p.y(), q.y());
    m_zmin = std::min(p.z(), q.z());
    m_xmax = std::max(p.x(), q.x());
    m_ymax = std::max(p.y(), q.y());
    m_zmax = std::max(p.z(), q.z());
  }
};

/*! output operator */
template < class Kernel >
std::ostream & operator<<(std::ostream & os,
                          const Iso_cuboid_bbox_3<Kernel> & r)
{
  return os << r.min() << " " << r.max();
}

/*! input operator */
template < class Kernel >
std::istream & operator>>(std::istream & is, Iso_cuboid_bbox_3<Kernel> & r)
{
  typedef typename Kernel::Point_3      Point_3;
  Point_3 p, q;
  is >> p >> q;
  set(p, q);
  return is;
}

// CGAL_END_NAMESPACE

#endif
