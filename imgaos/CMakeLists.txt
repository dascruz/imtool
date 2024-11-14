add_library(imgaos imageaos.cpp
        maxlevel.cpp
        info.cpp)
target_link_libraries(imgaos PRIVATE common)
