BASEDIR =.
MULTITHREAD =1
USE_SHARED =1
USE_SHARED_SGAL =1
USE_DYNAMIC_SGAL =0

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
endif

CPPSOURCES = knot.cpp
CPPSOURCES+= Knot_scene.cpp
CPPSOURCES+= Knot_option_parser.cpp

LCPPDEFS =
LCPPINCS =
LCPPOPTS =

LLDLIBS =
LLDOPTS =

LCPPINCS+= -I$(BASEDIR)

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

ifeq ($(DE), msvc)
LCPPDEFS+= -DBOOST_ALL_DYN_LINK
LCPPOPTS+= -GR  # Enables run-time type information (RTTI).
LCPPOPTS+= -vmg # Uses full generality for pointers to members.
LCPPOPTS+= -GS  # Apply buffers security check.
LCPPOPTS+= -Oy- # Disable Omission of frame pointer (x86 only).
LCPPOPTS+= -WX  # Treat all compiler warnings as errors
LCPPOPTS+= -Zc:wchar_t -Zc:forScope # Specify standard behavior
# CPPWARNINGOPTS = -W4  # Specify the level of warning; 0 to 4

ifeq ($(DEBUG), 0)
LCPPOPTS+= -Gy  # Enable function-level linking
LCPPOPTS+= -Oi  # Generate intrinsic functions
endif

# Add BOOST, GL2PS, and Magick lib directories:
ifdef BOOST_LIB_DIR
LLDOPTS+= -libpath:"$(BOOST_LIB_DIR)"
endif
ifdef MAGICK_LIB_DIR
LLDOPTS+= -libpath:"$(MAGICK_LIB_DIR)"
endif
ifdef GL2PS_LIB_DIR
LLDOPTS+= -libpath:"$(GL2PS_LIB_DIR)"
endif
ifdef V8_LIB_DIR
LLDOPTS+= -libpath:"$(V8_LIB_DIR)"
endif
ifdef ICU_LIB_DIR
LLDOPTS+= -libpath:"$(ICU_LIB_DIR)"
endif

ifneq ($(USE_DYNAMIC_SGAL), 1)
LLDLIBS+= SGAL.lib
endif

# LLDLIBS+= gl2ps.lib
LLDLIBS+= glu32.lib opengl32.lib
LLDLIBS+= $(LDSYSLIBS)

LIBS =
ifneq ($(USE_DYNAMIC_SGAL), 1)
LIBS+= $(LIBDIR)/SGAL.lib
endif

endif # msvc

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS+= -Wall
# LCPPOPTS+= -Werror
# LCPPOPTS+= -pedantic
LLDOPTS+= -rdynamic -ldl
ifdef V8_LIB_DIR
LLDOPTS+= -L$(V8_LIB_DIR)
endif
ifdef ICU_LIB_DIR
LLDOPTS+= -L$(ICU_LIB_DIR)
endif

ifneq ($(USE_DYNAMIC_SGAL), 1)
LLDLIBS+= -lSGAL
endif

# V8 engine stuff:
LLDLIBS+= -lv8_base.x64
LLDLIBS+= -lv8_nosnapshot.x64
LLDLIBS+= -licui18n
LLDLIBS+= -licuuc
LLDLIBS+= -licudata
LLDLIBS+= -lrt

LLDLIBS+= -lgl2ps
LLDLIBS+= -lMagick++
LLDLIBS+= -lboost_program_options$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_filesystem$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lboost_system$(BOOST_LIBNAME_SUFFIX)
LLDLIBS+= -lXxf86vm
LLDLIBS+= -lGLU -lGL
LLDLIBS+= -lXi
# LLDLIBS+= -lXmu

LIBS =
ifneq ($(USE_SHARED_SGAL), 1)
LIBS+= $(LIBDIR)/libSGAL.a
endif
endif

LDIRT = parse.output

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/bisoncpp.mak
include $(MAKEDIR)/lexcpp.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlink.mak
