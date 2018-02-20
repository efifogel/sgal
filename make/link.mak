ifeq ($(DE), msvc)
%.exe	: %.o
	@echo
	@echo Linking $@
ifeq ($(DEBUG), 1)
ifeq ($(DEBUG_PDB), 1)
	$(LINKF) $< -pdb:$(basename $@).pdb -Out:$@
else
	$(LINKF) $< -Out:$@
endif
else
	$(LINKF) $< -Out:$@
endif

%.bsc	: %.sbr
	$(BSCMAKE) -o $@ $<
else
ifeq ($(DE), gnu)
.o::
	@echo
	@echo Linking $@
	$(LINKF) $< -o $@
else
.o.exe::
	@echo
	@echo Linking $@
	$(LINKF) name $@ file $<
endif
endif
