include $(ROOT)/make/comdef.mak

NOBIBTEX=1
NOINSTALL=1
TEXSOURCE = flow.tex gan.tex program_prediction.tex simulated_annealing.tex

TARGET0 = $(TEXSOURCE:.tex=.png)
DIRT = $(TEXDIRT)

include $(MAKEDIR)/comrul.mak
# include $(MAKEDIR)/tex.mak

%.pdf : %.tex
	$(PDFLATEX) $<

%.png : %.pdf
	convert $< $@

$(TARGET0): $(FIGS)
