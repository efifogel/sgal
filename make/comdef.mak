# set PASS & NEXT PASS:
ifndef PASS
PASS =0
endif

ifeq ($(PASS), 0)
NEXTPASS=1
else
ifeq ($(PASS), 1)
NEXTPASS =2
else
ifeq ($(PASS), 2)
NEXTPASS =3
endif
endif
endif

# set BATCH:
ifndef BATCH
BATCH =0
endif

# set BUILDPATH
ifdef BUILDROOT
BUILDPATH =$(subst $(WORKDIR), $(BUILDROOT),`pwd`)
endif

ifndef BUILDPATH
BUILDPATH =.
endif

# set PLATFORM:
ifndef PLATFORM
PLATFORM =windows
endif

# set DE (development environment):
ifndef DE
ifeq ($(PLATFORM), console)
DE =msvc
else
ifeq ($(PLATFORM), windows)
DE =msvc
else
ifeq ($(PLATFORM), cygwin)
DE =gnu
else
ifeq ($(PLATFORM), linux)
DE =gnu
else
ifeq ($(PLATFORM), dos)
DE =watcom
else
DE =msvc
endif
endif
endif
endif
endif
endif

# set Window System (WS):
ifndef WS
ifeq ($(PLATFORM), console)
WS =console
else
ifeq ($(PLATFORM), windows)
WS =windows
else
ifeq ($(PLATFORM), cygwin)
WS =X11
else
ifeq ($(PLATFORM), linux)
WS =X11
else
ifeq ($(PLATFORM), dos)
WS =console
else
WS =windows
endif
endif
endif
endif
endif
endif

# set Operating System (OS):
ifndef OS
ifeq ($(PLATFORM), console)
OS =windows
else
ifeq ($(PLATFORM), windows)
OS =windows
else
ifeq ($(PLATFORM), cygwin)
OS =windows
else
ifeq ($(PLATFORM), linux)
OS =linux
else
ifeq ($(PLATFORM), dos)
OS =dos
else
OS =windows
endif
endif
endif
endif
endif
endif

GFX_API ?=glide
NOMAKEDEPEND ?=0
MAKEDUMMY ?=0
MULTITHREAD ?=0
DEBUG ?=0
PROFILE ?=0
RELEASE ?=1
NUMEGA ?=0
USE_CGAL ?=0

# set USE_SHARED, DEF_SHARED:
USE_SHARED ?=0
DEF_SHARED ?=0

CLIENT ?=EXPLORER

INCDIR =$(ROOT)/include
LIBDIR =$(ROOT)/lib
BINDIR =$(ROOT)/bin
DOCDIR =$(ROOT)/doc
TEXDIR =$(ROOT)/texmf/tex/latex
BIBDIR =$(ROOT)/texmf/bibtex/bib
DATADIR =$(ROOT)/data
MAKEDIR =$(ROOT)/make

PERLLIBDIR =$(LIBDIR)/perl

MAKEFILENAME ?=makefile

MAKEFILE_SUFFIX ?=.mak
COMMONRULES =$(MAKEDIR)/comrul$(MAKEFILE_SUFFIX)
BASEDIRRULES =$(MAKEDIR)/basedir$(MAKEFILE_SUFFIX)
COMPILERULES =$(MAKEDIR)/comp$(MAKEFILE_SUFFIX)
LINKRULES =$(MAKEDIR)/link$(MAKEFILE_SUFFIX)
YACCRULES =$(MAKEDIR)/yacc$(MAKEFILE_SUFFIX)
BISONRULES =$(MAKEDIR)/bison$(MAKEFILE_SUFFIX)
TARGETLIBRULES =$(MAKEDIR)/targlib$(MAKEFILE_SUFFIX)
TARGETLINKRULES =$(MAKEDIR)/targlink$(MAKEFILE_SUFFIX)
FILESRULES =$(MAKEDIR)/files$(MAKEFILE_SUFFIX)
SUBDIRSRULES =$(MAKEDIR)/subdirs$(MAKEFILE_SUFFIX)

