define run-lex
$(LEXF) -o$@ $(firstword $^)
endef

%.c : %.l
	$(run-lex)
