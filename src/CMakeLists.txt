set(kitapixeltools_SOURCES
    PixelToolsPlugin.cpp
    pixel/PixelEllipseTool.cpp
)

kis_add_library(kitapixeltools MODULE ${kitapixeltools_SOURCES})

target_include_directories(kitapixeltools PRIVATE . pixel)
target_compile_features(kitapixeltools PRIVATE cxx_std_17)
target_link_libraries(kitapixeltools PRIVATE kritaui kritaimage)

set_target_properties(kitapixeltools PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN"
)
