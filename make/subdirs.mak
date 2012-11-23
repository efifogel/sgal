# defines:
ALL	= $(SUBDIRS:=.all)
INSTALL	= $(SUBDIRS:=.install)
CLEAN	= $(SUBDIRS:=.clean)
CLOBBER	= $(SUBDIRS:=.clobber)

# ALL=transform.all

# rules:
.PHONY	:: default all install clean clobber
default :: all
all	:: $(ALL)
install	:: $(INSTALL)
clean	:: $(CLEAN)
clobber	:: $(CLOBBER)

.PHONY :: $(ALL) $(INSTALL) $(CLEAN) $(CLOBBER) subdirs_force
$(ALL): subdirs_force
	@echo $(ALL) ALL
	@echo Making in $(basename $@) subdirectory...
	@cd $(basename $@) && $(MAKEF) all

$(INSTALL): subdirs_force
	@echo Installing in $(basename $@) subdirectory...
	@cd $(basename $@) && $(MAKEF) install

$(CLEAN): subdirs_force
	@echo Cleaning in $(basename $@) subdirectory...
	@cd $(basename $@) && $(MAKEF) clean

$(CLOBBER): subdirs_force
	@echo Clobbering in $(basename $@) subdirectory...
	@cd $(basename $@) && $(MAKEF) clobber

subdirs_force: ;

# $(PREFIX)all $(PREFIX)install $(PREFIX)clean $(PREFIX)clobber::
# 	@if test -n "$(dirs)"; then $(MAKEF) MAKETARGET=$@ $(dirs); fi

# GMAKEFLAGS+=$(MAKETARGET)
# $(dirs):: force
# 	cd $@ && $(MAKEF)

