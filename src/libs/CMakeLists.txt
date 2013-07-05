#Add SGAL subdirectory
add_subdirectory(SGAL)
if (USE_CGAL)
	add_subdirectory(SCGAL)
endif()