BASEDIR =.
MULTITHREAD =1
USE_SHARED =1

include $(ROOT)/make/comdef.mak

BASENAME = knot
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
LMAKEDEPENDFLAGS+= -DBOOST_MPL_AND_HPP_INCLUDED
LMAKEDEPENDFLAGS+= -DBOOST_MPL_OR_HPP_INCLUDED
endif

CPPSOURCES = knot.cpp
CPPSOURCES+= Knot_scene.cpp
CPPSOURCES+= Knot_option_parser.cpp

LCPPINCS = -I$(BASEDIR)

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
endif

ifeq ($(DE), msvc)
LCPPOPTS =-GR
LCPPOPTS+= -vmg
LLDLIBS+= $(LDSYSLIBS)
LLDLIBS+= SGAL.lib gl2ps.lib
LLDLIBS+= glu32.lib opengl32.lib
ifeq ($(COMPILER_VER), 1200)
ifeq ($(DEBUG), 1)
LLDLIBS+= stlport_vc6_stldebug_static.lib
else
LLDLIBS+= stlport_vc6_static.lib
endif
endif
ifeq ($(DEBUG), 1)
ifeq ($(COMPILER_VER), 1200)
LLDOPTS+= -nodefaultlib:libcd
LLDOPTS+= -nodefaultlib:libcmtd
endif
ifeq ($(COMPILER_VER), 1310)
LLDOPTS+= -nodefaultlib:libc
LLDOPTS+= -nodefaultlib:msvcrt
endif
else
endif
LIBS+= $(LIBDIR)/gl2ps.lib
LIBS+= $(LIBDIR)/SGAL.lib
endif

ifeq ($(DE), gnu)
LCPPOPTS = -Wall
# LCPPOPTS+= -pedantic
LLDOPTS+= -L/usr/X11R6/lib
LLDLIBS+= -lSGAL -lgl2ps
LLDLIBS+= -lboost_program_options$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_filesystem$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_system$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lXxf86vm
LLDLIBS+= -lGLU -lGL
LLDLIBS+= -lXi
# LLDLIBS+= -lXmu

LIBS = $(LIBDIR)/libSGAL.a
LIBS+= $(LIBDIR)/libgl2ps.a
endif

LDIRT = parse.output

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/bisoncpp.mak
include $(MAKEDIR)/lexcpp.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlink.mak
