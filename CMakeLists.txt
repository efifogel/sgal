cmake_minimum_required(VERSION 3.5)
project(SGAL)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()

# Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)
option(SGAL_USE_SCGAL "Use SCGAL library" ON)
option(SGAL_USE_V8 "Use V8 Javascript engine library" ON)
option(SGAL_USE_NGM "Nef Gaussian Map (CGAL)" ON)
option(SGAL_USE_LTS "Lines Through Segments" ON)
option(SGAL_USE_AOS "Arrangement On Surface" ON)
option(SGAL_USE_VOS "Voronoi On Sphere" ON)
option(SGAL_USE_POS "Polycurves On Sphere" ON)
option(SGAL_USE_SGM "Spherical Gaussian Map" ON)
option(SGAL_USE_NEF "Nef" ON)
option(SGAL_USE_CGM "Cubical Gaussian Map" ON)
option(SGAL_USE_GMP "Link with GMP & MPFR" ON)
option(SGAL_USE_SSC "Smallest stabbing cube" ON)
option(SGAL_TRACE "Trace" OFF)
option(SGAL_BUILD_PYBINDINGS "Build the player as a shared library" OFF)

set(SCGAL_NT "3" CACHE STRING "CGAL number type")
set(SCGAL_KERNEL "0" CACHE STRING "CGAL kernel")
set(CGAL_IDENTIFICATION_XY "CGAL_X_MINUS_11_Y_7" CACHE STRING "CGAL identification location")

# SONAME, SOVERSION
#
# The rule is that each new release or bug fix release should increse the soversion.
#
# SOVERSION is $(SONAME_VERSION).$(SOVERSION_MINOR).$(SOVERSION_RELEASE)
#
# - If the binary interface of libraries do not change from previous release
#   (example: most bug fix releases), increase SOVERSION_RELEASE (this third number).
# - If the binary interface is changed, but remains compatible with
#   previous release (example: only addition of new functions), then increase
#   SOVERSION_MINOR (second number) and set SOVERSION_RELEASE to 0.
# - If the binary interface is changed in an incompatible way to previous
#   release, then increase the SONAME_VERSION, and set the two other
#   numbers to 0.
#
# SOVERSION history:
#   SGAL-0.1  : 0.1.0
#
set( SGAL_SONAME_VERSION "0" )
set( SGAL_SOVERSION      "0.1.0" )
message( STATUS "SGAL_SONAME_VERSION=${SGAL_SONAME_VERSION}" )
message( STATUS "SGAL_SOVERSION     =${SGAL_SOVERSION}" )

# General
set(SGAL_MODULES_REL_DIR cmake/modules)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/${SGAL_MODULES_REL_DIR})
include(SGAL_Macros)
include(SGAL_Common)

#Add src subdirectory
add_subdirectory(src)

include (InstallRequiredSystemLibraries)
#set (CPACK_RESOURCE_FILE_LICENSE
#     "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_GENERATOR TGZ)
set (CPACK_PACKAGE_VENDOR "Fogel Inc")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "1")
set (CPACK_PACKAGE_VERSION_PATCH "0")
include (CPack)
