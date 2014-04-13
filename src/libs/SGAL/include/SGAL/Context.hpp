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

#ifndef SGAL_CONTEXT_HPP
#define SGAL_CONTEXT_HPP

/*! \file
 * \brief Graphics state abstraction.
 * The Context object maintains and manages the OpenGl graphics state for a
 * scene graph. It contains all the default appearance mode values necessary
 * to render a shape, and provides the mechanism to control the graphics
 * pipeline state efficiently.
 *
 * OpenGL is a state machine: you set various modes that then remain in
 * effect until you change them. For example, the current color is a mode.
 * The Context maintains two notions of state:
 *   - Default State - default global graphics state defined on a per-context
 *     basis.
 *   - Current State - reflects the rendering (OpenGl) state, and is the
 *     accumulation of the Default State mode values and the appearance
 *     overriding mode values.
 *
 * Appearance mode, such as material and texture, can be specified per
 * shape by setting Appearance fields. Modes that are explicitly set in an
 * Appearance via the appropriate Appearance set() methods, override the
 * default values. The shape inherits the default appearance values from the
 * context Default State for all the modes that are not set explicitly.
 *
 * For optimal performance, as few Appearance fields as possible should be
 * set by setting the global defaults in Context to values that satisfy the
 * majority of geometries in a scene graph. This practice minimizes state
 * changes while rendering.

 * Four bit masks are used to generate minimum modal changes while traversing
 * the scene graph. Each bit in a bit mask corresponds to a specific
 * appearance mode value.
 *   - Appearance pending - specifies pending changes to appearance modes.
 *   - Appearance override - specifies which appearance modes should be
 *     overriden.
 *   - Context (default) pending - specifies pending changes to the context
 *     Default State.
 *   - Context (current) override - specifies which appearance modes have been
 *     overriden during the previous traversal to yield the Current State.
 *
 * Each mode has a corresponding set() and get() methods in Appearance. Each
 * set() method sets the new mode value and turns on the corresponding bit in
 * the appearance pendding and override bit masks. Each get() method simply
 * returns the last set mode value.
 *
 * Each mode has a corresponding set(), get(), and draw() methods in Context.
 * Each set() method set the new default mode value and turns on the
 * corresponding bit in the context pending bit mask. Each get() method simply
 * returns the last set default mode value. Each draw() method transfers the
 * current mode value to the rendering pipe.
 *
 * When an Appearance node is encountered during traversal the draw_app()
 * method is invoked. It is given a pointer to the Appearance node. This
 * method determines which modes should be changed using bit mask operations,
 * and applies the changes accordingly using the draw() methods, Only the
 * necessary modes are affected independently on the traversal order. This is
 * done in a few steps. During the first step the new values are extracted
 * from the Context Default State, and during the second step the new values
 * are extracted from the Appearance fields explicitly set previously.
 *
 * The algorithm follows:
 *   -# Draw from Default State:
 *      - \f$\mbox{\em def-mask} = (\mbox{\em default-pending} \cup \mbox{\em current-override}) \cap \neg \mbox{\em appearance-override}\f$
 *      - \f$\mbox{if} (\mbox{\em def-mask}) \; \mbox{draw}(\mbox{em def-mask},\mbox{em default})\f$
 *  -# Draw from appearance:
 *      - \f$\mbox{\em app-mask} = (\mbox{\em appearance} \equiv \mbox{\em current-appearance})\;?\;\mbox{\em appearance-pending} : \mbox{\em appearance-override}\f$
 *      - \f$\mbox{if} (\mbox{\em app-mask}) \; \mbox{draw}(\mbox{em app-mask},\mbox{em appearance})\f$
 *  -# Update:
 *      - \f$\mbox{\em default-pending} = \mbox{\em default-pending} \cap \neg \mbox{\em def-mask}\f$
 *      - \f$\mbox{\em appearance-pending} = \mbox{\em appearance-pending} \cap \neg \mbox{\em app-mask}\f$
 *      - \f$\mbox{\em current-override} = \mbox{\em appearance-override}\f$
 *      - \f$\mbox{\em current-appearance} = \mbox{\em appearance}\f$
 *
 * In general, Appearance settings override Context default modes. It is
 * possible, however, to override Appearance settings using the
 * pushOverrideAppearance() method. Only one override appearance per context
 * can be in place at a time.
 * It is possible to override some properties that are geometry-specific
 * through the use of pushOverrideGeoProp(). Currently, CullFace,
 * Eline_width, and EPointSize are the only geometry-specific properties that
 * can be overridden.
 *
 * \author Tali Tzvi
 * \author Efi Fogel         <efifogel@gmail.com>
 * \version 1.0
 * \date    2000-2001
 * \todo Fix DrawApp() bitmask logic. See pigeon code above.
 * \todo There Replace the stack current states with a stack of default
 * states.
 * \todo Integrate or remove all together the override appearance stack
 * (overrideAppStack).
 * \todo Reorganize the graphic fields and corresponding mask bits.
 * \todo Implement Fog.
 * \todo Implement Sound.
 * \todo This class used to contain a list of all contexes that exists, while
 * only one was kept active. Fix it, or clean up the residual (i.e., IsDirty,
 * FreeId, NewId, s_contextList, s_context, s_current_context).
 */

