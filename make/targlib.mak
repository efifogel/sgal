ifeq ($(DE), msvc)
ifneq "$(DEFSOURCE)" ""
$(TARGET0) :: %.lib : $(DEFSOURCE) $(OBJECTS)
	@echo Creating library $@ ...
	$(LIBF) -def:$(filter %.def,$^) -out:$@ $(OBJECTS)
else
$(TARGET0) :: %.lib : $(OBJECTS)
	@echo Creating library $@ ...
	$(LIBF) -out:$@ $(OBJECTS)
endif
else
ifeq ($(DE), gnu)
$(TARGET0) :: $(OBJECTS)
	@echo Creating library $@ ...
# Currently, I don't bother about versioning, i.e. the files are .so, not .so.1.0
	$(LIBF) $@ $(OBJECTS)
else
$(TARGET0) :: $(OBJECTS)
	@echo Updating library $@ ...
	@%create tmp.lbc
	@for %i in ($?) do $(%SILENT)%append tmp.lbc -+%i
	$(LIBF) $@ @tmp.lbc
endif
endif
