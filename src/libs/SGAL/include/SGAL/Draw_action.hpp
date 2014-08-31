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

#ifndef SGAL_DRAW_ACTION_HPP
#define SGAL_DRAW_ACTION_HPP

/*! \file
 * Draw_action is a class used while traversing the scene graph for rendering.
 * It holds information about which pass is being rendered and an indication
 * whether more passes are required.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Configuration;
class Node;
class Matrix4f;

class SGAL_SGAL_DECL Draw_action : public Action {
public:
  /*! Constructor */
  Draw_action(Configuration* configuration = nullptr);

  /*! Destructor */
  virtual ~Draw_action();

  /*! Apply the draw action to a given node. For now this means calling
   * the draw method on the Node.
   * \param node (in) a pointer to the node to draw.
   */
  virtual Trav_directive apply(Node* node);

  /*! Determines whether a second pass is required. A second pass is typically
   * required to render the transparent objects.
   * \return true if a 2nd pass is required and false otherwise.
   */
  Boolean is_second_pass_required();

  /*! Set the flag that indicates whether a second pass is required. This flag
   * should be set, for example, when a transparent object has to be rendered.
   */
  void set_second_pass_required(Boolean flag);

  /*! Increment the pass number. */
  void inc_pass_no();

  /*! Obtain the number of the current pass being rendered.
   * Note that countng starts from zero. (0 is first pass.)
   * \return the pass number.
   */
  Int get_pass_no();

  /*! Reset the pass number to zero. */
  void reset_pass_no();

  /*! Determine whether the current pass is the first one.
   * \return true if current pass is the first pass and false otherwise.
   */
  Boolean is_first_pass();

  void set_current_lod(Int lod);

  Int get_current_lod() const;

  void set_current_wtm(const Matrix4f* wtm);

  const Matrix4f* get_current_wtm() const;

  void set_snap_from_front(Boolean flag);

  Boolean is_snap_from_front() const;

  void set_clear(Uint mask);

  Uint get_clear() const;

  void set_snap(Boolean flag);

  Boolean get_snap() const;

  void set_apply_camera(Boolean flag);

  Boolean get_apply_camera() const;

  /*! Set the configuration container */
  void set_configuration(Configuration* conf);

  /*! Obtain the configuration container */
  Configuration* get_configuration() const;

private:
  /*! Indicates which pass is being rendered, 0 being the first pass. */
  Int m_pass_no;

  /*! Indicates whether a second pass is required for transparent objects */
  Boolean m_second_pass_required;

  /*! Indicates current level of detail for shapes. */
  Int m_current_lod;

  /*! The world transformation matrix */
  const Matrix4f* m_current_wtm;

  /*! indicates for the snapshot from which buffer to take the image from */
  Boolean m_snap_from_front;

  /*! Indicates whether the frame buffers should be cleared */
  Uint m_clear;

  /*! Indicates whether to campture a snapshot */
  Boolean m_snap;

  /*! Indicates whether to apply the active camera */
  Boolean m_apply_camera;

  /*! Configuration \todo move to action? */
  Configuration* m_configuration;
};

inline void Draw_action::set_current_lod(Int lod) { m_current_lod = lod; }

inline Int Draw_action::get_current_lod() const { return m_current_lod; }

inline void Draw_action::set_current_wtm(const Matrix4f* wtm)
{ m_current_wtm = wtm; }

inline const Matrix4f* Draw_action::get_current_wtm() const
{ return m_current_wtm; }

inline void Draw_action::set_snap_from_front(Boolean flag)
{ m_snap_from_front = flag; }

inline Boolean Draw_action::is_snap_from_front() const
{ return m_snap_from_front; }

inline void Draw_action::set_clear(Uint mask) { m_clear = mask; }

inline Uint Draw_action::get_clear() const { return m_clear; }

inline void Draw_action::set_snap(Boolean flag) { m_snap = flag; }

inline Boolean Draw_action::get_snap() const { return m_snap; }

inline void Draw_action::set_apply_camera(Boolean flag)
{ m_apply_camera = flag; }

inline Boolean Draw_action::get_apply_camera() const
{ return m_apply_camera; }

//! \brief sets the configuration container.
inline void Draw_action::set_configuration(Configuration* conf)
{ m_configuration = conf; }

//! \brief obtains the configuration container.
inline Configuration* Draw_action::get_configuration() const
{ return m_configuration; }

SGAL_END_NAMESPACE

#endif