#include <string.h>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Bit_mask.hpp"
#include "SGAL/Vector4ub.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Gfx_conf.hpp"

#if 0
#if defined(__GNUC__) && !defined(_WIN32)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif
#endif

SGAL_BEGIN_NAMESPACE

#define NO_AUDIO

class Material;
class Appearance;
class Fog;
class Texture;
class Tex_gen;
class Halftone;
class Light;
class Override_geo_prop;
class Window_handle;
class Open_gl_os_init;
class Camera;

struct Light_target {
  Boolean m_set;        // Has GL target been updated?
  Boolean m_enabled;    // Is GL target enabled?
  Light* m_defined; // Light defined at GL target
  Matrix4f m_mat;    // Current matrix when light is bound
};

class SGAL_SGAL_DECL Context {
private:
#define SGAL_MAX_STACK 16
#define SGAL_MAX_LIGHTS 8

public:
  typedef boost::shared_ptr<Texture>            Shared_texture;
  typedef boost::shared_ptr<Material>           Shared_material;
  typedef boost::shared_ptr<Halftone>           Shared_halftone;
  typedef boost::shared_ptr<Tex_gen>            Shared_tex_gen;

  /*! Constructor */
  Context();

#if defined(_WIN32)
  /*! Constructor */
  Context(const HDC& hdc);
#endif

#if 0
#if defined(__GNUC__) && !defined(_WIN32)
  /*! Constructor */
  Context(Display* display, XVisualInfo* visual);
#endif
#endif

  /*! Destructor */
  virtual ~Context();

  //! \todo void set_win_handle(Window_handle* win_handle);
  //! \todo Window_handle* get_win_handle ();
  //! \todo Window_handle* get_win_handle () const;

#if defined(_WIN32)
  void make_current(HDC hdc);
#endif
#if 0
#if defined(__GNUC__) && !defined(_WIN32)
  void make_current(::Display* display, ::Window window);
#endif
#endif

  void make_current();

  static void release_current();

  static Context* get_current() { return s_current_context; }

  void delete_context();

  void swap_buffers();

  enum {
    BEGIN_STATE_ELEMENTS = -1,
    STATE_ELEMENTS,
    CULL_FACE,
    LINE_WIDTH,
    POINT_SIZE
  };

  void set_viewport(Uint x, Uint y, Uint w, Uint h);

  void get_viewport(Uint& x, Uint& y, Uint& w, Uint& h) const;

  void set_material_mode_enable(Gfx::Material_mode material_mode_enable);

  Gfx::Material_mode get_material_mode_enable() const;

  void set_texture(Shared_texture texture);

  Shared_texture get_texture() const;

  void set_halftone(Shared_halftone halftone);

  Shared_halftone get_halftone() const;

  void set_tex_enable(bool tex_enable);

  Boolean get_tex_enable() const;

