define run-bison
$(BISONF) $(firstword $^) -o $(basename $@).cpp
mv $(basename $@).hpp $(basename $@).h
endef

%.cpp %.h: %.y
	$(run-bison)
