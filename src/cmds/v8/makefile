BASEDIR =.
MULTITHREAD =1
USE_SHARED =1

include $(ROOT)/make/comdef.mak

BASENAME = hello_world
TARGET0 = $(BASENAME)$(EXEFILESUFFIX)
INSTALLDIR0 = "$(BINDIR)"

ifeq ($(DE), msvc)
ifeq ($(COMPILER_VER), 1400)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = "$(BINDIR)"
else
ifeq ($(COMPILER_VER), 1500)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = "$(BINDIR)"
else
ifeq ($(COMPILER_VER), 1600)
MANIFEST_FILE = $(TARGET0).manifest
TARGET1 = $(MANIFEST_FILE)
INSTALLDIR1 = "$(BINDIR)"
endif
endif
endif
endif

CPPSOURCES = hello_world.cpp

LCPPDEFS =
LCPPOPTS =
LCPPINCS =

LLDLIBS =
LLDOPTS =

ifdef V8_INC_DIR
LCPPINCS+= -I$(V8_INC_DIR)
endif

ifeq ($(DE), msvc)
LCPPOPTS+= -GR  # Enables run-time type information (RTTI).
LCPPOPTS+= -vmg # Uses full generality for pointers to members.
LCPPOPTS+= -GS  # Apply buffers security check.
LCPPOPTS+= -Oy- # Disable Omission of frame pointer (x86 only).
# LCPPOPTS+= -WX  # Treat all compiler warnings as errors
LCPPOPTS+= -Zc:wchar_t -Zc:forScope # Specify standard behavior
# CPPWARNINGOPTS = -W4  # Specify the level of warning; 0 to 4

LLDLIBS+= xxx.lib

ifneq ($(DEBUG), 1)
LCPPOPTS+= -Gy  # Enable function-level linking
LCPPOPTS+= -Oi  # Generate intrinsic functions
endif
LLDLIBS+= $(LDSYSLIBS)

endif # msvc

ifeq ($(DE), gnu)
LCPPOPTS+= -Wall
ifdef V8_LIB_DIR
LLDOPTS+= -L$(V8_LIB_DIR)
endif
ifdef ICU_LIB_DIR
LLDOPTS+= -L$(ICU_LIB_DIR)
endif

LLDLIBS+= -lv8_base.x64
LLDLIBS+= -lv8_nosnapshot.x64
LLDLIBS+= -licui18n
LLDLIBS+= -licuuc
LLDLIBS+= -licudata
LLDLIBS+= -lrt
endif # gnu

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlink.mak

