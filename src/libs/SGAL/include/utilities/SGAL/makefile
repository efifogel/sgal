include $(ROOT)/make/comdef.mak

FILES = App_error_handle_int.hpp
FILES+= Bench_option_parser.hpp
FILES+= Conf_option_parser.hpp
FILES+= Generic_option_parser.hpp
FILES+= Native_window_item.hpp
FILES+= Native_window_manager.hpp
FILES+= Option_parser.hpp
FILES+= Scene.hpp
FILES+= Window_item.hpp
FILES+= Window_manager.hpp
FILES+= Window_option_parser.hpp
ifeq ($(WS), console)
FILES+= Windows_window_manager.hpp
FILES+= Windows_window_item.hpp
endif
ifeq ($(WS), X11)
FILES+= X11_window_manager.hpp
FILES+= X11_window_item.hpp
endif

INSTALLDIR =$(INCDIR)/SGAL
include $(MAKEDIR)/files.mak
