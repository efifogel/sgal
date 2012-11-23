ifeq ($(DEF_SHARED), 1)
include $(MAKEDIR)/targlink.mak
else
include $(MAKEDIR)/targlib.mak
endif
