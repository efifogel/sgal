# Add source files
set(PLAYER_SOURCE_FILES ${PLAYER_SOURCE_FILES} player.cpp)
set(PLAYER_SOURCE_FILES ${PLAYER_SOURCE_FILES} Player_scene.cpp)
set(PLAYER_SOURCE_FILES ${PLAYER_SOURCE_FILES} Player_option_parser.cpp)

#Add defines
if (NOT WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif (NOT WIN32)
if (SGAL_USE_SCGAL)
  add_definitions(-DSGAL_USE_SCGAL)
  add_definitions(-DSCGAL_NT=${SCGAL_NT})
  add_definitions(-DSCGAL_KERNEL=${SCGAL_KERNEL})
  add_definitions(-DCGAL_IDENTIFICATION_XY=${CGAL_IDENTIFICATION_XY})
endif()

add_definitions(-DSGAL_ALL_DYN_LINK)
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
include_directories(../../libs/SGAL/include/geometries/visitors)
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

#Add our executable
if (SGAL_BUILD_PYBINDINGS)
  find_package(PythonLibs REQUIRED)
  find_package(Boost REQUIRED COMPONENTS python)
  include_directories(${PYTHON_INCLUDE_DIRS})
  add_definitions(-DSGAL_BUILD_PYBINDINGS)
  add_library(player SHARED ${PLAYER_SOURCE_FILES} player_pybindings.cpp)
  target_link_libraries(player ${Boost_LIBRARIES})

  # don't prepend wrapper library name with lib
  set_target_properties(player PROPERTIES PREFIX "")
  target_link_libraries(player SGAL ${CMAKE_DL_LIBS})
endif()

add_executable(player-bin ${PLAYER_SOURCE_FILES})
set_target_properties(player-bin PROPERTIES OUTPUT_NAME player)

target_link_libraries(player-bin SGAL ${CMAKE_DL_LIBS})
if (SGAL_USE_SCGAL)
  if (SGAL_BUILD_PYBINDINGS)
    target_link_libraries(player SCGAL)
  endif()
  target_link_libraries(player-bin SCGAL)
endif()

# Don't skip the full RPATH for the build tree
# set_property(TARGET player PROPERTY SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
# set_property(TARGET player PROPERTY BUILD_WITH_INSTALL_RPATH TRUE)

# the RPATH to be used when installing
if (SGAL_USE_V8)
  if (SGAL_BUILD_PYBINDINGS)
    set_property(TARGET player PROPERTY INSTALL_RPATH "${V8_LIBRARY_DIR}")
  endif()
  set_property(TARGET player-bin PROPERTY INSTALL_RPATH "${V8_LIBRARY_DIR}")
endif()

# don't add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
if (SGAL_BUILD_PYBINDINGS)
  set_property(TARGET player PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
  set_property(TARGET player PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
set_property(TARGET player-bin PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
set_property(TARGET player-bin PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

if (SGAL_BUILD_PYBINDINGS)
  install(TARGETS player
          RUNTIME DESTINATION ${SGAL_INSTALL_BIN_DIR}
	  LIBRARY DESTINATION ${SGAL_INSTALL_LIB_DIR}
	  ARCHIVE DESTINATION ${SGAL_INSTALL_LIB_DIR})
endif()

install(TARGETS player-bin
        RUNTIME DESTINATION ${SGAL_INSTALL_BIN_DIR}
	LIBRARY DESTINATION ${SGAL_INSTALL_LIB_DIR}
	ARCHIVE DESTINATION ${SGAL_INSTALL_LIB_DIR})
