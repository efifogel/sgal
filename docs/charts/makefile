include $(ROOT)/make/comdef.mak

NOBIBTEX=1
NOINSTALL=1
TEXSOURCE = \
  flow.tex \
  gan.tex \
  program_prediction.tex \
  simulated_annealing.tex \
  gc_flow.tex \
  gc_orientation.tex \
  gc_predicate.tex \
  gc_trouble.tex

TARGET0 = $(TEXSOURCE:.tex=.png)
DIRT = $(TEXDIRT)

include $(MAKEDIR)/comrul.mak
# include $(MAKEDIR)/tex.mak

%.pdf : %.tex
	$(PDFLATEX) $<

%.png : %.pdf
	convert $< $@

$(TARGET0): $(FIGS)