  void draw_tex_enable(bool tex_enable);

  void set_tex_mode(Gfx::Tex_mode tex_mode);

  Gfx::Tex_mode get_tex_mode() const;

  void set_tex_blend_color(Float v0, Float v1, Float v2, Float v3);

  void set_tex_blend_color(const Vector4f& tex_blend_color);

  void get_tex_blend_color(Vector4f& tex_blend_color) const;

  void get_tex_blend_color(Float* v0, Float* v1, Float* v2, Float* v3) const;

  void set_tex_env(Gfx::Tex_env tex_env);

  Gfx::Tex_env get_tex_env() const;

  void set_tex_gen(Shared_tex_gen tex_gen);

  Shared_tex_gen get_tex_gen() const;

  void set_tex_gen_enable(Boolean tex_gen_enable);

  Boolean get_tex_gen_enable() const;

  void set_material(Shared_material material);

  Shared_material get_material() const;

  void set_light_enable(Boolean light_enable);

  Boolean get_light_enable() const;

  Light* get_light(const Int i) const;

  void set_shade_model(Gfx::Shade_model shade_model);

  Gfx::Shade_model get_shade_model() const;

  void draw_shade_model(Gfx::Shade_model shade_model);

  void set_transp_enable(Boolean transp_enable);

  Boolean get_transp_enable() const;

  void set_transp_mode(Gfx::Transparency_mode transp_mode);

  Gfx::Transparency_mode get_transp_mode() const;

  void set_alpha_func(Gfx::Alpha_func alpha_func);

  Gfx::Alpha_func get_alpha_func() const;

  void set_alpha_ref(Float alpha_ref);

  Float get_alpha_ref() const;

  void set_blend_color(const Vector4f& blend_color);

  void get_blend_color(Vector4f& blend_color) const;

  void set_blend_color(Float v0, Float v1, Float v2, Float v3);

  void get_blend_color(Float* v0, Float* v1, Float* v2, Float* v3) const;

  void set_src_blend_func(Gfx::Src_blend_func src_blend_func);

  Gfx::Src_blend_func get_src_blend_func() const;

  void set_dst_blend_func(Gfx::Dst_blend_func dst_blend_func);

  Gfx::Dst_blend_func get_dst_blend_func() const;

  void set_color_mask(const Vector4ub& color_mask);

  void get_color_mask(Vector4ub& color_mask) const;

  void set_color_mask(Ubyte v0, Ubyte v1, Ubyte v2, Ubyte v3);

  void get_color_mask(Ubyte* v0, Ubyte* v1, Ubyte* v2, Ubyte* v3) const;

  void set_depth_enable(Boolean depth_enable);

  Boolean get_depth_enable() const;

  void set_depth_func(Gfx::Depth_func depth_func);

  Gfx::Depth_func get_depth_func() const;

  void set_depth_mask(Boolean depth_mask);

  Boolean get_depth_mask() const;

  void set_fog_enable(Boolean fog_enable);

  Boolean get_fog_enable() const;

  void set_polygon_stipple_enable(Boolean enable);

  Boolean get_polygon_stipple_enable() const;

  void set_poly_mode(Gfx::Poly_mode poly_mode);

  Gfx::Poly_mode get_poly_mode() const;

  void set_cull_face(Gfx::Cull_face cull_face);

  Gfx::Cull_face get_cull_face() const;

  void draw_cull_face(Gfx::Cull_face cull_face);

  /*! Set the attribute that indicates whether one- or two-sided lighting
   * calculations are done for polygons.
   */
  void set_light_model_sides(Gfx::Light_model_sides light_model);

  /*! Obtain the attribute that indicates whether one- or two-sided lighting
   * calculations are done for polygons.
   */
  Gfx::Light_model_sides get_light_model_sides() const;

  /*! Apply the attribute that indicates whether one- or two-sided lighting
   * calculations are done for polygons.
   */
  void draw_light_model_sides(Gfx::Light_model_sides light_model);

  /*! Set the attribute that specifies whether a single color should be
   * generated from the lighting computation for a vertex.
   */
  void set_light_model_color_control(Gfx::Light_model_color_control model);

