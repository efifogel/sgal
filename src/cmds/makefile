include $(ROOT)/make/comdef.mak
SUBDIRS = player knot
ifeq ($(USE_CGAL), 1)
ifeq ($(USE_CGM), 1)
SUBDIRS+= player2
endif
endif
include $(MAKEDIR)/subdirs.mak
