#Set some values...

# packages
find_package(SCGAL REQUIRED)
find_package(SGAL REQUIRED)

# var - should be somehow taken from the SCGAL compilation.
set(SCGAL_KERNEL "0" CACHE STRING "CGAL Kernel")
set(SCGAL_NT "3" CACHE STRING "CGAL number type")
# defs
add_definitions(-DSCGAL_NT=${SCGAL_NT})
add_definitions(-DSCGAL_KERNEL=${SCGAL_KERNEL})

# Add source files
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Store.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Wall.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Store_front.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/init.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Enterance.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Counter.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Room.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Fitting_room.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Storage_room.cpp)
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/Fitting_room_lobby.cpp)

# Engines
set(FPG_SOURCE_FILES ${FPG_SOURCE_FILES} lib/engines/Floor_plan_evaluator.cpp)

# Add all include directories
include_directories(include)
include_directories(include/engines)
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

if (APPLE)
  add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
  add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
endif(APPLE)

# Add library
if (BUILD_SHARED_LIBS)
  add_definitions(-DSGAL_ALL_DYN_LINK)
  add_library(FPG SHARED ${FPG_SOURCE_FILES})
else(BUILD_SHARED_LIBS)
  add_library(FPG STATIC ${FPG_SOURCE_FILES})
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
