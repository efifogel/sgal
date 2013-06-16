include $(ROOT)/make/comdef.mak
SUBDIRS =
SUBDIRS+= boost
ifeq ($(DE), msvc)
ifeq ($(COMPILER_VER), 1200)
# SUBDIRS+= STLport
endif
# SUBDIRS+= getopt
SUBDIRS+= flex
SUBDIRS+= GL
SUBDIRS+= gl2ps
endif
SUBDIRS+= SGAL
ifeq ($(USE_GLUT), 1)
SUBDIRS+= SGLUT
endif
ifeq ($(USE_CGAL), 1)
SUBDIRS+= SCGAL
endif
ifeq ($(USE_EGO), 1)
SUBDIRS+= SEGO
endif
include $(MAKEDIR)/subdirs.mak
