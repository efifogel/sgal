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
// $Source$
// $Revision: 7628 $
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

class Sphere_bound;
class Box_bound;
class Element;

class Frustum {
public:
  /*! Constructor */
  Frustum();

  /*! Destructor */
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

  void set_near(float near_dist);
  void set_far(float far_dist);
  void get_near_far(float& near_dist, float& far_dist);
    
  void set_fov(float fov);
  float get_fov();
  void set_horiz_fov(float horiz_fov);
  void set_vert_fov(float vert_fov);
  void set_right(float right);
  void set_left(float left);
  void set_top(float top);
  void set_bottom(float bottom);

  void get_near(Vector3f& ll, Vector3f& lr, Vector3f& ul, Vector3f& ur);
  void get_far(Vector3f& ll, Vector3f& lr, Vector3f& ul, Vector3f& ur);

  /*! Obtain the six coordinates defining the two diagonal corners */
  void get_diag_corners(float& left, float& right, float& bottom,
                        float& top, float& near_dist, float& far_dist);
  
  void get_corners(float& left, float& right, float& bottom, float& top,
                   float& near_clip, float& far_clip);

  void set_aspect_mode(Frustum::Aspect_mode mode);
  Frustum::Aspect_mode get_aspect_mode() const;

  /*! Set the aspect ratio of the frustum dimensions */
  void set_aspect_ratio(float ratio);

  /*! Obtain the aspect ratio of the frustum dimensions */
  float get_aspect_ratio() const { return m_aspect_ratio; }

  void get_gl_proj_mat(Matrix4f& mat);

  Plane* get_facets();

public:
  /*! Set the attributes of this container. */
  void set_attributes(Element* elem);

  /*! Make this frustum perspective. */
  void make_ortho(Float left, Float right, Float bottom, Float top);
  void make_ortho(Float left, Float right, Float bottom, Float top,
                  Float near_dist, Float far_dist);

  /*! Make this frustum orthonormal. */
  void make_persp(Float left, Float right, Float bottom, Float top);
  void make_persp(Float left, Float right, Float bottom, Float top,
                  Float near_dist, Float far_dist);

  unsigned int contains(const Vector3f& pt) const;
  unsigned int contains(const Sphere_bound* sphere) const;
  unsigned int contains(const Box_bound* box) const;

  void apply();

  /*! Set the perturbation distances for accumulation AA*/
  void set_perturbation_scale(float xpert, float ypert);
  
protected:
  // void calc(Frustum::Aspect_mode aspect_mode);
  void clean_corners();
  void clean_planes();  

private:
  /*! The frustum types (prespective, orthogonal, etc) */
  static const char* s_type_strings[];
  
  Frustum::Frustum_type m_type;

  float m_near_dist;

  float m_far_dist;

  float m_left;

  float m_right;

  float m_top;

  float m_bottom;

  float m_horiz_fov;

  float m_vert_fov;

  float m_xcenter;

  float m_ycenter;

  Frustum::Aspect_mode m_aspect_mode;

  float m_aspect_ratio;

  Vector3f m_corners[Frustum::NUM_CORNERS];

  Plane m_facets[Frustum::NUM_PLANES];

  bool m_dirty_corners;
  bool m_dirty_planes;

  /*! The perturbation scale along the x axis for accumulation AA */
  float m_x_perturbation_scale;

  /*! The perturbation scale along the y axis for accumulation AA */
  float m_y_perturbation_scale;

  /*! Parse the type string-attribute */
  int parse_type(const std::string& type);
};

/*! \brief set the perturbation scales for accumulation AA*/
inline void Frustum::set_perturbation_scale(float xpert, float ypert)
{
  m_x_perturbation_scale = xpert;
  m_y_perturbation_scale = ypert;
}

SGAL_END_NAMESPACE

#endif
