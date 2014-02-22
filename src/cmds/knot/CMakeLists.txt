find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

# Add source files
set(KNOT_SOURCE_FILES ${KNOT_SOURCE_FILES} knot.cpp)
set(KNOT_SOURCE_FILES ${KNOT_SOURCE_FILES} Knot_scene.cpp)
set(KNOT_SOURCE_FILES ${KNOT_SOURCE_FILES} Knot_option_parser.cpp)

#Add defines
add_definitions(-DBOOST_ALL_DYN_LINK )

#Add include dirs
include_directories(../player)
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
install(TARGETS knot DESTINATION ${SGAL_INSTALL_BIN_DIR})
