include $(ROOT)/include/make/comdef.mak

HTTP ?=0
COMPRESS ?=0

ENBFILES =*.enb
ENB0FILES =$(ENXFILES:.enx=0.enb)
NOINSTALL=1
UPDATE_ENB0=1

ifeq ($(HTTP), 1)
PROTOCOL=http
else
PROTOCOL=file
MAKEHTMLPAGEFLAGS =-f
endif

ifeq ($(COMPRESS), 1)
infoFileExt =enc
CMPDIR =cmp
INFFILES =$(ENCFILES)
else
infoFileExt =ens
CMPDIR =std
INFFILES =$(ENSFILES)
endif

######## 1st pass ########
ifeq ($(BASEDIR), .)
TARGET0 = $(X3DFILES)
INSTALLDIR0 =
OBJDIR =$(CMPDIR)_$(PROTOCOL)
DIRT = tmp.wrl
else
######## 2nd pass ########
TARGET0 =$(MAINTESTPAGE)
TARGET1 =$(HTMLFILES)
ifeq ($(COMPRESS), 1)
TARGET2 =$(ENB0FILES)
endif
INSTALLDIR0 =
INSTALLDIR1 =
INSTALLDIR2 =
DIRT = $(INFFILES) $(LOGFILES) $(ENBFILES) $(ENXFILES)
ifndef BASEDIR
DIRT += $(X3DFILES) 
endif
endif

MANUAL_SNAPSHOT ?=0
AUTO_SNAPSHOT ?=0
