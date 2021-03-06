# Copyright (c) 2004  Israel.
# All rights reserved.
#
# This file is part of the collection of Efi Fogel.
# You may redistribute it under the terms of the Q Public License version 1.0.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Author(s)     : Efi Fogel

BASEDIR =.
MULTITHREAD =1
USE_SHARED =1
DEF_SHARED =1

include $(ROOT)/make/cgaldef.mak
include $(MAKEDIR)/scgaldef.mak

BASELIBNAME =SGAL
BASEINSTALLDIR =$(ROOT)

TARGET0 = $(LIBNAME)
INSTALLDIR0 = $(LIBINSTALLDIR)

ifeq ($(DE), msvc)
TARGET1 = $(BASELIBNAME).lib
INSTALLDIR1 = $(LIBDIR)
endif

BISONSOURCES = parse.y
LEXSOURCES = scan.l

CPPSOURCES = assertions.cpp
CPPSOURCES+= Event_handler.cpp
CPPSOURCES+= Event.cpp

# engines
CPPSOURCES+= Sphere_plane_intersection.cpp
CPPSOURCES+= Incrementor.cpp

# events:
CPPSOURCES+= Draw_event.cpp
CPPSOURCES+= Keyboard_event.cpp
CPPSOURCES+= Mouse_event.cpp
CPPSOURCES+= Motion_event.cpp
CPPSOURCES+= Passive_motion_event.cpp
CPPSOURCES+= Redraw_event.cpp
CPPSOURCES+= Reset_event.cpp
CPPSOURCES+= Reshape_event.cpp
CPPSOURCES+= Simulate_event.cpp
CPPSOURCES+= Start_event.cpp
CPPSOURCES+= Stop_event.cpp
CPPSOURCES+= Tick_event.cpp

# Parser:
CPPSOURCES+= Element.cpp

# scene_graph:
CPPSOURCES+= Accumulation.cpp
CPPSOURCES+= Appearance.cpp
CPPSOURCES+= Arc.cpp
CPPSOURCES+= Background.cpp
CPPSOURCES+= Bindable_node.cpp
CPPSOURCES+= Bindable_stack.cpp
CPPSOURCES+= Boundary_set.cpp
CPPSOURCES+= Box.cpp
CPPSOURCES+= Billboard.cpp
CPPSOURCES+= Camera.cpp
CPPSOURCES+= Collision.cpp
CPPSOURCES+= Color_background.cpp
CPPSOURCES+= Color_map.cpp
CPPSOURCES+= Cone.cpp
CPPSOURCES+= Configuration.cpp
CPPSOURCES+= Container.cpp
CPPSOURCES+= Container_factory.cpp
CPPSOURCES+= Container_proto.cpp
CPPSOURCES+= Context.cpp
CPPSOURCES+= Color_array.cpp
CPPSOURCES+= Coord_array_2d.cpp
CPPSOURCES+= Coord_array_3d.cpp
CPPSOURCES+= Coord_array.cpp
CPPSOURCES+= Coord_transformer.cpp
CPPSOURCES+= Coord_minkowski.cpp
CPPSOURCES+= Composed_shader.cpp
CPPSOURCES+= Cube_environment.cpp
CPPSOURCES+= Cull_context.cpp
CPPSOURCES+= Cylinder.cpp
CPPSOURCES+= Cylindrical_patch.cpp
CPPSOURCES+= Directional_light.cpp
CPPSOURCES+= Draw_action.cpp
CPPSOURCES+= Ellipsoid.cpp
CPPSOURCES+= Environment_background.cpp
CPPSOURCES+= Exporter.cpp
CPPSOURCES+= Elevation_grid.cpp
CPPSOURCES+= Extrusion.cpp
CPPSOURCES+= Field.cpp
CPPSOURCES+= Field_info.cpp
CPPSOURCES+= Font.cpp
CPPSOURCES+= Font_outliner.cpp
CPPSOURCES+= Font_style.cpp
CPPSOURCES+= Fragment_shader.cpp
CPPSOURCES+= Frustum.cpp
CPPSOURCES+= Geo_set.cpp
CPPSOURCES+= Geodesic.cpp
CPPSOURCES+= Gfx_conf.cpp

ifeq ($(USE_GLUT), 1)
CPPSOURCES+= Glut_font.cpp
endif

