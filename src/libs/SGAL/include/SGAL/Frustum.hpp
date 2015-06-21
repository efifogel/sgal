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

#ifndef SGAL_FRUSTUM_HPP
#define SGAL_FRUSTUM_HPP

/*! \file
 * An implementation of Frustum.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Plane.hpp"

SGAL_BEGIN_NAMESPACE

class Bounding_sphere;
class Box_bound;
class Element;

class SGAL_SGAL_DECL Frustum {
public:
  /*! Construct.
   */
  Frustum();

  /*! Destruct.
   */
  virtual ~Frustum();

public:
  // Enums
  enum Aspect_mode {
    CALC_NONE,
    CALC_HORIZ,
    CALC_VERT
  };

  enum Frustum_type {
    SIMPLE,
    ORTHOGONAL,
    PERSPECTIVE,
    NUM_TYPES
  };

  enum Frust_planes {
    LEFT_PLANE = 0,
    RIGHT_PLANE,
    NEAR_PLANE,
    FAR_PLANE,
    BOTTOM_PLANE,
    TOP_PLANE,
    NUM_PLANES
  };

  enum Frust_corners {
    NEAR_LL = 0,
    NEAR_LR,
    NEAR_UL,
    NEAR_UR,
    FAR_LL,
    FAR_LR,
    FAR_UL,
    FAR_UR,
    NUM_CORNERS
  };

public:
  // virtual functions
  virtual void copy(const Frustum* src);

public:
  /*! Set the frustum type (orthogonal, perspective, etc) */
  void set_type(Frustum_type type);

  /*! Obtain the frustum type (orthogonal, perspective, etc) */
  Frustum_type get_type() const { return m_type; }

  /*! Set the distance from the origin to the near plane.
   * \param near_dist the distance from the near plane.
   */
  void set_near(Float near_dist);

  /*! Sets the distance from the origin to the far plane.
   * \param far_dist the distance.
   */
  void set_far(Float far_dist);

  /*! Obtain the distances from the origin to the near plane and to
   * the far plane.
   */
  void get_near_far(Float& near_dist, Float& far_dist);

  void set_fov(Float fov);
  Float get_fov();
  void set_horiz_fov(Float horiz_fov);
  void set_vert_fov(Float vert_fov);
  void set_right(Float right);
  void set_left(Float left);
  void set_top(Float top);
  void set_bottom(Float bottom);

  void get_near(Vector3f& ll, Vector3f& lr, Vector3f& ul, Vector3f& ur);
  void get_far(Vector3f& ll, Vector3f& lr, Vector3f& ul, Vector3f& ur);

  /*! Obtain the six coordinates defining the two diagonal corners. */
  void get_diag_corners(Float& left, Float& right, Float& bottom,
                        Float& top, Float& near_dist, Float& far_dist);

  void get_corners(Float& left, Float& right, Float& bottom, Float& top,
                   Float& near_clip, Float& far_clip);

  /*! Sets the aspect recalculation mode. The mode can be
   * SGAL_FRUST_CALC_NONE, which means that whatever the application sets
   * for the frustum dimensions is honored, or either FRUST_CALC_VERT
   * or FRUST_CALC_HORIZ, meaning that the vertical or horizontal
   * frustum dimensions, respectively, should be calculated from the
   * horizontal or vertical dimensions using aspect_ratio. Causes the frustum
   * dimensions to be recalculated immediately if necessary.
   */
  void set_aspect_mode(Frustum::Aspect_mode mode);

  Frustum::Aspect_mode get_aspect_mode() const;

  /*! Set the aspect ratio of the frustum dimensions. */
  void set_aspect_ratio(Float ratio);

  /*! Obtain the aspect ratio of the frustum dimensions. */
  Float get_aspect_ratio() const;

  /*! Obtain the viewing matrix.
   */
  void get_proj_mat(Matrix4f& mat);

  Plane* get_facets();

public:
  /*! Set the attributes of this container.
   */
  void set_attributes(Element* elem);

  /*! Make this frustum perspective.
   */
  void make_ortho(Float left, Float right, Float bottom, Float top);
  void make_ortho(Float left, Float right, Float bottom, Float top,
                  Float near_dist, Float far_dist);

  /*! Make this frustum orthonormal.
   */
  void make_persp(Float left, Float right, Float bottom, Float top);
  void make_persp(Float left, Float right, Float bottom, Float top,
                  Float near_dist, Float far_dist);

  unsigned int contains(const Vector3f& pt) const;
  unsigned int contains(const Bounding_sphere* sphere) const;
  unsigned int contains(const Box_bound* box) const;

  void apply();

  /*! Set the perturbation distances for accumulation AA.
   */
  void set_perturbation_scale(Float xpert, Float ypert);

protected:
  // void calc(Frustum::Aspect_mode aspect_mode);
  void clean_corners();
  void clean_planes();

private:
  /*! The frustum types (prespective, orthogonal, etc). */
  static const char* s_type_strings[];

  Frustum::Frustum_type m_type;

  Float m_near_dist;

  Float m_far_dist;

  Float m_left;

  Float m_right;

  Float m_top;

  Float m_bottom;

  Float m_horiz_fov;

  Float m_vert_fov;

  Float m_xcenter;

  Float m_ycenter;

  Frustum::Aspect_mode m_aspect_mode;

  Float m_aspect_ratio;

  Vector3f m_corners[Frustum::NUM_CORNERS];

  Plane m_facets[Frustum::NUM_PLANES];

  Boolean m_dirty_corners;
  Boolean m_dirty_planes;

  /*! The perturbation scale along the x axis for accumulation AA. */
  Float m_x_perturbation_scale;

  /*! The perturbation scale along the y axis for accumulation AA. */
  Float m_y_perturbation_scale;
};

//! \brief set the perturbation scales for accumulation AA.
inline void Frustum::set_perturbation_scale(Float xpert, Float ypert)
{
  m_x_perturbation_scale = xpert;
  m_y_perturbation_scale = ypert;
}

//! \brief obtains the aspect ratio of the frustum dimensions.
inline Float Frustum::get_aspect_ratio() const { return m_aspect_ratio; }

SGAL_END_NAMESPACE

#endif
