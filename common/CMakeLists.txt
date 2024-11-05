add_library(
        common
        progargs.cpp
        binaryio.cpp
        metadata.cpp
        ../imgsoa/imagesoa.cpp
        ../imgsoa/main.cpp
)

target_link_libraries(common PRIVATE Microsoft.GSL::GSL)
