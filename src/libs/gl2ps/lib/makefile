BASEDIR =.

include $(ROOT)/make/comdef.mak

BASELIBNAME =gl2ps
BASEINSTALLDIR =$(ROOT)

TARGET0 = $(LIBNAME)
INSTALLDIR0 = $(LIBINSTALLDIR)

ifeq ($(DE), msvc)
TARGET1 = $(BASELIBNAME).lib
INSTALLDIR1 = $(LIBDIR)
endif

CSOURCES = gl2ps.c

LCINCS =-I$(BASEDIR)/../include

ifeq ($(PLATFORM), cygwin)
LCPPOPTS =-GR
endif

ifeq ($(PLATFORM), linux)
LCPPOPTS = -Wall
# LCPPOPTS+= -pedantic
endif

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlib.mak
