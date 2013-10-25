PSVIEWF =gv

# CROSS = i686-pc-linux-gnu-
CPP = $(CROSS)g++
ASM = $(CROSS)as
CC  = $(CROSS)gcc

COMPILER_STR := $(shell g++ --version)
COMPILER_RE = g++ (.*) \([0-9]\.[0-9]\.[0-9]\)
COMPILER_VER_CMD = expr "$(COMPILER_STR)" : "$(COMPILER_RE)"
COMPILER_VER ?= $(shell $(COMPILER_VER_CMD))

ifeq ($(DEF_SHARED), 1)
# The implicit assumption is that these parameters are always used as
# $(LIBS) OUTPUT OBJECTS. It is used like that in targlink.mak, and in a few
# local makefiles.
LIBPROG = $(CROSS)g++
GLIBFLAGS = -shared -o
else
LIBPROG = $(CROSS)ar
GLIBFLAGS = cruv
endif
LINK = $(CROSS)g++

# A bug concerning run-time type information created for classes included and
# not used. Currently fixed by specifying -fno-rtti.
GCC_LINK_BUG = 0
# A bug concerning \'s in cpp's output (due to file names with \) which are
# misunderstood by cc1plus.
GCC_BACKSLASH_BUG = 1

# COMP_OS_DEF =-D__LINUX__

#-------------------------------------------------
# ---- C includes, definitions and options ------
#-------------------------------------------------
GCINCS =
GCDEFS = $(COMP_OS_DEF)
# GCDEFS+= -D_REENTRANT
GCOPTS =
GCINCS = -I$(INCDIR)

#-------------------------------------------------
# ---- C++ includes, definitions and options ------
#-------------------------------------------------
GCPPINCS = -I$(INCDIR)

# If BOOST_INC_DIR is defined, add it to the list of include directories
# Otherwise, it is assumed that boost is installed in the standard place
ifdef BOOST_INC_DIR
GCPPINCS+= -I$(BOOST_INC_DIR)
endif

GCPPDEFS = $(COMP_OS_DEF)
# GCPPDEFS+= -D_REENTRANT
# GCPPOPTS =-std=c++0x

# GCOPTS+= -D__STL_HAS_NO_NAMESPACES
# GCPPOPTS+= -D__STL_HAS_NO_NAMESPACES

ifeq ($(DEBUG), 1)
GCOPTS += -g
GCPPOPTS+= -g
else #debug
GCOPTS+= -O # -fomit-frame-pointer
GCPPOPTS+= -O2
endif

ifeq ($(PROFILE), 1)
GCPPOPTS+= -pg
endif

ifeq ($(GCC_LINK_BUG), 1)
GCPPOPTS += -fno-rtti
endif

ifeq ($(DEF_SHARED), 1)
GCOPTS += -fPIC
GCPPOPTS += -fPIC
endif

GLIBPATHOPTS = -L$(LIBDIR)

GLDOPTS = $(GLIBPATHOPTS) -lstdc++

# If BOOST_LIB_DIR is defined, add it to the list of lib directories
# Otherwise, it is assumed that boost is installed in the standard place
ifdef BOOST_LIB_DIR
GLDOPTS+= -L$(BOOST_LIB_DIR) -Wl,-rpath $(BOOST_LIB_DIR)
endif

ifeq ($(PROFILE), 1)
GLDOPTS+= -pg
endif

LIBFILEPREFIX = lib
ifeq ($(DEF_SHARED), 1)
LIBFILESUFFIX = .so
else
LIBFILESUFFIX = .a
endif
LIBLINKPREFIX = -l
LIBLINKSUFFIX =
LIBDIRPREFIX = -L

GASMFLAGS =

EXEFILESUFFIX =

ifeq ($(DEF_SHARED), 1)
RELINSTALLDIR =lib
else
RELINSTALLDIR =lib
endif

# suppress warning (pragma once)
# GMAKEDEPENDFLAGS = -w
# GMAKEDEPENDFLAGS+= -nostdinc -undef

# TBD: should be one of the following:
#
# Key Feature
# --- -------
# s   Static link to runtime.
# g   Debug runtime.
# y   Debug Python system.
# d   Debug enabled code.
# p   STLport runtime, instead of the vendor toolset runtime.
# n   STLport runtime using the "native" IO streams instead of the STLport IO str
DE_GOBJDIR =

MAKEDEPEND =gcc
# Consider only user header files included with `#include file"'.
GMAKEDEPENDFLAGS+= -MM
# Treat missing header files as generated files:
GMAKEDEPENDFLAGS+= -MG

output_operatior=-o
# Due to a bug in gcc 3.0.3
# The workaround will work only if $GCC_VER is present!
ifeq ($(GCC_VER),3.0.3)
output_operatior=>
endif
define run-makedepend-cpp
$(MAKEDEPENDF) $(MAKEDEPEND_CPPINCS) $(MAKEDEPEND_CPPDEFS) $< $(output_operatior) $@
endef

define run-makedepend-c
$(MAKEDEPENDF) $(MAKEDEPEND_CINCS) $(MAKEDEPEND_CDEFS) $< $(output_operatior) $@
endef

define run-makedepend-a
$(MAKEDEPENDF) $(MAKEDEPEND_ASMINCS) $(MAKEDEPEND_ASMDEFS) $< $(output_operatior) $@
endef