LIBNAME =$(LIBFILEPREFIX)$(BASELIBNAME)$(LIBFILESUFFIX)
LIBINSTALLDIR =$(BASEINSTALLDIR)/$(RELINSTALLDIR)
TMPINSTFILE := $(subst /,_,$(ROOT))
INSTFILE = $(subst :,,$(TMPINSTFILE)).inst
FILESINSTFILE = $(subst :,,$(TMPINSTFILE)).file.inst

LEX =flex
YACC =yacc
BISON =bison
COPY =cp
INST =install
PERL =perl
QT_MOC =moc

# Initialize variables to default values:
LOBJDIR =

include $(MAKEDIR)/$(DE)$(MAKEFILE_SUFFIX)

DVI2PS =dvips
DVI2PSFLAGS =-P cmz
DVI2PSF =$(DVI2PS) $(DVI2PSFLAGS)

BIBTEXF =bibtex

MAKEINDEXFLAGS =-q
MAKEINDEX =makeindex
MAKEINDEXF =$(MAKEINDEX) $(MAKEINDEXFLAGS)

LATEX =latex
ifeq ($(LATEX_TYPE), MiKTeX)
LATEXFLAGS =-include-directory=$(TEXDIR)
endif
LATEXF =$(LATEX) $(LATEXFLAGS)

PS2PDF =ps2pdf
PS2PDFFLAGS =
PS2PDFF =$(PS2PDF) $(PS2PDFFLAGS)

DVI2PDF =dvipdf
DVI2PDFFLAGS = -R0
DVI2PDFF =$(DVI2PDF) $(DVI2PDFFLAGS)

PDFLATEX =pdflatex

DVI2PDFM =dvipdfm
DVI2PDFMFLAGS =
DVI2PDFMF =$(DVI2PDFM) $(DVI2PDFMFLAGS)

DOXYGEN =doxygen
HHC =hhc

DVITEXFILES = $(TEXSOURCE:.tex=.dvi)
DVITEXFILES = $(TEXSOURCE:.tex=.out)
AUXTEXFILES = $(TEXSOURCE:.tex=.aux)
LOGTEXFILES = $(TEXSOURCE:.tex=.log)
BBLTEXFILES = $(TEXSOURCE:.tex=.bbl)
BLGTEXFILES = $(TEXSOURCE:.tex=.blg)
IDXTEXFILES = $(TEXSOURCE:.tex=.idx)
MTCTEXFILES = $(TEXSOURCE:.tex=.mtc)
MTCTEXFILES+= $(TEXSOURCE:.tex=.mtc1)
MTCTEXFILES+= $(TEXSOURCE:.tex=.mtc2)
MTCTEXFILES+= $(TEXSOURCE:.tex=.mtc3)
MTCTEXFILES+= $(TEXSOURCE:.tex=.mtc4)
INDTEXFILES = $(TEXSOURCE:.tex=.ind)
ILGTEXFILES = $(TEXSOURCE:.tex=.ilg)
HLGTEXFILES = $(TEXSOURCE:.tex=.hlg)
TOCTEXFILES = $(TEXSOURCE:.tex=.toc)
REFTEXFILES = $(TEXSOURCE:.tex=.ref)
INCTEXFILES = $(TEXSOURCE:.tex=.inc)
BMTTEXFILES = $(TEXSOURCE:.tex=.bmt)
BRFTEXFILES = $(TEXSOURCE:.tex=.brf)
NAVTEXFILES = $(TEXSOURCE:.tex=.nav)
LOFTEXFILES = $(TEXSOURCE:.tex=.lof)
SNMTEXFILES = $(TEXSOURCE:.tex=.snm)

