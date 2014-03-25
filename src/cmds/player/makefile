BASEDIR =.
MULTITHREAD =1
USE_SHARED =1
USE_SHARED_SGAL =1
USE_DYNAMIC_SGAL =0
USE_SHARED_SCGAL =1
USE_DYNAMIC_SCGAL =0
USE_SHARED_SEGO =1
USE_DYNAMIC_SEGO =1

ifeq ($(USE_CGAL), 1)
include $(ROOT)/make/cgaldef.mak
include $(MAKEDIR)/scgaldef.mak
else
include $(ROOT)/make/comdef.mak
endif

BASENAME = player
TARGET0 = $(BASENAME)$(EXEFILESUFFIX)
INSTALLDIR0 = "$(BINDIR)"

ifeq ($(DE), msvc)
ifeq ($(COMPILER_VER), 1400)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = "$(BINDIR)"
else
ifeq ($(COMPILER_VER), 1500)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = "$(BINDIR)"
else
ifeq ($(COMPILER_VER), 1600)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = "$(BINDIR)"
endif
endif
endif
endif

CPPSOURCES = player.cpp
CPPSOURCES+= Player_scene.cpp
CPPSOURCES+= Player_option_parser.cpp

LCPPDEFS =
LCPPOPTS =
LCPPINCS =

LLDLIBS =
LLDOPTS = $(CGALLDOPTS)

LCPPINCS+= -I$(BASEDIR)

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

ifdef V8_INC_DIR
LCPPINCS+= -I"$(V8_INC_DIR)"
endif

LCPPDEFS =
ifeq ($(TRACE), 1)
LCPPDEFS+= -DSGAL_TRACE
endif

ifeq ($(USE_CGAL), 1)
ifeq ($(USE_CGM), 1)
LCPPDEFS+= -DUSE_CGM
SGAL_INCLUDE_AOS=1
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

ifeq ($(USE_NEF), 1)
LCPPDEFS+= -DUSE_NEF
endif

ifeq ($(USE_ENV), 1)
LCPPDEFS+= -DUSE_ENV
endif

ifeq ($(USE_EGO), 1)
LCPPDEFS+= -DUSE_EGO
endif

# Include:
ifeq ($(USE_CGM), 1)
LCPPINCS+= -I$(CGM_3_WORKDIR)/include
endif

ifeq ($(USE_LTS), 1)
LCPPINCS+= -I$(LTS_3_WORKDIR)/include
endif

ifeq ($(SGAL_INCLUDE_AOS), 1)
LCPPINCS+= -I$(CGAL_WORKDIR)/Arrangement_on_surface_2/include
endif
ifeq ($(USE_VOS), 1)
LCPPINCS+= -I$(ENVELOPE_VORONOI_2_WORKDIR)/include
endif
ifeq ($(USE_NEF), 1)
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_S2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_3/include
endif
LCPPINCS+= $(CGALCPPINCS)

LCPPDEFS+= -DSCGAL_NT=$(SCGAL_NT)
LCPPDEFS+= -DSCGAL_KERNEL=$(SCGAL_KERNEL)
LCPPDEFS+= -DUSE_CGAL
endif

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

ifeq ($(DE), msvc)
LCPPDEFS+= -DBOOST_ALL_DYN_LINK
LCPPOPTS+= -GR  # Enables run-time type information (RTTI).
LCPPOPTS+= -vmg # Uses full generality for pointers to members.
LCPPOPTS+= -GS  # Apply buffers security check.
LCPPOPTS+= -Oy- # Disable Omission of frame pointer (x86 only).
# LCPPOPTS+= -WX  # Treat all compiler warnings as errors
LCPPOPTS+= -Zc:wchar_t -Zc:forScope # Specify standard behavior
# CPPWARNINGOPTS = -W4  # Specify the level of warning; 0 to 4

# Add BOOST, GL2PS, GMP, MFFR, and Magick lib directories:
ifdef BOOST_LIB_DIR
LLDOPTS+= -libpath:"$(BOOST_LIB_DIR)"
endif
ifdef GL2PS_LIB_DIR
LLDOPTS+= -libpath:"$(GL2PS_LIB_DIR)"
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
ifdef V8_LIB_DIR
LLDOPTS+= -libpath:"$(V8_LIB_DIR)"
endif

