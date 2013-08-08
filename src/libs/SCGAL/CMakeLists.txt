find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

#Set some values...

set(SCGAL_AOS_BASE 0)
if (USE_AOS)
  set(SCGAL_AOS_BASE 1)
endif()

if (USE_VOS)
  set(SCGAL_AOS_BASE 1)
endif()

if (USE_SGM)
  set(SCGAL_AOS_BASE 1)
endif()

set(SCGAL_ARRANGEMENT_ON_SURFACE 0)
if (SCGAL_AOS_BASE)
  set(SCGAL_ARRANGEMENT_ON_SURFACE 1)
endif()
if (USE_QUADRIC)
  set(SCGAL_ARRANGEMENT_ON_SURFACE 1)
endif()

#Add source files
if (SCGAL_ARRANGEMENT_ON_SURFACE)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_surface_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_renderer.cpp)
endif()

if (SCGAL_AOS_BASE)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_base_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_renderers.cpp)
endif()

if (USE_AOS)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_graph_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_marked_geo.cpp)
  # Ignore in Debug mode, cause is_valid() on the sphere does not compile
  #ifneq ($(DEBUG), 1)
  #  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Polygon_set_on_sphere_geo.cpp
  #endif
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_overlay_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_sphere_sim_geo.cpp)
endif()

if (USE_VOS)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Geodesic_voronoi_on_sphere_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Voronoi_diagram_on_sphere_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Power_diagram_on_sphere_geo.cpp)
endif()

if (USE_QUADRIC)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Vector3d.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Quadric.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Quadric_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_quadric_geo.cpp)
endif(USE_QUADRIC)

set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/engines/Boolean_operation.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/init.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Polyhedron_geo.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_coord_array.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_coord2_array.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_coord_minkowski.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_normal_array.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_plane_array.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_polyhedron_geo.cpp)
set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Triangulation_geo.cpp)

if (USE_ENV)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Lower_envelope_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Lower_envelope_tri_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Lower_envelope_sphere_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Lower_envelope_plane_geo.cpp)
endif(USE_ENV)

if (USE_CGM)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Cubical_gaussian_map_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Arrangement_on_cube_renderers.cpp)
endif(USE_CGM)

if (USE_SGM)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Spherical_gaussian_map_base_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Spherical_gaussian_map_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Spherical_gaussian_map_colored_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Spherical_gaussian_map_marked_geo.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Assembly.cpp)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Assembly_part.cpp)
endif()

if (USE_NEF)
  set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Exact_nef_polyhedron.cpp)
  if (USE_NGM)
    set(SCGAL_SOURCE_FILES ${SCGAL_SOURCE_FILES} lib/geometries/Nef_gaussian_map_geo.cpp)
  endif(USE_NGM)
endif(USE_NEF)

#Add all include directories
include_directories(include/)
include_directories(include/engines)
include_directories(include/geo)
include_directories(include/geometries)
include_directories(include/primitives)
include_directories(../SGAL/include/)
include_directories(../SGAL/include/SGAL)
include_directories(../SGAL/include/engines)
include_directories(../SGAL/include/events)
include_directories(../SGAL/include/geo)
include_directories(../SGAL/include/geometries)
include_directories(../SGAL/include/geometries/primitive_shapes)
include_directories(../SGAL/include/interpolators)
include_directories(../SGAL/include/IO)
include_directories(../SGAL/include/lights)
include_directories(../SGAL/include/loaders)
include_directories(../SGAL/include/primitives)
include_directories(../SGAL/include/sensors)
include_directories(../SGAL/include/utilities)

#Add defines
add_definitions(-DSGAL_SCGAL_SOURCE)
add_definitions(-DGL2PSDLL)
add_definitions(-DSGAL_LIB)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

#Some hard coded defines - will fix those later
add_definitions(-DSCGAL_NT=${SCGAL_NT})
add_definitions(-DSCGAL_KERNEL=${SCGAL_KERNEL})

#Add library
if (BUILD_SHARED_LIBS)
  add_definitions(-DSGAL_ALL_DYN_LINK)
  add_library(SCGAL SHARED ${SCGAL_SOURCE_FILES})
else(BUILD_SHARED_LIBS)
  add_library(SCGAL STATIC ${SCGAL_SOURCE_FILES})
endif(BUILD_SHARED_LIBS)


set_target_properties(SCGAL PROPERTIES
  VERSION "${SGAL_SOVERSION}"
  SOVERSION "${SGAL_SONAME_VERSION}"
)

#Add links
if (NOT WIN32)
  set (NON_WIN32_LIBRARIES
    ${X11_LIBRARIES}
    ${X11_Xxf86vm_LIB}
  )
  if (APPLE)
    set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
  endif(APPLE)
endif(NOT WIN32)

target_link_libraries(SCGAL
  SGAL
  ${CGAL_LIBRARY}
  ${NON_WIN32_LIBRARIES}
)

# Installation
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION "include"
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN ".git" EXCLUDE
  # PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
install(TARGETS SCGAL DESTINATION "lib")
