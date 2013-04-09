define run-bison
$(BISONF) $(firstword $^) -o $(basename $@).cpp
endef

%.cpp %.hpp: %.y
	$(run-bison)
