ifeq ($(STAGE), 2)

$(PREFIX)default $(PREFIX)install:: $(STAGE2FILE) $(STAGE2EXTRAFILES)
	$(MAKEF) STAGE=1 $@

else
ifeq ($(STAGE), 1)

$(PREFIX)default $(PREFIX)install:: $(STAGE1FILE) $(STAGE1EXTRAFILES)
	$(MAKEF) STAGE=0 $@

ifdef STAGE2FILE
include $(STAGE2FILE)
endif

# The ifeq STAGE 0 shouldn't be necessary, but is. Why?
else
ifeq ($(STAGE), 0)

ifdef STAGE2FILE
include $(STAGE2FILE)
endif

ifdef STAGE1FILE
include $(STAGE1FILE)
endif

.PHONY :: $(PREFIX)default $(PREFIX)clean $(PREFIX)clobber $(PREFIX)install \
$(PREFIX)lint $(PREFIX)tags $(PREFIX)force $(PREFIX)all

$(PREFIX)default:: $(PREFIX)all
$(PREFIX)all:: $(TARGET0) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7)
$(PREFIX)install:: $(PREFIX)default $(INSTFILE)
$(INSTFILE):: $(TARGET0) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(FORCEINST)
ifneq ($(NOINSTALL), 1)
	$(INST) -d $(INSTALLDIR0)
	$(INST) $(TARGET0) $(INSTALLDIR0)
ifdef TARGET1
ifdef INSTALLDIR1
	$(INST) -d $(INSTALLDIR1)
	$(INST) $(TARGET1) $(INSTALLDIR1)
endif
endif
ifdef TARGET2
ifdef INSTALLDIR2
	$(INST) -d $(INSTALLDIR2)
	$(INST) $(TARGET2) $(INSTALLDIR2)
endif
endif
ifdef TARGET3
ifdef INSTALLDIR3
	$(INST) -d $(INSTALLDIR3)
	$(INST) $(TARGET3) $(INSTALLDIR3)
endif
endif
ifdef TARGET4
ifdef INSTALLDIR4
	$(INST) -d $(INSTALLDIR4)
	$(INST) $(TARGET4) $(INSTALLDIR4)
endif
endif
ifdef TARGET5
ifdef INSTALLDIR5
	$(INST) -d $(INSTALLDIR5)
	$(INST) $(TARGET5) $(INSTALLDIR5)
endif
endif
ifdef TARGET6
ifdef INSTALLDIR6
	$(INST) -d $(INSTALLDIR6)
	$(INST) $(TARGET6) $(INSTALLDIR6)
endif
endif
ifdef TARGET7
ifdef INSTALLDIR7
	$(INST) -d $(INSTALLDIR7)
	$(INST) $(TARGET7) $(INSTALLDIR7)
endif
endif
	@touch $(INSTFILE)
endif

endif
endif
endif

$(PREFIX)lint: $(CSRCS)
	$(LINT) -ux $(CSRCS)

$(PREFIX)tags: $(CSRCS)
	ctags $(CSRCS)

$(PREFIX)clean::
ifdef DIRT
	rm -rf $(DIRT)
endif

$(PREFIX)clobber:: $(PREFIX)clean
	rm -rf $(TARGET0) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(INSTFILE)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(MAKEFILENAME): $(MAKEFILENAME)
	@sed -e s/BASEDIR\ *=./BASEDIR=../ $(MAKEFILENAME) > $(BUILDDIR)/$(MAKEFILENAME)

force:: ;

.DELETE_ON_ERROR:
