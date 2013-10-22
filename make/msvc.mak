PSVIEWF =$(GSVIEWDIR)/gsview32.exe
ASM  =ml.exe
ifneq ($(NUMEGA), 0)
CC   =$(NMDIR)/nmcl.exe
CPP  =$(NMDIR)/nmcl.exe
LINK =$(NMDIR)/nmlink.exe
else
CC   =cl.exe
CPP  =cl.exe
# cygwin come with a link.exe executable!
# LINK =link.exe
ifdef MSVCDIR
LINK ="$(MSVCDIR)/bin/link.exe"
else
ifdef VCINSTALLDIR
LINK ="$(VCINSTALLDIR)/bin/link.exe"
endif
endif
endif

COMPILER_STR :=$(shell cl 2>&1)
COMPILER_RE =Microsoft.*Version \([0-9][0-9].[0-9][0-9]\).*
COMPILER_VER_CMD =expr '$(COMPILER_STR)' : '$(COMPILER_RE)'
COMPILER_VER_LONG ?=$(shell $(COMPILER_VER_CMD))
COMPILER_VER=$(subst .,,$(COMPILER_VER_LONG))

LIBPROG =lib.exe
RSC =rc.exe
MTL =midl.exe
BSC32 =bscmake.exe
MT =mt.exe

GLIBFLAGS =-nologo
GASMFLAGS =-nologo
GMTFLAGS =-nologo

STLPORT_DIR = '$(MSVCDIR)/Include/stlport'
CSBROPT =-FR
GCINCS =
GCPPINCS =
ifeq ($(COMPILER_VER), 1200)
GCPPINCS+= -I$(STLPORT_DIR)
endif

# Define Platform SDK directory:
ifndef PLATFORM_SDK_DIR
PLATFORM_SDK_DIR = $(VCINSTALLDIR)/PlatformSDK
endif

# Define GMP directories:
ifdef GMP_DIR
ifndef GMP_INC_DIR
GMP_INC_DIR=$(GMP_DIR)/include
endif
ifndef GMP_LIB_DIR
GMP_LIB_DIR=$(GMP_DIR)/lib
endif
endif

STD_ROOT = $(shell cygpath -m $(ROOT))
GCINCS = -I$(STD_ROOT)/include
GCPPINCS+= -I$(STD_ROOT)/include

UNICODE_DEFS = -D_MBCS
# GCDEFS =-D_WIN32 -DWIN32
# GCPPDEFS =-D_WIN32 -DWIN32
GCDEFS += $(UNICODE_DEFS)
GCPPDEFS += $(UNICODE_DEFS)

GLDOPTS = -nologo

ifeq ($(WS), windows)
GCDEFS += -D_WINDOWS
GCPPDEFS += -D_WINDOWS
GLDOPTS+= -subsystem:windows
else
GCDEFS += -D_CONSOLE
GCPPDEFS += -D_CONSOLE
GLDOPTS+= -subsystem:console
endif

# Do not use secure functions, e.g., strcpy_s instead strcpy. The secure
# functions verify that the destination array is sufficiently large.
ifeq ($(COMPILER_VER), 1400)
GCDEFS += -D_CRT_SECURE_NO_DEPRECATE
GCPPDEFS += -D_CRT_SECURE_NO_DEPRECATE
GCDEFS += -D_SCL_SECURE_NO_DEPRECATE
GCPPDEFS += -D_SCL_SECURE_NO_DEPRECATE
else
ifeq ($(COMPILER_VER), 1500)
GCDEFS += -D_CRT_SECURE_NO_DEPRECATE
GCPPDEFS += -D_CRT_SECURE_NO_DEPRECATE
else
ifeq ($(COMPILER_VER), 1600)
GCDEFS += -D_CRT_SECURE_NO_DEPRECATE
GCPPDEFS += -D_CRT_SECURE_NO_DEPRECATE
else
ifeq ($(COMPILER_VER), 1700)
GCDEFS += -D_CRT_SECURE_NO_DEPRECATE
GCPPDEFS += -D_CRT_SECURE_NO_DEPRECATE
endif
endif
endif
endif

GENVELOPEFLAGS =-P$(BINDIR)/

GCDEBUGOPTS =
GCPPDEBUGOPTS =
GCOPTOPTS =-O2 -Ox     # optimize for maximal speed
GCPPOPTOPTS =-O2 -Ox   # optimize for maximal speed
GLDDEBUGOPTS = -release

