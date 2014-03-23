# - Try to find v8 (v8.lib on Windows and v8_base.x64.a on Linux)
# Once done this will define
#  V8_FOUND - System has v8
#  V8_INCLUDE_DIR - The v8 include directories
#  V8_LIBRARIES - The libraries needed to use V8
#  V8_LIB_DIR - The directory where lib files are.
#               Calling link_directories() with this path is NOT needed.

find_path(V8_INCLUDE_DIR NAMES v8.h
          PATH_SUFFIXES include
          HINTS ENV V8_INC_DIR
	        ENV V8_DIR)

if(CMAKE_CL_64)
    set(V8_ARCH x64)
else()
    set(V8_ARCH x86)
endif()

if(MSVC_VERSION EQUAL 1600)
    set(V8_RUNTIME vc10)
elseif(MSVC_VERSION EQUAL 1700)
    set(V8_RUNTIME vc11)
endif()

# CMake>=2.6 supports the notation "debug XXd optimized XX"
IF(CMAKE_BUILD_TYPE MATCHES Release)
  find_library(V8_LIB_OPT NAMES v8
             PATH_SUFFIXES lib
             HINTS ENV V8_LIB_DIR
	           ENV V8_DIR
		   ENV V8_DIR/build/Release)
list(APPEND V8_LIBRARIES optimized ${V8_LIB_OPT})
else()
find_library(V8_LIB_DBG NAMES v8d
             PATH_SUFFIXES lib
             HINTS ENV V8_LIB_DIR
	           ENV V8_DIR
		   ENV V8_DIR/build/Debug)
list(APPEND V8_LIBRARIES debug ${V8_LIB_DBG})
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set Czm_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(V8 DEFAULT_MSG
                                  V8_LIBRARIES
				  V8_INCLUDE_DIR)

mark_as_advanced(V8_INCLUDE_DIR V8_LIBRARIES)
