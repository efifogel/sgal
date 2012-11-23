%.dvi : %.tex
	$(LATEXF) $(basename $<)
ifneq ($(NOBIBTEX), 1)
	$(BIBTEXF) $(basename $<)
	$(LATEXF) $(basename $<)
endif
	$(LATEXF) $(basename $<)
ifeq ($(NOMAKEINDEX), 0)
	$(MAKEINDEXF) $(basename $<)
	$(LATEXF) $(basename $<)
endif

%.ps : %.dvi
	$(DVI2PSF) $< -o $@

%.eps : %.dvi
	$(DVI2PSF) -E $< -o $@

%.pdf : %.dvi
	$(DVI2PDFF) $< $@

.PHONY :: show

show :: default
	$(PSVIEWF) $(TARGET0)
