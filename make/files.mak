.PHONY :: $(PREFIX)default $(PREFIX)clean $(PREFIX)clobber $(PREFIX)install $(PREFIX)all

ifndef SUFFIX
ifdef FILES
SUFFIX =files
endif

ifdef FILTER
SUFFIX =exist
endif
endif

ifndef FILTER
FILTER =*.h
endif

GDIRT = $(FILESINSTFILE)

$(PREFIX)default:: $(PREFIX)all
$(PREFIX)clean::

$(PREFIX)clobber::
	@rm -rf $(DIRT)

$(PREFIX)install:: $(PREFIX)install_files_$(SUFFIX)
$(PREFIX)install_files_files:: $(PREFIX)default $(FILESINSTFILE)

$(FILESINSTFILE):: $(FILES) $(FORCEINST)
ifneq ($(NOINSTALL), 1)
ifdef FILES
ifdef INSTALLDIR
	$(INST) -d $(INSTALLDIR)
	$(INST) $(FILES) $(INSTALLDIR)
	@touch $(FILESINSTFILE)
endif
endif
endif

$(PREFIX)install_files_exist::
	$(INST) -d $(INSTALLDIR)
	$(INST) $(FILTER) $(INSTALLDIR)

$(PREFIX)force:: ;

$(PREFIX)all:: ;
