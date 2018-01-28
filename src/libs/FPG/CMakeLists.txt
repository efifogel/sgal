#Set some values...

# packages
find_package(SGAL REQUIRED)

# var - should be somehow taken from the SCGAL compilation.
set(SCGAL_KERNEL "0" CACHE STRING "CGAL Kernel")
set(SCGAL_NT "3" CACHE STRING "CGAL number type")
# defs
add_definitions(-DSCGAL_NT=${SCGAL_NT})
add_definitions(-DSCGAL_KERNEL=${SCGAL_KERNEL})

# Create custom commands for nodes
set(GENERATED_BASE_DIR "${CMAKE_BINARY_DIR}/src/libs/FPG")
set(GEN_NODE "${CMAKE_SOURCE_DIR}/src/scripts/gen_node.py")
set(SPECS_DIR ${CMAKE_CURRENT_LIST_DIR}/specs)
set(GENERATED_LIB_DIR "${GENERATED_BASE_DIR}/lib/generated")
set(GENERATED_INC_DIR "${GENERATED_BASE_DIR}/include/generated")
file(MAKE_DIRECTORY ${GENERATED_LIB_DIR})
file(MAKE_DIRECTORY ${GENERATED_INC_DIR}/FPG)

########
function(add_node name)
  set(GEN_NODE_OUTPUT
      ${GENERATED_INC_DIR}/FPG/${name}.hpp
      ${GENERATED_LIB_DIR}/${name}.cpp)
  set(GEN_NODE_ARGS --name ${name}
    --input-path ${SPECS_DIR}
    --output-inc-path ${GENERATED_INC_DIR}/FPG
    --output-lib-path ${GENERATED_LIB_DIR})
  add_custom_command(OUTPUT ${GEN_NODE_OUTPUT}
                     COMMAND ${GEN_NODE}
	             ARGS ${GEN_NODE_ARGS}
	             DEPENDS ${SPECS_DIR}/${name}_spec.txt ${GEN_NODE})
  set(FPG_AUTO_GENERATED_HDR_FILES ${FPG_AUTO_GENERATED_HDR_FILES} ${GENERATED_INC_DIR}/FPG/${name}.hpp CACHE INTERNAL "")
  set(FPG_AUTO_GENERATED_SRC_FILES ${FPG_AUTO_GENERATED_SRC_FILES} ${GENERATED_LIB_DIR}/${name}.cpp CACHE INTERNAL "")

endfunction()
########

set(FPG_AUTO_GENERATED_SRC_FILES "" CACHE INTERNAL "")
add_node(Cabinet)
add_node(Cash_register)
add_node(Column)
add_node(Counter)
add_node(Counter_area)
add_node(Counter_deck)
# add_node(Enterance)
add_node(Fitting_room)
add_node(Fitting_room_lobby)
# add_node(Room)
add_node(Storage_room)
add_node(Store)
# add_node(Store_front)
add_node(Wall)

# Add source files
set(FPG_SRC_FILES ${FPG_SRC_FILES} lib/init.cpp)

# Engines
set(FPG_SRC_FILES ${FPG_SRC_FILES} lib/engines/Floor_plan_evaluator.cpp)

# Add the generated files
set(FPG_SRC_FILES ${FPG_SRC_FILES} ${FPG_AUTO_GENERATED_SRC_FILES})

# Add all include directories
include_directories(include)
include_directories(include/engines)
include_directories(${GENERATED_INC_DIR})
include_directories(${SCGAL_INCLUDE_DIR})
include_directories(${SGAL_INCLUDE_DIR})

# Add defines
add_definitions(-DSGAL_FPG_SOURCE)
add_definitions(-DGL2PSDLL)
add_definitions(-DSGAL_LIB)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

# Add library
if (BUILD_SHARED_LIBS)
  add_definitions(-DSGAL_ALL_DYN_LINK)
  add_library(FPG SHARED ${FPG_SRC_FILES})
else(BUILD_SHARED_LIBS)
  add_library(FPG STATIC ${FPG_SRC_FILES})
endif(BUILD_SHARED_LIBS)

set_target_properties(FPG PROPERTIES
  VERSION "${FPG_SOVERSION}"
  SOVERSION "${FPG_SONAME_VERSION}"
)

# Add links
# if (NOT WIN32)
#   set (NON_WIN32_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
#   if (APPLE)
#     set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
#   endif(APPLE)
# endif(NOT WIN32)
target_link_libraries(FPG
                      ${SCGAL_LIBRARY}
		      ${SGAL_LIBRARY}
		      ${ImageMagick_Magick++_LIBRARY}
		      ${OPENGL_LIBRARIES}
		      ${NON_WIN32_LIBRARIES})

# Installation
set_property(TARGET FPG PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
set_property(TARGET FPG PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

install(TARGETS FPG
        RUNTIME DESTINATION ${XENIA_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${XENIA_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${XENIA_INSTALL_LIB_DIR})
