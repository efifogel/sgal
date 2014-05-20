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

# Target, etc.:
BASELIBNAME =SCGAL
BASEINSTALLDIR =$(ROOT)

TARGET0 = $(LIBNAME)
INSTALLDIR0 = $(LIBINSTALLDIR)

ifeq ($(DE), msvc)
TARGET1 = $(BASELIBNAME).lib
INSTALLDIR1 = $(LIBDIR)
endif

SCGAL_AOS_BASE = 0
ifeq ($(USE_AOS), 1)
SCGAL_AOS_BASE = 1
endif
ifeq ($(USE_VOS), 1)
SCGAL_AOS_BASE = 1
endif
ifeq ($(USE_SGM), 1)
SCGAL_AOS_BASE = 1
endif

SCGAL_ARRANGEMENT_ON_SURFACE = 0
ifeq ($(SCGAL_AOS_BASE), 1)
SCGAL_ARRANGEMENT_ON_SURFACE = 1
endif
ifeq ($(USE_QUADRIC), 1)
SCGAL_ARRANGEMENT_ON_SURFACE = 1
endif

CPPSOURCES =
ifeq ($(SCGAL_ARRANGEMENT_ON_SURFACE), 1)
CPPSOURCES+= Arrangement_on_surface_geo.cpp
CPPSOURCES+= Arrangement_renderer.cpp
endif
ifeq ($(SCGAL_AOS_BASE), 1)
CPPSOURCES+= Arrangement_on_sphere_base_geo.cpp
CPPSOURCES+= Arrangement_on_sphere_renderers.cpp
endif
ifeq ($(USE_AOS), 1)
CPPSOURCES+= Arrangement_on_sphere_geo.cpp
CPPSOURCES+= Arrangement_on_sphere_graph_geo.cpp
CPPSOURCES+= Arrangement_on_sphere_marked_geo.cpp
# Ignore in Debug mode, cause is_valid() on the sphere does not compile
CPPSOURCES+= Polygon_set_on_sphere_geo.cpp
CPPSOURCES+= Arrangement_on_sphere_overlay_geo.cpp
CPPSOURCES+= Arrangement_on_sphere_sim_geo.cpp
endif
ifeq ($(USE_VOS), 1)
CPPSOURCES+= Geodesic_voronoi_on_sphere_geo.cpp
CPPSOURCES+= Voronoi_diagram_on_sphere_geo.cpp
CPPSOURCES+= Power_diagram_on_sphere_geo.cpp
endif
ifeq ($(USE_QUADRIC), 1)
CPPSOURCES+= Vector3d.cpp
CPPSOURCES+= Quadric.cpp
CPPSOURCES+= Quadric_geo.cpp
CPPSOURCES+= Arrangement_on_quadric_geo.cpp
endif

ifeq ($(USE_AOT), 1)
CPPSOURCES+= Arrangement_on_torus_base_geo.cpp
CPPSOURCES+= Arrangement_on_torus_geo.cpp
CPPSOURCES+= Arrangement_on_torus_renderers.cpp
endif

CPPSOURCES+= Boolean_operation.cpp
CPPSOURCES+= init.cpp
CPPSOURCES+= Polyhedron_geo.cpp
CPPSOURCES+= Exact_coord_array_2d.cpp
CPPSOURCES+= Exact_coord_array.cpp
CPPSOURCES+= Exact_coord_minkowski.cpp
CPPSOURCES+= Exact_normal_array.cpp
CPPSOURCES+= Exact_plane_array.cpp
CPPSOURCES+= Exact_polyhedron_geo.cpp
CPPSOURCES+= Smallest_stabbing_cube.cpp
CPPSOURCES+= Triangulation_geo.cpp
ifeq ($(USE_ENV), 1)
CPPSOURCES+= Lower_envelope_geo.cpp
CPPSOURCES+= Lower_envelope_tri_geo.cpp
CPPSOURCES+= Lower_envelope_sphere_geo.cpp
CPPSOURCES+= Lower_envelope_plane_geo.cpp
endif
ifeq ($(USE_CGM), 1)
CPPSOURCES+= Cubical_gaussian_map_geo.cpp
CPPSOURCES+= Arrangement_on_cube_renderers.cpp
endif
ifeq ($(USE_SGM), 1)
CPPSOURCES+= Spherical_gaussian_map_base_geo.cpp
CPPSOURCES+= Spherical_gaussian_map_geo.cpp
CPPSOURCES+= Spherical_gaussian_map_colored_geo.cpp
CPPSOURCES+= Spherical_gaussian_map_marked_geo.cpp
CPPSOURCES+= Assembly.cpp
CPPSOURCES+= Assembly_part.cpp
endif
ifeq ($(USE_NEF), 1)
CPPSOURCES+= Exact_nef_polyhedron.cpp
ifeq ($(USE_NGM), 1)
CPPSOURCES+= Nef_gaussian_map_geo.cpp
endif
endif

