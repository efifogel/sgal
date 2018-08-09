#Add source files

# dxf

set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/init.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_base_boundary_path.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_base_entity.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_block.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_boundary_path.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_circle_edge.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_ellipse_edge.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_extended_data.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_header_full.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_line_edge.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_parser.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_builder.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_data.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_pattern_data.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_polyline_boundary_path.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_spline_edge.cpp)
set(DXF_OTHER_SOURCE_FILES ${DXF_OTHER_SOURCE_FILES} lib/Dxf_writer.cpp)
set(DXF_OTHER_HEADER_FILES ${DXF_OTHER_HEADER_FILES} include/dxf/Dxf_block.hpp)
set(DXF_OTHER_HEADER_FILES ${DXF_OTHER_HEADER_FILES} include/dxf/Dxf_class.hpp)
set(DXF_OTHER_HEADER_FILES ${DXF_OTHER_HEADER_FILES} include/dxf/Dxf_header.hpp)
set(DXF_OTHER_HEADER_FILES ${DXF_OTHER_HEADER_FILES} include/dxf/Dxf_parser.hpp)
set(DXF_OTHER_HEADER_FILES ${DXF_OTHER_HEADER_FILES} include/dxf/Dxf_table.hpp)
set(DXF_SOURCE_FILES ${DXF_SOURCE_FILES} ${DXF_OTHER_SOURCE_FILES} ${DXF_OTHER_HEADER_FILES})
source_group(src FILES ${DXF_OTHER_SOURCE_FILES} ${DXF_OTHER_HEADER_FILES})

# dxf entities
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_3dface_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_3dsolid_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_acad_proxy_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_arcalignedtext_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_arc_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_attdef_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_attrib_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_body_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_circle_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_dimension_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_ellipse_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_hatch_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_image_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_insert_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_leader_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_line_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_lwpolyline_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_mline_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_mtext_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_ole2frame_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_oleframe_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_point_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_polyline_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_ray_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_region_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_rtext_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_seqend_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_shape_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_solid_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_spline_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_text_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_tolerance_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_trace_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_vertex_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_viewport_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_wipeout_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_xline_entity.cpp)
set(DXF_ENTITIES_SOURCE_FILES ${DXF_ENTITIES_SOURCE_FILES} lib/entities/Dxf_user_entity.cpp)
set(DXF_SOURCE_FILES ${DXF_SOURCE_FILES} ${DXF_ENTITIES_SOURCE_FILES} ${DXF_ENTITIES_HEADER_FILES})
source_group(src\\entities FILES ${DXF_ENTITIES_SOURCE_FILES} ${DXF_ENTITIES_HEADER_FILES})

# dxf objects
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_acad_proxy_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_acdbdictionarywdflt_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_acdbnavisworksmodeldef_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_acdbplaceholder_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_datatable_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_dictionary_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_dictionaryvar_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_dimassoc_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_field_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_geodata_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_group_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_idbuffer_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_imagedef_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_imagedef_reactor_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_layer_filter_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_layer_index_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_layout_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_lightlist_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_material_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_mlinestyle_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_object_ptr_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_plotsettings_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_rastervariables_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_render_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_section_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_sortentstable_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_spatial_filter_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_spatial_index_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_sunstudy_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_tablestyle_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_underlaydefinition_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_vba_project_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_visualstyle_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_wipeoutvariables_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_xrecord_object.cpp)
set(DXF_OBJECTS_SOURCE_FILES ${DXF_OBJECTS_SOURCE_FILES} lib/objects/Dxf_user_object.cpp)
set(DXF_SOURCE_FILES ${DXF_SOURCE_FILES} ${DXF_OBJECTS_SOURCE_FILES} ${DXF_OBJECTS_HEADER_FILES})
source_group(src\\objects FILES ${DXF_OBJECTS_SOURCE_FILES} ${DXF_OBJECTS_HEADER_FILES})

# dxf tables
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_dimstyle_entry.cpp)
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_block_record_entry.cpp)
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_layer_entry.cpp)
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_ltype_entry.cpp)
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_style_entry.cpp)
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_view_entry.cpp)
set(DXF_TABLES_SOURCE_FILES ${DXF_TABLES_SOURCE_FILES} lib/tables/Dxf_vport_entry.cpp)
set(DXF_SOURCE_FILES ${DXF_SOURCE_FILES} ${DXF_TABLES_SOURCE_FILES} ${DXF_TABLES_HEADER_FILES})
source_group(src\\tables FILES ${DXF_TABLES_SOURCE_FILES} ${DXF_TABLES_HEADER_FILES})

# Add all include directories
include_directories(../SGAL/include/)
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
include_directories(include)
include_directories(include/tables)
include_directories(include/objects)
include_directories(include/entities)

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
  add_library(DXF SHARED ${DXF_SOURCE_FILES})
else(BUILD_SHARED_LIBS)
  add_library(DXF STATIC ${DXF_SOURCE_FILES})
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

set(DXF_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/objects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/entities
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tables
  CACHE PATH "DXF source include directories")

set(DXF_PUBLIC_HEADERS "")
foreach (dir ${DXF_INCLUDE_DIRS})
  file(GLOB_RECURSE SGAL_TMP ${dir}/dxf/*.hpp)
  list (APPEND DXF_PUBLIC_HEADERS ${SGAL_TMP})
endforeach(dir)

set_target_properties(DXF PROPERTIES PUBLIC_HEADER "${DXF_PUBLIC_HEADERS}")

install(TARGETS DXF
  EXPORT SGALTargets
  RUNTIME DESTINATION ${SGAL_INSTALL_BIN_DIR} COMPONENT bin
  LIBRARY DESTINATION ${SGAL_INSTALL_LIB_DIR} COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${SGAL_INSTALL_INC_DIR}/dxf" COMPONENT dev
  ARCHIVE DESTINATION ${SGAL_INSTALL_LIB_DIR})
