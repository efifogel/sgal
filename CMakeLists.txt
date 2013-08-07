cmake_minimum_required(VERSION 2.6)
project(SGAL)

#Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)
option(USE_CGAL "Use CGAL related stuff" ON)
set(SCGAL_NT "3" CACHE STRING "CGAL number type")
set(SCGAL_KERNEL "0" CACHE STRING "CGAL kernel")

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
