# Add source files
set(KNOT_SOURCE_FILES ${KNOT_SOURCE_FILES} knot.cpp)
set(KNOT_SOURCE_FILES ${KNOT_SOURCE_FILES} Knot_scene.cpp)
set(KNOT_SOURCE_FILES ${KNOT_SOURCE_FILES} Knot_option_parser.cpp)

#Add defines
if (NOT WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif (NOT WIN32)

#Add include dirs
include_directories(../../libs/SGAL/include)
include_directories(../../libs/SGAL/include/engines)
include_directories(../../libs/SGAL/include/events)
include_directories(../../libs/SGAL/include/geo)
include_directories(../../libs/SGAL/include/geometries)
include_directories(../../libs/SGAL/include/geometries/primitive_shapes)
include_directories(../../libs/SGAL/include/interpolators)
include_directories(../../libs/SGAL/include/IO)
include_directories(../../libs/SGAL/include/lights)
include_directories(../../libs/SGAL/include/loaders)
include_directories(../../libs/SGAL/include/primitives)
include_directories(../../libs/SGAL/include/sensors)
include_directories(../../libs/SGAL/include/utilities)
include_directories(../../libs/SCGAL/include)
include_directories(../../libs/SCGAL/include/engines)
include_directories(../../libs/SCGAL/include/geo)
include_directories(../../libs/SCGAL/include/geometries)
include_directories(../../libs/SCGAL/include/primitives)

#Add our executable (disabled for now...)
add_executable(knot ${KNOT_SOURCE_FILES})
target_link_libraries(knot SGAL)

# Don't skip the full RPATH for the build tree
# set_property(TARGET player PROPERTY SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
# set_property(TARGET player PROPERTY BUILD_WITH_INSTALL_RPATH TRUE)

# the RPATH to be used when installing
set_property(TARGET knot PROPERTY INSTALL_RPATH "${V8_LIBRARY_DIR}")

# don't add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set_property(TARGET knot PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS knot DESTINATION ${SGAL_INSTALL_BIN_DIR})
