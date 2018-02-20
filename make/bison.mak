define run-bison
$(BISONF) $(firstword $^) -o $(basename $@).c
endef

%.c %.h : %.y
	$(run-bison)
