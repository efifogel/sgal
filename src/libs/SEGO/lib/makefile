# Copyright (c) 2012  Israel.
# All rights reserved.
#
# This file is part of the collection of Efi Fogel.
# You may redistribute it under the terms of the Q Public License version 1.0.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Author(s)     : Efi Fogel

BASEDIR =.
MULTITHREAD =1
USE_SHARED =1
DEF_SHARED =1

include $(ROOT)/make/cgaldef.mak
include $(MAKEDIR)/scgaldef.mak

# Target, etc.:
BASETARGET =SEGO
BASEINSTALLDIR =$(ROOT)

CPPSOURCES = Ego.cpp
CPPSOURCES+= Ego_brick.cpp
CPPSOURCES+= init.cpp
CPPSOURCES+= Ego_voxelizer.cpp
CPPSOURCES+= Ego_voxels_filler_base.cpp
CPPSOURCES+= Ego_voxels_filler.cpp
CPPSOURCES+= Ego_voxels_layer_filler.cpp
CPPSOURCES+= Ego_voxels_vertex_list_graph.cpp
CPPSOURCES+= Ego_voxels_vertex_list_graph_utils.cpp
CPPSOURCES+= Ego_voxels_incidence_graph.cpp
CPPSOURCES+= Ego_voxels_filler_graph.cpp
CPPSOURCES+= Ego_voxels_layer_filler_graph.cpp
CPPSOURCES+= Ego_voxels.cpp
CPPSOURCES+= Ego_voxels_tiler.cpp

LCPPDEFS+= -DSGAL_LIB
LCPPDEFS+= -DSGAL_SGAL_SOURCE
LCPPDEFS+= -DSCGAL_NT=$(SCGAL_NT)
LCPPDEFS+= -DSCGAL_KERNEL=$(SCGAL_KERNEL)

# LCPPDEFS+= -DVERBOSE

# Include:
LCPPINCS = -I$(BASEDIR)
LCPPINCS+= -I$(BASEDIR)/../include

# Add BOOST, GMP, MPFR, Magick, and gl2ps include directory:
ifdef BOOST_INC_DIR
LCPPINCS+= -I"$(BOOST_INC_DIR)"
endif

ifdef GMP_INC_DIR
LCPPINCS+= -I"$(GMP_INC_DIR)"
endif

ifdef MPFR_INC_DIR
LCPPINCS+= -I"$(MPFR_INC_DIR)"
endif

ifdef MAGICK_INC_DIR
LCPPINCS+= -I"$(MAGICK_INC_DIR)"
endif

ifdef GL2PS_INC_DIR
LCPPINCS+= -I"$(GL2PS_INC_DIR)"
endif

#
LCPPINCS+= $(CGALCPPINCS)

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS+= -Wall -Werror
LCPPOPTS+= -frounding-math
# LCPPOPTS+= -pedantic
endif

ifeq ($(DE), msvc)
LCPPDEFS+=-DBOOST_ALL_DYN_LINK
LCPPOPTS+= -GR
LCPPOPTS+= -vmg
endif

ifeq ($(DE), gnu)
LCPPDEFS+= -DGL_GLEXT_PROTOTYPES
LCPPOPTS = -Wall -Werror
# LCPPOPTS+= -pedantic
LCPPOPTS+=`Magick++-config --cxxflags --cppflags`
endif

include $(MAKEDIR)/basedir.mak
include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/comp.mak
include $(MAKEDIR)/targlib.mak

vpath %.cpp $(BASEDIR)/geo
vpath %.cpp $(BASEDIR)/geometries
