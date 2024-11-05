add_library(imgaos imageaos.cpp)
target_link_libraries(imgaos PRIVATE common Microsoft.GSL::GSL)
