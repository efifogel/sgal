include $(ROOT)/make/comdef.mak
SUBDIRS =
SUBDIRS+= boost
ifeq ($(DE), msvc)
SUBDIRS+= flex
SUBDIRS+= GL
endif
SUBDIRS+= SGAL
ifeq ($(USE_GLUT), 1)
SUBDIRS+= SGLUT
endif
ifeq ($(USE_SCGAL), 1)
SUBDIRS+= SCGAL
endif
include $(MAKEDIR)/subdirs.mak
