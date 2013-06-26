BASEDIR =.
MULTITHREAD =1
USE_SHARED =1
USE_SHARED_SGAL =1
USE_DYNAMIC_SGAL =0
USE_SHARED_SCGAL =1
USE_DYNAMIC_SCGAL =0
USE_SHARED_SEGO =1
USE_DYNAMIC_SEGO =1

include $(ROOT)/make/cgaldef.mak
include $(MAKEDIR)/scgaldef.mak

BASENAME = player2
TARGET0 = $(BASENAME)$(EXEFILESUFFIX)
INSTALLDIR0 = $(BINDIR)

ifeq ($(DE), msvc)
ifeq ($(COMPILER_VER), 1400)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = $(BINDIR)
else
ifeq ($(COMPILER_VER), 1500)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = $(BINDIR)
else
ifeq ($(COMPILER_VER), 1600)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = $(BINDIR)
endif
endif
endif
endif

CPPSOURCES = player.cpp
CPPSOURCES+= Player_scene.cpp
CPPSOURCES+= Player_option_parser.cpp
CPPSOURCES+= Player2_scene.cpp
CPPSOURCES+= Player2_option_parser.cpp

LCPPDEFS =
LCPPINCS =
LCPPOPTS =

LLDLIBS =
LLDOPTS =

LCPPINCS+= -I$(BASEDIR)
LCPPINCS+= -I$(BASEDIR)/../player

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

ifeq ($(USE_CGM), 1)
LCPPDEFS+= -DUSE_CGM
endif

ifeq ($(USE_SGM), 1)
LCPPDEFS+= -DUSE_SGM
endif

ifeq ($(USE_AOS), 1)
LCPPDEFS+= -DUSE_AOS
endif

ifeq ($(USE_NEF), 1)
LCPPDEFS+= -DUSE_NEF
endif

LCPPINCS+= -I$(CGAL_WORKDIR)/Arrangement_on_surface_2/include
LCPPINCS+= -I$(CGM_3_WORKDIR)/include
ifeq ($(USE_NEF), 1)
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_S2/include
# LCPPINCS+= -I$(CGAL_WORKDIR)/Nef_3/include
endif
LCPPINCS+= $(CGALCPPINCS)

LCPPDEFS+= -DSCGAL_NT=$(SCGAL_NT)
LCPPDEFS+= -DCGAL_CGM_TRAITS=$(CGAL_CGM_TRAITS)
LCPPDEFS+= -DSCGAL_KERNEL=$(SCGAL_KERNEL)
LCPPDEFS+= -DUSE_CGAL

ifeq ($(DE), msvc)
LCPPDEFS+= -DBOOST_ALL_DYN_LINK
LCPPOPTS+= -GR
LCPPOPTS+= -vmg
LCPPOPTS+= -errorReport:prompt
# LCPPOPTS+= -W3

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

LLDLIBS+= SCGAL.lib
LLDLIBS+= SGAL.lib
# LLDLIBS+= gl2ps.lib
ifeq ($(DEBUG), 1)
LLDLIBS+= CORE_DB_Magick++_.lib
else
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
LIBS+= $(ROOT)/lib/SGAL.lib
endif
ifneq ($(USE_DYNAMIC_SCGAL), 1)
LIBS+= $(ROOT)/lib/SCGAL.lib
endif

endif # msvc

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS+= -Wall
# LCPPOPTS+= -Werror
# LCPPOPTS+= -pedantic
LLDOPTS+= -rdynamic -ldl
LLDOPTS+= $(CGALLDOPTS)

ifneq ($(USE_DYNAMIC_SCGAL), 1)
LLDLIBS+= -lSCGAL
endif
ifneq ($(USE_DYNAMIC_SCGAL), 1)
LLDLIBS+= -lSCGAL
endif
LLDLIBS+= $(CGALLDLIBS)
ifeq ($(USE_GMP), 1)
LLDLIBS+= -lgmp -lmpfr -lgmpxx
endif

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
LIBS+= $(ROOT)/lib/libSGAL.a
endif
ifneq ($(USE_SHARED_SCGAL), 1)
LIBS+= $(ROOT)/lib/libSCGAL.a
endif
endif

LDIRT = parse.output

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/bisoncpp.mak
include $(MAKEDIR)/lexcpp.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlink.mak

vpath %.cpp $(BASEDIR)/../player
