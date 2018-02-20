ifeq ($(DE), msvc)

ifneq "$(DEFSOURCE)" ""
$(TARGET0) :: $(DEFSOURCE) $(OBJECTS) $(LIBS)
	@echo Linking $@
ifeq ($(DEBUG_PDB), 1)
	$(LINKF) -def:$(filter %.def,$^) $(OBJECTS) -pdb:$(basename $@).pdb -out:$@
else
	$(LINKF) -def:$(filter %.def,$^) $(OBJECTS) -out:$@
endif
else
$(TARGET0) :: $(OBJECTS) $(LIBS)
	@echo Linking $@
ifeq ($(DEBUG_PDB), 1)
	$(LINKF) $(OBJECTS) -pdb:$(basename $@).pdb -Out:$@
else
	$(LINKF) $(OBJECTS) -Out:$@
endif
endif
else
ifeq ($(DE), gnu)
# When using shared libraries, $(TARGET0) shouldn't depend on $(LIBS)...
# I'm not changing this yet (chickened).
$(TARGET0) :: $(OBJECTS) $(LIBS)
	@echo Linking $@
	$(LINK) $(OBJECTS) $(LDFLAGS) -o $@
else
ifeq ($(DE), watcom)
$(TARGET0) :: $(OBJECTS) $(LIBS)
	@echo Linking $@
	@%create tmp.lbc
	@for %i in ($(OBJECTS)) do $(%SILENT)%append tmp.lbc file %i
	$(LINKF) name $@ @tmp.lbc
endif
endif
endif