  /*! Obtain the attribute that specifies whether a single color should be
   * generated from the lighting computation for a vertex.
   */
  Gfx::Light_model_color_control get_light_model_color_control() const;

  /*! Apply the attribute that specifies whether a single color should be
   * generated from the lighting computation for a vertex.
   */
  void draw_light_model_color_control(Gfx::Light_model_color_control model);

  void set_line_width(Float line_width);
  Float get_line_width() const;

  void set_point_size(Float point_size);
  Float get_point_size() const;

  void set_normalize_enable(Boolean normalize_enable);
  Boolean get_normalize_enable() const;

  void set_local_viewer(Boolean local_viewer);
  Boolean get_local_viewer() const;

  void set_line_stipple_pattern(Uint stipple);
  Uint get_line_stipple_pattern() const;
  void set_line_stipple_factor(Uint factor);
  Uint get_line_stipple_factor() const;

  void set_tex_transform(const Matrix4f& xform);
  void get_tex_transform(Matrix4f& xform) const;

  void set_back_material(Shared_material material);
  Shared_material get_back_material() const;

  void clear(Uint which);
  void clear(Uint which, Float r, Float g, Float b, Float a);
  void clear(Uint which, Float r, Float g, Float b, Float a, Int stencil);

  void clear(Uint which, const Vector4f& color)
  { clear(which, color[0], color[1], color[2], color[3]); }

  void clear(Uint which, const Vector4f& color, Int stencil)
  { clear(which, color[0], color[1], color[2], color[3], stencil); }

  void clear_color_buffer();
  void clear_depth_buffer();
  void clear_stencil_buffer();
  void clear_color_depth_buffer();
  void clear_color_depth_buffer(const Vector4f& color);
  void clear_color_depth_stencil_buffer();
  void clear_color_depth_stencil_buffer(const Vector4f& color);
  void clear_color_depth_stencil_buffer(const Vector4f& color, Int stencil);

  void get_matrix(Matrix4f& m) const;
  Int get_matrix_depth() const;

  void push_matrix();
  void push_ident_matrix();
  void pop_matrix();

  void load_matrix(const Matrix4f& m);
  void load_ident_matrix();
  void mult_matrix(const Matrix4f& m);

  void translate(Float x, Float y, Float z);
  void rotate(Float x, Float y, Float z, Float radians);
  void scale(Float xs, Float ys, Float zs);

  void push_state();
  void pop_state();

  void push_override_appearance(Appearance* app);
  Appearance* pop_override_appearance();

  void push_override_geo_prop(Override_geo_prop* app);
  Override_geo_prop* pop_override_geo_prop();

  void push_lights();
  void pop_lights();
  Int get_light_target(Light* l, const Matrix4f& mat, Int& already_defined);
  void disable_light_targets();

#if 0
  void push_fog(Fog* fog);
  void pop_fog();
  Boolean Sync_fog_state(Fog* fog);
#endif

  void disable_light(Light* lt);

  /*! Obtain the aspect ration based on the view port */
  float get_aspect_ratio() const;

  void set_blend_funcs(Gfx::Src_blend_func src_blend_func,
                       Gfx::Dst_blend_func dst_blend_func);

  void draw_app(Appearance* app);

  void set_active_camera(Camera* camera) { m_active_camera = camera; }
  Camera* get_active_camera() const { return m_active_camera; }

  Uint get_red_bits() const;
  Uint get_green_bits() const;
  Uint get_blue_bits() const;
  Uint get_alpha_bits() const;

  Uint get_accum_red_bits() const;
  Uint get_accum_green_bits() const;
  Uint get_accum_blue_bits() const;
  Uint get_accum_alpha_bits() const;

  Uint get_depth_bits() const;
  Uint get_stencil_bits() const;

  Uint get_number_of_samples() const;

private:
  int m_mat_stack_depth;

  /*! Appearance override stack */
  Appearance* m_override_app_stack[SGAL_MAX_STACK];
  int m_override_app_stack_top;

