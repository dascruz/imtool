add_library(
        common
        progargs.cpp
        binaryio.hpp
)

target_link_libraries(common PRIVATE Microsoft.GSL::GSL)