ifeq ($(USE_EGO), 1)
ifneq ($(USE_DYNAMIC_SEGO), 1)
LLDLIBS+= SEGO.lib
endif
endif

ifeq ($(USE_CGAL), 1)
ifneq ($(USE_DYNAMIC_SCGAL), 1)
LLDLIBS+= SCGAL.lib
endif
endif

ifneq ($(USE_DYNAMIC_SGAL), 1)
LLDLIBS+= SGAL.lib
endif

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
# LLDLIBS+= boost_system-vc$(VC_NUM)-mt-1_47.lib
# LLDLIBS+= boost_thread-vc$(VC_NUM)-mt-1_47.lib
# LLDLIBS+= boost_program_options-vc$(VC_NUM)-mt-1_47.lib
# LLDLIBS+= boost_filesystem-vc$(VC_NUM)-mt-1_47.lib
ifeq ($(USE_CGAL), 1)
LLDLIBS+= mpfr-vc$(VC_NUM)-mt.lib
LLDLIBS+= gmp-vc$(VC_NUM)-mt.lib
endif
# LLDLIBS+= gl2ps.lib
LLDLIBS+= glu32.lib opengl32.lib
LLDLIBS+= $(LDSYSLIBS)

LIBS =
ifneq ($(USE_DYNAMIC_SGAL), 1)
LIBS+= $(LIBDIR)/SGAL.lib
endif

ifeq ($(USE_CGAL), 1)
ifneq ($(USE_DYNAMIC_SCGAL), 1)
LIBS+= $(LIBDIR)/SCGAL.lib
endif
endif

ifeq ($(USE_EGO), 1)
ifneq ($(USE_DYNAMIC_SEGO), 1)
LIBS+= $(LIBDIR)/SEGO.lib
endif
endif
endif # msvc

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS+= -Wall
# LCPPOPTS+= -Werror
# LCPPOPTS+= -pedantic
LCPPOPTS+= `Magick++-config --cxxflags --cppflags`
LCPPOPTS+= -frounding-math
LLDOPTS+= -rdynamic -ldl
ifdef V8_LIB_DIR
LLDOPTS+= -L$(V8_LIB_DIR)
LLDOPTS+= -Wl,-rpath,$(V8_LIB_DIR)
endif
ifeq ($(USE_CGAL), 1)
ifneq ($(USE_DYNAMIC_SCGAL), 1)
LLDLIBS+= -lSCGAL
endif
endif

ifeq ($(USE_EGO), 1)
ifneq ($(USE_DYNAMIC_SEGO), 1)
LLDLIBS+= -lSEGO
endif
endif

ifneq ($(USE_DYNAMIC_SGAL), 1)
LLDLIBS+= -lSGAL
endif

ifeq ($(USE_CGAL), 1)
LLDLIBS+= $(CGALLDLIBS)
endif

ifeq ($(USE_GMP), 1)
LLDLIBS+= -lgmp -lmpfr -lgmpxx
endif

# V8 engine stuff:
LLDLIBS+= -lv8

LLDLIBS+= -lgl2ps
LLDLIBS+= -lMagick++
LLDLIBS+= -lboost_program_options$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_filesystem$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_thread$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_system$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lXxf86vm
LLDLIBS+= -lGLU -lGL
LLDLIBS+= -lXi
# LLDLIBS+= -lXmu
LLDLIBS+= -lX11 -lm

LIBS =
ifneq ($(USE_SHARED_SGAL), 1)
LIBS+= $(LIBDIR)/libSGAL.a
endif
ifeq ($(USE_CGAL), 1)
ifneq ($(USE_SHARED_SCGAL), 1)
LIBS+= $(LIBDIR)/libSCGAL.a
endif
endif
ifeq ($(USE_EGO), 1)
ifneq ($(USE_SHARED_SEGO), 1)
LIBS+= $(LIBDIR)/libSEGO.a
endif
endif
endif

LDIRT = parse.output

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/bisoncpp.mak
include $(MAKEDIR)/lexcpp.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlink.mak

seg_cached_lazy_sc_cgal_gmpq_inst:
	$(MAKEF) "SCGAL_NT=$(SCGAL_CGAL_GMPQ_NT)" "SCGAL_KERNEL=$(SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL)" install

# Linker additional command line options:
# /OPT:REF /OPT:ICF

x:
	echo $(LIBDIR)