# The single-threaded CRT (libc.lib, libcd.lib) (formerly the /ML or /MLd
# options) is no longer available in 1400 (VC8) and 1500 (VC9).
# Instead, use the multithreaded CRT.
ifeq ($(COMPILER_VER), 1400)
MULTITHREAD =1
else
ifeq ($(COMPILER_VER), 1500)
MULTITHREAD =1
endif
endif

ifeq ($(MULTITHREAD), 1)
ifeq ($(USE_SHARED), 1)
ifeq ($(DEBUG), 1)
GCMDOPTS = -MDd
GCPPMDOPTS+= -MDd
else
GCMDOPTS = -MD
GCPPMDOPTS+= -MD
endif
else
ifeq ($(DEBUG), 1)
GCMDOPTS = -MTd
GCPPMDOPTS+= -MTd
else
GCMDOPTS = -MT
GCPPMDOPTS+= -MT
endif
endif
else
ifeq ($(USE_SHARED), 1)
ifeq ($(DEBUG), 1)
GCMDOPTS = -LDd
GCPPMDOPTS+= -LDd
else
GCMDOPTS = -LD
GCPPMDOPTS+= -LD
endif
else
ifeq ($(DEBUG), 1)

ifeq ($(COMPILER_VER), 1400)
else
ifeq ($(COMPILER_VER), 1500)
else
ifeq ($(COMPILER_VER), 1600)
else
ifeq ($(COMPILER_VER), 1700)
else
GCMDOPTS = -MLd
GCPPMDOPTS+= -MLd
endif
endif
endif
endif

else
ifeq ($(COMPILER_VER), 1400)
else
ifeq ($(COMPILER_VER), 1500)
else
ifeq ($(COMPILER_VER), 1600)
else
ifeq ($(COMPILER_VER), 1700)
else
GCMDOPTS = -ML
GCPPMDOPTS+= -ML
endif
endif
endif
endif
endif
endif
endif

ifeq ($(DEBUG), 1)
DEBUG_PDB =1
endif

ifndef DEBUG_PDB
DEBUG_PDB =0
endif

ifeq ($(DEBUG), 1)
GCDEBUGOPTS =-Gm -ZI
GCPPDEBUGOPTS =-Gm -ZI

ifeq ($(COMPILER_VER), 1400)
GCDEBUGOPTS+= -RTC1
GCPPDEBUGOPTS+= -RTC1
else
ifeq ($(COMPILER_VER), 1500)
GCDEBUGOPTS+= -RTC1
GCPPDEBUGOPTS+= -RTC1
else
ifeq ($(COMPILER_VER), 1600)
GCDEBUGOPTS+= -RTC1
GCPPDEBUGOPTS+= -RTC1
else
ifeq ($(COMPILER_VER), 1700)
GCDEBUGOPTS+= -RTC1
GCPPDEBUGOPTS+= -RTC1
else
GCDEBUGOPTS+= -GZ
GCPPDEBUGOPTS+= -GZ
endif
endif
endif
endif

GCOPTOPTS =-Od
GCPPOPTOPTS =-Od
GLDDEBUGOPTS = -debug

ifeq ($(COMPILER_VER), 1400)
GLDDEBUGOPTS+= -incremental:yes
else
ifeq ($(COMPILER_VER), 1500)
GLDDEBUGOPTS+= -incremental:yes
else
GLDDEBUGOPTS+= -incremental
endif
endif

ifeq ($(COMPILER_VER), 1200)
ifeq ($(DEBUG_PDB), 1)
GLDDEBUGOPTS+= -pdbtype:sept
endif
endif

# enable old-style debug info
# GCDEBUGOPTS += -Z7
# GCPPDEBUGOPTS += -Z7

endif

#GCOPTS += -YX          # automates precompiled headers
#GCPPOPTS += -YX        # automates precompiled headers
#GLDOPTS+= -PDBTYPE:SEPT
GCOPTS =-nologo
# GCOPTS+= -W3
GCOPTS+= -FD # 1400 => ???
GCOPTS+= $(GCOPTOPTS)
GCOPTS+= $(GCMDOPTS)
GCOPTS+= $(GCDEBUGOPTS)
GCPPOPTS =-nologo
CPPWARNINGOPTS+= -W3

# error prompting:
ifeq ($(COMPILER_VER), 1400)
GERRORPROMP = -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1500)
GERRORPROMP = -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1600)
GERRORPROMP = -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1700)
GERRORPROMP = -errorReport:prompt
endif
endif
endif
endif

