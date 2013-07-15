#Check for bison
find_package(BISON 2.7 REQUIRED)
find_package(FLEX 2.5.31 REQUIRED)
find_package(OpenGL REQUIRED)

set (GENERATED_DIR "${CMAKE_BINARY_DIR}/src/libs/SGAL/generated")

# Create custom command for flex/lex (note the outputs)
#add_custom_command(
#	TARGET SGAL
#	PRE_BUILD
#	COMMAND ${CMAKE_COMMAND}
#	ARGS -E make_directory ${GENERATED_DIR})

file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
	TARGET SGAL
	SOURCE lib/loaders/scan.l
	COMMAND ${FLEX_EXECUTABLE}
	ARGS -d -+ --nounistd -o ${GENERATED_DIR}/scan.cpp ${CMAKE_CURRENT_LIST_DIR}/lib/loaders/scan.l
	OUTPUTS ${GENERATED_DIR}/scan.cpp)

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command(
	TARGET SGAL
	SOURCE lib/loaders/parse.y
	COMMAND ${BISON_EXECUTABLE}
	ARGS -y ${CMAKE_CURRENT_LIST_DIR}/lib/loaders/parse.y -o ${GENERATED_DIR}/parse.cpp
    PRE_BUILD
	DEPENDS ${GENERATED_DIR}/scan.cpp
	OUTPUTS ${GENERATED_DIR}/parse.cpp)

# Add parser.c to the list of sources
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} ${GENERATED_DIR}/parse.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} ${GENERATED_DIR}/scan.cpp)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties(${GENERATED_DIR}/parse.cpp GENERATED)
set_source_files_properties(${GENERATED_DIR}/scan.cpp GENERATED)

# Include binary directory to include lexer.c in parser.c
include_directories(${GENERATED_DIR})

#Look for cpp files (Removed for now due to unmaintained files)
#file(GLOB_RECURSE SGAL_SOURCE_FILES *.cpp)

set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/assertions.cpp)

#Engines
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/engines/Sphere_plane_intersection.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/engines/Incrementor.cpp)

#Primitives
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/primitives/Item_primitive.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/primitives/Math_defs.cpp)

#Events
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Draw_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Keyboard_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Mouse_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Motion_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Passive_motion_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Redraw_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Reset_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Reshape_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Simulate_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Start_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Stop_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Tick_event.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Event_handler.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/events/Event.cpp)

#Geometries
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Color_array.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Coord_array.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Coord_transformer.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Coord_minkowski.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Extrusion.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Geo_set.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Geometry.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Indexed_face_set.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Indexed_line_set.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Indexed_geodesic_set.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Indexed_face_set_draw.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Mesh_set.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Normal_array.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Piece.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Tex_coord_array.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Tex_coord_array_2d.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Tex_coord_array_3d.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/Text.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Arc.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Box.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Cone.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Cylinder.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Cylindrical_patch.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Ellipsoid.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Geodesic.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Sphere.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geometries/primitive_shapes/Torus.cpp)

# scene_graph:
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Accumulation.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Appearance.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Background.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Bindable_node.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Bindable_stack.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Camera.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Collision.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Color_background.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Color_map.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Configuration.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Container.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Container_factory.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Container_proto.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Context.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Composed_shader.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Cube_environment.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Cull_context.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Draw_action.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Environment_background.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Field.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Font.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Font_style.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Fragment_shader.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Frustum.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Gfx_conf.cpp)

#TODO Figure this out
#ifeq ($(USE_GLUT), 1)
#set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Glut_font.cpp
#endif

set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Gl_wrapper.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Group.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Halftone.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Image.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Image_background.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Image_base.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Image_sphere_environment.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Image_texture.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Imagemagick_font.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/initialize.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Loader.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Isect_action.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Material.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Multisample.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Navigation_info.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Node.cpp)
# set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Record_action.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Route.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Scene_graph.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Simulation.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Shader.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Shape.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Snapshot.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Sphere_bound.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Sphere_environment.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Switch.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Tex_gen.cpp)
# set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Text_texture.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Texture.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Texture_2d.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Texture_font.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Texture_transform.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Trace.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Transform.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/Vertex_shader.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/World_info.cpp)

#Sensors
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Drag_sensor.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Key_sensor.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Navigation_sensor.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Proximity_sensor.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Single_key_sensor.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Time_sensor.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/sensors/Touch_sensor.cpp)

#Light
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/lights/Light.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/lights/Point_light.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/lights/Spot_light.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/lights/Directional_light.cpp)

#Geo
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Line.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Matrix3f.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Matrix4f.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Plane.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Vector2f.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Vector2u.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Vector2sh.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Vector3f.cpp)
# set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Vector3sh.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Vector4f.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/geo/Rotation.cpp)

#Loaders
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/loaders/Utilities.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/loaders/Element.cpp)

#Utilities
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Generic_option_parser.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Bench_option_parser.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Conf_option_parser.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Option_parser.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Native_window_item.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Window_option_parser.cpp)

if (WIN32)
	set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Windows_window_item.cpp)
	set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/Windows_window_manager.cpp)
else()
	set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/X11_window_item.cpp)
	set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/utilities/X11_window_manager.cpp)
endif()

#Interpolators
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Interpolator.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Rotation_interpolator.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Position_interpolator.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Coordinate_interpolator.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Vector3f_interpolator.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Scalar_interpolator.cpp)
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/interpolators/Color_interpolator.cpp)

#IO
set(SGAL_SOURCE_FILES ${SGAL_SOURCE_FILES} lib/IO/Vrml_formatter.cpp)

#Add all include directories
include_directories(include/)
include_directories(include/SGAL)
include_directories(include/engines)
include_directories(include/events)
include_directories(include/geo)
include_directories(include/geometries)
include_directories(include/geometries/primitive_shapes)
include_directories(include/interpolators)
include_directories(include/IO)
include_directories(include/lights)
include_directories(include/loaders)
include_directories(include/primitives)
include_directories(include/sensors)
include_directories(include/utilities)
include_directories(lib)
include_directories(lib/loaders)
include_directories(lib/primitives)
include_directories(lib/interpolators)

#Add defines
add_definitions(-DSGAL_SGAL_SOURCE)
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

#Add library
if (BUILD_SHARED_LIBS)
	add_definitions(-DSGAL_ALL_DYN_LINK)
	add_library(SGAL SHARED ${SGAL_SOURCE_FILES})
else()
	add_library(SGAL STATIC ${SGAL_SOURCE_FILES})
endif()

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

target_link_libraries(SGAL
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${ImageMagick_Magick++_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${NON_WIN32_LIBRARIES}
)

# Installation
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION "include"
    FILES_MATCHING
    PATTERN "*.hpp"
#   PATTERN ".git" EXCLUDE
#   PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
install(TARGETS SGAL DESTINATION "lib")
