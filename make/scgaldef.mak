# Default values:
SCGAL_NT ?= $(SCGAL_CGAL_GMPQ_NT)
SCGAL_KERNEL ?= $(SCGAL_CARTESIAN_KERNEL)

# GOBJDIR
ifeq ($(SCGAL_NT), $(SCGAL_DOUBLE_NT))
GOBJDIR :=$(GOBJDIR)_double
else
ifeq ($(SCGAL_NT), $(SCGAL_LEDA_RAT_NT))
GOBJDIR :=$(GOBJDIR)_leda_rat
else
ifeq ($(SCGAL_NT), $(SCGAL_QUOTIENT_MP_FLOAT_NT))
GOBJDIR :=$(GOBJDIR)_quotient
else
ifeq ($(SCGAL_NT), $(SCGAL_CGAL_GMPQ_NT))
GOBJDIR :=$(GOBJDIR)_cgal_gmpq
else
ifeq ($(SCGAL_NT), $(SCGAL_CGAL_GMPZ_NT))
GOBJDIR :=$(GOBJDIR)_cgal_gmpz
endif
endif
endif
endif
endif

ifeq ($(CGAL_CGM_TRAITS), $(CGAL_ARR_NON_CACHING_SEGMENT_TRAITS))
GOBJDIR :=$(GOBJDIR)_non_caching_seg
else
ifeq ($(CGAL_CGM_TRAITS), $(CGAL_ARR_SEGMENT_TRAITS))
GOBJDIR :=$(GOBJDIR)_seg
endif
endif

ifeq ($(SCGAL_KERNEL), $(SCGAL_CARTESIAN_KERNEL))
GOBJDIR :=$(GOBJDIR)_cartesian
else
ifeq ($(SCGAL_KERNEL), $(SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL))
GOBJDIR :=$(GOBJDIR)_lazy_sc
else
ifeq ($(SCGAL_KERNEL), $(SCGAL_HOMOGENEOUS_KERNEL))
GOBJDIR :=$(GOBJDIR)_homogeneous
endif
endif
endif