  /*! Geo-property override stack */
  Override_geo_prop* m_override_geo_prop_stack[SGAL_MAX_STACK];
  int m_override_geo_prop_stack_top;

  /*! Light stack */
  Light_target m_light_stack[SGAL_MAX_STACK][SGAL_MAX_LIGHTS];
  Uint m_light_stack_depth;

  /*! Fog stack */
  Fog* m_fog_stack[SGAL_MAX_STACK];
  Uint m_fog_stack_depth;

  /*! Accumulated rendering state stack */
  Gfx m_current_state_stack[SGAL_MAX_STACK];

  // Stack of accumulated state not effected by Appearances
  Gfx m_default_state_;
  int m_stack_depth;

  // Misc state not in any stack
  Boolean m_normalize_enable;
  Boolean m_local_viewer;

  // This replaces the window system specific data members
  //! \todo Window_handle* m_win_handle;
  //! \todo Open_gl_os_init* m_gfx_handle;

  Boolean m_made_current;
  Int m_viewport[4];

  /*! Pointer to current Appearance */
  Appearance* m_last_app;
  Boolean m_is_current;

  Gfx* m_current_state;
  Gfx* m_default_state;

  Camera* m_active_camera;

private:
  Uint m_red_bits;
  Uint m_green_bits;
  Uint m_blue_bits;
  Uint m_alpha_bits;

  Uint m_accum_red_bits;
  Uint m_accum_green_bits;
  Uint m_accum_blue_bits;
  Uint m_accum_alpha_bits;

  Uint m_depth_bits;
  Uint m_stencil_bits;

  Uint m_number_of_samples;

  void draw_state_elements(const Bit_mask& set_mask, const Gfx* gfx);
  void draw_state_elements(const Bit_mask& set_mask, const Appearance* app);

public:
  void draw_texture(Shared_texture texture);
  void draw_halftone(Shared_halftone halftone);
  void draw_tex_mode(Gfx::Tex_mode tex_mode);
  void draw_tex_blend_color(const Vector4f& tex_blend_color);
  void draw_tex_env(Gfx::Tex_env tex_env);
  void draw_tex_gen(Shared_tex_gen tex_gen);
  void draw_tex_gen_enable(Boolean tex_gen_enable);
  void draw_material(Shared_material material, Shared_material back_material);
  void draw_light_enable(Boolean light_enable);
  void draw_transp_enable(Boolean transp_enable);
  void draw_transp_mode(Gfx::Transparency_mode transp_mode);
  void draw_alpha_func(Gfx::Alpha_func alpha_func);
  void draw_alpha_ref(Float alpha_ref);
  void draw_blend_color(const Vector4f& blend_color);
  void draw_src_blend_func(Gfx::Src_blend_func src_blend_func);
  void draw_dst_blend_func(Gfx::Dst_blend_func dst_blend_func);
  void draw_color_mask(const Vector4ub& color_mask);
  void draw_depth_enable(Boolean depth_enable);
  void draw_depth_func(Gfx::Depth_func depth_func);
  void draw_depth_mask(Boolean depth_mask);
  void draw_fog_enable(Boolean fog_enable);
  void draw_polygon_stipple_enable(Boolean enable);
  void draw_material_mode_enable(Gfx::Material_mode material_mode_enable);
  void draw_poly_mode(Gfx::Poly_mode poly_mode);
  void draw_line_width(float line_width);
  void draw_point_size(float point_size);
  void draw_line_stipple_pattern(unsigned int pattern);
  void draw_line_stipple_factor(unsigned int factor);
  void draw_tex_transform(const Matrix4f& matrix);
  void draw_back_material(Shared_material material,
                          Shared_material back_material);

  void draw_blend_funcs(Gfx::Src_blend_func src_blend_func,
                        Gfx::Dst_blend_func dst_blend_func);

private:
  void init();

  void init_context_attributes();

#define STATE_ELEMENT_COUNT (END_STATE_ELEMENTS - BEGIN_STATE_ELEMENTS)

  static Context* s_current_context;
};

