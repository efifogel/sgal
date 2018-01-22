# Find required libraries
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem)
if (NOT Boost_FOUND)
  message(STATUS "This library cannot be compiled, as it requires Boost.")
  return()
endif()

find_package(ImageMagick REQUIRED COMPONENTS Magick++)
find_package(OpenGL REQUIRED)

find_package(CGAL REQUIRED)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

# Add includes and libs
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})

if (WIN32)
  include_directories(libs/GL/include)
endif(WIN32)

# Add some compiler options
if (WIN32)
  add_definitions(-GR)
  add_definitions(-vmg)
endif()

# Use C++11 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (SGAL_TRACE)
  add_definitions(-DSGAL_TRACE)
endif()

set (XENIA_INSTALL_INC_DIR "include" CACHE STRING "The folder where XENIA header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set (XENIA_INSTALL_LIB_DIR "lib"     CACHE STRING "The folder where XENIA libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set (XENIA_INSTALL_BIN_DIR "bin"
  CACHE STRING "The folder where XENIA executables, e.g., binaried and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set (XENIA_INSTALL_CMAKE_DIR "${XENIA_INSTALL_LIB_DIR}/XENIA"
  CACHE STRING "The folder where XENIA CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

# Add subdirectories
add_subdirectory(libs)
