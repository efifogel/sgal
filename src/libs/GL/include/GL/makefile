include $(ROOT)/make/comdef.mak
FILES =
FILES+= glext.h
FILES+= glut.h
ifeq ($(DE), msvc)
FILES+= wglext.h
else
FILES+= glx.h
endif
INSTALLDIR =$(INCDIR)/GL
include $(MAKEDIR)/files.mak
