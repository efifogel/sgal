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
// $Id: Accumulation.hpp 7628 2009-05-26 11:11:45Z efif $
// $Revision: 7628 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ACCUMULATION_HPP
#define SGAL_ACCUMULATION_HPP

/*! \file
 * Encapsulates the OpenGL accumulation application. Specifies the
 * parameters needed to accumulate frame buffers produced in a sequence. 
 * It is used to implement antialiasing of the highest quality at the price
 * of a delay.
 */

#include <vector>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/SGAL_defs.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;

class SGAL_CLASSDEF Accumulation : public Container {
public:
  typedef struct jitter { Float x, y; } Jitter;

  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    DELAY,
    QUALITY
  };
  
  /*! The quality of accumulation used for antialiasing */
  enum Quality {
    Q_HIGH = 0,
    Q_LOW
  };

  /*! Constructor */
  Accumulation(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Accumulation() {}

  /*! Construct the prototype */
  static Accumulation * prototype() { return new Accumulation(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Accumulation(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto * get_prototype();
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Obtain the total number of iterations */
  Uint get_num_iters() const { return m_num_iters; }

  /*! Set the flag that indicates whether accumulation is enabled */
  void set_enabled(Boolean flag) { m_enabled = flag; }

  /*! Obtain the flag that indicates whether accumulation is enabled */
  Boolean is_enabled() const { return m_enabled; }

  /*! Set the accumulation delay in miliseconds */
  void set_delay(Uint ms) { m_delay = ms; }

  /*! Obtain the accumulation delay in miliseconds */  
  Uint get_delay() const { return m_delay; }

  /*! Set the accumulation quality */
  void set_quality(Quality quality) { m_quality = quality; }

  /*! Obtain the accumulation quality */
  Quality get_quality() const { return m_quality; }

  /*! Set the total number of iterations */
  void set_num_iters(Uint num_iters);
  
  /*! Apply actions when accumulation starts */
  void pre_render(Draw_action * draw_action);

  /*! Apply actions when accumulation ends */
  void post_render(Draw_action * draw_action);

  /*! Enactivate accumulation */
  void enactivate();

  /*! Disactivate */
  void disactivate() { m_active = SGAL_FALSE; }
  
  /*! Return ture if the frame buffer is being accumulated */
  Boolean is_active() { return m_active; }

  /*! Return ture if accumulation is done */
  Boolean is_done();
  
  /*!  Return ture if the frame buffer showld be shown */
  Boolean do_show() { return m_show; }

  /*! Reset the delay start time to the current time */
  void reset_delay_time();

  /*! Set defualt values */
  void set_default(Boolean def_enabled = s_def_enabled,
                   Uint def_delay = s_def_delay,
                   Quality def_quality = s_def_quality);

  /*! Obtain the jitter value of the current iteration */
  void get_jitter(float & x, float & y);

  /*! Set the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer
   * \param green_bits the number of red bits stored in the accumulation buffer
   * \param blue_bits the number of red bits stored in the accumulation buffer
   * \param alpha_bits the number of red bits stored in the accumulation buffer
   */
  virtual void set_number_of_bits(Uint red_bits, Uint green_bits,
                                  Uint blue_bits, Uint alpha_bits);

  /*! Obtain the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer
   * \param green_bits the number of red bits stored in the accumulation buffer
   * \param blue_bits the number of red bits stored in the accumulation buffer
   * \param alpha_bits the number of red bits stored in the accumulation buffer
   */
  virtual void get_number_of_bits(Uint & red_bits, Uint & green_bits,
                                  Uint & blue_bits, Uint & alpha_bits) const;
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:  
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! jiter arrays */
  static Jitter s_j2[];
  static Jitter s_j3[];
  static Jitter s_j4[];
  static Jitter s_j8[];
  static Jitter s_j15[];
  static Jitter s_j24[];
  static Jitter s_j66[];

  typedef std::pair<Uint,Jitter*> Jitter_pair;
  
  /*! Size array */
  static Jitter_pair s_sizes[];
  
  /*! The jittering values */
  Jitter * m_jitters;

  /*! The number of jittering values */
  Uint m_num_jitters;
  
  /*! Indicates whether accumulation is enabled */
  Boolean m_enabled;

  /*! The accumulation delay in miliseconds */
  Uint m_delay;

  /*! The auality of the accumulation used for antialiasing */
  Quality m_quality;

  /*! The total number of accumulation iterations */
  Uint m_num_iters;
  
  /*! Indicate whether to start accumulating */
  Boolean m_active;
  
  /*! The contribution each iteration */
  Float m_contribution;

  /*! The The accumulation factor */
  Float m_accumulate;

  /*! The iteration number */
  Uint m_iteration_no;

  /*! Indicates whether to show the frame buffer */
  Boolean m_show;

  /*! Number of red bits stored in the accumulation buffer */
  Uint m_red_bits;

  /*! Number of green bits stored in the accumulation buffer */
  Uint m_green_bits;

  /*! Number of blue bits stored in the accumulation buffer */
  Uint m_blue_bits;

  /*! Number of alpha bits stored in the accumulation buffer */
  Uint m_alpha_bits;
  
  // default values
  static Boolean s_def_enabled;
  static Uint s_def_delay;
  static Quality s_def_quality;

  static const Uint s_def_red_bits;
  static const Uint s_def_green_bits;
  static const Uint s_def_blue_bits;
  static const Uint s_def_alpha_bits;
  
  static const Char * s_quality_names[];
};

SGAL_END_NAMESPACE

#endif
