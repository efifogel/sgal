# Add SGAL subdirectory
if (WIN32)
  add_subdirectory(boost)
  add_subdirectory(GL)
endif()
add_subdirectory(SGAL)
if (USE_CGAL)
  add_subdirectory(SCGAL)
endif()
