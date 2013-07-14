# Add source files
set(PLAYER_SOURCE_FILES ${PLAYER_SOURCE_FILES} player.cpp)
set(PLAYER_SOURCE_FILES ${PLAYER_SOURCE_FILES} Player_scene.cpp)
set(PLAYER_SOURCE_FILES ${PLAYER_SOURCE_FILES} Player_option_parser.cpp)

#Add defines
if (USE_CGAL)
	add_definitions(-DUSE_CGAL)
	add_definitions(-DSCGAL_NT=${SCGAL_NT})
	add_definitions(-DSCGAL_KERNEL=${SCGAL_KERNEL})
	add_definitions(-DCGAL_CGM_TRAITS=${CGAL_CGM_TRAITS})
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
add_executable (player ${PLAYER_SOURCE_FILES}) 

target_link_libraries(player SGAL)

if (USE_CGAL)
	target_link_libraries(player SCGAL)
endif()
install(TARGETS player DESTINATION "bin")
