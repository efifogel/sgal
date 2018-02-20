include $(ROOT)/make/comdef.mak

# Nuber types:
SCGAL_DOUBLE_NT =             0
SCGAL_LEDA_RAT_NT =           1
SCGAL_QUOTIENT_MP_FLOAT_NT =  2
SCGAL_CGAL_GMPQ_NT =          3
SCGAL_CGAL_GMPZ_NT =          4

# Traits:
CGAL_ARR_SEGMENT_TRAITS =             0
CGAL_ARR_NON_CACHING_SEGMENT_TRAITS = 1

# Kernel types:
SCGAL_CARTESIAN_KERNEL =             0
SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL = 1
SCGAL_HOMOGENEOUS_KERNEL =           2

# Identification XY values:
CGAL_X_MINUS_1_Y_0 =  0
CGAL_X_MINUS_8_Y_6 =  1
CGAL_X_MINUS_11_Y_7 = 2

# Default values:
CGAL_CGM_TRAITS ?= $(CGAL_ARR_SEGMENT_TRAITS)
CGAL_IDENTIFICATION_XY ?= $(CGAL_X_MINUS_1_Y_0)

CGALCPPINCS = -I"$(CGAL_DIR)/include"
CGALCPPOPTS =
CGALCPPDEFS =

ifeq ($(DE), gnu)

CGALLIBDIRS = -L"$(CGAL_DIR)/lib"

# only versions less than 3.2 require (GCCROOT)/include be explicitly provided
# as a directory to search for header files
ifdef GCCROOT
CGALLIBDIRS+= -L$(GCCROOT)/lib
endif

ifdef GCCROOT
CGALLIBOPTS+= -Wl,-rpath $(GCCROOT)/lib
endif
CGALLIBOPTS+= -Wl,-rpath "$(CGAL_DIR)/lib"

LEDALIBS = -lP -lG -lL -lW
QTLIBS = -lqt-mt
ifeq ($(COMPILER_VER), 4.0.4)
QTLIBS+= -lfreetype
endif

CGALLIB = -lCGAL
CGALCORELIB = -lCGAL_Core
CGALQTLIB = -lCGAL_Qt

CGALLDLIBS = $(CGALLIB)
ifeq ($(USE_CORE), 1)
CGALLDLIBS+= $(CGALCORELIB)
endif
endif

ifeq ($(DE), msvc)

CGALLIBDIRS = -libpath:"$(CGAL_DIR)/lib"

LCPPOPTS = -GR -Zm900

LE=
ifeq ($(DEBUG), 1)
LE=_mld
else
LE=_ml
endif
LLDLIBS+= CGAL.lib
ifeq ($(USE_GMP), 1)
LLDLIBS+= gmp.lib
endif
LLDLIBS+= user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ws2_32.lib
endif

CGALCPPINCS+=
CGALCPPDEFS+=
CGALCPPOPTS+=

LCPPINCS = $(CGALCPPINCS)
LCPPDEFS = $(CGALCPPDEFS)
LCPPOPTS = $(CGALCPPOPTS)

CGALLDOPTS = $(CGALLIBOPTS) $(CGALLIBDIRS)

LLDOPTS = $(CGALLDOPTS)
LLDLIBS = $(CGALLDLIBS)
