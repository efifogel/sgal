ifeq ($(BASEDIR), .)
PREFIX =x
BASEDIR_ =1
default:: all
all install clean:: $(BUILDDIR) $(BUILDDIR)/$(MAKEFILENAME) force
	+@cd $(BUILDDIR) && $(MAKEF) $@

clobber::
	@rm -rf $(BUILDDIR)
endif # BASEDIR

vpath %.l $(BASEDIR)
vpath %.y $(BASEDIR)
vpath %.c $(BASEDIR)
vpath %.asm $(BASEDIR)
vpath %.cpp $(BASEDIR)
vpath %.C $(BASEDIR)

ifeq ($(DE), msvc)
vpath %.idl $(BASEDIR)
vpath %.rc $(BASEDIR)
vpath %.def $(BASEDIR)
endif
