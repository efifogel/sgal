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
GMP_INC_DIR="$(GMP_DIR)/include"
endif
ifndef GMP_LIB_DIR
GMP_LIB_DIR="$(GMP_DIR)/lib"
endif
endif

STD_ROOT = $(shell cygpath -m $(ROOT))
GCINCS = -I$(STD_ROOT)/include
GCPPINCS+= -I$(STD_ROOT)/include

# If GMP_INC_DIR is defined, add it to the list of include directories
ifdef GMP_INC_DIR
GCPPINCS+= -I"$(GMP_INC_DIR)"
endif

UNICODE_DEFS = -D_MBCS
GCDEFS =-D_WIN32 -DWIN32 # -DENBAYA_MSVC -DENBAYA_WINDOWS
GCPPDEFS =-D_WIN32 -DWIN32 # -DENBAYA_MSVC -DENBAYA_WINDOWS 
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
ifeq ($(COMPILER_VER), 1600)
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
GCMDOPTS = -MLd
GCPPMDOPTS+= -MLd
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
GCMDOPTS = -ML
GCPPMDOPTS+= -ML
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
GCDEBUGOPTS =-Gm -Zi
GCPPDEBUGOPTS =-Gm -Zi

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
GCDEBUGOPTS+= -GZ
GCPPDEBUGOPTS+= -GZ
endif
endif
endif

GCOPTOPTS =-Od
GCPPOPTOPTS =-Od
GLDDEBUGOPTS = -debug
GLDDEBUGOPTS+= -incremental:yes


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
# GCPPOPTS+= -W3

# error prompting:
ifeq ($(COMPILER_VER), 1400)
GERRORPROMP = -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1500)
GERRORPROMP = -errorReport:prompt
else
ifeq ($(COMPILER_VER), 1600)
GERRORPROMP = -errorReport:prompt
endif
endif
endif

GCERRORPROMP = $(GERRORPROMP)
GCPPERRORPROMP = $(GERRORPROMP)
GLDERRORPROMP = $(GERRORPROMP)

# Code Generation:
GCPPCODEOPTS = -GR
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
GCPPCODEOPTS+= -GX
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
ifdef GMP_LIB_DIR
GLIBPATHOPTS+= -libpath:"$(GMP_LIB_DIR)"
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
GLDOPTS+= -nodefaultlib:libcmt.lib
GLDOPTS+= -nodefaultlib:libcpmt.lib
GLDOPTS+= -nodefaultlib:libc.lib
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

ifeq ($(MAKEDEPEND), gcc)
# GMAKEDEPENDFLAGS+= -isystem '$(MSVCDIR)/Include'
# GMAKEDEPENDFLAGS+= -isystem '$(MSVCDIR)/ATL/Include'
# GMAKEDEPENDFLAGS+= -isystem '$(MSVCDIR)/MFC/Include'
# GMAKEDEPENDFLAGS+= -nostdinc
ifeq ($(COMPILER_VER), 1200)
MSVCDIR_STLPORT_TMP=$(shell cygpath "$(MSVCDIR)/Include/stlport")
GMAKEDEPENDFLAGS+= -isystem "$(MSVCDIR_STLPORT_TMP)"
endif

ifdef MSVCDIR
MSVCDIR_TMP=$(shell cygpath "$(MSVCDIR)/Include")
GMAKEDEPENDFLAGS+= -isystem "$(MSVCDIR_TMP)"
else
ifdef VCINSTALLDIR
VCINSTALLDIR_TMP=$(shell cygpath "$(VCINSTALLDIR)/Include")
GMAKEDEPENDFLAGS+= -isystem "$(VCINSTALLDIR_TMP)"
endif
endif

ifdef BOOST_INC_DIR
BOOST_TMP=$(shell cygpath "$(BOOST_INC_DIR)")
GMAKEDEPENDFLAGS+= -isystem "$(BOOST_TMP)"
endif
ifdef GMP_INC_DIR
GMP_TMP=$(shell cygpath "$(GMP_INC_DIR)")
GMAKEDEPENDFLAGS+= -isystem "$(GMP_TMP)"
endif
GMAKEDEPENDFLAGS+= -DNOMINMAX -D_M_IA64 -D_M_IX86 -D_X86_ -D_MSC_VER=$(COMPILER_VER)
endif

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
endif
endif
endif

GLEXFLAGS = --nounistd
