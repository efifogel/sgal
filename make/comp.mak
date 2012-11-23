MAKEDEPEND_CPPINCS=$(CPPINCS)
MAKEDEPEND_CINCS=$(CINCS)
MAKEDEPEND_ASMINCS=$(ASMINCS)

ifeq ($(DE), msvc)
# This is a workaround for make version 3.81.
# It appears that this version of make rejects, when the charcter ':'
# appears in the path, as in '-Ic:/x/y/include'.
TMP_CPPINCS1=$(patsubst '-I%','%',$(CPPINCS))
TMP_CPPINCS2=$(patsubst -I%,%,$(TMP_CPPINCS1))
TMP_CPPINCS3=$(shell cygpath $(TMP_CPPINCS2))
MAKEDEPEND_CPPINCS=$(patsubst %,-I%,$(TMP_CPPINCS3))

TMP_CINCS1=$(patsubst '-I%','%',$(CINCS))
TMP_CINCS2=$(patsubst -I%,%,$(TMP_CINCS1))
TMP_CINCS3=$(shell cygpath $(TMP_CINCS2))
MAKEDEPEND_CINCS=$(patsubst %,-I%,$(TMP_CINCS3))

TMP_ASMINCS1=$(patsubst '-I%','%',$(ASMINCS))
TMP_ASMINCS2=$(patsubst -I%,%,$(TMP_ASMINCS1))
TMP_ASMINCS3=$(shell cygpath $(TMP_ASMINCS2))
MAKEDEPEND_ASMINCS=$(patsubst %,-I%,$(TMP_ASMINCS3))

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

output_operatior=-o
# Due to a bug in gcc 3.0.3
# The workaround will work only if $GCC_VER is present! 
ifeq ($(GCC_VER),3.0.3)
output_operatior=>
endif
%.d : %.cpp %.o
	@echo Generating dependency for $<
	$(MAKEDEPENDF) $(MAKEDEPEND_CPPINCS) $(CPPDEFS) $< $(output_operatior) $@

%.d : %.C %.o
	@echo Generating dependency for $<
	$(MAKEDEPENDF) $(MAKEDEPEND_CPPINCS) $(CPPDEFS) $< $(output_operatior) $@

%.d : %.c %.o
	@echo Generating dependency for $<
	$(MAKEDEPENDF) $(MAKEDEPEND_CINCS) $(CDEFS) $< $(output_operatior) $@

%.d : %.asm %.o
	@echo Generating dependency for $<
	$(MAKEDEPENDF) $(MAKEDEPEND_ASMINCS) $(ASMDEFS) $< $(output_operatior) $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

endif
endif