CPPSOURCES+= Gl_wrapper.cpp
CPPSOURCES+= Group.cpp
CPPSOURCES+= Geometry.cpp
CPPSOURCES+= Halftone.cpp
CPPSOURCES+= Image.cpp
CPPSOURCES+= Image_background.cpp
CPPSOURCES+= Image_base.cpp
CPPSOURCES+= Image_sphere_environment.cpp
CPPSOURCES+= Image_texture.cpp
CPPSOURCES+= Imagemagick_font.cpp
CPPSOURCES+= Indexed_face_set.cpp
CPPSOURCES+= Indexed_line_set.cpp
CPPSOURCES+= Indexed_line_set_draw.cpp
CPPSOURCES+= Indexed_geodesic_set.cpp
CPPSOURCES+= Indexed_face_set_draw.cpp
CPPSOURCES+= initialize.cpp
CPPSOURCES+= Item_primitive.cpp
CPPSOURCES+= Light.cpp
CPPSOURCES+= Loader.cpp
CPPSOURCES+= Isect_action.cpp
CPPSOURCES+= Material.cpp
CPPSOURCES+= Math_defs.cpp
CPPSOURCES+= Mesh_set.cpp
CPPSOURCES+= Multisample.cpp
CPPSOURCES+= Navigation_info.cpp
CPPSOURCES+= Node.cpp
CPPSOURCES+= Normal_array.cpp
CPPSOURCES+= Piece.cpp
CPPSOURCES+= Point_light.cpp
# CPPSOURCES+= Record_action.cpp
CPPSOURCES+= Rotation.cpp
CPPSOURCES+= Route.cpp
CPPSOURCES+= Scene_graph.cpp
CPPSOURCES+= Script_base.cpp
ifeq ($(USE_V8), 1)
CPPSOURCES+= Script.cpp
endif
CPPSOURCES+= Simulation.cpp
CPPSOURCES+= Shader.cpp
CPPSOURCES+= Shape.cpp
CPPSOURCES+= Snapshot.cpp
CPPSOURCES+= Sphere.cpp
CPPSOURCES+= Sphere_bound.cpp
CPPSOURCES+= Sphere_environment.cpp
CPPSOURCES+= Spot_light.cpp
CPPSOURCES+= Switch.cpp
CPPSOURCES+= Tex_coord_array.cpp
CPPSOURCES+= Tex_coord_array_2d.cpp
CPPSOURCES+= Tex_coord_array_3d.cpp
CPPSOURCES+= Tex_coord_array_4d.cpp
CPPSOURCES+= Tex_gen.cpp
# CPPSOURCES+= Text_texture.cpp
CPPSOURCES+= Text.cpp
CPPSOURCES+= Texture.cpp
CPPSOURCES+= Texture_2d.cpp
CPPSOURCES+= Texture_font.cpp
CPPSOURCES+= Texture_transform.cpp
CPPSOURCES+= Torus.cpp
CPPSOURCES+= Trace.cpp
CPPSOURCES+= Transform.cpp
CPPSOURCES+= Vertex_shader.cpp
CPPSOURCES+= World_info.cpp

# Sensors:
CPPSOURCES+= Drag_sensor.cpp
CPPSOURCES+= Key_sensor.cpp
CPPSOURCES+= Navigation_sensor.cpp
CPPSOURCES+= Proximity_sensor.cpp
CPPSOURCES+= Single_key_sensor.cpp
CPPSOURCES+= Time_sensor.cpp
CPPSOURCES+= Touch_sensor.cpp

# Geometry:
CPPSOURCES = Bounding_box.cpp
CPPSOURCES+= Line.cpp
CPPSOURCES+= Matrix3f.cpp
CPPSOURCES+= Matrix4f.cpp
CPPSOURCES+= Plane.cpp
CPPSOURCES+= Utilities.cpp
CPPSOURCES+= Vector2f.cpp
CPPSOURCES+= Vector2u.cpp
CPPSOURCES+= Vector2sh.cpp
CPPSOURCES+= Vector3f.cpp
CPPSOURCES+= Vector3sh.cpp
CPPSOURCES+= Vector4f.cpp

# Utilities:
CPPSOURCES+= Generic_option_parser.cpp
CPPSOURCES+= Bench_option_parser.cpp
CPPSOURCES+= Conf_option_parser.cpp
CPPSOURCES+= File_format.cpp
CPPSOURCES+= IO_option_parser.cpp
CPPSOURCES+= Option_parser.cpp
CPPSOURCES+= Native_window_item.cpp
CPPSOURCES+= Window_option_parser.cpp
ifeq ($(WS), console)
CPPSOURCES+= Windows_window_item.cpp
CPPSOURCES+= Windows_window_manager.cpp
endif
ifeq ($(WS), X11)
CPPSOURCES+= X11_window_item.cpp
CPPSOURCES+= X11_window_manager.cpp
endif

# Interpolators:
CPPSOURCES+= Interpolator.cpp
CPPSOURCES+= Rotation_interpolator.cpp
CPPSOURCES+= Position_interpolator.cpp
CPPSOURCES+= Coordinate_interpolator.cpp
CPPSOURCES+= Vector3f_interpolator.cpp
CPPSOURCES+= Scalar_interpolator.cpp
CPPSOURCES+= Color_interpolator.cpp

