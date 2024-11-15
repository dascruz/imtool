add_library(imgaos imageaos.cpp resize2.cpp compress.cpp)
target_link_libraries(imgaos PRIVATE common)
