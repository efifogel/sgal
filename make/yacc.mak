define run-yacc
$(YACCF) $(firstword $^)
mv ytab.c $@
endef

%.c: %.y
	$(run-yacc)
