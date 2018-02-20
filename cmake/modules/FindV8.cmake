# - Try to find v8 (v8.lib on Windows and v8_base.x64.a on Linux)
# Once done this will define
#  V8_FOUND - System has v8
#  V8_INCLUDE_DIR - The v8 include directories
#  V8_LIBRARY - The library needed to use V8
#  V8_LIBRARY_DIR - The directory where lib files are.

find_path(V8_INCLUDE_DIR NAMES v8.h
          PATH_SUFFIXES include
          HINTS ENV V8_INC_DIR
	        ENV V8_DIR)

if(CMAKE_CL_64)
    set(V8_ARCH x64)
else()
    set(V8_ARCH x86)
endif()

# CMake>=2.6 supports the notation "debug XXd optimized XX"
if (UNIX)
  find_library(V8_LIB_OPT NAMES v8
               PATH_SUFFIXES lib
               HINTS ENV V8_LIB_DIR
	             ENV V8_DIR)
  get_filename_component(V8_LIBRARY_DIR ${V8_LIB_OPT} PATH)
  set(V8_LIBRARY optimized ${V8_LIB_OPT})
  list(APPEND V8_LIBRARY debug ${V8_LIB_OPT})
elseif(WIN32)
  if(MSVC_VERSION EQUAL 1600)
    set(V8_RUNTIME vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(V8_RUNTIME vc11)
  endif()

  IF(CMAKE_BUILD_TYPE MATCHES Release)
    find_library(V8_LIB_OPT NAMES v8
                 PATH_SUFFIXES lib
                 HINTS ENV V8_DIR/build/Release
		       ENV V8_LIB_DIR
	               ENV V8_DIR)
    get_filename_component(V8_LIBRARY_DIR ${V8_LIB_OPT} PATH)
    list(APPEND V8_LIBRARY optimized ${V8_LIB_OPT})
  else()
    find_library(V8_LIB_DBG NAMES v8d
                 PATH_SUFFIXES lib
                 HINTS ENV V8_DIR/build/Debug
		       ENV V8_LIB_DIR
	               ENV V8_DIR)
    get_filename_component(V8_LIBRARY_DIR ${V8_LIB_DBG} PATH)
    list(APPEND V8_LIBRARY debug ${V8_LIB_DBG})
  endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set V8_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(V8 DEFAULT_MSG
                                  V8_LIBRARY
				  V8_INCLUDE_DIR)

mark_as_advanced(V8_INCLUDE_DIR V8_LIBRARY V8_LIBRARY_DIR)
