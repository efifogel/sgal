if(NOT SGAL_MACROS_FILE_INCLUDED)
  set(SGAL_MACROS_FILE_INCLUDED 1)

  macro(hide_variable var)
    set (${var} ${${var}} CACHE INTERNAL "Variable hidden from user" FORCE)
  endmacro()
endif()