ifeq ($(USE_LTS), 1)
CPPSOURCES+= Lines_through_segments_geo.cpp
endif

LCPPDEFS+= -DSGAL_LIB
LCPPDEFS+= -DSGAL_SCGAL_SOURCE
ifeq ($(TRACE), 1)
LCPPDEFS+= -DSGAL_TRACE
endif

LCPPDEFS+= -DSCGAL_NT=$(SCGAL_NT)
LCPPDEFS+= -DSCGAL_KERNEL=$(SCGAL_KERNEL)

# LCPPDEFS+= -DVERBOSE

ifdef CGAL_ARR_TRACING_TRAITS
LCPPDEFS+= -DCGAL_ARR_TRACING_TRAITS
LOBJDIR :=$(LOBJDIR)_arr_tracing_traits
endif

ifdef CGAL_ARR_GEODESIC_ARC_ON_SPHERE_DETAILS
LCPPDEFS+= -DCGAL_ARR_GEODESIC_ARC_ON_SPHERE_DETAILS
LOBJDIR :=$(LOBJDIR)_arr_geodesic_arc_on_sphere_details
endif

ifeq ($(CGAL_ARR_SPHERICAL_GAUSSIAN_MAP_3_DEBUG), 1)
LCPPDEFS+= -DCGAL_ARR_SPHERICAL_GAUSSIAN_MAP_3_DEBUG=1
LOBJDIR :=$(LOBJDIR)_arr_spherical_gaussian_map_3_debug
endif

ifeq ($(CGAL_IDENTIFICATION_XY), $(CGAL_X_MINUS_8_Y_6))
LCPPDEFS+= -DCGAL_IDENTIFICATION_XY=CGAL_X_MINUS_8_Y_6
LOBJDIR :=$(LOBJDIR)_identification_xy_x_minus_8_y_6
else
ifeq ($(CGAL_IDENTIFICATION_XY), $(CGAL_X_MINUS_11_Y_7))
LCPPDEFS+= -DCGAL_IDENTIFICATION_XY=CGAL_X_MINUS_11_Y_7
LOBJDIR :=$(LOBJDIR)_identification_xy_x_minus_11_y_7
endif
endif

ifdef CGAL_SL_VERBOSE
LCPPDEFS+= -DCGAL_SL_VERBOSE
LOBJDIR :=$(LOBJDIR)_sl
endif

ifeq ($(USE_CGM), 1)
LCPPDEFS+= -DUSE_CGM
SGAL_INCLUDE_AOS=1
endif

ifeq ($(USE_NGM), 1)
LCPPDEFS+= -DUSE_NGM
endif

ifeq ($(USE_LTS), 1)
LCPPDEFS+= -DUSE_LTS
SGAL_INCLUDE_AOS=1
endif

ifeq ($(USE_SGM), 1)
LCPPDEFS+= -DUSE_SGM
SGAL_INCLUDE_AOS=1
endif

ifeq ($(USE_AOS), 1)
LCPPDEFS+= -DUSE_AOS
SGAL_INCLUDE_AOS=1
endif

ifeq ($(USE_VOS), 1)
LCPPDEFS+= -DUSE_VOS
SGAL_INCLUDE_AOS=1
endif

ifeq ($(USE_NEF), 1)
LCPPDEFS+= -DUSE_NEF
endif

ifeq ($(USE_QUADRIC), 1)
LCPPDEFS+= -DUSE_QUADRIC
endif

ifeq ($(USE_ENV), 1)
LCPPDEFS+= -DUSE_ENV
endif

# Include:
LCPPINCS = -I$(BASEDIR)
LCPPINCS+= -I$(BASEDIR)/../include
LCPPINCS+= -I$(BASEDIR)/../include/primitives
LCPPINCS+= -I$(BASEDIR)/../include/engines
LCPPINCS+= -I$(BASEDIR)/../include/geo
LCPPINCS+= -I$(BASEDIR)/../include/geometries

# Add BOOST, GMP, MPFR, Magick, and gl2ps include directory:
ifdef BOOST_INC_DIR
LCPPINCS+= -I"$(BOOST_INC_DIR)"
endif

ifdef GMP_INC_DIR
LCPPINCS+= -I"$(GMP_INC_DIR)"
endif

ifdef MPFR_INC_DIR
LCPPINCS+= -I"$(MPFR_INC_DIR)"
endif

ifdef MAGICK_INC_DIR
LCPPINCS+= -I"$(MAGICK_INC_DIR)"
endif

ifdef GL2PS_INC_DIR
LCPPINCS+= -I"$(GL2PS_INC_DIR)"
endif

