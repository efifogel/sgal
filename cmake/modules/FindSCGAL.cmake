# - Try to find scgal (scgal.lib on Windows and libSCGAL.so on Linux)
# Once done this will define
#  SCGAL_FOUND - System has scgal
#  SCGAL_INCLUDE_DIR - The scgal include directories
#  SCGAL_LIBRARY - The library needed to use SCGAL
#  SCGAL_LIBRARY_DIR - The directory where lib files are.

find_path(SCGAL_INCLUDE_DIR NAMES SCGAL/basic.hpp
          PATH_SUFFIXES include
          HINTS ENV SCGAL_INC_DIR
	        ENV SCGAL_DIR)

if(CMAKE_CL_64)
    set(SCGAL_ARCH x64)
else()
    set(SCGAL_ARCH x86)
endif()

# CMake>=2.6 supports the notation "debug XXd optimized XX"
if (UNIX)
  find_library(SCGAL_LIBRARY NAMES SCGAL
               PATH_SUFFIXES lib
               HINTS ENV SCGAL_LIB_DIR
	             ENV SCGAL_DIR)
  get_filename_component(SCGAL_LIBRARY_DIR ${SCGAL_LIBRARY} PATH CACHE)
elseif(WIN32)
  if(MSVC_VERSION EQUAL 1600)
    set(SCGAL_RUNTIME vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(SCGAL_RUNTIME vc11)
  endif()

  IF(CMAKE_BUILD_TYPE MATCHES Release)
    find_library(SCGAL_LIB_OPT NAMES SCGAL
                 PATH_SUFFIXES lib
                 HINTS ENV SCGAL_DIR/build/Release
		       ENV SCGAL_LIB_DIR
	               ENV SCGAL_DIR)
    get_filename_component(SCGAL_LIBRARY_DIR ${SCGAL_LIB_OPT} PATH)
    list(APPEND SCGAL_LIBRARY optimized ${SCGAL_LIB_OPT})
  else()
    find_library(SCGAL_LIB_DBG NAMES scgald
                 PATH_SUFFIXES lib
                 HINTS ENV SCGAL_DIR/build/Debug
		       ENV SCGAL_LIB_DIR
	               ENV SCGAL_DIR)
    get_filename_component(SCGAL_LIBRARY_DIR ${SCGAL_LIB_DBG} PATH)
    list(APPEND SCGAL_LIBRARY debug ${SCGAL_LIB_DBG})
  endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SCGAL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SCGAL DEFAULT_MSG
                                  SCGAL_LIBRARY
				  SCGAL_INCLUDE_DIR)

mark_as_advanced(SCGAL_INCLUDE_DIR SCGAL_LIBRARY SCGAL_LIBRARY_DIR)