TEXDIRT =$(DVITEXFILES) $(AUXTEXFILES) $(LOGTEXFILES) $(BBLTEXFILES) \
$(BLGTEXFILES) $(MTCTEXFILES) $(MTC1TEXFILES) $(IDXTEXFILES) $(TOCTEXFILES) \
$(REFTEXFILES) $(INCTEXFILES) $(BMTTEXFILES) $(INDTEXFILES) $(ILGTEXFILES) \
$(BRFTEXFILES) $(NAVTEXFILES) $(LOFTEXFILES) $(SNMTEXFILES)

MAKEDEPENDFILENAME =Makedepend$(MAKEFILE_SUFFIX)

CINCS =$(LCINCS) $(GCINCS)
CDEFS =$(LCDEFS) $(GCDEFS)
COPTS =$(LCOPTS) $(GCOPTS)
CFLAGS =$(CINCS) $(COPTS) $(CDEFS) $(ECFLAGS) $(EFLAGS)
CCF =$(CC) $(CFLAGS)

CPPINCS =$(LCPPINCS) $(GCPPINCS)
CPPDEFS =$(LCPPDEFS) $(GCPPDEFS)
CPPOPTS =$(LCPPOPTS) $(GCPPOPTS)
CPPFLAGS =$(CPPINCS) $(CPPOPTS) $(CPPDEFS) $(ECPPFLAGS) $(EFLAGS)
CPPF =$(CPP) $(CPPFLAGS)

TMPCPPSOURCES =$(CPPSOURCES:.cpp=.C)
CPPOBJECTS =$(TMPCPPSOURCES:.C=.o)
COBJECTS =$(CSOURCES:.c=.o)
YACCOBJECTS =$(YACCSOURCES:.y=.o)
BISONOBJECTS =$(BISONSOURCES:.y=.o)
LEXOBJECTS =$(LEXSOURCES:.l=.o)
ASMOBJECTS =$(ASMSOURCES:.asm=.o)
ifeq ($(DE), msvc)
RESOBJECTS =$(RESSOURCES:.rc=.res)
endif

GDIRT =*.o *.d
DIRT =$(LDIRT) $(GDIRT)
OBJECTS =$(CPPOBJECTS) $(COBJECTS) $(ASMOBJECTS) $(YACCOBJECTS) $(BISONOBJECTS) $(LEXOBJECTS) $(RESOBJECTS)

LLDFLAGS =$(LLDOPTS) $(LLDLIBS)
GLDFLAGS =$(GLDOPTS) $(GLDLIBS)
LDFLAGS =$(GLDFLAGS) $(LLDFLAGS)
LINKF =$(LINK) $(LDFLAGS)

LIBFLAGS =$(GLIBFLAGS) $(LLIBFLAGS)
LIBF =$(LIBPROG) $(LIBFLAGS)

ASMFLAGS =$(LASMFLAGS) $(GASMFLAGS)
ASMF =$(ASM) $(ASMFLAGS)

GYACCFLAGS =-d
YACCFLAGS =$(LYACCFLAGS) $(GYACCFLAGS)
YACCF =$(YACC) $(YACCFLAGS)

GBISONFLAGS =--verbose -ld
BISONFLAGS =$(LBISONFLAGS) $(GBISONFLAGS)
BISONF =$(BISON) $(BISONFLAGS)

# GLEXFLAGS =-S$(ROOT)/lib/flex/flex.skl
# LEX_PREFIX =yy
# GLEXFLAGS+=-P$(LEX_PREFIX)
LEXFLAGS =$(LLEXFLAGS) $(GLEXFLAGS)
LEXF =$(LEX) $(LEXFLAGS)

GMAKEFLAGS =
ifdef MAKEFILENAME
GMAKEFLAGS+= -f $(MAKEFILENAME)
endif
GMAKEFLAGS+= PASS=$(NEXTPASS) 

