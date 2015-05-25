# Add SGAL subdirectory
if (WIN32)
  add_subdirectory(GL)
endif()
add_subdirectory(SGAL)
add_subdirectory(boost)
if (SGAL_USE_SCGAL)
  add_subdirectory(SCGAL)
endif()
