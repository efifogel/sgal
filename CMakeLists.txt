cmake_minimum_required(VERSION 2.6)
project(SGAL)

#Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)
option(USE_CGAL "Use CGAL related stuff" ON)
set(SCGAL_NT "3" CACHE STRING "CGAL number type")
set(CGAL_CGM_TRAITS "0" CACHE STRING "CGAL traits")
set(SCGAL_KERNEL "0" CACHE STRING "CGAL kernel")

#Add src subdirectory
add_subdirectory(src)
