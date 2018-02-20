include $(MAKEDIR)/comrul.mak
######## 1st pass ########
ifeq ($(BASEDIR), .)

.PHONY :: default all install clean

default:: all
all install clean:: $(BUILDDIR) $(BUILDDIR)/$(MAKEFILENAME) force
	@cd $(BUILDDIR) && $(MAKEF) $@

clobber::
	@rm -rf $(BUILDDIR)

%.x3d : %.wrl $(WRL2X3D)
	cat $< > tmp.wrl
ifeq ($(MANUAL_SNAPSHOT), 1)
	echo 'DEF snap enbSnapshot { filename "$(RELDIR)/$(basename $<)" }' >> tmp.wrl
	echo 'DEF snap-KEY enbSingleKeySensor { keyCode "115" }' >> tmp.wrl
	echo 'ROUTE snap-KEY.release TO snap.trigger' >> tmp.wrl
endif
	$(WRL2X3DF) tmp.wrl $@

else
######## 2nd pass ########

include $(MAKEDIR)/conv.mak

$(TARGET0) :: Makefile.mak $(GENMAINHTMLSCRIPT)
	$(GENMAINHTMLF) $@ $(HTMLFILES)

.SECONDARY: $(ENCFILES) $(ENSFILES) $(X3DFILES)

vpath %.x3d $(BASEDIR)

endif # BASEDIR
