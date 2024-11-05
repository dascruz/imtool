add_library(imgsoa imagesoa.cpp)
target_link_libraries(imgsoa PRIVATE common Microsoft.GSL::GSL)
