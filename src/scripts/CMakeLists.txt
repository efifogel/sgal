set(SGAL_GEN_NODE
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_node.py
  CACHE PATH "SGAL container-code generator script")
set(SGAL_SCRIPT_FILES ${SGAL_SCRIPT_FILES} gen_node.py)
install(PROGRAMS ${SGAL_SCRIPT_FILES}
        DESTINATION ${SGAL_INSTALL_BIN_DIR})