/*! \brief obtains the texture attribute. */
inline Context::Shared_texture Context::get_texture() const
{ return m_current_state->m_texture; }

/*! \brief obtains the halftone attribute. */
inline Context::Shared_halftone Context::get_halftone() const
{ return m_current_state->m_halftone; }

/*! \brief obtains the texture enable attribute. */
inline Boolean Context::get_tex_enable() const
{ return m_current_state->m_tex_enable; }

/*! \brief obtains the texture mode attribute. */
inline Gfx::Tex_mode Context::get_tex_mode() const
{ return m_current_state->m_tex_mode; }

/*! \brief obtains the texture blend color attribute. */
inline void Context::get_tex_blend_color(Vector4f& tex_blend_color) const
{ tex_blend_color = m_current_state->m_tex_blend_color; }

/*! \brief obtains the texture environment attribute. */
inline Gfx::Tex_env Context::get_tex_env() const
{ return m_current_state->m_tex_env; }

/*! \brief obtains the texture generation attribute. */
inline Context::Shared_tex_gen Context::get_tex_gen() const
{ return m_current_state->m_tex_gen; }

/*! \brief obtains the texture generation enable attribute. */
inline Boolean Context::get_tex_gen_enable() const
{ return m_current_state->m_tex_gen_enable != 0; }

/*! \brief obtains the material attribute. */
inline Context::Shared_material Context::get_material() const
{ return m_current_state->m_material; }

/*! \brief */
inline Boolean Context::get_light_enable() const
{ return m_current_state->m_light_enable != 0; }

/*! \brief */
inline Gfx::Shade_model Context::get_shade_model() const
{ return m_current_state->m_shade_model; }

/*! \brief */
inline Boolean Context::get_transp_enable() const
{ return m_current_state->m_transp_enable != 0; }

/*! \brief */
inline Gfx::Transparency_mode Context::get_transp_mode() const
{ return m_current_state->m_transp_mode; }

/*! \brief */
inline Gfx::Alpha_func Context::get_alpha_func() const
{ return m_current_state->m_alpha_func; }

/*! \brief */
inline Float Context::get_alpha_ref() const
{ return m_current_state->m_alpha_ref; }

/*! \brief */
inline void Context::get_blend_color(Vector4f& blend_color) const
{ blend_color = m_current_state->m_blend_color; }

/*! \brief */
inline Gfx::Src_blend_func Context::get_src_blend_func() const
{ return m_current_state->m_src_blend_func; }

/*! \brief */
inline Gfx::Dst_blend_func Context::get_dst_blend_func() const
{ return m_current_state->m_dst_blend_func; }

/*! \brief */
inline void Context::get_color_mask(Vector4ub& color_mask) const
{ color_mask = m_current_state->m_color_mask; }

/*! \brief */
inline Gfx::Depth_func Context::get_depth_func() const
{ return m_current_state->m_depth_func; }

/*! \brief */
inline Boolean Context::get_depth_mask() const
{ return m_current_state->m_depth_mask; }

/*! \brief */
inline Boolean Context::get_fog_enable() const
{ return m_current_state->m_fog_enable; }

/*! \brief */
inline Boolean Context::get_polygon_stipple_enable() const
{ return m_current_state->m_polygon_stipple_enable; }

/*! \brief */
inline Gfx::Poly_mode Context::get_poly_mode() const
{ return m_current_state->m_poly_mode; }

/*! \brief */
inline Gfx::Cull_face Context::get_cull_face() const
{ return m_current_state->m_cull_face; }

/*! \brief obtains the attribute that indicates whether one- or two-sided
 * lighting calculations are done for polygons.
 */
inline Gfx::Light_model_sides Context::get_light_model_sides() const
{ return m_current_state->m_light_model_sides; }

/*! \brief obtains the attribute that specifies whether a single color should
 * be generated from the lighting computation for a vertex.
 */
inline Gfx::Light_model_color_control Context::get_light_model_color_control()
  const
{ return m_current_state->m_light_model_color_control; }

/*! \brief */
inline Float Context::get_line_width() const
{ return m_current_state->m_line_width; }