#
ifeq ($(USE_CGM), 1)
LCPPINCS+= -I$(CGM_3_WORKDIR)/include
endif

ifeq ($(USE_LTS), 1)
LCPPINCS+= -I$(LTS_3_WORKDIR)/include
endif

ifeq ($(SGAL_INCLUDE_AOS), 1)
LCPPINCS+= -I$(CGAL_WORKDIR)/Arrangement_on_surface_2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Boolean_set_operations_2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Partition_2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Polygon/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Envelope_3/include
endif
ifeq ($(USE_VOS), 1)
LCPPINCS+= -I$(ENVELOPE_VORONOI_2_WORKDIR)/include
endif

ifeq ($(USE_NEF), 1)
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_3/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_S2/include
endif

ifeq ($(USE_QUADRIC), 1)
ifdef EXACUS_INC_DIR
LCPPINCS+= -I$(EXACUS_INC_DIR)
else
ifdef EXACUS_DIR
LCPPINCS+= -I$(EXACUS_DIR)/include
endif
endif

endif

LCPPINCS+= $(CGALCPPINCS)

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS+= -Wall
# LCPPOPTS+= -Werror
# LCPPOPTS+= -pedantic
LCPPOPTS+= `Magick++-config --cxxflags --cppflags`
LCPPOPTS+= -frounding-math
endif

ifeq ($(DE), msvc)
LCPPDEFS+= -DBOOST_ALL_DYN_LINK
LCPPDEFS+= -DSGAL_ALL_DYN_LINK
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

ifdef GMP_LIB_DIR
LLDOPTS+= -libpath:"$(GMP_LIB_DIR)"
endif
ifdef MPFR_LIB_DIR
LLDOPTS+= -libpath:"$(MPFR_LIB_DIR)"
endif
LLDOPTS+= -LTCG  # whole-program optimization

LLDLIBS+= SGAL.lib
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

VC_NUM=80
ifeq ($(COMPILER_VER), 1400)
VC_NUM=90
endif
ifeq ($(COMPILER_VER), 1600)
VC_NUM=100
endif
LLDLIBS+= mpfr-vc$(VC_NUM)-mt.lib
LLDLIBS+= gmp-vc$(VC_NUM)-mt.lib
LLDLIBS+= glu32.lib opengl32.lib
LLDLIBS+= $(LDSYSLIBS)

LIBS =
ifneq ($(USE_DYNAMIC_SGAL), 1)
LIBS+= $(LIBDIR)/SGAL.lib
endif

endif # DEF_SHARED

endif # msvc

LDIRT = parse.output

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targ.mak

vpath %.cpp $(BASEDIR)/geo
vpath %.cpp $(BASEDIR)/engines
vpath %.cpp $(BASEDIR)/geometries

seg_std_double:
	$(MAKEF) "SCGAL_NT=$(SCGAL_DOUBLE_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)"

seg_std_quotient_mp_float:
	$(MAKEF) "SCGAL_NT=$(SCGAL_QUOTIENT_MP_FLOAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)"

seg_std_cgal_gmpq:
	$(MAKEF) "SCGAL_NT=$(SCGAL_CGAL_GMPQ_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)"

seg_std_leda_rat:
	$(MAKEF) "SCGAL_NT=$(SCGAL_LEDA_RAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)"

seg_std_double_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_DOUBLE_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)" install

seg_std_quotient_mp_float_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_QUOTIENT_MP_FLOAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)" install

seg_std_cgal_gmpq_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_CGAL_GMPQ_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)" install

seg_std_leda_rat_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_LEDA_RAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS)" install

#

seg_cached_double:
	$(MAKEF) "SCGAL_NT=$(SCGAL_DOUBLE_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)"

seg_cached_quotient_mp_float:
	$(MAKEF) "SCGAL_NT=$(SCGAL_QUOTIENT_MP_FLOAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)"

seg_cached_cgal_gmpq:
	$(MAKEF) "SCGAL_NT=$(SCGAL_CGAL_GMPQ_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)"

seg_cached_leda_rat:
	$(MAKEF) "SCGAL_NT=$(SCGAL_LEDA_RAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)"

seg_cached_double_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_DOUBLE_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)" install

seg_cached_quotient_mp_float_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_QUOTIENT_MP_FLOAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)" install

seg_cached_cgal_gmpq_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_CGAL_GMPQ_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)" install

seg_cached_leda_rat_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_LEDA_RAT_NT)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)" install

seg_cached_lazy_sc_cgal_gmpq_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_CGAL_GMPQ_NT)" "SCGAL_KERNEL=$(SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL)" "CGAL_CGM_TRAITS=$(CGAL_ARR_SEGMENT_TRAITS)" install

x:
	echo $(BOOST_LIB_DIR_QUOTELESS)
