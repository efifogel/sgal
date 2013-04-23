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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Control the generation of texture coordinates.
 */

#ifndef SGAL_TEX_GEN_HPP
#define SGAL_TEX_GEN_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Plane.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Element;

class Tex_gen : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    MODE_S,
    MODE_T,
    MODE_R,
    MODE_Q,
    PLANE_S,
    PLANE_T,
    PLANE_R,
    PLANE_Q,
    LAST
  };

  enum Mode {
    OFF = -1,
    OBJECT_LINEAR,
    EYE_LINEAR,
    SPHERE_MAP,
    NORMAL_MAP,
    REFLECTION_MAP
  };

  /*! Constructor */
  Tex_gen(Boolean proto = false);

  /*! Destructor */
  virtual ~Tex_gen();

  /*! Construct the prototype. */
  static Tex_gen* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Applies the texture generation attribute. */
  virtual void draw(Context * ctx);

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Set the s-texture coordinate generation function. */
  void set_mode_s(Mode mode_s);

  /*! Obtain the s-texture coordinate generation function. */
  Mode get_mode_s() const;

  /*! Set the t-texture coordinate generation function. */
  void set_mode_t(Mode mode_t);

  /*! Obtain the t-texture coordinate generation function. */
  Mode get_mode_t() const;

  /*! Set the r-texture coordinate generation function. */
  void set_mode_r(Mode mode_r);

  /*! Obtain the r-texture coordinate generation function. */
  Mode get_mode_r() const;

  /*! Set the q-texture coordinate generation function. */
  void set_mode_q(Mode mode_q);

  /*! Obtain the q-texture coordinate generation function. */
  Mode get_mode_q() const;

  /*! Set the s-texture coordinate generation reference-plane. */
  void set_plane_s(const Plane& plane_s);

  /*! Obtain the (non const) s-texture coordinate generation reference-plane. */
  Plane& get_plane_s();

  /*! Obtain the (const) s-texture coordinate generation reference-plane. */
  const Plane& get_plane_s() const;
  
  /*! Set the t-texture coordinate generation reference-plane. */
  void set_plane_t(const Plane& plane_t);

  /*! Obtain the (non const) t-texture coordinate generation reference-plane. */
  Plane& get_plane_t();

  /*! Obtain the (const) t-texture coordinate generation reference-plane. */
  const Plane& get_plane_t() const;
  
  /*! Set the r-texture coordinate generation reference-plane. */
  void set_plane_r(const Plane& plane_r);

  /*! Obtain the (non const) r-texture coordinate generation reference-plane. */
  Plane& get_plane_r();

  /*! Obtain the (const) r-texture coordinate generation reference-plane. */
  const Plane& get_plane_r() const;
  
  /*! Set the q-texture coordinate generation reference-plane. */
  void set_plane_q(const Plane& plane_q);

  /*! Obtain the (non const) q-texture coordinate generation reference-plane. */
  Plane& get_plane_q();

  /*! Obtain the (const) q-texture coordinate generation reference-plane. */
  const Plane& get_plane_q() const;
  
protected: 
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  Int m_dirty_id;

  Int m_dirty_count;

  friend class Context;

  /*! The s-texture coordinate generation function. */
  Mode m_mode_s;

  /*! The t-texture coordinate generation function. */
  Mode m_mode_t;

  /*! The r-texture coordinate generation function. */
  Mode m_mode_r;

  /*! The q-texture coordinate generation function. */
  Mode m_mode_q;

  /*! The s-texture coordinate generation reference-plane. */
  Plane m_plane_s;

  /*! The t-texture coordinate generation reference-plane. */
  Plane m_plane_t;

  /*! The r-texture coordinate generation reference-plane. */
  Plane m_plane_r;

  /*! The q-texture coordinate generation reference-plane. */
  Plane m_plane_q;

private:
  void init();
};

/*! \brief constructs the prototype. */
inline Tex_gen* Tex_gen::prototype() { return new Tex_gen(true); }

/*! \brief clones. */
inline Container* Tex_gen::clone() { return new Tex_gen(); }

/*! \brief obtains the tag (type) of the container */
inline const std::string& Tex_gen::get_tag() const { return s_tag; }

/*! \brief obtains the s-texture coordinate generation function. */
inline Tex_gen::Mode Tex_gen::get_mode_s() const { return m_mode_s; }

/*! \brief obtains the t-texture coordinate generation function. */
inline Tex_gen::Mode Tex_gen::get_mode_t() const { return m_mode_t; }

/*! \brief obtains the r-texture coordinate generation function. */
inline Tex_gen::Mode Tex_gen::get_mode_r() const { return m_mode_r; }

/*! \brief obtains the q-texture coordinate generation function. */
inline Tex_gen::Mode Tex_gen::get_mode_q() const { return m_mode_q; }

/*! \brief obtains the (non const) s-texture coordinate generation
 * reference-plane.
 */
inline Plane& Tex_gen::get_plane_s() { return m_plane_s; }

/*! \brief Obtain the (const) s-texture coordinate generation reference-plane. */
inline const Plane& Tex_gen::get_plane_s() const { return m_plane_s; }
  
/*! \brief obtains the (non const) t-texture coordinate generation
 * reference-plane.
 */
inline Plane& Tex_gen::get_plane_t() { return m_plane_t; }

/*! \brief Obtain the (const) t-texture coordinate generation reference-plane. */
inline const Plane& Tex_gen::get_plane_t() const { return m_plane_t; }
  
/*! \brief obtains the (non const) r-texture coordinate generation
 * reference-plane.
 */
inline Plane& Tex_gen::get_plane_r() { return m_plane_r; }

/*! \brief Obtain the (const) r-texture coordinate generation reference-plane. */
inline const Plane& Tex_gen::get_plane_r() const { return m_plane_r; }
  
/*! \brief obtains the (non const) q-texture coordinate generation
 * reference-plane.
 */
inline Plane& Tex_gen::get_plane_q() { return m_plane_q; }

/*! \brief Obtain the (const) s-texture coordinate generation reference-plane. */
inline const Plane& Tex_gen::get_plane_q() const { return m_plane_q; }
  
SGAL_END_NAMESPACE

#endif
