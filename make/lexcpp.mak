define run-lex
$(LEXF) -o$@ $(firstword $^)
endef

%.cpp : %.l
	$(run-lex)