/*! \brief */
inline Float Context::get_point_size() const
{ return m_current_state->m_point_size; }

/*! \brief */
inline void Context::set_tex_blend_color(Float v0, Float v1, Float v2,
                                         Float v3)
{
  Vector4f tex_blend_color(v0, v1, v2, v3);
  set_tex_blend_color(tex_blend_color);
}

/*! \brief */
inline void Context::get_tex_blend_color(Float* v0, Float* v1,
                                         Float* v2, Float* v3) const
{
  Vector4f tex_blend_color;
  get_tex_blend_color(tex_blend_color);
  tex_blend_color.get(v0, v1, v2, v3);
}

/*! \brief */
inline void Context::set_blend_color(Float v0, Float v1, Float v2, Float v3)
{
  Vector4f blend_color(v0, v1, v2, v3);
  set_blend_color(blend_color);
}

/*! \brief */
inline void Context::get_blend_color(Float* v0, Float* v1,
                                     Float* v2, Float* v3) const
{
  Vector4f blend_color;
  get_blend_color(blend_color);
  blend_color.get(v0, v1, v2, v3);
}

/*! \brief */
inline void Context::set_color_mask(Ubyte v0, Ubyte v1, Ubyte v2, Ubyte v3)
{
  Vector4ub color_mask(v0, v1, v2, v3);
  set_color_mask(color_mask);
}

/*! \brief */
inline void Context::get_color_mask(Ubyte* v0, Ubyte* v1,
                                    Ubyte* v2, Ubyte* v3) const
{
  Vector4ub color_mask;
  get_color_mask(color_mask);
  color_mask.get(v0, v1, v2, v3);
}

/*! \brief */
inline void Context::get_matrix(Matrix4f& m) const
{ glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*) &m); }

/*! \brief */
inline Int Context::get_matrix_depth() const { return m_mat_stack_depth; }

/*! \brief */
inline void Context::push_matrix()
{
  glPushMatrix();
  m_mat_stack_depth++;
}

/*! \brief */
inline void Context::push_ident_matrix()
{
  glPushMatrix();
  glLoadIdentity();
  m_mat_stack_depth++;
}

/*! \brief */
inline void Context::pop_matrix()
{
  glPopMatrix();
  m_mat_stack_depth--;
}

/*! \brief */
inline void Context::load_matrix(const Matrix4f& m)
{ glLoadMatrixf((const GLfloat*) &m); }

/*! \brief */
inline void Context::load_ident_matrix()
{ glLoadIdentity(); }

/*! \brief */
inline void Context::mult_matrix(const Matrix4f& m)
{ glMultMatrixf((const GLfloat*) &m); }

/*! \brief */
inline void Context::translate(Float x, Float y, Float z)
{ glTranslatef(x, y, z); }

/*! \brief */
inline void Context::rotate(Float x, Float y, Float z, Float radians)
{ glRotatef(rad2deg(radians), x, y, z); }

/*! \brief */
inline void Context::scale(Float x, Float y, Float z) { glScalef(x, y, z); }

/*! \brief */
inline Boolean Context::get_normalize_enable() const
{ return m_normalize_enable; }

/*! \brief */
inline Boolean Context::get_local_viewer() const { return m_local_viewer; }

/*! \brief */
inline Uint Context::get_line_stipple_pattern() const
{ return(m_current_state->m_line_stipple_pattern); }

/*! \brief */
inline Uint Context::get_line_stipple_factor() const
{ return(m_current_state->m_line_stipple_factor); }

/*! \brief */
inline void Context::get_tex_transform(Matrix4f& xform) const
{ xform = m_current_state->m_tex_transform; }

/*! \brief */
inline Context::Shared_material Context::get_back_material() const
{ return m_current_state->m_back_material; }

/*! \brief */
inline Boolean Context::get_depth_enable() const
{ return m_current_state->m_depth_enable != 0; }

/*! \brief */
inline Gfx::Material_mode Context::get_material_mode_enable() const
{ return m_current_state->m_material_mode_enable; }

SGAL_END_NAMESPACE

#endif
