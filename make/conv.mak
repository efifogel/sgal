%.x3d : %.wrl $(WRL2X3D)
	$(WRL2X3DF) $< $@

%.enx : %.x3d $(X3D2ENX)
	$(X3D2ENXF) $< $@

%.ens : %.x3d $(X3D2ENX)
	$(X3D2ENXF) $< $(basename $@).enx

ifeq ($(COMPRESS), 1)
%.enc : %.enx $(X3D2ENX)
	$(X3D2ENXF) -zippedEnx $<
ifdef UPDATE_ENB0
	cp $(basename $<)0c.enb $(basename $<)0.enb
endif

%.html : %.enc $(MAKEHTMLSCRIPT)
	$(MAKEHTMLPAGEF) -infoFile $< -htmlFile $@
else
%.html : %.ens $(MAKEHTMLSCRIPT)
	$(MAKEHTMLPAGEF) -infoFile $< -htmlFile $@
endif

