# Add source files
set(PLAYER2_SOURCE_FILES ${PLAYER2_SOURCE_FILES} player.cpp)
set(PLAYER2_SOURCE_FILES ${PLAYER2_SOURCE_FILES} ../player/Player_scene.cpp)
set(PLAYER2_SOURCE_FILES ${PLAYER2_SOURCE_FILES} ../player/Player_option_parser.cpp)
set(PLAYER2_SOURCE_FILES ${PLAYER2_SOURCE_FILES} Player2_scene.cpp)
set(PLAYER2_SOURCE_FILES ${PLAYER2_SOURCE_FILES} Player2_option_parser.cpp)

#Add defines
add_definitions(-DSGAL_ALL_DYN_LINK )
if (SGAL_USE_V8)
  add_definitions(-DSGAL_USE_V8)
endif()

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
add_executable (player2 ${PLAYER2_SOURCE_FILES})
target_link_libraries(player2 SGAL SCGAL)

set_property(TARGET player2 PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS player2 DESTINATION ${SGAL_INSTALL_BIN_DIR})
