add_library(
        common
        progargs.cpp
        binaryio.hpp
        metadata.cpp
        ../imgsoa/imagesoa.cpp
)

target_link_libraries(common PRIVATE Microsoft.GSL::GSL)
