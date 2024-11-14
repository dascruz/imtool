add_library(imgsoa imagesoa.cpp
        info.cpp
        maxlevel.cpp)
target_link_libraries(imgsoa PRIVATE common)
