FILES = comdef.mak
FILES+= comrul.mak
FILES+= subdirs.mak
FILES+= files.mak
FILES+= msvc.mak
FILES+= gnu.mak
FILES+= basedir.mak
FILES+= baselibs.mak
FILES+= conv.mak
FILES+= comp.mak
FILES+= link.mak
FILES+= yacc.mak bison.mak
FILES+= lex.mak
FILES+= targlink.mak
FILES+= targlib.mak
FILES+= targ.mak
FILES+= bisoncpp.mak
FILES+= lexcpp.mak
FILES+= tex.mak
FILES+= artdef.mak
FILES+= artrul.mak
FILES+= cgaldef.mak cgalrul.mak scgaldef.mak
# FILES+= extensions.mak
# FILES+= watcom.mak
# FILES+= autoconv.mak
# FILES+= conviff.mak
# FILES+= convdem.mak
# FILES+= convpro.mak
# FILES+= targdll.mak
# FILES+= yacccpp.mak

INSTALLDIR =$(ROOT)/make

default clean clobber:

install:
	install -d $(INSTALLDIR)
	install $(FILES) $(INSTALLDIR)
