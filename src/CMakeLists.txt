set(SGAL_MAJOR_VERSION 0)
set(SGAL_MINOR_VERSION 1)
set(SGAL_PATCH_VERSION 0)
set(SGAL_VERSION
  ${SGAL_MAJOR_VERSION}.${SGAL_MINOR_VERSION}.${SGAL_PATCH_VERSION})

# Use C++11 for this directory and its sub-directories.
list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_generalized_initializers has_cpp11)
if (has_cpp11 LESS 0)
  message(STATUS "NOTICE: These examples requires a C++11 compiler and will not be compiled.")
  return()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find required libraries
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options regex)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
if (SGAL_USE_V8)
  find_package(V8 REQUIRED)
  link_directories(${V8_LIBRARY_DIR})
endif()

if (NOT WIN32)
  find_package(X11 REQUIRED)
endif(NOT WIN32)

find_package(CGAL REQUIRED COMPONENTS Core)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

find_package(Eigen3 3.1.0) #(requires 3.1.0 or greater)
if (NOT EIGEN3_FOUND)
  message(STATUS "This library cannot be compiled, as it requires Eigen3.")
  return()
endif()
include(${EIGEN3_USE_FILE})

find_package(Fontconfig)
if (NOT FONTCONFIG_FOUND)
  message(STATUS "This library cannot be compiled, as it requires Fontconfig.")
  return()
endif()

# Add includes and libs
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${V8_INCLUDE_DIR})
if (NOT WIN32)
  include_directories(${X11_INCLUDE_DIR})
endif(NOT WIN32)

include_directories(libs/boost/include)
include_directories(libs/gl2ps/include)
if (WIN32)
  include_directories(libs/GL/include)
  include_directories(libs/flex/include)
endif(WIN32)

# Add some compiler options
if (WIN32)
  add_definitions(-GR)
  add_definitions(-vmg)
endif()

# Use dynamic link for all SGAL libraries
add_definitions(-DSGAL_ALL_DYN_LINK)

if (SGAL_TRACE)
  add_definitions(-DSGAL_TRACE)
endif()

# Installation
if (SGAL_WIN32_CMAKE_ON_CYGWIN)
  exec_program(cygpath ARGS -w "${CMAKE_INSTALL_PREFIX}"
               OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX2)
  file(TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX2} CMAKE_INSTALL_PREFIX)
endif()

# Offer the user the choice of overriding the installation directories
set (SGAL_INSTALL_LIB_DIR lib CACHE PATH
  "Installation directory for libraries")
set (SGAL_INSTALL_BIN_DIR bin CACHE PATH
  "Installation directory for executables")
set (SGAL_INSTALL_INC_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(SGAL_DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(SGAL_DEF_INSTALL_CMAKE_DIR lib/cmake/SGAL)
endif()
set(SGAL_INSTALL_CMAKE_DIR ${SGAL_DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INC CMAKE)
  set(var SGAL_INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Add subdirectories
add_subdirectory(scripts)
add_subdirectory(libs)
add_subdirectory(cmds)

# Add all targets to the build-tree export set
if (SGAL_USE_SCGAL)
  export(TARGETS SGAL SCGAL player-bin
    FILE "${PROJECT_BINARY_DIR}/SGALTargets.cmake")
else()
  export(TARGETS SGAL player
    FILE "${PROJECT_BINARY_DIR}/SGALTargets.cmake")
endif()

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE SGAL)

# Create the SGALConfig.cmake and SGALConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${SGAL_INSTALL_CMAKE_DIR}"
   "${SGAL_INSTALL_INC_DIR}")

# ... for the build tree
# set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
set(CONF_INCLUDE_DIRS
  "${SGAL_SGAL_INCLUDE_DIRS}"
  "${SGAL_SCGAL_INCLUDE_DIRS}"
  "${SGAL_BOOST_INCLUDE_DIRS}")
configure_file(SGALConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SGALConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${SGAL_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(SGALConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SGALConfig.cmake" @ONLY)

# ... for both
configure_file(SGALConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/SGALConfigVersion.cmake" @ONLY)

# Install the SGALConfig.cmake and SGALConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SGALConfig.cmake"
  "${PROJECT_BINARY_DIR}/SGALConfigVersion.cmake"
  DESTINATION "${SGAL_INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT SGALTargets DESTINATION
  "${SGAL_INSTALL_CMAKE_DIR}" COMPONENT dev)