# IO
CPPSOURCES+= Stl_formatter.cpp
CPPSOURCES+= Text_formatter.cpp
CPPSOURCES+= Vrml_formatter.cpp

LCPPDEFS =
LCPPOPTS =
LCPPINCS =

LCPPINCS+= -I.
LCPPINCS+= -I$(BASEDIR)
LCPPINCS+= -I$(BASEDIR)/loaders
LCPPINCS+= -I$(BASEDIR)/primitives
LCPPINCS+= -I$(BASEDIR)/interpolators

SGAL_INC_DIR = $(BASEDIR)/../include
LCPPINCS+= -I$(SGAL_INC_DIR)
LCPPINCS+= -I$(SGAL_INC_DIR)/engines
LCPPINCS+= -I$(SGAL_INC_DIR)/events
LCPPINCS+= -I$(SGAL_INC_DIR)/geometries
LCPPINCS+= -I$(SGAL_INC_DIR)/geometries/primitive_shapes
LCPPINCS+= -I$(SGAL_INC_DIR)/IO
LCPPINCS+= -I$(SGAL_INC_DIR)/loaders
LCPPINCS+= -I$(SGAL_INC_DIR)/sensors
LCPPINCS+= -I$(SGAL_INC_DIR)/utilities
LCPPINCS+= -I$(SGAL_INC_DIR)/geo
LCPPINCS+= -I$(SGAL_INC_DIR)/interpolators
LCPPINCS+= -I$(SGAL_INC_DIR)/lights
LCPPINCS+= -I$(SGAL_INC_DIR)/primitives

# Add BOOST, Magick, and gl2ps include directory:
ifdef BOOST_INC_DIR
LCPPINCS+= -I"$(BOOST_INC_DIR)"
endif

ifdef MAGICK_INC_DIR
LCPPINCS+= -I"$(MAGICK_INC_DIR)"
endif

ifdef GL2PS_INC_DIR
LCPPINCS+= -I"$(GL2PS_INC_DIR)"
endif

ifdef V8_INC_DIR
LCPPINCS+= -I"$(V8_INC_DIR)"
endif

LCPPDEFS+= -DSGAL_LIB
LCPPDEFS+= -DSGAL_SGAL_SOURCE
ifeq ($(TRACE), 1)
LCPPDEFS+= -DSGAL_TRACE
endif

ifeq ($(USE_V8), 1)
LCPPDEFS+= -DUSE_V8
endif

# Extract flex version
FLEX_MAJOR_VER_CMD = expr "$(flex --version)" : 'flex \([0-9]\)\.[0-9].[0-9]'
FLEX_MINOR_VER_CMD = expr "$(flex --version)" : 'flex [0-9]\.\([0-9]\).[0-9]'
FLEX_UPDATE_VER_CMD = expr "$(flex --version)" : 'flex [0-9]\.[0-9].\([0-9]\)'
FLEX_VER_CMD = $(expr $(expr $(FLEX_MAJOR_VER) * 1000) + $(expr $(expr $(FLEX_MINOR_VER) * 100) + $(FLEX_UPDATE_VER)))

FLEX_MAJOR_VER = $(shell $(FLEX_MAJOR_VER_CMD))
FLEX_MINOR_VER = $(shell $(FLEX_MINOR_VER_CMD))
FLEX_UPDATE_VER = $(shell $(FLEX_UPDATE_VER_CMD))
FLEX_VER = $(shell $(FLEX_VER_CMD))

# Extract bison version

BISON_MAJOR_VER_CMD = expr "$$(bison --version)" : 'bison[^0-9]\+\([0-9]\)\.[0-9]'
BISON_MINOR_VER_CMD = expr "$$(bison --version)" : 'bison[^0-9]\+[0-9]\.\([0-9]\)'
BISON_VER_CMD = $(expr $(expr $(BISON_MAJOR_VER) * 1000) + $(expr $(BISON_MINOR_VER) * 100))

BISON_MAJOR_VER = $(shell $(BISON_MAJOR_VER_CMD))
BISON_MINOR_VER = $(shell $(BISON_MINOR_VER_CMD))
BISON_VER = $(shell $(BISON_VER_CMD))

# Compilation define switch:
# LCPPDEFS+= -DSGAL_FLEX_VER=$(FLEX_VER)

ifeq ($(DE), msvc)
LCPPDEFS+= -DBOOST_ALL_DYN_LINK
LCPPDEFS+= -DSGAL_ALL_DYN_LINK
# LCPPOPTS+= -fp:precise
LCPPOPTS+= -GR  # Enables run-time type information (RTTI).
LCPPOPTS+= -vmg # Uses full generality for pointers to members.
LCPPOPTS+= -GS  # Apply buffers security check.
LCPPOPTS+= -Oy- # Disable Omission of frame pointer (x86 only).
# LCPPOPTS+= -WX  # Treat all compiler warnings as errors
LCPPOPTS+= -Zc:wchar_t -Zc:forScope # Specify standard behavior
# CPPWARNINGOPTS = -W4  # Specify the level of warning; 0 to 4

