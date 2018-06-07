#Add source files

set(SGAL_DXF_SOURCE_FILES ${SGAL_DXF_SOURCE_FILES} lib/init.cpp)

# Add all include directories
include_directories(include/)
include_directories(../SGAL/include/)
include_directories(../SGAL/include/SGAL)
include_directories(../SGAL/include/engines)
include_directories(../SGAL/include/events)
include_directories(../SGAL/include/geo)
include_directories(../SGAL/include/geometries)
include_directories(../SGAL/include/geometries/visitors)
include_directories(../SGAL/include/geometries/primitive_shapes)
include_directories(../SGAL/include/interpolators)
include_directories(../SGAL/include/IO)
include_directories(../SGAL/include/lights)
include_directories(../SGAL/include/loaders)
include_directories(../SGAL/include/primitives)
include_directories(../SGAL/include/sensors)
include_directories(../SGAL/include/utilities)

# Add defines
add_definitions(-DSGAL_DXF_SOURCE)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

# Add library
if (BUILD_SHARED_LIBS)
  add_definitions(-DSGAL_ALL_DYN_LINK)
  add_library(DXF SHARED ${SGAL_DXF_SOURCE_FILES})
else(BUILD_SHARED_LIBS)
  add_library(DXF STATIC ${SGAL_DXF_SOURCE_FILES})
endif(BUILD_SHARED_LIBS)

set_target_properties(DXF PROPERTIES
  VERSION "${SGAL_SOVERSION}"
  SOVERSION "${SGAL_SONAME_VERSION}"
)

# Add links
if (NOT WIN32)
  set (NON_WIN32_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
  if (APPLE)
    set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
  endif(APPLE)
endif(NOT WIN32)

set_target_properties(DXF PROPERTIES OUTPUT_NAME SGAL_dxf)

target_link_libraries(DXF SGAL ${NON_WIN32_LIBRARIES})

# Installation
# set_property(TARGET DXF PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
# set_property(TARGET DXF PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

set(SGAL_DXF_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  CACHE PATH "DXF source include directories")

set(SGAL_DXF_PUBLIC_HEADERS "")
foreach (dir ${SGAL_DXF_INCLUDE_DIRS})
  file(GLOB_RECURSE SGAL_TMP ${dir}/dxf/*.hpp)
  list (APPEND SGAL_DXF_PUBLIC_HEADERS ${SGAL_TMP})
endforeach(dir)

set_target_properties(DXF PROPERTIES PUBLIC_HEADER "${SGAL_DXF_PUBLIC_HEADERS}")

install(TARGETS DXF
  EXPORT SGALTargets
  RUNTIME DESTINATION ${SGAL_INSTALL_BIN_DIR} COMPONENT bin
  LIBRARY DESTINATION ${SGAL_INSTALL_LIB_DIR} COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${SGAL_INSTALL_INC_DIR}/dxf" COMPONENT dev
  ARCHIVE DESTINATION ${SGAL_INSTALL_LIB_DIR})
