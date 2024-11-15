add_library(imgaos imageaos.cpp resize2.cpp cutfreq.cpp compress.cpp info.cpp maxlevel.cpp)
target_link_libraries(imgaos PRIVATE common)
