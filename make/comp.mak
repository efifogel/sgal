ifeq ($(DE), msvc)
%.tlb : %.idl
	$(MTLF) -tlb $@ $<

%.res : %.rc
	@echo Compiling Resources $<
	$(RSCF) -fo$@ $<

%.o : %.asm
	@echo Assembling $<
	$(ASMF) -c -Ta $< -Fo$@

%.o : %.c
	@echo Compiling $<
ifeq ($(DEBUG), 1)
ifeq ($(DEBUG_PDB), 1)
	$(CCF) -c $< -Fd$(basename $@).pdb -Fo$@
else
	$(CCF) -c $< -Fo$@
endif
else
	$(CCF) -c $< -Fo$@
endif

%.o : %.cpp
	@echo Compiling $<
ifeq ($(DEBUG), 1)
ifeq ($(DEBUG_PDB), 1)
	$(CPPF) -c $< -Fd$(basename $@).pdb -Fo$@
else
	$(CPPF) -c $< -Fo$@
endif
else
	$(CPPF) -c $< -Fo$@
endif

%.o : %.C
	@echo Compiling $<
ifeq ($(DEBUG), 1)
ifeq ($(DEBUG_PDB), 1)
	$(CPPF) -c -Tp $< -Fd$(basename $@).pdb -Fo$@
else
	$(CPPF) -c -Tp $< -Fo$@
endif
else
	$(CPPF) -c -Tp $< -Fo$@
endif

else
ifeq ($(DE), gnu)

%.o : %.asm
	@echo Assembling $<
	$(ASMF) -c $< -o $@

%.o : %.c
	@echo Compiling $<
	$(CCF) -c $< -o $@

%.o : %.cpp
	@echo Compiling $<
	$(CPPF) -c $< -o $@

%.o : %.C
	@echo Compiling $<
	$(CPPF) -c $< -o $@

else
ifeq ($(DE), watcom)

%.o : %.asm
	@echo Assembling $<
	$(ASMF) -c -Ta $< -Fo$@

%.o : %.c
	@echo Compiling $<
	$(CCF) $< -fo=$@

%.o : %.cpp
	@echo Compiling $<
	$(CPPF) $< -fo=$@

%.o : %.C
	@echo Compiling $<
	$(CPPF) $< -fo=$@

endif
endif
endif

ifndef BASEDIR_
ifneq ($(NOMAKEDEPEND), 1)
%.d : %.cpp %.o
	@echo Generating dependency for $<
	$(run-makedepend-cpp)

%.d : %.C %.o
	@echo Generating dependency for $<
	$(run-makedepend-cpp)

%.d : %.c %.o
	@echo Generating dependency for $<
	$(run-makedepend-c)

%.d : %.asm %.o
	@echo Generating dependency for $<
	$(run-makedepend-a)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

endif
endif