ifeq ($(DEF_SHARED), 1)
LCPPDEFS+= -DGL2PSDLL

ifdef BOOST_LIB_DIR
LLDOPTS+= -libpath:"$(BOOST_LIB_DIR)"
endif
ifdef MAGICK_LIB_DIR
LLDOPTS+= -libpath:"$(MAGICK_LIB_DIR)"
endif
ifdef V8_LIB_DIR
LLDOPTS+= -libpath:"$(V8_LIB_DIR)"
endif
LLDOPTS+= -LTCG  # whole-program optimization

ifeq ($(DEBUG), 1)
# Using the debug version is not really necessary.
# If the debug libraries are needed though, they must be compiled from sources
# (as they are not installed).
# LLDLIBS+= CORE_DB_Magick++_.lib
LLDLIBS+= CORE_RL_Magick++_.lib
else
LCPPOPTS+= -Gy  # Enable function-level linking
LCPPOPTS+= -Oi  # Generate intrinsic functions
LLDLIBS+= CORE_RL_Magick++_.lib
endif
LLDLIBS+= v8.lib
LLDLIBS+= glu32.lib opengl32.lib
LLDLIBS+= $(LDSYSLIBS)
endif

endif

LCPPINCS+= $(CGALCPPINCS)

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS+= -Wall
LCPPOPTS+= -Werror
# LCPPOPTS+= -pedantic
LCPPOPTS+=`Magick++-config --cxxflags --cppflags`
endif

LDIRT = parse.output

vpath %.cpp $(BASEDIR)/primitives
vpath %.cpp $(BASEDIR)/engines
vpath %.cpp $(BASEDIR)/events
vpath %.cpp $(BASEDIR)/loaders
vpath %.cpp $(BASEDIR)/geo
vpath %.cpp $(BASEDIR)/lights
vpath %.cpp $(BASEDIR)/sensors
vpath %.cpp $(BASEDIR)/geometries
vpath %.cpp $(BASEDIR)/geometries/primitive_shapes
vpath %.cpp $(BASEDIR)/utilities
vpath %.cpp $(BASEDIR)/interpolators
vpath %.cpp $(BASEDIR)/IO
vpath %.y $(BASEDIR)/loaders
vpath %.l $(BASEDIR)/loaders

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/bisoncpp.mak
include $(MAKEDIR)/lexcpp.mak

# For older versions of flex (less than 2.5.31)
ifeq ($(DE), msvc)
ifeq ($(COMPILER_VER), 1600)
# The insertion of 'include <stdint.h>' prevents a warning at least with VC 10
define run-lex
$(LEXF) -d -t -+ $(firstword $^) | \
sed -e 's/#define yyFlexLexer yyFlexLexer//' \
-e 's/class istream;/#include <iostream>/' \
-e 's/istream/std::istream/' \
-e 's/std::std/std/' \
-e '1 a #include <stdint.h>' \
> $@
endef
else
define run-lex
$(LEXF) -d -t -+ $(firstword $^) | \
sed -e 's/#define yyFlexLexer yyFlexLexer//' \
-e 's/class istream;/#include <iostream>/' \
-e 's/istream/std::istream/' \
-e 's/std::std/std/' \
> $@
endef
endif
else
define run-lex
$(LEXF) -d -t -+ $(firstword $^) | \
sed -e 's/#define yyFlexLexer yyFlexLexer//' \
-e 's/class istream;/#include <iostream>/' \
-e 's/istream/std::istream/' \
-e 's/std::std/std/' \
> $@
endef
endif

ifeq ($(BISON_VER), 2500)
define run-bison
sed -e 's/%require "2.7"/%require "2.5"/' \
    -e 's/%define api.namespace "SGAL"/%define namespace "SGAL"/' \
$(firstword $^) > tmp.y
$(BISONF) tmp.y -o $(basename $@).cpp
endef
endif

ifeq ($(BISON_VER), 2700)
define fix-bison
sed -i -e 's/0 < count || -count < line/0 < count || -count < (int) line/' \
       -e 's/0 < count || -count < column/0 < count || -count < (int) column/' \
position.hh
endef
else
define fix-bison
endef
endif

include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targ.mak

parse.cpp parse.hpp: parse.y
	$(run-bison)
	$(fix-bison)

scan.o: parse.hpp

# cancel the built-in implicit rule for yacc:
%.c : %.y

.PHONY :: flex-ver bison-ver
flex-ver:
	@echo $(FLEX_VER)

bison-ver:
	@echo $(BISON_VER)