# GMAKEFLAGS = DE=$(DE)
# GMAKEFLAGS+= OS=$(OS)
# GMAKEFLAGS+= WS=$(WS)
# GMAKEFLAGS+= NOMAKEDEPEND=$(NOMAKEDEPEND)
# GMAKEFLAGS+= MAKEDUMMY=$(MAKEDUMMY)

MYMAKEFLAGS =$(GMAKEFLAGS) $(LMAKEFLAGS)
MAKEF =$(MAKE) $(MYMAKEFLAGS)

# Obsolete
# ifeq ($(MAKEDEPEND), makedepend)
# # I think?
# GMAKEDEPENDFLAGS =-x -a -o .o -S ::
# endif

MAKEDEPENDFLAGS =$(LMAKEDEPENDFLAGS) $(GMAKEDEPENDFLAGS)
MAKEDEPENDF =$(MAKEDEPEND) $(MAKEDEPENDFLAGS)

TMPCPPSOURCES =$(CPPSOURCES:.cpp=.C)
CPPDEPENDS =$(TMPCPPSOURCES:.C=.d)
CDEPENDS =$(CSOURCES:.c=.d)
YACCDEPENDS =$(YACCSOURCES:.y=.d)
BISONDEPENDS =$(BISONSOURCES:.y=.d)
LEXDEPENDS =$(LEXSOURCES:.l=.d)
DEPENDS =$(CPPDEPENDS) $(CDEPENDS) $(YACCDEPENDS) $(BISONDEPENDS) $(LEXDEPENDS)

ifndef FIRSTSTAGE
FIRSTSTAGE =0
endif

# ifneq ($(NOMAKEDEPEND), 1)
# STAGE1FILE =$(MAKEDEPENDFILENAME)
# FIRSTSTAGE =1
# endif

# Notice that a STAGE=X on the make command line overrides everything,
# so the only chance to change STAGE is on the first make run.
ifndef STAGE
STAGE =$(FIRSTSTAGE)
endif

GOBJDIR :=$(PLATFORM)

ifeq ($(DEBUG), 1)
GOBJDIR :=$(GOBJDIR)_debug
endif
GOBJDIR :=$(GOBJDIR)_$(DE)_$(COMPILER_VER)
ifeq ($(MULTITHREAD), 1)
GOBJDIR :=$(GOBJDIR)_mt
endif
GOBJDIR :=$(GOBJDIR)$(DE_GOBJDIR)

ifeq ($(PROFILE), 1)
GOBJDIR :=$(GOBJDIR)_prof
endif

ifeq ($(NUMEGA), 1)
GOBJDIR :=$(GOBJDIR)_nm
endif

ifndef OBJDIR
OBJDIR =$(GOBJDIR)$(LOBJDIR)
endif

BUILDDIR =$(BUILDPATH)/$(OBJDIR)

GLIDEDEFS =-DGLIDE
ifeq ($(GLIDE_VER), 2.0)
GLIDEDEFS+= -DGLIDE_VER=200
GLIDELIB=glide
else
GLIDEDEFS+= -DGLIDE_VER=240
GLIDELIB=glide2x
endif

OPENGLDEFS =-DOPENGL

ifeq ($(MAKEDUMMY), 1)
ASM  =echo
CC   =echo
CPP  =echo
LIBPROG  =echo
LINK =echo
LEX  =echo
YACC =echo
BISON =echo
INST =echo
PERL =echo
MAKEDEPEND =echo
.CONTINUE
endif

DEBUGDEFS =-DNDEBUG
ifeq ($(DEBUG), 1)
DEBUGDEFS =-D_DEBUG
endif

GCDEFS+= $(DEBUGDEFS) $(DLLDEFS)
GCPPDEFS+= $(DEBUGDEFS) $(DLLDEFS)

ifeq ($(GFX_API), opengl)
GFX_DEFS =$(OPENGLDEFS)
else
GFX_DEFS =$(GLIDEDEFS)
endif

