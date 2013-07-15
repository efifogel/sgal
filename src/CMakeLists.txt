#Find required libraries
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
if (NOT WIN32)
  find_package(X11 REQUIRED)
endif(NOT WIN32)

if (USE_CGAL)
	find_package(CGAL REQUIRED)
	include(${CGAL_USE_FILE})
endif()

#Add includes and libs
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ImageMagick_Magick++_INCLUDE_DIR})
if (NOT WIN32)
  include_directories(${X11_INCLUDE_DIR})
endif(NOT WIN32)
# TODO do we really need all of them? on all platforms?
include_directories(libs/GL/include)
include_directories(libs/flex/include)
include_directories(libs/gl2ps/include)
include_directories(libs/boost/include)

#Add some compiler options
if (WIN32)
	add_definitions(-GR)
	add_definitions(-vmg)
endif()

add_definitions(-DBOOST_ALL_DYN_LINK)

#Add subdirectories
add_subdirectory(libs)
add_subdirectory(cmds)