GCERRORPROMP = $(GERRORPROMP)
GCPPERRORPROMP = $(GERRORPROMP)
GLDERRORPROMP = $(GERRORPROMP)

# Code Generation:
# GCPPCODEOPTS = -GR
# enable C++ EH
ifeq ($(COMPILER_VER), 1400)
GCPPCODEOPTS+= -EHsc
else
ifeq ($(COMPILER_VER), 1500)
GCPPCODEOPTS+= -EHsc
else
ifeq ($(COMPILER_VER), 1600)
GCPPCODEOPTS+= -EHsc
else
ifeq ($(COMPILER_VER), 1700)
GCPPCODEOPTS+= -EHsc
else
GCPPCODEOPTS+= -GX
endif
endif
endif
endif

GCPPOPTS+= -FD
GCPPOPTS+= $(GCPPCODEOPTS)
GCPPOPTS+= $(GCPPOPTOPTS)
GCPPOPTS+= $(GCPPMDOPTS)
GCPPOPTS+= $(GCPPDEBUGOPTS)
GCPPOPTS+= $(GCPPERRORPROMP)

#GCOPTS += -Gf           # eliminate duplicate strings
#GCPPOPTS += -Gf         # eliminate duplicate strings

EXEFILESUFFIX = .exe

LIBFILEPREFIX =
ifeq ($(DEF_SHARED), 1)
LIBFILESUFFIX = .dll
else
LIBFILESUFFIX = .lib
endif
LIBLINKPREFIX =
LIBLINKSUFFIX = .lib
LIBDIRPREFIX = -libpath:
# no need for a LIBDIRSUFFIX currently.

GLIBPATHOPTS = -libpath:$(STD_ROOT)/lib
ifdef BOOST_LIB_DIR
GLIBPATHOPTS+= -libpath:"$(BOOST_LIB_DIR)"
endif

LDDLLLIBS =
LDDLLOPTS = -dll

GLDOPTS+= -machine:I386
GLDOPTS+= $(GLIBPATHOPTS)
GLDOPTS+= -map
GLDOPTS+= $(GLDDEBUGOPTS)
GLDOPTS+= $(GLDERRORPROMP)
GLDLIBS =
LDSYSLIBS+= kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib
LDSYSLIBS+= advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib
LDSYSLIBS+= odbc32.lib odbccp32.lib

ifeq ($(DEF_SHARED), 1) # Dos/Windows DLLs
RELINSTALLDIR =bin
GLDOPTS += $(LDDLLOPTS)
else
RELINSTALLDIR =lib
endif

GRSCFLAGS = -l0x409 $(DEBUGDEFS)
RSCFLAGS = $(GRSCFLAGS) $(LRSCFLAGS)
RSCF =$(RSC) $(RSCFLAGS)

MTLFLAGS =$(GMTLFLAGS) $(LMTLFLAGS)
MTLF =$(MTL) $(MTLFLAGS)

BSC32FLAGS =$(GBSC32FLAGS) $(LBSC32FLAGS)
BSC32F =$(BSC32) $(BSC32FLAGS)

MTFLAGS =$(GMTFLAGS) $(LMTFLAGS)
MTF =$(MT) $(MTFLAGS)

GDIRT+= *.pdb *.map

ifeq ($(MULTITHREAD), 1)
ifeq ($(USE_SHARED), 1)
ifeq ($(DEBUG), 1)
GLDOPTS+= -nodefaultlib:libcmtd.lib
GLDOPTS+= -nodefaultlib:libcpmtd.lib
GLDOPTS+= -nodefaultlib:libcd.lib
else
# GLDOPTS+= -nodefaultlib:libcmt.lib
# GLDOPTS+= -nodefaultlib:libcpmt.lib
# GLDOPTS+= -nodefaultlib:libc.lib
endif
else
ifeq ($(DEBUG), 1)
GLDOPTS+= -nodefaultlib:msvcrtd.lib
else
# GLDOPTS+= -nodefaultlib:msvcrt.lib
endif
endif
else
ifeq ($(USE_SHARED), 1)
ifeq ($(DEBUG), 1)
GLDOPTS+=
else
GLDOPTS+=
endif
else
ifeq ($(DEBUG), 1)
# GLDOPTS+= -nodefaultlib:libcmtd.lib
# GLDOPTS+= -nodefaultlib:libcd.lib
else
# GLDOPTS+= -nodefaultlib:libcmt.lib
# GLDOPTS+= -nodefaultlib:libc.lib
endif
endif
endif