TEMPLIBS = $(subst <,$(LIBLINKPREFIX),$(BASELIBS))
GLDLIBS+= $(subst >,$(LIBLINKSUFFIX),$(TEMPLIBS))
TEMPLIBS2 = $(subst <,$(LIBDIR)/$(LIBFILEPREFIX),$(BASELIBS))
LIBS = $(subst >,$(LIBLINKSUFFIX),$(TEMPLIBS2))

TARGET = $(TARGET0) $(TARGET1) $(TARGET2)

X3DFILES =$(X3DSOURCES) $(VRMLSOURCES:.wrl=.x3d) $(MAXSOURCES:.max=.x3d)
ENXFILES = $(X3DFILES:.x3d=.enx)
HTMLFILES =$(X3DFILES:.x3d=.html)
ENSFILES =$(X3DFILES:.x3d=.ens)
ENCFILES =$(X3DFILES:.x3d=.enc)
LOGFILES =$(X3DFILES:.x3d=.log)

# VRML to X3D:
WRL2X3D = $(BINDIR)/wrl2x3d.exe
WRL2X3DFLAGS =
WRL2X3DF = $(WRL2X3D) $(WRL2X3DFLAGS)

# X3D to ENX:
X3D2ENX = $(BINDIR)/Convertor.exe
X3D2ENXFLAGS =
X3D2ENXF = $(X3D2ENX) $(X3D2ENXFLAGS)

# Make html page:
MAKEHTMLSCRIPT =$(BINDIR)/genEmbedingHtml.pl
MAKEHTMLPAGE = $(PERL) $(MAKEHTMLSCRIPT)
MAKEHTMLPAGEFLAGS =
MAKEHTMLPAGEF = $(MAKEHTMLPAGE) $(MAKEHTMLPAGEFLAGS)

GENMAINHTMLSCRIPT =$(BINDIR)/genMainHtml.pl
GENMAINHTML = $(PERL) $(GENMAINHTMLSCRIPT)
GENMAINHTMLFLAGS =
GENMAINHTMLF = $(GENMAINHTML) $(GENMAINHTMLFLAGS)

# extract BOOST directories:
ifdef BOOST_DIR
ifndef BOOST_INC_DIR
BOOST_INC_DIR="$(BOOST_DIR)/include"
endif
ifndef BOOST_LIB_DIR
BOOST_LIB_DIR="$(BOOST_DIR)/lib"
endif
endif

# extract GMP include directories:
ifdef GMP_DIR
ifndef GMP_INC_DIR
GMP_INC_DIR="$(GMP_DIR)/include"
endif
ifndef GMP_LIB_DIR
GMP_LIB_DIR="$(GMP_DIR)/lib"
endif
endif

# extract MPFR directories:
ifdef MPFR_DIR
ifndef MPFR_INC_DIR
MPFR_INC_DIR="$(MPFR_DIR)/include"
endif
ifndef MPFR_LIB_DIR
MPFR_LIB_DIR="$(MPFR_DIR)/lib"
endif
endif

# extract MAGICK include directories:
ifdef MAGICK_DIR
ifndef MAGICK_INC_DIR
MAGICK_INC_DIR="$(MAGICK_DIR)/include"
endif
ifndef MAGICK_LIB_DIR
MAGICK_LIB_DIR="$(MAGICK_DIR)/lib"
endif
endif

# extract GL2PS directories:
ifdef GL2PS_DIR
ifndef GL2PS_INC_DIR
GL2PS_INC_DIR="$(GL2PS_DIR)/include"
endif
ifndef GL2PS_LIB_DIR
GL2PS_LIB_DIR="$(GL2PS_DIR)/lib"
endif
endif

# extract CGAL directories:
ifdef CGAL_DIR
ifndef CGAL_INC_DIR
CGAL_INC_DIR="$(CGAL_DIR)/include"
endif
ifndef CGAL_LIB_DIR
CGAL_LIB_DIR="$(CGAL_DIR)/lib"
endif
endif
