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

#ifndef SGAL_BOUNDING_BOX_HPP
#define SGAL_BOUNDING_BOX_HPP

#include <algorithm>
#include <iostream>
#include "boost/tuple/tuple.hpp"

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Bounding_box {
private:
  Float m_x_min;
  Float m_y_min;
  Float m_z_min;
  Float m_x_max;
  Float m_y_max;
  Float m_z_max;

public:
  /*! Construct default. */
  Bounding_box();

  /*! Construct from elements. */
  Bounding_box(Float x_min, Float y_min, Float z_min,
               Float x_max, Float y_max, Float z_max);

  /*! Set */
  void set(Float x_min, Float y_min, Float z_min,
           Float x_max, Float y_max, Float z_max);

  /*! Obtain min x. */
  Float xmin() const;

  /*! Obtain min y. */
  Float ymin() const;

  /*! Obtain min z. */
  Float zmin() const;

  /*! Obtain max x. */
  Float xmax() const;

  /*! Obtain max y. */
  Float ymax() const;

  /*! Obtain max z. */
  Float zmax() const;

  /*! Obtain min. */
  Float min(Uint dim) const;

  /*! Obtain max. */
  Float max(Uint dim) const;

  /*! Obtain a bounding box of the current bounding box and a given one.
   * \param bb (in) the given boundng box.
   */
  Bounding_box operator+(const Bounding_box& bb) const;

  /*! Update the bounding box to be the bounding box of the current bounding
   * box and a given one.
   * \param bb (in) the given boundng box.
   * \return the current bounding box.
   */
  Bounding_box& operator+=(const Bounding_box& bb);

  /*! Obtain the dimensions in order of their length.
   * \return a tuple of dimension indicess in order of dimension length.
   */
  boost::tuple<Uint, Uint, Uint> get_longest_dimensions() const;
};

//! \brief construct.
inline Bounding_box::Bounding_box() :
  m_x_min(0), m_y_min(0), m_z_min(0),
  m_x_max(0), m_y_max(0), m_z_max(0)
{}

//! Constructor from elements.
inline Bounding_box::Bounding_box(Float x_min, Float y_min, Float z_min,
                                  Float x_max, Float y_max, Float z_max)
{ set(x_min, y_min, z_min, x_max, y_max, z_max); }

//! \brief sets the bounding box.
inline void Bounding_box::set(Float x_min, Float y_min, Float z_min,
                              Float x_max, Float y_max, Float z_max)
{
  m_x_min = x_min; m_y_min = y_min; m_z_min = z_min;
  m_x_max = x_max; m_y_max = y_max; m_z_max = z_max;
}

inline Float Bounding_box::xmin() const { return m_x_min; }
inline Float Bounding_box::ymin() const { return m_y_min; }
inline Float Bounding_box::zmin() const { return m_z_min; }
inline Float Bounding_box::xmax() const { return m_x_max; }
inline Float Bounding_box::ymax() const { return m_y_max; }
inline Float Bounding_box::zmax() const { return m_z_max; }

inline Float Bounding_box::min(Uint dim) const
{ return (dim == 0) ? xmin() : ((dim == 1) ? ymin() : zmin()); }
inline Float Bounding_box::max(Uint dim) const
{ return (dim == 0) ? xmax() : ((dim == 1) ? ymax() : zmax()); }

//! \brief exporter.
inline std::ostream& operator<<(std::ostream& os, const Bounding_box& bbox)
{
  os << bbox.xmin() << " " << bbox.ymin() << " " << bbox.zmin() << " "
     << bbox.xmax() << " " << bbox.ymax() << " " << bbox.zmax();
  return os;
}

//! \brief importer.
inline std::istream& operator>>(std::istream& in, Bounding_box& bbox)
{
  Float x_min, y_min, z_min, x_max, y_max, z_max;
  in >> x_min >> y_min >> z_min >> x_max >> y_max >> z_max;
  bbox.set(x_min, y_min, z_min, x_max, y_max, z_max);
  return in;
}

SGAL_END_NAMESPACE

#endif
