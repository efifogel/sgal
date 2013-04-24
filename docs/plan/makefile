include $(ROOT)/make/comdef.mak

NOBIBTEX=1
NOINSTALL=1
TEXSOURCE = plan.tex

TARGET0 = $(TEXSOURCE:.tex=.pdf)
DIRT = $(TEXDIRT)
DVI2PDFFLAGS = -q "-R0 -Ppdf -G0"
DVI2PSFLAGS = "-R0 -P pdf"

include $(MAKEDIR)/comrul.mak
# include $(MAKEDIR)/tex.mak

FIGS = \

$(TARGET0): $(FIGS)

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

%.ps: %.dvi
	$(DVI2PSF) $< -o $@

%.pdf: %.ps
	$(PS2PDFF) $<

# %.pdf: %.dvi
# 	$(DVI2PDFF) $<

# graphs:
# 	make -C $@

# diagrams:
# 	make -C $@

# sign: $(PDF)
# 	openssl req -x509 -nodes -days 365 -subj '/C=US/ST=Oregon/L=Portland/CN=www.madboa.com' -newkey rsa:1024 -keyout mycert.pem -out mycert.pem
# 	openssl pkcs12 -export -in mycert.pem -out mycert.pfx
# 	PortableSigner -n -s mycert.pfx -t $(PDF) -o $(PDF:.pdf=)-signed.pdf

# clean:
# 	make clean -C graphs
# 	make clean -C diagrams
# 	rm -f $(DVI) $(PS) $(PDF) $(AUX) $(LOG) $(SNM) $(OUT) $(NAV) $(TOC) $(BBL) $(BLG) $(LOF) $(IDX) $(PDF:.pdf=)-signed.pdf *~ *.ps *.log

# .PHONY: graphs diagrams 
