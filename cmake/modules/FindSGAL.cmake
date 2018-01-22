# - Try to find scgal (scgal.lib on Windows and scgal_base.x64.a on Linux)
# Once done this will define
#  SGAL_FOUND - System has scgal
#  SGAL_INCLUDE_DIR - The scgal include directories
#  SGAL_LIBRARY - The library needed to use SGAL
#  SGAL_LIBRARY_DIR - The directory where lib files are.

find_path(SGAL_INCLUDE_DIR NAMES SGAL/basic.hpp
          PATH_SUFFIXES include
          HINTS ENV SGAL_INC_DIR
	        ENV SGAL_DIR)

if(CMAKE_CL_64)
    set(SGAL_ARCH x64)
else()
    set(SGAL_ARCH x86)
endif()

# CMake>=2.6 supports the notation "debug XXd optimized XX"
if (UNIX)
  find_library(SGAL_LIBRARY NAMES SGAL
               PATH_SUFFIXES lib
               HINTS ENV SGAL_LIB_DIR
	             ENV SGAL_DIR)
  get_filename_component(SGAL_LIBRARY_DIR ${SGAL_LIBRARY} PATH)
elseif(WIN32)
  if(MSVC_VERSION EQUAL 1600)
    set(SGAL_RUNTIME vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(SGAL_RUNTIME vc11)
  endif()

  IF(CMAKE_BUILD_TYPE MATCHES Release)
    find_library(SGAL_LIB_OPT NAMES SGAL
                 PATH_SUFFIXES lib
                 HINTS ENV SGAL_DIR/build/Release
		       ENV SGAL_LIB_DIR
	               ENV SGAL_DIR)
    get_filename_component(SGAL_LIBRARY_DIR ${SGAL_LIB_OPT} PATH)
    list(APPEND SGAL_LIBRARY optimized ${SGAL_LIB_OPT})
  else()
    find_library(SGAL_LIB_DBG NAMES scgald
                 PATH_SUFFIXES lib
                 HINTS ENV SGAL_DIR/build/Debug
		       ENV SGAL_LIB_DIR
	               ENV SGAL_DIR)
    get_filename_component(SGAL_LIBRARY_DIR ${SGAL_LIB_DBG} PATH)
    list(APPEND SGAL_LIBRARY debug ${SGAL_LIB_DBG})
  endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SGAL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SGAL DEFAULT_MSG
                                  SGAL_LIBRARY
				  SGAL_INCLUDE_DIR)

mark_as_advanced(SGAL_INCLUDE_DIR SGAL_LIBRARY SGAL_LIBRARY_DIR)
