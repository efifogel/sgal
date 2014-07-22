# Find required libraries
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
find_package(OpenGL REQUIRED)
if (USE_V8)
  find_package(V8 REQUIRED)
  link_directories(${V8_LIBRARY_DIR})
endif()

if (NOT WIN32)
  find_package(X11 REQUIRED)
endif(NOT WIN32)

if (USE_CGAL)
  find_package(CGAL REQUIRED COMPONENTS Core)
  include(${CGAL_USE_FILE})
endif()

# Add includes and libs
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ImageMagick_Magick++_INCLUDE_DIR})
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

if (CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++0x)
endif()

add_definitions(-DBOOST_ALL_DYN_LINK)

# Installation
if (SGAL_WIN32_CMAKE_ON_CYGWIN)
  exec_program(cygpath ARGS -w "${CMAKE_INSTALL_PREFIX}"
               OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX2)
  file(TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX2} CMAKE_INSTALL_PREFIX)
endif()

set (SGAL_INSTALL_INC_DIR "include" CACHE STRING "The folder where SGAL header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set (SGAL_INSTALL_LIB_DIR "lib"     CACHE STRING "The folder where SGAL libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set (SGAL_INSTALL_BIN_DIR "bin"
  CACHE STRING "The folder where SGAL executables, e.g., binaried and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set (SGAL_INSTALL_CMAKE_DIR "${SGAL_INSTALL_LIB_DIR}/SGAL"
  CACHE STRING "The folder where SGAL CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

# Add subdirectories
add_subdirectory(libs)
add_subdirectory(cmds)
