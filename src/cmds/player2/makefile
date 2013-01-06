BASEDIR =.
MULTITHREAD =1
USE_SHARED =1

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
LMAKEDEPENDFLAGS+= -DBOOST_TOKENIZER_JRB070303_HPP_
LMAKEDEPENDFLAGS+= -DBOOST_TRANSFORM_ITERATOR_23022003THW_HPP
LMAKEDEPENDFLAGS+= -DBOOST_FILESYSTEM_PATH_HPP
LMAKEDEPENDFLAGS+= -DBOOST_FILESYSTEM_DIRECTORY_HPP
LMAKEDEPENDFLAGS+= -DBOOST_FILESYSTEM_EXCEPTION_HPP
LMAKEDEPENDFLAGS+= -DBOOST_FILESYSTEM_OPERATIONS_HPP
LMAKEDEPENDFLAGS+= -DBOOST_MPL_AND_HPP_INCLUDED
LMAKEDEPENDFLAGS+= -DBOOST_MPL_OR_HPP_INCLUDED
LMAKEDEPENDFLAGS+= -DBOOST_RANDOM_HPP
LMAKEDEPENDFLAGS+= -DBOOST_RANDOM_CONST_MOD_HPP
LMAKEDEPENDFLAGS+= -DPROGRAM_OPTIONS_VP_2003_05_19
LMAKEDEPENDFLAGS+= -DBOOST_RANDOM_LINEAR_CONGRUENTIAL_HPP
LMAKEDEPENDFLAGS+= -DBOOST_RANDOM_UNIFORM_INT_HPP
LMAKEDEPENDFLAGS+= -D_MACHINE__DEFAULT_TYPES_H
endif

CPPSOURCES = player.cpp
CPPSOURCES+= Player_scene.cpp
CPPSOURCES+= Player_option_parser.cpp
CPPSOURCES+= Player2_scene.cpp
CPPSOURCES+= Player2_option_parser.cpp

LCPPINCS = -I$(BASEDIR)
LCPPINCS = -I$(BASEDIR)/../player

# Add BOOST, GMP, MPFR, and Magick include directory:
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

# LCPPINCS+= -I$(CGAL_WORKDIR)/trunk/Arrangement_2/include
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

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
endif

ifeq ($(DE), msvc)
LCPPDEFS+=-DBOOST_ALL_DYN_LINK
LCPPOPTS =-GR
LCPPOPTS+= -vmg
LCPPOPTS+= -W3
ifeq ($(COMPILER_VER), 1400)
LCPPOPTS+= -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1500)
LCPPOPTS+= -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1600)
LCPPOPTS+= -errorReport:prompt
endif
endif
endif

ifdef MAGICK_LIB_DIR
LLDOPTS+= -libpath:"$(MAGICK_LIB_DIR)"
endif
ifdef BOOST_LIB_DIR
LLDOPTS+= -libpath:"$(BOOST_LIB_DIR)"
endif
ifdef GMP_LIB_DIR
LLDOPTS+= -libpath:"$(GMP_LIB_DIR)"
endif
ifdef MPFR_LIB_DIR
LLDOPTS+= -libpath:"$(MPFR_LIB_DIR)"
endif

LLDLIBS+= SCGAL.lib
LLDLIBS+= SGAL.lib gl2ps.lib
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
ifeq ($(COMPILER_VER), 1200)
ifeq ($(DEBUG), 1)
LLDLIBS+= stlport_vc6_stldebug_static.lib
else
LLDLIBS+= stlport_vc6_static.lib
endif
endif
# LLDOPTS+= -nodefaultlib:MSVCRT
ifeq ($(COMPILER_VER), 1200)
ifeq ($(DEBUG), 1)
LLDOPTS+= -nodefaultlib:libcd
LLDOPTS+= -nodefaultlib:libcmtd
else
LLDOPTS+= -nodefaultlib:libc
# LLDOPTS+= -nodefaultlib:libcmt
endif
endif
LIBS = $(ROOT)/lib/SGAL.lib
LIBS+= $(ROOT)/lib/gl2ps.lib
LIBS+= $(ROOT)/lib/SCGAL.lib
endif

ifeq ($(DE), gnu)
LCPPOPTS+= -Wall
# LCPPOPTS+= -pedantic
LLDOPTS = $(CGALLDOPTS)

LLDLIBS = -lSCGAL
LLDLIBS+= $(CGALLDLIBS)
ifeq ($(USE_QUADRIC), 1)
LLDLIBS+= -lQdX -lNiX -lSoX

ifdef EXACUS_LIB_DIR
LLDOPTS+= -L$(EXACUS_LIB_DIR)
LLDOPTS+= -Xlinker -R$(EXACUS_LIB_DIR)
else
ifdef EXACUS_DIR
LLDOPTS+= -L$(EXACUS_DIR)/lib
LLDOPTS+= -Xlinker -R$(EXACUS_DIR)/lib
endif
endif
endif

ifeq ($(USE_GMP), 1)
LLDLIBS+= -lgmp -lmpfr -lgmpxx
endif
LLDLIBS+= -lSGAL -lgl2ps
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

LIBS = $(ROOT)/lib/libSGAL.a
LIBS+= $(ROOT)/lib/libgl2ps.a
LIBS+= $(ROOT)/lib/libSCGAL.a
ifeq ($(USE_GLUT), 1)
LIBS+= $(ROOT)/lib/libSGLUT.a
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
