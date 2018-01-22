project(XENIA)
cmake_minimum_required(VERSION 2.6)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()

# Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)

# General
set(XENIA_MODULES_REL_DIR cmake/modules)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/${XENIA_MODULES_REL_DIR})

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
#   XENIA-0.1  : 0.1.0
#
set( XENIA_SONAME_VERSION "0" )
set( XENIA_SOVERSION      "0.1.0" )
message( STATUS "XENIA_SONAME_VERSION=${XENIA_SONAME_VERSION}" )
message( STATUS "XENIA_SOVERSION     =${XENIA_SOVERSION}" )

# Add src subdirectory
add_subdirectory(src)

include (InstallRequiredSystemLibraries)

set (CPACK_GENERATOR TGZ)
set (CPACK_PACKAGE_VENDOR "Xenia Optimal Ltd")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "1")
set (CPACK_PACKAGE_VERSION_PATCH "0")
include (CPack)