MAKEDEPEND =cl
GMAKEDEPENDFLAGS+= -showIncludes -Zs -w

BOOST_INC_DIR_TMP = $(subst ",,$(BOOST_INC_DIR))
BOOST_INC_DIR_REG = $(subst /,\/,$(BOOST_INC_DIR_TMP))

MAGICK_INC_DIR_TMP = $(subst ",,$(MAGICK_INC_DIR))
MAGICK_INC_DIR_REG = $(subst /,\/,$(MAGICK_INC_DIR_TMP))

CGAL_INC_DIR_TMP = $(subst ",,$(CGAL_INC_DIR))
CGAL_INC_DIR_REG = $(subst /,\/,$(CGAL_INC_DIR_TMP))

GMP_INC_DIR_TMP = $(subst ",,$(GMP_INC_DIR))
GMP_INC_DIR_REG = $(subst /,\/,$(GMP_INC_DIR_TMP))

MPFR_INC_DIR_TMP = $(subst ",,$(MPFR_INC_DIR))
MPFR_INC_DIR_REG = $(subst /,\/,$(MPFR_INC_DIR_TMP))

define run-makedepend-cpp
$(MAKEDEPENDF) $(MAKEDEPEND_CPPINCS) $(MAKEDEPEND_CPPDEFS) $(MAKEDEPEND_CPPOPTS) $< | \
sed -e '1 c $(basename $@).o: $< \\' \
-e 's/Note: including file: *//' \
-e 's/\\/\//g' \
-e '/Program Files\|$(BOOST_INC_DIR_REG)\|$(CGAL_INC_DIR_REG)\|$(GMP_INC_DIR_REG)\|$(MAGICK_INC_DIR_REG)/I d' \
-e '/with/,$$ d' \
-e 's/\([A-Za-z]\):/\/cygdrive\/\1/g' \
-e '$$! s/$$/ \\/g' \
> $@
endef

define run-makedepend-c
$(MAKEDEPENDF) $(MAKEDEPEND_CINCS) $(MAKEDEPEND_CDEFS) $(MAKEDEPEND_COPTS) $< | \
sed -e '1 c $(basename $@).o: $< \\' \
-e 's/Note: including file: *//' \
-e 's/\\/\//g' \
-e '/Program Files\|$(BOOST_INC_DIR_REG)\|$(CGAL_INC_DIR_REG)\|$(GMP_INC_DIR_REG)\|$(MAGICK_INC_DIR_REG)/I d' \
-e '/with/,$$ d' \
-e 's/\([A-Za-z]\):/\/cygdrive\/\1/g' \
-e '$$! s/$$/ \\/g' \
> $@
endef

define run-makedepend-a
$(MAKEDEPENDF) $(MAKEDEPEND_ASMINCS) $(MAKEDEPEND_ASMDEFS) $(MAKEDEPEND_ASMOPTS) $< | \
sed -e '1 c $(basename $@).o: $< \\' \
-e 's/Note: including file: *//' \
-e 's/\\/\//g' \
-e '/Program Files\|$(BOOST_INC_DIR_REG)\|$(CGAL_INC_DIR_REG)\|$(GMP_INC_DIR_REG)\|$(MAGICK_INC_DIR_REG)/I d' \
-e '/with/,$$ d' \
-e 's/\([A-Za-z]\):/\/cygdrive\/\1/g' \
-e '$$! s/$$/ \\/g' \
> $@
endef

# TBD: should be one of the following:
#
# Key Feature
# --- -------
# s   Static link to runtime.
# g   Debug runtime.
# y   Debug Python system.
# d   Debug enabled code.
# p   STLport runtime, instead of the vendor toolset runtime.
# n   STLport runtime using the "native" IO streams instead of the STLport IO streams.

DE_GOBJDIR =_$(USE_SHARED)

ifeq ($(COMPILER_VER), 1400)
GLDOPTS+= -manifest
else
ifeq ($(COMPILER_VER), 1500)
GLDOPTS+= -manifest
else
ifeq ($(COMPILER_VER), 1600)
GLDOPTS+= -manifest
else
ifeq ($(COMPILER_VER), 1700)
GLDOPTS+= -manifest
endif
endif
endif
endif

GLEXFLAGS = --nounistd
