# Must be checked out at krita/plugins/krita-pixel-tools in the Krita tree.

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(src)

# Install as an importable Python-plugin-style package:
# pixeltools.desktop + pixel_tools/{extension.py, *.action, lib/*.dll}.
set(install_dir ${CMAKE_INSTALL_PREFIX}/krita-pixel-tools)

install(
  TARGETS kitapixeltools
  LIBRARY DESTINATION ${install_dir}/pixel_tools/lib
  RUNTIME DESTINATION ${install_dir}/pixel_tools/lib
)
install(DIRECTORY python/ DESTINATION ${install_dir}/pixel_tools)
install(FILES src/pixel_tools.action DESTINATION ${install_dir}/pixel_tools)
install(FILES pixeltools.desktop DESTINATION ${install_dir})
