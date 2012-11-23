BASEDIR =.

include $(ROOT)/make/comdef.mak

BASETARGET =gl2ps
BASEINSTALLDIR =$(ROOT)

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
