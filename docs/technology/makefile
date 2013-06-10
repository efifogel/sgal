include $(ROOT)/make/comdef.mak

NOBIBTEX=1
NOINSTALL=1
TEXSOURCE = technology.tex

TARGET0 = $(TEXSOURCE:.tex=.pdf)
DIRT = $(TEXDIRT)
DVI2PDFFLAGS = -q "-R0 -Ppdf -G0"
DVI2PSFLAGS = "-R0 -P pdf"

include $(MAKEDIR)/comrul.mak
include $(MAKEDIR)/tex.mak

FIGS = \

$(TARGET0): ../header.tex $(FIGS)
