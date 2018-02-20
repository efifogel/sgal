include $(ROOT)/make/comdef.mak

FILES = tetrahedron.wrl
FILES+= cube.wrl
FILES+= octahedron.wrl
FILES+= dodecahedron.wrl
FILES+= icosahedron.wrl

INSTALLDIR =$(DATADIR)/vrml
include $(MAKEDIR)/files.mak

SUBDIRS = cgm
SUBDIRS+= ngm
SUBDIRS+= nef
SUBDIRS+= ch
SUBDIRS+= tri
SUBDIRS+= pol

include $(MAKEDIR)/subdirs.mak
